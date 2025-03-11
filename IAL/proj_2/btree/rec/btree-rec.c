/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
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
        *tree = NULL;  
    }
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
 if (tree == NULL) {
        return false;
    }
    if (tree->key == key) { // kluc je rovny
        *value = &(tree->content);
        return true;
    }
   else if  (key < tree->key) {// kluc je mensi
        return bst_search(tree->left, key, value);
    }
    else // kluc je vedsi
    return bst_search(tree->right, key, value);
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */

void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
   if (*tree == NULL) {
        *tree = (bst_node_t*)malloc(sizeof(bst_node_t));  
        if (*tree != NULL) {
            (*tree)->key = key;
            (*tree)->content = value;
            (*tree)->left = NULL;
            (*tree)->right = NULL;
        }
         return;
    }

    if ((*tree)->key == key) { // kluc je rovny
         if ((*tree)->content.value != NULL)
           free((*tree)->content.value);
        (*tree)->content = value;
        return;
    }
    else if (key < (*tree)->key) { //kluc je mensi
        bst_insert(&(*tree)->left, key, value);
    }
    else { //kluc je vedsi
        bst_insert(&(*tree)->right, key, value);
    }
}
/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{

   if((*tree)->right == NULL){
        target->key = (*tree)->key;
        if(target->content.value!=NULL)
            free(target->content.value);
        target->content = (*tree)->content;
        bst_node_t *to_delete = *tree;
        *tree = (*tree)->right;
        free(to_delete);
        return;
   }
    else{
      bst_replace_by_rightmost(target, &((*tree)->right));
      return;
    }
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
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
    if (*tree == NULL) 
        return;

    if (key < (*tree)->key) {
        bst_delete(&(*tree)->left, key);
    } else if (key > (*tree)->key) {
        bst_delete(&(*tree)->right, key);
    } else if ((*tree)->left == NULL && (*tree)->right == NULL)  { 

        if((*tree)->content.value!=NULL)
                free((*tree)->content.value); 
            free(*tree);                   
            *tree = NULL;
     } else if ((*tree)->left == NULL) {  
            bst_node_t *to_delete = *tree;
            *tree = (*tree)->right;
            if((*tree)->content.value!=NULL)
                free((to_delete)->content.value); 
            free(to_delete) ;
    } else if ((*tree)->right == NULL) {
            bst_node_t *to_delete = *tree;
            *tree = (*tree)->left;
            if((*tree)->content.value!=NULL)
                free((to_delete)->content.value); 
            free(to_delete) ;       
    }
    
    else 
        bst_replace_by_rightmost(*tree, &(*tree)->left);;

    }

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  if(*tree != NULL)
    {
      bst_dispose(&(*tree)->left);
      bst_dispose(&(*tree)->right);
      if((*tree)->content.value!=NULL)
          free((*tree)->content.value); 
      free(*tree);
      *tree=NULL;
    }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  if (tree != NULL) {
        bst_add_node_to_items(tree, items);
        bst_preorder(tree->left, items);
        bst_preorder(tree->right, items);
    }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
    if (tree != NULL) {
        bst_inorder(tree->left, items);
        bst_add_node_to_items(tree, items);
        bst_inorder(tree->right, items);
    }
}



/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
    if (tree != NULL) {
        bst_postorder(tree->left, items);
        bst_postorder(tree->right, items);
        bst_add_node_to_items(tree, items);
    }
}

