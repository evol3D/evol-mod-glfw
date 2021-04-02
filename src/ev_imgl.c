#include <ev_imgl.h>
#include <glad/glad.h>

#include <cglm/cglm.h>

EVMODAPI void
_ev_imgl_setclearcolor(
  F32 r, 
  F32 g, 
  F32 b, 
  F32 a)
{
  glClearColor(r, g, b, a);
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
  F32 from_x,
  F32 from_y,
  F32 from_z,
  F32 to_x,
  F32 to_y,
  F32 to_z)
{
  glBegin(GL_LINES);
  glVertex3f(from_x, from_y, from_z);
  glVertex3f(to_x, to_y, to_z);
  glEnd();
}

EVMODAPI void
_ev_imgl_drawaabb(
  F32 from_x,
  F32 from_y,
  F32 from_z,
  F32 to_x,
  F32 to_y,
  F32 to_z)
{
  glBegin(GL_LINES);

  glVertex3f(from_x, from_y, from_z);
  glVertex3f(from_x, from_y, to_z  );

  glVertex3f(from_x, from_y, from_z);
  glVertex3f(from_x, to_y  , from_z);

  glVertex3f(from_x, from_y, from_z);
  glVertex3f(to_x  , from_y, from_z);

  glVertex3f(to_x  , to_y  , to_z  );
  glVertex3f(to_x  , to_y  , from_z);
  glVertex3f(to_x  , to_y  , to_z  );
  glVertex3f(to_x  , from_y, to_z  );
  glVertex3f(to_x  , to_y  , to_z  );
  glVertex3f(from_x, to_y  , to_z  );

  glVertex3f(from_x, from_y, to_z);
  glVertex3f(from_x,   to_y, to_z);
  glVertex3f(from_x, from_y, to_z);
  glVertex3f(  to_x, from_y, to_z);

  glVertex3f(from_x, to_y, from_z);
  glVertex3f(from_x, to_y,   to_z);
  glVertex3f(from_x, to_y, from_z);
  glVertex3f(  to_x, to_y, from_z);

  glVertex3f(to_x, from_y, from_z);
  glVertex3f(to_x,   to_y, from_z);
  glVertex3f(to_x, from_y, from_z);
  glVertex3f(to_x, from_y,   to_z);
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

  glLoadMatrixf(proj);
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
  glLoadMatrixf(proj);
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
  F32 eye_x, 
  F32 eye_y, 
  F32 eye_z,
  F32 dir_x, 
  F32 dir_y, 
  F32 dir_z)
{
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  mat4 view;

  vec3 eye = {eye_x, eye_y, eye_z}; 
  vec3 dir = {dir_x, dir_y, dir_z};
  vec3 up  = {0.0  , 1.0  , 0.0  };

  glm_look(eye, dir, up, view);

  glLoadMatrixf(view);
}