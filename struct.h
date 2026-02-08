//
// Created by loogze on 2026/2/6.
//

#ifndef CALENDAR_STRUCT_H
#define CALENDAR_STRUCT_H
#include <time.h>
#include <stdbool.h>

typedef struct Time {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
} tm;

typedef struct Event {
    char Title[50];
    char Description[512];
    time_t StartTime;
    time_t EndTime;
    bool within;
    bool outofdate;
} Event,*EventPtr;

enum EVENT_STATE {
    ADD, REMOVE,
};

typedef struct EventLink {
    // 起始/ 终止
    EventPtr event_data;
    time_t occur_time;
    enum EVENT_STATE state; // 类型：增加/ 删除
    struct EventLink *next;
} EventList, *EventListPtr;

typedef struct FullEventList {
    EventListPtr start;
    EventListPtr end;
}FullEventList,*FullEventListPtr;

#endif //CALENDAR_STRUCT_H