#include "Vec2.h"

struct Vec2 add(struct Vec2 a, struct Vec2 b){
    struct Vec2 temp;
    temp.x = a.x + b.x;
    temp.y = a.y + b.y;
    return temp;
}

struct Vec2 sub(struct Vec2 a, struct Vec2 b){
    struct Vec2 temp;
    temp.x = a.x - b.x;
    temp.y = a.y - b.y;
    return temp;

}