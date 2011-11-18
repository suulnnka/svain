#include "string_list.h"
#include "lex.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int token_list_del(token_list *list){
	if(list->next != NULL)
		token_list_del(list->next);
	if(list->str != NULL)
		free(list->str);
	free(list);
	return 0;
}

char * new_string(char *old_string,char ch){
	char *str = (char *)malloc(strlen(old_string)+2);
	memset(str,0,sizeof(str));
	memcpy(str,old_string,strlen(old_string));
	str[strlen(old_string)] = ch;
	free(old_string);
	return str;
}

token_list * new_token_node(){
	token_list * node;
	node = (token_list *)malloc(sizeof(token_list));
	assert(node);

	node->token = none_token;
	node->next = NULL;
	node->prev = NULL;
	node->str = NULL;
	node->number = 0;

	return node;
}

token_list * lex(string_list *str){
	token_list *root = new_token_node();
	root->token = root_token;
	token_list *current;

	current = new_token_node();
	root->next = current;
	current->prev = root;
	current->token = none_token;

	char ch = str->next_char(str);
	assert(ch != EOF);

	while(current->token != eof_token){
		lex_state state = get_lex_state(ch);
		switch(state){
		case number_s:
			current = number_lex(str,current,&ch);
			break;
		case string_s:
			current = string_lex(str,current,&ch);
			break;
		case space_s:
			ch = str->next_char(str);
			break;
		case symbol_s:
			current = symbol_lex(str,current,&ch);
			break;
		case none_s:
			current->token = eof_token;
			break;
		}
	}

}

token_list * number_lex(string_list *str,token_list * node,char *c){
	char ch = (*c);
	node->token = number;
	node->number = 0;

	do{
		node->number = node->number * 10 + ch - '0';
		ch = str->next_char(str);
	}while(get_lex_state(ch) == number_s);

	token_list * next = new_token_node();
	next->token = none_token;

	node->next = next;
	next->prev = node;

	(*c) = ch;

	return next;
}

token_list * string_lex(string_list *str,token_list * node,char *c){
	char ch = (*c);

	node->token = iden;
	node->str = (char *)malloc(1);
	assert(node->str);

	do{
		node->str = new_string(node->str,ch);
		ch = str->next_char(str);
	}while(isalnum(ch));

	if(!strcmp("IF",node->str))
		node->token = if_iden;
	if(!strcmp("THEN",node->str))
		node->token = then_iden;
	if(!strcmp("WHILE",node->str))
		node->token = while_iden;
	if(!strcmp("DO",node->str))
		node->token = do_iden;
	if(!strcmp("READ",node->str))
		node->token = read_iden;
	if(!strcmp("WRITE",node->str))
		node->token = write_iden;
	if(!strcmp("CALL",node->str))
		node->token = call_iden;
	if(!strcmp("CONST",node->str))
		node->token = const_iden;
	if(!strcmp("VAR",node->str))
		node->token = var_iden;
	if(!strcmp("PROCEDURE",node->str))
		node->token = procedure_iden;
	if(!strcmp("BEGIN",node->str))
		node->token = begin_iden;
	if(!strcmp("END",node->str))
		node->token = end_iden;
	if(!strcmp("ODD",node->str))
		node->token = odd_iden;

	token_list * next = new_token_node();
	next->token = none_token;

	node->next = next;
	next->prev = node;

	(*c) = ch;

	return next;

}

token_list * symbol_lex(string_list *str,token_list * node,char *c){
	char ch = (*c);
	char nch = str->next_char(str);

	node->token = symbol_token;

	switch(ch){
	case ':':
		if(nch != '=')
			error("must be := .");
		nch = str->next_char(str);
		node->token = val;
		break;
	case '.':
		node->token = period;
		break;
	case ',':
		node->token = comma;
		break;
	case ';':
		node->token = semicolon;
		break;
	case '+':
		node->token = add;
		break;
	case '-':
		node->token = sub;
		break;
	case '*':
		node->token = mul;
		break;
	case '/':
		node->token = div;
		break;
	case '=':
		node->token = equal;
		break;
	case '#':
		node->token = not_equal;
		break;
	case '(':
		node->token = left_rnd;
		break;
	case ')':
		node->token = right_rnd;
		break;
	case '<':
		if(nch == '='){
			node->token = not_more;
			nch = str->next_char(str);
		}else{
			node->token = less;
		}
		break;
	case '>':
		if(nch == '='){
			node->token = not_less;
			nch = str->next_char(str);
		}else{
			node->token = more;
		}
		break;
	}

	assert(node->token != symbol_token);

	token_list * next = new_token_node();
	next->token = none_token;

	node->next = next;
	next->prev = node;

	(*c) = nch;

	return next;
}

lex_state get_lex_state(char ch){
	if(isdigit(ch))
		return number_s;
	if(isalpha(ch))
		return string_s;
	if(ch == EOF)
		return none_s;
	if(isspace(ch))
		return space_s;
	assert(!iscntrl(ch));
	if(ispunct(ch))
		return symbol_s;
	printf("ascii : %d\n",ch);
	error("wtf ?");
}

