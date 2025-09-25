#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
static ht_item HT_DELETED_ITEM = {NULL, NULL};

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

static int ht_get_hash(const char* str, const int num_buckets, const int attempt){
    const int hash_a = ht_hash(str, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(str, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}


void ht_insert(ht_hash_table* ht,const char* key,const char* value){
    if(ht->count >= ht->size){
        // there is no memory
        // resize.
    }
    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item* current_item = ht->items[index];
    int i = 1;
    while(current_item != NULL ){
        if(current_item != &HT_DELETED_ITEM){
            if(strcmp(current_item->key, key) == 0){
                ht_del_item(current_item);
                ht->items[index] = item;
                return;
            }
        }
        index = ht_get_hash(item->key, ht->size, i);
        current_item = ht->items[index];
        i++;
    }
    ht->items[index] = item;
    ht->count++;
}

char* ht_search(ht_hash_table* ht, const char* key){
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while(item != NULL){
        if(item != & HT_DELETED_ITEM){
            if(strcmp(key, item->key) == 0){
                return item->value;
            }
        }
        index = ht_get_hash(key, ht->size, i);
        i++;
        item = ht->items[index];
    }
    return NULL;
}




void ht_delete(ht_hash_table* ht, const char* key){
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while(item != NULL){
        if(item != &HT_DELETED_ITEM){
            if(strcmp(key, item->key)==0){
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        index  = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    ht->count--;
}


static ht_hash_table* ht_new_sized(const int base_size){
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    if(!ht){
        return NULL;
    }
}