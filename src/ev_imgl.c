#include <ev_imgl.h>
#include <glad/glad.h>

#include <cglm/cglm.h>

#include <evol/common/ev_log.h>

EVMODAPI void
_ev_imgl_setclearcolor(
  Vec4 color)
{
  glClearColor(color.r, color.g, color.b, color.a);
}

EVMODAPI void
_ev_imgl_setcolor3f(
  F32 r, 
  F32 g, 
  F32 b)
{
  glColor3f(r, g, b);
}

EVMODAPI void
_ev_imgl_clearcolorbuffer()
{
  glClear(GL_COLOR_BUFFER_BIT);
}

EVMODAPI void
_ev_imgl_cleardepthbuffer()
{
  glClear(GL_DEPTH_BUFFER_BIT);
}

EVMODAPI void
_ev_imgl_drawline(
  Vec3 from,
  Vec3 to)
{
  glBegin(GL_LINES);
  glVertex3fv((float*)&from);
  glVertex3fv((float*)&to);
  glEnd();
}

EVMODAPI void
_ev_imgl_drawaabb(
  Vec3 from,
  Vec3 to)
{
  glBegin(GL_LINES);

  glVertex3f(from.x, from.y, from.z);
  glVertex3f(from.x, from.y, to.z  );

  glVertex3f(from.x, from.y, from.z);
  glVertex3f(from.x, to.y  , from.z);

  glVertex3f(from.x, from.y, from.z);
  glVertex3f(to.x  , from.y, from.z);

  glVertex3f(to.x  , to.y  , to.z  );
  glVertex3f(to.x  , to.y  , from.z);
  glVertex3f(to.x  , to.y  , to.z  );
  glVertex3f(to.x  , from.y, to.z  );
  glVertex3f(to.x  , to.y  , to.z  );
  glVertex3f(from.x, to.y  , to.z  );

  glVertex3f(from.x, from.y, to.z);
  glVertex3f(from.x,   to.y, to.z);
  glVertex3f(from.x, from.y, to.z);
  glVertex3f(  to.x, from.y, to.z);

  glVertex3f(from.x, to.y, from.z);
  glVertex3f(from.x, to.y,   to.z);
  glVertex3f(from.x, to.y, from.z);
  glVertex3f(  to.x, to.y, from.z);

  glVertex3f(to.x, from.y, from.z);
  glVertex3f(to.x,   to.y, from.z);
  glVertex3f(to.x, from.y, from.z);
  glVertex3f(to.x, from.y,   to.z);
  glEnd();
}

EVMODAPI void
_ev_imgl_perspective(
  F32 hfov, 
  F32 aspectRatio, 
  F32 near, 
  F32 far)
{
  glMatrixMode (GL_PROJECTION); //set the matrix to projection
  glLoadIdentity ();

  mat4 proj;
  glm_perspective(glm_rad(hfov), aspectRatio, near, far, proj);

  glLoadMatrixf((float*)proj);
}

EVMODAPI void
_ev_imgl_orthographic(
  F32 left, 
  F32 right, 
  F32 bottom, 
  F32 top, 
  F32 near, 
  F32 far)
{
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  mat4 proj;
  glm_ortho(left, right, bottom, top, near, far, proj);
  glLoadMatrixf((float*)proj);
}

EVMODAPI void
_ev_imgl_setviewport(
    U32 x,
    U32 y,
    U32 w,
    U32 h)
{
  glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h);
}

EVMODAPI void
_ev_imgl_clearbuffers()
{
  glClear(
    GL_COLOR_BUFFER_BIT 
    | GL_DEPTH_BUFFER_BIT);
}

EVMODAPI void
_ev_imgl_setcameraview(
  Vec3 eye,
  Vec3 dir)
{
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  mat4 view;

  Vec3 up  = (Vec3){{0.0, 1.0, 0.0}};

  glm_look((float*)&eye, (float*)&dir, (float*)&up, view);

  glLoadMatrixf((float*)view);
}

EVMODAPI void
_ev_imgl_setcameraviewmat(
    Matrix4x4 viewMatrix)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf((float*)viewMatrix);
}

EVMODAPI void
_ev_imgl_setcameraprojmat(
    Matrix4x4 projMatrix)
{
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf((float*)projMatrix);
}
