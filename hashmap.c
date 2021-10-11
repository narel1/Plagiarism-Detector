#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<string.h>
//elementArray -> Array
//number_of_elements -> size

unsigned long hashing(unsigned char *str)
    {
        unsigned long hash = 5381;
        int c;

        while (c = *str++)
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash;
    }



int addElement(char *key, int value,struct hashmap* hashmap,int file_no){
  unsigned int hash = (hashing(key)% hashmap->capacity);
            // printf("%s\n","DEBUG_STATMENT 1" );
    struct hashMapElement *e = (struct hashMapElement*)malloc(500*sizeof(struct hashMapElement));

            // printf("%s\n","c DEBUG_STATMENT 2" );
    for(int i=0; i<NUMBER_OF_FILES; i++){
      e->value[i] = 0;
    }
    e->value[file_no]= value;
    e->key = key;
    hashmap->size++;
  if(hashmap->array_position[hash] == 0){
    hashmap->array_position[hash]=1;
    hashmap->Array[hash] = e;
  }else{
     // printf("%s\n", "Collision");
    struct hashMapElement *e1 = hashmap->Array[hash];
    while(e1->next!=NULL){
      e1 = e1->next;
    }
    e1->next = e;
    // free(e1);
  }
// free(e);
  return hash;
}
  
int getElement(char* key, struct hashmap* hashmap,int file_no){
  unsigned int hash = (hashing(key)% hashmap->capacity);
  struct hashMapElement *e = hashmap->Array[hash];
  while(e!=NULL){
    if(e->key == NULL)
        break;

    int a = strcmp(e->key,key);
    if(!a){
      // printf("%d %d\n",file_no , e->value[file_no] );
      return e->value[file_no];
    }else{
      e = e->next;
    }
  }
  // free(e);
  return -1; // not there

}


void updateElement(char *key,int updateValue,struct hashmap* hashmap,int file_no){
  unsigned int hash = (hashing(key)% hashmap->capacity);
  // printf("%d\n", hash);
  struct hashMapElement *e = hashmap->Array[hash];
  // printf("%s\n",e->key );
  while(e!=NULL){
  // printf("%s\n",e->key );
    if(e->key == NULL)
        break;
    int a = strcmp(e->key,key);
    // printf("%d\n", a );
    if(!a){
      // printf("%d\n", updateValue);
      e->value[file_no] = updateValue;
      e = e->next;
      // break;
    }else{
      e = e->next;
    }
  }
  // free(e);
}


bool containsElement(char *key, struct hashmap* hashmap, int file_no){
  // printf("%s\n", key);
  unsigned int hash = (hashing(key)% hashmap->capacity);
  // if(hash == 6){
  //   printf("%s %d\n",key,hash);
  // }
  
  struct hashMapElement *e = hashmap->Array[hash];
    // printf("%s %s\n",e->key , key );
  while(e!=NULL){
    // printf("%s\n", "1" );
    // printf("%s %s\n",e->key , key );
    if(e->key == NULL){
      break;
    }
    int a = strcmp(e->key,key);
    // printf("%d\n", a );
    if(!a){
      // free(e);
      return true;
    }else{
      e = e->next;
    }
  }
  // free(e);
  return false;
}

void clean_up(struct hashmap* hashmap){
  free(hashmap->array_position);
  for(int i=0; i<DEFAULT_SIZE; i++){
    struct hashMapElement *element = hashmap->Array[i];
    struct hashMapElement *tmp;
    while(element!=NULL){
      tmp = element;
      element = element->next;
      free(tmp);
    }
  }
  free(hashmap->Array);
  free(hashmap);

}

void init_hashmap(struct hashmap *m){
  // printf("Done Something 1\n");

  m->array_position = (int*)malloc(sizeof(int)*DEFAULT_SIZE);
  for(int i=0; i<DEFAULT_SIZE; i++){
    m->array_position[i] = 0;
  }
  // printf("Done Something 2\n");
  m->Array = (struct hashMapElement**)calloc(DEFAULT_SIZE*sizeof(struct hashMapElement),sizeof(struct hashMapElement*));
  for( int i =0; i<=DEFAULT_SIZE; i++){
    m->Array[i] = (struct hashMapElement*) malloc(sizeof(struct hashMapElement));
    m->Array[i] = NULL;
  }
  // printf("Done Something 3\n");
  m->addElement = addElement;
  m->getElement = getElement;
  m->updateElement = updateElement;
  m->containsElement = containsElement;
  m->clean_up = clean_up;
  m->init_hashmap = init_hashmap;
  m->capacity = DEFAULT_SIZE;
  m->size = 0;
}


