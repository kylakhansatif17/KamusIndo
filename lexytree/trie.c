#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

TrieNode *newTrieNode(void) {
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (node == NULL) { fprintf(stderr, "Error: malloc gagal\n"); exit(1); }
    int i;
    for (i = 0; i < ALFABET; i++) node->children[i] = NULL;
    node->isEndOfWord   = 0;
    node->bentukAsli[0] = '\0';
    node->sinonim       = NULL;
    return node;
}

void insertTrie(TrieNode *root, const char *kataNorm, const char *bentukAsli) {
    if (root == NULL || kataNorm == NULL || kataNorm[0] == '\0') return;

    TrieNode *current = root;
    int i;
    for (i = 0; kataNorm[i] != '\0'; i++) {
        int idx = kataNorm[i] - 'a';
        if (idx < 0 || idx >= ALFABET) continue;
        if (current->children[idx] == NULL)
            current->children[idx] = newTrieNode();
        current = current->children[idx];
    }
    current->isEndOfWord = 1;
    /* simpan bentuk asli untuk ditampilkan ke user */
    strncpy(current->bentukAsli, bentukAsli, MAX_KATA - 1);
    current->bentukAsli[MAX_KATA - 1] = '\0';
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

/* DFS helper */
static void dfsCollect(TrieNode *node, char *currentWord, int depth,
                       char hasil[][MAX_KATA], int *count) {
    if (*count >= MAX_SARAN) return;

    if (node->isEndOfWord) {
        /* tampilkan bentukAsli jika ada, fallback ke currentWord */
        if (node->bentukAsli[0] != '\0')
            strncpy(hasil[*count], node->bentukAsli, MAX_KATA - 1);
        else {
            currentWord[depth] = '\0';
            strncpy(hasil[*count], currentWord, MAX_KATA - 1);
        }
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

void getSuggestion(TrieNode *root, const char *prefixNorm,
                   char hasil[][MAX_KATA], int *count) {
    *count = 0;
    TrieNode *node = searchTrie(root, prefixNorm);
    if (node == NULL) return;

    char currentWord[MAX_KATA];
    strncpy(currentWord, prefixNorm, MAX_KATA - 1);
    currentWord[MAX_KATA - 1] = '\0';

    int depth = (int)strlen(prefixNorm);
    dfsCollect(node, currentWord, depth, hasil, count);
}

TrieNode *findNode(TrieNode *root, const char *kataNorm) {
    if (root == NULL || kataNorm == NULL || kataNorm[0] == '\0') return NULL;
    TrieNode *current = root;
    int i;
    for (i = 0; kataNorm[i] != '\0'; i++) {
        int idx = kataNorm[i] - 'a';
        if (idx < 0 || idx >= ALFABET) return NULL;
        if (current->children[idx] == NULL) return NULL;
        current = current->children[idx];
    }
    return (current->isEndOfWord) ? current : NULL;
}

void freeTrie(TrieNode *root) {
    if (root == NULL) return;
    int i;
    for (i = 0; i < ALFABET; i++) freeTrie(root->children[i]);
    SinonimNode *curr = root->sinonim;
    while (curr != NULL) {
        SinonimNode *tmp = curr->next;
        free(curr);
        curr = tmp;
    }
    free(root);
}