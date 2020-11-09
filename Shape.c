#include <math.h>
#include <GL/glut.h>
#include "Vec2.h"
#include "Shape.h"

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
void DrawCircle(struct Vec2 center, int r, int n, GLenum mode,double stheta, double etheta, float stroke, GLubyte color[]){
    int i;
    //円描画
    glLineWidth(stroke);
    glBegin(mode); 
    glColor4ub(color[0], color[1], color[2], color[3]);
    double range = etheta - stheta;
    for (i = 0; i < n; i++) {
        double rate = (double)i / n;
        double x = r * cos(range * rate + stheta);
        double y = r * sin(range * rate + stheta);
        glVertex2i(center.x + x, center.y + y); // 頂点座標を指定
    }
    glEnd(); 
}

void DrawEllipse(struct Vec2 center, int w_r, int h_r, int n, GLenum mode,double stheta, double etheta, float stroke, GLubyte color[]){
    int i;
    double range = etheta - stheta;
    glLineWidth(stroke);    
    glBegin(mode);
    glColor4ub(color[0], color[1], color[2], color[3]);
    for (i = 0; i < n; i++) {
        double rate = (double)i / n;
        double x = w_r * cos(range * rate + stheta);
        double y = h_r * sin(range * rate + stheta);
        glVertex2i(center.x + x, center.y + y); // 頂点座標を指定
    }
    glEnd();    
}
void DrawZero(struct Vec2 center, int width, int height, float stroke, GLubyte color[]){
    DrawEllipse(center, width / 2, height / 2, 1024, GL_LINE_LOOP, 0.0, 2 * M_PI, stroke, color);
}

void DrawOne(struct Vec2 center, int width, int height, float stroke, GLubyte color[]){
    glLineWidth(stroke);    
    glBegin(GL_LINES);
    glColor4ub(color[0], color[1], color[2], color[3]);
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
    glColor4ub(color[0], color[1], color[2], color[3]);
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
    glColor4ub(color[0], color[1], color[2], color[3]);
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
    glColor4ub(color[0], color[1], color[2], color[3]);
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
    glColor4ub(color[0], color[1], color[2], color[3]);
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
    glColor4ub(color[0], color[1], color[2], color[3]);
    glVertex2i(center.x, center.y - height / 2);
    glVertex2i(circle_center.x - sqrt(2) * width / 3, circle_center.y - width / 6);

    glEnd();
    DrawCircle(circle_center, width / 2, 1024, GL_LINE_LOOP, 0.0, 2 * M_PI, stroke, color);
}


void DrawSeven(struct Vec2 center, int width, int height, float stroke, GLubyte color[]){

    glLineWidth(stroke);    
    glBegin(GL_LINES);
    glColor4ub(color[0], color[1], color[2], color[3]);
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
    glColor4ub(color[0], color[1], color[2], color[3]);
    glVertex2i(center.x, center.y + height / 2);
    glVertex2i(circle_center.x + sqrt(2) * width / 3, circle_center.y + width / 6);

    glEnd();
    DrawCircle(circle_center, width / 2, 1024, GL_LINE_LOOP, 0.0, 2 * M_PI, stroke, color);
}

void DrawColon(struct Vec2 center, int width, int height,float stroke, GLubyte color[]){

    glPointSize(stroke);    
    glBegin(GL_POINTS);
    glColor4ub(color[0], color[1], color[2], color[3]);
    glVertex2i(center.x, center.y - height / 4);
    glVertex2i(center.x, center.y + height / 4);

    glEnd();    
}