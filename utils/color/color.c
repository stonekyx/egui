#include "color.h"

static double
get_std_alpha
(si_t alpha)
{
    return 1-(double)alpha/COLOR_ALPHA_MAX;
}

static si_t
get_cus_alpha
(double alpha)
{
    return (si_t)((1-alpha)*COLOR_ALPHA_MAX);
}

struct color *
alpha_blend
(struct color *a,
 struct color *b)
{
    static struct color ans;
    double oa,
           aa,
           ba;

    a->r &= (1<<8)-1;
    a->g &= (1<<8)-1;
    a->b &= (1<<8)-1;
    a->a &= (1<<8)-1;
    b->r &= (1<<8)-1;
    b->g &= (1<<8)-1;
    b->b &= (1<<8)-1;
    b->a &= (1<<8)-1;
    aa=get_std_alpha(a->a);
    if(b->a <= 0) {
        ans.a=0;
        ans.r=a->r * aa + b->r * (1-aa);
        ans.g=a->g * aa + b->g * (1-aa);
        ans.b=a->b * aa + b->b * (1-aa);
        return &ans;
    }

    ba=get_std_alpha(b->a);

    oa = aa + ba * (1-aa);

    ans.r = (a->r * aa + b->r * ba * (1-aa)) / oa;
    ans.g = (a->g * aa + b->g * ba * (1-aa)) / oa;
    ans.b = (a->b * aa + b->b * ba * (1-aa)) / oa;
    ans.a = get_cus_alpha(oa);

    return &ans;
}
