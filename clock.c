#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main (void){
    time_t t;
    struct tm* ts;
    char* week [7] =  {"Sun","Mon","Tue", "Wed", "Thu", "Fri", "Sat"};
    while(1){
    time(&t);
    ts = localtime(&t);
    printf("%d年%2d月%2d日 (%s) %2d時%2d分%2d秒\n",ts->tm_year + 1900, ts->tm_mon + 1, ts->tm_mday,week[ts->tm_wday], ts->tm_hour, ts->tm_min, ts->tm_sec);
    sleep(1);
    }
    return 1;
}