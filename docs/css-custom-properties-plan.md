# CSS 自定义属性 (`--*`) 与 `var()` 实现计划

记录 CSS 自定义属性与 `var()` 的架构评估、风险与分阶段方案。**这是触及 CSS 引擎
核心的大改动**(解析/存储/级联/计算值四层),与前面增量式的 DOM API 不同,需谨慎
分阶段并充分回归。本文档供决策:是否、以及如何推进。

---

## 一、目标与价值

```css
:root { --brand: #3366ff; --gap: 8px; }
.box  { color: var(--brand); margin: var(--gap, 4px); }
```

自定义属性 + var() 是现代设计系统(主题、令牌)的基础。缺失会导致大量现代页面
颜色/尺寸算不出(回退到无效值)。价值高,但实现成本与风险也高。

## 二、现状基线(已实地查证)

CSS 引擎的属性模型是**整数 ID 制**,自定义属性的任意字符串名与之根本不兼容:

| 层 | 现状 | 与自定义属性的冲突 |
|----|------|--------------------|
| **Tokenizer**(tokenizer.flex) | `ident = -?{nmstart}{nmchar}*`,`nmstart=[_a-zA-Z]`。单个前导连字符,其后须为字母 | **`--foo` 无法 token 化**(第二个 `-` 不是 nmstart)。`var(` 作为 FUNCTION 可识别,但 `var(--foo)` 内部的 `--foo` 不行 |
| **属性名→ID**(makeprop.pl 生成 gperf) | `getPropertyID(name)` 未知名→`CSS_PROP_INVALID=0` | `--foo` 映射为 0 → **属性被丢弃** |
| **声明存储**(CSSProperty) | `{int m_id; RefPtr<CSSValue> m_value}` | int 键无法表达任意字符串名的自定义属性 |
| **级联/继承**(CSSStyleSelector::applyProperty(int id, CSSValue*)) | 按 int id 的大 switch 应用到 RenderStyle 字段 | 自定义属性不对应任何 RenderStyle 字段;且**默认继承**(与普通属性不同) |
| **计算值**(applyProperty / convertToLength 等) | 直接消费 CSSValue | var() 须在应用前**替换**为自定义属性的值(含 fallback、循环检测) |

**结论**:四层都要改,其中 tokenizer 与存储模型是根基性改动。

## 三、架构方案

### 3.1 Tokenizer(解析层)
- 扩展 `ident` 或新增 custom-property token:允许 `--` 前缀
  (`customproperty = "--"{nmchar}*`)。需改 tokenizer.flex 并用 flex 重新生成
  tokenizer.cpp(已验证 flex 工具链可用)。**风险:tokenizer 是 CSS 解析最底层,
  改动影响所有 CSS 解析,须充分回归。**
- 注意 SEC-001:重新生成的 tokenizer.cpp 历史上有绝对路径泄露,重生成时一并清理。

### 3.2 自定义属性的存储与级联
- 自定义属性**不进** CSSProperty 的 int-id 模型。改为在 RenderStyle 上挂一个
  **custom properties map**(`HashMap<AtomicString, RefPtr<CSSValue>>`,存于
  StyleInheritedData 以获得**自动继承**语义)。
- CSSParser 解析到 `--name: value` 时,把 value 原样(token 序列/字符串)存入一个
  特殊 CSSValue(CSSCustomPropertyValue,保留原始 token 以便 var() 替换)。
- CSSStyleSelector 在级联时把自定义属性写入 RenderStyle 的 custom map(继承自父)。

### 3.3 var() 替换(计算值层)
- 普通属性值里出现 `var(--name[, fallback])` 时,在**应用该属性前**做替换:
  从 RenderStyle 的 custom map 取 `--name` 的值(token 序列),替换进原值再重新
  解析为目标属性。无值则用 fallback;仍无则属性视为无效(unset)。
- **循环检测**:`--a: var(--b); --b: var(--a)` 须检测并视为无效,防无限递归。

### 3.4 JS 接口(可选,后置)
- `element.style.setProperty('--x', v)` / `getPropertyValue('--x')` /
  `getComputedStyle(el).getPropertyValue('--x')`。先做 CSS 声明层,JS 读写后置。

## 四、分阶段(降风险、可回退)

每阶段独立可编译、可测、可提交:

- **阶段 A(解析地基)**:tokenizer 支持 `--` token + CSSParser 接受 `--name: value`
  声明并存入 CSSCustomPropertyValue(此阶段仅"不报错地解析并保存",不影响渲染)。
  退出:含自定义属性的样式表不再丢弃该声明;全套回归无变。
- **阶段 B(存储+继承)**:RenderStyle custom map + 级联写入 + 继承。退出:
  `getComputedStyle` 能读到自定义属性值(经 JS 或 C++ 测试)。
- **阶段 C(var() 替换)**:普通属性值的 var() 替换 + fallback + 循环检测。退出:
  `color: var(--brand)` 实际生效;含循环的安全失效。
- **阶段 D(JS setProperty/getPropertyValue,可选后置)**。

**建议:先做 A,作为试金石验证 tokenizer 改动的回归影响**(类比 Flexbox 第一里程碑)。
A 风险最高(动 tokenizer),若 A 平稳,B/C 是增量。

## 五、风险评估

| 风险 | 级别 | 缓解 |
|------|------|------|
| tokenizer 改动影响全部 CSS 解析 | **高** | 阶段 A 单独验证;全套 CSS benchmark 回归;保留旧 tokenizer 对照 |
| 级联/继承语义引入回归 | 中 | custom map 独立于现有属性路径,不改 applyProperty 的 int switch |
| var() 循环 / 性能 | 中 | 循环检测 + 替换深度上限 |
| 重新生成 tokenizer 的 SEC-001 路径泄露 | 低 | 重生成后清理路径 |
| 工作量 | — | 估 ~3-5k 行,A/B/C 三个子里程碑 |

## 六、与"放弃清单"的关系

var() 属于 roadmap 类别 A(可在现有架构上补 —— 它是解析/存储/计算值层,不触碰
软件渲染架构),与被放弃的 transition/animation(需帧调度/合成)性质不同。**做它
不违背分叉定位。**

## 七、待决策

1. 是否推进?(价值高、风险中高、工作量 3-5k 行)
2. 若推进,是否按 A→B→C 分阶段、每阶段单独提交并回归?
3. 阶段 A(tokenizer)是关键试金石,建议先只做 A 再评估。

> 本文档为评估与计划,**尚未动手实现**。等决策后按选定阶段推进。
