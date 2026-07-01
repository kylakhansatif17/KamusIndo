#ifndef TRIE_H
#define TRIE_H

#define ALFABET     26
#define MAX_KATA    50
#define MAX_SARAN   9

typedef struct SinonimNode {
    char kata[MAX_KATA];
    struct SinonimNode *next;
} SinonimNode;

typedef struct TrieNode {
    struct TrieNode *children[ALFABET];
    int isEndOfWord;
    char bentukAsli[MAX_KATA]; /* bentuk asli untuk tampilan */
    SinonimNode *sinonim;
} TrieNode;

/* Prototipe */
TrieNode *newTrieNode(void);

/* insertTrie sekarang juga terima bentukAsli untuk disimpan */
void insertTrie(TrieNode *root, const char *kataNorm, const char *bentukAsli);

TrieNode *searchTrie(TrieNode *root, const char *prefix);

/* getSuggestion mengisi hasil dengan bentukAsli */
void getSuggestion(TrieNode *root, const char *prefixNorm,
                   char hasil[][MAX_KATA], int *count);

TrieNode *findNode(TrieNode *root, const char *kataNorm);

void freeTrie(TrieNode *root);

#endif