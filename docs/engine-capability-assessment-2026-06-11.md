# Agave 内核能力评估：与主流浏览器（Chrome/Safari）的差距

> 评估日期：2026-06-11
> 评估方式：实地调研代码库（CSSPropertyNames.in、CSSSelector、DOM、rendering、
>           bindings），非凭印象。
> 背景：阶段 1（Flexbox+Grid）、阶段 2（CSS3 视觉+选择器）完成后的现状评估。

## 一、结论

**尚未追平，差距仍显著。** 阶段 1+2 让 Agave 在**布局与基础视觉**上达到约"2015 年
浏览器"水平——能正确渲染大量现代静态页面（不错乱、不丢样式），但与今天持续演进的
Chrome/Safari（2024 引擎）相比，在 CSS 广度、DOM API、动态能力上有**约 8–10 年代差**。

区分两个概念：
- **常用子集的正确性**：阶段 1+2 做得扎实，主流页面布局/视觉基本正确——务实、高价值。
- **特性完整性**：离主流浏览器全集仍差很远。

## 二、实测数据（来自代码库）

### CSS 布局 —— 接近追平常用子集 ✅
- RenderBlock / Table / Inline / FlexibleBox / Grid 均存在且经测试。
- Flexbox、Grid（含结构性选择器）为近期补齐，覆盖现代布局主力。
- 差距：writing-mode（纵排/国际化）、aspect-ratio、object-fit、多列完善度、
  容器查询（container queries）、subgrid 等。

### CSS 视觉 —— 常用够用，高级缺失 ⚠️
- 已有：圆角、阴影、渐变（linear/radial）、transform（含命中测试）、
  filter（blur/drop-shadow/opacity）。
- 关键缺口：
  - **calc() 完全缺失**（实测确认无解析）——现代 CSS 基础设施，影响面极大。
  - **var() / CSS 自定义属性缺失**——现代设计系统普遍依赖。
  - **transition / animation 缺失**（属性名都未注册）——动态视觉完全没有。
  - 视口单位 vw/vh/vmin、clip-path、mask、backdrop-filter、mix-blend-mode 等缺失。
  - 3D transform、颜色矩阵 filter（已交 picasso 增强）。

### CSS 选择器 —— 接近追平常用集 ✅
- 43 种伪类/伪元素；结构性选择器（nth-child An+B 等）刚补齐。
- 差距：:is()/:where()/:has() 等新选择器缺失。

### DOM API —— 差距最大的一块 ❌
- 已有：addEventListener、getComputedStyle。
- 缺失：querySelector / querySelectorAll、classList、dataset、
  getBoundingClientRect、MutationObserver。
- 影响：这些是现代 JS 操作 DOM 的基础，缺失使绝大多数现代 Web App 脚本无法运行。

### HTML5 —— 部分 ⚠️
- 已有：canvas、SVG。
- 缺失：<video>/<audio>（无 media 元素类）、<template>、<dialog>、新表单控件。

### JS 引擎 —— Agave 的相对优势 ✅
- 使用 QuickJS（现代引擎，支持 ES2020+：Promise/async/let/const/箭头函数/模块）。
- 比原版 2007 WebKit 的 JSCore 先进得多。瓶颈不在 JS 语言层，而在 DOM/Web API 绑定。

## 三、差距分类汇总

| 维度 | 状态 | 差距 |
|------|------|------|
| 核心布局（block/flex/grid/table） | 接近追平常用子集 | 小 |
| CSS 选择器 | 接近追平常用集 | 小-中 |
| CSS 视觉静态效果 | 常用够用 | 中 |
| CSS 基础设施（calc/var） | 缺失 | 大 |
| CSS 动态（transition/animation） | 缺失 | 大 |
| 现代 DOM API（querySelector 等） | 缺失 | 大 |
| JS 语言（QuickJS） | 现代 | 小 |
| 多媒体 / 新 HTML5 元素 | 部分缺失 | 中-大 |

## 四、判断

1. 追平主流浏览器全集不现实，也非 Agave 定位（嵌入式、可信内容、轻量）所需。
2. 当前三个最高 ROI 缺口（远高于继续堆 CSS 视觉）：
   - **calc()**：现代 CSS 布局基础设施，缺它很多页面尺寸算不对。
   - **querySelector / classList 等现代 DOM API**：现代脚本的命脉。
   - **transition / animation**：现代 UI 的动态预期。
