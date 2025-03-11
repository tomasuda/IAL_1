/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
   for (int i = 0; i < MAX_HT_SIZE; i++) {
        (*table)[i] = NULL;
    }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    if(key==NULL || table == NULL)
    return NULL;
    int index = get_hash(key) % MAX_HT_SIZE;  // musi ostat v hraniciach tabulky
    ht_item_t *item = (*table)[index];   


    while (item != NULL) {
        if (strcmp(item->key, key) == 0) {
            return item;  // nasiel sa prvok
        }
        item = item->next; 
    }

    return NULL;  // hladany kluc sa nenasiel
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    if (key == NULL || table == NULL) return;

    ht_item_t *item = ht_search(table, key);
    if (item == NULL) {
        int index = get_hash(key) % MAX_HT_SIZE;
        
        ht_item_t *new_item = (ht_item_t *)malloc(sizeof(ht_item_t));
        if (new_item == NULL) {
            return;
        }

        new_item->key = (char *)malloc(strlen(key) + 1);
        if (new_item->key == NULL) {
            free(new_item);  // Uvoľníme alokovanú ked zlyha druha alokacia
            new_item=NULL;
            return;
        }

        strcpy(new_item->key, key);
        new_item->value = value;
        new_item->next = (*table)[index];
        (*table)[index] = new_item;
    } else {
        item->value = value;
    }
}


  

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  if(key==NULL || table == NULL)
    return NULL;
  ht_item_t *item = ht_search(table,key); // miesto v tabulke
  if (item == NULL ){
    return NULL;
  }
  else{
    return &item->value;
  }
  
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  if(key==NULL || table == NULL)
    return;

  int index = get_hash(key) % MAX_HT_SIZE;
    ht_item_t *current = (*table)[index];
    ht_item_t *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (previous == NULL) {
                (*table)[index] = current->next;
            } else {
                previous->next = current->next;
            }
            free(current->key); 
             current->key = NULL;
            free(current);  
            current = NULL;     
            return;
        }
        previous = current;
        current = current->next;
    }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  if(table == NULL)
      return;

    for (int i = 0; i < MAX_HT_SIZE; i++) {
        ht_item_t *item = (*table)[i];
        
        while (item != NULL) {
            ht_item_t *next_item = item->next; 
            free(item->key);  
            item->key =NULL;
            free(item);       
            item = next_item; 
        }

        (*table)[i] = NULL;
    }
}
