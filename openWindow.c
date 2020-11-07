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
void DrawStinger(struct Vec2, int, int, int , double, double, GLubyte []);
void DrawCircle(struct Vec2, int, int, int, double, double, float, GLubyte []);
void DrawEllipse(struct Vec2, int, int, int, int ,double, double, float ,GLubyte []);
void Mouse(int, int, int, int);
void Entry(int);
void DrawZero(struct Vec2, int, int, float, GLubyte []);
void DrawOne(struct Vec2, int, int, float, GLubyte []);
void DrawTwo(struct Vec2, int, int, float, GLubyte []);
void DrawTree(struct Vec2, int, int, float, GLubyte []);
void DrawFour(struct Vec2, int, int, float, GLubyte []);
void DrawFive(struct Vec2, int, int, float, GLubyte []);
void DrawSix(struct Vec2, int, int, float, GLubyte []);
void DrawSeven(struct Vec2, int, int, float, GLubyte []);
void DrawEight(struct Vec2, int, int, float, GLubyte []);
void DrawNine(struct Vec2, int, int, float, GLubyte []);
void DrawColon(struct Vec2, int, int ,float, GLubyte []);


//デジタル用関数ポインタ配列
void (*numbers[10])(struct Vec2, int, int, float, GLubyte []) = {DrawZero, DrawOne, DrawTwo, DrawTree, DrawFour, DrawFive, DrawSix, DrawSeven, DrawEight, DrawNine};

int enterFlag;
enum STATUS{
    ANALOG,
    DIGITAL
};
enum STATUS status;

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitWindowSize(WIN_SIZE, WIN_SIZE);
    glutCreateWindow("アナログ時計");
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutTimerFunc(500, Timer, 0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    //マウス操作のコールバック関数
    glutMouseFunc(Mouse);
    glutEntryFunc(Entry);

    status = ANALOG;
    enterFlag = 0;

    glutMainLoop();
    return 0;
}

