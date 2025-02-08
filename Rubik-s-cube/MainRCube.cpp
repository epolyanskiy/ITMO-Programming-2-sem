#include "MainRCube.h"

void mini_cubs27::rotateZ() {
    unsigned int tmp = sCube[5];
    sCube[5] = sCube[3];
    sCube[3] = sCube[4];
    sCube[4] = sCube[2];
    sCube[2] = tmp;
}
void mini_cubs27::rotateY() {
    unsigned int tmp = sCube[2];
    sCube[2] = sCube[1];
    sCube[1] = sCube[3];
    sCube[3] = sCube[0];
    sCube[0] = tmp;
}
void mini_cubs27::rotateX()
{
    unsigned int tmp = sCube[0];
    sCube[0] = sCube[4];
    sCube[4] = sCube[1];
    sCube[1] = sCube[5];
    sCube[5] = tmp;
}

void mini_cubs27::setColor(int i, int color)
{
    this->sCube[i] = color;
}
unsigned char* mini_cubs27::at(int i)
{
    _color[0] = sCube[i] >> 16;
    _color[1] = sCube[i] >> 8;
    _color[2] = sCube[i];
    return _color;
}
void mini_cubs27::draw()
{
    glPushMatrix();
    glBegin(GL_QUADS);

    glColor3ubv(at(0));
    glNormal3f(0, 0, 1);
    glVertex3f(size, size, size);
    glVertex3f(0, size, size);
    glVertex3f(0, 0, size);
    glVertex3f(size, 0, size);

    glColor3ubv(at(1));
    glNormal3f(0, 0, -1);
    glVertex3f(size, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);
    glVertex3f(size, size, 0);

    glColor3ubv(at(2));
    glNormal3f(0, -1, 0);
    glVertex3f(size, 0, size);
    glVertex3f(0, 0, size);
    glVertex3f(0, 0, 0);
    glVertex3f(size, 0, 0);

    glColor3ubv(at(3));
    glNormal3f(0, 1, 0);
    glVertex3f(size, size, 0);
    glVertex3f(0, size, 0);
    glVertex3f(0, size, size);
    glVertex3f(size, size, size);

    glColor3ubv(at(4));
    glNormal3f(-1, 0, 0);
    glVertex3f(0, size, size);
    glVertex3f(0, size, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, size);

    glColor3ubv(at(5));
    glNormal3f(1, 0, 0);
    glVertex3f(size, size, 0);
    glVertex3f(size, size, size);
    glVertex3f(size, 0, size);
    glVertex3f(size, 0, 0);

    glEnd();
    glPopMatrix();
}

void mini_cubs27::draw(double x, double y, double z)
{
    glPushMatrix();
    glTranslated(x, y, z);
    draw();
    glPopMatrix();
}

