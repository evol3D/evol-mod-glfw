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
  int keyStates[GLFW_KEY_LAST + 1];
  int prevKeyStates[GLFW_KEY_LAST + 1];
} InputData = {0};

DECLARE_EVENT_LISTENER(keyListener, (KeyEvent *event) {
  if(event->type == EVENT_TYPE(KeyPressedEvent)) {

    InputData.keyStates[((KeyPressedEvent*)event)->keyCode] = GLFW_PRESS;

  } else if(event->type == EVENT_TYPE(KeyReleasedEvent)) {

    InputData.keyStates[((KeyReleasedEvent*)event)->keyCode] = GLFW_RELEASE;

  }
})

void
ev_input_init()
{
  InputData.activeWindow = NULL;
  ev_log_debug("INPUT's View of EVENT_TYPE_KeyPressedEvent%llu", EVENT_TYPE_KeyPressedEvent);

  ACTIVATE_EVENT_LISTENER(keyListener, KeyEvent);

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
  return InputData.keyStates[key] == GLFW_PRESS;
}

EVMODAPI bool
ev_input_getkeyup(
    KeyCode key)
{
  return InputData.keyStates[key] == GLFW_RELEASE;
}

EVMODAPI bool
ev_input_getkeyjustpressed(
    KeyCode key)
{
  bool res = InputData.keyStates[key] == GLFW_PRESS &&
    InputData.prevKeyStates[key] == GLFW_RELEASE;
  return res;
}

EVMODAPI bool
ev_input_getkeyjustreleased(
    KeyCode key)
{
  return InputData.keyStates[key] == GLFW_RELEASE &&
    InputData.prevKeyStates[key] == GLFW_PRESS;
}

EVMODAPI void
ev_input_update()
{
  memcpy(InputData.prevKeyStates, InputData.keyStates, GLFW_KEY_LAST * sizeof(InputData.keyStates[0]));
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
