// This file is under heavy maintenance. The entire API is being changed
#define EV_MODULE_DEFINE
#include <evol/evolmod.h>

#include <GLFW/glfw3.h>

struct ev_WindowData {
    bool glfwInitialized;

    vec(WindowHandle) windows;
    vec(WindowHandle) dbg_windows;
} WindowData;

void 
__ev_vecdestr_windowhandle(
    WindowHandle* p_handle)
{
    GLFWwindow *handle = *(GLFWwindow**)p_handle;
    glfwSetWindowShouldClose(handle, 1);
}


EV_CONSTRUCTOR
{
  if(!glfwInit()) {
	return 1;
  }
  WindowData.glfwInitialized = true;
  WindowData.windows = vec_init(WindowHandle, NULL, __ev_vecdestr_windowhandle);
  WindowData.dbg_windows = vec_init(WindowHandle, NULL, __ev_vecdestr_windowhandle);
  return 0;
}

void 
__ev_windowresize_eventdispatch_callback(
    GLFWwindow *window, 
    U32 width, 
    U32 height)
{
  DISPATCH_EVENT(WindowResizedEvent, {
      .handle = (WindowHandle) window,
      .width = width,
      .height = height,
  });
}

void 
__ev_key_eventdispatch_callback(
    GLFWwindow *window, 
    I32 key, 
    I32 _scancode, 
    I32 action, 
    I32 mods)
{
  EV_UNUSED_PARAM(_scancode);

  switch (action) {
    case GLFW_PRESS:
      DISPATCH_EVENT(KeyPressedEvent, {.handle = (WindowHandle) window, .keyCode = key, .mods = mods});
      break;
    case GLFW_RELEASE:
      DISPATCH_EVENT(KeyReleasedEvent, {.handle = (WindowHandle) window, .keyCode = key, .mods = mods});
      break;
    case GLFW_REPEAT:
    default:
      break;
  }
}

void 
__ev_cursormove_eventdispatch_callback(
    GLFWwindow *window, 
    F64 x, 
    F64 y)
{
  DISPATCH_EVENT(MouseMovedEvent, {
    .handle = (WindowHandle) window,
    .position = {
      .x = x,
      .y = y,
    }
  });
}

void
__ev_initialize_eventcallbacks(
    WindowHandle handle)
{
  glfwSetWindowSizeCallback(handle, __ev_windowresize_eventdispatch_callback);
  glfwSetKeyCallback(handle, __ev_key_eventdispatch_callback);
  glfwSetCursorPosCallback(handle, __ev_cursormove_eventdispatch_callback);
}

EVMODAPI WindowHandle 
_ev_window_create(
    U32 width, 
    U32 height, 
    CONST_STR title)
{
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  WindowHandle handle = glfwCreateWindow(width, height, title, NULL, NULL);

  if (!handle) 
    return NULL;

  vec_push(&WindowData.windows, &handle);
  __ev_initialize_eventcallbacks(handle);

  return handle;
}

EVMODAPI U32 
_ev_window_update(
    WindowHandle handle)
{
  if(!glfwWindowShouldClose(handle)) {
    glfwPollEvents();
    return 0;
  } else {
    return 1;
  }
}

EV_DESTRUCTOR
{
  // Close all windows
  vec_fini(WindowData.windows);
  vec_fini(WindowData.dbg_windows);

  if(WindowData.glfwInitialized)
    glfwTerminate();

  WindowData.glfwInitialized = false; // Useless as this is only executed when the module is about to be unloaded.

  return 0;
}

EVMODAPI void 
_ev_window_setwidth(
    WindowHandle handle, 
    U32 width)
{
    U32 oldWidth, oldHeight;
    glfwGetWindowSize((GLFWwindow*)handle, &oldWidth, &oldHeight);
    glfwSetWindowSize((GLFWwindow*)handle, width, oldHeight);
}

EVMODAPI void 
_ev_window_setheight(
    WindowHandle handle, 
    U32 height)
{
    U32 oldWidth, oldHeight;
    glfwGetWindowSize((GLFWwindow*)handle, &oldWidth, &oldHeight);
    glfwSetWindowSize((GLFWwindow*)handle, oldWidth, height);
}

EVMODAPI void 
_ev_window_setsize(
    WindowHandle handle, 
    U32 width, 
    U32 height)
{
    glfwSetWindowSize((GLFWwindow*)handle, width, height);
}

EVMODAPI void 
_ev_window_destroy(
    WindowHandle handle)
{
  if (handle)
	glfwSetWindowShouldClose((GLFWwindow*)handle, 1);
}

EVMODAPI WindowHandle 
_ev_dbgwindow_create(
    U32 width, 
    U32 height, 
    CONST_STR title)
{
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
  WindowHandle dbgWindowHandle = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!dbgWindowHandle)
      return NULL;
  vec_push(&WindowData.dbg_windows, &dbgWindowHandle);
  __ev_initialize_eventcallbacks(dbgWindowHandle);
  return dbgWindowHandle;
}

EVMODAPI void
_ev_dbgwindow_destroy(
    WindowHandle handle)
{
  glfwSetWindowShouldClose((GLFWwindow *) handle, 1);
}

EVMODAPI void 
_ev_dbgwindow_setactive(
    WindowHandle handle)
{
  glfwMakeContextCurrent(handle);
}

EVMODAPI U32 
_ev_dbgwindow_update(
    WindowHandle handle)
{
  if(!glfwWindowShouldClose(handle)) {
    glfwPollEvents();
    return 0;
  } else {
    return 1;
  }
}

EV_BINDINGS
{
  // Window namespace bindings
  EV_NS_BIND_FN(Window, create   , _ev_window_create   );
  EV_NS_BIND_FN(Window, update   , _ev_window_update   );
  EV_NS_BIND_FN(Window, setWidth , _ev_window_setwidth );
  EV_NS_BIND_FN(Window, setHeight, _ev_window_setheight);
  EV_NS_BIND_FN(Window, setSize  , _ev_window_setsize  );
  EV_NS_BIND_FN(Window, destroy  , _ev_window_destroy  );

  // DbgWindow namespace bindings
  EV_NS_BIND_FN(DbgWindow, create   , _ev_dbgwindow_create   );
  EV_NS_BIND_FN(DbgWindow, destroy  , _ev_dbgwindow_destroy  );
  EV_NS_BIND_FN(DbgWindow, update   , _ev_dbgwindow_update   );
  EV_NS_BIND_FN(DbgWindow, setActive, _ev_dbgwindow_setactive);
}