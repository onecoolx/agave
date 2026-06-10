# CSS Grid 重构详细计划与执行清单

> 状态：规划中（未开工）
> 关联：docs/lightweight-engine-roadmap.md 阶段 1 第二项
> 前置：现代 Flexbox（1a+1b）已完成，验证了"老架构能承载现代布局"
> 目标：在 Agave 现有 RenderBlock/RenderStyle 框架上实现 CSS Grid 常用子集
>       （与现代 WebKit 分叉，**重新实现而非移植**）

---

## 〇、迭代纪律：沿用 Flexbox 的成功模式

延续 Flexbox 重构验证有效的纪律：

- **编译宏隔离**：用 `ENABLE(MODERN_GRID)` 隔离新代码，默认开启但可关闭回退。
- **新增 RenderGrid 子类**：`RenderGrid : RenderBlock`，与现有渲染对象并列，
  不改动 RenderBlock/RenderFlexibleBox，零回归风险。
- **CSS 数据与老式属性隔离**：新增 StyleGridData（DataRef），独立于其他 style 数据。
- **每里程碑配套测试**：单元测试（gtest，OPT_UNITTEST=ON）+ benchmark headless 页。
- **mp.sh 提交，不自行 push**；提交信息符合 commitlint（≤70 字符，type(scope): desc）。
- **每里程碑完成做正反向代码审查 + 文档对齐**。

> Grid 比 Flexbox 更复杂（二维 vs 单轴），更需要先规划、分里程碑、早验证。

---

## 一、现状基线（已查证）

- **无任何 RenderGrid 代码**（grep 仅命中 RenderTableSection 的 m_grid 表格网格、
  RenderFrameSet 的 frame GRID，均无关）→ **完全 greenfield**。
- **EDisplay 枚举**：`..., BOX, INLINE_BOX, FLEX, INLINE_FLEX, NONE`。
  Grid 需在此追加 `GRID, INLINE_GRID`（注意：CSS 值关键字顺序须与枚举对齐，
  因 CSSStyleSelector 用 `EDisplay(id - CSS_VAL_INLINE)` 算术映射——Flexbox 已踩过此坑）。
- **RenderObject::createObject**：display 分派创建 RenderObject 子类的位置，
  GRID/INLINE_GRID → new RenderGrid（参照 FLEX/INLINE_FLEX → RenderFlexibleBox）。
- **Length 类型**：`Auto, Relative, Percent, Fixed, Static, Intrinsic, MinIntrinsic`。
  **缺 `fr`（fractional）单位**——Grid 轨道尺寸的核心。需新增表示方式
  （见下「关键数据建模」）。
- **可复用基础**：RenderBlock 的 calcPrefWidths 框架、setOverrideSize/isFlexingChildren
  尺寸协商机制（Flexbox 已验证可用）、placeChild 式定位、与 layoutBlock 的集成点。

---

## 二、关键技术挑战（比 Flexbox 难的地方）

1. **二维布局**：Grid 同时计算行与列两个维度，item 占据 (row, col) 单元格甚至跨多格，
   而 Flexbox 只有单主轴 + 单行/多行。轨道尺寸需先列后行（或迭代）。
2. **轨道尺寸算法（最难）**：`grid-template-columns/rows` 的值可以是
   fixed/percent/`auto`/`fr`/`minmax()`/`repeat()`/`min-content`/`max-content`。
   `fr` 是分配剩余空间的弹性单位（类似 flex-grow 但作用于轨道而非 item）。
   规范的轨道尺寸算法是多步迭代（base size + growth limit + 自由空间分配）。
3. **item 放置算法**：显式定位（`grid-column: 2 / 4`）、自动放置（`grid-auto-flow`）、
   命名线/区域（`grid-template-areas`）。自动放置有"打包/稀疏"两种模式。
4. **隐式网格**：item 超出显式网格时自动生成轨道（`grid-auto-rows/columns`）。
5. **对齐**：`justify-items/self`、`align-items/self`（单元格内对齐）、
   `justify-content/align-content`（整个网格在容器内分布）——比 Flexbox 多一套。

**最大风险**：轨道尺寸算法（含 fr 分配、minmax、内容based 尺寸）。
与 Flexbox 的 grow/shrink 类似但更复杂（二维 + 多种轨道函数）。

---

## 三、关键数据建模决策

### 轨道尺寸表示（fr 单位问题）

CSS Grid 轨道函数无法用现有 Length 表示。需新增结构，建议：

