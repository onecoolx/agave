# 里程碑：CSS/DOM 地基补全

> 提出依据：docs/engine-capability-assessment-2026-06-11.md
> 目标：补齐"不丢样式、现代脚本能跑"的两块地基——现代 DOM 查询 API 与 CSS 计算
>       基础设施。优先级高于阶段 3 的 localStorage/Fetch。
> 前置：阶段 1（布局）、阶段 2（视觉+选择器）完成。

## 一、范围与排序

按"复用度高、风险低"优先排序（先 DOM 查询，后 CSS 计算）：

### 4a：现代 DOM 查询 API（复用现有选择器引擎，风险低）
- `Element.classList`（add/remove/toggle/contains/length/item）—— 类列表基础设施
  （NamedMappedAttrMap::m_classList）已存在，主要是包装 + 绑定。
- `document.querySelector(selector)` / `querySelectorAll(selector)` —— 复用 CSSParser
  选择器解析 + CSSStyleSelector 匹配逻辑，新增树遍历匹配。
- `Element.querySelector` / `querySelectorAll`（限定子树）。

### 4b：CSS 计算基础设施（触及 Length 模型，风险中-高）
- `calc()`：需扩展 Length 模型以承载"混合单位表达式"（当前 Length 只能是单一
  Fixed 或 Percent，calc(100% - 20px) 无法表达）。
- `var()` / 自定义属性 `--x`：需在 style 解析期做变量替换。
- 这一块改动核心数据结构，单独推进、充分测试。

## 二、可行性调研结论

- **选择器匹配引擎可复用**：CSSStyleSelector 已有 checkSelector/checkOneSelector，
  querySelector 只需"解析选择器字符串 + 遍历子树调用匹配"。
- **类列表基础设施已存在**：NamedMappedAttrMap::getClassList() 提供类名列表。
- **绑定机制**：IDL + bindings/scripts/generate-bindings.pl 生成 QJS 绑定（committed，
  需重新生成）。
- **calc 的难点**：Length 是单一类型（Fixed/Percent/Auto…），不支持混合单位表达式；
  需引入 CalculationValue 或在 CSSPrimitiveValue 层做表达式树，并在 computeLengthInt
  路径求值。这是 4b 的主要工程量与风险。

## 三、工程纪律（延续）

- 编译宏隔离（沿用 ENABLE(MODERN_CSS3) 或新增开关）。
- 每子任务：单元测试 + benchmark（headless）。
- 三维审查（正向/反向/安全）；安全：选择器字符串、calc 表达式深度等外部输入加上限。
- mp.sh 提交，commitlint，不带里程碑编号，--signoff，不 push。

## 四、退出标准

- 4a：querySelector/querySelectorAll/classList 可用，覆盖常见选择器；现代脚本能用
  标准方式查询/操作 DOM。
- 4b：calc() 支持常见混合单位（length ± length/percent，乘除常数）；var() 支持
  基本变量定义与引用。
- 全套回归无新增失败；ASan 无内存错误；恶意输入鲁棒。

## 五、进度日志

- 2026-06-11：基于内核能力评估创建本里程碑计划，调研可行性。先做 4a（DOM 查询），
  后做 4b（CSS 计算）。

## 4a 完成（2026-06-11）：现代 DOM 查询 API

- querySelector / querySelectorAll（Document + Element，复用 CSSStyleSelector 匹配
  引擎；parseSelector 经 parseRule 提取选择器链；StaticNodeList 静态快照）。
- Element.classList（DOMTokenList：add/remove/toggle/contains/length/item/toString，
  直接读写 class 属性，与 className 实时同步）。
- IDL + QJS 绑定：Document.idl/Element.idl 加方法，新建 DOMTokenList.idl，用
  generate-bindings.pl 重新生成 buildQJS 绑定。

### 关键发现：QJS 绑定生成是健康可重现的
此前以为 committed 绑定与重新生成"分叉"（含手工定制），实测证明是**假象**：根因是
重新生成时缺 `LANGUAGE_JAVASCRIPT` define（location 属性被 `#if defined(
LANGUAGE_JAVASCRIPT)` 包裹）。用正确参数重新生成与 committed **0 差异**：

```
perl -I bindings/scripts bindings/scripts/generate-bindings.pl \
  --generator QJS \
  --defines "LANGUAGE_JAVASCRIPT=1 ENABLE_XPATH=1 ENABLE_SVG=1" \
  --include dom --include html --include css --include page \
  --outputdir buildQJS  dom/<Interface>.idl
```

结论：QJS 绑定可随 IDL 干净重新生成，无丢失风险。这是后续扩展 DOM/Web API 的
可靠基础（不必手写 custom binding）。KJS 引擎当前 OFF（未编译），buildKJS 暂不同步。

### 修复的引用计数 bug
`Shared` 基类初始 refcount 为 0，**不能用 adoptRef**（它不 ref）。StaticNodeList 与
Element::classList 改用 `PassRefPtr/RefPtr(new ...)`（构造即 ref），否则首个 JS 包装
finalizer deref 后对象被提前释放 → use-after-free。

### 测试
8 C++ 单元测试（selectors_api_test.cpp）+ 12 benchmark 断言
（dom_selectors_api_test.html，含 classList 改类触发 CSS 规则的布局联动）。
698 全套通过（唯一失败为预存 flaky 计时基准）。恶意/畸形选择器鲁棒（无崩溃）。
