#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"
#include "kamus.h"
#include "sinonim.h"

#define FILE_KAMUS   "kamus.txt"
#define FILE_SINONIM "sinonim.txt"
#define PANJANG_PREFIX 2  

/* Menampilkan menu dan instruksi penggunaan. */
void tampilMenu(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printf("============================================================\n");
    printf("         Aplikasi Pencari Kata Indonesia        \n");
    printf("============================================================\n");
    printf("  Fitur: Autocomplete | Sinonim\n");
    printf("  Masukkan tepat 2 huruf untuk mencari kata\n");
    printf("============================================================\n\n");
}

/* Input user menerima prefix 2 huruf a-z */
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

        /* validasi prefix tepat 2 huruf*/
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
/* Membaca input pilihan user */
int bacaPilihan(int count) {
    char buf[16];

    while (1) {
        if (!fgets(buf, sizeof(buf), stdin))
            continue;

        if (strlen(buf) != 2 || !isdigit((unsigned char)buf[0])) {
            printf("  Input harus satu angka (0-%d): ", count);
            continue;
        }

        int pilihan = buf[0] - '0';

        if (pilihan > count) {
            printf("  Nomor tidak valid. Pilih 0-%d: ", count);
            continue;
        }

        return pilihan;
    }
}

/* Pertanyaan untuk melanjutkan pencarian kata lain (y/n) */
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

int main(void) {
    TrieNode *root = newTrieNode();

    printf("  Memuat kamus...\n");
    loadKamus(root, FILE_KAMUS);      

    printf("  Memuat sinonim...\n");
    loadSinonim(root, FILE_SINONIM);  
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
            TrieNode *node = findNode(root, kataDipilih);
            tampilSinonim(node);
        }

    } while (tanyaLanjut());

    freeTrie(root);
    printf("\nTerima kasih telah menggunakan aplikasi.\n\n");
    return 0;
}