```cpp
struct GridTrackSize {
    enum Type { LengthType, FrType, MinMaxType, AutoType };
    Type type;
    Length length;     // 用于 fixed/percent/auto
    float flex;        // 用于 fr（1fr, 2fr）
    // minmax 可后置（里程碑 2b），先支持 length | fr | auto
};
```

`grid-template-columns: 100px 1fr 2fr` → `[GridTrackSize(100px), GridTrackSize(1fr), GridTrackSize(2fr)]`。

### item 网格定位

```cpp
struct GridPosition {
    bool isAuto;       // auto 放置
    int line;          // 显式起始线（1-based，CSS 习惯）
    int span;          // 跨越轨道数（默认 1）
};
// grid-column-start/end, grid-row-start/end
```

`repeat(n, ...)` 在解析阶段展开为 n 份。命名线/区域后置到里程碑 2b。

---

## 四、里程碑 2a：显式网格核心

**完成标志**：固定/fr/auto 轨道的显式网格布局正确，覆盖最常见的"卡片网格/
仪表盘等分布局"用法（约 70-80% 真实 Grid 页面）。

### Checklist

- [x] **2a-0 探针（前置）**：display:grid 最小骨架（RenderGrid 子类 + 创建分派），
      2x2 固定轨道 + item 按 DOM 顺序自动放置，验证 RenderBlock 框架能承载二维定位。
      **结论：架构足够，setOverrideSize 二维尺寸协商可复用，无需补基础设施。**
- [ ] **2a-1 CSS 属性接入**：`display:grid/inline-grid`、`grid-template-columns/rows`
      （fixed/percent/fr/auto + repeat 展开）、`grid-column/row`（start/end/span）、
      `gap`(row-gap/column-gap) → CSSPropertyNames + CSSValueKeywords + CSSParser + 值映射
- [ ] **2a-2 RenderStyle 承载**：StyleGridData（GridTrackSize 列表、GridPosition、gap）
      + 存取 + inherit/diff（与其他 style 数据隔离）
- [ ] **2a-3 样式应用**：CSSStyleSelector 应用 grid 属性；display:grid 分派到 RenderGrid
- [ ] **2a-4 轨道尺寸算法**：列轨道尺寸（fixed → 占用；auto → 内容；fr → 分配剩余）
      + 行轨道尺寸（同理）；先单维独立计算（列优先）
- [ ] **2a-5 item 放置 + 定位**：显式 grid-column/row 定位 + span；
      auto 放置（grid-auto-flow: row，按行打包）；item 定位到单元格矩形 + gap
- [ ] **2a-6 固有尺寸 + 测试**：grid 容器 min/max-content 接入 calcPrefWidths；
      单元测试 + benchmark 回归页（对照 Chrome 几何）

### 工期（人类熟练工单人全职估算）

| 子任务 | 代码量 | 预计 |
|--------|--------|------|
| 2a-0 探针 | ~0.3k | 2-3 天 |
| 2a-1 CSS 属性 | ~2-2.5k | 1-1.5 周 |
| 2a-2 RenderStyle | ~1-1.5k | 3-5 天 |
| 2a-3 样式应用 | ~0.5-1k | 2-3 天 |
| 2a-4 轨道尺寸 | ~2-3k | 2-3 周 |
| 2a-5 item 放置 | ~1.5-2k | 1.5-2 周 |
| 2a-6 固有尺寸+测试 | — | 1.5-2 周 |
| **2a 合计** | **~7-9k** | **乐观6周/现实8周/保守10周** |

**2a 最大风险**：轨道尺寸算法（fr 分配 + auto 内容尺寸的二维交互）。
fr 分配可复用 Flexbox grow 的经验；auto 轨道需要内容尺寸传播（Flexbox 1a-5 已验证
calcPrefWidths 协议可用，降低此风险）。

---

## 五、里程碑 2b：Grid 补全

**完成标志**：CSS Grid 规范常用子集完整，能渲染绝大多数现代页面 Grid 布局。

### Checklist

- [ ] **2b-1 minmax + 更多轨道函数**：`minmax(min, max)`、`min-content`/`max-content`、
      `fit-content`、`repeat(auto-fill/auto-fit)`
- [ ] **2b-2 命名线 + 区域**：`grid-template-areas`、命名网格线、`grid-area`
- [ ] **2b-3 隐式网格 + 自动流补全**：`grid-auto-rows/columns`、
      `grid-auto-flow: column/dense`
