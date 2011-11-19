#include "lex.h"
#include "parsing.h"
#include "string_list.h"
#include "base.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct token_list * ctl;//current_token_list

struct AST * parsing(struct token_list * list){
	struct AST * root;

	ctl = list;

	assert(ctl->token == root_token);
	assert(ctl->next);

	ctl = ctl->next;

	root = main_program_parsing(root);
	assert(ctl->token == none_token);

	return root;
}

struct AST * main_program_parsing(){
	struct AST_main_program *cur =
			(struct AST_main_program *)malloc(sizeof(struct AST_main_program));
	cur->type = main_program_type;
	cur->codegen = main_program_codegen;
	cur->del = main_program_del;
	cur->program = NULL;

	cur->program = program_parsing();

	assert((cur->program);
	assert(ctl->token == period);
	ctl = ctl->next;

	return (AST *)cur;
}

struct AST_program * program_parsing(){
	struct AST_program *cur =
			(struct AST_program *)malloc(sizeof(struct AST_program));
	cur->type = program_type;
	cur->codegen = program_codegen;
	cur->del = program_del;

	cur->const_block = NULL;
	cur->const_block = const_block_parsing();
	assert(cur->const_block);

	cur->var_block = NULL;
	cur->var_block = var_block_parsing();
	assert(cur->var_block);

	cur->procedure_block = NULL;
	cur->procedure_block = procedure_block_parsing();
	assert(cur->procedure_block);

	cur->stat = NULL;
	cur->stat = stat_parsing();
	assert(cur->stat);

	return cur;
}

struct AST_const_block * const_block_parsing(){
	struct AST_const_block *cur =
			(struct AST_const_block *)malloc(sizeof(struct AST_const_block));
	cur->type = const_block_type;
	cur->codegen = const_block_codegen;
	cur->del = const_block_del;

	cur->const_define = NULL;

	if(ctl->token != const_iden)
		return cur;
	else
		ctl = ctl->next;

	cur->const_define = const_define_parsing();
	assert(cur->const_define);

	return cur;
}

struct AST_const_define * const_define_parsing(){
	struct AST_const_define *cur =
			(struct AST_const_define *)malloc(sizeof(struct AST_const_define));
	cur->type = const_define_type;
	cur->codegen = const_define_codegen;
	cur->del = const_define_del;

	cur->const_define = NULL;
	cur->symbol = NULL;
	cur->number = NULL;

	cur->symbol = symbol_parsing();
	assert(cur->symbol);

	assert(ctl->token == equal);
	ctl = ctl->next;

	cur->number = number_parsing();
	assert(cur->number);

	assert(ctl->token == comma || ctl->token == semicolon)

	if(ctl->token == comma){
		ctl = ctl->next;
		cur->const_define = const_define_parsing();
		assert(cur->const_define);
	}else{
		ctl = ctl->next;
	}

	return cur;
}

struct AST_number * number_parsing(){
	struct AST_number *cur =
			(struct AST_number *)malloc(sizeof(struct AST_number));
	cur->type = number_type;
	cur->codegen = number_codegen;
	cur->del = number_del;

	assert(ctl->token == number_token);

	cur->number = ctl->number;
	ctl = ctl->next;

	return cur;
}

struct AST_var_block * var_block_parsing(){
	struct AST_var_block *cur =
			(struct AST_var_block *)malloc(sizeof(struct AST_var_block));
	cur->type = var_block_type;
	cur->codegen = var_block_codegen;
	cur->del = var_block_del;

	cur->var_block = NULL;
	cur->symbol = NULL;

	assert(ctl->token == var_iden);
	ctl = ctl->next;

	cur->symbol = symbol_parsing();
	assert(cur->symbol);

	assert(ctl->next == comma || ctl->token == semicolon);

	if(ctl->token == comma){
		ctl = ctl->next;
		cur->var_block = var_block_2nd_parsing();
		assert(cur->var_block);
	}else{
		ctl = ctl->next;
	}

	return cur;
}

struct AST_var_block * var_block_2nd_parsing(){
	struct AST_var_block *cur =
			(struct AST_var_block *)malloc(sizeof(struct AST_var_block));
	cur->type = var_block_type;
	cur->codegen = var_block_codegen;
	cur->del = var_block_del;

	cur->var_block = NULL;
	cur->symbol = NULL;

	cur->symbol = symbol_parsing();
	assert(cur->symbol);

	assert(ctl->next == comma || ctl->token == semicolon);

	if(ctl->token == comma){
		ctl = ctl->next;
		cur->var_block = var_block_2nd_parsing();
		assert(cur->var_block);
	}else{
		ctl = ctl->next;
	}

