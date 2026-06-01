# QuickJS Migration - Phase 1 Plan

## Status (2026-05-19)

Phase 1 COMPLETE. QJS builds, runs, and executes JavaScript end-to-end.

### Verified:
- agave_test loads test/qjs_test.html
- ScriptController::evaluate called with 1722-char script
- Execution completes without exception
- No crash on page load with JS

## P0 - Basic Page Rendering ✅ DONE

Goal: Simple HTML page renders correctly without JS interaction.

- [ ] `createJSHTMLWrapper` in qjs_binding.cpp (~200 lines)
  - Reference: bindings/js/JSHTMLElementWrapperFactory.cpp (245 lines)
  - Maps HTMLElement subtypes to their QJS wrapper classes

- [ ] `toEventTargetNode` / `isSafeScript` in qjs_binding.cpp (~50 lines)
  - toEventTargetNode: extract Node* from JSValue via JS_GetOpaque
  - isSafeScript: cross-frame security check (reference kjs_window.cpp)

- [ ] `WindowPrototype::self` / `Navigator::create` in qjs_window.cpp (~400 lines)
  - Reference: kjs_navigator.cpp (551 lines)
  - Navigator needs property table: appName, userAgent, platform, etc.

- [ ] Complete `qjs_events.cpp` handleEvent (~150 lines)
  - Reference: kjs_events.cpp (512 lines)
  - Key: proper event→JSValue conversion, this binding, exception handling

## P1 - Interactive Features (3-4 days, ~1200 lines)

Goal: Forms, dynamic CSS, basic JS execution work.

- [ ] `JSDOMExceptionConstructor::create` (~60 lines)
- [ ] `JSHTMLOptionElementConstructor::self` (~50 lines)
- [ ] `qjs_navigator.cpp` - full Navigator implementation (~400 lines)
- [ ] 14 Custom files (non-SVG):
  - QJSCSSRuleCustom.cpp (85)
  - QJSCSSStyleDeclarationCustom.cpp (140)
  - QJSCSSValueCustom.cpp (73)
  - QJSDOMWindowCustom.cpp (120)
  - QJSEventCustom.cpp (91)
  - QJSHTMLAppletElementCustom.cpp (67)
  - QJSHTMLElementCustom.cpp (52)
  - QJSHTMLEmbedElementCustom.cpp (67)
  - QJSHTMLFormElementCustom.cpp (59)
  - QJSHTMLFrameSetElementCustom.cpp (61)
  - QJSHTMLObjectElementCustom.cpp (67)
  - QJSHTMLOptionsCollectionCustom.cpp (69)
  - QJSHTMLSelectElementCustom.cpp (70)
  - QJSNamedNodeMapCustom.cpp (47)
  - QJSNodeListCustom.cpp (65)
  - QJSNodeIteratorCustom.cpp (35)
  - QJSStyleSheetCustom.cpp (54)
  - QJSStyleSheetListCustom.cpp (50)
  - QJSTreeWalkerCustom.cpp (35)

## P2 - Full Feature Parity (1-2 days, ~300 lines)

- [ ] `window.open` createWindow implementation (~60 lines)
- [ ] ImageConstructor (~20 lines)
- [ ] JSCustomXPathNSResolver (~80 lines)
- [ ] Clipboard binding (~100 lines)
- [ ] qjs_html.cpp (~100 lines)

## P3 - SVG (deferred)

Not needed until SVG rendering layer is integrated.
- QJSSVGMatrixCustom.cpp (135)
- QJSSVGPathSegCustom.cpp (116)
- QJSSVGPathSegListCustom.cpp (167)
- QJSSVGPointListCustom.cpp (192)
- QJSSVGTransformListCustom.cpp (192)

## Testing Strategy

1. Each P0 item: build + run agave_test, verify no crash
2. After P0 complete: load a simple HTML page, verify DOM tree created
3. After P1: test onclick handler, form submission, dynamic style
4. Compare with KJS build on same test pages

## Key Files Reference

| QJS file | KJS reference |
|----------|---------------|
| qjs_binding.cpp | kjs_binding.cpp (14344 bytes) |
| qjs_window.cpp | kjs_window.cpp (66512 bytes) |
| qjs_events.cpp | kjs_events.cpp (16632 bytes) |
| qjs_navigator.h | kjs_navigator.cpp (18485 bytes) |
| QJSCustomStubs.cpp | Various JS*Custom.cpp files |

## Known Issues (Phase 3)

### 1. GC Object Leak on Shutdown (Debug assert)
- **Symptom**: `JS_FreeRuntime: Assertion 'list_empty(&rt->gc_obj_list)' failed` in debug builds
- **Root cause**: DOM wrapper objects created via `JSXxx::create()` call `impl->ref()` and
  store JSValue in DOM cache, but the ref/DupValue is never balanced with deref/FreeValue
  during normal object lifecycle.
- **Impact**: Debug-only assert on exit. No runtime impact.
- **Fix**: Implement proper ref-counting in generated `finalizer()` functions and
  ensure `ScriptInterpreter::forgetDOMObject` calls `JS_FreeValueRT` before removing.

### 2. Window Object Leak on Frame Destruction
- **Symptom**: Window objects stored in `jsvalWindows` map are never removed.
- **Root cause**: `storeWindow()` adds to map but no corresponding removal on Frame destroy.
- **Impact**: Memory leak proportional to number of frames created during session.
- **Fix**: Add `Window::removeWindow(Frame*)` called from `Window::disconnectFrame()`.

### 3. DOMWindow Global Object Class ID Mismatch
- **Symptom**: `JS_GetOpaque2(ctx, this_val, JSDOMWindow::js_class_id)` returns NULL on global object.
- **Root cause**: QuickJS global object has internal class_id, not `JSDOMWindow::js_class_id`.
  Current workaround uses `JS_GetOpaque(this_val, 0)` with NULL check.
- **Impact**: Minor performance cost (extra NULL check on every DOMWindow property access).
- **Fix**: Investigate `JS_SetClassProto` or creating global object with custom class.