- [ ] **2b-4 对齐**：`justify-items/self`、`align-items/self`、
      `justify-content`、`align-content`
- [ ] **2b-5 边界完善 + 回归**：嵌套 grid、grid 与 flex 互嵌、百分比轨道、
      与 table/float 交互；扩充 benchmark 回归页

### 工期

| 子任务 | 代码量 | 预计 |
|--------|--------|------|
| 2b-1 minmax/轨道函数 | ~1-1.5k | 1-1.5 周 |
| 2b-2 命名线/区域 | ~1.5-2k | 1.5-2 周 |
| 2b-3 隐式网格/自动流 | ~1-1.5k | 1 周 |
| 2b-4 对齐 | ~1-1.5k | 1-1.5 周 |
| 2b-5 边界+回归 | — | 1-1.5 周 |
| **2b 合计** | **~4.5-6.5k** | **乐观5周/现实6.5周/保守8周** |

---

## 六、总工期

| 阶段 | 代码量 | 乐观 | 现实 | 保守 |
|------|--------|------|------|------|
| 2a | ~7-9k | 6 周 | 8 周 | 10 周 |
| 2b | ~4.5-6.5k | 5 周 | 6.5 周 | 8 周 |
| **合计** | **~12-15k** | 11 周 | **~14.5 周（3.5 月）** | 18 周 |

> 人类熟练工程师单人全职估算。比 Flexbox（~10-12k）规模大约 20-30%，
> 主要在轨道尺寸算法与二维放置的复杂度。AI 实际耗时另算。

---

## 七、退出与清理

- Grid 为全新模块（RenderGrid），无旧实现需删除——`ENABLE_MODERN_GRID` 宏在功能
  稳定后可直接移除（保留宏仅为开发期可关闭回退）。
- 与 Flexbox 旧码清理、KJS 清理一起，纳入后续整体清理工作。

---

## 八、建议的起步

按 Flexbox 验证有效的模式，**先做 2a-0 探针**：最小 display:grid 骨架
（RenderGrid 子类 + 固定 2x2 轨道 + DOM 顺序放置），验证：
1. RenderBlock 框架能否承载二维单元格定位
2. 轨道尺寸 → item override size → 子项布局的尺寸协商链路是否走得通
（Flexbox 已验证 setOverrideSize 机制，预期可复用，风险低）

探针跑通后再决定是否继续 2a 全量。

---

## 九、进度日志

（每完成一项更新此处与上方 checklist）

- 2026-06-10：计划与 checklist 建立。确认 Grid 为 greenfield（无旧实现）；
  关键挑战为二维轨道尺寸算法 + fr 单位数据建模；沿用 Flexbox 的编译宏隔离/
  里程碑切分/测试配套/提交纪律。待 review 后决定是否启动 2a-0 探针。

- 2026-06-10：**2a-0 探针完成，结论：架构足够，无需补基础设施。**
  - 编译宏 ENABLE_MODERN_GRID（CMakeLists + mconfig.h.in，默认 ON）。
  - EDisplay 加 GRID/INLINE_GRID；CSSValueKeywords 加 grid/inline-grid（191/192）；
    display 范围检查、isDisplayReplacedType/isOriginalDisplayInlineType、
    CSSComputedStyleDeclaration display switch 均覆盖。
  - 新建 RenderGrid : RenderBlock（RenderGrid.h/.cpp，宏隔离）；RenderObject::createObject
    分派 GRID/INLINE_GRID → RenderGrid；RenderObject 加 isRenderGrid() 虚函数。
  - 关键复用：RenderBox::calcWidth 扩展为 grid 父容器也认 override 宽度
    （isRenderGrid + isFlexingChildren）——与 Flexbox 同一套尺寸协商机制。
  - 探针 layoutGrid：硬编码 2x2 等分单元格，前 4 个流子项按 DOM 顺序放入
    [0,0][0,1][1,0][1,1]，setOverrideSize(宽) + 临时 setHeight(高) 填充单元格。
  - 验证（开宏，headless）：400x200 容器 → 2x2 单元格 200x100，四项位置
    (0,0)(200,0)(0,100)(200,100) 与尺寸全部正确（10/10 断言通过）。
  - 默认态：编译正常，flexbox 29 单元测试全过，无回归
    （唯一失败的 MicroBenchmark 是预存的计时阈值 flaky，ASan 下 2050ms>2000ms，与 grid 无关）。
  - **影响**：2a 最大风险（二维尺寸协商能否复用）已排除 → 可继续 2a 全量。
