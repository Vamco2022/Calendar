#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../Compound/inc/string.h"

#include "../include/struct.h"
#include "../include/err_handle.h"
#include "../include/event.h"
/* 事件的结构体：
 *   1. 事件开始时间
 *   2. 事件时长
 *   3. 事件标题
 *   4. 事件描述
 */


/* 事件对象的:
 *   1. 创建
 *   2. 复制
 *   3. 销毁。 */


// EventPtr Create_An_Event(const char *Title, const char *Description, const time_t Start, const time_t End) {
//     EventPtr p;
//     p = malloc(sizeof(Event));
//     if (p == NULL) error_handle(NO_ENOUGH_MEMORY);
//     strcpy(p->Title, Title);
//     strcpy(p->Description, Description);
//     p->StartTime = Start;
//     p->EndTime = End;
//     return p;
// }
//
// int Delete_An_Event(Event *p) {
//     if (p != NULL) {
//         free(p);
//     } else {
//         error_handle(EVENT_IS_EMPTY);
//     }
// }

// 链表的定义
EventListPtr ListHead = NULL;
EventListPtr ListEnd = NULL; // position

FullEventListPtr Init_Eventlist(EventPtr eventptr) {
    const EventListPtr start = malloc(sizeof(EventList));
    if (start == NULL) error_handle(NO_ENOUGH_MEMORY);
    const EventListPtr end = malloc(sizeof(EventList));
    if (end == NULL) {
        free(start);
        error_handle(NO_ENOUGH_MEMORY);
    }
    start->state = ADD;
    start->event_data = eventptr;
    start->occur_time = eventptr->StartTime;
    end->state = REMOVE;
    end->event_data = eventptr;
    end->occur_time = eventptr->EndTime;
    FullEventListPtr F_event;
    if ((F_event = malloc(sizeof(FullEventList)))) {
        F_event->start = start;
        F_event->end = end;
        return F_event;
    }
    free(start);
    free(end);
    error_handle(NO_ENOUGH_MEMORY);
    return NULL;
}

void JoinIntoEventlist(EventListPtr NeedToJoinPtr) {
    EventListPtr p = ListHead;
    while (NeedToJoinPtr->event_data < p->event_data) {
        p = p->next;
    }
    NeedToJoinPtr->next = p->next;
    p->next = NeedToJoinPtr;
}


void ADD_EventList(EventPtr event) {
    FullEventListPtr F_event = Init_Eventlist(event);
    EventListPtr start = F_event->start;
    EventListPtr end = F_event->end;
    free(F_event);
    if (ListEnd == NULL) {
        // 第一次创建的处理
        ListHead = start;
        start->next = end;
        ListEnd = end;
        ListEnd->next = NULL;
    } else {
        // "就是要自己删"  -- 什么澈 (Mon 12 Jan 16:26:25 CST 2026)

        /*                     -----[-------------{}------]-----------{-}----------*--------[--]--                */
        /*                             ^                            ^  ^                                          */
        /*                ListEnd->occur_time          >?             event->StartTime                            */
        /*                                       ->                                                               */

        if (ListEnd->occur_time <= event->StartTime) {
            // 事件在最后
            ListEnd->next = start;
            start->next = end;
            ListEnd = end;
            return;
        }
        if (ListHead->occur_time > event->EndTime) {
            // 事件在最前
            end->next = ListHead;
            start->next = end;
            ListHead = start;
        } // "Warning: the condition is always true."  -- CLion (Mon 12 Jan 16:50:21 CST 2026)
        JoinIntoEventlist(end);
        JoinIntoEventlist(start);
    }
}

static time_t time_yet;

void CheckList() {
    const time_t now = time(NULL);
    if (time_yet == now) return;
    time_yet = now;
    if (ListHead == NULL || ListHead->occur_time != now) return;
    const EventListPtr needToDelete = ListHead;
    if (ListHead->next != NULL)
        ListHead = ListHead->next;
    free(needToDelete);
}

void PrintEventList(EventList *const list) {
    if (!list) {
        return;
    }

    const EventList *current_list = list;
    Event *current = current_list->event_data;
    do {
        printf("[%d] %lld %s\n", list->state, current->StartTime, current->Title);

        current_list = current_list->next;

        current = current_list->event_data;
    } while (current);
}

// typedef struct StampNode {
//     time_t stamp;
//     struct EventNode *next;
// } stamp_node,*stamp_node_ptr;
//
// struct StampNode StampChain[1024] = {0};
//

