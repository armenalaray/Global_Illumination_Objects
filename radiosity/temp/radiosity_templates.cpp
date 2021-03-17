#include "radiosity_templates.h"

void list_user(List<int,My_allocator>& lm, List<int,My_allocator>& ly)
{
    list_fct(lm.begin(),lm.end());
    list_fct(ly.begin(),ly.end());
}


