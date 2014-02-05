#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <widget.h>
#include "compiler.h"

void event_handler(struct widget *sub, struct widget *pub, si_t event)
{
    NOT_USED(sub);
    NOT_USED(event);
    puts("hello world");
    timer_run(TIMER_POINTER(pub));
}

int main(int argc, char *argv[])
{
    struct timer *timer = timer_init(0);
    struct timespec ts = {
        .tv_sec=0,
        .tv_nsec=0
    };
    if(argc<3) {
        printf("Usage: %s <sec> <nsec>\n", argv[0]);
        exit(1);
    }
    ts.tv_sec = atoi(argv[1]);
    ts.tv_nsec = atoi(argv[2]);
    timer_set_time(timer, &ts);
    timer_register_event_handler(timer, NULL, TIMER_EVENT_ALL, event_handler);
    timer_run(timer);
    sleep(10);
    return 0;
}

