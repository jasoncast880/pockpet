#LCD Dirty-Rendering Tile Engine
todo: 
Structural - replace the lower-level constructs with more automatic and intuitive syntax:
replace getters/setters with friend classes. Review the logic for the RO5/any memory allocation & deallocation.

the challenges faced:
-linked lists vs arrays (ie for sprite arrays)
 ->pros cons for insertion-time complexity, mem usage, memory access, scalability
-formatting standardization, using indexed colors vs 565.
-formatting tools! how to use scripting and terminal to build something i like

TODO: 
Build a FreeRTOS Port to replace the new, delete with PvPortMalloc, Delete.