	return cur;
}

struct AST_symbol * symbol_parsing(){
	struct AST_symbol *cur =
			(struct AST_symbol *)malloc(sizeof(struct AST_symbol));
	cur->type = symbol_type;
	cur->codegen = symbol_codegen;
	cur->del = symbol_del;

	assert(ctl->token == iden);
	assert(strlen(ctl->str) > 0);

	cur->str = (char *)malloc(sizeof(ctl->str));
	strcpy(cur->str,ctl->str);

	return cur;
}

struct AST_procedure_block * procedure_parsing(){
	struct AST_procedure_block *cur =
			(struct AST_procedure_block *)malloc(sizeof(struct AST_procedure_block));
	cur->type = procedure_type;
	cur->codegen = procedure_codegen;
	cur->del = procedure_del;

	cur->func_name = NULL;
	cur->program = NULL;
	cur->next_func = NULL;

	if(ctl->token != procedure_iden)
		return cur;
	ctl = ctl->next;

	cur->func_name =  symbol_parsing();
	assert(cur->func_name);

	assert(ctl->token == semicolon);
	ctl = ctl->next;

	cur->program = program_parsing();
	assert(cur->program);

	assert(ctl->token == semicolon);
	ctl = ctl->next;

	cur->next_func = procedure_parsing();

	return cur;

}

struct AST_stat * stat_parsing(){
	AST_stat * cur = NULL;
	cur = (AST_stat *)var_stat_parsing();
	if(cur)
		return cur;
	cur = (AST_stat *)if_stat_parsing();
	if(cur)
		return cur;
	cur = (AST_stat *)while_stat_parsing();
	if(cur)
		return cur;
	cur = (AST_stat *)call_stat_parsing();
	if(cur)
		return cur;
	cur = (AST_stat *)read_stat_parsing();
	if(cur)
		return cur;
	cur = (AST_stat *)write_stat_parsing();
	if(cur)
		return cur;
	cur = (AST_stat *)complex_stat_parsing();
	if(cur)
		return cur;

	cur = (struct AST_stat *)malloc(sizeof(struct AST_stat));
	cur->type = none_stat_type;
	cur->codegen = non_stat_codegen;
	cur->del = non_stat_del;

	return cur;
}

struct AST_var_stat * var_stat_parsing(){
	AST_var_stat * cur = NULL;

	assert(ctl);
	assert(ctl->next);
	if(ctl->token != iden || ctl->next->token != equal)
		return (AST_stat *)cur;

	cur = (struct AST_var_stat *)malloc(sizeof(struct AST_var_stat));
	cur->type = var_stat_type;
	cur->codegen = var_stat_codegen;
	cur->del = var_stat_del;

	cur->symbol = NULL;
	cur->expr = NULL;

	cur->symbol =  symbol_parsing();
	assert(cur->symbol);

	assert(ctl->token == equal);
	ctl = ctl->next;

	cur->expr = expr_parsing();
	assert(cur->expr);

	return (AST_stat *)cur;
}

struct AST_if_stat * if_stat_parsing(){
	AST_if_stat * cur = NULL;

	assert(ctl);
	if(ctl->token != if_iden)
		return (AST_stat *)cur;
	ctl = ctl->next;

	cur = (struct AST_if_stat *)malloc(sizeof(struct AST_if_stat));
	cur->type = if_stat_type;
	cur->codegen = if_stat_codegen;
	cur->del = if_stat_del;

	cur->condition = NULL;
	cur->stat = NULL;

	cur->condition =  condition_parsing();
	assert(cur->condition);

	assert(ctl->token == then_iden);
	ctl = ctl->next;

	cur->stat = stat_parsing();
	assert(cur->stat);

	return (AST_stat *)cur;
}

struct AST_while_stat * while_stat_parsing(){
	AST_while_stat * cur = NULL;

	assert(ctl);
	if(ctl->token != while_iden)
		return (AST_stat *)cur;
	ctl = ctl->next;

	cur = (struct AST_while_stat *)malloc(sizeof(struct AST_while_stat));
	cur->type = while_stat_type;
	cur->codegen = while_stat_codegen;
	cur->del = while_stat_del;

	cur->condition = NULL;
	cur->stat = NULL;

	cur->condition =  condition_parsing();
	assert(cur->condition);

	assert(ctl->token == do_iden);
	ctl = ctl->next;

	cur->stat = stat_parsing();
	assert(cur->stat);

	return (AST_stat *)cur;
}

struct AST_call_stat * call_stat_parsing(){
	AST_call_stat * cur = NULL;

	assert(ctl);
	if(ctl->token != call_iden)
		return (AST_stat *)cur;
	ctl = ctl->next;

