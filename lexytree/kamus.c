#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "kamus.h"

/* Implementasi fungsi kamus */

/* Membaca file kamus dan menyisipkan setiap huruf ke dalam Trie  */
void loadKamus(TrieNode *root, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Gagal memuat kamus.txt\n");
        fprintf(stderr, "Pastikan file '%s' ada di folder yang sama.\n", filename);
        exit(1);
    }

    char kata[MAX_KATA];
    while (fgets(kata, sizeof(kata), fp) != NULL) {
        /* hapus newline */
        int len = (int)strlen(kata);
        if (len > 0 && kata[len - 1] == '\n') kata[--len] = '\0';
        if (len > 0 && kata[len - 1] == '\r') kata[--len] = '\0';

        /* lowercase */
        int i;
        for (i = 0; kata[i]; i++)
            kata[i] = (char)tolower((unsigned char)kata[i]);

        len = (int)strlen(kata);
        if (len == 0) continue; /* lewati baris kosong */

        insertTrie(root, kata);
    }
    fclose(fp);
}