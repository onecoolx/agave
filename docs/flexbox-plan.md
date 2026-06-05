# Flexbox 重构详细计划与执行清单

> 状态：进行中（探针阶段）
> 关联：docs/lightweight-engine-roadmap.md 阶段 1
> 目标：在 Agave 现有 RenderBlock/RenderStyle 框架上实现现代 CSS Flexbox
>       （与现代 WebKit 分叉，**重新实现而非移植**）

---

## 〇、迭代纪律：新旧实现编译宏隔离（所有重构通用）

**核心原则：永远保证主干有一个可用的稳定版本。**

- 用编译宏隔离新旧实现：
  ```c
  #if ENABLE(MODERN_FLEXBOX)
      // 新的现代 flexbox 实现（开发中）
  #else
      // 旧的 -webkit-box 实现（稳定，默认）
  #endif
  ```
- **默认关闭新实现**（`ENABLE_MODERN_FLEXBOX` 默认未定义），主干始终走稳定旧码。
- 新实现在宏后开发、测试环境开宏验证。
- 价值：①随时可回退（关宏即回稳定版）；②可对照验证（新旧切换对比，
  如当年 KJS 对照 QuickJS）；③主干任何时间点都能编译运行；④渐进切换。
- **退出条件**：新 flexbox 完整开发 + 充分验证稳定后，才删除旧 `-webkit-box` 代码、
  移除宏。在此之前新旧并存。

> 本原则适用于后续所有重构（Grid、动画降级等），不限于 Flexbox。

---

## 一、里程碑 1a：单行 Flexbox 核心

**完成标志**：常见单行 flex 布局（导航栏/卡片行/居中/等分）几何正确，与浏览器一致，
覆盖约 80% 真实页面 flex 用法。

### Checklist

- [x] **探针**（前置）：display:flex 最小骨架（单行 main 轴，2-3 固定宽度 item）+ 测试页，
      验证现有 RenderBlock 框架能否承载 flex 尺寸协商。**结论：架构足够，无需补基础设施**
      （详见进度日志 2026-06-05）。
- [ ] **1a-1 CSS 属性接入**：`display:flex/inline-flex`、`flex-direction`(row/column)、
      `flex-grow`、`flex-shrink`、`flex-basis`、`justify-content`、`align-items`、
      `flex`(简写) → CSSPropertyNames + CSSParser + 值映射
- [ ] **1a-2 RenderStyle 承载**：StyleFlexibleBoxData（现代字段）+ 枚举
      (EFlexDirection/EJustifyContent/EAlignItems) + 存取 + inherit/diff
      （与老式 box 数据隔离，避免冲突）
- [ ] **1a-3 样式应用**：CSSStyleSelector 应用 flex 属性；`display:flex` 分派到
      modern flex 路径（宏隔离）
- [ ] **1a-4 核心布局算法**：main 轴 basis→grow/shrink 分配 + cross 轴 align-items 对齐
      + justify-content 主轴分布（**单行**，暂不 wrap）
- [ ] **1a-5 固有尺寸**：flex 容器 min/max-content 计算，接入 calcPrefWidths 协议
- [ ] **1a-6 测试+调试**：flex 测试页对照 Chrome 几何，修边界（嵌套/百分比/min-max），
      沉淀 benchmark 回归页

### 工期（人类熟练工单人全职）

| 子任务 | 代码量 | 预计 |
|--------|--------|------|
| 1a-1 CSS 属性 | ~1.5-2k | 4-6 天 |
| 1a-2 RenderStyle | ~1-1.5k | 3-4 天 |
| 1a-3 样式应用 | ~0.5-1k | 2-3 天 |
| 1a-4 核心算法 | ~2.5-3k | 2.5-4 周 |
| 1a-5 固有尺寸 | ~0.5-1k | 3-5 天 |
| 1a-6 测试调试 | — | 1.5-2.5 周 |
| **1a 合计** | **~6-8k** | **乐观4周/现实5周/保守7-8周** |

