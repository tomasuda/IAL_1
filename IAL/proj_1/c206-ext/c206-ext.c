/*
 *  Předmět: Algoritmy (IAL) - FIT VUT v Brně
 *  Rozšíření pro příklad c206.c (Dvousměrně vázaný lineární seznam)
 *  Vytvořil: Daniel Dolejška, září 2024
 */

#include "c206-ext.h"

bool error_flag;
bool solved;

/**
 * Tato metoda simuluje příjem síťových paketů s určenou úrovní priority.
 * Přijaté pakety jsou zařazeny do odpovídajících front dle jejich priorit.
 * "Fronty" jsou v tomto cvičení reprezentovány dvousměrně vázanými seznamy
 * - ty totiž umožňují snazší úpravy pro již zařazené položky.
 * 
 * Parametr `packetLists` obsahuje jednotlivé seznamy paketů (`QosPacketListPtr`).
 * Pokud fronta s odpovídající prioritou neexistuje, tato metoda ji alokuje
 * a inicializuje. Za jejich korektní uvolnení odpovídá volající.
 * 
 * V případě, že by po zařazení paketu do seznamu počet prvků v cílovém seznamu
 * překročil stanovený MAX_PACKET_COUNT, dojde nejdříve k promazání položek seznamu.
 * V takovémto případě bude každá druhá položka ze seznamu zahozena nehledě
 * na její vlastní prioritu ovšem v pořadí přijetí.
 * 
 * @param packetLists Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param packet Ukazatel na strukturu přijatého paketu
 */

//kontroluje ci je fronta prazdna
bool IsEmpty(DLList *packetLists) {
    return (packetLists->firstElement == NULL);
}

// funkcia vytvara novu frontu 
QosPacketListPtr NewFront(DLList *packetLists,QosPacketListPtr targetList,PacketPtr packet){
        targetList = (QosPacketListPtr)malloc(sizeof(QosPacketList));
        targetList->priority = packet->priority;
        targetList->list = (DLList *)malloc(sizeof(DLList));
        if (targetList->list== NULL){
            return NULL;
        }
        DLL_Init(targetList->list);
        DLL_InsertLast(packetLists, (long)targetList);
        return targetList;

}
//funkcia na mazanie kazdeho druheho packetu
void Packed_Dtor( QosPacketListPtr targetList) {
    DLLElementPtr elem = targetList->list->firstElement;
    bool deleteNext = false;

        while (elem != NULL) {
            DLLElementPtr nextElem = elem->nextElement;
            
            if (deleteNext) {
                if (elem->previousElement != NULL) {
                    elem->previousElement->nextElement = elem->nextElement;
                }
                if (elem->nextElement != NULL) {
                    elem->nextElement->previousElement = elem->previousElement;
                }
                free(elem);
                targetList->list->currentLength--;
            }
            deleteNext = !deleteNext;
            elem = nextElem;
        }
    }

void receive_packet(DLList *packetLists, PacketPtr packet) {
    DLLElementPtr element = packetLists->firstElement;
    QosPacketListPtr targetList = NULL;
    
    // Prechádzame fronty a hľadáme frontu s rovnakou prioritou ako má prijatý paket
    while (element != NULL) {
        QosPacketListPtr priorlist = (QosPacketListPtr)element->data;
        if (priorlist->priority == packet->priority) {
            targetList = priorlist;
            break;
        }
        element = element->nextElement;
    }

    if (targetList == NULL) {
       targetList = NewFront(packetLists,targetList,packet);
       if (targetList==NULL){
        error_flag=true;
        return;
        }
    }

    // Pridáme nový paket do konca fronty
    DLL_InsertLast(targetList->list, (long)packet);

    if (targetList->list->currentLength > MAX_PACKET_COUNT) {
        Packed_Dtor(targetList);
    }
}


/**
 * Tato metoda simuluje výběr síťových paketů k odeslání. Výběr respektuje
 * relativní priority paketů mezi sebou, kde pakety s nejvyšší prioritou
 * jsou vždy odeslány nejdříve. Odesílání dále respektuje pořadí, ve kterém
 * byly pakety přijaty metodou `receive_packet`.
 * 
 * Odeslané pakety jsou ze zdrojového seznamu při odeslání odstraněny.
 * 
 * Parametr `packetLists` obsahuje ukazatele na jednotlivé seznamy paketů (`QosPacketListPtr`).
 * Parametr `outputPacketList` obsahuje ukazatele na odeslané pakety (`PacketPtr`).
 * 
 * @param packetLists Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param outputPacketList Ukazatel na seznam paketů k odeslání
 * @param maxPacketCount Maximální počet paketů k odeslání
 */



void send_packets(DLList *packetLists, DLList *outputPacketList, int maxPacketCount) {
    int sentPacketCount = 0;

    // Prechádzanie front podľa priority (od najvyššej po najnižšiu)
    while (sentPacketCount < maxPacketCount) {
        DLLElementPtr element = packetLists->firstElement;
        QosPacketListPtr highestPriorityList = NULL;

        // Nájdeme frontu s najvyššou prioritou
        while (element != NULL) {
            QosPacketListPtr qosList = (QosPacketListPtr)element->data;
            if (!IsEmpty(qosList->list)) {
                if (highestPriorityList == NULL || qosList->priority > highestPriorityList->priority) {
                    highestPriorityList = qosList;
                }
            }
            element = element->nextElement;
        }

        // Ak už neexistujú žiadne fronty s paketmi, ukončíme odosielanie
        if (highestPriorityList == NULL) {
            break;
        }

        // Odosiela prvý paket z fronty s najvyššou prioritou
        PacketPtr packet;
        DLL_GetFirst(highestPriorityList->list,(long *)&packet);

        // Pridá paket do výstupného zoznamu
        DLL_InsertLast(outputPacketList, (long)packet);

        // Odstráni ho zo zdrojovej fronty
        DLL_DeleteFirst(highestPriorityList->list);

        sentPacketCount++;
    }
}