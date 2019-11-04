#ifndef __HASH_H_
#define __HASH_H_

typedef void *hash_table;

hash_table criaHashTable(int n, char *(*getId)(void *));

void insereHashTable(hash_table t, void *item);

void *getHashTable(hash_table t, char *chave);

int hashFunction(char *chave, int size);

void remove_hash(hash_table t, char *chave);

#endif