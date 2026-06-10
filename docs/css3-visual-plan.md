# 阶段 2：CSS3 视觉 + 选择器补全 — 计划

> 对应路线图《lightweight-engine-roadmap.md》阶段 2（P1）。
> 前置：阶段 1（现代 CSS 布局：Flexbox + Grid）已彻底完成。

## 一、目标

让页面**视觉接近设计稿、不丢样式**。阶段 1 解决了"布局错乱"，阶段 2 解决
"长得不对"——圆角、阴影、渐变、变换、半透明等现代视觉效果，以及现代结构性
选择器。

**完成标志**：主流现代页面的视觉呈现接近设计稿，常见 CSS3 视觉属性与选择器
不被忽略。

## 二、现状调研结论（关键，决定了工作量）

绝大多数视觉效果**底层绘制原语已存在**，阶段 2 主体是"加 CSS 解析 + 接到已有
原语"，而非从零实现绘制。这与路线图判断一致，ROI 高。

### GraphicsContext 已具备的绘制原语
- `fillRect(FloatRect, CanvasGradient*)` — 渐变填充（picasso 后端 SVG/Canvas 已在用）
- `setShadow(IntSize, blur, Color)` / `clearShadow()` — 阴影 + 模糊
- `fillRoundedRect(...)` / `addRoundedRectClip(...)` — 圆角矩形填充与裁剪
- `beginTransparencyLayer(opacity)` / `endTransparencyLayer()` — 透明层
- `clip(Path)` / `fillPath(Path)` / `strokePath(Path)` — 路径裁剪与填充
- `concatCTM(AffineTransform)` — 坐标变换（目前仅 SVG 路径在用）

### 已解析 / 已应用但未渲染
- `-webkit-transform`、`-webkit-transform-origin`：已解析（CSSParser）、已存入
  style（CSSStyleSelector，`RenderStyle::m_transform`），并有 `CSSTransformValue`。
  **但 HTML 渲染未接入**——`concatCTM` 只在 SVG 路径用，`RenderObject::localTransform()`
  只返回平移。→ transform 的"最后一公里"是把 m_transform 应用到 RenderLayer 绘制。
- `-webkit-border-radius`（含四角）、`-webkit-box-shadow`：已解析、已应用到 style。
  需确认绘制接入是否完整，并补无前缀别名。

### 缺失
- **CSS 渐变解析**：`linear-gradient()` / `radial-gradient()` 作为 background-image
  值，目前**未解析**（后端有 CanvasGradient 能画，但 CSS 层没有入口）。
- **无前缀别名**：`border-radius`、`box-shadow`、`transform`、`transition` 等只有
  `-webkit-` 前缀，缺标准无前缀名（与阶段 1 修复的 `box-sizing` 同类问题，现代页面
  普遍用无前缀）。
