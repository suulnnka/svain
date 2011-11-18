#include "run.h"
#include "base.h"
#include "assert.h"

int run(char *obj_name){
	FILE *obj = fopen(obj_name,"r");
	if(obj == NULL){
		printf("obj_file_name : %s\n",obj_name);
		error("no such file .");
	}
	fclose(obj);
	error("TODO run .");
	return 0;
}
