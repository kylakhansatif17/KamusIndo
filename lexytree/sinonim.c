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
