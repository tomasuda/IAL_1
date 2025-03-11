#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
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
 */

char toLowercase(char ch) {
    if (ch >= 'A' && ch <= 'Z') {
        return ch + ('a' - 'A'); 
    }
    return ch;  
}

int isLetter(char ch) {
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
        return true;  
    }
    return false;  
}
void letter_count(bst_node_t **tree, char *input) {
     bst_init(tree);  

    int i = 0;
    while (input[i] != '\0') {//prechadzame retazec
        char key = input[i];
        bst_node_content_t *value = NULL;
        i++;


        if (isalpha(key)) { 
            key = toLowercase(key);// pismeno
        } else if (key == ' ') { // Znak mezery
            key = ' ';
        } else { // Ostatne znaky
            key = '_';
        }

    
        if (bst_search(*tree, key, &value)) {

            (*(int *)(value->value))++;
        } else {
            //pokial neexistuje tvoryme novy uzol
            bst_node_content_t new_content;
            new_content.value = malloc(sizeof(int));  // Alokujeme miesto pro int
            if (new_content.value == NULL) {
                bst_dispose(tree);
                return;
            }
            *(int *)(new_content.value) = 1;  // Nastavíme počet na 1
            new_content.type = INTEGER;  

            // Vložíme nový uzol s klíčem `key` a hodnotou `new_content`
            bst_insert(tree, key, new_content);
        }
    }

}



