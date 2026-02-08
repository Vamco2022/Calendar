//
// Created by loogze on 2026/2/6.
//

#ifndef CALENDAR_ERR_HANDLE_H
#define CALENDAR_ERR_HANDLE_H

#include <stdio.h>
#include <stdlib.h>

enum err_state {
    NO_ENOUGH_MEMORY,
    EVENT_IS_EMPTY,
};

int error_handle(enum err_state error);

#endif //CALENDAR_ERR_HANDLE_H