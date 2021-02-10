#include "evol/common/ev_macros.h"
#include "evol/core/eventsystem.h"
#define EV_MODULE_DEFINE
#include <evol/evolmod.h>

#include <GLFW/glfw3.h>

#define MAX_WINDOW_TITLE_LENGTH 256
#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600
#define DEFAULT_WINDOW_TITLE "Hello Window!"

#include <stdbool.h>
#include <string.h>

#include <evol/meta/strings.h>

struct ev_WindowData {
    U32 width;
    U32 height;

    char windowTitle[MAX_WINDOW_TITLE_LENGTH];
    GLFWwindow *windowHandle;

    bool created;
} WindowData;

struct ev_InputData {
  U32 dummy;
};

EVMODAPI bool createWindow();

EV_CONSTRUCTOR
{
  if(!glfwInit()) {
    goto glfw_initialization_failed;
  }

  WindowData.width = DEFAULT_WINDOW_WIDTH;
  WindowData.height = DEFAULT_WINDOW_HEIGHT;

  evstore_entry_t name;
  if(evstore_get_checktype(GLOBAL_STORE, EV_CORE_NAME, EV_TYPE_SDS, &name) == EV_STORE_ENTRY_FOUND) {
    strcpy(WindowData.windowTitle, name.data);
  } else {
    strcpy(WindowData.windowTitle, DEFAULT_WINDOW_TITLE);
  }
  WindowData.windowHandle = NULL;
  WindowData.created = false;

  if(!createWindow()) {
    goto window_creation_failed;
  }
  WindowData.created = true;


  return 0;

window_creation_failed:
glfw_initialization_failed:
  return 1;
}

void windowResizeCallback(GLFWwindow *window, int width, int height)
{
  (void)window;
  DISPATCH_EVENT(WindowResizedEvent, {
      .width = width,
      .height = height,
  });
}

void keyCallback(GLFWwindow *_window, int key, int _scancode, int action, int mods)
{
  EV_UNUSED_PARAM(_window);
  EV_UNUSED_PARAM(_scancode);

  switch (action) {
    case GLFW_PRESS:
      DISPATCH_EVENT(KeyPressedEvent, {.keyCode = key, .mods = mods});
      break;
    case GLFW_RELEASE:
      DISPATCH_EVENT(KeyReleasedEvent, {.keyCode = key, .mods = mods});
      break;
    case GLFW_REPEAT:
    default:
      break;
  }
}

void cursorMoveCallback(GLFWwindow *_window, F64 x, F64 y)
{
  EV_UNUSED_PARAM(_window);
  DISPATCH_EVENT(MouseMovedEvent, {
    .position = {
      .x = x,
      .y = y,
    }
  });
}

void
initializeCallbacks()
{
  glfwSetWindowSizeCallback(WindowData.windowHandle, windowResizeCallback);
  glfwSetKeyCallback(WindowData.windowHandle, keyCallback);
  glfwSetCursorPosCallback(WindowData.windowHandle, cursorMoveCallback);
}

void
initializeEventListeners()
{
}

EVMODAPI bool createWindow()
{
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  WindowData.windowHandle = glfwCreateWindow(WindowData.width, WindowData.height, WindowData.windowTitle, NULL, NULL);
  if(WindowData.windowHandle == NULL) {
    return false;
  }

  initializeCallbacks();
  initializeEventListeners();

  return true;
}

EV_UPDATE
{
  EV_UNUSED_PARAM(deltaTime);

  if(!glfwWindowShouldClose(WindowData.windowHandle)) {
    glfwPollEvents();
    return 0;
  } else {
    return 1;
  }
}

EV_DESTRUCTOR
{
  if(WindowData.created) {
    glfwDestroyWindow(WindowData.windowHandle);
  }
  glfwTerminate();

  return 0; // Don't know of a way for these 2 functions to fail
}

void _updateWindowSize()
{
  glfwSetWindowSize(WindowData.windowHandle, WindowData.width, WindowData.height);
}

EVMODAPI void setWindowHeight(U32 height)
{
  WindowData.height = height;
  _updateWindowSize();
}

EVMODAPI void setWindowWidth(U32 width)
{
  WindowData.width = width;
  _updateWindowSize();
}

EVMODAPI void setWindowSize(U32 width, U32 height)
{
  WindowData.width = width;
  WindowData.height = height;
  _updateWindowSize();
}
