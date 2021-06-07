#define IMPORT_MODULE evmod_glfw
#include <evol/meta/module_import.h>
#define IMPORT_MODULE evmod_script
#include <evol/meta/module_import.h>

#include <GLFW/glfw3.h>
#include <input/mod_input.h>
#include "script_api.h"

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
  ScriptType keyCodeSType = ScriptInterface->addType(ctx_h, "unsigned int", sizeof(KeyCode));
  ScriptType boolSType = ScriptInterface->getType(ctx_h, "bool");
  ScriptType voidSType = ScriptInterface->getType(ctx_h, "void");

  ScriptInterface->addFunction(ctx_h, ev_input_getkeydown_wrapper, "ev_input_getkeydown", boolSType, 1, &keyCodeSType);
  ScriptInterface->addFunction(ctx_h, ev_input_getkeyup_wrapper, "ev_input_getkeyup", boolSType, 1, &keyCodeSType);

  ScriptInterface->addFunction(ctx_h, ev_input_lockcursor_wrapper, "ev_input_lockcursor", voidSType, 0, NULL);
  ScriptInterface->addFunction(ctx_h, ev_input_unlockcursor_wrapper, "ev_input_unlockcursor", voidSType, 0, NULL);

  ScriptInterface->loadAPI(ctx_h, "subprojects/evmod_glfw/script_api.lua");
}
