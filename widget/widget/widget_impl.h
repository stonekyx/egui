#ifndef _WBL_WIDGET_IMPL_H_
#define _WBL_WIDGET_IMPL_H_ 1

struct WidgetImpl {
    static const char *const config_name;
    static const char *config_path;
    Widget *owner;
    WidgetImpl(Widget * p);

    struct WidgetStyle {
        WidgetStyle(
                const si_t &flag,
                const rectangle &area,
                const si_t &border_size,
                const int_range_t &width_range,
                const int_range_t &height_range,
                const si_t &cursor,
                const color &back_color,
                const color &fore_color
                ):
            flag(flag),
            area(area),
            border_size(border_size),
            width_range(width_range),
            height_range(height_range),
            cursor(cursor),
            back_color(back_color),
            fore_color(fore_color)
        {
        }

        /* 一个标志 */
        si_t flag;

        /* 工作区域 */
        struct rectangle area;

        /* 边界宽度 */
        si_t border_size;

        /* 高度&宽度 */
        struct int_range_t width_range, height_range;

        /* 鼠标形状 */
        si_t cursor;

        /* 背景色 */
        struct color back_color;

        /* 前景色 */
        struct color fore_color;
    };

    static WidgetStyle default_style;
    virtual WidgetStyle *get_default_style();
    virtual const char *get_config_path();

    virtual struct config_parser *read_style_config();
    virtual void init_with_default_style();
};

#endif
