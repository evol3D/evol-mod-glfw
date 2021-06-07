#include <evol/evol.h>
#include <GLFW/glfw3.h>

#define IMPORT_MODULE evmod_glfw
#include <evol/meta/module_import.h>
#define IMPORT_MODULE evmod_script
#include <evol/meta/module_import.h>

#include <input/mod_input.h>
#include "script_api/script_api.h"

struct ev_InputData {
  WindowHandle activeWindow;
} InputData;

void
ev_input_init()
{
  InputData.activeWindow = NULL;
  evolmodule_t script_mod = evol_loadmodule_weak("script");
  if(script_mod) {
    imports(script_mod, (ScriptInterface));
    ScriptInterface->registerAPILoadFn(ev_inputmod_scriptapi_loader);
  }
}

EVMODAPI bool
ev_input_getkeydown(
    KeyCode key)
{
  int state = glfwGetKey(InputData.activeWindow, key);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

EVMODAPI bool
ev_input_getkeyup(
    KeyCode key)
{
  int state = glfwGetKey(InputData.activeWindow, key);
  return state == GLFW_RELEASE;
}

EVMODAPI void
ev_input_setactivewindow(
    WindowHandle handle)
{
  // TODO add checking for valid window
  InputData.activeWindow = handle;
}

EVMODAPI void
ev_input_lockcursor()
{
  glfwSetInputMode(InputData.activeWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

EVMODAPI void
ev_input_unlockcursor()
{
  glfwSetInputMode(InputData.activeWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void
ev_input_deinit()
{
}
