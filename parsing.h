#ifndef PARSING_H
#define PARSING_H

#include "lex.h"

#define AST_HEAD					\
	ast_type type;					\
	char* (*codegen)(struct AST *);	\
	int (*del)(struct AST *);

#define AST_STAT_HEAD				\
	AST_HEAD

enum factor_type{
	symbol_factor_type,
	number_factor_type,
	expr_factor_type
};

enum ast_type{

	root_type,
	none_type,

	main_program_type,
	program_type,
	const_block_type,
	const_define_type,
	number_type,
	var_block_type,
	symbol_type,
	procedure_type,

	stat_type,
	none_stat_type,
	var_stat_type,
	if_stat_type,
	while_stat_type,
	call_stat_type,
	read_stat_type,
	write_stat_type,
	complex_stat_type,
	condition_type,
	expr_type,
	term_type,
	factor_type

};

struct AST{
	AST_HEAD
};
struct AST_main_program{
	AST_HEAD
	struct AST_program * program;
};
struct AST_program{
	AST_HEAD
	struct AST_const_block * const_block;
	struct AST_var_block * var_block;
	struct AST_procedure_block * procedure_block;
	struct AST_stat * stat;
};
struct AST_const_block{ //可为none
	AST_HEAD
	struct AST_const_define * const_define;
};
struct AST_const_define{
	AST_HEAD
	struct AST_const_define * const_define;
	struct AST_symbol * symbol;
	struct AST_number * number;
};
struct AST_number{
	AST_HEAD
	int number;
};
struct AST_var_block{
	AST_HEAD
	struct AST_var_block * var_block;
	struct AST_symbol * symbol;
};
struct AST_symbol{
	AST_HEAD
	char *str;
};
struct AST_procedure_block{
	AST_HEAD
	struct AST_symbol * func_name;
	struct AST_program * program;
	struct AST_procedure_block * next_func;
};
struct AST_stat{
	AST_STAT_HEAD
};
struct AST_var_stat{
	AST_STAT_HEAD
	struct AST_symbol * symbol;
	struct AST_expr * expr;
};
struct AST_if_stat{
	AST_STAT_HEAD
	struct AST_condition * condition;
	struct AST_stat * stat;
};
struct AST_while_stat{
	AST_STAT_HEAD
	struct AST_condition * condition;
	struct AST_stat * stat;
};
struct AST_call_stat{
	AST_STAT_HEAD
	struct AST_symbol * symbol;
};
struct AST_read_stat{
	AST_STAT_HEAD
	struct AST_read_stat * next_read_stat;
	struct AST_symbol * symbol;
};
struct AST_write_stat{
	AST_STAT_HEAD
	struct AST_write_stat * next_write_stat;
	struct AST_symbol * symbol;
};
struct AST_complex_stat{
	AST_STAT_HEAD
	struct AST_stat * stat;
	struct AST_complex_stat * next_stat;
};
struct AST_condition{
	AST_HEAD
	token_type condition_type;//看type是否等于odd了
	struct AST_expr * left_expr;
	struct AST_expr * right_expr;
};
struct AST_expr{
	AST_HEAD
	token_type op;
	struct AST_term * term;
	struct AST_expr * next;
};
struct AST_term{
	AST_HEAD
	token_type op;
	struct AST_factor * factor;
	struct AST_term * next;
};
struct AST_factor{
	AST_HEAD
	factor_type fact_type;//看type区分
	struct AST * factor;
};

AST * parsing(token_list*);

#endif // parsing_H


