#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "Vec2.h"
#include "Shape.h"

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

void ConvertRGBtoHSV(int[], int[]);
void ConvertHSVtoRGB(int[], int[]);
void Display(void);
void Reshape(int, int);
void Timer(int);
struct Time GetTime();

void Mouse(int, int, int, int);
void Entry(int);


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

    int rgb[3] = {255,255,255};
    int hsv[3];
    ConvertRGBtoHSV(rgb, hsv);
    printf("%d, %d, %d\n", hsv[0], hsv[1], hsv[2]);
    ConvertHSVtoRGB(hsv, rgb);
    printf("%d, %d, %d\n", rgb[0], rgb[1], rgb[2]);

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
    GLubyte mcolor[4] = {255, 0, 0, 255};
    DrawCircle(m_top, (m_r - m_inner) / 2, 360, GL_POLYGON, 0.0, 2 * M_PI, 1.0,  mcolor);
    DrawStinger(center, m_inner, m_r, 1024, -M_PI / 2 + (2 * M_PI * (60 * m + s)) / 3600, 3 * M_PI / 2 + (2 * M_PI * (60 * m + s)) / 3600 , mcolor);
    GLubyte hcolor[4] = {255, 128, 128, 255};
    DrawCircle(h_top, (h_r - h_inner) / 2, 360, GL_POLYGON, 0.0, 2 * M_PI, 1.0, hcolor);
    DrawStinger(center, h_inner, h_r, 1024, -M_PI / 2 + (2 * M_PI * (3600 * h + 60 * m + s)) / 43200, 3 * M_PI / 2 + (2 * M_PI * (3600 * h + 60 * m + s)) / 43200 , hcolor);
    //glFlush();
    //文字表示
    char text[100];
    sprintf(text, "%4d/%02d/%02d (%s) %02d:%02d:%02d", nowtime.year, nowtime.month, nowtime.day, nowtime.weekday, nowtime.hour, nowtime.min, nowtime.sec);
    glRasterPos2i(30, 30);
    //glColor3ub(255, 255, 255);
    for(i = 0;i < strlen(text); i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[i]);
    }
    //デジタル用透過背景
    if(status == DIGITAL){
        glBegin(GL_QUADS);
        glColor4ub(0, 0, 0, 150);
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
        GLubyte color[4] = {255, 255, 0, 255};
        float stroke = 3.0;
        int number_size = 40;
        struct Vec2 draw_point;
        draw_point.x = (number_size + 5) / 2;
        draw_point.y = WIN_SIZE / 2;
        for(i = 0; i < sizeof(digital_clock) / sizeof(digital_clock[1]); i++){
            //数字
            if(digital_clock[i] - '0' >= 0 && digital_clock[i] - '0' <= 9){
                numbers[digital_clock[i] - '0'](draw_point, number_size, number_size * 2, stroke, color);
            }
            if(digital_clock[i] == ':'){
                DrawColon(draw_point, number_size, number_size * 2, stroke, color);
            }
            draw_point.x += (number_size + 5);
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

void ConvertRGBtoHSV(int rgb[], int hsv[]){
    double r = rgb[0] / 256.0;
    double g = rgb[1] / 256.0;
    double b = rgb[2] / 256.0;
    double h, s, v;
    double min = r < g ?(r < b ? r : b):(g < b ? g : b);
    double max = r > g ?(r > b ? r : b):(g > b ? g : b); 
    if(min == max)
        h = 0.0;
    else if(min == b) 
        h = 60 * (g - r) / (max - min) + 60;
    else if(min == r)
        h = 60 * (b - g) / (max - min) + 180;
    else if(min == g)
        h = 60 * (r - b) / (max - min) + 300;
    
    if(max != min)
        s = (max - min) / max;
    else s = 0.0;

    v = max;
    if(h < 0) h += 360.0;
    if(h > 360.0) h -= 360.0;
    
    hsv[0] = h + 0.5;
    hsv[1] = s * 100 + 0.5;
    hsv[2] = v * 100 + 0.5;
}


void ConvertHSVtoRGB(int hsv[], int rgb[]){
    double h = hsv[0];
    double s = hsv[1] / 100.0;
    double v = hsv[2] / 100.0;
    double r, g, b;
    double c = s * v;
    double k = h / 60.0;
    double x = c * (1 - fabs((int)k % 2 - 1));
    r = v - c; g = v - c; b = v - c;

    switch ((int)k) {
        default:
            break;
        case 0:
            r += c;
            g += x;
            break;
        case 1:
            r += x;
            g += c;
            break;
        case 2:
            g += c;
            b += x;
            break;
        case 3:
            g += x;
            b += c;
            break;
        case 4:
            r += x;
            b += c;
            break;
        case 5:
            r += c;
            b += x;
    }
    rgb[0] = r * 256 + 0.5;
    rgb[1] = g * 256 + 0.5;
    rgb[2] = b * 256 + 0.5;
}