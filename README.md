Some UEFI GUI Early Examples

Two samples:

1) Interacting with a mouse
2) Stepping through display modes

Only tested on a Minnowboard Turbot attached to a 23" Dell UltraSharp display,
built with GCC_49 tool configuration on Linux.

Quick build steps:

uefi_gui_stuff $ ln -s ~/edk2/MdeModulePkg/Application/uefi_gui_stuff .
uefi_gui_stuff $ gvim ~/edk2/MdeModulePkg/MdeModulePkg.dsc
# Search for HelloWorld
# Add MdeModulePkg/Application/uefi_gui_stuff/gui_stuff.inf
uefi_gui_stuff $ cd ~/edk2
uefi_gui_stuff $ build

Probably faster ways and whatnot.
