#include "trie.h"
#include <stdio.h>
#include <string.h>

int main() {
	printf("[*] allocating")
	struct Trie* trie = trie_alloc();
	printf("[*] inserting from file")
	trie_insert_from_file(trie, "dictionary.txt");
	trie_print_mappings(trie);
	printf("%s",trie_lookup(trie, "at"));
	trie_to_dot(trie, "trie.dot");
	printf("[*] freeing trie \n")
	trie_free(trie);
	printf("[*] printing mappings \n");
	trie = trie_alloc();
	trie_print_mappings(trie);
	return 0;
}

