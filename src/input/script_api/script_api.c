#define IMPORT_MODULE evmod_glfw
#include <evol/meta/module_import.h>
#define IMPORT_MODULE evmod_script
#include <evol/meta/module_import.h>

#include <GLFW/glfw3.h>
#include <input/mod_input.h>
#include "script_api.h"

void
ev_input_getmousebuttondown_wrapper(
    bool *out,
    MouseButtonID *button)
{
  *out = ev_input_getmousebuttondown(*button);
}

void
ev_input_getmousebuttonup_wrapper(
    bool *out,
    MouseButtonID *button)
{
  *out = ev_input_getmousebuttonup(*button);
}

void
ev_input_getkeydown_wrapper(
    bool *out,
    KeyCode *key)
{
  *out = ev_input_getkeydown(*key);
}

void
ev_input_getkeyup_wrapper(
    bool *out,
    KeyCode *key)
{
  *out = ev_input_getkeyup(*key);
}

void
ev_input_getmousepos_wrapper(
    MousePosition *out)
{
  *out = ev_input_getmousepos();
}

void
ev_input_getdeltamousepos_wrapper(
    MousePosition *out)
{
  *out = ev_input_getdeltamousepos();
}

void
ev_input_getmousebuttonjustpressed_wrapper(
    bool *out,
    MouseButtonID *button)
{
  *out = ev_input_getmousebuttonjustpressed(*button);
}

void
ev_input_getmousebuttonjustreleased_wrapper(
    bool *out,
    MouseButtonID *button)
{
  *out = ev_input_getmousebuttonjustreleased(*button);
}

void
ev_input_getkeyjustpressed_wrapper(
    bool *out,
    KeyCode *key)
{
  *out = ev_input_getkeyjustpressed(*key);
}

void
ev_input_getkeyjustreleased_wrapper(
    bool *out,
    KeyCode *key)
{
  *out = ev_input_getkeyjustreleased(*key);
}

void
ev_input_lockcursor_wrapper()
{
  ev_input_lockcursor();
}

void
ev_input_unlockcursor_wrapper()
{
  ev_input_unlockcursor();
}

void
ev_inputmod_scriptapi_loader(
    EVNS_ScriptInterface *ScriptInterface,
    ScriptContextHandle ctx_h)
{
  ScriptType mouseButtonSType = ScriptInterface->addType(ctx_h, "unsigned int", sizeof(MouseButtonID));
  ScriptType keyCodeSType = ScriptInterface->addType(ctx_h, "unsigned int", sizeof(KeyCode));
  ScriptType boolSType = ScriptInterface->getType(ctx_h, "bool");
  ScriptType voidSType = ScriptInterface->getType(ctx_h, "void");
  ScriptType doubleSType = ScriptInterface->getType(ctx_h, "double");

  ScriptType mousePosSType = ScriptInterface->addStruct(ctx_h, "MousePosition", sizeof(MousePosition), 2, (ScriptStructMember[]) {
      {"x", doubleSType, offsetof(MousePosition, x)},
      {"y", doubleSType, offsetof(MousePosition, y)},
  });

  ScriptInterface->addFunction(ctx_h, ev_input_getmousepos_wrapper, "ev_input_getmousepos", mousePosSType, 0, NULL);
  ScriptInterface->addFunction(ctx_h, ev_input_getdeltamousepos_wrapper, "ev_input_getdeltamousepos", mousePosSType, 0, NULL);

  ScriptInterface->addFunction(ctx_h, ev_input_getkeydown_wrapper, "ev_input_getkeydown", boolSType, 1, &keyCodeSType);
  ScriptInterface->addFunction(ctx_h, ev_input_getkeyup_wrapper, "ev_input_getkeyup", boolSType, 1, &keyCodeSType);
  ScriptInterface->addFunction(ctx_h, ev_input_getkeyjustpressed_wrapper, "ev_input_getkeyjustpressed", boolSType, 1, &keyCodeSType);
  ScriptInterface->addFunction(ctx_h, ev_input_getkeyjustreleased_wrapper, "ev_input_getkeyjustreleased", boolSType, 1, &keyCodeSType);

  ScriptInterface->addFunction(ctx_h, ev_input_getmousebuttondown_wrapper, "ev_input_getmousebuttondown", boolSType, 1, &mouseButtonSType);
  ScriptInterface->addFunction(ctx_h, ev_input_getmousebuttonup_wrapper, "ev_input_getmousebuttonup", boolSType, 1, &mouseButtonSType);
  ScriptInterface->addFunction(ctx_h, ev_input_getmousebuttonjustpressed_wrapper, "ev_input_getmousebuttonjustpressed", boolSType, 1, &mouseButtonSType);
  ScriptInterface->addFunction(ctx_h, ev_input_getmousebuttonjustreleased_wrapper, "ev_input_getmousebuttonjustreleased", boolSType, 1, &mouseButtonSType);

  ScriptInterface->addFunction(ctx_h, ev_input_lockcursor_wrapper, "ev_input_lockcursor", voidSType, 0, NULL);
  ScriptInterface->addFunction(ctx_h, ev_input_unlockcursor_wrapper, "ev_input_unlockcursor", voidSType, 0, NULL);

  ScriptInterface->loadAPI(ctx_h, "subprojects/evmod_glfw/script_api.lua");
}
