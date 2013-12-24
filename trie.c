#include "trie.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

//r0456142, F. Yigit Ozkan

Trie* trie_alloc() {
	Trie *practTrie = NULL;
	practTrie = (Trie *) malloc(sizeof(Trie));

	if (practTrie == NULL) {
		// malloc error
		printf("ERROR: malloc failed\n");
		return NULL;
	}
	if (practTrie != NULL) {
		// initalize trie root value and children's values
		practTrie->value = NULL;
		int i;
		for (i = 0; i < TRIE_NB_CHILDREN; i++) {
			practTrie->children[i] = NULL;
		}
	}
	return practTrie;
}

void trie_free(Trie* trie) {
	if (trie == NULL) {
		return;
	}
	int i;
	for (i = 0; i < TRIE_NB_CHILDREN; i++) {
		if (trie->children[i] != NULL) {
			trie_free(trie->children[i]);
		}
	}

	free(trie->value);
	trie->value = NULL;
	free(trie);
	trie = NULL;
	return;
}

int trie_is_valid_key(const char* key) {
	register int i = 0;

	int thereIsUpper = 0;

	while (key[i] != '\0') {
		int ascii = key[i];
		if (ascii < 97 || ascii > 122) {
			thereIsUpper = 1;
		}
		i++;
	}

	if (thereIsUpper == 0) {
		return 1;
	} else {
		return 0;
	}

}

int trie_get_child_index(char c) {
	int ascii = c;
	int result = ascii - 97;
	return result;
}

char trie_get_child_char(int i) {
	int ascii = i + 97;
	char result = ascii;
	return result;
}

int trie_insert(Trie* trie, const char* key, const char* value) {
	if (trie_is_valid_key(key) != 1) {
		// invalid key
		return 0;
	}

	if (key[0] == '\0') {
		//found the key
		//printf("found the key: %s",key);
		free(trie->value);
		trie->value = strdup(value);
		return 1;
	} else {
		// recursive
		int k = trie_get_child_index(key[0]);
		if (trie->children[k] == NULL) {
			trie->children[k] = trie_alloc();
		}
		char key_without_first_char[100];
		strcpy(key_without_first_char, key + 1);
		//printf("key without first char: %s \n",key_without_first_char);
		trie_insert(trie->children[k], key_without_first_char, value);
	}
	return 1;
}

const char* trie_lookup(Trie* trie, const char* key) {
	if (trie_is_valid_key(key) != 1) {
		printf("Key is not valid!");
		return NULL;
	}

	int i;
	char* result = NULL;
	int length = strlen(key);
	for (i = 0; i <= length; i++) {
		// iterate through the key
		int x = trie_get_child_index(key[i]);
		if (x == -97) {
			return result;
		} else {
			Trie* kid = trie->children[x];
			if (kid == NULL) {
				return NULL;
			}
			trie = kid;
			result = trie->value;
		}
	}
	return result;
}

//gegeven operaties

static void trie_remove_aux(Trie** trie, const char* key) {
	if (*trie == NULL)
		return;

	if (*key == '\0') // key found
			{
		free((*trie)->value);
		(*trie)->value = NULL;
	} else {
		Trie** child = &(*trie)->children[trie_get_child_index(*key)];
		trie_remove_aux(child, key + 1);
	}

	int i;
	for (i = 0; i < TRIE_NB_CHILDREN; i++) {
		if ((*trie)->children[i] != NULL)
			return;
	}

	// all children are NULL: remove current node
	free(*trie);
	*trie = NULL;
}

void trie_remove(Trie* trie, const char* key) {
	trie_remove_aux(&trie, key);
}

void trie_insert_from_file(Trie* trie, const char* file_name) {
	FILE* file = fopen(file_name, "r");

	if (file == NULL) {
		fprintf(stderr, "Unable to open %s for reading: %s\n", file_name,
				strerror(errno));
		return;
	}

	while (!feof(file)) {
		char key[64];
		char value[64];
		int nb_matched = fscanf(file, "%63[a-z] : %63[a-z]\n", key, value);

		if (nb_matched == 2)
			trie_insert(trie, key, value);
		else {
			fprintf(stderr, "Syntax error while reading file\n");
			fclose(file);
			return;
		}
	}

	fclose(file);
}

static char* str_append_char(const char* str, char c) {
	size_t len = strlen(str);
	char* new_str = malloc(len + 2);
	strcpy(new_str, str);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return new_str;
}

static void trie_print_mappings_aux(Trie* trie, const char* current_prefix) {
	if (trie->value != NULL)
		printf("%s: %s\n", current_prefix, trie->value);

	int i;
	for (i = 0; i < TRIE_NB_CHILDREN; i++) {
		Trie* child = trie->children[i];

		if (child != NULL) {
			char* child_prefix = str_append_char(current_prefix,
					trie_get_child_char(i));
			trie_print_mappings_aux(child, child_prefix);
			free(child_prefix);
		}
	}
}

void trie_print_mappings(Trie* trie) {
	trie_print_mappings_aux(trie, "");
}

static void trie_to_dot_aux(Trie* trie, FILE* file) {
	const char* label = trie->value != NULL ? trie->value : "NULL";
	fprintf(file, "    \"%p\" [label=\"%s\"];\n", trie, label);

	int i;
	for (i = 0; i < TRIE_NB_CHILDREN; i++) {
		Trie* child = trie->children[i];

		if (child != NULL) {
			trie_to_dot_aux(child, file);
			fprintf(file, "    \"%p\" -> \"%p\" [label=\"%c\"];\n", trie, child,
					trie_get_child_char(i));
		}
	}
}

void trie_to_dot(Trie* trie, const char* file_name) {
	FILE* file = fopen(file_name, "w");

	if (file == NULL) {
		fprintf(stderr, "Unable to open %s for writing: %s\n", file_name,
				strerror(errno));
		return;
	}

	fprintf(file, "digraph G\n{\n");
	trie_to_dot_aux(trie, file);
	fprintf(file, "}\n");

	fclose(file);
}
