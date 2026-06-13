# Agave 轻量浏览器引擎 — 分叉路线规划

> 日期：2026-06-04
> 基于 commit: 21fe09f8
> 状态：战略规划（未开工）

---

## 〇、战略定位（先读这一节）

经客观评估，Agave 在标准完整度上落后现代 WebKit 约 15 年，且其纯软件渲染架构
与现代 Web 的 GPU 合成/动画模型根本对立。**追赶现代 WebKit 既不现实也无市场**
（WPE WebKit / Servo 已占据中高端可嵌入引擎市场）。

因此战略调整为：

> **与现代 WebKit 彻底分叉，做一个定位清晰的"小巧精致"引擎。
> 只在现有架构上补可兼容的特性，不补与架构冲突的特性。
> 目标场景：无 GPU 的中端嵌入式设备，渲染可信/半可信内容。**

这不是"残缺的浏览器"，而是"为特定生态位精准裁剪的引擎"。

### 生存生态位（唯一现实的市场）

- 无 GPU、纯软件渲染的中端 SoC/MCU（几十 MB RAM，有屏）
- 渲染**可信/半可信内容**：设备厂商自有页面、受控的服务端内容、LLM 结构化输出
- 对自主可控 / 无授权费 / 深度可裁剪有需求的场景

**明确不做**：通用上网浏览器、渲染任意第三方网站、与 Chromium/WPE 拼标准完整度。

---

## 一、能力边界：做什么 / 放弃什么

特性按"能否在现有架构上补"分为两类。判据：是否只是新增
RenderObject 子类 / CSS 属性 / 独立模块（可补），还是需要改渲染架构本身（放弃）。

### ✅ 要做（类别 A：可在现有架构上补）

| 特性 | 实现方式 | 估算代码量 | 优先级 |
|------|---------|-----------|--------|
| 现代 Flexbox | 重写 RenderFlexibleBox::layout() + flex CSS 属性 | ~4-6k | P0 |
| CSS Grid | 新 RenderGrid : RenderBlock + grid 属性解析 | ~8-12k | P0 |
| CSS3 视觉补全（渐变/多背景/filter/更多 transform） | 加 CSS 属性 + 现有 paint() 内绘制 | ~3-5k | P1 |
| 现代 CSS 选择器（:nth-child 等补全） | 扩 CSSStyleSelector | ~1-2k | P1 |
| HTML5 表单控件补全 | 新 Render 子类 | ~3-5k | P2 |
| localStorage | 新 Storage 模块 + JS 绑定 | ~3-4k | P2 |
| Fetch / WebSocket | 新网络模块 + JS 绑定（网络层独立，不碰渲染） | ~5-8k | P2 |

**类别 A 合计 ~30-40k 行，约 1-1.5 人年。**

### ❌ 放弃（类别 B：本质是架构，与软渲染冲突）

| 特性 | 放弃理由 |
|------|---------|
| CSS Animations / Transitions | 需时间驱动 style 重算 + 帧调度 + 合成层；无 GPU 下每帧全量重绘，性能不可接受。是架构非特性 |
| GPU 合成层（RenderLayerCompositor） | 与纯软件渲染根本对立，补它=换渲染后端=重做引擎 |
| `<video>` / `<audio>` | 需解码管线 + 媒体时钟 + 合成，是独立大子系统，且依赖外部编解码器 |
| 现代 SVG | 现 ksvg2 是独立老架构，现代 SVG 要重写整个 SVG 渲染树（~20-30k 行） |
| WebGL / WebGPU / Canvas 加速 | 依赖 GPU |
| Service Worker / WebRTC / 等重型 Web 平台 API | 超出生态位需求，维护成本高 |

**放弃这些不是缺陷，而是定位选择**——目标场景（可信内容、无 GPU）本就不需要它们。

### 灰色地带（按需评估，默认不做）

- CSS Transitions 的**非动画降级**：可考虑直接跳到终态（无过渡动画），低成本满足"页面不报错"。仅当目标内容大量依赖时再做。
- 简单 SVG 图标渲染：若 ksvg2 现状够用则保留，不升级。

