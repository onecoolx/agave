# picasso 2.9.0 升级：字体 metrics 回归根因与修复

> 日期：2026-06-14
> 现象：升级到 picasso 2.9.0 后，所有文本的字体度量（ascent/descent/lineSpacing/
>       unitsEM）返回 0。headless 渲染文本 offsetHeight=0；20 个 font 单元测试失败。

## 根因

picasso 2.9.0 的构建按特性开关选择字体后端：
- `OPT_FREE_TYPE2`（CMakeLists.txt 默认 **OFF**）—— 控制是否编译 FreeType2 字体适配器。
- `OPT_FONT_CONFIG`（默认 **OFF**）—— 控制是否用 fontconfig 系统字体库。
- 二者仅在 picasso **自建单元/性能测试**时自动置 ON（CMakeLists.txt:76-88），普通库
  构建（agave 以 TESTS=OFF 方式构建）保持 OFF。

当 `OPT_FREE_TYPE2=OFF` 时，picasso 编译 **dummy 字体适配器**
（font_adapter_dummy.cpp），`platform_font_init` 直接返回 true 但不加载任何字体，
所有 metrics 为 0。

agave 的 `3rdparty/picasso.cmake` 此前未传 `-DOPT_FREE_TYPE2=ON`，故 2.9.0 构建出的
libpicasso **不含 FreeType**（`nm` 查无 FT_New_Face 符号，`ldd` 无 freetype 依赖，
pconfig.h 中 `ENABLE_FREE_TYPE2` 未定义）→ dummy 适配器 → 字体度量全 0。

### 为什么 2.8.0 没有这个问题

agave 对 picasso-2.8.0 打了 patch（packages/patchs/picasso-2.8.0/），其中
`defines.cmake` 含 `add_compile_definitions(ENABLE_FREE_TYPE2=1)`，`src.cmake` 含
`find_package(Freetype/Fontconfig REQUIRED)` + 链接系统 freetype/fontconfig。即 2.8.0
是**带 patch 强制启用 FreeType** 才正常。2.9.0 走标准 ExternalProject 构建、未打该
patch，于是丢失了 FreeType 启用。

**结论：不是 picasso 2.9.0 功能缺失，而是 agave 的 2.9.0 构建未启用 FreeType 后端。**
2.9.0 的字体度量代码与 2.8.0 patch 完全一致（FT 度量提取逻辑逐行相同）。

## 修复

在 `3rdparty/picasso.cmake` 的 picasso ExternalProject CMAKE_ARGS 增加：
```
-DOPT_FREE_TYPE2=ON    # 启用 FreeType2 字体后端（否则用 dummy 适配器，度量全 0）
-DOPT_FONT_CONFIG=OFF  # 不用系统 fontconfig；改用 picasso 的 font_config.cfg +
                       # 自带字体，使嵌入式/可信内容的 CJK 渲染与宿主系统已装字体
                       # 无关、可确定性复现
```

**为何 FONT_CONFIG 关闭**：fontconfig 走宿主系统字体库，若系统未安装中文字体或未配置，
中文会缺字/豆腐块。改用 `font_config.cfg` 指向 picasso 自带的中文字体
（ZCOOLXiaoWei-Regular.ttf），CJK 渲染不依赖宿主环境、可控且可复现——符合嵌入式
可信内容定位。（系统仍需 freetype dev 供 FreeType 后端链接；不需要 fontconfig。）

清理 picasso 构建产物后重新构建，验证：
- pconfig.h: `ENABLE_FREE_TYPE2=1`；`ENABLE_FONT_CONFIG` 未定义。
- libpicasso 动态依赖 libfreetype.so.6（不依赖 fontconfig）。
- headless 文本 offsetHeight 非 0；中文 "中文"（24px，inline-block）宽 115、高 24，
  字形从自带 ZCOOLXiaoWei 正确渲染。
- 802 单元测试全过；渲染 benchmark（gradient/transform/filter/shadow/real_layouts/
  transition/animation）全 ALL PASS。

## 顺带的测试健壮性改进

调查中修了 font 测试的预存脆弱性（与 picasso 无关）：
- `unit_tests/test.cpp` `Test_Init()` 增加幂等的 `WebCore::AtomicString::init()`：font
  测试不创建 Frame，原先依赖其它测试套件先初始化 AtomicString，单独跑会崩。
- `Test_Shutdown()` 改为 no-op：真实用法与 headless 都是进程内单次 init，不在套件间
  反复 ps_shutdown/ps_initialize。
- `font_data_test.cpp` 的 `lineGap() > 0` 放宽为 `>= 0`：leading 为 0 对许多字体合法，
  取决于平台选中的回退字体。