// int _main(void) {
//
//     Event *event1 = Create_An_Event("this is a title","aabababa", time(NULL), time(NULL) + 36 * 60);
//     Delete_An_Event(event1);
//
//     return 0;
// }

int Calendar(const Array(ptr) *const args) {
    if (!args) {
        return -1;
    }

    Event *event1 = Create_An_Event("Go shopping", "Buy some apples.", time(NULL), time(NULL) + 36 * 60);
    Event *event2 = Create_An_Event("aadadada", "adawdawdad", time(NULL), time(NULL) + 24 * 60);

    Delete_An_Event(event1);
    Delete_An_Event(event2);

    return 0;
}

size_t PrintUsage(const char *restrict const program_name) {
    return fprintf(stderr,
                   "USAGE: %s [COMMAND...]"NEWLINE
                   "COMMAND:"NEWLINE
                   "\tadd <name> <start_timestamp> <duration_seconds>"NEWLINE
                   "\t\tCreate an event."NEWLINE
                   "\tdel <name>"NEWLINE
                   "\t\tDelete an event."NEWLINE
                   /* "\t-r --rename <old_name> <new_name>"NEWLINE */
                   "\tremain <name>"NEWLINE
                   "\t\tOutput remaining time in seconds.",
                   program_name
    );
}

int ProcessArguments(const Array(ptr) *args) {
    if (!args || args->capacity <= 1) {
        PrintUsage((char *) fallback(get(ptr, args, 0)));
        return 0;
    }

    const String *const COMMAND_ADD = string("add");
    const String *const COMMAND_DEL = string("del");
    const String *const COMMAND_REMAIN = string("remain");

    for (register llong i = 1; i < args->capacity; i++) {
        const String *const current = get(ptr, args, i);
        if (!current) {
            continue;
        }

        if (!compare(current, COMMAND_ADD)) {
            i++;
        }
    }

    Delete(String, &COMMAND_REMAIN);
    Delete(String, &COMMAND_DEL);
    Delete(String, &COMMAND_ADD);

    return 0;
}

// 怎么获取时间来着？  -- 某某澈  Sat 21 Feb 21:09:49 CST 2026
int main(const int argc, const char *restrict const *restrict const argv) {
    Array(ptr) *args = array(ptr, argc);
    if (!args) {
        perror("Cannot initialise args.");
        return 1;
    }

    iterate(i, args) {
        set(ptr, args, i, string(argv[i]));
    }

    ProcessArguments(args);

    const int retval = Calendar(args);

    eraseref(String, args);
    Delete(Array(ptr), &args);

    return retval;
}

/*  1       2         3   4       5  6   */
/* [...]   [.....]   [...[...]..] ｛  [  } ]*/
/* [<--]---[<----]---[<--[...]..]<｛<-[<-}-]*/
/* [...]   [.....]   [...[...]..] ｛  [  } ]*/
/*                             |           */
/* 1   0   1     0   1   2   1  0 1  2  1 0*/
/* 1   1   1     1   1   1   1  1 1  1  1 1*/

// Event TIMETABLE[1024] = {0};  // 栈
//
// inline bool Within(const time_t moment, const time_t start, const time_t end)
// {
//     if (end < start) {
//         return false;
//     }
//
//     return (moment >= start && moment <= end);
// }
//
// void Mark(Event *const timetable, const size_t length, const time_t moment)
// {
//     if (!timetable || !length) {
//         return;
//     }
//
//     for (register size_t i = 0; i < length; i++) {
//         if (timetable[i].outofdate) {
//             continue;
//         }
//
//         if (timetable[i].StartTime < moment && timetable[i].EndTime < moment) {
//             timetable[i].outofdate = true;
//
//             continue;
//         }
//
//         timetable[i].within = Within(moment, timetable[i].StartTime, timetable[i].EndTime);
//     }
// }
//
// //                       Title, StartTime, EndTime, Description
// # define EVENT_FORMAT  "[%s] (%ld-%ld)\n\t\"%s\""
//
// void PrintEvent(const Event *const event)
// {
//     printf(EVENT_FORMAT, event->Title, event->StartTime, event->EndTime, event->Description);
// }
//
// void PrintOnGoingEvents(Event *const timetable, const size_t length)
// {
//     if (!timetable || !length) {
//         return;
//     }
//
//     for (register size_t i = 0; i < length; i++) {
//         if (timetable[i].within) {
//             PrintEvent(&timetable[i]);
//         }
//     }
// }
