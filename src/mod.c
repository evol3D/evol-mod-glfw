// NOTICE: This file is under heavy maintenance. Proceed with care

// If we ever get more than one debug window, mutexes should be used to ensure that
// contexts don't interfere with each other. Also, the idempotence of the glad metaloading
// should be tested.

#include <glad/glad.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define EV_WINDOW_VULKAN_SUPPORT

#define EV_MODULE_DEFINE
#include <evol/evolmod.h>

#include <evol/common/ev_profile.h>

struct ev_WindowData {
    bool glfwInitialized;

    vec(WindowHandle) windows;
    vec(WindowHandle) dbg_windows;
} WindowData;

struct ev_InputData {
  WindowHandle activeWindow;
} InputData;

void
init_scripting_api();

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

  InputData.activeWindow = NULL;

  init_scripting_api();
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
  U32 res = 0;
  if(!glfwWindowShouldClose(handle)) {
    glfwPollEvents();
  } else {
    res = 1;
  }
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

EVMODAPI bool
_ev_input_getkeydown(
    KeyCode key)
{
  int state = glfwGetKey(InputData.activeWindow, key);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

EVMODAPI bool
_ev_input_getkeyup(
    KeyCode key)
{
  int state = glfwGetKey(InputData.activeWindow, key);
  return state == GLFW_RELEASE;
}

EVMODAPI void
_ev_input_setactivewindow(
    WindowHandle handle)
{
  // TODO add checking for valid window
  InputData.activeWindow = handle;
}

EVMODAPI VkResult
_ev_window_createvulkansurface(
    WindowHandle handle,
    VkInstance instance,
    VkSurfaceKHR* surface)
{
  return glfwCreateWindowSurface(instance, handle, NULL, surface);
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
  EV_NS_BIND_FN(Window, createVulkanSurface, _ev_window_createvulkansurface);

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

  // Input namespace bindings
  EV_NS_BIND_FN(Input, setActiveWindow, _ev_input_setactivewindow);
  EV_NS_BIND_FN(Input, getKeyDown     , _ev_input_getkeydown);
  EV_NS_BIND_FN(Input, getKeyUp       , _ev_input_getkeyup);
}

// Initializing the scripting API
#define TYPE_MODULE evmod_script
#define NAMESPACE_MODULE evmod_script
#include <evol/meta/type_import.h>
#include <evol/meta/namespace_import.h>

void
_ev_input_getkeydown_wrapper(
    bool *out,
    KeyCode *key)
{
  *out = _ev_input_getkeydown(*key);
}

void
_ev_input_getkeyup_wrapper(
    bool *out,
    KeyCode *key)
{
  *out = _ev_input_getkeyup(*key);
}

void
init_scripting_api()
{
  evolmodule_t scripting_module = evol_loadmodule("script");
  if(!scripting_module) return;
  IMPORT_NAMESPACE(ScriptInterface, scripting_module);

  ScriptType keyCodeSType = ScriptInterface->addType("unsigned int", sizeof(KeyCode));
  ScriptType boolSType = ScriptInterface->getType("bool");

  ScriptInterface->addFunction(_ev_input_getkeydown_wrapper, "ev_input_getkeydown", boolSType, 1, &keyCodeSType);
  ScriptInterface->addFunction(_ev_input_getkeyup_wrapper, "ev_input_getkeyup", boolSType, 1, &keyCodeSType);

  ScriptInterface->loadAPI("subprojects/evmod_glfw/script_api.lua");

  evol_unloadmodule(scripting_module);
  // Invalidating namespace reference as the module is unloaded
  ScriptInterface = NULL;
}
