#ifndef SINONIM_H
#define SINONIM_H

#include "trie.h"

void loadSinonim(TrieNode *root, const char *filename);

void tampilSinonim(TrieNode *node);

#endif