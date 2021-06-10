#pragma once

void
ev_input_init();

void
ev_input_deinit();

void
ev_input_update();

EVMODAPI void
ev_input_setactivewindow(
    WindowHandle handle);

EVMODAPI bool
ev_input_getmousebuttonup(
    MouseButtonID button);

EVMODAPI bool
ev_input_getmousebuttondown(
    MouseButtonID button);

EVMODAPI bool
ev_input_getmousebuttonjustreleased(
    MouseButtonID button);

EVMODAPI bool
ev_input_getmousebuttonjustpressed(
    MouseButtonID button);

EVMODAPI bool
ev_input_getkeyup(
    KeyCode key);

EVMODAPI bool
ev_input_getkeydown(
    KeyCode key);

EVMODAPI bool
ev_input_getkeyjustreleased(
    KeyCode key);

EVMODAPI bool
ev_input_getkeyjustpressed(
    KeyCode key);

EVMODAPI void
ev_input_lockcursor();

EVMODAPI void
ev_input_unlockcursor();

EVMODAPI MousePosition
ev_input_getmousepos();

EVMODAPI MousePosition
ev_input_getdeltamousepos();

#define INPUT_BINDINGS() do {                                        \
    EV_NS_BIND_FN(Input, setActiveWindow, ev_input_setactivewindow); \
    EV_NS_BIND_FN(Input, getKeyDown     , ev_input_getkeydown);      \
    EV_NS_BIND_FN(Input, getKeyUp       , ev_input_getkeyup);        \
} while (0)
