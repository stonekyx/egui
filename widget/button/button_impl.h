#ifndef _BUTTON_IMPL_H_
#define _BUTTON_IMPL_H_ 1

#include "../widget/widget_impl.h"

struct ButtonImpl : public WidgetImpl {
    static const char *const config_name;
    static const char *config_path;
    Button *owner;
    /* 文字 */
    char *text;
    /* 字体 */
    si_t font;
    /* extra data */
    addr_t custom_data;

    struct ButtonStyle : public WidgetImpl::WidgetStyle {
        ButtonStyle(
                const WidgetImpl::WidgetStyle &base,
                const si_t &font):
            WidgetStyle(base),
            font(font)
        {
        }

        si_t font;
    };

    static ButtonStyle default_style;
    virtual ButtonStyle *get_default_style();
    virtual const char *get_config_path();

    virtual struct config_parser *read_style_config();
    virtual void init_with_default_style();
};

#endif
