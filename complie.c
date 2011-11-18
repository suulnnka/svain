#include "complie.h"
#include "basicio.h"
#include "string_list.h"
#include "lex.h"
#include "yacc.h"

int complie(char *src_name,char *obj_name){

	string_list *str = read_src(src_name);
	token_list *list = lex(str);
	AST *root = yacc(list);
	string_list *code = codegen(root);
	string_list *obj = as(code);
	write_obj(obj_name,obj);

	str->del(str);
	token_list_del(list);
	root->del(root);
	code->del(code);
	obj->del(obj);

	return 0;
}
