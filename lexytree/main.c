#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"
#include "kamus.h"
#include "sinonim.h"

#define FILE_KAMUS   "kamus.txt"
#define FILE_SINONIM "sinonim.txt"
#define PANJANG_PREFIX 2   /* input harus TEPAT 2 huruf */

static void normPrefix(const char *src, char *dst, int maxLen) {
    int j = 0, i;
    for (i = 0; src[i] != '\0' && j < maxLen - 1; i++) {
        char c = (char)tolower((unsigned char)src[i]);
        if (c >= 'a' && c <= 'z') dst[j++] = c;
    }
    dst[j] = '\0';
}

void tampilMenu(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printf("============================================================\n");
    printf("         LEXI TREE - Aplikasi Pencari Kata Indonesia        \n");
    printf("============================================================\n");
    printf("  Fitur: Autocomplete | Sinonim\n");
    printf("  Masukkan tepat 2 huruf untuk mencari kata\n");
    printf("============================================================\n\n");
}

char *inputUser(char *buffer, int maxLen) {
    while (1) {
        printf("Prefix > ");
        if (fgets(buffer, maxLen, stdin) == NULL) continue;

        /* hapus newline */
        int len = (int)strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') buffer[--len] = '\0';
        if (len > 0 && buffer[len - 1] == '\r') buffer[--len] = '\0';

        /* lowercase (sebelum validasi) */
        int i;
        for (i = 0; buffer[i]; i++)
            buffer[i] = (char)tolower((unsigned char)buffer[i]);

        len = (int)strlen(buffer);

        /* validasi panjang: harus TEPAT 2 */
        if (len != PANJANG_PREFIX) {
            printf("  Input harus tepat 2 huruf. Coba lagi.\n\n");
            continue;
        }

        /* validasi hanya a-z */
        int valid = 1;
        for (i = 0; buffer[i]; i++) {
            if (buffer[i] < 'a' || buffer[i] > 'z') {
                valid = 0;
                break;
            }
        }
        if (!valid) {
            printf("  Hanya huruf a-z yang diperbolehkan. Coba lagi.\n\n");
            continue;
        }

        return buffer;
    }
}

void tampilSuggestion(char hasil[][MAX_KATA], int count) {
    printf("\nSuggestion:\n");
    int i;
    for (i = 0; i < count; i++) {
        printf("  [%d] %-15s", i + 1, hasil[i]);
        if ((i + 1) % 3 == 0) printf("\n");
    }
    if (count % 3 != 0) printf("\n");

    printf("  [0] Tidak ditemukan\n");
    printf("\nPilih nomor (0-%d): ", count);
}

int bacaPilihan(int count) {
    char buf[16];
    while (1) {
        if (fgets(buf, sizeof(buf), stdin) == NULL) continue;

        char *endptr;
        long val = strtol(buf, &endptr, 10);

        if (endptr == buf || val < 0 || val > count) {
            printf("  Nomor tidak valid. Pilih 0-%d: ", count);
            continue;
        }
        return (int)val;
    }
}

int tanyaLanjut(void) {
    char jawab[16];
    while (1) {
        printf("\nCari kata lain? (y/n): ");
        if (fgets(jawab, sizeof(jawab), stdin) == NULL) continue;

        int len = (int)strlen(jawab);
        if (len > 0 && jawab[len - 1] == '\n') jawab[--len] = '\0';

        int i;
        for (i = 0; jawab[i]; i++)
            jawab[i] = (char)tolower((unsigned char)jawab[i]);

        if (strcmp(jawab, "y") == 0) return 1;
        if (strcmp(jawab, "n") == 0) return 0;
        printf("  Input tidak valid. Ketik y atau n.\n");
    }
}

int main(void) {
    TrieNode *root = newTrieNode();

    printf("  Memuat kamus...\n");
    loadKamus(root, FILE_KAMUS);      /* exit(1) di dalam jika gagal */

    printf("  Memuat sinonim...\n");
    loadSinonim(root, FILE_SINONIM);  /* exit(1) di dalam jika gagal */

    printf("  Data berhasil dimuat. Siap digunakan!\n");

    char prefix[MAX_KATA];
    char hasil[MAX_SARAN][MAX_KATA];
    int count;

    do {
        tampilMenu();
        inputUser(prefix, sizeof(prefix));

        getSuggestion(root, prefix, hasil, &count);

        if (count == 0) {
            printf("\nTidak ada kata dengan prefix tersebut\n");
            continue; /* lanjut ke tanyaLanjut, lewati tahap 5 & 6 */
        }

        tampilSuggestion(hasil, count);
        int pilihan = bacaPilihan(count);

        if (pilihan == 0) {
            printf("\nKata tidak ditemukan.\n");
        } else {
            char *kataDipilih = hasil[pilihan - 1];
            printf("\nKata terpilih : %s\n", kataDipilih);
            /* normalisasi sebelum findNode karena trie pakai bentuk a-z saja */
            char kataNorm[MAX_KATA];
            normPrefix(kataDipilih, kataNorm, MAX_KATA);
            TrieNode *node = findNode(root, kataNorm);
            tampilSinonim(node);
        }

    } while (tanyaLanjut());

    freeTrie(root);
    printf("\nTerima kasih telah menggunakan aplikasi.\n\n");
    return 0;
}