# MutationObserver 实现计划

记录 MutationObserver 的范围、架构与挂接点。MutationObserver 让脚本异步观察 DOM
变更（子节点增删、属性变化、文本变化），是现代框架（虚拟 DOM diff、响应式）依赖的
核心 API。

---

## 一、现状基线（已查证）

- **DOM 变更钩子**：
  - 子节点增删：`dispatchChildInsertionEvents(Node* child, ec)` /
    `dispatchChildRemovalEvents(Node* child, ec)`（ContainerNode.cpp:888/924）—— 集中
    点，**已知具体 added/removed 节点**，是 childList 记录的理想挂接点。
  - 属性：`Element::attributeChanged(Attribute*, preserveDecls)`（StyledElement.cpp:70）。
  - 文本：`CharacterData::dispatchModifiedEvent(StringImpl* prevValue)`
    （CharacterData.cpp:229）—— 带旧值。
- **微任务投递**：QuickJS `JS_EnqueueJob` 入队 + `ScriptInterpreter::drainMicrotasks()`
  （evaluate 后执行）。MutationObserver 规范要求记录在微任务检查点投递，正好契合。
- **无 per-node rare data**：观察者注册用一个**全局/文档级注册表**（side table）。

## 二、范围（务实子集）

实现 MutationObserver 规范的高频核心，砍掉低频特性：

**做**：
- `MutationObserver(callback)` 构造
- `observe(target, options)`：options 支持 `childList` / `attributes` /
  `characterData` / `subtree`
- `disconnect()`：移除该 observer 的所有注册并清空队列
- `takeRecords()`：取出并清空待投递记录
- `attributeOldValue` / `characterDataOldValue`：记录旧值
- MutationRecord：`type` / `target` / `addedNodes` / `removedNodes` /
  `previousSibling` / `nextSibling` / `attributeName` / `oldValue`
- 微任务投递：变更入队 → JS_EnqueueJob 调度一次投递 → 回调收到 records 数组

**不做（后置/砍）**：
- `attributeFilter`（只观察特定属性名）—— 低频，先观察全部属性
- 多个 observer 共享一次微任务的合并投递优化 —— 先每 observer 独立投递（语义正确，
  仅非最优）
- transient registered observers（subtree 节点移出后仍短暂观察）—— 罕用，砍

## 三、架构

### 注册模型
全局注册表 `MutationObserverRegistry`（或挂 Document）：`HashMap<Node*, Vector<Registration>>`，
Registration = { observer, options }。observe() 添加，disconnect()/节点销毁时移除。

### 变更收集（核心算法）
在每个 DOM 变更钩子处，对变更目标节点 **及其祖先链** 遍历：
- 目标节点的注册：若 options 匹配该变更类型 → 入队记录
- 祖先节点的注册：仅当 `subtree:true` 且匹配 → 入队记录
构造 MutationRecord 加入每个匹配 observer 的待投递队列。

### 投递
首次有记录入队时，`JS_EnqueueJob` 调度一个投递微任务（每 observer 一个 pending 标志
防重复调度）。微任务运行时：取出 observer 的记录队列 → 调用其 JS 回调
`callback(records, observer)` → 清空队列。

### 挂接点（C++）
- `dispatchChildInsertionEvents` / `dispatchChildRemovalEvents`：childList 记录
  （addedNodes / removedNodes / previous·nextSibling）
- `Element::attributeChanged`：attributes 记录（attributeName / oldValue）
- `CharacterData::dispatchModifiedEvent`：characterData 记录（oldValue）

为避免在热路径无谓开销：注册表为空时（无任何 observer）变更钩子里的收集**直接 early
return**（一个全局计数器 gMutationObserverCount > 0 才走收集逻辑）。

## 四、绑定

`MutationObserver` 是手写绑定（qjs_mutation_observer.cpp，类似 dataset/websocket），
持 JS 回调 JSValue（GC 保护）。MutationRecord 用 IDL 生成只读属性绑定。

## 五、裁剪宏

`ENABLE_MUTATION_OBSERVERS`（默认 ON，可裁剪）。关闭时 MutationObserver 不注册到全局、
变更钩子收集逻辑编译期去除。

## 六、退出标准

- `new MutationObserver(cb)` + `observe(el, {childList:true})` → appendChild 后回调收到
  childList 记录含 addedNodes
- `{attributes:true, attributeOldValue:true}` → setAttribute 后记录含 attributeName/oldValue
- `{characterData:true}` → 文本节点改变记录含 oldValue
- `{subtree:true}` → 后代变更也被观察
- `disconnect()` 后不再投递；`takeRecords()` 取出待投递
- 单元测试 + benchmark + 全套回归 + 三维审查 + 可裁剪宏 ON/OFF 编译
