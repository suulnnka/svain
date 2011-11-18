#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "complie.h"
#include "base.h"
#include "run.h"
#include "assert.h"

int main(int argc, char *argv[]) {

	if( argc < 2 ){
		#ifdef TEST
		complie("test.pl","test.s");
		run("test.s");
		#else
		error("few arg .");
		#endif
	}else{
		int i,j;
		for(i = 1; i<= argc;i++){
			char *obj_name = (char *)malloc(sizeof(argv[i]));
			strcpy(obj_name,argv[i]);
			for(j = strlen(obj_name)-1;j>=0;j--){
				if( j < 3 )
					error("file name error.");
				if(obj_name[j]!='\0'){
					obj_name[j] = '\0';
					obj_name[j-1] = 's';
					obj_name[j-2] = '.';
					assert(obj_name[j-3] != '.');
					break;
				}
			}

			complie(argv[i],obj_name);

			run(obj_name);

			free(obj_name);
		}
	}

	return 0;
}