	cur = (struct AST_call_stat *)malloc(sizeof(struct AST_call_stat));
	cur->type = call_stat_type;
	cur->codegen = call_stat_codegen;
	cur->del = call_stat_del;

	cur->symbol = NULL;

	cur->symbol = symbol_parsing();
	assert(cur->symbol);

	return (AST_stat *)cur;
}

struct AST_read_stat * read_stat_parsing(){
	AST_read_stat * cur = NULL;

	assert(ctl);
	if(ctl->token != read_iden)
		return (AST_stat *)cur;
	ctl = ctl->next;

	cur = (struct AST_read_stat *)malloc(sizeof(struct AST_read_stat));
	cur->type = read_stat_type;
	cur->codegen = read_stat_codegen;
	cur->del = read_stat_del;

	cur->symbol = NULL;
	cur->next_read_stat = NULL;

	assert(ctl->token == left_rnd);
	ctl = ctl->next;

	cur->symbol = symbol_parsing();
	assert(cur->symbol);

	assert(ctl->token == right_rnd || ctl->token == comma);
	if(ctl->token == comma){
		ctl = ctl->next;
		cur->next_read_stat = next_read_stat_parsing();
		assert(cur->next_read_stat);
	}else{
		ctl = ctl->next;
	}

	return (AST_stat *)cur;
}

struct AST_read_stat * next_read_stat_parsing(){
	AST_read_stat * cur =
		cur = (struct AST_read_stat *)malloc(sizeof(struct AST_read_stat));
	cur->type = read_stat_type;
	cur->codegen = read_stat_codegen;
	cur->del = read_stat_del;

	cur->symbol = NULL;
	cur->next_read_stat = NULL;

	cur->symbol = symbol_parsing();
	assert(cur->symbol);

	assert(ctl->token == right_rnd || ctl->token == comma);
	if(ctl->token == comma){
		ctl = ctl->next;
		cur->next_read_stat = next_read_stat_parsing();
		assert(cur->next_read_stat);
	}else{
		ctl = ctl->next;
	}

	return cur;
}

struct AST_write_stat * write_stat_parsing(){
	AST_write_stat * cur = NULL;

	assert(ctl);
	if(ctl->token != write_iden)
		return (AST_stat *)cur;
	ctl = ctl->next;

	cur = (struct AST_write_stat *)malloc(sizeof(struct AST_write_stat));
	cur->type = write_stat_type;
	cur->codegen = write_stat_codegen;
	cur->del = write_stat_del;

	cur->symbol = NULL;
	cur->next_write_stat = NULL;

	assert(ctl->token == left_rnd);
	ctl = ctl->next;

	cur->symbol = symbol_parsing();
	assert(cur->symbol);

	assert(ctl->token == right_rnd || ctl->token == comma);
	if(ctl->token == comma){
		ctl = ctl->next;
		cur->next_write_stat = next_write_stat_parsing();
		assert(cur->next_write_stat);
	}else{
		ctl = ctl->next;
	}

	return (AST_stat *)cur;
}

struct AST_write_stat * next_write_stat_parsing(){
	AST_write_stat * cur =
		cur = (struct AST_write_stat *)malloc(sizeof(struct AST_write_stat));
	cur->type = write_stat_type;
	cur->codegen = write_stat_codegen;
	cur->del = write_stat_del;

	cur->symbol = NULL;
	cur->next_write_stat = NULL;

	cur->symbol = symbol_parsing();
	assert(cur->symbol);

	assert(ctl->token == right_rnd || ctl->token == comma);
	if(ctl->token == comma){
		ctl = ctl->next;
		cur->next_write_stat = next_write_stat_parsing();
		assert(cur->next_write_stat);
	}else{
		ctl = ctl->next;
	}

	return cur;
}


struct AST_complex_stat * complex_stat_parsing(){
	AST_complex_stat * cur = NULL;

	assert(ctl);
	if(ctl->token != begin_iden)
		return (AST_stat *)cur;
	ctl = ctl->next;

	cur = (struct AST_complex_stat *)malloc(sizeof(struct AST_complex_stat));
	cur->type = complex_stat_type;
	cur->codegen = complex_stat_codegen;
	cur->del = complex_stat_del;

	cur->stat= NULL;
	cur->next_stat = NULL;

	cur->stat = stat_parsing();
	assert(cur->stat);

	assert(ctl->token == semicolon || ctl->token == end_iden);
	if(ctl->token == semicolon){
		ctl = ctl->next;
		cur->next_stat = next_complex_stat_parsing();
		assert(cur->next_stat);
	}else{
		ctl = ctl->next;
	}

