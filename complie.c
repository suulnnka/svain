#include "complie.h"
#include "basicio.h"
#include "string_list.h"
#include "lex.h"
#include "parsing.h"

int complie(char *src_name,char *obj_name){

	struct string_list *str = read_src(src_name);
	struct token_list *list = lex(str);
	struct AST *root = parsing(list);
	struct string_list *code = codegen(root);
	struct string_list *obj = as(code);
	write_obj(obj_name,obj);

	str->del(str);
	token_list_del(list);
	root->del(root);
	code->del(code);
	obj->del(obj);

	return 0;
}
