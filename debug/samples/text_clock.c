#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <widget.h>
#include "compiler.h"

struct termios old_term;

void term_init()
{
    struct termios new_term;
    int fd = 0;
    tcgetattr(fd, &old_term);
    new_term = old_term;
    new_term.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(fd, TCSANOW, &new_term);
}

void term_exit()
{
    int fd = 0;
    tcsetattr(fd, TCSAFLUSH, &old_term);
}

void event_handler(struct widget *sub, struct widget *pub, si_t event)
{
    static char buf[100];
    struct timespec spec;
    NOT_USED(sub);
    NOT_USED(event);

    clock_gettime(CLOCK_REALTIME, &spec);

    printf("\033[%dD", strlen(buf));
    strftime(buf, sizeof(buf), "%T", localtime(&spec.tv_sec));
    sprintf(buf+strlen(buf), ".%06ld", spec.tv_nsec/1000);

    printf("%s", buf);
    fflush(stdout);
    timer_run(TIMER_POINTER(pub));
}

int main(int argc, char *argv[])
{
    struct timer *timer = timer_init(0);
    struct timespec ts;
    int ch;
    if(argc<3) {
        printf("Usage: %s <sec> <usec>\n", argv[0]);
        exit(1);
    }
    ts.tv_sec = atoi(argv[1]);
    ts.tv_nsec = atoi(argv[2]);

    term_init();
    timer_set_time(timer, &ts);
    timer_register_event_handler(timer, NULL, TIMER_EVENT_ALL, event_handler);
    event_handler(NULL, WIDGET_POINTER(timer), TIMER_EVENT_ALL);
    while(tolower(ch = fgetc(stdin))!='q');
    term_exit();
    puts("");
    return 0;
}