---

## 二、实施路线（分阶段）

### 阶段 1：现代 CSS 布局（P0，最高 ROI）

**目标**：解决"现代页面布局错乱"这个最大兼容性痛点。

1. **现代 Flexbox**（~7-8k 行，详细评估见下）
   - 在现有 RenderBlock/RenderStyle 框架上**重新实现** flex 布局算法（参考 CSS Flexbox 规范，
     写适配 Agave 老框架的代码，**不移植现代 WebKit 代码**——避免依赖级联）
   - 加 flex 相关 CSS 属性到 CSSPropertyNames（现 211 个属性）
   - 改动点：rendering/RenderFlexibleBox.cpp、css/CSSStyleSelector.cpp、rendering/style/RenderStyle.h
   - **作为分叉路线的试金石**：验证"在老架构上补现代布局"是否走得通

#### Flexbox 详细工作量评估（第一里程碑）

**现状基线（已查证 commit 21fe09f8）**：
- RenderFlexibleBox 现有 **1148 行老式 `-webkit-box`**（2009 草案）：
  layoutHorizontalBox 342 行 + layoutVerticalBox 409 行 + flex 分配
- 老式 CSS 属性齐全（box-orient/flex/align/pack/direction）
- **现代 flex 属性全部缺失**（flex-grow/shrink/basis、justify-content、
  align-items/self/content、flex-wrap、flex-direction、order）
- 关键结论：老式 box 与现代 flex 是**算法模型本质不同**（老式=单行+整数权重+无wrap；
  现代=grow/shrink/basis三元+多行wrap+双轴对齐+order）。**属算法重写，非改造**，
  但可复用老式 box 的脚手架（calcPrefWidths 框架、placeChild、水平/垂直分派、与 RenderBlock 集成）。

**工作分解（WBS）**：

| # | 任务 | 代码量 | 估算 |
|---|------|--------|------|
| 1 | CSS 属性接入（CSSPropertyNames + CSSParser 解析 + 值映射） | ~1.5-2k | 3-5 天 |
| 2 | RenderStyle 承载（StyleFlexibleBoxData 现代字段 + 枚举 + inherit/diff） | ~1-1.5k | 2-4 天 |
| 3 | CSSStyleSelector 应用（属性→RenderStyle） | ~0.5-1k | 2-3 天 |
| 4 | **核心布局算法**（main 轴 grow/shrink/basis 分配 + cross 轴对齐 + 多行 wrap + align-content + auto margin + order） | ~3-4k | 3-5 周 |
| 5 | pref widths（flex 容器 min/max-content 固有尺寸） | ~0.5-1k | 3-5 天 |
| 6 | 测试 + 调试（flex 测试页、对照浏览器、嵌套/百分比/min-max 边界） | — | 2-3 周 |

**时间估算（单人全职）**：

| 口径 | 代码量 | 工时 |
|------|--------|------|
| 乐观（熟悉 WebKit+规范，无意外） | ~6k | 5-6 周 |
| **现实（含调试/边界/架构磨合）** | ~7-8k | **8-10 周（2-2.5 月）** |
| 保守（架构磨合困难/规范坑） | ~9k | 12-14 周（3 月+） |

**建议的里程碑切分（降风险、早验证）**：
- **里程碑 1a（4-5 周）**：单行 flex 核心——flex-direction(row/column) + grow/shrink/basis
  + justify-content + align-items。**覆盖约 80% 真实页面 flex 用法**，且验证
  "老架构能否承载现代布局"这一关键问题。
- **里程碑 1b（3-4 周）**：补全——flex-wrap 多行 + align-content + order + auto margin + 边界完善。

