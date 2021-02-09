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

  return 0;

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

DECLARE_EVENT_LISTENER(windowResizedListener, (WindowResizedEvent *event) {
    printf("WindowResized. New Size: (%u, %u)\n", event->width, event->height);
})

void
initializeCallbacks()
{
  glfwSetWindowSizeCallback(WindowData.windowHandle, windowResizeCallback);
}

void
initializeEventListeners()
{
  ACTIVATE_EVENT_LISTENER(windowResizedListener, WindowResizedEvent);
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

EVMODAPI void windowLoop()
{
  while(!glfwWindowShouldClose(WindowData.windowHandle)) {
    glfwPollEvents();
  }
}

EV_START {
  if(!createWindow()) {
    goto window_creation_failed;
  }
  WindowData.created = true;

  windowLoop();

  return 0;

window_creation_failed:
    return 1;
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
