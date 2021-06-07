#pragma once

void
ev_input_init();

void
ev_input_deinit();

EVMODAPI void
ev_input_setactivewindow(
    WindowHandle handle);

EVMODAPI bool
ev_input_getkeyup(
    KeyCode key);

EVMODAPI bool
ev_input_getkeydown(
    KeyCode key);

EVMODAPI void
ev_input_lockcursor();

EVMODAPI void
ev_input_unlockcursor();

#define INPUT_BINDINGS() do {                                        \
    EV_NS_BIND_FN(Input, setActiveWindow, ev_input_setactivewindow); \
    EV_NS_BIND_FN(Input, getKeyDown     , ev_input_getkeydown);      \
    EV_NS_BIND_FN(Input, getKeyUp       , ev_input_getkeyup);        \
} while (0)
