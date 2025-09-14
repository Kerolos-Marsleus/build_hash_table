#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

static ht_item* ht_new_item(const char* k, const char* v){
    ht_item* i = malloc(sizeof(ht_item));
    if(!i) return NULL;     // error checking for malloc
    i->key = strdup(k);
    i->value = strdup(v);

    if(!i->key || !i->value){   // error checking for malloc inside strdup
        free(i->key);
        free(i->value);
        free(i);
        return NULL;
    }

    return i;
}

ht_hash_table* ht_new(){
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));

    ht->size = 53;
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}

static void ht_del_item(ht_item* i){
    free(i->key);
    free(i->value);
    free(i);
}

void ht_del_hash_table(ht_hash_table* ht){
    for(int i = 0; i < ht->size; i++){
        ht_item* item = ht->items[i];
        if(item != NULL){
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}

static int ht_hash(const char* str, const int prime_number, const int ht_size){
    long hash = 0;
    const int len_str = strlen(str);
    for(int i = 0; i < len_str; i++){
        hash += (long)pow(prime_number, len_str - (i+1)) * str[i];
        hash = hash % ht_size;
    }
    return (int)hash;
}