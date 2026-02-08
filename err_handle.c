//
// Created by loogze on 2026/2/6.
//

#include "err_handle.h"

int error_handle(enum err_state error){
    if (error == NO_ENOUGH_MEMORY ) {
        puts("Not enough memory");
        exit(-1);
    }
    if (error == EVENT_IS_EMPTY) {
        puts("Event is empty");
        exit(-2);
    }
    puts("why you calling err,or you just forget???");  // 5 stars.
    exit(174);
}