**关键风险（决定落在哪个区间）**：
1. **与老架构磨合（最大变量）**：现代 flex 依赖的尺寸协商（definite/indefinite、
   min-content/max-content 传播）在 2007 年 RenderBlock 框架里**可能不完整**。
   若 calcWidth/calcHeight 协议不支持，需先补这层基础设施 → 工期往保守端走。
   **这是第一里程碑作为试金石的核心验证点，只能动手 1a 才能消除该不确定性。**
2. **嵌套 flex + 百分比尺寸**：规范最复杂、bug 最多处，调试耗时。
3. **规范范围控制**：第一里程碑应砍低频特性（order/align-content 后置），聚焦高频，
   把现实估算压到 6-8 周。

2. **CSS Grid**（~10k 行）
   - 新增 RenderGrid : RenderBlock，实现 layout()
   - Grid 轨道/区域算法（规范较复杂）
   - 加 grid CSS 属性

**阶段 1 完成标志**：主流现代静态页面布局正确（不溢出、不错位）。

### 阶段 2：CSS3 视觉 + 选择器补全（P1）—— ✅ 已完成（2026-06-11）

- 线性/径向渐变、多重背景、filter、完整 transform
- 现代选择器补全
- **完成标志**：页面视觉接近设计稿，不丢样式。
- 完成记录：见 docs/css3-visual-plan.md。3a 圆角/阴影、3b 渐变、3c transform
  （含命中测试）、3d 结构性选择器（nth-child An+B）、3e-1 filter（blur/drop-shadow/
  opacity）均完成；3e-2 filter 颜色矩阵交 picasso 增强（docs/picasso-color-filter-
  request.md）。多重背景后置。

### 阶段 3：基础 Web API（P2）—— ✅ 已完成（2026-06-13）

- localStorage、Fetch、WebSocket、HTML5 表单控件
- **完成标志**：轻交互 Web App（取数据、存状态）能跑。
- 完成记录（见 docs/engine-capability-assessment-2026-06-11.md 进度更新）：
  localStorage/sessionStorage（sqlite3 持久化）、Fetch、WebSocket（libcurl）、
  XMLHttpRequest、HTML5 表单（input 类型 + 约束验证）均完成；并附带补齐现代 DOM
  API（querySelector/classList/dataset/getComputedStyle/getBoundingClientRect/
  MutationObserver/matches/closest/addEventListener）与 CSS calc()、自定义属性+var()。
- 后续候选：transition/animation（需评估帧调度）、IntersectionObserver、
  requestAnimationFrame、<video>/<audio>。

### 安全加固（贯穿全程，见第三节）

与功能开发并行，不可后置。

---

## 三、安全漏洞策略（重点，必须在现有架构上解决）

### 问题本质

Agave 基于 ~2007 年 WebKit，存在大量该时代已知 / 此后发现的 CVE：解析器越界、
JS 绑定 UAF、整数溢出、DOM 生命周期错误等。**分叉切断了上游安全补丁**，
所以安全责任完全落到自己身上，且无法靠"同步上游"解决——必须在现有架构上自行加固。

### 三层防御策略

**第 1 层：定位约束（最有效，零代码）**
- **限定渲染可信/半可信内容**，不当通用浏览器上网
- 可信内容（设备自有页面、受控 LLM 输出）的攻击面极小
- 这是最根本的风险控制——把"渲染任意恶意网站"排除在使用场景外

**第 2 层：主动加固现有代码（核心工程）**
- **系统性审计高危模块**：HTML/CSS 解析器、JS 绑定层、DOM 生命周期、图片解码器
  （这些是 Web 引擎传统漏洞高发区）
- 已做的范例：本项目已根治 QuickJS DOM wrapper UAF、修复 cache OOB、sqlite memcmp OOB
  ——**延续这种"主动找+根治"的模式**，对照已知 WebKit CVE 列表逐个排查对应代码
- **用工具持续扫描**：valgrind（UAF/越界/泄漏）、ASAN（若环境允许）、静态分析（Coverity 类）
- **加固解析器输入边界**：所有 length/offset 计算加溢出检查；不信任外部数据长度
- **收缩攻击面**：编译期关掉不用的特性（插件、不需要的协议、危险 API）

