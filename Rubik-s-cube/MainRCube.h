#ifndef MainRCube_h
#define MainRCube_h

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

class mini_cubs27
{
private:

public:
    unsigned char _color[4];
    unsigned int sCube[6];
    double size;
    mini_cubs27()
    {
        memset(sCube, 0, sizeof(sCube));
        size = 0.0;
    }
    void rotateZ();
    void rotateY();
    void rotateX();
    void rotateX_reverse();
    void rotateZ_reverse();
    void rotateY_reverse();
    void setColor(int i, int color);
    unsigned char* at(int i);
    void draw();
    void draw(double x, double y, double z);
};

class RCube
{
public:
    mini_cubs27 a[3][3][3];
    double size;
    unsigned int color[6];
    int rotate[6];
    int RotNOW;
    RCube(){}
    RCube(double size, unsigned int* color)
    {
        clear(size, color);
    }
    void clear(double size, unsigned int* color);
    void draw();
    void rot90(int idx, int sign);
    void Rotate(int idx, int angle);
    void rot902(int idx, int sign);
    void Rotate2(int idx, int angle);
    bool S1C();
    void S1WhiteCross();
private:
    int _angle[4];
    bool is_rotating[4][4][4];
    mini_cubs27 tmp[4][4];
};

#endif