#include <math.h>
#include <stdio.h>

#include <GL/glut.h>

#define WIDTH 600
#define HEIGHT 600
#define POS_X 350
#define POS_Y 60
#define PI 3.141592653
#define MAX 27
#define ACCELERATION_MIN 0.021f
#define ACCELERATION_MAX 0.799f
#define CAMERA_MIN 24.0f
#define CAMERA_MAX 36.0f
#define FRICTION_GROUND 0.98f
#define FRICTION_WALL 0.6f

GLfloat BGColor[] = { 0.420f, 0.604f, 0.922f, 1.0f };
GLfloat WINcolor[] = { 1.0f, 0.125f, 0.094f, 1.000f };
GLfloat Light_Pos[] = { 0.0f, -20.0f, 80.0f, 0.0f };
GLfloat Light_Dif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat Ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat Diffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

float Acceleration = 0.06f;
float Camera = 28.0f;
float x, y, ax, ay, vx, vy;
int i, j, j2, k, a, b, tmp;
int DrtHon = -1, DrtVer = -1;
char IsPressHon = 0, IsPressVer = 0;
int OffsetAngle[] = { 2, -2 }, MaxOffsetAngle[] = { 30, -30 };
char txt[30];

int Map[MAX][MAX] = {
    { 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 ,0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0 ,0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,0 },
    { 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,0 },
    { 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,0 },
    { 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,0 },
    { 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 ,0 },
    { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1 ,0 },
    { 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1 ,0 },
    { 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1 ,0 },
    { 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 ,0 },
    { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,0 },
    { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,0 },
    { 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1 ,0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 ,0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 ,0 },
    { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 ,0 },
    { 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1 ,0 },
    { 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1 ,0 },
    { 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0 ,0 },
    { 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1 ,0 },
    { 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1 ,0 },
    { 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 ,0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 ,0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 },
};

int Map2[MAX][MAX] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

void Text_Bitmap(float x, float y, char* s)
{
    glRasterPos2f(x, y);
    while (*s != '\0')
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *(s++));
}

void Reload_TableView()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 1.0, 50.0);
    glTranslatef(0.0f, 0.0f, -Camera);
    //gluLookAt(0.0, -1.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(-0.6f * a, 0.0f, 1.0f, 0.0f);
    glRotatef(0.6f * b - 30, 1.0f, 0.0f, 0.0f);
    glMatrixMode(GL_MODELVIEW);
    ax = -Acceleration * sin(a * PI / 180);
    ay = -Acceleration * sin(b * PI / 180);
}

void Reload_TextView()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Reload_TableView();
    glLoadIdentity();
    glColor3fv(WINcolor);
    glBegin(GL_QUADS);
    glVertex2f(8.0f, -12.0f); //Kiri Bawah
    glVertex2f(14.0f, -12.0f); //Kanan Bawah
    glVertex2f(14.0f, -8.0f); //Kanan Atas
    glVertex2f(8.0f, -8.0f); //Kiri Atas
    glEnd();
    glColor3fv(BGColor);
    glBegin(GL_QUADS);
    glVertex2f(-12.0f, -12.0f); //Kiri Bawah
    glVertex2f(14.0f, -12.0f); //Kanan Bawah
    glVertex2f(14.0f, 14.0f); //Kanan Atas
    glVertex2f(-12.0f, 14.0f); //Kiri Atas
    glEnd();
    glEnable(GL_LIGHTING);
    glCallList(1);
    glLoadIdentity();
    glTranslatef(x, y, 0.5f);
    glutSolidSphere(0.4, 40, 40);
    glDisable(GL_LIGHTING);
    Reload_TextView();
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);
    sprintf(txt, "Cam: %.1f  Q(-) W(+)", Camera);
    Text_Bitmap(-0.9f, 0.9f, txt);
    sprintf(txt, "Acc: %.2f  E(-) R(+)", Acceleration);
    Text_Bitmap(-0.9f, 0.8f, txt);
    glutSwapBuffers();
}

void Init()
{
    glClearColor(0.710f, 0.325f, 0.494f, 1.000f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Dif);
    glLightfv(GL_LIGHT0, GL_POSITION, Light_Pos);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 90.0f);
    glNewList(1, GL_COMPILE);
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++) {
            if (Map[i][j] == 0) {
                glLoadIdentity();
                glTranslatef(-11.5f + j, -11.5f + i, 0.5f);
                glutSolidCube(1.0);
            }
        }
    }
    glEndList();
    x = -8.5f;
    y = -13.0f;
    vx = 0.0f;
    vy = 0.0f;
    a = 0;
    b = 0;
    ax = 0.0f;
    ay = 0.0f;
}

