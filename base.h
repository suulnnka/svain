#ifndef BASE_H
#define BASE_H

#define TEST

#define len(x) ((sizeof(x))/(sizeof(x[0])))
#define error(x) ({						\
	printf("func : %s\n",__FUNCTION__);	\
	printf("file : %s\n",__FILE__);		\
	printf("line : %d\n",__LINE__);		\
	printf("error : %s\n",(x));			\
	exit(-2);							\
	})

#endif // BASE_H
