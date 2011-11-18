#ifndef YACC_H
#define YACC_H

#include "lex.h"

//TODO ast_type

#define AST_HEAD				\
	ast_type type;				\
	char* (*codegen)(AST *);	\

#define AST_STAT_HEAD			\
	AST_HEAD

enum factor_type{
	symbol_factor_type,
	number_factor_type,
	expr_factor_type
};

struct AST{
	AST_HEAD
};
struct AST_main_program{
	AST_HEAD
	AST_program * program;
};
struct AST_program{
	AST_HEAD
	AST_const_block * const_block;
	AST_var_block * var_block;
	AST_procedure_block * procedure_block;
	AST_stat * stat;
};
struct AST_const_block{
	AST_HEAD
	AST_const_block * const_define;
};
struct AST_const_define{
	AST_HEAD
	AST_const_define * const_define;
	AST_symbol * symbol;
	AST_number * number;
};
struct AST_number{
	AST_HEAD
	int number;
};
struct AST_var_block{
	AST_HEAD
	AST_var_block * var_block;
	AST_symbol * symbol;
};
struct AST_symbol{
	AST_HEAD
	char *str;
};
struct AST_procedure_block{
	AST_HEAD
	AST_symbol * func_name;
	AST_program * program;
	AST_procedure_block * next_func;
};
struct AST_stat{
	AST_STAT_HEAD
};
struct AST_var_stat{
	AST_STAT_HEAD
	AST_symbol * symbol;
	AST_expr * expr;
};
struct AST_if_stat{
	AST_STAT_HEAD
	AST_condition * condition;
	AST_expr * expr;
};
struct AST_while_stat{
	AST_STAT_HEAD
	AST_condition * condition;
	AST_expr * expr;
};
struct AST_call_stat{
	AST_STAT_HEAD
	AST_symbol * symbol;
};
struct AST_read_stat{
	AST_STAT_HEAD
	AST_read_stat * next_read_stat;
	AST_symbol * symbol;
};
struct AST_write_stat{
	AST_STAT_HEAD
	AST_write_stat * next_write_stat;
	AST_symbol * symbol;
};
struct AST_complex_stat{
	AST_STAT_HEAD
	AST_stat * stat;
	AST_complex_stat * next_stat;
};
struct AST_condition{
	AST_HEAD
	token_type condition_type;//看type是否等于odd了
	AST_expr * left_expr;
	AST_expr * right_expr;
};
struct AST_expr{
	AST_HEAD
	token_type unop;
	AST_term * left_term;
	token_type binop;
	AST_term * right_term;
};
struct AST_term{
	AST_HEAD
	AST_factor * left_factor;
	token_type binop;
	AST_factor * right_factor;
};
struct AST_factor{
	AST_HEAD
	factor_type fact_type;//看type区分
	AST * factor;
};

AST * yacc(token_list*);

#endif // YACC_H


