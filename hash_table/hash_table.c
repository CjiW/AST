#include "hash_table.h"
unsigned int BKDRHash(char str[128]){
    char *p = str;
    unsigned int seed = 131;
    unsigned hash = 0;
    while (*p){
        hash = hash * seed + (*p++);
    }
    return (hash & 0x7fffffff);
}
// while (arr[hashIndex] != nullptr && arr[hashIndex]->key != key && arr[hashIndex]->key != -1) {
// 			// 如果容量已满会造成死循环
// 			hashIndex++;
// 			hashIndex %= capacity;
// 		}
int Push(char name[128]){
    unsigned int hash_code = BKDRHash(name);
    unsigned int idx = hash_code % HASH_SIZE;
    unsigned int p = idx;
    while (idents_[idx]!=NULL && idents_[idx]->key != hash_code && idents_[idx]->key != -1) {
        idx = (idx+1) % HASH_SIZE;
        if(p == idx) {
            return PUSH_ERR_FULL;
        }
    }
    if(idents_[idx] == NULL) {
        Ident *new_ident = (Ident*)malloc(sizeof(Ident));
        new_ident->key = hash_code;
        strcpy(new_ident->name, name);
        idents_[idx] = new_ident;
        return PUSH_OK;
    }
    if(idents_[idx]->key == -1) {
        idents_[idx]->key = hash_code;
        strcpy(idents_[idx]->name, name);
        return PUSH_OK;
    }
    return PUSH_ERR_EXSIT;
}
int Check(char name[128]);
int Remove(char name[128]) {
    unsigned int hash_code = BKDRHash(name);
    unsigned int idx = hash_code % HASH_SIZE;

}