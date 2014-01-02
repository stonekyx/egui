#ifndef _WBL_WIDGET_IMPL_H_
#define _WBL_WIDGET_IMPL_H_ 1

#include <iostream>

using namespace std;

struct WidgetImpl {
    static const char *const config_name;
    static const char *config_path;
    Widget *owner;
    WidgetImpl(Widget * p);

    struct WidgetStyle {
        WidgetStyle(std::initializer_list<>);
        /* 一个标志 */
        si_t flag;

        /* 工作区域 */
        si_t area_x;
        si_t area_y;
        si_t area_width;
        si_t area_height;

        /* 边界宽度 */
        si_t border_size;

        /* 高度&宽度 */
        si_t maximum_width;
        si_t minimum_width;
        si_t maximum_height;
        si_t minimum_height;

        /* 鼠标形状 */
        si_t cursor;

        /* 背景色 */
        si_t back_color_r;
        si_t back_color_g;
        si_t back_color_b;
        si_t back_color_a;

        /* 前景色 */
        si_t fore_color_r;
        si_t fore_color_g;
        si_t fore_color_b;
        si_t fore_color_a;
    };

    static WidgetStyle default_style;
    virtual WidgetStyle *get_default_style();
    virtual const char *get_config_path();

    virtual struct config_parser *read_style_config();
    virtual void init_with_default_style();
};

#endif
