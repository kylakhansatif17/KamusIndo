#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sinonim.h"

#define MAX_GRUP  30      /* maks kata dalam satu kelompok sinonim */
#define MAX_BARIS 512     /* panjang maks satu baris di sinonim.txt */

/* Hapus spasi di awal dan akhir string */
static void trimSpasi(char *s) {
    int start = 0;
    while (s[start] && isspace((unsigned char)s[start])) start++;
    if (start > 0) {
        int i = 0;
        while (s[start]) s[i++] = s[start++];
        s[i] = '\0';
    }
    int len = (int)strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1]))
        s[--len] = '\0';
}

/* Ubah seluruh karakter string menjadi huruf kecil. */
static void keLowercase(char *s) {
    int i;
    for (i = 0; s[i]; i++)
        s[i] = (char)tolower((unsigned char)s[i]);
}

static void tambahSinonim(TrieNode *node, const char *kata) {
    SinonimNode *cek = node->sinonim;
    while (cek != NULL) {
        if (strcmp(cek->kata, kata) == 0) return; /* sudah ada */
        cek = cek->next;
    }

    SinonimNode *baru = (SinonimNode *)malloc(sizeof(SinonimNode));
    if (baru == NULL) {
        fprintf(stderr, "Error: malloc gagal di tambahSinonim\n");
        exit(1);
    }
    strncpy(baru->kata, kata, MAX_KATA - 1);
    baru->kata[MAX_KATA - 1] = '\0';
    baru->next = node->sinonim;
    node->sinonim = baru;
}

void loadSinonim(TrieNode *root, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Gagal memuat sinonim.txt\n");
        fprintf(stderr, "Pastikan file '%s' ada di folder yang sama.\n", filename);
        exit(1);
    }

    char baris[MAX_BARIS];
    char buf_kata[MAX_GRUP][MAX_KATA];
    char *semua_kata[MAX_GRUP];
    int jumlah;

    while (fgets(baris, sizeof(baris), fp) != NULL) {
        int len = (int)strlen(baris);
        if (len > 0 && baris[len - 1] == '\n') baris[--len] = '\0';
        if (len > 0 && baris[len - 1] == '\r') baris[--len] = '\0';
        if (len == 0) continue;

        char *eq = strchr(baris, '=');
        if (eq == NULL) continue;

        *eq = '\0';
        char *bagian_kiri  = baris;      
        char *bagian_kanan = eq + 1;     

        trimSpasi(bagian_kiri);
        keLowercase(bagian_kiri);
        if (strlen(bagian_kiri) == 0) continue;

        jumlah = 0;

        strncpy(buf_kata[jumlah], bagian_kiri, MAX_KATA - 1);
        buf_kata[jumlah][MAX_KATA - 1] = '\0';
        semua_kata[jumlah] = buf_kata[jumlah];
        jumlah++;

        char *token = strtok(bagian_kanan, ",");
        while (token != NULL && jumlah < MAX_GRUP) {
            trimSpasi(token);
            keLowercase(token);
            if (strlen(token) > 0) {
                strncpy(buf_kata[jumlah], token, MAX_KATA - 1);
                buf_kata[jumlah][MAX_KATA - 1] = '\0';
                semua_kata[jumlah] = buf_kata[jumlah];
                jumlah++;
            }
            token = strtok(NULL, ",");
        }

        int i, j;
        for (i = 0; i < jumlah; i++) {
            TrieNode *node = findNode(root, semua_kata[i]);
            if (node == NULL) continue; 
            for (j = 0; j < jumlah; j++) {
                if (j == i) continue;
                tambahSinonim(node, semua_kata[j]);
            }
        }
    }

    fclose(fp);
}

/* Menampilkan daftar sinonim */
void tampilSinonim(TrieNode *node) {
    if (node == NULL || node->sinonim == NULL) {
        printf("  Sinonim tidak tersedia.\n");
        return;
    }
    printf("  Sinonim: ");
    SinonimNode *curr = node->sinonim;
    while (curr != NULL) {
        printf("%s", curr->kata);
        if (curr->next != NULL) printf(", ");
        curr = curr->next;
    }
    printf("\n");
}


