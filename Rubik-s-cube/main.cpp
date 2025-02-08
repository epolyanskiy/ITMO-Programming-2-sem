#include "MainRCube.h"

#include <time.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <string.h>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>

#define CUBE_SIZE 30
#define TIMER 25

unsigned int c[6] = { 0xFF0000, 0xFF6600 , 0x00FF33 , 0x0000FF , 0xFFFFFF , 0xFFFF00 };

//16711680 red
//16737792 orange
//65331 green
//255 blue
//16777215 white
//16776960 yellow

/*
[0 0 0] o-1 g-2 w-4                 1
[0 0 1] g-2 w-4                     2
[0 0 2] r-0 g-2 w-4                 3
[0 1 0] o-1 w-4                     4
[0 1 1] w-4                         5
[0 1 2] r-0 w-4                     6
[0 2 0] o-1b-3 w-4                  7
[0 2 1] b-4 w-5                     8
[0 2 2] r-0 b-3 w-4                 9

[1 0 0] o-1 g-2                     10
[1 0 1] g-2                         11
[1 0 2] r-0 g-2                     12
[1 1 0] o-1                         13
                                    14
[1 1 2] r-0                         15
[1 2 0] o-1 b-3                     16
[1 2 1] b-3                         17
[1 2 2] r-0 b-3                     18

[2 0 0] o-1 g-2 y-5                 19
[2 0 1] g-2 y-5                     20
[2 0 2] r-0 g-2 y-5                 21
[2 1 0] o-1 y-5                     22
[2 1 1] y-5                         23
[2 1 2] r-0 y-5                     24
[2 2 0] o-1 b-3 y-5                 25
[2 2 1] b-3 y-5                     26
[2 2 2] r-0 b-3 y-5                 27
*/


GLfloat lightPos[] = { 0, 100, 200, 0 };

int xRot = 24, yRot = 34, zRot = 0;

double translateZ = -55.0;

RCube cube;

int Movment = 0;

int RCube_Sloving = 0;

