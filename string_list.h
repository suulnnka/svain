#ifndef STRING_LIST_H
#define STRING_LIST_H

#define BUF_SIZE 512

struct string_list{
	string_node *root;
	string_node *cur;
	int pos;
	int is_over;
	int (*reset)(string_list *);
	char (*next_char)(string_list *);
	int (*write_str)(string_list * ,char *);
	int (*write_char)(string_list *,char);
	int (*write_cpy)(string_list *,char *,int);
	int (*read_cpy)(string_list *,char *);
	int (*del)(string_list *);
};
struct string_node{
	char buf[BUF_SIZE];
	int size;
	string_node * next;
};

string_list * new_string_list();
int string_list_reset(string_list *);
char string_list_next_char(string_list *);
int string_list_write_str(string_list *,char *);
int string_list_write_char(string_list *,char);
int string_list_write_cpy(string_list *,char *,int);
int string_list_read_cpy(string_list *,char *);
int string_list_del(string_list *);


#endif // STRING_LIST_H
