#pragma once
#include <evol/common/ev_macros.h>
#include <evol/common/ev_types.h>

EVMODAPI void
_ev_imgl_setclearcolor(
    F32 r, 
    F32 g, 
    F32 b, 
    F32 a);

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
  F32 from_x,
  F32 from_y,
  F32 from_z,
  F32 to_x,
  F32 to_y,
  F32 to_z);

EVMODAPI void
_ev_imgl_drawaabb(
  F32 from_x,
  F32 from_y,
  F32 from_z,
  F32 to_x,
  F32 to_y,
  F32 to_z);

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
  F32 eye_x, 
  F32 eye_y, 
  F32 eye_z,
  F32 dir_x, 
  F32 dir_y, 
  F32 dir_z);