**1a 最大风险**：2007 年 RenderBlock 尺寸协商（definite/indefinite、
min/max-content 传播）可能不支持现代 flex 所需。若需先补基础设施 → +2-3 周。
**此不确定性只能由探针消除。**

---

## 二、里程碑 1b：Flexbox 补全

**完成标志**：现代 flexbox 规范常用子集完整，能渲染绝大多数现代页面 flex 布局。

### Checklist

- [ ] **1b-1 多行 wrap**：`flex-wrap`(wrap/nowrap/wrap-reverse) + 多行分行算法
- [ ] **1b-2 多行对齐**：`align-content`（多行交叉轴分布）
- [ ] **1b-3 次要特性**：`order`（重排）、`align-self`（单项覆盖）、auto margin
- [ ] **1b-4 边界完善+回归**：column wrap、嵌套 flex、与 table/float 交互、
      百分比 basis 边界；扩充 benchmark 回归页

### 工期

| 子任务 | 代码量 | 预计 |
|--------|--------|------|
| 1b-1 多行 wrap | ~1.5-2k | 1.5-2 周 |
| 1b-2 align-content | ~0.5-1k | 3-5 天 |
| 1b-3 order/align-self/auto-margin | ~1-1.5k | 1-1.5 周 |
| 1b-4 边界+回归 | — | 1 周 |
| **1b 合计** | **~4k** | **乐观3周/现实3.5周/保守4-5周** |

---

## 三、总工期

| 阶段 | 代码量 | 乐观 | 现实 | 保守 |
|------|--------|------|------|------|
| 1a | ~6-8k | 4 周 | 5 周 | 7-8 周 |
| 1b | ~4k | 3 周 | 3.5 周 | 4-5 周 |
| **合计** | **~10-12k** | 7 周 | **~8.5 周（2 月）** | 11-13 周（3 月） |

> 人类熟练工程师单人全职估算。AI 实际耗时另算：代码产出快，但受编译-验证循环、
> 需对照渲染调试、架构磨合未知数限制。

---

## 四、退出与清理（里程碑完成后）

- 新 flexbox 通过全部 benchmark 回归 + 真实页面验证 + valgrind 干净后：
  - 将 `ENABLE_MODERN_FLEXBOX` 设为默认开启，观察一段时间
  - 稳定后删除旧 `-webkit-box` 实现（layoutHorizontalBox/layoutVerticalBox 等）+ 移除宏
- 删除前主干始终保留旧码作为可回退的稳定版本。

---

## 五、进度日志

（每完成一项更新此处与上方 checklist）

- 2026-06-05：计划与 checklist 建立；确立新旧编译宏隔离迭代纪律；启动探针。
- 2026-06-05：**探针完成，结论：架构足够，无需补基础设施。**
  - 实现 `RenderFlexibleBox::layoutModernFlexbox()`（宏 `ENABLE(MODERN_FLEXBOX)` 隔离，
    默认关闭走旧 -webkit-box）：最小单行 main 轴 flex——pass1 按 preferred 宽布局子节点并
    统计 grow，pass2 按 grow 权重分配剩余空间并定位。
  - **关键发现**：现代 flex 的尺寸协商可复用既有机制——`setOverrideSize()` +
    设置 `m_flexingChildren=true`（`RenderBox::calcWidth` 仅在父 flex 容器
    `isFlexingChildren()` 时认可 override 宽度）。这套基础设施已存在且可用。
  - 验证（开宏，headless）：①两个 flex:1 容器400 → 200:200 ✓；②grow 1:2 容器300 →
    103:197 ✓；③固定100+flex:1 容器400 → 100:300 ✓；④嵌套 flex 容器400 → 内层 x=400 ✓
    （嵌套尺寸传播正确，这是最关键的验证）。
  - 默认态（关宏）：编译正常，unit_tests 565 全过，旧 -webkit-box 页面无回归。
  - **影响**：1a 最大风险（架构磨合需补尺寸协商基础设施）已排除 → 工期可落乐观~现实区间。
  - 探针代码保留为 1a-4 的起点骨架（宏隔离，主干默认稳定）。
