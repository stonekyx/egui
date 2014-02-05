#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <linux/fb.h>
#include <stropts.h>

#include "event.h"
#include "config.h"
#include "compiler.h"

#define DEV_PATH "/dev/input"

static struct input_device mice[256];
static size_t micecnt;
static struct list message_lists[256];
static struct point curpos[256], initpos[256];

static struct fb_var_screeninfo var_screen_info;
static struct fb_fix_screeninfo fix_screen_info;

static void simp_screen_init(const char *path)
{
    int fd = open(path, O_RDWR);
    if (fd < 0) {
        exit(1);
    }
    if (-1 == ioctl(fd, FBIOGET_VSCREENINFO, &var_screen_info)) {
        exit(1);
    }
    if (-1 == ioctl(fd, FBIOGET_FSCREENINFO, &fix_screen_info)) {
        exit(1);
    }
    close(fd);
}

static si_t input_handler(struct egui_uds *uds, addr_t arg)
{
    size_t id = (size_t) arg;
    NOT_USED(uds);
    if (0 != mice[id].deal_with_input(mice + id, message_lists + id)) {
        return SELECTER_RETURN_TYPE_CONTINUE;
    }
    while (!list_empty(message_lists + id)) {
        union message *message = list_front(message_lists + id);
        struct point oldpos = curpos[id];
        if (message->base.type == MESSAGE_TYPE_MOUSE_MOVE) {
            if (curpos[id].x == -1 || curpos[id].y == -1) {
                initpos[id].x = var_screen_info.xres / 4;
                initpos[id].y = var_screen_info.yres / 2;
                curpos[id] = initpos[id];
            }
            if (message->mouse.code == INPUT_CODE_MOUSE_X_OFFSET) {
                curpos[id].x += message->mouse.value;
            }
            else if (message->mouse.code == INPUT_CODE_MOUSE_Y_OFFSET) {
                curpos[id].y += message->mouse.value;
            }
        }
        else if (message->base.type == MESSAGE_TYPE_MOUSE_MOVE_POINT) {
            if (message->mouse.code == INPUT_CODE_MOUSE_X_OFFSET) {
                int first = curpos[id].x==-1;
                curpos[id].x =
                    (double)message->mouse.value / MOUSE_RESOLUTION *
                    var_screen_info.xres;
                if(first) {
                    initpos[id] = curpos[id];
                }
            }
            else if (message->mouse.code == INPUT_CODE_MOUSE_Y_OFFSET) {
                curpos[id].y =
                    (double)message->mouse.value / MOUSE_RESOLUTION *
                    var_screen_info.yres;
            }
        }
        list_pop_front(message_lists + id);
        if (curpos[id].x < oldpos.x) {
            initpos[id] = curpos[id];
            return SELECTER_RETURN_TYPE_CONTINUE;
        }
        if (curpos[id].x - initpos[id].x >= (si_t) var_screen_info.xres / 2) {
            puts(mice[id].uds.file_path);
            fflush(stdout);
            return SELECTER_RETURN_TYPE_END;
        }
    }
    return SELECTER_RETURN_TYPE_CONTINUE;
}

static void input_destructor(struct egui_uds *uds, addr_t arg)
{
    size_t id = (size_t) arg;
    NOT_USED(uds);
    mice[id].input_exit(mice + id);
}

static struct event_listener *event_init()
{
    DIR *dev = opendir(DEV_PATH);
    struct dirent *entry;
    static struct event_listener res;
    event_listener_init(&res);
    micecnt = 0;
    while ((entry = readdir(dev))) {
        struct stat buf;
        static char path[256];
        snprintf(path, sizeof(path), "%s/%s", DEV_PATH, entry->d_name);
        stat(path, &buf);
        if (!S_ISCHR(buf.st_mode) || !strstr(entry->d_name, "event"))
            continue;
        mouse_init(&mice[micecnt], path, 1000);
        mice[micecnt].uds.file_path = strdup(path);
        list_init(message_lists + micecnt);
        curpos[micecnt].x = -1;
        curpos[micecnt].y = -1;
        event_listener_add_read_event(&res, &mice[micecnt].uds,
                                      (addr_t) micecnt, input_handler,
                                      input_destructor);
        micecnt++;
    }
    return &res;
}

int main()
{
    simp_screen_init("/dev/fb0");
    event_listener_exec(event_init());
    return 0;
}
