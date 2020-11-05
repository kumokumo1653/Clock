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
void DrawStinger(struct Vec2, int, int, int , int, double, GLubyte color[], double);
void DrawCircle(struct Vec2, int, int, int, GLubyte color[]);

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitWindowSize(WIN_SIZE, WIN_SIZE);
    glutCreateWindow("アナログ時計");
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutTimerFunc(1000, Timer, 0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glutMainLoop();
    return 0;
}

//描画関数
void Display(void){
    struct Vec2 center;
    struct Vec2 h_top;
    struct Vec2 m_top;
    struct Time nowtime;
    center.x = WIN_SIZE / 2;
    center.y = WIN_SIZE / 2;
    nowtime = GetTime();
    int h = nowtime.hour % 12;
    int m = nowtime.min;
    int s = nowtime.sec;
    int s_r = 40;
    int h_inner = 40;
    int h_r = 70;
    int m_inner = 70;
    int m_r = 90;
    glClear(GL_COLOR_BUFFER_BIT);

    //glBegin(GL_LINES);
    //glVertex2i(center.x + start * sin((2 * M_PI * s) / 60), center.y - start * cos((2 * M_PI * s) / 60));
    //glVertex2i(center.x + ls * sin((2 * M_PI * s) / 60), center.y - ls * cos((2 * M_PI * s) / 60));
    //glVertex2i(center.x + start * sin((2 * M_PI * (60 * m + s)) / 3600), center.y - start * cos((2 * M_PI * (60 * m + s)) / 3600));
    //glVertex2i(center.x + lm * sin((2 * M_PI * (60 * m + s)) / 3600), center.y - lm * cos((2 * M_PI * (60 * m + s)) / 3600));
    //glVertex2i(center.x + start * sin((2 * M_PI * (3600 * h + 60 * m + s)) / 43200), center.y - start * cos((2 * M_PI * (3600 * h + 60 * m + s)) / 43200));
    //glVertex2i(center.x + lh * sin((2 * M_PI * (3600 * h + 60 * m + s)) / 43200), center.y - lh * cos((2 * M_PI * (3600 * h + 60 * m + s)) / 43200));
    //glEnd();
    m_top.x = center.x + (m_r + m_inner) / 2 * sin((2 * M_PI * (60 * m + s)) / 3600);
    m_top.y = center.y - (m_r + m_inner) / 2 * cos((2 * M_PI * (60 * m + s)) / 3600);
    h_top.x = center.x + (h_r + h_inner) / 2 * sin((2 * M_PI * (3600 * h + 60 * m + s)) / 43200);
    h_top.y = center.y - (h_r + h_inner) / 2 * cos((2 * M_PI * (3600 * h + 60 * m + s)) / 43200);
    GLubyte scolor[3] = {255,255,0};
    DrawStinger(center, 0, s_r, 1024,GL_POLYGON, -M_PI / 2 + (2 * M_PI * s) / 60, scolor, 1.0);
    GLubyte mcolor[3] = {255, 0, 0};
    DrawCircle(m_top, (m_r - m_inner) / 2, 360, GL_POLYGON, mcolor);
    DrawStinger(center, m_inner, m_r, 1024,GL_POLYGON, -M_PI / 2 + (2 * M_PI * (60 * m + s)) / 3600 , mcolor, 1.0);
    GLubyte hcolor[3] = {255, 128, 128};
    DrawCircle(h_top, (h_r - h_inner) / 2, 360, GL_POLYGON, hcolor);
    DrawStinger(center, h_inner, h_r, 1024,GL_POLYGON, -M_PI / 2 + (2 * M_PI * (3600 * h + 60 * m + s)) / 43200 , hcolor,1.0);
    //glFlush();
    glutSwapBuffers();
}

//座標系の再定義関数
void Reshape(int w, int h){
    glutReshapeWindow(WIN_SIZE, WIN_SIZE);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glScaled(1, -1, 1);
    glTranslated(0, -h, 0);
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

void DrawStinger(struct Vec2 center,int b, int r, int n,int mode, double theta, GLubyte color[], double width){
    int i;
    //円描画
    if(mode == GL_LINE_LOOP){
        glLineWidth(width);
        glBegin(mode); 
        for (i = 0; i < n; i++) {
            double rate = (double)i / n;
            double x = r * cos(2.0 * M_PI * rate + theta);
            double y = r * sin(2.0 * M_PI * rate + theta);
            glColor4ub(color[0], color[1], color[2], i);
            glVertex2i(center.x + x, center.y + y); // 頂点座標を指定
        }
        glColor3ub(255,255,255);
        glLineWidth(1.0);
        glEnd(); 
    }else if(mode == GL_POLYGON){
        double normal_a = 256.0 / n;
        for (i = 0; i < n; i++) {
            glBegin(mode); 
            double rate = (double)i / n;
            double x = r * cos(2.0 * M_PI * rate + theta);
            double y = r * sin(2.0 * M_PI * rate + theta);
            double bx = b * cos(2.0 * M_PI * rate + theta);
            double by = b * sin(2.0 * M_PI * rate + theta);
            glColor4ub(color[0], color[1], color[2], (GLubyte)(i * normal_a));
            glVertex2i(center.x + bx, center.y + by); // 頂点座標を指定
            glVertex2i(center.x + x, center.y + y); // 頂点座標を指定
            rate = (double)(i + 1) / n;
            x = r * cos(2.0 * M_PI * rate + theta);
            y = r * sin(2.0 * M_PI * rate + theta);
            bx = b * cos(2.0 * M_PI * rate + theta);
            by = b * sin(2.0 * M_PI * rate + theta);
            glVertex2i(center.x + x, center.y + y); // 頂点座標を指定
            glVertex2i(center.x + bx, center.y + by); // 頂点座標を指定
            glEnd(); 
        }
    }
}
void DrawCircle(struct Vec2 center, int r, int n, int mode , GLubyte color[]){
    int i;
    //円描画
    glBegin(mode); 
    for (i = 0; i < n; i++) {
        double rate = (double)i / n;
        double x = r * cos(2.0 * M_PI * rate);
        double y = r * sin(2.0 * M_PI * rate);
        glColor3ub(color[0], color[1], color[2]);
        glVertex2i(center.x + x, center.y + y); // 頂点座標を指定
    }
    glEnd(); 
}