//描画関数
void Display(void){
    int i;
    struct Vec2 center;
    struct Vec2 h_top;
    struct Vec2 m_top;
    struct Time nowtime;
    center.x = WIN_SIZE / 2;
    center.y = WIN_SIZE / 2;
    nowtime = GetTime();

    char digital_clock[8] = {'0', '0', ':', '0', '0', ':', '0', '0'};
    digital_clock[0] += (nowtime.hour / 10);
    digital_clock[1] += (nowtime.hour % 10);
    digital_clock[3] += (nowtime.min / 10);
    digital_clock[4] += (nowtime.min % 10);
    digital_clock[6] += (nowtime.sec / 10);
    digital_clock[7] += (nowtime.sec % 10);

    int h = nowtime.hour % 12;
    int m = nowtime.min;
    int s = nowtime.sec;
    int s_r = 40;
    int h_inner = 40;
    int h_r = 70;
    int m_inner = 70;
    int m_r = 90;
    glClear(GL_COLOR_BUFFER_BIT);
    
    //針描画
    m_top.x = center.x + (m_r + m_inner) / 2 * sin((2 * M_PI * (60 * m + s)) / 3600);
    m_top.y = center.y - (m_r + m_inner) / 2 * cos((2 * M_PI * (60 * m + s)) / 3600);
    h_top.x = center.x + (h_r + h_inner) / 2 * sin((2 * M_PI * (3600 * h + 60 * m + s)) / 43200);
    h_top.y = center.y - (h_r + h_inner) / 2 * cos((2 * M_PI * (3600 * h + 60 * m + s)) / 43200);
    GLubyte scolor[3] = {255,255,0};
    if(s == 0){
        DrawStinger(center, 0, s_r, 1024,-M_PI / 2, 3 * M_PI / 2, scolor);
    }else{
        DrawStinger(center, 0, s_r, 1024,-M_PI / 2, -M_PI / 2 + (2 * M_PI * s) / 60, scolor);
    }
    GLubyte mcolor[3] = {255, 0, 0};
    DrawCircle(m_top, (m_r - m_inner) / 2, 360, GL_POLYGON, 0.0, 2 * M_PI, 1.0,  mcolor);
    DrawStinger(center, m_inner, m_r, 1024, -M_PI / 2 + (2 * M_PI * (60 * m + s)) / 3600, 3 * M_PI / 2 + (2 * M_PI * (60 * m + s)) / 3600 , mcolor);
    GLubyte hcolor[3] = {255, 128, 128};
    DrawCircle(h_top, (h_r - h_inner) / 2, 360, GL_POLYGON, 0.0, 2 * M_PI, 1.0, hcolor);
    DrawStinger(center, h_inner, h_r, 1024, -M_PI / 2 + (2 * M_PI * (3600 * h + 60 * m + s)) / 43200, 3 * M_PI / 2 + (2 * M_PI * (3600 * h + 60 * m + s)) / 43200 , hcolor);
    //glFlush();
    //デジタル用透過背景
    if(status == DIGITAL){
        glBegin(GL_QUADS);
        glColor4ub(0, 0, 0, 200);
        glVertex2i(0, 0);
        glVertex2i(WIN_SIZE, 0);
        glVertex2i(WIN_SIZE, WIN_SIZE);
        glVertex2i(0, WIN_SIZE);
        glEnd();

        //デジタル時刻表示
        //-------------------------debug
        //glBegin(GL_LINES);
        //glColor3ub(255,255,255);

        //glVertex2i(0, WIN_SIZE / 2);
        //glVertex2i(WIN_SIZE, WIN_SIZE / 2);
        //glVertex2i(0, WIN_SIZE / 2 + 100);
        //glVertex2i(WIN_SIZE, WIN_SIZE / 2 + 100);
        //glVertex2i(0, WIN_SIZE / 2 - 100);
        //glVertex2i(WIN_SIZE, WIN_SIZE / 2 - 100);

        //glVertex2i(WIN_SIZE / 2, 0);
        //glVertex2i(WIN_SIZE / 2, WIN_SIZE);
        //glVertex2i(WIN_SIZE / 2 + 50, 0);
        //glVertex2i(WIN_SIZE / 2 + 50, WIN_SIZE);
        //glVertex2i(WIN_SIZE / 2 - 50, 0);
        //glVertex2i(WIN_SIZE / 2 - 50, WIN_SIZE);
        //glEnd();
        //GLubyte color[3] = {255, 255, 0};
        //DrawFive(center, 100, 200, 2.0, color);
        //-----------------debug
        GLubyte color[3] = {255, 255, 0};
        float stroke = 3.0;
        int number_size = 40;
        struct Vec2 draw_point;
        draw_point.x = 5;
        draw_point.y = WIN_SIZE / 2;
        for(i = 0; i < sizeof(digital_clock) / sizeof(digital_clock[1]); i++){
            draw_point.x += number_size;
            //数字
            if(digital_clock[i] - '0' >= 0 && digital_clock[i] - '0' <= 9){
                numbers[digital_clock[i] - '0'](draw_point, number_size, number_size * 2, stroke, color);
            }
            if(digital_clock[i] == ':'){
                DrawColon(draw_point, number_size, number_size * 2, stroke, color);
            }
        }
    }
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
    glutTimerFunc(500, Timer, value);
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

void DrawStinger(struct Vec2 center,int b, int r, int n, double theta,double etheta, GLubyte color[]){
    int i;
    //円描画
    double normal_a = 256.0 / n;
    double range = etheta - theta;
    for (i = 0; i < n; i++) {
        glBegin(GL_POLYGON); 
        double rate = (double)i / n;
        double x = r * cos(range * rate + theta);
        double y = r * sin(range * rate + theta);
        double bx = b * cos(range * rate + theta);
        double by = b * sin(range * rate + theta);
        glColor4ub(color[0], color[1], color[2], (GLubyte)(i * normal_a));
        glVertex2i(center.x + bx, center.y + by); // 頂点座標を指定
        glVertex2i(center.x + x, center.y + y); // 頂点座標を指定
        rate = (double)(i + 1) / n;
        x = r * cos(range * rate + theta);
        y = r * sin(range * rate + theta);
        bx = b * cos(range * rate + theta);
        by = b * sin(range * rate + theta);
        glVertex2i(center.x + x, center.y + y); // 頂点座標を指定
        glVertex2i(center.x + bx, center.y + by); // 頂点座標を指定
        glEnd(); 
    }
}
void DrawCircle(struct Vec2 center, int r, int n, int mode,double stheta, double etheta, float stroke, GLubyte color[]){
    int i;
    //円描画
    glLineWidth(stroke);
    glBegin(mode); 
    double range = etheta - stheta;
    for (i = 0; i < n; i++) {
        double rate = (double)i / n;
        double x = r * cos(range * rate + stheta);
        double y = r * sin(range * rate + stheta);
        glColor3ub(color[0], color[1], color[2]);
        glVertex2i(center.x + x, center.y + y); // 頂点座標を指定
    }
    glEnd(); 
}

void DrawEllipse(struct Vec2 center, int w_r, int h_r, int n, int mode,double stheta, double etheta, float stroke, GLubyte color[]){
    int i;
    double range = etheta - stheta;
    glLineWidth(stroke);    
    glBegin(mode);
    glColor3ub(color[0], color[1], color[2]);
    for (i = 0; i < n; i++) {
        double rate = (double)i / n;
        double x = w_r * cos(range * rate + stheta);
        double y = h_r * sin(range * rate + stheta);
        glVertex2i(center.x + x, center.y + y); // 頂点座標を指定
    }
    glEnd();    
}

void Mouse(int b, int s, int x, int y){
    if(b == GLUT_LEFT_BUTTON){
        if(s == GLUT_UP){
            if(enterFlag){
                if(status == ANALOG) status = DIGITAL;
                else if(status == DIGITAL) status = ANALOG;
            }
        }
    }
}


void Entry(int s){
    if(s == GLUT_ENTERED) enterFlag = 1;
    if(s == GLUT_LEFT) enterFlag = 0;
}

void DrawZero(struct Vec2 center, int width, int height, float stroke, GLubyte color[]){
    DrawEllipse(center, width / 2, height / 2, 1024, GL_LINE_LOOP, 0.0, 2 * M_PI, stroke, color);
}

void DrawOne(struct Vec2 center, int width, int height, float stroke, GLubyte color[]){
    glLineWidth(stroke);    
    glBegin(GL_LINES);
    glColor3ub(color[0], color[1], color[2]);
    glVertex2i(center.x, center.y + height / 2);
    glVertex2i(center.x, center.y - height / 2);
    glVertex2i(center.x, center.y - height / 2);
    glVertex2i(center.x - width / 4, center.y - 3 * height / 8);

    glEnd();
}

void DrawTwo(struct Vec2 center, int width, int height, float stroke, GLubyte color[]){

    struct Vec2 ellipse_center;
    ellipse_center.x = center.x;
    ellipse_center.y = center.y - height / 4;
    DrawEllipse(ellipse_center, width / 2, height / 4, 1024, GL_LINE_STRIP, 0.0, -M_PI, stroke, color);

    glLineWidth(stroke);    
    glBegin(GL_LINES);
    glColor3ub(color[0], color[1], color[2]);
    glVertex2i(center.x + width / 2, center.y - height / 4);
    glVertex2i(center.x - width / 2, center.y + height / 2);
    glVertex2i(center.x - width / 2, center.y + height / 2);
    glVertex2i(center.x + width / 2, center.y + height / 2);
    glEnd();
}

void DrawTree(struct Vec2 center, int width, int height, float stroke, GLubyte color[]){

    struct Vec2 circle_center;
    circle_center.x = center.x;
    circle_center.y = center.y + height / 4;
    glLineWidth(stroke);    
    glBegin(GL_LINES);
    glColor3ub(color[0], color[1], color[2]);
    glVertex2i(center.x - 3 * width / 8, center.y - height / 2);
    glVertex2i(center.x + width / 2, center.y - height / 2);
    glVertex2i(center.x + width / 2, center.y - height / 2);
    glVertex2i(center.x, center.y);
    glEnd();
    DrawCircle(circle_center, width / 2, 1024, GL_LINE_STRIP, -M_PI / 2, M_PI, stroke, color);
}

void DrawFour(struct Vec2 center, int width, int height, float stroke, GLubyte color[]){

    glLineWidth(stroke);    
    glBegin(GL_LINES);
    glColor3ub(color[0], color[1], color[2]);
    glVertex2i(center.x + width / 4, center.y - height / 2);
    glVertex2i(center.x - width / 2, center.y + height / 4);
    glVertex2i(center.x - width / 2, center.y + height / 4);
    glVertex2i(center.x + width / 4, center.y + height / 4);
    glVertex2i(center.x + width / 4, center.y - height / 2);
    glVertex2i(center.x + width / 4, center.y + height / 2);
    glEnd();
}

void DrawFive(struct Vec2 center, int width, int height, float stroke, GLubyte color[]){
    struct Vec2 circle_center;
    circle_center.x = center.x;
    circle_center.y = center.y + height / 4;

    glLineWidth(stroke);    
    glBegin(GL_LINES);
    glColor3ub(color[0], color[1], color[2]);
    glVertex2i(center.x + width / 2, center.y - height / 2);
    glVertex2i(center.x - width / 4, center.y - height / 2);
    glVertex2i(center.x - width / 4, center.y - height / 2);
    glVertex2i(circle_center.x -  sqrt(2.0) * width / 4, circle_center.y - sqrt(2.0) * width / 4);
    glEnd();
    DrawCircle(circle_center, width / 2, 1024, GL_LINE_STRIP, -3 * M_PI / 4, M_PI, stroke, color);
}

void DrawSix(struct Vec2 center, int width, int height, float stroke, GLubyte color[]){

    struct Vec2 circle_center;
    circle_center.x = center.x;
    circle_center.y = center.y + height / 4;
    glLineWidth(stroke);    
    glBegin(GL_LINES);
    glColor3ub(color[0], color[1], color[2]);
    glVertex2i(center.x, center.y - height / 2);
    glVertex2i(circle_center.x - sqrt(2) * width / 3, circle_center.y - width / 6);

    glEnd();
    DrawCircle(circle_center, width / 2, 1024, GL_LINE_LOOP, 0.0, 2 * M_PI, stroke, color);
}


void DrawSeven(struct Vec2 center, int width, int height, float stroke, GLubyte color[]){

    glLineWidth(stroke);    
    glBegin(GL_LINES);
    glColor3ub(color[0], color[1], color[2]);
    glVertex2i(center.x - width / 2, center.y - height / 2);
    glVertex2i(center.x + width / 2, center.y - height / 2);
    glVertex2i(center.x + width / 2, center.y - height / 2);
    glVertex2i(center.x - width / 4, center.y + height / 2);

    glEnd();
}

void DrawEight(struct Vec2 center, int width, int height, float stroke, GLubyte color[]){
    struct Vec2 circle_center;
    circle_center.x = center.x;
    circle_center.y = center.y - 5 * width / 8;
    DrawCircle(circle_center, 3 * width / 8, 1024, GL_LINE_LOOP, 0.0, 2 * M_PI, stroke, color);
    struct Vec2 ellipse_center;
    ellipse_center.x = center.x;
    ellipse_center.y = center.y + 3 * width / 8;
    DrawEllipse(ellipse_center, width / 2, 5 * width / 8, 1024, GL_LINE_LOOP, 0.0, 2 * M_PI, stroke, color);
}

void DrawNine(struct Vec2 center, int width, int height, float stroke, GLubyte color[]){

    struct Vec2 circle_center;
    circle_center.x = center.x;
    circle_center.y = center.y - height / 4;
    glLineWidth(stroke);    
    glBegin(GL_LINES);
    glColor3ub(color[0], color[1], color[2]);
    glVertex2i(center.x, center.y + height / 2);
    glVertex2i(circle_center.x + sqrt(2) * width / 3, circle_center.y + width / 6);

    glEnd();
    DrawCircle(circle_center, width / 2, 1024, GL_LINE_LOOP, 0.0, 2 * M_PI, stroke, color);
}

void DrawColon(struct Vec2 center, int width, int height,float stroke, GLubyte color[]){

    glPointSize(stroke);    
    glBegin(GL_POINTS);
    glColor3ub(color[0], color[1], color[2]);
    glVertex2i(center.x, center.y - height / 4);
    glVertex2i(center.x, center.y + height / 4);

    glEnd();    
}
