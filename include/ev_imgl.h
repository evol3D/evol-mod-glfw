#pragma once
#include <evol/common/ev_macros.h>
#include <evol/common/ev_types.h>

EVMODAPI void
_ev_imgl_setclearcolor(
    Vec4 color);

EVMODAPI void
_ev_imgl_clearcolorbuffer();

EVMODAPI void
_ev_imgl_cleardepthbuffer();

EVMODAPI void
_ev_imgl_clearbuffers();

EVMODAPI void
_ev_imgl_setcolor3f(
    F32 r, 
    F32 g, 
    F32 b);

EVMODAPI void
_ev_imgl_drawline(
  Vec3 from,
  Vec3 to);

EVMODAPI void
_ev_imgl_drawaabb(
  Vec3 from,
  Vec3 to);

EVMODAPI void
_ev_imgl_orthographic(
  F32 left, 
  F32 right, 
  F32 bottom, 
  F32 top, 
  F32 near, 
  F32 far);

EVMODAPI void
_ev_imgl_perspective(
  F32 hfov, 
  F32 aspectRatio, 
  F32 near, 
  F32 far);

EVMODAPI void
_ev_imgl_setviewport(
    U32 x,
    U32 y,
    U32 w,
    U32 h);

EVMODAPI void
_ev_imgl_setcameraview(
  Vec3 eye, 
  Vec3 dir);

EVMODAPI void
_ev_imgl_setcameraviewmat(
  Matrix4x4 viewMatrix);

EVMODAPI void
_ev_imgl_setcameraprojmat(
  Matrix4x4 projMatrix);
