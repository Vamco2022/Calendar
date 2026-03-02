//
// Created by loogze on 2026/2/6.
//

#include "../include/err_handle.h"

int error_handle(enum err_state error){

    switch (error) {
        case NO_ENOUGH_MEMORY   : perror("Not enough memory");
                                  exit(-1);
        case EVENT_IS_EMPTY     : perror("Event is empty");
                                  exit(-2);
        case LIST_HEAD_IS_EMPTY : perror("List Head IS NULL!");
                                  exit(-3);
        default                 : perror("why you calling err,or you just forget???");
                                  exit(174);/* 114514 stars.*/
    }
}
