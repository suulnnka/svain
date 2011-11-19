#ifndef STRING_LIST_H
#define STRING_LIST_H

#define BUF_SIZE 512

struct string_list{
	string_node *root;
	string_node *cur;
	int pos;
	int is_over;
	int (*reset)(struct string_list *);
	char (*next_char)(struct string_list *);
	int (*write_str)(struct string_list * ,char *);
	int (*write_char)(struct string_list *,char);
	int (*write_cpy)(struct string_list *,char *,int);
	int (*read_cpy)(struct string_list *,char *);
	int (*del)(struct string_list *);
};
struct string_node{
	char buf[BUF_SIZE];
	int size;
	string_node * next;
};

struct string_list * new_string_list();
int string_list_reset(struct string_list *);
char string_list_next_char(struct string_list *);
int string_list_write_str(struct string_list *,char *);
int string_list_write_char(struct string_list *,char);
int string_list_write_cpy(struct string_list *,char *,int);
int string_list_read_cpy(struct string_list *,char *);
int string_list_del(struct string_list *);


#endif // STRING_LIST_H
