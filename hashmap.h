#ifndef _hashmap
#define _hashmap


#include<stdbool.h>
#define DEFAULT_SIZE 10000
#define NUMBER_OF_FILES 26
#define MAX_SIZE 256


struct hashMapElement{
  char *key;
  int value[NUMBER_OF_FILES];
  struct hashMapElement *next;
}hashMapElement;

struct hashmap{
  struct hashMapElement **Array;
  int *array_position;
  int size;
  int capacity;
	int (*addElement)(char *, int,struct hashmap*,int);  
	int (*getElement)(char*,struct hashmap*,int);
	void (*updateElement)(char *,int ,struct hashmap*,int);
	bool (*containsElement)(char *, struct hashmap* ,int);
	void (*clean_up)();
	void (*init_hashmap)(struct hashmap *);
}hashmap;


int addElement(char *, int ,struct hashmap*,int);  
int getElement(char*,struct hashmap*,int);
void updateElement(char *,int ,struct hashmap* ,int);
bool containsElement(char *,struct hashmap*, int );
void clean_up();
void init_hashmap(struct hashmap *);
#endif