**第 3 层：运行时隔离（架构允许范围内）**
- 内存分配器加固（已用自定义 QuickJS 分配器，可加 canary/越界检测）
- 限制资源上限（DOM 节点数、JS 堆、图片尺寸），防 DoS 类耗尽攻击
- 网络层：强制 HTTPS 校验、限制重定向、超时控制
- 进程级隔离（若 RTOS 支持）：渲染与系统其他部分隔离

### 安全工作的现实定位

- **不追求"绝对安全"**——老引擎做不到，诚实承认
- **目标是"在可信内容定位下，风险可控"**：通过定位约束（第 1 层）把高危场景排除，
  通过主动加固（第 2 层）消除已知高危漏洞，通过隔离（第 3 层）限制爆炸半径
- **持续投入**：安全不是一次性任务，是随功能开发并行的长期工程
- **建立 CVE 对照清单**：维护一份"WebKit 该时代已知 CVE → Agave 对应代码 → 修复状态"
  的跟踪表，逐项排查

---

## 四、明确的限制清单（对外承诺的能力边界）

使用 Agave 引擎的产品必须明确以下限制：

1. **不支持 CSS 动画/过渡的动画效果**（可降级到终态，无平滑动画）
2. **不支持 video/audio 媒体元素**
3. **不支持 WebGL/WebGPU/GPU 加速**，纯软件渲染
4. **现代 SVG 支持有限**（仅老式 ksvg2 能力）
5. **不适合渲染任意第三方网站**——定位为可信/半可信内容渲染器
6. **复杂页面内存成本高**（DOM/Render 硬成本，见 memory-analysis），
   不适合超低内存设备渲染重型页面
7. **安全保证以"可信内容"为前提**，不承诺抵御针对引擎漏洞的定向攻击

---

## 五、工作量与节奏总览

| 阶段 | 内容 | 代码量 | 估算 |
|------|------|--------|------|
| 阶段 1 | 现代 Flexbox（7-8k, 2-2.5月）+ CSS Grid（~10k） | ~18k | 5-7 人月 |
| 阶段 2 | CSS3 视觉 + 选择器 | ~5k | 2-3 人月 |
| 阶段 3 | localStorage/Fetch/WebSocket/表单 | ~15k | 4-6 人月 |
| 安全 | 审计 + 加固 + CVE 排查（并行） | — | 持续 |
| **合计** | 类别 A 全集 | **~35-40k 行** | **~1.5 人年** |

**最小可行起点（第一里程碑）**：阶段 1 的 **Flexbox 里程碑 1a**（单行 flex 核心：
flex-direction + grow/shrink/basis + justify-content + align-items，~4-5k 行，**4-5 周**）。
它覆盖约 80% 真实页面的 flex 用法，且是验证"老架构能否承载现代布局"这一关键问题的
最小试金石。跑通 1a 后再决定是否继续 1b 及后续阶段。详见阶段 1 的 Flexbox 详细评估。

---

## 六、决策检查点

在投入前应确认：

1. **生态位验证**：是否存在具体的"无 GPU + 中端内存 + 可信内容渲染"设备品类有真实需求？
   （这是整个路线的前提，找不到则应转为纯技术储备定位）
2. **资源承诺**：是否有 ~1 人年的持续开发投入？安全是长期负担。
3. **第一里程碑**：先做 Flexbox 里程碑 1a（单行 flex，4-5 周），跑通后再评估是否继续 1b 及后续阶段。

---

## 关联文档

- docs/evaluation-2026-06-01.md — 项目整体评估
- docs/embedded_gui_design.md — 嵌入式 GUI 设计
- examples/watchweb/docs/memory-analysis.md — 内存占用分析
- examples/watchweb/docs/render-thread-design.md — 渲染线程方案

---

## 后续任务优先级（2026-06-13 决策，按预算排序）

阶段 1/2/3 + 地基补全完成后，按"省预算优先、确定性高优先"推进：

