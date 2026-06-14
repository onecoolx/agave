# Picasso 2.9.0 能力评估：clip-path / mask / mix-blend-mode

> 日期：2026-06-14
> 评估对象：/home/jipeng/picasso（PICASSO_VERSION = 29000，即 2.9.0）
> 评估方式：实地阅读 include/picasso.h 公开 API + agave 侧 GraphicsContextPS 现状。
> 结论：**三者均可实现**。picasso 2.9.0 已提供全部所需底层原语，无需新增 picasso API
>       即可支持，升级到 2.9.0 后在 agave 侧接入即可。

## 一、逐项结论

### 1. mix-blend-mode —— ✅ 完全可实现，开箱即用

picasso 2.9.0 的 `ps_composite` 枚举已包含 CSS mix-blend-mode 所需的**全部混合模式**：

| CSS mix-blend-mode | picasso ps_composite |
|--------------------|----------------------|
| normal | COMPOSITE_SRC_OVER |
| multiply | COMPOSITE_MULTIPLY |
| screen | COMPOSITE_SCREEN |
| overlay | COMPOSITE_OVERLAY |
| darken | COMPOSITE_DARKEN |
| lighten | COMPOSITE_LIGHTEN |
| color-dodge | COMPOSITE_DODGE |
| color-burn | COMPOSITE_BURN |
| hard-light | COMPOSITE_HARDLIGHT |
| soft-light | COMPOSITE_SOFTLIGHT |
| difference | COMPOSITE_DIFFERENCE |
| exclusion | COMPOSITE_EXCLUSION |
| hue | COMPOSITE_HUE |
| saturation | COMPOSITE_SATURATION |
| color | COMPOSITE_COLOR |
| luminosity | COMPOSITE_LUMINOSITY |

- 接口：`ps_set_composite_operator(ctx, composite)`（picasso.h:1627）。
- **agave 侧现状**：GraphicsContextPS.cpp:590 的 setCompositeOperation 已封装该函数，
  但当前 CompositeOperator 枚举只映射了 Porter-Duff 子集（WebKit 时代的 12 个）。
  扩展为支持 blend mode 只需：扩 CompositeOperator 枚举 + 补 switch 分支 + CSS
  mix-blend-mode 属性解析 + 应用到层合成。
- 全部 16 种分离/非分离混合模式 picasso 都已实现，**无需 picasso 新增任何 API**。

### 2. clip-path —— ✅ 可实现

picasso 2.9.0 提供任意路径裁剪：
- `ps_clip_path(ctx, path, fill_rule)`（picasso.h:1851）—— 按任意 ps_path + 填充规则裁剪。
- `ps_clip_rect` / `ps_clip_rects` / `ps_clip`（当前路径）/ `ps_reset_clip`。
- **agave 侧现状**：GraphicsContext::clip(IntRect) 已封装矩形裁剪（GraphicsContextPS.cpp:437）。
- 接入工作：CSS clip-path 解析（basic-shape：inset/circle/ellipse/polygon、以及 path()）
  → 构造 ps_path → ps_clip_path。**无需 picasso 新增 API**（路径裁剪原语已在）。
- 注：clip-path 的 url(#ref) 引用 SVG 裁剪需 SVG 集成，basic-shape 与 path() 优先。

### 3. mask —— ✅ 可实现

picasso 2.9.0 提供 alpha mask：
- `ps_mask_create_with_data(data, w, h)`（picasso.h:1142）—— 从字节缓冲建 alpha mask。
- `ps_canvas_set_mask(canvas, mask)` / `ps_canvas_reset_mask`（picasso.h:644/654）。
- 接入工作：CSS mask（mask-image：渐变或图像）→ 渲染遮罩到缓冲 → ps_mask_create_with_data
  → set_mask。**无需 picasso 新增 API**。
- 注：mask 的完整 CSS 规范（mask-mode/mask-composite/多遮罩）较繁，可先做
  mask-image 单遮罩（亮度/alpha）覆盖主流用法。

## 二、总体判断

**三者都不需要 picasso 新增原语** —— picasso 2.9.0 的 composite/clip_path/mask API
已经齐全。这与之前"颜色矩阵 filter 需 picasso 新增"的情况不同（颜色矩阵确实需要
picasso 提供 ps_set_color_matrix，目前仍缺）。

因此路径是：**agave 依赖升级到 picasso 2.9.0**，然后在 agave 侧做 CSS 属性解析 +
GraphicsContext 封装扩展 + 渲染接入。全部为 A 类（不碰渲染架构核心）工作。

### token 成本估算（agave 侧，picasso 2.9.0 已就绪的前提下）

| 特性 | picasso 依赖 | agave 侧 token 估算 | 复杂度 |
|------|-------------|---------------------|--------|
| mix-blend-mode | ✅ 已就绪（COMPOSITE_*）| **200–350k** | 低-中（枚举扩展 + 属性 + 层合成接入）|
| clip-path（basic-shape + path()）| ✅ 已就绪（ps_clip_path）| **400–700k** | 中（basic-shape 解析 + 路径构造）|
| mask（mask-image 单遮罩）| ✅ 已就绪（mask API）| **400–700k** | 中（遮罩渲染到缓冲 + 接入）|

### 颜色矩阵 filter（对比）

仍需 picasso 新增 `ps_set_color_matrix` 或类似原语（见
docs/picasso-color-filter-request.md）。这是唯一**真正受阻于 picasso 新增能力**的项，
mix-blend-mode/clip-path/mask 则只需升级到已发布的 2.9.0。

## 三、建议

1. **优先 mix-blend-mode**：agave 侧成本最低（200–350k），picasso 全 16 模式已就绪，
   ROI 最高。
2. clip-path、mask 其次，按真实内容需求驱动。
3. 升级 agave 的 picasso 依赖到 2.9.0 是这三项的共同前提，应先验证升级不破坏现有
   渲染（回归现有 benchmark）。
4. 颜色矩阵 filter 仍走"请求 picasso 新增原语"路径，与上述三项解耦。
