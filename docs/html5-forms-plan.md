# HTML5 表单控件实现计划（阶段 3 收尾）

记录阶段 3（基础 Web API）最后一项 —— HTML5 表单控件 —— 的范围、架构决策与
实现切分。

---

## 一、现状基线（已查证）

Agave 基于约 2007 年 WebKit，表单基础比预期完整：

**已有**：
- input 类型：text / password / checkbox / radio / submit / reset / file /
  hidden / image / button / search / range
- placeholder（含 `RenderTextControl::updatePlaceholder` 渲染）、maxlength、
  readonly、表单提交（`HTMLFormElement::prepareSubmit/submit`）
- `HTMLSelectElement` / `HTMLTextAreaElement` / `HTMLButtonElement`
- 控件外观经 `RenderTheme` / `RenderThemePS`（picasso 自绘）
- `RegularExpression`（platform/，可用于 pattern 验证）
- 结构性伪类框架（`CSSSelector` PseudoChecked/Enabled/Disabled +
  `CSSStyleSelector::checkOneSelector` 匹配）

**关键现状**：未知 input 类型（含所有 HTML5 类型 email/number/url/tel/date/color
等）当前**静默降级为 TEXT**。即字段能渲染、能输入、能提交，但无类型语义、无验证。

**缺失（HTML5）**：
1. HTML5 input 类型语义：email / url / tel / number / search（文本类）+
   date / color（需 picker）
2. 约束验证：required / pattern / min / max / step / ValidityState /
   checkValidity() / setCustomValidity() / willValidate
3. 约束相关伪类：:valid / :invalid / :required / :optional
4. 提交时验证（无效表单阻止提交 + invalid 事件）

---

## 二、架构决策：picker UI 委托宿主，引擎只提供接口

date / color 选择器**不由引擎实现 UI，也不由引擎自绘**，而是经回调委托给上层
"浏览器应用"，引擎只提供调用接口并接收返回值。

### 依据

1. **既有先例**：`FileChooser::openFileChooser`（webview/FileChooserMC.cpp）正是
   此模式 —— 通过 `cb_choose_file` 回调让宿主弹出文件对话框，引擎不实现对话框 UI，
   只发起调用、拿回文件名、写回 DOM。
2. **UI 风格归属上层**：picker 外观应与宿主系统 / 浏览器应用的 UI 风格（主题、
   字体、动效、平台 HIG）一致；引擎自绘会与系统割裂。
3. **复用系统级实现**：宿主平台若有原生 date / color picker（如 RTOS / Android），
   可直接调用，获得原生体验。
4. **关注点分离**：引擎负责"何时需要选择 + 当前值 + 写回 value + 派发
   input/change 事件"；"选择器长什么样、怎么交互"归宿主。

### 回调接口设计（仿 cb_choose_file）

在 `MC_CALLBACK_INFO`（include/macross.h）与 `WebCallback`（webview/）新增：

```c
/* 返回 TRUE 表示用户确认选择，value 缓冲区填入结果（UTF-8）；FALSE 表示取消。 */
MC_BOOL (*cb_choose_date)(char* value, unsigned int buffer_size,
                          const char* initial_value);
MC_BOOL (*cb_choose_color)(char* value, unsigned int buffer_size,
                           const char* initial_value);
```

- date 返回值格式：`YYYY-MM-DD`（HTML date 控件的 value 格式）
- color 返回值格式：`#rrggbb`（HTML color 控件的 value 格式）
- 引擎侧：date / color input 被激活（点击）时，以当前 value 为 initial 调用回调；
  回调返回 TRUE 则 `setValue(result)` 并派发 input + change 事件。
- **回调未注册（如 headless）时**：降级为普通文本输入框，用户仍可手动输入，
  不阻塞功能 —— 这使该特性可单独验证。

---

## 三、实现切分

| # | 任务 | 说明 |
|---|------|------|
| 1 | 文本类 input 类型 | email/url/tel/number/search 加入 InputType 枚举 + setInputType 映射；渲染为文本框（number 可后续加步进 UI，先按文本） |
| 2 | 约束验证核心 | required / pattern / min / max / step + 各 validity 状态（valueMissing / typeMismatch / patternMismatch / rangeUnderflow / rangeOverflow / stepMismatch / tooLong / customError） |
| 3 | ValidityState + JS API | checkValidity() / setCustomValidity() / validity / willValidate / validationMessage |
| 4 | 提交时验证 | prepareSubmit 中校验，无效则阻止提交并派发 invalid 事件 |
| 5 | 约束伪类 | :valid / :invalid / :required / :optional |
| 6 | date / color | 类型识别 + cb_choose_date / cb_choose_color 接口 + 激活触发 / 写回 / 派发 change |

### 类型校验规则（typeMismatch）

- email：基本邮箱格式正则（local@domain）
- url：含 scheme 的绝对 URL
- tel：不校验格式（HTML 规范规定 tel 不做格式约束）
- number：可解析为浮点数
- date：`YYYY-MM-DD` 格式 + 合法日期
- color：`#rrggbb` 格式

---

## 四、裁剪宏

`ENABLE_HTML5_FORMS`（CMakeLists `OPT_HTML5_FORMS` 默认 ON + mconfig），与
localStorage/Fetch/WebSocket 同一裁剪模式。关闭时：HTML5 类型回退降级为 TEXT、
验证 API 退化为"始终有效"、伪类不匹配约束态。保证可裁剪以控 codesize。

---

## 五、退出标准

- email/url/number/tel/search 被正确识别，渲染为可输入文本框
- required 空字段、pattern 不匹配、number 越界 → validity 反映、checkValidity()
  返回 false、提交被阻止、:invalid 命中
- date/color：点击触发宿主回调（注册时），返回值写回并派发 change；未注册时降级
  文本输入
- 单元测试 + benchmark 覆盖；全套回归通过；三维审查通过；可裁剪宏 ON/OFF 均编译
