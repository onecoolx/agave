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

---

## SEC-002：QJS 绑定层非 ASCII 字符串往返损坏

- **发现日期**：2026-06-12（localStorage 里程碑审查时）
- **风险等级**：低（功能正确性，非可利用漏洞）
- **状态**：已修正（2026-06-12）

### 修正

根因：bindings/qjs 的 valueToString 系列用 `String(const char*)` 构造，把 QuickJS
返回的 UTF-8 字节按 Latin-1 逐字节展开。改用 `String::fromUTF8()` 正确解码。
（jsString 反向用 utf8()+JS_NewString 本就正确。）已加 SqliteStorageAreaTest.
UnicodeRoundTrip 回归测试。

### 问题描述

JS 字符串 ↔ WebCore::String 经 QJS 绑定（valueToString / jsString 等）往返时，
非 ASCII（多字节 UTF-8）字符被错误展开：例如 13 个 UTF-16 码元的字符串往返后变成
30。表现为 UTF-8 字节被当作 Latin-1 码元（双重/错误编码）。

### 影响范围

**全局绑定层问题，非某一 API 特有。** 实测 setAttribute/getAttribute、
localStorage、sessionStorage 均有相同症状（getlen=30 vlen=13）。ASCII 内容不受影响。

### 备注

非 localStorage 里程碑引入（既有缺陷）。修正点在 bindings/qjs 的字符串转换
（valueToString / jsString 应按 UTF-16/UTF-8 正确转换，而非 Latin-1 直通）。
作为独立的绑定层修正项后续处理。

---

## SEC-003：HTTPS/WSS 全局禁用 TLS 证书校验

- **发现日期**：2026-06-12（WebSocket 里程碑代码审查时）
- **风险等级**：高（中间人攻击，可利用）
- **状态**：第一步已完成（2026-06-12，条件校验）；证书错误 UI 为 future work

### 更新

- 2026-06-12：完成第一步。`initJob` 不再无条件关闭校验，改为调用
  `configureTLSVerification()`：优先用宿主注入的 `caPath()`，否则探测系统 CA
  bundle（`/etc/ssl/certs/ca-certificates.crt` 等已知路径，用 `CURLOPT_CAINFO`
  指向单一 PEM 文件以适配 mbedTLS），找到 CA 即开启 `VERIFYPEER=1/VERIFYHOST=2`；
  仅在无任何 CA 时保留旧的关闭行为。实测：有效证书 HTTPS 正常、自签名证书被拒、
  HTTP 明文正常、fetch/WebSocket 不受影响、全套回归 720 全过。
  注意：libcurl 公共头不暴露编译期 `CURL_CA_BUNDLE` 宏，故改为直接探测已知系统
  路径。

### 问题描述

`ResourceHandleManager::initJob()`（platform/network/curl/ResourceHandleManagerCurl.cpp）
对每个 curl 句柄无条件设置：

```cpp
curl_easy_setopt(d->m_handle, CURLOPT_SSL_VERIFYPEER, 0);
curl_easy_setopt(d->m_handle, CURLOPT_SSL_VERIFYHOST, 0);
```

这关闭了所有 HTTPS 连接的对端证书与主机名校验。该路径是**所有** HTTPS 请求的
公共入口——fetch、XMLHttpRequest、页面资源加载、wss:// WebSocket 全部受影响。
（WebSocket 自身的 doConnect 不碰 verify 选项、用 curl 安全默认，但 ResourceHandle
路径上的请求仍受此影响。）

### 影响

- 任何中间人都可冒充 HTTPS/WSS 服务器、解密或篡改流量，受害者无任何提示
- 影响面为全部安全传输请求

### 成因（设计背景）

源自约 2007 年代嵌入式浏览器的妥协：目标设备未必有标准 CA 目录，故由宿主程序
经 `macross_set_certificate_dir()` → `setCaPath()` 运行时注入证书路径；未注入时
直接关闭校验以"能连上"。`initJob` 中仅当 `caPath()` 非空才设 `CURLOPT_CAPATH`，
但校验是无条件关闭的。

### 技术难点

1. **TLS 后端为 mbedTLS**（非 OpenSSL）。mbedTLS 用 `mbedtls_x509_crt_parse_path()`
   加载 CAPATH 目录，仅识别 PEM/DER 文件；`/etc/ssl/certs` 内多为 OpenSSL hash 格式
   符号链接（`xxxx.0`），按目录解析慢且可能不完整。更可靠的是用 `CURLOPT_CAINFO`
   指向单一 bundle 文件（如 `/etc/ssl/certs/ca-certificates.crt`）。当前用 CAPATH，
   方向欠妥。
2. **无证书错误 UI/回调**。引擎没有证书校验失败时的用户提示或"继续"机制。开启校验
   后，自签名/过期证书站点会静默连接失败，用户无从得知。真实浏览器会提示"连接不
   安全"。补此机制需触及 ChromeClient/回调层，工作量较大。
3. **公共路径、headless 难充分验证**。修改影响全部 HTTPS/WSS 请求，而 headless 环境
   难以覆盖真实联网与各种证书场景，公共安全路径回归风险高。

### 修正方案（分步）

**第一步（本次）：条件校验，而非无条件关闭。**
- `caPath()` 已设置 → 用其作为 CA 来源并开启 `VERIFYPEER=1 / VERIFYHOST=2`
- 否则回退到 curl 内建默认 CA（`CURLOPT_CAINFO` 指向系统 bundle）并开启校验
- 仅在确实找不到任何 CA 时才降级为关闭（保留嵌入式无 CA 环境的原有行为）

效果：有系统 CA 的环境（如桌面 Linux）默认即安全；嵌入式无 CA 环境行为不变。

**第二步（future work）：证书错误用户决策机制。**
校验失败时向上层（ChromeClient）抛出可交互的证书错误，由用户选择是否继续。单列评估。

### 备注

非 WebSocket 里程碑引入（既有缺陷）。第一步在不改变嵌入式无 CA 环境行为的前提下，
为有 CA 的环境恢复安全默认，且需充分回归 HTTP/HTTPS/fetch/XHR/WebSocket。
