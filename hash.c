#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct node{
    char* key;
    char* val;
    struct node *next;
};
struct table{
    int size;
    struct node **list;
};
/*crearea hashmapului*/
struct table *createTable(int size){
int i;
    struct table *t = (struct table*)malloc(sizeof(struct table));
    t->size = size;
    t->list = (struct node**)malloc(sizeof(struct node*)*size);
    for(i=0;i<size;i++)
        t->list[i] = NULL;
    return t;
}
/*functie de hashcode*/
int hashCode( char *str)
{
    int hash = 12;
    int c;

    while (c = *str++)
        hash = (((hash << 5) + hash) + c) % 1000; /* hash * 33 + c */
		

    return hash;
} 
/*inserare element*/
void insert(struct table *t,char* key,char* val){
    int pos = hashCode(key);
    struct node *list = t->list[pos];
    struct node *newNode = (struct node*)malloc(sizeof(struct node));
    struct node *temp = list;
    while(temp){
        if(*temp->key==*key){
            temp->val = val;
            return;
        }
        temp = temp->next;
    }
    newNode->key = key;
    newNode->val = val;
    newNode->next = list;
    t->list[pos] = newNode;
}
/*cautare element*/
char* lookup(struct table *t,char* key){
    int pos = hashCode(key);
    struct node *list = t->list[pos];
    struct node *temp = list;
    while(temp){
        if(*temp->key==*key){
            return temp->val;
        }
        temp = temp->next;
    }
    return "null";
}