	return (AST_stat *)cur;
}

struct AST_complex_stat * next_complex_stat_parsing(){
	AST_complex_stat * cur =
		cur = (struct AST_complex_stat *)malloc(sizeof(struct AST_complex_stat));
	cur->type = complex_stat_type;
	cur->codegen = complex_stat_codegen;
	cur->del = complex_stat_del;

	cur->stat = NULL;
	cur->next_stat = NULL;

	cur->stat = stat_parsing();
	assert(cur->stat);

	assert(ctl->token == semicolon || ctl->token == end_iden);
	if(ctl->token == semicolon){
		ctl = ctl->next;
		cur->next_stat = next_complex_stat_parsing();
		assert(cur->next_stat);
	}else{
		ctl = ctl->next;
	}

	return cur;
}

struct AST_condition * condition_parsing(){
	AST_condition * cur =
		cur = (struct AST_condition *)malloc(sizeof(struct AST_condition));
	cur->type = condition_type;
	cur->codegen = condition_codegen;
	cur->del = condition_del;

	cur->left_expr = NULL;
	cur->right_expr = NULL;

	if(ctl->token == odd_iden){
		cur->condition_type = ctl->token;
		ctl = ctl->next;
		cur->left_expr = expr_parsing();
		assert(cur->left_expr);
	}else{
		cur->left_expr = expr_parsing();
		assert(cur->left_expr);

		assert(ctl->token == equal
			|| ctl->token == not_equal
			|| ctl->token == less
			|| ctl->token == more
			|| ctl->token == not_less
			|| ctl->token == not_more);

		cur->condition_type = ctl->token;
		ctl = ctl->next;

		cur->right_expr = right_parsing();
		assert(cur->right_expr);
	}

	return cur;
}

struct AST_expr * expr_parsing(){
	AST_expr * cur =
			(struct AST_expr *)malloc(sizeof(struct AST_expr));
	cur->type = expr_type;
	cur->codegen = expr_codegen;
	cur->del = expr_del;

	cur->term = NULL;
	cur->next = NULL;
	cur->op = add_token;

	if(ctl->token == add_token || ctl->token == sub_token){
		cur->op = ctl->token;
		ctl = ctl->next;
	}

	cur->term = term_parsing();
	assert(cur->term);

	cur->next = next_expr_parsing();
	assert(cur->next);

	return cur;
}

struct AST_expr * next_expr_parsing(){
	AST_expr * cur =
			(struct AST_expr *)malloc(sizeof(struct AST_expr));
	cur->type = expr_type;
	cur->codegen = expr_codegen;
	cur->del = expr_del;

	cur->term = NULL;
	cur->next = NULL;

	if(ctl->token == add_token || ctl->token == sub_token){
		cur->op = ctl->token;
		ctl = ctl->next;

		cur->term = term_parsing();
		assert(cur->term);

		cur->next = next_expr_parsing();
		assert(cur->next);

	}

	return cur;
}

struct AST_term * term_parsing(){
	AST_term * cur =
			(struct AST_term *)malloc(sizeof(struct AST_term));
	cur->type = term_type;
	cur->codegen = term_codegen;
	cur->del = term_del;

	cur->factor = NULL;
	cur->next = NULL;
	cur->op = none_op;

	cur->factor = factor_parsing();
	assert(cur->factor);

	cur->next = next_term_parsing();
	assert(cur->next);

	return cur;
}

struct AST_term * next_term_parsing(){
	AST_term * cur =
			(struct AST_term *)malloc(sizeof(struct AST_term));
	cur->type = term_type;
	cur->codegen = term_codegen;
	cur->del = term_del;

	cur->factor = NULL;
	cur->next = NULL;

	if(ctl->token == mul_token || ctl->token == div_token){
		cur->op = ctl->token;
		ctl = ctl->next;

		cur->factor = factor_parsing();
		assert(cur->factor);

		cur->next = next_term_parsing();
		assert(cur->next);

	}

	return cur;
}

struct AST_factor * factor_parsing(){
	AST_factor * cur =
			(struct AST_factor *)malloc(sizeof(struct AST_factor));
	cur->type = factor_type;
	cur->codegen = factor_codegen;
	cur->del = factor_del;

	cur->factor = NULL;

	assert(ctl->token==iden||ctl->token==number_token||ctl->token==left_rnd);
	if(ctl->token == iden){
		cur->factor = (AST *)symbol_parsing();
		cur->fact_type = symbol_factor_type;
	}else if(ctl->token == number_token){
		cur->factor = (AST *)number_parsing();
		cur->fact_type = number_factor_type;
	}else if(ctl->token == left_rnd){
		ctl = ctl->next;
		cur->factor = (AST *)expr_parsing();
		cur->fact_type = expr_factor_type;
		assert(ctl->token == right_rnd);
		ctl = ctl->next;
	}

	return cur;
}
