#include <stdio.h>
#include <unistd.h>
#include <widget.h>

void event_handler(struct widget *pub, struct widget *sub, si_t event)
{
    puts("hello world");
}

int main()
{
    struct timer *timer = timer_init(0);
    struct itimerspec its = {
        .it_value = {
            .tv_sec=0,
            .tv_nsec=300000000
        }
    };
    timer_set_time(timer, &its);
    timer_register_event_handler(WIDGET_POINTER(timer), NULL, TIMER_EVENT_ALL, event_handler);
    timer_run(timer);
    sleep(10);
    return 0;
}

