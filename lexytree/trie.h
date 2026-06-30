#ifndef TRIE_H
#define TRIE_H

#define ALFABET   26
#define MAX_KATA   50
#define MAX_SARAN  9 /* maksimum jumlah suggestion ditampilkan */

/* Struktur SinonimNode (Link Listed) */
typedef struct SinonimNode {
    char kata[MAX_KATA];
    struct SinonimNode *next;
} SinonimNode;

/* Struktur TrieNode (Non Binary Tree) */
typedef struct TrieNode {
    struct TrieNode *children[ALFABET]; 
    int isEndOfWord; /* 1 jika merupakan akhir dari sebuah kata */
    SinonimNode *sinonim; /* pointer ke daftar sinonim */
} TrieNode;

/* Fungsi-fungsi untuk operasi pada Trie */

/* Membuat node Trie baru */
TrieNode *newTrieNode(void);

/* Menyisipkan kata ke dalam Trie per huruf */
void insertTrie(TrieNode *root, const char *kata);

TrieNode *searchTrie(TrieNode *root, const char *prefix);

void getSuggestion(TrieNode *root, const char *prefix,char hasil[][MAX_KATA], int *count);

TrieNode *findNode(TrieNode *root, const char *kata);

void freeTrie(TrieNode *root);

#endif