# Rubic's cube
This is laboratory work No. 8 on the subject "C++ Programming"

You need to connect the OpenGL and GLUT libraries:

```cpp
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
```
## Colors

Сolor code:
- 16711680 red
- 16737792 orange
- 65331 green
- 255 blue
- 16777215 white
- 16776960 yellow

Сolors in the array:

- [0 0 0] o-1 g-2 w-4                 1
- [0 0 1] g-2 w-4                     2
- [0 0 2] r-0 g-2 w-4                 3
- [0 1 0] o-1 w-4                     4
- [0 1 1] w-4                         5
- [0 1 2] r-0 w-4                     6
- [0 2 0] o-1b-3 w-4                  7
- [0 2 1] b-4 w-5                     8
- [0 2 2] r-0 b-3 w-4                 9
- [1 0 0] o-1 g-2                     10
- [1 0 1] g-2                         11
- [1 0 2] r-0 g-2                     12
- [1 1 0] o-1                         13
- [1 1 2] r-0                         15
- [1 2 0] o-1 b-3                     16
- [1 2 1] b-3                         17
- [1 2 2] r-0 b-3                     18
- [2 0 0] o-1 g-2 y-5                 19
- [2 0 1] g-2 y-5                     20
- [2 0 2] r-0 g-2 y-5                 21
- [2 1 0] o-1 y-5                     22
- [2 1 1] y-5                         23
- [2 1 2] r-0 y-5                     24
- [2 2 0] o-1 b-3 y-5                 25
- [2 2 1] b-3 y-5                     26
- [2 2 2] r-0 b-3 y-5                 27