- **结构性选择器**：已有 first-child / first-of-type / hover / focus / not( / lang( /
  root / empty / checked / enabled / disabled / target 等；**缺** `nth-child()`、
  `nth-of-type()`、`last-child`、`last-of-type`、`only-child`、`only-of-type`、
  `nth-last-child()` 等。
- **多重背景**：`background-image` 多值（逗号分隔多层）需确认。
- **filter**：`filter: blur()/brightness()` 等无前缀属性未解析；软渲染下仅部分可行。

## 三、里程碑拆分

按"先补全已有原语的 CSS 入口（高 ROI、低风险）→ 再做需要新绘制逻辑的"排序。

### 里程碑 3a：无前缀别名 + 圆角/阴影绘制补全（P0，最高 ROI）
把已实现的视觉能力用标准无前缀属性暴露出来，确保绘制闭环。
- 3a-1 无前缀别名：`border-radius`（+四角）、`box-shadow`、`transform`、
  `transform-origin`、`transition`（解析层别名，复用既有 -webkit case）
- 3a-2 圆角绘制闭环：验证/补全 `border-radius` 在 RenderBox 边框与背景裁剪的绘制
- 3a-3 盒阴影绘制闭环：验证/补全 `box-shadow` 经 setShadow 绘制（外阴影优先）
- 3a-4 单元测试（解析）+ benchmark（headless 几何/存活）

### 里程碑 3b：CSS 渐变（P1）
- 3b-1 解析 `linear-gradient()`（角度/方向 + 多色标）作为 background-image 值
- 3b-2 解析 `radial-gradient()`（基本形状 + 色标）
- 3b-3 把渐变接到背景绘制：构造 CanvasGradient，走 `fillRect(FloatRect, CanvasGradient*)`
- 3b-4 多重背景（逗号分隔多层）——若复杂度可控则纳入，否则后置
- 3b-5 单元测试 + benchmark

### 里程碑 3c：HTML transform 渲染接入（P1，技术风险最高）
- 3c-1 RenderLayer 绘制路径应用 `m_transform`（translate/scale/rotate）
- 3c-2 transform-origin 支持
- 3c-3 命中测试（hit testing）按变换逆映射（点击坐标）—— **完整实现**（用户确认）
- 3c-4 重绘区域（dirty rect）随变换扩展，避免变换元素残影
- 3c-5 单元测试 + benchmark
- **范围决策**：完整做，含命中测试与正确的重绘区域，不走"只读静态视觉"路线。
- **风险**：变换涉及图层合成、重绘区域、命中测试，是阶段 2 最易引入回归的部分。
  应对：分步落地（先绘制变换 → 再命中测试逆映射 → 再重绘区域），每步配回归测试；
  动画/过渡（transition 驱动的连续变换）仍后置到 3e 之后另议。

### 里程碑 3d：现代结构性选择器（P1）
- 3d-1 `nth-child(an+b)` 解析（含 odd/even）与匹配
- 3d-2 `nth-of-type` / `last-child` / `last-of-type` / `only-child` / `only-of-type` /
  `nth-last-child`
- 3d-3 单元测试（选择器匹配）+ benchmark

### 里程碑 3e（可选）：filter 子集
- `filter: blur() / opacity() / brightness()` 等软渲染可行子集；不可行项明确后置。
- 视前四个里程碑进度与价值再决定是否纳入本阶段。

## 四、可行性与风险

| 里程碑 | 可行性 | 主要风险 |
|--------|--------|----------|
| 3a 别名+圆角/阴影 | 高（原语齐全） | 低；绘制闭环可能有边角 case |
| 3b 渐变 | 高（后端能画） | 中；渐变 CSS 语法解析较繁琐 |
| 3c transform 渲染 | 中 | 高；图层/重绘/命中测试回归风险（完整做，分步落地） |
| 3d 选择器 | 高 | 低；nth 公式解析 + 性能 |
| 3e filter | 中低 | 软渲染性能/可行性受限 |

**总体策略**：3a→3b→3d 为价值确定、风险可控的主线；3c 完整实现（含命中测试与
重绘区域），分步落地、每步回归；3e 按需。

## 五、工程纪律（延续阶段 1）

- 编译宏隔离新实现（如 `ENABLE(MODERN_CSS3)` 或复用现有开关），默认开启
- 每个子任务配套：单元测试（gtest）+ benchmark（headless 几何/存活验证）
- 每个里程碑完成后做三维代码审查：正向（功能）/ 反向（bug/边界/重复）/ 安全
  （无敏感信息、无注释泄露、无界输入加固）
- 提交走 mp.sh 流程，commitlint 格式，**不带里程碑编号**，--signoff，不 push
- 安全贯穿：渐变色标数、选择器 nth 公式、transform 链等外部输入加合理上限

## 六、退出标准

- 3a/3b/3d 完成；3c 完整实现（视觉 + 命中测试 + 正确重绘区域）
- 自建"真实视觉样例页"（卡片阴影 + 圆角、渐变按钮/背景、斑马纹表格 nth-child、
  简单 transform）headless 验证通过
- 全套单元测试无回归；ASan 下无内存错误；恶意输入不崩溃/不 OOM
- 三维审查通过

## 七、进度日志

- 2026-06-10：完成阶段 2 调研与计划拆分。确认绝大多数视觉原语已存在，主体工作为
  "CSS 解析入口 + 接驳已有原语"。待与用户评审后确定首个里程碑。

- 2026-06-10：**里程碑 3a 完成（无前缀别名 + 圆角/阴影绘制闭环）。**
  - 在 CSSParser::parseValue 顶部把无前缀 CSS3 视觉属性 ID 重映射到既有
    -webkit- ID（border-radius/四角、box-shadow、transform、transform-origin）。
    因 addProperty 用重映射后的 ID 存储，解析/应用/绘制全链路（都 keyed on
    -webkit- 枚举）单点复用，无需散落改动。
  - 圆角、盒阴影的解析+应用+绘制+overflow 全链路本就完整（RenderBox/RenderObject
    paintBoxShadow、addRoundedRectClip），3a 只补标准无前缀入口。
  - transform/transform-origin：无前缀名现已被识别并解析；transform 的 apply 目前
    仍在 unimplemented 列表（只存 transform-origin），完整渲染接入是里程碑 3c。
  - 测试：5 解析单元测试（无前缀/前缀 border-radius、四角、box-shadow、
    transform-origin）+ 8 benchmark 断言（圆角/阴影纯视觉不改盒尺寸、前缀等效、
    存活）。662 全套通过，无回归。三维审查（正向/反向/安全）通过。

- 2026-06-10：**里程碑 3b 完成（CSS 渐变）。**
  - 新增编译宏 ENABLE_MODERN_CSS3（默认 ON），隔离 CSS3 视觉新实现。
  - 数据模型：RenderStyle.h 加 StyleGradient（Linear/Radial + 角度 + 色标向量）
    与 GradientColorStop；BackgroundLayer 并行存 RefPtr<StyleGradient>，拷贝/赋值/
    相等比较都纳入（gradient 按值比较）。
  - 解析：CSSParser::parseGradient 解析 linear-gradient()（<angle>deg 或 "to side/
    corner" 方向，0deg=top 顺时针）与 radial-gradient()（居中），多色标
    （color [percentage]?）。新增 CSSGradientValue（CSSValue 子类）承载解析结果；
    在 parseBackgroundImage 识别 QFunction 调 parseGradient。
  - 应用：mapBackgroundImage 对 isGradientValue 存入 BackgroundLayer；因 gradient 非
    primitive/list 值会被 HANDLE_BACKGROUND_VALUE 宏跳过，故在 background-image apply
    case 里直接处理。
  - 绘制：RenderBox::paintGradientBackground 在背景色之后、图片之前构造 CanvasGradient
    （线性按角度求过盒中心的端点，径向用 center+min(w,h)/2），走
    fillRect(FloatRect, CanvasGradient*)。
  - 修复关键 bug：StyleGradient 误用 adoptRef 致引用计数从 0 起，CSSGradientValue
    析构时提前删除而 BackgroundLayer 仍持有 → use-after-free（ASan 捕获）；改用普通
    RefPtr 使计数正确。
  - 安全：色标上限 kMaxGradientStops=64、位置钳制 [0,1]、≥2 色标方有效；角度 float
    安全；无未受信输入放大向量。
  - 已知限制：CSSGradientValue::cssText() 返回空（序列化不完整，不影响渲染）；
    多重背景、复杂 radial 形状/尺寸关键字后置。
  - 测试：6 解析单元测试 + 8 benchmark 断言（线性/角度/方向/径向/渐变+圆角组合）。
    668 全套通过，ASan 无内存错误，三维审查通过。

- 2026-06-10：**里程碑 3c 完成（HTML transform 渲染接入，完整）。**
  - 数据模型：RenderStyle.h 加 TransformOperation（translate/scale/rotate/skew/
    matrix），StyleTransformData 存 Vector<TransformOperation>；加 hasTransform()、
    transformOperations()、applyTransform(AffineTransform&, w, h)。
  - 应用：CSSStyleSelector 实现 -webkit-transform apply（从 CSSTransformValue 列表
    提取操作，translate 支持百分比）；从 unimplemented 列表移除。
  - 几何：applyTransform 按 transform-origin（默认 50%）构造矩阵。注意底层矩阵原语
    为后乘语义（M <- Op*M），故按 translate(-origin)、逆序操作、translate(origin)
    构建，使 mapPoint 得到 T(o)·ops·T(-o)·p。
  - 绘制：RenderLayer::paintLayer 对变换层 save() + concatCTM(平移到绝对位置∘局部
    变换∘反平移) 包裹，末尾 restore()。
  - 命中测试：hitTestLayer 开头用 RAII 守卫，按逆矩阵映射命中点到元素局部坐标
    （isInvertible 守卫），各 return 路径自动恢复原点。
  - 重绘区域：computeAbsoluteRepaintRect 对变换盒用 mapRect 扩展 box-local 重绘矩形，
    避免变换后残影。
  - 封装边界：webcore 层只用 AffineTransform 封装对象与 GraphicsContext::concatCTM/
    save/restore；实现 concatCTM（GraphicsContextPS，经 setMatrix）；picasso ps_* 仅
    限 platform/picasso/ 内，不泄漏到 webcore。
  - 修复 4 个 bug：矩阵组合顺序、validUnit 不支持 FAngle（致 rotate/skew 全被拒）、
    percent Length 误用 value() 触发断言崩溃、parseTransform 的 a->fValue 笔误。
  - 测试：8 解析+几何单元测试 + 6 benchmark 断言（含点击命中变换后位置、原位置不
    命中）。676 全套通过（唯一失败为预存 flaky 计时基准，重跑通过）。三维审查通过。