void display()
{
    glPushMatrix();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1, 0, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glTranslatef(0, 0, translateZ);
    glRotatef(xRot, 1, 0, 0);
    glRotatef(yRot, 0, 1, 0);
    glTranslatef(CUBE_SIZE / -2.0, CUBE_SIZE / -2.0, CUBE_SIZE / -2.0);
    cube.draw();
    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat fAspect = (GLfloat)w / (GLfloat)h;
    gluPerspective(60, fAspect, 1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void lightning()
{
    glClearColor(0.192, 0.192, 0.192, 0.0);

    srand(time(0));

    float mat_specular[] = { 0.3, 0.3, 0.3, 0 };
    float diffuseLight[] = { 0.2, 0.2, 0.2, 1 };
    float ambientLight[] = { 0.9, 0.9, 0.9, 1.0 };
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMateriali(GL_FRONT, GL_SHININESS, 128);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    cube.clear(CUBE_SIZE, c);
}

void timerCallback(int) {
    if (cube.RotNOW != -1) {
        cube.Rotate(cube.RotNOW, 6);
    }
    display();
}

void rotate(int index, int angle) {
    cube.Rotate(index, angle);
    while (cube.RotNOW != -1) {
        cube.Rotate(index, angle);
        display();
    }
}

void PifPaf(RCube& cube) {
    rotate(3, -6);
    rotate(4, 6);
    rotate(3, 6);
    rotate(4, -6);
    rotate(3, -6);
}

bool S1C(RCube& cube) {
    if (cube.a[0][1][0].sCube[4] == 16777215 and
        cube.a[0][1][2].sCube[4] == 16777215 and
        cube.a[0][0][1].sCube[4] == 16777215 and
        cube.a[0][2][1].sCube[4] == 16777215) {
        return true;
    }
    return false;
}

//16711680 red
//16737792 orange
//65331 green
//255 blue
//16777215 white
//16776960 yellow

void S1WhiteCross(RCube& cube) {
    while (!S1C(cube)) {
        //под белой стороной

        if (cube.a[2][2][1].sCube[5] == 16777215) {  //бело-синее ребро за 2 движения
            while (cube.a[0][2][1].sCube[4] == 16777215) {
                rotate(4, 6);
                display();
            }
            while (cube.a[0][2][1].sCube[4] != 16777215) {
                cube.Rotate(3, 6);
                display();
            }

        }
        if (cube.a[2][1][0].sCube[5] == 16777215) {  //бело-оранжевое ребро за 2 движения
            while (cube.a[0][1][0].sCube[4] == 16777215) {
                rotate(4, 6);
                display();
            }
            while (cube.a[0][1][0].sCube[4] != 16777215) {
                cube.Rotate(0, 6);
                display();
            }
        }
        if (cube.a[2][0][1].sCube[5] == 16777215) { //бело-зеленое ребро за 2 движения
            while (cube.a[0][0][1].sCube[4] == 16777215) {
                rotate(4, 6);
                display();
            }
            while (cube.a[0][0][1].sCube[4] != 16777215) {
                cube.Rotate(2, 6);
                display();
            }
        }
        if (cube.a[2][1][2].sCube[5] == 16777215) { //бело-красное ребро за 2 движения
            while (cube.a[0][1][2].sCube[4] == 16777215) {
                rotate(4, 6);
                display();
            }
            while (cube.a[0][1][2].sCube[4] != 16777215) {
                cube.Rotate(1, 6);
                display();
            }
        }

        //   //   //   //   //  под белой стороной  //   //   //  //  //   //

        if (cube.a[2][2][1].sCube[3] == 16777215) {  //бело-синее ребро (развернуто)
            while (cube.a[0][2][1].sCube[4] == 16777215) {
                rotate(4, 6);
                display();
            }
            rotate(5, -6);
            rotate(0, 6);
            rotate(3, 6);
            rotate(0, -6);
        }
        if (cube.a[2][1][0].sCube[1] == 16777215) {  //бело-оранжевое ребро (развернуто)
            while (cube.a[0][1][0].sCube[4] == 16777215) {
                rotate(4, 6);
                display();
            }
            rotate(5, -6);
            rotate(2, 6);
            rotate(0, -6);
            rotate(2, -6);
        }
        if (cube.a[2][0][1].sCube[2] == 16777215) { //бело-зеленое ребро (развернуто)
            while (cube.a[0][1][0].sCube[4] == 16777215) {
                rotate(4, 6);
                display();
            }
            rotate(5, -6);
            rotate(1, -6);
            rotate(2, -6);
            rotate(1, 6);
        }
        if (cube.a[2][1][2].sCube[0] == 16777215) { //бело-красное ребро (развернуто)
            while (cube.a[0][1][2].sCube[4] == 16777215) {
                rotate(4, 6);
                display();
            }
            rotate(5, -6);
            rotate(3, -6);
            rotate(1, 6);
            rotate(3, -6);
        }


        //   //   //   если белая часть во втором слое   //  //   //

        if (cube.a[1][2][0].sCube[1] == 16777215) {  //синее-оранжевое ребро (оранжевое)
            while (cube.a[0][2][1].sCube[4] == 16777215) {
                rotate(4, 6);
                display();
            }
            rotate(3, 6);
        }
        if (cube.a[1][2][0].sCube[3] == 16777215) {  //синее-оранжевое ребро (синий)
            while (cube.a[0][1][0].sCube[4] == 16777215) {
                rotate(4, 6);
                display();
            }
            rotate(0, 6);
        }




        if (cube.a[1][0][0].sCube[1] == 16777215) {  //зелено-оранжевое ребро (оранжевое)
            while (cube.a[0][0][1].sCube[4] == 16777215) {
                rotate(4, 6);
                display();
            }
            rotate(2, 6);

        }
        if (cube.a[1][0][0].sCube[2] == 16777215) { //зелено-оранжевое ребро (зелено)
            while (cube.a[0][1][0].sCube[4] == 16777215) {
                rotate(4, 6);
                display();
            }
            rotate(0, -6);

        }




        if (cube.a[1][0][2].sCube[0] == 16777215) {  //зелено-красное ребро (красный)
            while (cube.a[0][0][1].sCube[4] == 16777215) {
                rotate(4, 6);
                display();
            }
            rotate(2, -6);

        }
        if (cube.a[1][0][2].sCube[2] == 16777215) { //зелено-красное ребро (зеленый)
            while (cube.a[0][1][2].sCube[4] == 16777215) {
                rotate(4, 6);
                display();
            }
            rotate(1, -6);

        }
        if (cube.a[1][2][2].sCube[3] == 16777215) {  //сине-красное ребро (синие)
            while (cube.a[0][1][2].sCube[4] == 16777215) {
                rotate(4, 6);
                display();
            }
            rotate(1, 6);

        }
        if (cube.a[1][2][2].sCube[0] == 16777215) { //сине-красное ребро (красное)
            while (cube.a[0][2][1].sCube[4] == 16777215) {
                rotate(4, 6);
                display();
            }
            rotate(3, -6);

        }

        //   //   //   перевертыш у креста   //  //   //

        if (cube.a[0][2][1].sCube[3] == 16777215) { //синий
            rotate(3, 6);
            rotate(3, 6);
        }

        if (cube.a[0][1][2].sCube[0] == 16777215) {  //красный
            rotate(1, 6);
            rotate(1, 6);
        }

        if (cube.a[0][0][1].sCube[2] == 16777215) {  //зеленый
            rotate(2, 6);
            rotate(2, 6);
        }

        if (cube.a[0][1][0].sCube[1] == 16777215) {  //оранжевый
            rotate(0, 6);
            rotate(0, 6);
        }
    }
}

bool S2C(RCube& cube) {
    if (cube.a[0][1][0].sCube[1] == 16737792 and
        cube.a[0][1][2].sCube[0] == 16711680 and
        cube.a[0][0][1].sCube[2] == 65331    and
        cube.a[0][2][1].sCube[3] == 255) {
        return true;
    }
    return false;
}

//16711680 red
//16737792 orange
//65331 green
//255 blue
//16777215 white
//16776960 yellow

void S2CWhiteCross(RCube& cube) {
    while (!S2C(cube)) {
        bool flag = false;
        if (cube.a[0][0][1].sCube[2] == 65331) {  //зеленый
            if (cube.a[0][1][0].sCube[1] == 16737792) { //зеленый и оранжевый    корректен
                rotate(1, -6);
                rotate(4, 6);
                rotate(1, 6);
                rotate(4, -6);
                rotate(1, -6);
            }
            else if (cube.a[0][1][2].sCube[0] == 16711680) { //зеленый и красный  корректен
                PifPaf(cube);
            }
            else if (cube.a[0][2][1].sCube[3] == 255) { //зеленый и синий  корректен  (противоположные)
                rotate(1, -6);
                rotate(4, 6);
                rotate(1, 6);
                rotate(4, -6);
                rotate(1, -6);

                rotate(4, -6);
                flag = true;
            }
        }

        else if (cube.a[0][1][2].sCube[0] == 16711680 ) {  //красный

            if (cube.a[0][0][1].sCube[2] == 65331) { //красный и зеленый    корректен
                PifPaf(cube);
            }
            else if (cube.a[0][2][1].sCube[3] == 255) { //красный и синий    корректен
                rotate(0, 6);
                rotate(4, 6);
                rotate(0, -6);
                rotate(4, -6);
                rotate(0, 0);
            }
            else if (cube.a[0][1][0].sCube[1] == 16737792) { //красный и оранжевый    корректен  (противоположные)
                PifPaf(cube);

                rotate(4, -6);
                flag = true;
            }
        }

        else if (cube.a[0][2][1].sCube[3] == 255) {  //синий
            if (cube.a[0][1][2].sCube[0] == 16711680) { //синий красный   корректен
                rotate(0, 6);
                rotate(4, 6);
                rotate(0, -6);
                rotate(4, -6);
                rotate(0, 0);
            }
            else if (cube.a[0][1][0].sCube[1] == 16737792) { //синий и оранжевый    корректен
                rotate(2, 6);
                rotate(4, 6);
                rotate(2, -6);
                rotate(4, -6);
                rotate(2, 6);
            }
            else if (cube.a[0][0][1].sCube[2] == 65331) { //синий и зеленый    корректен  (противоположные)
                rotate(1, -6);
                rotate(4, 6);
                rotate(1, 6);
                rotate(4, -6);
                rotate(1, -6);

                rotate(4, -6);
                flag = true;
            }
        }

        else if (cube.a[0][1][0].sCube[1] == 16737792) {  //оранжевый
            if (cube.a[0][2][1].sCube[3] == 255) { //оранжевый и синий  корректен
                rotate(2, 6);
                rotate(4, 6);
                rotate(2, -6);
                rotate(4, -6);
                rotate(2, 6);
            }
            else if (cube.a[0][0][1].sCube[2] == 65331) { //оранжевый и зеленый    корректен
                rotate(1, -6);
                rotate(4, 6);
                rotate(1, 6);
                rotate(4, -6);
                rotate(1, -6);
            }
            else if (cube.a[0][1][2].sCube[0] == 16711680) { //оранжевый и красный    корректен  (противоположные)
                PifPaf(cube);

                rotate(4, -6);
                flag = true;
            }
        }
        if (!S2C(cube)) {
            rotate(4, 6);
        }
    }
}

bool S3C(RCube& cube) {
    if (S1C(cube) and
        S2C(cube) and
        cube.a[0][0][0].sCube[1] == 16737792 and cube.a[0][0][0].sCube[2] == 65331 and cube.a[0][0][0].sCube[4] == 16777215 and
        cube.a[0][0][2].sCube[0] == 16711680 and cube.a[0][0][2].sCube[2] == 65331 and cube.a[0][0][2].sCube[4] == 16777215 and
        cube.a[0][2][0].sCube[1] == 16737792 and cube.a[0][2][0].sCube[3] == 255   and cube.a[0][2][0].sCube[4] == 16777215 and
        cube.a[0][2][2].sCube[0] == 16711680 and cube.a[0][2][2].sCube[3] == 255   and cube.a[0][2][2].sCube[4] == 16777215) {
        return true;
    }
    return false;
}

std::vector <int> colorsTempVec;

//16711680 red
//16737792 orange
//65331 green
//255 blue
//16777215 white
//16776960 yellow

void S3FirstLayer(RCube& cube) {
    while (!S3C(cube)) {
        bool flag;
        //проверка верхнего слоя (где желтый центр) на наличее угла с белым цветом
        while(true){
            colorsTempVec.clear();
            flag = false;
            for (int i = 0; i < 6; i++) {  // оранжево-зелено-желтый   (проверка есть ли белый цвет)
                if (cube.a[2][0][0].sCube[i] == 16777215) {
                    flag = true;
                }
                else if (cube.a[2][0][0].sCube[i] != 0) {
                    colorsTempVec.push_back(cube.a[2][0][0].sCube[i]);
                }
            }
            if (flag) {

                std::sort(colorsTempVec.begin(), colorsTempVec.end());
                if (colorsTempVec[0] == 65331 and colorsTempVec[1] == 16737792) { // бело-зелено-оранжевый на своем месте (пиф пафами опускаем)

                    int count = 0;
                    while (true) {
                        if (cube.a[0][0][0].sCube[4] == 16777215 and count % 2 == 1) break;

                        rotate(0, 6);
                        rotate(5, -6);
                        rotate(0, -6);
                        rotate(5, 6);
                        count++;
                    }
                }
                else if (colorsTempVec[0] == 65331 and colorsTempVec[1] == 16711680) { // бело-зелено-красный     (U)    на своем месте (пиф пафами опускаем)

                    rotate(5, -6);
                    int count = 0;
                    while (true) {
                        if (cube.a[0][0][2].sCube[4] == 16777215 and count % 2 == 1) break;
                        rotate(2, 6);
                        rotate(5, -6);
                        rotate(2, -6);
                        rotate(5, 6);
                        count++;
                    }
                }
                else if (colorsTempVec[0] == 255 and colorsTempVec[1] == 16711680) { // бело-сине-красный     (U)(U)     на своем месте (пиф пафами опускаем)
                    rotate(5, -6);
                    rotate(5, -6);
                    int count = 0;
                    while (true) {
                        if (cube.a[0][2][2].sCube[4] == 16777215 and count % 2 == 1) break;
                        rotate(1, -6);
                        rotate(5, -6);
                        rotate(1, 6);
                        rotate(5, 6);
                        count++;
                    }
                }
                else if (colorsTempVec[0] == 255 and colorsTempVec[1] == 16737792) { // бело-сине-оранжевый     (U')     на своем месте (пиф пафами опускаем)

                    rotate(5, 6);
                    int count = 0;
                    while (true) {
                        if (cube.a[0][2][0].sCube[4] == 16777215 and count % 2 == 1) break;
                        rotate(3, -6);
                        rotate(5, -6);
                        rotate(3, 6);
                        rotate(5, 6);
                        count++;
                    }
                }
            }
            else {
                bool Wflag1 = false, Wflag2 = false, Wflag3 = false, Wflag4 = false;
                for (int i = 0; i < 6; i++) {
                    if (cube.a[2][0][0].sCube[i] == 16777215) {
                        Wflag1 = true;
                        break;
                    }
                }
                for (int i = 0; i < 6; i++) {
                    if (cube.a[2][0][2].sCube[i] == 16777215) {
                        Wflag2 = true;
                        break;
                    }
                }
                for (int i = 0; i < 6; i++) {
                    if (cube.a[2][2][0].sCube[i] == 16777215) {
                        Wflag3 = true;
                        break;
                    }
                }
                for (int i = 0; i < 6; i++) {
                    if (cube.a[2][2][2].sCube[i] == 16777215) {
                        Wflag4 = true;
                        break;
                    }
                }
                if (Wflag1 or Wflag2 or Wflag3 or Wflag4) {
                    rotate(5, 6);
                }
                else {
                    break;
                }

            }
        }


        flag = false;
        bool flag2 = false;


        //проверяем, есть ли в нижнем слое белый цвет, который надор вытащить наверх        !!!!!!!!!! если стоит правильный не трогаем !!!!!!!!!!!


        for (int i = 0; i < 6; i++) {
            if (cube.a[0][0][0].sCube[i] == 16777215) {
                if (cube.a[0][0][0].sCube[1] == 16737792 and cube.a[0][0][0].sCube[2] == 65331 and cube.a[0][0][0].sCube[4] == 16777215) {  //оранжево-зелено-белый угол
                    break;
                }
                else {
                    flag = true;
                    break;
                }
            }
        }
        if (flag) {
            while (true) {
                for (int i = 0; i < 6; i++) {  // оранжево-зелено-желтый   (проверка есть ли белый цвет)
                    if (cube.a[2][0][0].sCube[i] == 16777215) {
                        flag2 = true;
                        break;
                    }
                }
                if (flag2) {
                    rotate(5, 6);  //если на ним есть белый, его надо сдивнуть, иначе он будет внизу
                }
                else {
                    break;
                }
            }
            rotate(0, 6);
            rotate(5, -6);
            rotate(0, -6);
            rotate(5, 6);

        }
        else {
            flag = false;
            flag2 = false;
            for (int i = 0; i < 6; i++) {
                if (cube.a[0][0][2].sCube[i] == 16777215) {   //красно-зелено-белый угол
                    if (cube.a[0][0][2].sCube[0] == 16711680 and cube.a[0][0][2].sCube[2] == 65331 and cube.a[0][0][2].sCube[4] == 16777215) {
                        break;
                    }
                    else {
                        flag = true;
                        break;
                    }
                }

            }
            if (flag) {
                while (true) {
                    for (int i = 0; i < 6; i++) {  // красно-зелено-желтый   (проверка есть ли белый цвет)
                        if (cube.a[2][0][2].sCube[i] == 16777215) {
                            flag2 = true;
                            break;
                        }
                    }
                    if (flag2) {
                        rotate(5, 6);  //если на ним есть белый, его надо сдивнуть, иначе он будет внизу
                    }
                    else {
                        break;
                    }
                }
                rotate(2, 6);
                rotate(5, -6);
                rotate(2, -6);
                rotate(5, 6);

            }
            else {
                flag = false;
                flag2 = false;
                for (int i = 0; i < 6; i++) {
                    if (cube.a[0][2][2].sCube[i] == 16777215) {   //красно-сине-белый угол
                        if (cube.a[0][2][2].sCube[0] == 16711680 and cube.a[0][2][2].sCube[3] == 255 and cube.a[0][2][2].sCube[4] == 16777215) {
                            break;
                        }
                        else {
                            flag = true;
                            break;
                        }
                    }

                }
                if (flag) {

                    while (true) {
                        for (int i = 0; i < 6; i++) {  // красно-сине-желтый   (проверка есть ли белый цвет)
                            if (cube.a[2][2][2].sCube[i] == 16777215) {
                                flag2 = true;
                                break;
                            }
                        }
                        if (flag2) {
                            rotate(5, 6);  //если на ним есть белый, его надо сдивнуть, иначе он будет внизу
                        }
                        else {
                            break;
                        }
                    }
                    rotate(1, -6);
                    rotate(5, -6);
                    rotate(1, 6);
                    rotate(5, 6);

                }

                else {
                    flag = false;
                    flag2 = false;
                    for (int i = 0; i < 6; i++) {
                        if (cube.a[0][2][0].sCube[i] == 16777215) {   //оранжево-сине-белый угол
                            if (cube.a[0][2][0].sCube[1] == 16737792 and cube.a[0][2][0].sCube[3] == 255 and cube.a[0][2][0].sCube[4] == 16777215) {
                                break;
                            }
                            else {
                                flag = true;
                                break;
                            }
                        }

                    }
                    if (flag) {
                        while (true) {
                            for (int i = 0; i < 6; i++) {  // оранжево-сине-желтый   (проверка есть ли белый цвет)
                                if (cube.a[2][2][0].sCube[i] == 16777215) {
                                    flag2 = true;
                                    break;
                                }
                            }
                            if (flag2) {
                                rotate(5, 6);  //если на ним есть белый, его надо сдивнуть, иначе он будет внизу
                            }
                            else {
                                break;
                            }
                        }
                        rotate(3, -6);
                        rotate(5, -6);
                        rotate(3, 6);
                        rotate(5, 6);

                    }
                }
            }
        }
    }
}





bool S4C(RCube& cube) {
    if (S1C(cube) and
        S2C(cube) and
        S3C(cube) and
        cube.a[1][0][0].sCube[1] == 16737792 and cube.a[1][0][0].sCube[2] == 65331 and
        cube.a[1][0][2].sCube[0] == 16711680 and cube.a[1][0][2].sCube[2] == 65331 and
        cube.a[1][2][0].sCube[1] == 16737792 and cube.a[1][2][0].sCube[3] == 255 and
        cube.a[1][2][2].sCube[0] == 16711680 and cube.a[1][2][2].sCube[3] == 255) {
        return true;
    }
    return false;
}

//16711680 red
//16737792 orange
//65331 green
//255 blue
//16777215 white
//16776960 yellow

std::vector <int> colorsTempVecCopy;
void S4SecondLayer(RCube& cube) {
    bool flag;

    while (!S4C(cube)) {

        while (true) {
            colorsTempVec.clear();
            colorsTempVecCopy.clear();
            flag = false;
            for (int i = 0; i < 6; i++) {
                if (cube.a[2][1][0].sCube[i] == 16776960) {   //смотрим желто-оранжевое ребро и проверяем наличе там желтого цвета
                    flag = true;
                }
                else if (cube.a[2][1][0].sCube[i] != 0) {
                    colorsTempVec.push_back(cube.a[2][1][0].sCube[i]);
                    colorsTempVecCopy.push_back(cube.a[2][1][0].sCube[i]);
                }
            }
            if (!flag) {

                std::sort(colorsTempVecCopy.begin(), colorsTempVecCopy.end());
                if (colorsTempVecCopy[0] == 65331 and colorsTempVecCopy[1] == 16737792) {  // оранжево-зеленое ребро 2 варианта


                    if (cube.a[2][1][0].sCube[1] == 16737792) {  //cube.a[2][1][0].sCube[1]  ---> та сторона, которая смотрит на нас
                        rotate(5, 6);
                        rotate(2, -6);
                        rotate(5, -6);
                        rotate(2, 6);
                        rotate(5, -6);
                        rotate(0, 6);
                        rotate(5, 6);
                        rotate(0, -6);
                    }
                    else if (cube.a[2][1][0].sCube[1] == 65331) {
                        rotate(5, -6);  // повернули к нужному месту

                        rotate(5, -6);
                        rotate(0, 6);
                        rotate(5, 6);
                        rotate(0, -6);
                        rotate(5, 6);
                        rotate(2, -6);
                        rotate(5, -6);
                        rotate(2, 6);


                    }
                }
                else if (colorsTempVecCopy[0] == 255 and colorsTempVecCopy[1] == 16711680) {  //сине-красное 2 варианта


                    if (cube.a[2][1][0].sCube[1] == 255) {   //??????
                        rotate(5, 6);

                        rotate(5, -6);
                        rotate(1, -6);
                        rotate(5, 6);
                        rotate(1, 6);
                        rotate(5, 6);
                        rotate(3, 6);
                        rotate(5, -6);
                        rotate(3, -6);
                    }
                    else if (cube.a[2][1][0].sCube[1] == 16711680) {
                        rotate(5, 6);
                        rotate(5, 6);

                        rotate(5, 6);
                        rotate(3, 6);
                        rotate(5, -6);
                        rotate(3, -6);
                        rotate(5, -6);
                        rotate(1, -6);
                        rotate(5, 6);
                        rotate(1, 6);
                    }
                }
                else if (colorsTempVecCopy[0] == 255 and colorsTempVecCopy[1] == 16737792) {  //оранжево-синее   2 варианта


                    if (cube.a[2][1][0].sCube[1] == 255) {
                        rotate(5, 6);

                        rotate(5, 6);
                        rotate(0, -6);
                        rotate(5, -6);
                        rotate(0, 6);
                        rotate(5, -6);
                        rotate(3, -6);
                        rotate(5, 6);
                        rotate(3, 6);
                    }
                    else if (cube.a[2][1][0].sCube[1] == 16737792) {
                        rotate(5, -6);
                        rotate(3, -6);
                        rotate(5, 6);
                        rotate(3, 6);
                        rotate(5, 6);
                        rotate(0, -6);
                        rotate(5, -6);
                        rotate(0, 6);
                    }
                }
                else if (colorsTempVecCopy[0] == 65331 and colorsTempVecCopy[1] == 16711680) {   //красно-зеленое   2 варианта  65331    16711680


                    if (cube.a[2][1][0].sCube[1] == 65331) {



                        rotate(5, -6);

                        rotate(5, 6);
                        rotate(1, 6);
                        rotate(5, -6);
                        rotate(1, -6);
                        rotate(5, -6);
                        rotate(2, 6);
                        rotate(5, 6);
                        rotate(2, -6);
                    }
                    else if (cube.a[2][1][0].sCube[1] == 16711680) {


                        rotate(5, 6);
                        rotate(5, 6);

                        rotate(5, -6);
                        rotate(2, 6);
                        rotate(5, 6);
                        rotate(2, -6);
                        rotate(5, 6);
                        rotate(1, 6);
                        rotate(5, -6);
                        rotate(1, -6);
                    }
                }
            }
            else {
                bool Yflag1 = false, Yflag2 = false, Yflag3 = false, Yflag4 = false;
                bool Ansflag1 = false, Ansflag2 = false, Ansflag3 = false, Ansflag4 = false;  // проверяем, остались ли ребра без желтого цвета, если да, то делаем поворот, иначе проверяем уже установленные

                for (int i = 0; i < 6; i++) {
                    if (cube.a[2][0][1].sCube[i] == 16776960) {
                        Yflag1 = true;
                        break;
                    }
                }
                if (!Yflag1) Ansflag1 = true;

                for (int i = 0; i < 6; i++) {
                    if (cube.a[2][1][0].sCube[i] == 16776960) {
                        Yflag2 = true;
                        break;
                    }
                }
                if (!Yflag2) Ansflag2 = true;

                for (int i = 0; i < 6; i++) {
                    if (cube.a[2][1][2].sCube[i] == 16776960) {
                        Yflag3 = true;
                        break;
                    }
                }
                if (!Yflag3) Ansflag3 = true;

                for (int i = 0; i < 6; i++) {
                    if (cube.a[2][2][1].sCube[i] == 16776960) {
                        Yflag4 = true;
                        break;
                    }
                }
                if (!Yflag4) Ansflag4 = true;

                if (Ansflag1 or Ansflag2 or Ansflag3 or Ansflag4) {

                    rotate(5, 6);
                }
                else {

                    break;
                }

            }
        }

        bool rotateFlag = false;
        if (cube.a[1][0][0].sCube[1] != 16737792 or cube.a[1][0][0].sCube[2] != 65331) {  //оранжево-зеленое ребро

            flag = false;
            for (int i = 0; i < 6; i++) {
                if (cube.a[1][0][0].sCube[i] == 16776960) {   //проверяем наличе там желтого цвета
                    flag = true;
                }
            }
            if (!flag) {
                rotateFlag = true;
                rotate(5, -6);
                rotate(0, 6);
                rotate(5, 6);
                rotate(0, -6);
                rotate(5, 6);
                rotate(2, -6);
                rotate(5, -6);
                rotate(2, 6);
            }
        }


        if (!rotateFlag and (cube.a[1][0][2].sCube[0] != 16711680 or cube.a[1][0][2].sCube[2] != 65331)) {  //красно-зеленое ребро

            flag = false;
            for (int i = 0; i < 6; i++) {
                if (cube.a[1][0][2].sCube[i] == 16776960) {   //проверяем наличе там желтого цвета
                    flag = true;
                }
            }
            if (!flag) {
                rotateFlag = true;
                rotate(5, -6);
                rotate(2, 6);
                rotate(5, 6);
                rotate(2, -6);
                rotate(5, 6);
                rotate(1, 6);
                rotate(5, -6);
                rotate(1, -6);
            }
        }

        if (!rotateFlag and (cube.a[1][2][0].sCube[1] != 16737792 or cube.a[1][2][0].sCube[3] != 255)) {  //оранжево-синее ребро

            flag = false;
            for (int i = 0; i < 6; i++) {
                if (cube.a[1][2][0].sCube[i] == 16776960) {   //проверяем наличе там желтого цвета
                    flag = true;
                }
            }
            if (!flag) {
                rotateFlag = true;
                rotate(5, -6);
                rotate(3, -6);
                rotate(5, 6);
                rotate(3, 6);
                rotate(5, 6);
                rotate(0, -6);
                rotate(5, -6);
                rotate(0, 6);
            }
        }

        if (!rotateFlag and (cube.a[1][2][2].sCube[0] != 16711680 or cube.a[1][2][2].sCube[3] != 255)) {  //красно-синее ребро

            flag = false;
            for (int i = 0; i < 6; i++) {
                if (cube.a[1][2][2].sCube[i] == 16776960) {   //проверяем наличе там желтого цвета
                    flag = true;
                }
            }
            if (!flag) {
                rotateFlag = true;
                rotate(5, 6);
                rotate(3, 6);
                rotate(5, -6);
                rotate(3, -6);
                rotate(5, -6);
                rotate(1, -6);
                rotate(5, 6);
                rotate(1, 6);
            }
        }


    }
}


bool S5C(RCube& cube) {
    //  16711680  16737792   65331    255       16777215  16776960
    //  красный  оранжевый  зеленый   синий     белый     желтый
    if (S1C(cube) and
        S2C(cube) and
        S3C(cube) and
        S4C(cube) and
        cube.a[2][0][1].sCube[5] == 16776960 and
        cube.a[2][1][0].sCube[5] == 16776960 and
        cube.a[2][1][2].sCube[5] == 16776960 and
        cube.a[2][2][1].sCube[5] == 16776960) {
        return true;
    }
    return false;
}

//16711680 red
//16737792 orange
//65331 green
//255 blue
//16777215 white
//16776960 yellow

void S5YellowCross(RCube& cube) {
    while (!S5C(cube)) {

        int Ycount = 0;
        bool YGflag = false, YOflag = false, YBflag = false, YRflag = false;
        if (cube.a[2][0][1].sCube[5] == 16776960) {
            Ycount++;
            YGflag = true;
        }
        if (cube.a[2][1][0].sCube[5] == 16776960) {
            Ycount++;
            YOflag = true;
        }
        if (cube.a[2][1][2].sCube[5] == 16776960) {
            Ycount++;
            YRflag = true;
        }
        if (cube.a[2][2][1].sCube[5] == 16776960) {
            Ycount++;
            YBflag = true;
        }

        if (Ycount == 2) {
            if (YOflag and YRflag) {
                rotate(2, 6);
                rotate(0, 6);
                rotate(5, -6);
                rotate(0, -6);
                rotate(5, 6);
                rotate(2, -6);
            }
            else if (YBflag and YGflag) {
                rotate(5, 6);

                rotate(2, 6);
                rotate(0, 6);
                rotate(5, -6);
                rotate(0, -6);
                rotate(5, 6);
                rotate(2, -6);
            }
            else if (YBflag and YRflag) {
                rotate(2, 6);
                rotate(0, 6);
                rotate(5, -6);
                rotate(0, -6);
                rotate(5, 6);
                rotate(0, 6);
                rotate(5, -6);
                rotate(0, -6);
                rotate(5, 6);
                rotate(2, -6);
            }
            else if (YBflag and YOflag) {
                rotate(5, 6);

                rotate(2, 6);
                rotate(0, 6);
                rotate(5, -6);
                rotate(0, -6);
                rotate(5, 6);
                rotate(0, 6);
                rotate(5, -6);
                rotate(0, -6);
                rotate(5, 6);
                rotate(2, -6);
            }
            else if (YGflag and YOflag) {
                rotate(5, 6);
                rotate(5, 6);

                rotate(2, 6);
                rotate(0, 6);
                rotate(5, -6);
                rotate(0, -6);
                rotate(5, 6);
                rotate(0, 6);
                rotate(5, -6);
                rotate(0, -6);
                rotate(5, 6);
                rotate(2, -6);
            }
            else if (YGflag and YRflag) {
                rotate(5, -6);

                rotate(2, 6);
                rotate(0, 6);
                rotate(5, -6);
                rotate(0, -6);
                rotate(5, 6);
                rotate(0, 6);
                rotate(5, -6);
                rotate(0, -6);
                rotate(5, 6);
                rotate(2, -6);
            }
        }
        if (Ycount == 0) {
            rotate(2, 6);
            rotate(0, 6);
            rotate(5, -6);
            rotate(0, -6);
            rotate(5, 6);
            rotate(2, -6);
        }


    }
}

bool S6C(RCube& cube) {
    //  16711680  16737792   65331    255       16777215  16776960
    //  красный  оранжевый  зеленый   синий     белый     желтый
    if (S1C(cube) and
        S2C(cube) and
        S3C(cube) and
        S4C(cube) and
        S5C(cube) and
        cube.a[2][0][1].sCube[2] == 65331 and
        cube.a[2][1][0].sCube[1] == 16737792 and
        cube.a[2][1][2].sCube[0] == 16711680 and
        cube.a[2][2][1].sCube[3] == 255) {
        return true;
    }
    return false;
}

//16711680 red
//16737792 orange
//65331 green
//255 blue
//16777215 white
//16776960 yellow

void S6CUpFace(RCube& cube) {
    while (!S6C(cube)) {

        int Correctcount = 0;
        bool YGflag = false, YOflag = false, YBflag = false, YRflag = false;
        if (cube.a[2][0][1].sCube[2] == 65331) {
            Correctcount++;
            YGflag = true;
        }
        if (cube.a[2][1][0].sCube[1] == 16737792) {
            Correctcount++;
            YOflag = true;
        }
        if (cube.a[2][1][2].sCube[0] == 16711680) {
            Correctcount++;
            YRflag = true;
        }
        if (cube.a[2][2][1].sCube[3] == 255) {
            Correctcount++;
            YBflag = true;
        }

        if (Correctcount == 0) {
            rotate(5, 6);
        }
        else if (Correctcount == 1) {
            if (YGflag) {
                while (!S6C(cube)) {
                    rotate(0, 6);
                    rotate(5, -6);
                    rotate(0, -6);
                    rotate(5, -6);
                    rotate(0, 6);
                    rotate(5, 6);
                    rotate(5, 6);
                    rotate(0, -6);
                }
            }
            else if (YOflag) {
                while (!S6C(cube)) {
                    rotate(3, -6);
                    rotate(5, -6);
                    rotate(3, 6);
                    rotate(5, -6);
                    rotate(3, -6);
                    rotate(5, 6);
                    rotate(5, 6);
                    rotate(3, 6);
                }
            }
            else if (YRflag) {
                while (!S6C(cube)) {
                    rotate(2, 6);
                    rotate(5, -6);
                    rotate(2, -6);
                    rotate(5, -6);
                    rotate(2, 6);
                    rotate(5, 6);
                    rotate(5, 6);
                    rotate(2, -6);
                }
            }
            else if (YBflag) {
                while (!S6C(cube)) {
                    rotate(1, -6);
                    rotate(5, -6);
                    rotate(1, 6);
                    rotate(5, -6);
                    rotate(1, -6);
                    rotate(5, -6);
                    rotate(5, -6);
                    rotate(1, 6);
                }
            }
        }
        else if (Correctcount == 2) {
            if (YBflag and YGflag) {
                rotate(1, -6);
                rotate(5, -6);
                rotate(1, 6);
                rotate(5, -6);
                rotate(1, -6);
                rotate(5, -6);
                rotate(5, -6);
                rotate(1, 6);
            }
            else if (YRflag and YOflag) {
                rotate(5, 6);
                rotate(5, 6);

                rotate(1, -6);
                rotate(5, -6);
                rotate(1, 6);
                rotate(5, -6);
                rotate(1, -6);
                rotate(5, 6);
                rotate(5, 6);
                rotate(1, 6);
            }
            else {
                rotate(5, 6);
            }
        }
    }
}


void findCorner(RCube& cube, bool& YGOflag, bool& YRGflag, bool& YBOflag, bool& YRBflag, int &count) {
    std::vector <int> CornerTempVec;
    for (int i = 0; i < 6; i++) {
        if (cube.a[2][0][0].sCube[i] != 0) {
            CornerTempVec.push_back(cube.a[2][0][0].sCube[i]);
        }
    }
    std::sort(CornerTempVec.begin(), CornerTempVec.end()); //должен быть оранжевый зеленый желтый
    if (CornerTempVec[0] == 65331 and CornerTempVec[1] == 16737792 and CornerTempVec[2] == 16776960) {
        YGOflag = true;
        count++;
    }


    CornerTempVec.clear();
    for (int i = 0; i < 6; i++) {
        if (cube.a[2][0][2].sCube[i] != 0) {
            CornerTempVec.push_back(cube.a[2][0][2].sCube[i]);
        }
    }
    std::sort(CornerTempVec.begin(), CornerTempVec.end()); //должен быть красный зеленый желтый
    if (CornerTempVec[0] == 65331 and CornerTempVec[1] == 16711680 and CornerTempVec[2] == 16776960) {
        YRGflag = true;
        count++;
    }



    CornerTempVec.clear();
    for (int i = 0; i < 6; i++) {
        if (cube.a[2][2][0].sCube[i] != 0) {
            CornerTempVec.push_back(cube.a[2][2][0].sCube[i]);
        }
    }
    std::sort(CornerTempVec.begin(), CornerTempVec.end()); //должен быть оранжевый синий желтый
    if (CornerTempVec[0] == 255 and CornerTempVec[1] == 16737792 and CornerTempVec[2] == 16776960) {
        YBOflag = true;
        count++;
    }


    CornerTempVec.clear();
    for (int i = 0; i < 6; i++) {
        if (cube.a[2][2][2].sCube[i] != 0) {
            CornerTempVec.push_back(cube.a[2][2][2].sCube[i]);
        }
    }
    std::sort(CornerTempVec.begin(), CornerTempVec.end()); //должен быть красный синий желтый
    if (CornerTempVec[0] == 255 and CornerTempVec[1] == 16711680 and CornerTempVec[2] == 16776960) {
        YRBflag = true;
        count++;
    }
}



bool S7C(RCube& cube) {
    bool YGOflag = false, YRGflag = false, YBOflag = false, YRBflag = false;
    int count = 0;
    findCorner(cube, YGOflag, YRGflag, YBOflag, YRBflag, count);

    if (S1C(cube) and
        S2C(cube) and
        S3C(cube) and
        S4C(cube) and
        S5C(cube) and
        S6C and
        YGOflag and YRGflag and YBOflag and YRBflag) {
        return true;
    }
    return false;
}

//16711680 red
//16737792 orange
//65331 green
//255 blue
//16777215 white
//16776960 yellow

void S7Corners(RCube& cube) {
    while (!S7C(cube)) {

        bool YGOflag = false, YRGflag = false, YBOflag = false, YRBflag = false;
        int count = 0;
        findCorner(cube, YGOflag, YRGflag, YBOflag, YRBflag, count);


        if (count == 0) {
            rotate(5, -6);  //YRB
            rotate(1, -6);
            rotate(5, 6);
            rotate(0, -6);

            rotate(5, -6);
            rotate(1, 6);
            rotate(5, 6);
            rotate(0, 6);
        }
        else if (count == 1) {
            if (YGOflag) {
                rotate(5, -6);
                rotate(0, 6);
                rotate(5, 6);
                rotate(1, 6);

                rotate(5, -6);
                rotate(0, -6);
                rotate(5, 6);
                rotate(1, -6);
            }
            else if(YRGflag){
                rotate(5, -6);
                rotate(2, 6);
                rotate(5, 6);
                rotate(3, 6);

                rotate(5, -6);
                rotate(2, -6);
                rotate(5, 6);
                rotate(3, -6);
            }
            else if (YBOflag) {
                rotate(5, -6);
                rotate(3, -6);
                rotate(5, 6);
                rotate(2, -6);

                rotate(5, -6);
                rotate(3, 6);
                rotate(5, 6);
                rotate(2, 6);
            }
            else if (YRBflag) {
                rotate(5, -6);  //YRB
                rotate(1, -6);
                rotate(5, 6);
                rotate(0, -6);

                rotate(5, -6);
                rotate(1, 6);
                rotate(5, 6);
                rotate(0, 6);
            }
        }

    }
}

bool AllCubeSloved(RCube& cube) {
    if (S1C(cube) and
        S2C(cube) and
        S3C(cube) and
        S4C(cube) and
        S5C(cube) and
        S6C and
        cube.a[2][0][0].sCube[1] == 16737792 and cube.a[2][0][0].sCube[2] == 65331 and cube.a[2][0][0].sCube[5] == 16776960 and
        cube.a[2][0][2].sCube[0] == 16711680 and cube.a[2][0][2].sCube[2] == 65331 and cube.a[2][0][2].sCube[5] == 16776960 and
        cube.a[2][2][0].sCube[1] == 16737792 and cube.a[2][2][0].sCube[3] == 255   and cube.a[2][2][0].sCube[5] == 16776960 and
        cube.a[2][2][2].sCube[0] == 16711680 and cube.a[2][2][2].sCube[3] == 255   and cube.a[2][2][2].sCube[5] == 16776960) {
        return true;
    }
    return false;
}



void S0All(RCube& cube) {
    while (!AllCubeSloved(cube)) {
        while (cube.a[2][2][2].sCube[5] != 16776960) {
            rotate(1, 6);
            rotate(4, -6);
            rotate(1, -6);
            rotate(4, 6);
        }
        rotate(5, 6);
    }
}

bool s1=false;
bool s2=false;
bool s3=false;
bool s4=false;
bool s5=false;
bool s6=false;
bool s7=false;
bool s8=false;

void specialKeys(int key, int, int)
{
    if (key == GLUT_KEY_F1) S1WhiteCross(cube);
    if (key == GLUT_KEY_F2) S2CWhiteCross(cube);
    if (key == GLUT_KEY_F3) S3FirstLayer(cube);
    if (key == GLUT_KEY_F4) S4SecondLayer(cube);
    if (key == GLUT_KEY_F5) S5YellowCross(cube);
    if (key == GLUT_KEY_F6) S6CUpFace(cube);
    if (key == GLUT_KEY_F7) S7Corners(cube);
    if (key == GLUT_KEY_F8) S0All(cube);

    if (key == GLUT_KEY_DOWN)
    {
        xRot += 3;
        if (xRot >= 360)
            xRot -= 360;
        glutPostRedisplay();
    }

    if (key == GLUT_KEY_UP)
    {
        xRot -= 3;
        if (xRot < 0)
            xRot += 360;
        glutPostRedisplay();
    }

    if (key == GLUT_KEY_RIGHT)
    {
        yRot += 3;
        if (yRot >= 360)
            yRot -= 360;
        glutPostRedisplay();
    }

    if (key == GLUT_KEY_LEFT)
    {
        yRot -= 3;
        if (yRot < 0)
            yRot += 360;
        glutPostRedisplay();
    }


}
void keys(unsigned char key, int, int)
{
    if (cube.RotNOW == -1 && key >= '0' && key <= '5')
    {
        cube.Rotate(key - '0', 6);
        display();
        //L=0
        //1-r
        //2=d
        //3=u
        //4=f
        //5=b
    }

    if (key == 'c') {
        cube.clear(CUBE_SIZE, c);
        glutPostRedisplay();
    }
    if (key == 'a') {
        Movment = 1 - Movment;
    }
}




void timer(int)
{
    glutTimerFunc(TIMER, timer, 0);
    if (Movment)
    {
        if (cube.RotNOW == -1) {
            int r = rand() % 6 + '0';
            keys(r, 0, 0);
        }
        else {
            cube.Rotate(cube.RotNOW, 6);
        }
    }
    else
    {
        if (cube.RotNOW != -1) {
            cube.Rotate(cube.RotNOW, 6);
        }
    }
    display();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1100, 700);
    glutInitWindowPosition(1, 1);
    glutCreateWindow("RCube");
    lightning();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keys);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(TIMER, timer, 0);
    glutMainLoop();
    return 0;
}