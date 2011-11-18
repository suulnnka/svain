#include "lex.h"
#include "yacc.h"
#include "string_list.h"
#include "base.h"
#include <assert.h>

AST * yacc(token_list * list){
	AST * root = (AST *)malloc(sizeof(AST));

	assert(list->next && list->token == root_token);
	list = list->next;

	list = block_yacc(root,list);
	assert(list->token == period);

	return root;
}

token_list * block_yacc(AST * current,token_list * list){

}