void RCube::clear(double size, unsigned int* color)
{
    memset(rotate, 0, sizeof(rotate));
    this->size = size;
    RotNOW = -1;

    int i, j, k;
    for (i = 0; i < 6; i++)
        this->color[i] = color[i];

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            a[i][j][2].setColor(0, color[0]);
        }
    }

    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            a[i][j][0].setColor(1, color[1]);
        }
    }

    for (k = 0; k < 3; k++){
        for (j = 0; j < 3; j++){
            a[j][0][k].setColor(2, color[2]);
        }
    }

    for (k = 0; k < 3; k++){
        for (j = 0; j < 3; j++){
            a[j][2][k].setColor(3, color[3]);
        }
    }

    for (i = 0; i < 3; i++){
        for (k = 0; k < 3; k++){
            a[0][k][i].setColor(4, color[4]);
        }
    }
    for (i = 0; i < 3; i++){
        for (k = 0; k < 3; k++){
            a[2][k][i].setColor(5, color[5]);
        }
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 3; k++) {
                a[i][j][k].size = (size / 3.0) * 0.85;
            }
        }
    }
    std::cout << '\n';
    std::cout << '\n';
}
void RCube::draw()
{
    const double K = 0.4;
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(((1.0 - K) / 2) * size + K * size / 2, ((1.0 - K) / 2) * size + K * size / 2, ((1.0 - K) / 2) * size + K * size / 2);
    glutSolidCube(size * K);
    glPopMatrix();

    memset(is_rotating, true, sizeof(is_rotating));
    if (RotNOW != -1)
    {
        glPushMatrix();
        int i, j, k;
        if (RotNOW == 0 || RotNOW == 1)
        {
            k = (RotNOW & 1) * 2;
            for (i = 0; i < 3; i++){
                for (j = 0; j < 3; j++){
                    is_rotating[i][j][k] = false;
                }
            }
            glTranslated(size / 2, size / 2, 0);
            glRotatef(rotate[RotNOW], 0, 0, 1);
            glTranslated(-size / 2, -size / 2, 0);
            for (i = 0; i < 3; i++){
                for (j = 0; j < 3; j++){
                    a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
                }
            }
        }
        else if (RotNOW == 2 || RotNOW == 3)
        {
            j = (RotNOW & 1) * 2;
            for (i = 0; i < 3; i++){
                for (k = 0; k < 3; k++){
                    is_rotating[i][j][k] = false;
                }
            }
            glTranslated(size / 2, 0, size / 2);
            glRotatef(rotate[RotNOW], 0, 1, 0);
            glTranslated(-size / 2, 0, -size / 2);
            for (i = 0; i < 3; i++){
                for (k = 0; k < 3; k++){
                    a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
                }
            }
        }
        else if (RotNOW == 4 || RotNOW == 5)
        {
            i = (RotNOW & 1) * 2;
            for (j = 0; j < 3; j++){
                for (k = 0; k < 3; k++){
                    is_rotating[i][j][k] = false;
                }
            }

            glTranslated(0, size / 2, size / 2);
            glRotatef(rotate[RotNOW], 1, 0, 0);
            glTranslated(0, -size / 2, -size / 2);
            for (j = 0; j < 3; j++){
                for (k = 0; k < 3; k++){
                    a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
                }
            }
        }
        glPopMatrix();
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (is_rotating[i][j][k]) {
                    a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
                }
            }
        }
    }
}

void RCube::rot90(int idx, int sign)
{
    int i, j, k;
    if (sign == -1)
        sign = 3;
    while (sign--)
    {
        if (idx == 0 || idx == 1)
        {
            k = (idx & 1) * 2;
            for (i = 0; i < 3; i++)
                for (j = 0; j < 3; j++)
                    tmp[j][2 - i] = a[i][j][k];
            for (i = 0; i < 3; i++)
                for (j = 0; j < 3; j++)
                    tmp[i][j].rotateZ(), a[i][j][k] = tmp[i][j];
        }
        else if (idx == 2 || idx == 3)
        {
            j = (idx & 1) * 2;
            for (i = 0; i < 3; i++)
                for (k = 0; k < 3; k++)
                    tmp[k][2 - i] = a[i][j][k];
            for (i = 0; i < 3; i++)
                for (k = 0; k < 3; k++)
                    tmp[i][k].rotateX(), a[i][j][k] = tmp[i][k];
        }
        else if (idx == 4 || idx == 5)
        {
            i = (idx & 1) * 2;
            for (j = 0; j < 3; j++)
                for (k = 0; k < 3; k++)
                    tmp[k][2 - j] = a[i][j][k];
            for (j = 0; j < 3; j++)
                for (k = 0; k < 3; k++)
                    tmp[j][k].rotateY(), a[i][j][k] = tmp[j][k];
        }
    }
}

void RCube::Rotate(int idx, int angle)
{
    if (RotNOW == -1 || RotNOW == idx)
    {
        rotate[idx] += angle;

        if (rotate[idx] % 90 != 0)
        {
            RotNOW = idx;
        }
        else
        {
            if ((rotate[idx] < 0) ^ (RotNOW == 2 || RotNOW == 3)) {
                rot90(idx, 1);
            }
            else {
                rot90(idx, -1);
            }
            rotate[idx] = 0;
            RotNOW = -1;
        }
    }
}