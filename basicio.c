#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "basicio.h"
#include "base.h"
#include "string_list.h"

struct string_list * read_src(char *src_name){
	FILE *fp = fopen(src_name,"rb");
	if(fp == NULL){
		printf("src_file_name : %s\n",src_name);
		error("no such file.");
	}

	struct string_list * list = new_string_list();

	while(!feof(fp)){
		char buf[BUF_SIZE];
		int size = fread(buf,1,BUF_SIZE,fp);
		list->write_cpy(list,buf,size);
	}

	list->reset(list);
	fclose(fp);

	//TODO 转unicode
	return list;
}
int write_obj(char *obj_name,struct string_list *list){
	FILE *fp = fopen(obj_name,"wb");
	assert(fp);
	char buf[BUF_SIZE];
	list->reset(list);
	while(list->is_over == 0){
		int size = list->read_cpy(list,buf);
		fwrite(buf,1,size,fp);
	}
	fclose(fp);

	return 0;
}
