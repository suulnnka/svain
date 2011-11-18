#ifndef BASICIO_H
#define BASICIO_H

#include "string_list.h"

string_list * read_src(char *);
int write_obj(char *,string_list *);

#endif // BASICIO_H
