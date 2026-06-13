# DOM API 补全实现计划

记录"DOM API 补全"里程碑的范围、现状调研与实现切分。目标：补齐现代脚本操作 DOM
的高频刚需 API，使主流交互式页面的脚本能跑起来。

---

## 一、现状基线（已实地调研代码库）

引擎用 QuickJS，DOM 绑定经 IDL 生成（bindings/scripts/generate-bindings.pl
--generator QJS → buildQJS/）。已完成 querySelector/querySelectorAll/classList/calc()。

调研结论分三类：**已就绪只差注册** / **C++已有缺绑定** / **全缺**。

| API | C++ | IDL | QJS 绑定 | JS 可用 | 分类 |
|-----|-----|-----|---------|--------|------|
| textContent | ✅ | ✅ Node.idl | ✅ | ✅ | 已可用 |
| contains | ✅ | ✅ Element.idl | ✅ | ✅ | 已可用 |
| getComputedStyle | ✅ DOMWindow | ✅ | ✅ QJSDOMWindow | ❌ window 手写绑定未注册 | 差注册 |
| **addEventListener/removeEventListener** | ✅ EventTargetNode.h | EventTarget.idl（未接入 Node） | ❌ QJSEventTargetNode.cpp **缺实现**（.h 已设计齐全） | ❌ | C++有缺绑定 |
| **getBoundingClientRect** | ❌（但 RenderObject::absoluteBoundingBoxRect() 就绪） | ❌ | ❌ | ❌ | 缺对象+方法 |
| **dataset** | ❌ | ❌ | ❌ | ❌ | 全缺 |
| **matches / closest** | matchesSelector 引擎就绪 | ❌ | ❌ | ❌ | 缺方法 |
| **children / firstElementChild / lastElementChild / nextElementSibling / previousElementSibling / childElementCount** | ❌ | ❌ | ❌ | ❌ | 全缺（遍历，小） |
| MutationObserver | ❌ | ❌ | ❌ | ❌ | 全缺（大，后置） |

### 关键发现

1. **addEventListener 半成品**：`bindings/qjs/QJSEventTargetNode.h` 已完整设计
   （AddEventListener/RemoveEventListener/DispatchEvent + 全部 on* 处理器枚举），
   `qjs_events.h` 的 `JSEventListener`（包装 JSValue 的 EventListener，bridge 到 QJS
   函数）也已存在且被 window 的 AddEventListener 使用。**唯独 QJSEventTargetNode.cpp
   实现从未写**。KJS 路径有完整的 `JSEventTargetNode.cpp` 可作参考。这是本里程碑
   工作量最大、价值最高的一项。
2. **getBoundingClientRect 底层就绪**：`RenderObject::absoluteBoundingBoxRect()`
   返回绝对边界，只需建一个 ClientRect 对象 + Element 方法。
3. **getComputedStyle 几乎免费**：C++/IDL/QJSDOMWindow 全有，只是实际 window 用的是
   手写绑定（qjs_window.cpp，注释说完整属性列表会致 QuickJS 内存损坏，故只注册子集）。
   需在手写绑定里安全地补注册 getComputedStyle。

---

## 二、实现切分（按 ROI 与风险排序）

低风险高就绪度的先做，addEventListener（最大件）居中，MutationObserver 后置。

| # | 任务 | 工作量 | 要点 |
|---|------|--------|------|
| 1 | **getComputedStyle 注册** | 小 | 在 window 手写绑定补注册（已有 C++/绑定），注意 window 内存损坏陷阱 |
| 2 | **getBoundingClientRect + ClientRect** | 中 | 新建 ClientRect 类（top/right/bottom/left/width/height/x/y）+ Element 方法，用 absoluteBoundingBoxRect() |
| 3 | **Element 遍历 API** | 小-中 | children/firstElementChild/lastElementChild/nextElementSibling/previousElementSibling/childElementCount，复用现有 traverse |
| 4 | **matches / closest** | 小 | 复用 matchesSelector 引擎（querySelector 同款），closest 向上遍历 |
| 5 | **dataset (DOMStringMap)** | 中 | data-* 属性映射；驼峰↔连字符转换；动态读写代理 |
| 6 | **addEventListener/removeEventListener/dispatchEvent** | 大 | 实现 QJSEventTargetNode.cpp，接入 Node 原型链，JSEventListener 生命周期（GC 根）。参考 KJS JSEventTargetNode.cpp + window AddEventListener |
| 7 | MutationObserver | 大 | **后置**，单独里程碑评估（需变更记录队列 + 微任务投递） |

本里程碑范围：**任务 1–6**。MutationObserver 后置。

---

## 三、关键技术风险

1. **addEventListener 绑定生命周期**：JSEventListener 持有 JS 回调 JSValue，必须在
   GC 中正确标记（mark），否则回调被回收致 UAF；EventTargetNode 销毁时要清理监听器。
   QJSEventTargetNode.h 已声明 mark/finalizer，需正确实现。
2. **Node 原型链接入**：Element 继承 EventTargetNode（C++），但 QJS 绑定的原型链需让
   Node/Element 实例能查到 addEventListener。需确认 QJSNode 原型如何挂接
   EventTargetNode 方法（可能在 QJSNode 初始化时混入，或 Element 原型继承）。
3. **window 手写绑定陷阱**：getComputedStyle 注册要避开"完整属性列表致内存损坏"问题
   ——只增量加单个属性，参考 localStorage/fetch 的注册方式。

---

## 四、裁剪宏

DOM API 是核心能力，不加裁剪宏（与 localStorage/Fetch 等可选 Web API 不同，DOM 查询/
事件是基础设施）。getBoundingClientRect 的 ClientRect 等为小对象，无独立裁剪价值。

---

## 五、退出标准

- getComputedStyle(el) 在 JS 中可用并返回样式
- el.getBoundingClientRect() 返回正确的 top/left/width/height
- el.children / firstElementChild / nextElementSibling 等遍历正确
- el.matches(sel) / el.closest(sel) 正确
- el.dataset.foo 读写映射到 data-foo
- **el.addEventListener('click', fn) 能注册并在事件触发时调用 fn**；removeEventListener
  能移除；dispatchEvent 工作
- 每项配单元测试 + benchmark；全套回归通过；三维审查通过
