#ifndef LEX_H
#define LEX_H

#include "string_list.h"

struct token_list{
	token_type token;
	char *str;
	long long int int_num;
	double float_num;
	token_list *next;
	token_list *prev;
};

int token_list_del(token_list *);

token_list * lex(string_list *);

#endif // LEX_H
