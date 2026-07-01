#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "kamus.h"

/* Validasi untuk mengambil karakter a-z saja */
static int normalisasiKata(const char *src, char *dst, int maxLen) {
    int j = 0;
    int i;
    for (i = 0; src[i] != '\0' && j < maxLen - 1; i++) {
        char c = (char)tolower((unsigned char)src[i]);
        if (c >= 'a' && c <= 'z')
            dst[j++] = c;
    }
    dst[j] = '\0';
    return j;
}

/* Membaca file kamus dan menyisipkan setiap huruf ke dalam Trie  */
void loadKamus(TrieNode *root, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Gagal memuat kamus.txt\n");
        fprintf(stderr, "Pastikan file '%s' ada di folder yang sama.\n", filename);
        exit(1);
    }

    char baris[MAX_KATA * 2];
    char kata[MAX_KATA];

    while (fgets(baris, sizeof(baris), fp) != NULL) {
        /* hapus newline */
        int len = (int)strlen(baris);
        if (len > 0 && baris[len - 1] == '\n') baris[--len] = '\0';
        if (len > 0 && baris[len - 1] == '\r') baris[--len] = '\0';
        if (len == 0) continue;

        /* normalisasi: ambil hanya a-z */
        int normLen = normalisasiKata(baris, kata, MAX_KATA);
        if (normLen == 0) continue;

        insertTrie(root, kata);
    }
    fclose(fp);
}