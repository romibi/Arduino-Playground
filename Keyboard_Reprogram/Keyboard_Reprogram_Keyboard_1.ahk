#SingleInstance force
#InstallKeybdHook

#if (getKeyState("F13", "P"))
F13::return

; normal hotkeys from first keyboard connected to arduino e.g.
.::Send {U+2026}

#if
