# Picasso Bug Report: ps_wide_text_out_length 读取未初始化值

> 报告日期：2026-06-04
> Picasso 版本：2.8.0（proj/picasso/src/picasso）
> 发现工具：Valgrind memcheck（--track-origins=yes）
> 发现场景：Agave 浏览器引擎用 picasso 渲染网页文本（headless 测试 dom_test.html）

## 一、问题摘要

`ps_wide_text_out_length()` 在遍历传入的宽字符文本时，用
`while (*p && len)` 作为循环条件——**把输入当作 NUL 结尾字符串**。
但实际调用方（WebKit/WebCore 字体绘制）传入的是**按 `len` 计数的 glyph 数组，
并非 NUL 结尾**。这导致 `*p` 的解引用可能读到调用方缓冲区中**未被初始化的部分**，
Valgrind 报告 "Conditional jump or move depends on uninitialised value(s)"。

## 二、Valgrind 证据

```
==xxxxx== Conditional jump or move depends on uninitialised value(s)
==xxxxx==    at ps_wide_text_out_length (picasso_font_api.cpp:347)
==xxxxx==    by WebCore::Font::drawGlyphs (FontPS.cpp:45)
==xxxxx==    by WebCore::Font::drawGlyphBuffer (Font.cpp:669)
==xxxxx==    by WebCore::Font::drawSimpleText (Font.cpp:643)
==xxxxx==    ...
==xxxxx==  Uninitialised value was created by a stack allocation
==xxxxx==    at WebCore::Font::drawSimpleText (Font.cpp:612)
```

关键：未初始化值的**来源**是调用方 `drawSimpleText` 栈上的 `GlyphBuffer`
（`Vector<Glyph, 2048>` 内联栈缓冲），**使用点**是 picasso 的 `while (*p ...)`。

## 三、根因分析

### picasso 侧代码（picasso_font_api.cpp:329-368）

```c
void PICAPI ps_wide_text_out_length(ps_context* ctx, float x, float y,
                                    const ps_uchar16* text, unsigned int len)
{
    ...
    const ps_uchar16* p = text;
    while (*p && len) {              // ← line 347: 同时用 NUL 终止符和 len 计数
        ps_uchar16 c = *p;
        const picasso::glyph* glyph = ctx->fonts->current_font()->get_glyph(c);
        if (glyph) {
            if (ctx->font_kerning) { ... }     // line 352
            ...
        }
        len--;
        p++;
    }
    ...
}
```

`while (*p && len)`：C 的 `&&` 先求值左操作数 `*p`。循环用 `len` 计数递减，
本意是处理 `len` 个字符。但**额外**用 `*p`（NUL 终止）作为终止条件，这隐含假设
`text` 是 NUL 结尾的 C 宽字符串。

### 调用方语义（WebCore/FontPS.cpp:45）

```cpp
const GlyphBufferGlyph* glyphs = (GlyphBufferGlyph*)glyphBuffer.glyphs(from);
ps_wide_text_out_length(gc, point.x(), point.y()-font->ascent(), glyphs, numGlyphs);
```

`glyphs` 是 `GlyphBuffer` 内 `Vector<Glyph, 2048>` 的数据指针，
**长度由 `numGlyphs` 精确界定，数组内容不保证以 0 结尾，数组之后的内联缓冲未初始化**。

### 冲突

- 调用方契约：`text` 是 `len` 个元素的数组（计数语义）
- picasso 实现：把 `text` 当 NUL 结尾字符串（`*p` 终止）+ len 双重条件

当 glyph 数组内容恰好不含 0、且 `len` 尚未递减到 0 时循环正常；但 Valgrind 表明
存在 `*p` 读到未初始化内存的路径——**因为 `*p` 在每次迭代都被解引用，包括依赖
NUL 终止假设去探测数组边界时**，触碰到调用方未初始化的栈缓冲。

即使运行时"碰巧"没崩溃，这也是**未定义行为**：对计数型数组施加 NUL 终止假设。

## 四、建议修复

**纯计数遍历，去掉 NUL 终止假设**——`len` 已精确给出元素个数：

```c
const ps_uchar16* p = text;
while (len) {                    // 仅用 len 计数，不解引用探测终止符
    ps_uchar16 c = *p;
    const picasso::glyph* glyph = ctx->fonts->current_font()->get_glyph(c);
    if (glyph) {
        if (ctx->font_kerning) { ... }
        ...
    }
    len--;
    p++;
}
```

改动：`while (*p && len)` → `while (len)`。

理由：
1. `len` 是 API 的显式长度参数，是唯一可靠的边界
2. 计数语义下不应假设 NUL 结尾——glyph 值 0 可能是合法字形索引，
   用 `*p` 终止还会**错误地提前截断**含 0 字形的文本
3. 消除对数组边界外内存的解引用

### 同类位置

`ps_text_out_length()`（picasso_font_api.cpp:288，UTF-8 版本）建议一并检查是否
有相同的 `while (*p && len)` 模式。`ps_draw_text()`（line 370）及其它接收
`(text, len)` 的接口也应统一为纯计数遍历。

## 五、影响评估

- **严重度**：中。当前多为"读未初始化值"（UB + 潜在提前截断），实际崩溃罕见，
  但属确定性 UB，在不同编译器/内存布局下行为不可预测。
- **触发条件**：渲染任意文本（dom_test.html 等含可见文本的页面稳定复现）。
- **副作用**：含值为 0 的字形索引的文本会被错误截断（功能 bug，非仅内存问题）。

## 六、复现方法

```
# Agave 引擎 + picasso，headless 渲染含文本的页面
valgrind --tool=memcheck --track-origins=yes \
    ./headless_test dom_test.html 5000 2>&1 | grep -A12 "uninitialised"
```

任何含可见文本的页面均可复现。