3. 阶段 3（Web API）方向正确，但建议先补 calc() + querySelector 这两个"地基"，
   其缺失比缺 Fetch 更致命。

## 五、建议的优先级调整

在进入阶段 3（localStorage/Fetch/WebSocket）之前，插入一个
**"CSS/DOM 地基补全"里程碑**：
- calc()、var()（CSS 基础设施）
- querySelector / querySelectorAll、classList（现代 DOM API）

对"不丢样式、脚本能跑"的实际收益最高，是进入交互式 Web App 支持前的必要地基。

## 六、备注

本报告为某一时点的快照，随后续里程碑推进，相关结论应同步更新。

---

# 进度更新（2026-06-11，地基补全里程碑后）

自首次评估后，完成了"CSS/DOM 地基补全"里程碑（4a + 4b）。以下据代码实测更新。

## 已补齐的高 ROI 缺口

| 缺口 | 首评状态 | 现状 |
|------|----------|------|
| **calc()** | 完全缺失 | ✅ 已支持（混合单位线性式 percent±px、乘除常数；不支持 em/嵌套/min-max，安全拒绝）|
| **querySelector / querySelectorAll** | 缺失 | ✅ 已支持（Document + Element，复用选择器引擎）|
| **classList（DOMTokenList）** | 缺失 | ✅ 已支持（add/remove/toggle/contains，与 className 同步）|
| 结构性选择器 nth-child 等 | 缺失（阶段 2 前） | ✅ 已支持（An+B 全形式）|
| CSS3 视觉（圆角/阴影/渐变/transform/filter） | 缺失 | ✅ 已支持（阶段 2）|

## 工具链能力打通（战略意义）

- **QJS 绑定可干净重新生成**：证明 IDL → 绑定流程健康（正确 defines 下与 committed
  0 差异）。后续 DOM/Web API 扩展不必手写脆弱的 custom binding。
- **grammar 可现代化重新生成**（bison 3.5），并已去除 DeprecatedString 依赖。
- 这两点让阶段 3 的 Web API 扩展（多为 IDL + C++ 实现）成本大幅下降。

## 仍存在的差距（更新后）

### CSS
- **var() / 自定义属性**：仍缺（已规划，按需后置）。
- **transition / animation**：仍完全缺失——现代 UI 动态效果的最大空白。
- writing-mode、aspect-ratio、object-fit、clip-path、backdrop-filter、
  mix-blend-mode、:is()/:where()/:has()：仍缺。
- filter 颜色矩阵（grayscale 等）：已交 picasso 增强（3e-2）。

### DOM API
- 仍缺：dataset、getBoundingClientRect、MutationObserver。
- 已有：addEventListener、getComputedStyle、querySelector/All、classList。

### Web API（阶段 3 目标）
- **已有且实质实现**：XMLHttpRequest（xml/XMLHttpRequest.cpp，798 行）——AJAX 基线
  在；setTimeout 有；JSON 由 QuickJS 内置（ES2020）。
- **仍缺**：localStorage / sessionStorage、Fetch、WebSocket、HTML5 新表单控件、
  <video>/<audio>。

## 差距分类（更新）

| 维度 | 状态 | 差距 |
|------|------|------|
| 核心布局 | 接近追平常用子集 | 小 |
| CSS 选择器（含 nth-child/querySelector） | 接近追平常用集 | 小 |
| CSS 视觉静态效果 | 常用够用 | 中 |
| CSS 基础设施 calc() | ✅ 已补 | 小（var 待补）|
| **CSS 动态 transition/animation** | **缺失** | **大（当前最大空白）** |
| 现代 DOM 查询 API | ✅ 已补主力 | 小（dataset/getBoundingClientRect 待补）|
| JS 语言（QuickJS ES2020+） | 现代 | 小 |
| 存储/网络（localStorage/Fetch/WS） | 部分（XHR 有） | 中 |
| 多媒体 / 新 HTML5 元素 | 部分缺失 | 中-大 |

## 更新后的判断

地基补全后，Agave 在**静态页面渲染 + 基础脚本交互**上已显著接近"现代可信内容
浏览器"的可用线。剩余最高优先的两个方向：

1. **transition / animation**：现代 UI 视觉动态的最大空白（CSS 侧）。
2. **localStorage + Fetch**：交互式 Web App 的存储与现代网络基线（Web API 侧，
   即阶段 3 主体；XHR 已提供过渡能力）。

dataset / getBoundingClientRect 等零散 DOM API 可随阶段 3 顺带补齐。
