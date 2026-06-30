#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

/* Implementasi fungsi-fungsi Trie */

/* Membuat node Trie baru */
TrieNode *newTrieNode(void) {
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (node == NULL) {
        fprintf(stderr, "Error: malloc gagal di newTrieNode\n");
        exit(1);
    }
    int i;
    for (i = 0; i < ALFABET; i++)
        node->children[i] = NULL;
    node->isEndOfWord = 0;
    node->sinonim     = NULL;
    return node;
}


/* Menyisipkan kata ke dalam Trie per huruf */
void insertTrie(TrieNode *root, const char *kata) {
    if (root == NULL || kata == NULL || kata[0] == '\0') return;

    TrieNode *current = root;
    int i;
    for (i = 0; kata[i] != '\0'; i++) {
        int idx = kata[i] - 'a';
        if (idx < 0 || idx >= ALFABET) continue; /* lewati non-huruf */
        if (current->children[idx] == NULL)
            current->children[idx] = newTrieNode();
        current = current->children[idx];
    }
    current->isEndOfWord = 1;
}

TrieNode *searchTrie(TrieNode *root, const char *prefix) {
    if (root == NULL || prefix == NULL) return NULL;

    TrieNode *current = root;
    int i;
    for (i = 0; prefix[i] != '\0'; i++) {
        int idx = prefix[i] - 'a';
        if (idx < 0 || idx >= ALFABET) return NULL;
        if (current->children[idx] == NULL) return NULL;
        current = current->children[idx];
    }
    return current;
}

static void dfsCollect(TrieNode *node, char *currentWord, int depth,
                       char hasil[][MAX_KATA], int *count) {
    if (*count >= MAX_SARAN) return;

    if (node->isEndOfWord) {
        currentWord[depth] = '\0';
        strncpy(hasil[*count], currentWord, MAX_KATA - 1);
        hasil[*count][MAX_KATA - 1] = '\0';
        (*count)++;
    }

    int i;
    for (i = 0; i < ALFABET && *count < MAX_SARAN; i++) {
        if (node->children[i] != NULL) {
            currentWord[depth] = (char)('a' + i);
            dfsCollect(node->children[i], currentWord, depth + 1,
                       hasil, count);
        }
    }
}

void getSuggestion(TrieNode *root, const char *prefix,char hasil[][MAX_KATA], int *count) {
    *count = 0;
    TrieNode *node = searchTrie(root, prefix);
    if (node == NULL) return;

    char currentWord[MAX_KATA];
    strncpy(currentWord, prefix, MAX_KATA - 1);
    currentWord[MAX_KATA - 1] = '\0';

    int depth = (int)strlen(prefix);
    dfsCollect(node, currentWord, depth, hasil, count);
}

TrieNode *findNode(TrieNode *root, const char *kata) {
    if (root == NULL || kata == NULL || kata[0] == '\0') return NULL;

    TrieNode *current = root;
    int i;
    for (i = 0; kata[i] != '\0'; i++) {
        int idx = kata[i] - 'a';
        if (idx < 0 || idx >= ALFABET) return NULL;
        if (current->children[idx] == NULL) return NULL;
        current = current->children[idx];
    }
    return (current->isEndOfWord) ? current : NULL;
}

void freeTrie(TrieNode *root) {
    if (root == NULL) return;

    int i;
    for (i = 0; i < ALFABET; i++)
        freeTrie(root->children[i]);

    /* bebaskan linked list sinonim */
    SinonimNode *curr = root->sinonim;
    while (curr != NULL) {
        SinonimNode *tmp = curr->next;
        free(curr);
        curr = tmp;
    }
    free(root);
}