1. **现代选择器 :is() / :where() / :has()** —— ✅ 已完成（commit 35785470）。复用成熟选择器引擎，三维审查通过。
2. **writing-mode** —— 国际化纵排，触及布局方向。
3. **aspect-ratio + object-fit** —— 实用、范围可控。
4. **IntersectionObserver** —— 需视口/布局观察基础设施。
5. **transition + animation** —— 价值最高（最大单一空白）但最贵、风险最高（需新建
   帧驱动动画子系统，headless 验证有不确定性）。**视预算情况而定**；推进前先做
   可行性调研。
6. **<video> / <audio>（媒体元素）** —— **暂缓**。当前无需求；低端设备上媒体解码
   管线不实用、不符轻量定位。记录备查，未来按需再评估。

预计成本量级（参考，±50%，主要变数是工具链踩坑）：
- :is/:where ~150–300k token；:has ~300–500k
- writing-mode ~600k–1M
- aspect-ratio + object-fit ~250–450k
- IntersectionObserver ~400–700k
- transition+animation ~1.5–2.5M（先 ~150k 可行性调研）

### 执行顺序决策（2026-06-13）

现代选择器完成后，确定推进顺序：
1. **A：aspect-ratio + object-fit**（下一步，低风险高频）。
2. **C：writing-mode**（其后，国际化纵排）。
3. **B：transition + animation**（最后，视预算而定；推进前先做 ~150k 可行性调研，
   确认软渲染/无合成器架构能否承载帧驱动动画）。

### 进度更新（2026-06-13，路线 A 完成后复盘）

- **A：aspect-ratio + object-fit —— ✅ 已完成**（commit 0bf82520）。三维审查通过，
  780 全套测试通过。
- **C：writing-mode —— 暂缓**（移入「后续开发项目」）。调研发现本引擎的 CSS 盒布局
  完全没有逻辑坐标抽象（早于 WebKit 的 writing-mode 重构），全功能纵排等于重演逻辑
  坐标大重构，触及最复杂脆弱的 RenderBlock/RenderBox/bidi/InlineTextBox 代码，且无
  有用的「部分实现」中间态。writing-mode 属于小众功能，高价值场景集中于 CJK 竖排
  （日语出版/中文古籍）。当前目标内容无 CJK 竖排需求，性价比过低，暂缓。
- **下一步：B：transition + animation 可行性调研**（~150k）。评估软渲染/无合成器
  架构能否承载帧驱动动画，再决定是否全力实现。

### 后续开发项目（当前无实际需求，按需再评估）

- **writing-mode**（CJK 竖排）—— 需逻辑坐标抽象大重构，无 CJK 竖排内容前不做。
- **`<video>` / `<audio>`（媒体元素）** —— 低端设备媒体解码管线不实用、不符轻量定位。

### 进度更新（2026-06-13，B2 + transform 插值完成）

- **B1：transition —— ✅ 已完成**（commit 2cfde407）。
- **B2：animation + @keyframes —— ✅ 已完成**（commit 9bf265db）。@keyframes 解析、
  animation 属性集（name/duration/delay/timing-function/iteration-count/direction/
  fill-mode/play-state + 简写）、AnimationController 关键帧时间轴插值（iteration/
  direction 语义）。796 测试通过。
- **transform 插值 —— ✅ 已完成**（commit 9c4ce5a6）。transition 与 animation 均可
  动画 transform：兼容的操作序列（同长度同类型）逐分量插值，不兼容则中点离散跳变。

已知限制：
- `translateX()` / `translateY()` 单轴函数存在预存解析边界（在普通 CSS 中也受影响，
  与动画无关），动画请用等价的 `translate(x, y)`。`scale()`/`rotate()`/`translate()`
  正常。
- 动画插值范围：数值（opacity/宽高/margin/padding）、颜色、transform。离散属性不插值。
- 无合成层，动画在主渲染路径同步执行；低端设备复杂动画可能掉帧（已用脏矩形最小重绘 +
  无动画停表节流缓解）。
