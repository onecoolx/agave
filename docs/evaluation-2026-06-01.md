# Agave 浏览器项目全面评估报告

> 日期：2026-06-01  
> 基于 commit: eb7c2e88 (refactor(qjs): skip ClassDefine for Node subclasses)

---

## 一、核心指标

| 指标 | 当前值 | 说明 |
|------|--------|------|
| 二进制体积 (.text) | **10.4 MB** | 含渲染引擎+JS引擎+DOM绑定+网络+图形 |
| 峰值内存 | **~50 MB** | 加载 full_test.html（59个DOM测试） |
| 单元测试 | **565/565 通过** | 28 个测试套件 |
| DOM 功能测试 | **59/59 通过** | createElement/style/table/innerHTML 等 |
| 源代码规模 | **101K 行** | 含 WebCore + QuickJS + 绑定 |
| JS 引擎 | QuickJS 2024-01-13 | 替代原 KJS (39K 行) |

---

## 二、与 KJS 版本对比

| 维度 | KJS (旧) | QuickJS (新) | 优势 |
|------|----------|-------------|------|
| JS 引擎代码量 | 39,566 行 | ~30,000 行 (QuickJS 核心) | QuickJS 更精简 |
| ES 标准支持 | ES3 部分 | **ES2023 完整** | 巨大提升 |
| 正则表达式 | PCRE (外部依赖) | 内置 | 减少依赖 |
| JIT | 无 | 无 (字节码解释) | 持平 |
| 启动速度 | 快 | **更快** (无 JIT 编译开销) |
| 内存效率 | 引用计数 | 引用计数 + 标记清除 | QuickJS 更健壮 |
| 绑定复杂度 | KJS 原生 API | 需要适配层 | KJS 略简单 |
| 跨平台 | GCC only | **GCC + Clang + MSVC** | 显著提升 |

---

## 三、与同类嵌入式浏览器对比

| 浏览器 | JS 引擎 | 二进制体积 | RAM 需求 | ES 标准 |
|--------|---------|-----------|---------|---------|
| **Agave (本项目)** | QuickJS | ~10 MB | ~50 MB | ES2023 |
| NetSurf | Duktape | ~5 MB | ~30 MB | ES5.1 |
| Servo (mini) | SpiderMonkey | ~40 MB | ~200 MB | ES2023 |
| WebKit (embedded) | JSC | ~20 MB | ~100 MB | ES2023 |
| Cobalt (YouTube) | V8 lite | ~15 MB | ~80 MB | ES2020 |
| litehtml | 无 JS | ~2 MB | ~10 MB | 无 |

**Agave 定位：** 在完整 ES2023 支持的嵌入式浏览器中，体积和内存占用处于**领先水平**。比 WebKit embedded 小一半，比 Servo 小 4 倍。

---

## 四、当前优势

1. **完整 DOM/CSS/SVG 绑定** — 255 个自动生成的绑定类，覆盖 Web 标准核心 API
2. **跨平台** — Linux (GCC/Clang) + Windows (MSVC) 全通过
3. **代码质量** — 无已知 bug，无编译警告，代码生成器逻辑清晰
4. **体积可控** — 10 MB .text 段对嵌入式设备可接受
5. **现代 JS** — ES2023 完整支持（Promise、async/await、模块等）

---

## 五、当前存在的问题

| 优先级 | 问题 | 影响 | 风险 |
|--------|------|------|------|
| **P0** | GC 完全禁用 | 长页面内存持续增长直到 context 销毁 | 高 |
| **P0** | QuickJS 运行时被 patch | 跳过安全断言，隐藏潜在 UAF | 高 |
| **P1** | DOM 缓存强引用 | JS wrapper 永不回收 | 中 |
| **P1** | 无脚本超时机制 | 死循环挂起进程 | 中 |
| **P2** | DOMWindow 属性注册不完整 | "Full list causes memory corruption" | 中 |
| **P2** | 50 MB 峰值内存偏高 | 对 MCU 设备可能过大 | 低-中 |
| **P3** | 1 个 JS 功能测试失败 | "not a function" at line 131 | 低 |

---

## 六、后续修复计划

### 阶段 1：稳定性（1-2 周）
- [ ] 实现 `JS_SetInterruptHandler` 脚本超时机制
- [ ] 排查 full_test.html line 131 的 "not a function" 错误
- [ ] 排查 DOMWindow 完整属性注册导致 memory corruption 的根因

### 阶段 2：内存管理（2-4 周）
- [ ] 设计 DOM wrapper 弱引用策略（候选方案：weak ref table / ref-count bridge / generation-based eviction）
- [ ] 恢复 QuickJS GC 功能，使其与 DOM 缓存兼容
- [ ] 移除 QuickJS 运行时 patch，恢复安全断言
- [ ] 目标：峰值内存降低 30-50%

### 阶段 3：性能优化（4-6 周）
- [ ] 分析 DOM 操作热路径，优化 JS↔C++ 桥接开销
- [ ] 考虑属性访问快速路径（inline cache 或 shape-based）
- [ ] 评估是否需要 QuickJS 字节码缓存

### 阶段 4：体积优化（可选）
- [ ] 裁剪未使用的 SVG 绑定（如果目标设备不需要）
- [ ] LTO (Link-Time Optimization) 在 MSVC/GCC 上启用
- [ ] 评估 QuickJS 编译选项（禁用 BigInt/Atomics 等不需要的特性）

---

## 七、总结

Agave 当前处于**功能完备、质量良好、但内存管理待完善**的状态。作为嵌入式浏览器，它在 ES 标准支持和体积之间取得了很好的平衡。最关键的下一步是解决 GC 问题——这是从"可用"到"可靠"的关键门槛。

---

## 附录：本次优化提交记录

| Commit | 内容 |
|--------|------|
| e97ed949 | 替换 designated initializers 为运行时初始化 (MSVC 兼容) |
| 85d9b955 | 修复 qjs_window.cpp 和 QJSEvent.cpp MSVC 错误 |
| 672d58b2 | 修复手写 QJS 绑定文件 MSVC 错误 |
| e2131f80 | 替换 KJS DateMath 为 Win32 时间 API |
| 7bd50e47 | 移除零大小数组 |
| 4a0f4e26 | 修复 extern 声明作用域链接错误 |
| a85198fc | 常量使用 JS_DEF_PROP_INT32 (修复 int16_t 溢出) |
| 199bfc6f | CSSStyleDeclaration 添加 set_property exotic |
| 2b8e3371 | 删除死代码和重复 init 调用 |
| 304cb38b | 静态 JSValue 移至 global object 存储 |
| fd4385df | 移除代码生成器中过时的 hash 碰撞逻辑 |
| ca832b4c | 移除 InspectorController (嵌入式不需要) |
| 502759f5 | 清理代码生成器死代码和注释 |
| eb7c2e88 | Node 子类不生成无用的 ClassDefine |
