# Agave 未完成事项 & 与现代浏览器差距分析

> 日期：2026-06-14
> 依据：代码库实测 + docs/engine-capability-assessment-2026-06-11.md +
>       docs/lightweight-engine-roadmap.md
> 背景：完成阶段 1/2/3、地基补全、CSS 自定义属性、现代选择器、aspect-ratio/
>       object-fit、以及完整动画系统（transition/animation/@keyframes/transform
>       插值/fill-mode/play-state）之后的现状盘点。

## 一、未完成任务清单（含复杂度）

复杂度维度：是否触及渲染架构核心、是否需新子系统、token 成本量级。

### A 类：可在现有架构上增量补（不碰核心）

| 任务 | 类别 | 复杂度 | 说明 |
|------|------|--------|------|
| picasso 颜色矩阵 filter（grayscale/sepia/invert/saturate/brightness/contrast/hue-rotate）| CSS 视觉 | 低（等 picasso）| 解析已就绪（ColorMatrixOp 已 parse），等 picasso 提供色彩变换原语接入，需求文档已写 |
| aspect-ratio em/嵌套/min-max（calc 完善）| CSS 基础设施 | 低-中 | 当前 calc 不支持 em/嵌套/min-max（安全拒绝），补全需扩展 calc 求值器 |
| clip-path、mask、mix-blend-mode | CSS 视觉 | 中 | 各自独立属性，clip-path/mask 需 picasso 裁剪/遮罩支持 |
| 视口单位 vw/vh/vmin/vmax | CSS 基础设施 | 中 | 需接入视口尺寸到长度求值 |
| `<template>` / `<dialog>` | HTML5 元素 | 中 | 新增元素类 + 行为，不碰渲染核心 |
| IntersectionObserver / ResizeObserver | DOM API | 中 | 需视口/布局观察基础设施 |
| History API | DOM API | 低-中 | pushState/popstate，相对独立 |
| letter-spacing/word-spacing/font-size 动画插值 | CSS 动态 | 中 | 当前未做，需 FontDescription 重建 + updateFont，每帧成本需评估 |
| translateX()/translateY() 单轴解析修复 | bug 修复 | 低 | 预存解析边界（普通 CSS 也受影响），translate() 可规避 |
| 多列布局（multicol）完善度 | CSS 布局 | 中 | 已有基础，完整度待补 |
| subgrid、container queries | CSS 布局 | 中-高 | 现代布局新特性，需扩展 Grid/查询机制 |
| requestAnimationFrame | 帧调度 | 低-中 | 动画 Timer 基础设施已在，封装 JS API 成本低 |

### B 类：暂缓（无当前需求，性价比低）

| 任务 | 复杂度 | 暂缓理由 |
|------|--------|---------|
| writing-mode（CJK 纵排）| 极高 | 需引入贯穿 RenderBlock/RenderBox/bidi/InlineTextBox 的逻辑坐标抽象 = 重演 WebKit 当年大重构。小众，无 CJK 竖排需求 |
| `<video>` / `<audio>` | 极高 | 需解码管线 + 媒体时钟 + 合成，独立大子系统，依赖外部编解码器。不符无 GPU 嵌入式定位 |

### C 类：架构性放弃（与定位根本冲突）

| 任务 | 放弃理由 |
|------|---------|
| GPU 合成层（RenderLayerCompositor）| 与纯软件渲染根本对立，补它=换渲染后端=重做引擎。这正是 Agave"无 GPU 嵌入式"定位核心取舍 |
| 3D transform（含 perspective）| 需 3D 合成/Z 排序管线，软渲染下不实用 |
| WebGL / WebGPU | 需 GPU |

> 注：路线图早期文档曾把 transition/animation 列入"放弃"，已通过可行性调研推翻并成功
> 实现（复用 GIF 的 Timer 帧驱动模式，无需合成器）。该结论已过时。

## 二、与现代主流浏览器（Chrome/Safari 2024）的差距

### 已基本抹平 ✅
核心布局、CSS 选择器（含 :is/:where/:has）、CSS 基础设施（calc/var）、CSS 动态
（transition/animation/@keyframes）、现代 DOM API、存储/网络、JS 语言（QuickJS ES2020+）。

### 仍存在的差距 —— 多数与定位无关

| 差距维度 | 与现代浏览器距离 | 对"Agave 目标"是否关键 |
|----------|-----------------|----------------------|
| GPU 合成 / 硬件加速 | 本质差距（数量级工程）| 否——正是定位取舍 |
| JS 引擎性能/完整度 | V8/JSC 远超 QuickJS | 中——够用非高性能 |
| 多媒体（video/audio/WebGL/WebRTC）| 完全缺失 | 否——不符定位 |
| 网络现代化（HTTP/2/3、Service Worker、PWA）| 缺失 | 视场景 |
| 布局深水区（writing-mode/subgrid/container queries/sticky）| 缺失 | 小众，多数不关键 |
| CSS 视觉高级（clip-path/mask/backdrop-filter/颜色矩阵 filter）| 部分缺失 | 锦上添花 |
| 无障碍/国际化/复杂文字排版（a11y 树、shaping、bidi）| 不完整 | 视场景 |
| 安全现代化（CSP、现代 TLS 套件、沙箱）| 部分 | 视场景 |

## 三、核心判断

- **朝 Agave 目标（无 GPU 嵌入式、渲染可信内容）：已非常接近该定位上限。** 静态页面
  正确布局、视觉接近、脚本能操作 DOM、能取数存状态、能跑现代 CSS 动画。A 类增量为
  锦上添花，按真实内容需求驱动即可。
