#ifndef TRIE_H
#define TRIE_H

#define TRIE_NB_CHILDREN 26

/**
 * Type representing a node in a trie. Note that a trie itself is represented by
 * its root node.
 */
typedef struct Trie
{
    char* key;
    /**
     * The value stored in this node. Can be NULL if no value is stored.
     */
    char* value;

    /**
     * Array holding pointers to the children of this node. children[i] is a
     * pointer to the child representing character i + 'a'/
     */
    struct Trie* children[TRIE_NB_CHILDREN];
} Trie;

/**
 * Allocate a new empty trie.
 *
 * This will dynamically allocate a new struct Trie and return a pointer to it.
 * The value pointer and all pointers in the children array will be set to NULL.
 *
 * @returns The newly allocated struct Trie or NULL if we are out of memory.
 */
struct Trie* trie_alloc();

/**
 * Frees the given trie.
 *
 * All memory that has ever been dynamically allocated for the given trie will
 * be freed after invoking this function.
 *
 * @param trie The trie to be freed
 */
void trie_free(struct Trie* trie);

/**
 * Returns whether the given key is valid to be used in a trie.
 *
 * A key is valid if all its characters are lowercase letters (i.e., [a-z]).
 *
 * @param key The key to check.
 *
 * @returns 1 if the given key is valid, 0 otherwise.
 */
int trie_is_valid_key(const char* key);

/**
 * Returns the index in the children array corresponding to the given character.
 *
 * @param c The character to get the index for.
 *
 * @returns 0 for 'a', 1 for 'b',...
 */
int trie_get_child_index(char c);

/**
 * Returns the character corresponding to the given index in the children array.
 *
 * @param i The index to get the character for.
 *
 * @returns 'a' for 0, 'b' for 1,...
 */
char trie_get_child_char(int i);

/**
 * Inserts the given key-value mapping in the given trie.
 *
 * If the trie does not contain the given mapping yet, a new mapping
 * will be created. Otherwise, the existing mapping will be updated
 * and the original value is freed.
 *
 * @param trie  The trie to insert the mapping in.
 * @param key   The key to use in the mapping
 * @param value The value to insert for the given key. The given string will be
 *              copied so it may be freed after this function returns.
 *
 * @returns 1 if the mapping was successfully inserted, 0 otherwise. Insertion
 *          can fail if the given key is not valid (see trie_is_valid_key) or if
 *          we are out of memory.
 */
int trie_insert(struct Trie* trie, const char* key, const char* value);

/**
 * Return the value associated with the given key in the given trie.
 *
 * @param trie The trie do the lookup on.
 * @param key  The key to get the value for.
 *
 * @returns The value associated with key in the trie or NULL if the given key
 *          is not present in the trie.
 */
const char* trie_lookup(struct Trie* trie, const char* key);

/**
 * Removes the given key from the trie.
 *
 * If the trie contains the given key, trie_lookup(trie, key) will return NULL
 * after this function has been called. Otherwise, this function does nothing.
 *
 * @param trie The trie to remove the key from.
 * @param key  The key to remove.
 */
void trie_remove(struct Trie* trie, const char* key);

/**
 * Inserts mappings from a text file into a trie.
 *
 * The given file should contain lines of the form key:value. For each such line
 * a trie_insert(trie, key, value) will be called.
 *
 * @param trie      The trie to insert the mapings into.
 * @param file_name The name of the file to read the mappings from.
 */
void trie_insert_from_file(struct Trie* trie, const char* file_name);

/**
 * Print all mappings in the given trie.
 *
 * All mappings will be printed in the form key:value.
 *
 * @param trie The trie to print the mappings from.
 */
void trie_print_mappings(struct Trie* trie);

/**
 * Creates a graphical representation of the given trie.
 *
 * The created file can be converted to a PDF file by running:
 * @code
 * dot -Tpdf file_name -o file.pdf
 * @endcode
 *
 * @param trie The trie to create a graphical representation of.
 */
void trie_to_dot(struct Trie* trie, const char* file_name);

#endif
