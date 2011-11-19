#include "string_list.h"
#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct string_list * new_string_list(){

	struct string_list *list = (struct string_list *)malloc(sizeof(struct string_list));
	assert(list!=NULL);
	list->root = (string_node *)malloc(sizeof(struct string_node));
	assert(list->root != NULL);

	memset(list->root->buf,0,BUF_SIZE);
	list->root->size = 0;
	list->root->next = NULL;

	list->cur = list->root;
	list->pos = 0;
	list->is_over = 0;

	list->reset = string_list_reset;
	list->del = string_list_del;
	list->next_char = string_list_next_char;
	list->write_char = string_list_write_char;
	list->write_str = string_list_write_str;
	list->write_cpy = string_list_write_cpy;
	list->read_cpy = string_list_read_cpy;

	return list;
}
int string_list_reset(struct string_list *list){
	list->cur = list->root;
	list->pos = 0;
	list->is_over = 0;

	return 0;
}
char string_list_next_char(struct string_list *list){
	assert(list->root->size > 0); // string_list is empty
	if( list->is_over != 0 )
		error("can't read more byte in the string_list.");
	if( list->pos < list->cur->size ){
		char ch = list->cur->buf[list->pos];
		list->pos ++;
		if(list->pos == list->cur->size && list->cur->next == NULL)
			list->is_over = 1;
		return ch;
	}

	assert(list->pos == list->cur->size); // 谁修改过pos的值?
	assert(list->cur->next != NULL); //为什么is_over不管用?

	list->pos = 1;
	list->cur = list->cur->next;

	assert(list->cur->size > 0); //为什么list->cur->buf里没有东西

	return list->cur->buf[0];
}
int string_list_write_str(struct string_list *list,char *s){
	int i;
	assert( strlen(s) != 0 ); // 为什么会传进来空串
	for(i = 0 ;i<strlen(s);i++){
		char ch = s[i];
		assert(ch != '\0');
		list->write_char(list,ch);
	}
	return 0;
}
int string_list_write_char(struct string_list *list,char ch){
	list->is_over = 1;
	if(list->cur->size < BUF_SIZE){
		list->cur->buf[list->cur->size] = ch;
		list->cur->size ++ ;
		list->pos ++;
		return 0;
	}
	assert(list->cur->size == BUF_SIZE);

	//if full.new a string_node
	list->cur->next = (string_node *)malloc(sizeof(struct string_node));
	assert(list->cur->next != NULL);

	list->cur = list->cur->next;
	memset(list->cur,0,BUF_SIZE);
	list->cur->buf[0] = ch;
	list->cur->size = 1;
	list->pos = 1;

	return 0;
}
int string_list_write_cpy(struct string_list *list,char *buf,int size){
	list->is_over = 1;
	if(list->cur == list->root){
		assert(list->cur->size == 0);
	}else{
		assert(list->cur->size == BUF_SIZE);
		assert(list->cur->next == NULL);

		list->cur->next = (string_node *)malloc(sizeof(struct string_node));
		assert(list->cur->next != NULL);
		list->cur = list->cur->next;
		list->cur->size = 0;
	}
	memcpy(list->cur->buf,buf,size);
	list->cur->size = size;
	list->pos = size;
	return 0;
}

int string_list_read_cpy(struct string_list *list,char *buf){
	assert(list->root->size > 0);
	assert(list->cur->size > 0);
	if(list->is_over)
		return 0;

	//保证list->pos为0,并且list->cur->size不为0
	assert(list->pos == 0 || list->pos == list->cur->size);

	if(list->pos == list->cur->size){
		assert(list->pos == BUF_SIZE);
		assert(list->cur->next != NULL);
		list->cur = list->cur->next;
		list->pos = 0;
	}

	assert(list->cur->size > 0);

	//start copy
	memcpy(buf,list->cur->buf,list->cur->size);
	list->pos = list->cur->size;

	//对is_over赋值
	if(list->pos != BUF_SIZE){
		list->is_over = 1;
		assert(list->cur->next == NULL);
	}
	if(list->pos == BUF_SIZE && list->cur->next == NULL)
		list->is_over = 1;

	return list->pos;
}

int string_list_del(struct string_list *list){
	string_node_del(list->root);
	free(list);
	return 0;
}
int string_node_del(string_node *node){
	if(node->next != NULL)
		string_node_del(node->next);
	free(node);
	return 0;
}