void Timer(int value)
{
    vx += ax;
    vy += ay;
    vx *= FRICTION_GROUND;
    vy *= FRICTION_GROUND;
    if (vx > 0) {
        j = (int)(y + 11.6f);
        j2 = (int)(y + 12.4f);
        k = (int)vx;
        tmp = (int)(x + 43.5f) - 30; // (int)(x + 12 + 0.5 + 1.0) // 0.5 - Radius Ball 1.0 - Size Wall
        for (i = 0; i < k; i++) {
            if (!Map[j][tmp] || !Map[j2][tmp])
                break;
            tmp++;
        }
        if (i == k) {
            tmp = (int)(x + vx + 42.5f) - 30;
            if (Map[j][tmp] && Map[j2][tmp])
                i++;
        }
        if (i > k) {
            x += vx;
        } else {
            x = (int)(x + i + 25) - 24.5f;
            vx *= -FRICTION_WALL;
        }
    } else {
        j = (int)(y + 11.6f);
        j2 = (int)(y + 12.4f);
        k = -(int)vx;
        tmp = (int)(x + 40.5f) - 30; // (int)(x + 12 - 0.5 - 1.0)
        for (i = 0; i < k; i++) {
            if (!Map[j][tmp] || !Map[j2][tmp])
                break;
            tmp--;
        }
        if (i == k) {
            tmp = (int)(x + vx + 41.5f) - 30;
            if (Map[j][tmp] && Map[j2][tmp])
                i++;
        }
        if (i > k) {
            x += vx;
        } else {
            x = (int)(x - i + 25) - 24.5f;
            vx *= -FRICTION_WALL;
        }
    }
    if (vy > 0) {
        j = (int)(x + 11.6f);
        j2 = (int)(x + 12.4f);
        k = (int)vy;
        tmp = (int)(y + 43.5f) - 30;
        for (i = 0; i < k; i++) {
            if (!Map[tmp][j] || !Map[tmp][j2])
                break;
            tmp++;
        }
        if (i == k) {
            tmp = (int)(y + vy + 42.5f) - 30;
            if (Map[tmp][j] && Map[tmp][j2])
                i++;
        }
        if (i > k)
            y += vy;
        else {
            y = (int)(y + i + 25) - 24.5f;
            vy *= -FRICTION_WALL;
        }
    } else {
        j = (int)(x + 11.6f);
        j2 = (int)(x + 12.4f);
        k = -(int)vy;
        tmp = (int)(y + 40.5f) - 30;
        for (i = 0; i < k; i++) {
            if (!Map[tmp][j] || !Map[tmp][j2])
                break;
            tmp--;
        }
        if (i == k) {
            tmp = (int)(y + vy + 41.5f) - 30;
            if (Map[tmp][j] && Map[tmp][j2])
                i++;
        }
        if (i > k)
            y += vy;
        else {
            y = (int)(y - i + 25) - 24.5f;
            vy *= -FRICTION_WALL;
        }
    }
    if (DrtHon > -1) {
        if (a == 0) {
            if (!IsPressHon)
                DrtHon = -1;
            else
                a += OffsetAngle[DrtHon];
        } else if (a != MaxOffsetAngle[DrtHon])
            a += OffsetAngle[DrtHon];
    }
    if (DrtVer > -1) {
        if (b == 0) {
            if (!IsPressVer)
                DrtVer = -1;
            else
                b += OffsetAngle[DrtVer];
        } else if (b != MaxOffsetAngle[DrtVer])
            b += OffsetAngle[DrtVer];
    }
    glutPostRedisplay();
    glutTimerFunc(25, Timer, 0);
}

void Special(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_LEFT:
        DrtHon = 0;
        IsPressHon = 1;
        break;
    case GLUT_KEY_RIGHT:
        DrtHon = 1;
        IsPressHon = 1;
        break;
    case GLUT_KEY_DOWN:
        DrtVer = 0;
        IsPressVer = 1;
        break;
    case GLUT_KEY_UP:
        DrtVer = 1;
        IsPressVer = 1;
        break;
    }
}

void Special_Up(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_LEFT:
        if (DrtHon == 0) {
            DrtHon = 1;
            IsPressHon = 0;
        }
        break;
    case GLUT_KEY_RIGHT:
        if (DrtHon == 1) {
            DrtHon = 0;
            IsPressHon = 0;
        }
        break;
    case GLUT_KEY_DOWN:
        if (DrtVer == 0) {
            DrtVer = 1;
            IsPressVer = 0;
        }
        break;
    case GLUT_KEY_UP:
        if (DrtVer == 1) {
            DrtVer = 0;
            IsPressVer = 0;
        }
        break;
    }
}

void Keyboard(GLubyte key, int x, int y)
{
    switch (key) {
    case 'q':
        if (Camera > CAMERA_MIN)
            Camera -= 1.0f;
        break;
    case 'w':
        if (Camera < CAMERA_MAX)
            Camera += 1.0f;
        break;
    case 'e':
        if (Acceleration > ACCELERATION_MIN)
            Acceleration -= 0.01f;
        break;
    case 'r':
        if (Acceleration < ACCELERATION_MAX)
            Acceleration += 0.01f;
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(POS_X, POS_Y);
    glutCreateWindow("Labyrinth");
    Init();
    glutSpecialFunc(Special);
    glutSpecialUpFunc(Special_Up);
    glutKeyboardFunc(Keyboard);
    glutTimerFunc(0, Timer, 0);
    glutDisplayFunc(Display);
    glutMainLoop();
}
