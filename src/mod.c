// NOTICE: This file is under heavy maintenance. Proceed with care

// If we ever get more than one debug window, mutexes should be used to ensure that
// contexts don't interfere with each other. Also, the idempotence of the glad metaloading
// should be tested.
#define EV_MODULE_DEFINE
#include <evol/evolmod.h>

#include <evol/common/ev_profile.h>

#include <glad/glad.h>
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
    glfwDestroyWindow(handle);
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
  ev_BeginCPUSample(WindowUpdate, EV_CPUSF_None);
  U32 res = 0;
  if(window_title) {
    ev_log_info("Window Title: %s", window_title);
  }
  if(!glfwWindowShouldClose(handle)) {
    glfwPollEvents();
  } else {
    res = 1;
  }
  ev_EndCPUSample();
  return res;
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
_ev_window_getsize(
    WindowHandle handle, 
    U32 *width, 
    U32 *height)
{
    glfwGetWindowSize((GLFWwindow*)handle, width, height);
}

EVMODAPI void 
_ev_window_destroy(
  WindowHandle handle)
{
  if (handle) {
    glfwDestroyWindow((GLFWwindow*)handle);
  }
}

EVMODAPI void
_ev_window_setshouldclose(
  WindowHandle handle, 
  I32 value)
{
  if(handle) {
    glfwSetWindowShouldClose((GLFWwindow *)handle, value);
  }
}

EVMODAPI WindowHandle 
_ev_dbgwindow_create(
    U32 width, 
    U32 height, 
    CONST_STR title)
{
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0); 
  WindowHandle dbgWindowHandle = glfwCreateWindow(width, height, title, NULL, NULL);

  if (!dbgWindowHandle)
      return NULL;

  glfwMakeContextCurrent(dbgWindowHandle);
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

  vec_push(&WindowData.dbg_windows, &dbgWindowHandle);
  __ev_initialize_eventcallbacks(dbgWindowHandle);
  return dbgWindowHandle;
}

EVMODAPI void
_ev_dbgwindow_destroy(
    WindowHandle handle)
{
  if(handle) {
    glfwDestroyWindow((GLFWwindow*)handle);
  }
}

EVMODAPI void 
_ev_dbgwindow_setactive(
    WindowHandle handle)
{
  glfwMakeContextCurrent(handle);
}

EVMODAPI void
_ev_dbgwindow_startframe(
  WindowHandle handle) 
{
  _ev_dbgwindow_setactive(handle);
}

EVMODAPI void
_ev_dbgwindow_endframe(
  WindowHandle handle)
{
  glfwSwapBuffers(handle);
}

EVMODAPI U32 
_ev_dbgwindow_update(
    WindowHandle handle)
{
  if(!glfwWindowShouldClose(handle)) {
    glfwPollEvents();
    return 0;
  } else {
    _ev_dbgwindow_destroy(handle);
    return 1;
  }
}

#include <ev_imgl.h>

EV_BINDINGS
{
  // Window namespace bindings
  EV_NS_BIND_FN(Window, create        , _ev_window_create        );
  EV_NS_BIND_FN(Window, update        , _ev_window_update        );
  EV_NS_BIND_FN(Window, setWidth      , _ev_window_setwidth      );
  EV_NS_BIND_FN(Window, setHeight     , _ev_window_setheight     );
  EV_NS_BIND_FN(Window, setSize       , _ev_window_setsize       );
  EV_NS_BIND_FN(Window, getSize       , _ev_window_getsize       );
  EV_NS_BIND_FN(Window, destroy       , _ev_window_destroy       );
  EV_NS_BIND_FN(Window, setShouldClose, _ev_window_setshouldclose);

  // DbgWindow namespace bindings
  EV_NS_BIND_FN(DbgWindow, create    , _ev_dbgwindow_create    );
  EV_NS_BIND_FN(DbgWindow, destroy   , _ev_dbgwindow_destroy   );
  EV_NS_BIND_FN(DbgWindow, update    , _ev_dbgwindow_update    );
  EV_NS_BIND_FN(DbgWindow, startFrame, _ev_dbgwindow_startframe);
  EV_NS_BIND_FN(DbgWindow, endFrame  , _ev_dbgwindow_endframe  );

  // imGL namespace bindings
  EV_NS_BIND_FN(imGL, setClearColor   , _ev_imgl_setclearcolor   );
  EV_NS_BIND_FN(imGL, clearBuffers    , _ev_imgl_clearbuffers    );
  EV_NS_BIND_FN(imGL, clearColorBuffer, _ev_imgl_clearcolorbuffer);
  EV_NS_BIND_FN(imGL, clearDepthBuffer, _ev_imgl_cleardepthbuffer);
  EV_NS_BIND_FN(imGL, setColor3f      , _ev_imgl_setcolor3f      );
  EV_NS_BIND_FN(imGL, drawLine        , _ev_imgl_drawline        );
  EV_NS_BIND_FN(imGL, drawAABB        , _ev_imgl_drawaabb        );
  EV_NS_BIND_FN(imGL, projOrtho       , _ev_imgl_orthographic    );
  EV_NS_BIND_FN(imGL, projPersp       , _ev_imgl_perspective     );
  EV_NS_BIND_FN(imGL, setCameraView   , _ev_imgl_setcameraview   );
  EV_NS_BIND_FN(imGL, setViewport     , _ev_imgl_setviewport     );
}