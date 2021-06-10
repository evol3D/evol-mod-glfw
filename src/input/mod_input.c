#include <evol/evol.h>
#include <GLFW/glfw3.h>

#define IMPORT_MODULE evmod_glfw
#include <evol/meta/module_import.h>
#define IMPORT_MODULE evmod_script
#include <evol/meta/module_import.h>

#include <input/mod_input.h>
#include "script_api/script_api.h"

#define MAX_MOUSEBUTTON_COUNT (16)
#define MAX_KEY_COUNT (GLFW_KEY_LAST + 1)

struct ev_InputData {
  WindowHandle activeWindow;
  int keyStates[MAX_KEY_COUNT];
  int prevKeyStates[MAX_KEY_COUNT];

  MousePosition prevMousePos;
  MousePosition currMousePos;
  MousePosition deltaMousePos;
  int mouseButtonStates[MAX_MOUSEBUTTON_COUNT];
  int prevmouseButtonStates[MAX_MOUSEBUTTON_COUNT];
} InputData = {0};

DECLARE_EVENT_LISTENER(keyListener, (KeyEvent *event) {
  if(event->type == EVENT_TYPE(KeyPressedEvent)) {

    InputData.keyStates[((KeyPressedEvent*)event)->keyCode] = GLFW_PRESS;

  } else if(event->type == EVENT_TYPE(KeyReleasedEvent)) {

    InputData.keyStates[((KeyReleasedEvent*)event)->keyCode] = GLFW_RELEASE;

  }
})

DECLARE_EVENT_LISTENER(mouseListener, (MouseEvent *event) {
  if(event->type == EVENT_TYPE(MouseButtonPressedEvent)) {
    InputData.mouseButtonStates[((MouseButtonPressedEvent*)event)->buttonId] = GLFW_PRESS;
  } else if(event->type == EVENT_TYPE(MouseButtonReleasedEvent)) {
    InputData.mouseButtonStates[((MouseButtonPressedEvent*)event)->buttonId] = GLFW_RELEASE;
  } else if(event->type == EVENT_TYPE(MouseMovedEvent)) {
    InputData.currMousePos = ( (MouseMovedEvent*)event )->position;
    InputData.deltaMousePos = (MousePosition) {
      .x = InputData.currMousePos.x - InputData.prevMousePos.x,
      .y = InputData.currMousePos.y - InputData.prevMousePos.y,
    };
  }
})


void
ev_input_init()
{
  InputData.activeWindow = NULL;

  ACTIVATE_EVENT_LISTENER(keyListener, KeyEvent);
  ACTIVATE_EVENT_LISTENER(mouseListener, MouseEvent);

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

EVMODAPI bool
ev_input_getmousebuttondown(
    MouseButtonID button)
{
  return InputData.mouseButtonStates[button] == GLFW_PRESS;
}

EVMODAPI bool
ev_input_getmousebuttonup(
    MouseButtonID button)
{
  return InputData.mouseButtonStates[button] == GLFW_RELEASE;
}

EVMODAPI bool
ev_input_getmousebuttonjustpressed(
    MouseButtonID button)
{
  bool res = InputData.mouseButtonStates[button] == GLFW_PRESS &&
    InputData.prevmouseButtonStates[button] == GLFW_RELEASE;
  return res;
}

EVMODAPI bool
ev_input_getmousebuttonjustreleased(
    MouseButtonID button)
{
  return InputData.mouseButtonStates[button] == GLFW_RELEASE &&
    InputData.prevmouseButtonStates[button] == GLFW_PRESS;
}

EVMODAPI void
ev_input_update()
{
  memcpy(InputData.prevKeyStates, InputData.keyStates, MAX_KEY_COUNT * sizeof(InputData.keyStates[0]));
  memcpy(InputData.prevmouseButtonStates, InputData.mouseButtonStates, MAX_MOUSEBUTTON_COUNT * sizeof(InputData.mouseButtonStates[0]));
  InputData.prevMousePos = InputData.currMousePos;
  InputData.deltaMousePos = (MousePosition){0,0};
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

EVMODAPI MousePosition
ev_input_getmousepos()
{
  ev_log_debug("Current mouse position = (%f, %f)", InputData.currMousePos.x, InputData.currMousePos.y);
  return InputData.currMousePos;
}

EVMODAPI MousePosition
ev_input_getdeltamousepos()
{
  return InputData.deltaMousePos;
}

void
ev_input_deinit()
{
}
