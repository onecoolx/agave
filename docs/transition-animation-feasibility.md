# CSS Transition / Animation 可行性调研

**日期**：2026-06-13
**范围**：评估在 Agave（2007 年代 WebKit 分叉，软渲染、无 GPU 合成器）实现 CSS
`transition` 与 `animation`（含 `@keyframes`）的可行性、帧驱动机制与 headless 验证手段。
**预算**：~150k token 调研，不含实现。

---

## 结论：可行（中等工程，风险可控）

帧驱动动画在本架构**有成熟先例可循**，不需要新建合成器或推翻渲染管线。建议推进实现，
按里程碑分期：先 `transition`（更简单、自带触发点），后 `animation` + `@keyframes`。

---

## 关键发现

### 1. 已有成熟的定时器→重绘先例：GIF 动画

`platform/graphics/BitmapImage`（GIF 多帧）已经实现了完整的"定时器驱动的随时间视觉更新"：

- `Timer<BitmapImage> m_frameTimer` + `startAnimation()` / `advanceAnimation()`：到点推进帧。
- `advanceAnimation` 调 `imageObserver()->animationAdvanced(this)`。
- `CachedImage::animationAdvanced`（loader/CachedImage.cpp:285）将更新传播到重绘。
- `shouldPauseAnimation` 提供"无人观察则暂停"的节流。

**CSS 动画可完全复用这一模式**：一个定时器周期性地重算插值后的样式值并触发重绘。

### 2. 定时器基础设施齐备且与事件循环集成

- `platform/Timer.h`：`TimerBase` / `Timer<T>`，支持 `startOneShot` / `startRepeating`，
  基于 `currentTime()` 真实时钟。
- `platform/picasso/SharedTimerPS.cpp`：`eventDispatch()` 每次循环检查共享定时器到期，
  到期即 `sharedTimerFired()` → WebCore 触发所有到期定时器。
- headless（test/headless.cpp）的 `pump()` 每 10ms 调 `macross_event_dispatch()`
  （→ `eventDispatch()`）+ `macross_view_update()`，使用真实 `nanosleep`。

**意味着 headless 下定时器按真实墙钟触发，动画会真实推进且可观察**——与 GIF 同理。

### 3. 样式可在不重载页面的前提下重算并重绘

- `RenderObject::setStyle`（rendering/RenderObject.cpp:2153）比较新旧样式，按需
  `setNeedsLayout` 或仅 `repaint()`（line 2187/2210/2229）。
- `Document::recalcStyle`（dom/Document.cpp:1022）支持增量重算。

**每帧只需把插值样式塞给渲染对象并触发最小重绘**，无需重新解析或重载。

### 4. 当前完全没有任何 transition/animation 痕迹

- `CSSPropertyNames.in` / `RenderStyle.h` 无 transition/animation/keyframe 属性。
- 无 `requestAnimationFrame`。
- 是一张白纸，需从属性、@keyframes 规则解析、动画控制器全新搭建（但不触碰布局核心）。

---

## 建议实现方案（分期）

### 里程碑 B1：transition（更简单，先做）

1. **属性**：`transition-property` / `transition-duration` / `transition-timing-function`
   / `transition-delay` + `transition` 简写。存入 RenderStyle（rareNonInherited）。
2. **触发**：在 `RenderObject::setStyle` 比较新旧样式时，对可插值且声明了 transition 的
   属性，创建一个"运行中的过渡"（起始值、目标值、时长、缓动）。
3. **驱动**：新建 `AnimationController`（页面级或文档级），持有一个 `Timer`，
   按帧（如 ~16ms 或按需）推进所有运行中过渡，计算插值样式，触发重绘。复用 GIF 的
   "shouldPause 节流"思路：无运行中动画则停表。
4. **插值**：先支持数值类属性（opacity、宽高、margin/padding、颜色、transform 已有矩阵）。
   缓动先支持 linear / ease / ease-in / ease-out / cubic-bezier。

### 里程碑 B2：animation + @keyframes（其后）

1. **@keyframes 规则解析**：新增 at-rule，存关键帧（百分比 → 属性集）。
2. **属性**：`animation-name/duration/timing-function/delay/iteration-count/direction/
   fill-mode/play-state` + `animation` 简写。
3. **驱动**：复用 B1 的 AnimationController，按 keyframe 时间轴插值。

---

## 风险与限制

- **性能**：每帧重算样式 + 重绘在低端无 GPU 设备上成本不低。需用脏矩形最小重绘 +
  无动画时停表节流（GIF 已有此模式）。复杂动画或大量并发动画可能掉帧——符合"轻量、
  可信内容"定位，文档化为预期限制。
- **插值范围**：首版只做数值/颜色/transform 等可线性插值属性；离散属性（display 等）
  按规范不插值或仅在端点切换。
- **合成层**：本引擎无合成器，动画不能像现代浏览器那样在合成线程跑；全部在主渲染路径，
  与布局/重绘同步。这是架构约束，非缺陷。
- **headless 验证**：动画随真实时间推进，可在 pump 总时长内多次采样断言中间/终态值
  （如读取 getComputedStyle 或几何）。需注意时序确定性——建议断言"区间"或"终态"而非
  精确某一帧。

## 成本再估

- B1（transition）：~600k–1M token（控制器 + 插值 + 触发 + 测试）。
- B2（animation+@keyframes）：~600k–1M token（at-rule 解析 + 时间轴）。
- 合计低于此前 ~1.5–2.5M 的悲观估计，因为**帧驱动机制有现成先例、不需新建合成器**。