- **朝通用现代浏览器：仍有本质距离**，但大块（GPU 合成、多媒体、3D）已理性划在范围
  之外——这是定位选择，非缺陷。
- **可能的未来瓶颈**：(1) JS 引擎性能（QuickJS 解释执行，重计算/大型 SPA 吃力）；
  (2) 布局边界完备性（float/table/复杂嵌套极端情况，需真实页面测试逐步补齐）。

## 四、推荐的收尾候选（低成本、与架构契合）

1. **picasso 颜色矩阵 filter** —— 需求文档已就绪，补齐 filter 最后一块（待 picasso 增强）。
2. **requestAnimationFrame** —— 动画 Timer 基础设施已在，封装 JS 侧帧调度成本低。

## 五、A 类任务可行性与 token 成本评估

> 评估基于代码实测的接入点。token 区间为单个里程碑的实现+测试+三维审查，±50%
> （主要变数：工具链踩坑、回归修复）。参照历史：:is/:where ~150-300k，:has ~300-500k，
> aspect-ratio+object-fit ~实测约 1 个会话，transition/animation 各 ~600k-1M。

| 任务 | 能否做 | 接入点（实测）| token 估算 | 备注 |
|------|--------|--------------|-----------|------|
| **requestAnimationFrame** | ✅ 容易 | qjs_window.cpp 已有 setTimeout/installTimeout（支持函数回调+timer），rAF 即 callback-list + ~16ms tick | **150–300k** | 最低风险收尾项；动画 Timer 基础设施已在 |
| **picasso 颜色矩阵 filter** | ⚠️ 受阻于 picasso | RenderLayer.cpp:1479 已有 ColorMatrixOp 跳过桩；解析就绪。**内核侧仅需在 picasso 提供 ps_set_color_matrix 后接 1 个 case** | 内核侧 **100–200k**；picasso 侧不计 | 架构原则：内核不做逐像素，等 picasso |
| **translateX()/translateY() 解析修复** | ✅ 容易 | CSSParser.cpp:4337 parseTransform 的 args 游标问题（类似已修的 cubic-bezier）| **80–150k** | 小 bug 修复 + 回归 |
| **History API（pushState/popstate）** | ✅ 可做 | qjs_window/qjs_history 绑定 + Frame 历史栈 | **250–450k** | 相对独立，无渲染核心改动 |
| **`<template>` / `<dialog>`** | ✅ 可做 | 新增 HTMLTemplateElement/HTMLDialogElement 类 + 解析器惰性内容处理（template 需 content fragment）| template **300–500k**；dialog **250–400k** | template 的惰性内容树较繁；dialog 较简单 |
| **letter/word-spacing/font-size 动画插值** | ✅ 可做 | AnimationController blend + RenderStyle setFontSize 需 FontDescription 重建 | **200–350k** | font-size 每帧重建字体，需评估性能；spacing 较简单 |
| **视口单位 vw/vh/vmin/vmax** | ⚠️ 中等侵入 | 视口源已有（FrameView::visibleWidth/Height，见 MediaQueryEvaluator）。**但 computeLengthDouble(style) 不接收视口**，需把 view/视口 thread 进长度求值的众多调用点 | **400–700k** | 侵入面在"长度求值签名"，回归面较广 |
| **calc() 补 em/嵌套/min-max** | ✅ 可做 | calc 求值器（Length side-table）；em 需 style 字体、嵌套需递归求值、min/max 需区间 | **350–600k** | em 较易（已有 computeLengthDouble），嵌套+min/max 需扩展求值结构 |
| **clip-path / mask** | ⚠️ 受阻于 picasso | 需 picasso 裁剪路径/遮罩原语；内核侧解析+接入 | 内核 **300–500k**；picasso 侧不计 | 类似颜色矩阵，底层能力依赖 picasso |
| **mix-blend-mode** | ⚠️ 受阻于 picasso | 需 picasso 混合模式原语 | 内核 **200–400k**；picasso 侧不计 | 同上 |
| **IntersectionObserver** | ✅ 可做 | MutationObserver 已提供 observer 模式模板；需视口/布局相交计算 + 回调调度 | **400–700k** | 需布局几何相交判断 |
| **ResizeObserver** | ✅ 可做 | 同上 observer 模式；监听盒尺寸变化 | **350–600k** | 比 IntersectionObserver 略简单 |
| **多列布局完善 / subgrid / container queries** | ⚠️ 较重 | 触及 RenderBlock 多列流 / Grid 求轨 / 查询求值 | multicol **400–700k**；subgrid **800k–1.2M**；container queries **800k–1.5M** | container queries 需"按容器尺寸重求样式"的新机制，最重 |

### 分组小结

- **低成本、立即可做（无外部依赖）**：requestAnimationFrame、translateX 修复、
  History API。合计约 **0.5–0.9M**，纯收尾增量。
- **中成本、可做但有侵入/性能评估**：calc 补全、vw/vh、letter-spacing 动画、
  template/dialog、Intersection/ResizeObserver。各 **0.2–0.7M**。
- **受阻于 picasso（内核侧成本低，但要等底层）**：颜色矩阵 filter、clip-path、mask、
  mix-blend-mode。内核侧各 **0.1–0.5M**，但需 picasso 先提供原语。
- **较重的现代布局**：subgrid、container queries（各 0.8M+），需求驱动再做。

### 推荐落地顺序（按 ROI）

1. requestAnimationFrame（最低成本、补齐动画 JS 基础）
2. translateX/Y 解析修复（消除已知 bug）
3. 颜色矩阵 filter 内核侧预接入（待 picasso 就绪即通）
4. calc em/嵌套补全（提升现有 calc 实用度）
5. 其余按真实内容需求驱动

