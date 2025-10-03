// usagi_shopping_list.c
// Interactive shopping list builder "Usagi" in C
// Features:
// - Add items interactively (Usagi asks: "What do you want to add?")
// - After each add, Usagi asks: "Anything else? (y/n)"
// - View, remove, save, load, and quit commands
// - Uses a dynamically resizing array to store items

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_CAPACITY 8
#define BUFFER_SIZE 1024

// Utility: trim newline and whitespace from end
void rtrim(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r' || isspace((unsigned char)s[n-1]))) {
        s[--n] = '\0';
    }
}

// Shopping list structure
typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} ShoppingList;

ShoppingList *create_list(void) {
    ShoppingList *list = malloc(sizeof(ShoppingList));
    if (!list) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    list->capacity = INITIAL_CAPACITY;
    list->count = 0;
    list->items = calloc(list->capacity, sizeof(char*));
    if (!list->items) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    return list;
}

void free_list(ShoppingList *list) {
    if (!list) return;
    for (size_t i = 0; i < list->count; ++i) free(list->items[i]);
    free(list->items);
    free(list);
}

void ensure_capacity(ShoppingList *list) {
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        char **tmp = realloc(list->items, list->capacity * sizeof(char*));
        if (!tmp) {
            fprintf(stderr, "Out of memory\n");
            exit(EXIT_FAILURE);
        }
        list->items = tmp;
    }
}

void add_item(ShoppingList *list, const char *item) {
    if (!item || item[0] == '\0') return;
    ensure_capacity(list);
    list->items[list->count] = strdup(item);
    if (!list->items[list->count]) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    list->count++;
}

void print_list(const ShoppingList *list) {
    if (list->count == 0) {
        printf("(shopping list is empty)\n");
        return;
    }
    printf("Your shopping list:\n");
    for (size_t i = 0; i < list->count; ++i) {
        printf("%zu. %s\n", i+1, list->items[i]);
    }
}

void remove_item(ShoppingList *list, size_t index) {
    if (index >= list->count) {
        printf("Invalid index\n");
        return;
    }
    free(list->items[index]);
    for (size_t i = index; i + 1 < list->count; ++i) {
        list->items[i] = list->items[i+1];
    }
    list->count--;
}

void save_to_file(const ShoppingList *list, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("Failed to open file");
        return;
    }
    for (size_t i = 0; i < list->count; ++i) {
        fprintf(f, "%s\n", list->items[i]);
    }
    fclose(f);
    printf("Saved %zu items to '%s'\n", list->count, filename);
}

void load_from_file(ShoppingList *list, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Failed to open file");
        return;
    }
    char buf[BUFFER_SIZE];
    while (fgets(buf, sizeof(buf), f)) {
        rtrim(buf);
        if (buf[0] == '\0') continue;
        add_item(list, buf);
    }
    fclose(f);
    printf("Loaded items from '%s' (now %zu items)\n", filename, list->count);
}

// Parse a command like: /save filename
// returns 1 if handled as command, 0 otherwise
int try_handle_command(ShoppingList *list, const char *input) {
    if (input[0] != '/') return 0;
    // copy input so we can tokenize
    char tmp[BUFFER_SIZE];
    strncpy(tmp, input, sizeof(tmp)-1);
    tmp[sizeof(tmp)-1] = '\0';
    char *cmd = strtok(tmp, " \t");
    if (!cmd) return 1;

    if (strcmp(cmd, "/view") == 0) {
        print_list(list);
    } else if (strcmp(cmd, "/remove") == 0) {
        char *arg = strtok(NULL, " \t");
        if (!arg) {
            printf("Usage: /remove INDEX\n");
        } else {
            long idx = strtol(arg, NULL, 10);
            if (idx <= 0) printf("Specify a positive index\n");
            else remove_item(list, (size_t)idx - 1);
        }
    } else if (strcmp(cmd, "/save") == 0) {
        char *arg = strtok(NULL, " \t");
        if (!arg) printf("Usage: /save filename\n");
        else save_to_file(list, arg);
    } else if (strcmp(cmd, "/load") == 0) {
        char *arg = strtok(NULL, " \t");
        if (!arg) printf("Usage: /load filename\n");
        else load_from_file(list, arg);
    } else if (strcmp(cmd, "/clear") == 0) {
        while (list->count) remove_item(list, 0);
        printf("Cleared the list\n");
    } else if (strcmp(cmd, "/help") == 0) {
        printf("Commands:\n");
        printf("  /view            - show list\n");
        printf("  /remove INDEX    - remove item by number\n");
        printf("  /save FILE       - save list to file\n");
        printf("  /load FILE       - load items from file (appends)\n");
        printf("  /clear           - remove all items\n");
        printf("  /quit            - quit immediately\n");
    } else if (strcmp(cmd, "/quit") == 0) {
        printf("Goodbye!\n");
        exit(EXIT_SUCCESS);
    } else {
        printf("Unknown command. Type /help for commands.\n");
    }
    return 1;
}

int main(void) {
    ShoppingList *list = create_list();
    char buf[BUFFER_SIZE];

    printf("Welcome to Usagi's Shopping List!\n");
    printf("Type an item to add it. Type /help for commands.\n\n");

    for (;;) {
        printf("Usagi: What do you want to add? \n> ");
        if (!fgets(buf, sizeof(buf), stdin)) break;
        rtrim(buf);
        if (buf[0] == '\0') {
            printf("(no input)\n");
            continue;
        }
        // Check for commands
        if (try_handle_command(list, buf)) continue;

        // Add the provided item
        add_item(list, buf);
        printf("Added: %s\n", buf);

        // Ask if anything else
        for (;;) {
            printf("Usagi: Anything else? (y/n) \n> ");
            if (!fgets(buf, sizeof(buf), stdin)) goto done;
            rtrim(buf);
            if (buf[0] == '\0') { printf("Please answer y or n.\n"); continue; }
            if (buf[0] == 'y' || buf[0] == 'Y') {
                break; // ask for next item
            } else if (buf[0] == 'n' || buf[0] == 'N') {
                goto done;
            } else if (buf[0] == '/') {
                // allow commands here too
                if (try_handle_command(list, buf)) continue;
                else { printf("Please answer y or n or enter a command starting with /.\n"); }
            } else {
                printf("Please answer y or n. You can also use /view, /save, /help, etc.\n");
            }
        }
    }

done:
    printf("\nFinal list:\n");
    print_list(list);
    free_list(list);
    return 0;
}
