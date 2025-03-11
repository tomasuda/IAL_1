#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
 */
void letter_count(bst_node_t **tree, char *input) {
    // bst_init(tree);  // Inicializácia stromu

    // int i = 0;
    // while (input[i] != '\0') {  // Prechádzame každý znak reťazca
    //     char key = input[i];
    //     bst_node_content_t *value = NULL;

    //     // Určíme kľúč na základe typu znaku
    //     if (isalpha(key)) { // a-z, A-Z znaky (case-insensitive)
    //         key = tolower(key);
    //     } else if (key == ' ') { // Znak medzery
    //         key = ' ';
    //     } else { // Ostatné znaky
    //         key = '_';
    //     }

    //     // Skontrolujeme, či sa znak už nachádza v strome
    //     if (bst_search(*tree, key, &value)) {
    //         // Ak už existuje, zvýšime počet výskytov
    //         (*(int *)(value->value))++;
    //     } else {
    //         // Ak neexistuje, vytvoríme nový uzol s počiatočným počtom 1
    //         bst_node_content_t new_content;
    //         new_content.value = malloc(sizeof(int));  // Alokujeme miesto pre int
    //         if (new_content.value == NULL) {
    //             fprintf(stderr, "Chyba alokace paměti.\n");
    //             exit(1);
    //         }
    //         *(int *)(new_content.value) = 1;  // Nastavíme počet na 1
    //         new_content.type = INTEGER;  // Ujistíme se, že používáme správný typ (INTEGER)

    //         // Vložíme nový uzol s kľúčom `key` a hodnotou `new_content`
    //         bst_insert(tree, key, new_content);
    //     }
    //     i++; // Pokračujeme na ďalší znak
    // }
}

// Testovacia funkcia pre letter_count
void count(void) {
    char input[] = "abBccc_ 123 *";
    
    bst_node_t *tree = NULL;
    letter_count(&tree, input);

    bst_node_content_t *value = NULL;

    // Test pre znak 'a'
    if (bst_search(tree, 'a', &value)) {
        if (*(int *)(value->value) == 1) {
            printf("Test passed for 'a'.\n");
        } else {
            printf("Test failed for 'a'. Expected 1, got %d.\n", *(int *)(value->value));
        }
    }

    // Test pre znak 'b'
    if (bst_search(tree, 'b', &value)) {
        if (*(int *)(value->value) == 2) {
            printf("Test passed for 'b'.\n");
        } else {
            printf("Test failed for 'b'. Expected 2, got %d.\n", *(int *)(value->value));
        }
    }

    // Test pre znak 'c'
    if (bst_search(tree, 'c', &value)) {
        if (*(int *)(value->value) == 3) {
            printf("Test passed for 'c'.\n");
        } else {
            printf("Test failed for 'c'. Expected 3, got %d.\n", *(int *)(value->value));
        }
    }

    // Test pre medzeru
    if (bst_search(tree, ' ', &value)) {
        if (*(int *)(value->value) == 2) {
            printf("Test passed for space.\n");
        } else {
            printf("Test failed for space. Expected 2, got %d.\n", *(int *)(value->value));
        }
    }

    // Test pre znak '_'
    if (bst_search(tree, '_', &value)) {
        if (*(int *)(value->value) == 5) {
            printf("Test passed for '_'.\n");
        } else {
            printf("Test failed for '_'. Expected 5, got %d.\n", *(int *)(value->value));
        }
    }

    // Zrušenie stromu
    bst_dispose(&tree);
}
