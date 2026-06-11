# Picasso 需求：上下文级颜色滤镜原语

> 提出方：Agave 浏览器内核（CSS filter 支持，里程碑 3e）
> 日期：2026-06-11
> 状态：待 picasso 评估 / 实现
> 关联：docs/css3-visual-plan.md 里程碑 3e-2

## 一、背景与架构原则

Agave 正在实现 CSS3 `filter` 属性。其中**空间类滤镜**已可用 picasso 现有原语实现：
- `blur()` → `ps_set_blur`
- `drop-shadow()` → `ps_set_shadow`
- `opacity()` → `ps_set_alpha`

但**颜色矩阵类滤镜**目前没有对应的 picasso 原语：
- `grayscale()`、`sepia()`、`invert()`、`saturate()`、`brightness()`、`contrast()`、
  `hue-rotate()`

### 为什么请求 picasso 实现，而不是在内核做

Agave 的架构原则：**图形图像处理尽量交给 picasso，内核不做像素级处理**。理由：

1. **性能**：颜色矩阵是逐像素运算，在内核用 CPU 循环既慢又重复造轮子。
2. **硬件加速**：picasso 底层未来可能引入 GPU / SIMD 加速；颜色变换若在 picasso 内，
   可直接受益，内核无需改动。
3. **一致性**：picasso 已是统一的绘制后端（blur/shadow/gradient 都在其中），颜色滤镜
   归属同一层更内聚。
4. **可移植**：内核保持与具体像素格式无关，picasso 负责各像素格式/平台的实现。

因此，颜色滤镜应作为 picasso 的上下文级能力提供。

## 二、期望的 API

参照现有上下文状态设置原语（`ps_set_blur`/`ps_set_alpha`/`ps_set_gamma` 的风格），
建议提供"上下文级颜色变换"接口。两种可选设计：

### 方案 A（推荐）：通用颜色矩阵

最通用，可表达全部 CSS 颜色滤镜（CSS filter 规范本身就用 4x5 颜色矩阵定义）：

```c
/*
 * 设置上下文的颜色变换矩阵。后续绘制的内容在合成前，每个像素的 RGBA 经此
 * 4x5 矩阵变换：
 *   [R']   [m0  m1  m2  m3  m4 ]   [R]
 *   [G'] = [m5  m6  m7  m8  m9 ] * [G]
 *   [B']   [m10 m11 m12 m13 m14]   [B]
 *   [A']   [m15 m16 m17 m18 m19]   [A]
 *   (最后一列为偏移量，输入分量归一化到 0~1)
 * 传 NULL 清除颜色变换（恢复为单位变换）。
 */
PEXPORT void PICAPI ps_set_color_matrix(ps_context* ctx, const float matrix[20]);
```

内核只需把 CSS filter 函数换算成颜色矩阵传入，所有颜色滤镜都能表达，无需 picasso
为每个滤镜单独加接口。

### 方案 B：分立的便捷接口

若通用矩阵成本高，可提供分立接口（参数范围对齐 CSS filter）：

```c
PEXPORT float PICAPI ps_set_grayscale(ps_context* ctx, float amount); // 0~1
PEXPORT float PICAPI ps_set_sepia(ps_context* ctx, float amount);     // 0~1
PEXPORT float PICAPI ps_set_invert(ps_context* ctx, float amount);    // 0~1
PEXPORT float PICAPI ps_set_saturate(ps_context* ctx, float amount);  // 0~ (1=原值)
PEXPORT float PICAPI ps_set_brightness(ps_context* ctx, float amount);// 0~ (1=原值)
PEXPORT float PICAPI ps_set_contrast(ps_context* ctx, float amount);  // 0~ (1=原值)
PEXPORT float PICAPI ps_set_hue_rotate(ps_context* ctx, float deg);   // 角度
```

**倾向方案 A**：一个接口覆盖全部，且与 CSS filter 规范的矩阵定义天然对应。

## 三、语义要求

1. **作用范围**：与 `ps_set_blur` 一致——设置后影响后续绘制，受 `ps_save`/`ps_restore`
   状态栈管理。
2. **与其他状态叠加**：应能与 blur、alpha、shadow 叠加（CSS 允许多个 filter 串联）。
3. **默认值**：单位矩阵（无变换）。
4. **数值范围**：输入 RGBA 归一化 0~1；结果应钳制到 0~1。

## 四、CSS filter → 颜色矩阵换算参考

供 picasso 实现方案 B 时参考，或供内核在方案 A 下换算（标准公式，来自 CSS Filter
Effects Level 1 规范）：

- **grayscale(a)**、**sepia(a)**、**saturate(a)**、**hue-rotate(deg)**：规范给出确定的
  矩阵系数。
- **invert(a)**：`v' = (1 - 2a)·v + a`（对 RGB）。
- **brightness(b)**：`v' = b·v`。
- **contrast(c)**：`v' = c·v + (0.5 - 0.5c)`。

（规范原文：https 链接略，避免外链；实现时以 W3C Filter Effects L1 的矩阵为准。）

## 五、对 Agave 内核侧的影响

picasso 提供上述接口后，Agave 内核侧只需：
1. GraphicsContext 增加封装（如 `setColorMatrix`），与已封装的 `setBlur` 同样模式，
   picasso API 不暴露到 webcore 层。
2. 在 RenderLayer 绘制带颜色滤镜的层时，换算并调用封装方法。
3. 完成里程碑 3e-2。

在此能力就绪前，Agave 的 CSS 颜色滤镜函数将被解析但不生效（与无 filter 等同，不影响
布局与其他渲染）。
