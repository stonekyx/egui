#include <stdio.h>
#include <ctype.h>
#include <termios.h>
#include <fcntl.h>
#include <widget.h>

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
    time_t t = time(NULL);
    printf("\033[%dD", strlen(buf));
    strftime(buf, sizeof(buf), "%T", localtime(&t));
    printf("%s", buf);
    fflush(stdout);
    timer_run(TIMER_POINTER(pub));
}

int main()
{
    struct timer *timer = timer_init(0);
    struct timespec ts = {
        .tv_sec = 1,
        .tv_nsec = 0
    };
    int ch;
    term_init();
    timer_set_time(timer, &ts);
    timer_register_event_handler(timer, NULL, TIMER_EVENT_ALL, event_handler);
    event_handler(NULL, WIDGET_POINTER(timer), TIMER_EVENT_ALL);
    while(tolower(ch = fgetc(stdin))!='q');
    term_exit();
    puts("");
    return 0;
}

