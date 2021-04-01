#include <ev_imgl.h>
#include <glad/glad.h>

EVMODAPI void
_ev_imgl_setclearcolor(F32 r, F32 g, F32 b, F32 a)
{
  glClearColor(r, g, b, a);
}

EVMODAPI void
_ev_imgl_clearcolorbuffer()
{
  glClear(GL_COLOR_BUFFER_BIT);
}
