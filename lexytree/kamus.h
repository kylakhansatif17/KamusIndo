#ifndef KAMUS_H
#define KAMUS_H
#include "trie.h"
/* Fungsi untuk memuat kamus ke dalam Trie */

/* Membaca file kamus dan menyisipkan setiap huruf ke dalam Trie  */
void loadKamus(TrieNode *root, const char *filename);

#endif 