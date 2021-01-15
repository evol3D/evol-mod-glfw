#define EV_MODULE_DEFINE
#include <evol/evolmod.h>
#include <GLFW/glfw3.h>

#include <assert.h>

#define MAX_WINDOW_TITLE_LENGTH 256
#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600
#define DEFAULT_WINDOW_TITLE "Hello Window!"

#include <stdbool.h>

#include <string.h>

struct ev_WindowData {
    unsigned int width;
    unsigned int height;

    char windowTitle[MAX_WINDOW_TITLE_LENGTH];
    GLFWwindow *windowHandle;

    bool created;
} WindowData;

EV_CONSTRUCTOR {
  glfwInit();

  WindowData.width = DEFAULT_WINDOW_WIDTH;
  WindowData.height = DEFAULT_WINDOW_HEIGHT;
  strcpy(WindowData.windowTitle, DEFAULT_WINDOW_TITLE);
  WindowData.windowHandle = NULL;
  WindowData.created = false;
}

EV_EXPORT bool createWindow()
{
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  WindowData.windowHandle = glfwCreateWindow(WindowData.width, WindowData.height, WindowData.windowTitle, NULL, NULL);

  return WindowData.windowHandle != NULL;
}

EV_EXPORT void windowLoop()
{
  while(!glfwWindowShouldClose(WindowData.windowHandle)) {
    glfwPollEvents();
  }
}

EV_START {
  if(createWindow()) {
    WindowData.created = true;
    windowLoop();
  }
}

EV_DESTRUCTOR {
  if(WindowData.windowHandle) {
    glfwDestroyWindow(WindowData.windowHandle);
  }
  glfwTerminate();
}

void _updateWindowSize()
{
  glfwSetWindowSize(WindowData.windowHandle, WindowData.width, WindowData.height);
}

EV_EXPORT void setWindowHeight(uint32_t height)
{
  WindowData.height = height;
  _updateWindowSize();
}

EV_EXPORT void setWindowWidth(uint32_t width)
{
  WindowData.width = width;
  _updateWindowSize();
}

EV_EXPORT void setWindowSize(uint32_t width, uint32_t height)
{
  WindowData.width = width;
  WindowData.height = height;
  _updateWindowSize();
}
