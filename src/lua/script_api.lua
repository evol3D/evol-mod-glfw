Input = {}

Input.KeyCode = {
  Nil         = -1,
  Apostrophe  = 39, -- '
  Comma       = 44, -- ,
  Minus       = 45, -- -
  Period      = 46, -- .
  SlashFwd    = 47, -- /
  SlashBck    = 92, -- \
  Semicolon   = 59, -- ;
  Equal       = 61, -- =
  SqBracketL  = 91, -- [
  SqBracketR  = 93, -- ]
  GraveAccent = 96, -- `
  Space       = 32,
  Num0        = 48,
  Num1        = 49,
  Num2        = 50,
  Num3        = 51,
  Num4        = 52,
  Num5        = 53,
  Num6        = 54,
  Num7        = 55,
  Num8        = 56,
  Num9        = 57,
  A           = 65,
  B           = 66,
  C           = 67,
  D           = 68,
  E           = 69,
  F           = 70,
  G           = 71,
  H           = 72,
  I           = 73,
  J           = 74,
  K           = 75,
  L           = 76,
  M           = 77,
  N           = 78,
  O           = 79,
  P           = 80,
  Q           = 81,
  R           = 82,
  S           = 83,
  T           = 84,
  U           = 85,
  V           = 86,
  W           = 87,
  X           = 88,
  Y           = 89,
  Z           = 90,
  Esc         = 256,
  Enter       = 257,
  Tab         = 258,
  Backspace   = 259,
  Insert      = 260,
  Delete      = 261,
  Right       = 262,
  Left        = 263,
  Down        = 264,
  Up          = 265,
  PageUp      = 266,
  PageDown    = 267,
  Home        = 268,
  End         = 269,
  CapsLock    = 280,
  ScrLock     = 281,
  NumLock     = 282,
  PrtSc       = 283,
  Pause       = 284,
  F1          = 290,
  F2          = 291,
  F3          = 292,
  F4          = 293,
  F5          = 294,
  F6          = 295,
  F7          = 296,
  F8          = 297,
  F9          = 298,
  F10         = 299,
  F11         = 300,
  F12         = 301,
  F13         = 302,
  F14         = 303,
  F15         = 304,
  F16         = 305,
  F17         = 306,
  F18         = 307,
  F19         = 308,
  F20         = 309,
  F21         = 310,
  F22         = 311,
  F23         = 312,
  F24         = 313,
  F25         = 314,
  ShiftL      = 340,
  ShiftR      = 344,
  CtrlL       = 341,
  CtrlR       = 345,
  AltL        = 342,
  AltR        = 346,
  SuperL      = 343,
  SuperR      = 347,
  Menu        = 348
}

Input.getKeyDown = function(keyCode)
  return C('ev_input_getkeydown', keyCode) == true
end

Input.getKeyUp = function(keyCode)
  return C('ev_input_getkeyup', keyCode) == true
end

Input.lockCursor = function()
  C('ev_input_lockcursor')
end

Input.unlockCursor = function()
  C('ev_input_unlockcursor')
end
