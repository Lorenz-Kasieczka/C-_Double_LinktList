/*----------------------------------------------------------
 *				HTBLA-Leonding / Class: <your class>
 * ---------------------------------------------------------
 * Exercise Number: S02
 * Title:			Doubly Linked List implementation
 * Author:			Lorenz Kasieczka
 * ----------------------------------------------------------
 * Description:
 * Implementation of a doubly linked list.
 * ----------------------------------------------------------
 */

/*
  Implementation notes:

  1) The 'ListData' struct of this linked list SHALL have
     - a pointer to the head node, 
     - a pointer to the tail node
     - and the size of the list 
    as members!

  2) List list, node, and iterator allocation: 
     Use functions `mem_alloc(…)` and `mem_free(…)`
     declared in `allocator.h`. DO NOT use `malloc(…)` and `free(…)` directly
     as unit tests will fail.

  3) Use 'limits.h' to get maximum and minimum values for numeric types, if needed.

  4) Implement 'list_iterator.h' in this file as well.

  5) Avoid code duplication wherever (reasonably) possible.
     This is valid for implementation of similar functions as well
     as for reoccurring code patterns, such as list iteration.
     Nevertheless, aim for efficiency, e.g. `remove_all` shall traverse 
     the list only once and not use `remove` as defined, because
     the later would start at the beginning of the list for each iteration.
*/

#include "doubly_linked_list_with_iterator.h"
#include <stdio.h>
#include "allocator.h"
#include <limits.h>
/** The type of list nodes */
typedef struct IntNodeData* IntNode;
/** The implementation of list node data */
struct IntNodeData {
    IntNode next;
    IntNode prev;
    int payload;
};
/** The implementation of list data */
struct IntListData {
    IntNode head;
};
/** The implementation of list iterator data */
struct IntListIteratorData{
    int length;

    IntNode cur;
    IntNode head;
    IntNode tail;
};
/* ===================================================================== */
/* private list functions */




/* abstract away and generalize also memory allocation for list nodes */
static IntNode list_obtain_node(int value) {
    IntNode node = alloc_mem(sizeof(struct IntNodeData));
    if (node != 0) {
        node->payload = value;
        node->next = 0;
        node->prev = 0;
    }
    return node;

}

static void list_release_node(IntNode node) {
    free_mem(node);
    node = 0;
}

/* optional: implement a function for printing the content of the list - may be useful for debugging */
void list_dump(char* prefix, IntList list) {
    if (list_is_valid(list)) {
        printf("\n%s: X->", prefix);
        IntNode cur = list->head;
        while (cur != 0) {
            printf("[%d]->", cur->payload);
            cur = cur->next;
        }
        printf("@\n");
    } else {
        printf("List is invalid!\n");
    }
}
/* ===================================================================== */
IntList list_obtain(){
    IntList list = alloc_mem(sizeof(struct IntListData));
    if (list != 0) {
        list->head = 0;
    }
    return list;
}
void list_release(IntList* p_list) {
    if (list_is_valid(*p_list)) {
            list_clear(*p_list);
            free_mem(*p_list);
            *p_list = 0;
    }
}
bool list_is_valid(IntList list){
    return list != 0;
}
bool list_is_empty(IntList list){
    return !list_is_valid(list)|| list->head==0;
}
int list_get_size(IntList list){
    int count = 0;
    if (list_is_valid(list)) {
        IntNode cur = list->head;
        while (cur != 0) {
            count++;
            cur = cur->next;
        }
    }
    return count;
}
bool list_contains(IntList list, int value){
    IntNode cur = 0;
    if (list_is_valid(list)) {
        cur = list->head;
        while (cur != 0 && cur->payload != value) {
            cur = cur->next;
        }
    }
    return (cur != 0);
}
int list_get_at(IntList list, unsigned int index){
    int value = 0;
    if (list_is_valid(list)) {
        IntNode temp = list->head;
        for (int i = 0; i < index; ++i) {
            temp = temp->next;
        }
        if (temp != 0) {
            value = temp->payload;
        }
        return value;
    }
    else{
        return 0;
    }

}
void list_insert(IntList list, int value){
    list_insert_at(list,UINT_MAX,value);
}
void list_insert_at(IntList list, unsigned int index, int value){
    if(list_is_valid(list)) {

        IntNode node = list_obtain_node(value);
        if (node != 0) {
            /* search the node before index until end of list */
            if (index != 0 && list->head != 0) {
                IntNode temp = list->head;
                for (unsigned int step = 0; temp->next != 0 && step < index - 1; step++) {
                    temp = temp->next;
                    temp->prev= temp->prev->next;
                }
                node->next = temp->next;
                temp->next = node;
            } else { /* empty list or insert at beginning*/
                node->next = list->head;
                list->head = node;
            }
        }
    }
    else{
        list->head->payload = value;
    }
}
void list_append(IntList list, IntList list_to_append){
    if(list_is_valid(list)&&list_is_valid(list_to_append)){
        IntNode list1 = list->head;
        for (int i = 0; i < list_get_size(list); ++i) {
            list1 = list1->next;

        }
              list1-> next = list_to_append->head;
    }
}
void list_remove(IntList list, int value){
if(list_is_valid(list)){
    IntNode temp = list->head;
    for (int i = 0; i < list_get_size(list); ++i) {
        if(temp->payload==value){
            temp->payload=0;
            return;
        }
       temp = temp->next;
    }
}
}
void list_remove_all(IntList list, int value){
    if(list_is_valid(list)){
        IntNode temp = list->head;
        for (int i = 0; i < list_get_size(list); ++i) {
            if(temp->payload==value){
                temp->payload=0;
            }
            temp = temp->next;
        }
    }
}
int list_remove_at(IntList list, unsigned int index){
    if(list_is_valid(list)){
        IntNode temp = list->head;
        int value;
        for (int i = 0; i < index; ++i) {
            temp = temp->next;
        }
        value = temp->payload;
        temp->payload = 0;
        return value;
    }
    return 0;

}
void list_clear(IntList list){
    if(list_is_valid(list)){
        IntNode temp = list->head;
        for (int i = 0; i < list_get_size(list); ++i) {
            temp->payload=0;
            if (temp->next != 0){
                temp = temp->next;
            }
            else{
                return;
            }

        }
    }
}
IntListIterator list_it_obtain(IntList list){
    if(list_is_valid(list) && list->head != NULL){
        IntListIterator newIt = alloc_mem(sizeof (struct IntListIteratorData));
        if(newIt != NULL){
            newIt->head = list->head;

            IntNode cur = list->head;
            while(cur->next != NULL){
                cur = cur->next;
            }

            newIt->tail = cur;
            newIt->length = list_get_size(list);
            newIt->cur = list->head;
            return newIt;
        }
    }
    return 0;
}

void list_it_release(IntListIterator* p_it){
    free_mem(*p_it);
    *p_it = 0;
}

bool list_it_is_valid(IntListIterator it){
    if(it != NULL)
    {
        return true;
    }
    return false;
}

bool list_it_next(IntListIterator it){
    if(list_it_is_valid(it)){
        if(it->cur->next != NULL){
            it->cur = it->cur->next;
            return true;
        }
        return false;
    }
}

bool list_it_previous(IntListIterator it){
    if(list_it_is_valid(it)){
        if(it->cur->prev != NULL){
            it->cur = it->cur->prev;
            return true;
        }
        return false;
    }
}

int list_it_get(IntListIterator it){
    if(list_it_is_valid(it)){
        return it->cur->payload;
    }
}

void list_it_set(IntListIterator it, int value){
    if(list_it_is_valid(it)){
        it->cur->payload = value;
    }
}



