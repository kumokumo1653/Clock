#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "Vec2.h"

#define WIN_SIZE 360

struct Time{
    int year;
    int month;
    int day;
    char* weekday;
    int hour;
    int min;
    int sec;
};

void Display(void);
void Reshape(int, int);
void Timer(int);
struct Time GetTime();
void DrawPolygonal(struct Vec2, int, int );

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitWindowSize(WIN_SIZE, WIN_SIZE);
    glutCreateWindow("アナログ時計");
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutTimerFunc(1000, Timer, 0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glutMainLoop();
    return 0;
}

//描画関数
void Display(void){
    struct Vec2 center;
    struct Time nowtime;
    center.x = WIN_SIZE / 2;
    center.y = WIN_SIZE / 2;
    nowtime = GetTime();
    int start = 30;
    int ls = 70;
    int lm = 90;
    int lh = 50;
    int h = nowtime.hour % 12;
    int m = nowtime.min;
    int s = nowtime.sec;
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINES);

    glVertex2i(center.x + start * sin((2 * M_PI * s) / 60), center.y - start * cos((2 * M_PI * s) / 60));
    glVertex2i(center.x + ls * sin((2 * M_PI * s) / 60), center.y - ls * cos((2 * M_PI * s) / 60));
    glVertex2i(center.x + start * sin((2 * M_PI * (60 * m + s)) / 3600), center.y - start * cos((2 * M_PI * (60 * m + s)) / 3600));
    glVertex2i(center.x + lm * sin((2 * M_PI * (60 * m + s)) / 3600), center.y - lm * cos((2 * M_PI * (60 * m + s)) / 3600));
    glVertex2i(center.x + start * sin((2 * M_PI * (3600 * h + 60 * m + s)) / 43200), center.y - start * cos((2 * M_PI * (3600 * h + 60 * m + s)) / 43200));
    glVertex2i(center.x + lh * sin((2 * M_PI * (3600 * h + 60 * m + s)) / 43200), center.y - lh * cos((2 * M_PI * (3600 * h + 60 * m + s)) / 43200));

    glEnd();
    //glFlush();
    glutSwapBuffers();
}

//座標系の再定義関数
void Reshape(int w, int h){
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glScaled(1, -1, 1);
    glTranslated(0, -h, 0);
    glutReshapeWindow(WIN_SIZE, WIN_SIZE);
}

//タイマー割り込み
void Timer(int value){
    glutPostRedisplay();
    glutTimerFunc(1000, Timer, value);
}
struct Time GetTime(){
    time_t t;
    struct Time nowtime;
    struct tm* ts;
    char* week [7] =  {"Sun","Mon","Tue", "Wed", "Thu", "Fri", "Sat"};
    time(&t);
    ts = localtime(&t);
    nowtime.year = ts->tm_year + 1900;
    nowtime.month = ts->tm_mon + 1;
    nowtime.day = ts->tm_mday;
    nowtime.weekday = week[ts->tm_wday];
    nowtime.hour = ts->tm_hour;
    nowtime.min = ts->tm_min;
    nowtime.sec = ts->tm_sec;
    return nowtime;
}

void DrawPolygonal(struct Vec2 center, int r, int n){
    int i;
    glBegin(GL_POLYGON); // ポリゴンの描画
    for (i = 0; i < n; i++) {
        double rate = (double)i / n;
        double x = r * cos(2.0 * M_PI * rate);
        double y = r * sin(2.0 * M_PI * rate);
        glVertex2i(center.x + x, center.y + y); // 頂点座標を指定
    }
    glEnd(); // ポリゴンの描画終了
}
