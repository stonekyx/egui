#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([egui], [0.1], [stonekyx@gmail.com])
AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([subdir-objects])
LT_INIT

# Input device paths.
AC_ARG_WITH([mouse],
            AS_HELP_STRING([--with-mouse=PATH],
                           [specify path to the mouse event device]),
            [],
            []
            )
AC_ARG_WITH([kbd],
            AS_HELP_STRING([--with-kbd=PATH],
                           [specify path to the keyboard event device]),
            [],
            []
            )
AS_IF([test "$with_kbd"],
      [],
      [read with_kbd <<< `ls -1 /dev/input/by-path/*event*kbd* /dev/input/by-path/*kbd*event* 2>/dev/null | head -1`])
AS_IF([test "$with_mouse"],
      [],
      [read with_mouse <<< `ls -1 /dev/input/by-path/*event*mouse* /dev/input/by-path/*mouse*event* 2>/dev/null | head -1`])
AC_DEFINE_UNQUOTED([MOUSE_EVENT_DEVICE], ["$with_mouse"], [Path to mouse event device.])
AC_DEFINE_UNQUOTED([KBD_EVENT_DEVICE], ["$with_kbd"], [Path to keyboard event device.])

# Mouse-moving resolution on EV_ABS events
AC_ARG_WITH([mouse-resolution],
            AS_HELP_STRING([--with-mouse-resolution=NUMBER],
                           [specify mouse moving resolution for some devices. Bigger number means higher resolution.]),
            [],
            []
            )
AS_IF([test "$with_mouse_resolution"],
      [],
      [let with_mouse_resolution=65536])
AC_DEFINE_UNQUOTED([MOUSE_RESOLUTION], [$with_mouse_resolution], [Mouse-moving resolution.])

# Whether use fbtools
AC_ARG_ENABLE([fbtools],
              AS_HELP_STRING([--enable-fbtools=yes],
                             [specify whether fbtools should be used.]),
              [],
              []
              )
AS_IF([test "$enable_fbtools" = "no"],
      [FBTOOLS_SYSTEM=""],
      AC_DEFINE([USE_FBTOOLS], [], [Enable fbtools.])
      FBTOOLS_SYSTEM='graph/graph_lower/fbtools.lo'
      )
AC_SUBST([FBTOOLS_SYSTEM])

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h stddef.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/time.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T

AC_LANG([C])

my_save_cflags="$CFLAGS"
CFLAGS="-std=c99 -pedantic -Wall -g"
AC_MSG_CHECKING([whether CC supports -std=c99 -pedantic -Wall -g])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
                [AC_MSG_RESULT([yes])]
                [AM_CFLAGS="-std=c99 -Wall -g"],
                [AC_MSG_RESULT([no])]
                [AC_MSG_WARN([Your compiler may not support c99, compiling is likely to fail.])]
                )
AC_SUBST([AM_CFLAGS])

CFLAGS="-I/usr/include"
AC_MSG_CHECKING([whether CC supports -I])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
                [AC_MSG_RESULT([yes])]
                [AM_CPPFLAGS='-I$(top_srcdir)/include'],
                [AC_MSG_RESULT([no])]
                [AC_MSG_WARN([Your compiler may not support -I flag, compiling is very likely to fail.])]
                )
AC_SUBST([AM_CPPFLAGS])
CFLAGS="$my_save_cflags"

# Checks for __attribute__((packed))
AC_MSG_CHECKING([whether CC supports __attribute__((packed))])
AC_RUN_IFELSE([
               AC_LANG_SOURCE([[struct test_t{char c; long l;}__attribute__((packed));
                               int main(){
                               return sizeof(struct test_t)!=sizeof(char)+sizeof(long);}
                               ]])
               ],
               [echo "test program returned $?"],
               [AC_MSG_FAILURE([test program returned $?])]
               )

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([getcwd memmove memset munmap select socket strchr strerror strstr])

# Substitutions

AC_CONFIG_FILES([Makefile])
#                 application/Makefile
#                 client_lib/Makefile
#                 comm/Makefile
#                 debug/Makefile
#                 debug/comm/Makefile
#                 debug/config_parser/Makefile
#                 debug/event/Makefile
#                 debug/graph/Makefile
#                 debug/lib/Makefile
#                 debug/samples/Makefile
#                 debug/samples/calculator/Makefile
#                 debug/samples/file_browser/Makefile
#                 debug/widget/Makefile
#                 event/Makefile
#                 graph/Makefile
#                 graph/graph_engine/Makefile
#                 graph/graph_lower/Makefile
#                 server_lib/Makefile
#                 utils/Makefile
#                 utils/color/Makefile
#                 utils/config_parser/Makefile
#                 utils/data_structures/Makefile
#                 utils/geometry/Makefile
#                 widget/Makefile
#                 widget/button/Makefile
#                 widget/dialog/Makefile
#                 widget/icon/Makefile
#                 widget/image_view/Makefile
#                 widget/label/Makefile
#                 widget/message_box/Makefile
#                 widget/panel/Makefile
#                 widget/scroll_bar/Makefile
#                 widget/text_line/Makefile
#                 widget/widget/Makefile
#                 widget/window/Makefile
#                 window_manager/Makefile])
AC_OUTPUT
