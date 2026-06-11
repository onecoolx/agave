# 安全待办（Security TODO）

记录已发现、待后续修正的安全 / 信息泄露问题。每项注明发现日期、风险等级、
现状与建议修正方式。

---

## SEC-001：生成代码内嵌原始开发者绝对路径

- **发现日期**：2026-06-10（阶段 1 收尾安全审计时）
- **风险等级**：低（信息泄露，非可利用漏洞）
- **状态**：部分修正（CSSGrammar.cpp/.h 已随里程碑 3d 重新生成而消除；其余生成
  文件 tokenizer.cpp、XPathGrammar.cpp 等仍待处理）

### 更新

- 2026-06-11：里程碑 3d 用系统 bison 3.5.1 重新生成了 `CSSGrammar.cpp/.h`
  （buildQJS/buildKJS），其 `#line` 指令现为相对路径 `"CSSGrammar.y"`，原始
  开发者绝对路径已消除。说明"在生成目录内用相对路径调用生成器"这一修正方式可行。
  其余生成文件可按同法逐个处理。

### 问题描述

`src/webcore/buildQJS/` 与 `src/webcore/buildKJS/` 下由 yacc/flex/gperf 生成的
代码文件，内嵌了原始开发环境的绝对路径，含项目原始代号与开发者用户名，例如：

```
#line 1 "/home/<user>/<original-project-codename>/source/webcore/css/CSSGrammar.y"
```

涉及文件（生成产物）：
- `CSSGrammar.cpp`
- `tokenizer.cpp`
- `XPathGrammar.cpp`
- 其他 gperf/flex/yacc 生成文件

### 成因

这些路径来自构建时 yacc/flex 的 `#line` 指令，以及 gperf 命令行回显，记录了
生成时的源文件绝对路径。生成器默认使用调用时的绝对路径。

### 影响

- 泄露原始项目代号与开发者用户名
- 仅为信息泄露，不可被远程利用；不影响运行时安全

### 建议修正

1. 生成时使用相对路径调用生成器（在生成目录内用相对路径引用 `.y`/`.flex`/
   `.gperf` 源），使 `#line` 记录相对路径。
2. 或在生成后用脚本将绝对路径前缀统一改写为中性相对路径（如 `webcore/css/...`）。
3. 在 CI/构建流程中固化该处理，避免重新生成时回归。
4. 一次性清理仓库中现存的生成文件路径。

### 备注

超出阶段 1（现代 CSS 布局）范围，作为独立安全清理项，建议在阶段间隙处理。
