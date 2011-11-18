#ifndef LEX_H
#define LEX_H

#include "string_list.h"

enum token_type{

	number_token,

	if_iden,		//if
	then_iden,		//then
	while_iden,		//while
	do_iden,		//do
	read_iden,		//read
	write_iden,		//write
	call_iden,		//call

	const_iden,		//const
	var_iden,		//var
	procedure_iden,	//procedure

	begin_iden,		//begin
	end_iden,		//end

	val,			//:=

	period,			//.
	comma,			//,
	semicolon,		//;

	odd_iden,		//odd

	add_token,		//+
	sub_token,		//-
	mul_token,		//*
	div_token,		///

	equal,			//=
	not_equal,		//#
	less,			//<
	more,			//>
	not_less,		//>=
	not_more,		//<=

	left_rnd,		//(
	right_rnd,		//)

	wrong_number,
	wrong_iden,
	wrong_symbol,

	eof_token,

	symbol_token,

	root_token,

	none_token,

	iden			//other iden
};
enum lex_state{
	none_s,
	number_s,
	string_s,
	symbol_s,
	space_s
};

struct token_list{
	token_type token;
	char *str;
	int number;
	token_list *next;
	token_list *prev;
};

int token_list_del(token_list *);

token_list * lex(string_list *);

token_list * number_lex(string_list *str,token_list * node,char *ch);

token_list * string_lex(string_list *str,token_list * node,char *ch);

token_list * symbol_lex(string_list *str,token_list * node,char *ch);


#endif // LEX_H
