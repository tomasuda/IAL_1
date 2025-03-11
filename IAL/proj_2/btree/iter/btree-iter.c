/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree)
{
  if (tree != NULL) {
        *tree = NULL;  // Nastavíme ukazatel na kořen stromu na NULL.
    }
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
  bst_node_t *position = tree;  

    while (position != NULL) {
        if (key < position->key) {
            position = position->left;  
        }
        else if (key > position->key) {
            position = position->right;  
        }
        else {  
            *value = &(position->content);  
            return true;
        }
    }
    return false; 
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
    bst_node_t *current = *tree;
    bst_node_t *parent = NULL;
    bool found = false;
    
    while (current != NULL && !found) {
        parent = current;
        
        if (key < current->key) {
            current = current->left;  
        } else if (key > current->key) {
            current = current->right;
        } else {
            found = true;           
        }
    }

    if (found) {
        if (current->content.value != NULL) {
            free(current->content.value); 
        }
        current->content = value; 
    } 
    else {
        bst_node_t *new_node = malloc(sizeof(bst_node_t));
        if (new_node == NULL) return; 

        new_node->key = key;
        new_node->content = value;
        new_node->left = NULL;
        new_node->right = NULL;

        if (parent == NULL) {//novy uzol je koren
            *tree = new_node;
        } else if (key < parent->key) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
    }
  

}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
    bst_node_t *parent = NULL;
    bst_node_t *current = *tree;

    while (current->right != NULL) {
        parent = current;
        current = current->right;
    }

    target->key = current->key;
    free(target->content.value);
    target->content = current->content;

    if (parent != NULL) {
        parent->right = current->left; 
    } else {
        *tree = current->left; 
    }

    free(current);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
    bst_node_t *parent = NULL;
    bst_node_t *current = *tree;

    while (current != NULL && current->key != key) {// vyhladavanie uzlu s klucom
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (current == NULL) {// uzol neexistuje
        return;
    }

    // Uzel s klucom
    if (current->left == NULL || current->right == NULL) {
        bst_node_t *child = (current->left != NULL) ? current->left : current->right;

        if (parent == NULL) {
            *tree = child;
        } else if (parent->left == current) {
            parent->left = child;
        } else {
            parent->right = child;
        }
        free(current->content.value);
        free(current); 
    } else {
        bst_replace_by_rightmost(current, &current->left);
    }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
    stack_bst_t s1;
    stack_bst_init(&s1);

     do {
        if (*tree == NULL) {
            if(!stack_bst_empty(&s1)){
                *tree = stack_bst_top(&s1);
                stack_bst_pop(&s1);
            }
        } 
        else {
            if ((*tree)->right != NULL) {
                 stack_bst_push(&s1,(*tree)->right);
            }
            bst_node_t *aux = *tree;
             *tree = (*tree)->left;
            free((aux)->content.value);
            free(aux);
        }
    }while ((*tree != NULL) || (!stack_bst_empty(&s1)));
    *tree = NULL;
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items)
{  
    while (tree != NULL) {
        stack_bst_push(to_visit,tree);
        bst_add_node_to_items(tree,items);
        tree = tree->left;
    }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
    if (tree == NULL) 
        return;

    stack_bst_t to_visit;
    stack_bst_init(&to_visit);
    stack_bst_top(&to_visit);
    bst_leftmost_preorder(tree, &to_visit, items);

    while (!stack_bst_empty(&to_visit)) {
        tree = stack_bst_top(&to_visit);
        stack_bst_pop(&to_visit);
        bst_leftmost_preorder(tree->right, &to_visit, items);
    }
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
    while (tree != NULL) {
        stack_bst_push(to_visit, tree);  
        tree = tree->left;              
    }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
    stack_bst_t s1;
    stack_bst_init(&s1);
    bst_leftmost_inorder(tree,&s1);
while (!stack_bst_empty(&s1)){
    tree = stack_bst_top(&s1);
    stack_bst_pop(&s1);
    bst_add_node_to_items(tree,items); // změna od PreOrder
    bst_leftmost_inorder(tree->right,&s1);
}

}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
   
    while (tree != NULL){
    stack_bst_push(to_visit,tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
    }


}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
   bool fromLeft;
    stack_bst_t s1;
    stack_bool_t sb1;
  
    stack_bst_init(&s1);     
    stack_bool_init(&sb1);     

    bst_leftmost_postorder(tree, &s1, &sb1); // lavy podstrom

    while (!stack_bst_empty(&s1)) {
        tree = stack_bst_top(&s1);       
        fromLeft = stack_bool_top(&sb1); 
        stack_bool_pop(&sb1);        

        if (fromLeft) {
            stack_bool_push(&sb1, false); // pravy strom musi cakat
            bst_leftmost_postorder(tree->right, &s1, &sb1); 
        } else {
            stack_bst_pop(&s1);
            bst_add_node_to_items(tree, items);
        }
    }
}
