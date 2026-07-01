#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "kamus.h"

/* Normalisasi: ambil hanya a-z untuk jalur trie */
static int normalisasiKata(const char *src, char *dst, int maxLen) {
    int j = 0, i;
    for (i = 0; src[i] != '\0' && j < maxLen - 1; i++) {
        char c = (char)tolower((unsigned char)src[i]);
        if (c >= 'a' && c <= 'z') dst[j++] = c;
    }
    dst[j] = '\0';
    return j;
}

/* Bentuk tampil: lowercase + tanda hubung + spasi, buang titik dll */
static int bentukTampil(const char *src, char *dst, int maxLen) {
    int j = 0, i;
    for (i = 0; src[i] != '\0' && j < maxLen - 1; i++) {
        char c = (char)tolower((unsigned char)src[i]);
        if ((c >= 'a' && c <= 'z') || c == '-' || c == ' ')
            dst[j++] = c;
    }
    while (j > 0 && dst[j-1] == ' ') j--;
    dst[j] = '\0';
    return j;
}

void loadKamus(TrieNode *root, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Gagal memuat kamus.txt\n");
        fprintf(stderr, "Pastikan file '%s' ada di folder yang sama.\n", filename);
        exit(1);
    }

    char baris[MAX_KATA * 2];
    char norm[MAX_KATA];
    char tampil[MAX_KATA];

    while (fgets(baris, sizeof(baris), fp) != NULL) {
        int len = (int)strlen(baris);
        if (len > 0 && baris[len-1] == '\n') baris[--len] = '\0';
        if (len > 0 && baris[len-1] == '\r') baris[--len] = '\0';
        if (len == 0) continue;

        /* norm: untuk jalur di trie */
        if (normalisasiKata(baris, norm, MAX_KATA) == 0) continue;

        /* tampil: yang ditampilkan ke user (spasi & tanda hubung tetap) */
        bentukTampil(baris, tampil, MAX_KATA);

        /* simpan keduanya */
        insertTrie(root, norm, tampil);
    }
    fclose(fp);
}