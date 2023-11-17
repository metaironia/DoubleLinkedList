#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "classic_list_func.h"
#include "classic_list_log.h"


enum ListStatus ClassicListCtor (ClassicList *created_list, const size_t list_capacity) {

    assert (created_list);

    if (list_capacity <= 0)
        return LIST_STATUS_FAIL;

    created_list -> capacity  = list_capacity + 1;
    created_list -> list_size = 0;
    created_list -> mainItems = ClassicListStructArrayCtor (created_list);

    ON_DEBUG (CLASSIC_LIST_DUMP (created_list));

    ClassicListCreateDummyNode (created_list);

    (created_list -> controlItems).free = &(created_list -> mainItems)[DUMMY_ELEM_POS];

    FillClassicList (created_list, DUMMY_ELEM_POS);

    LIST_VERIFY (created_list);

    return LIST_STATUS_OK;
}

enum ListStatus ClassicListDtor (ClassicList *list_for_destruct) {

    LIST_VERIFY (list_for_destruct);

    ClassicListStructArrayDtor (list_for_destruct);

    memset (list_for_destruct, 0, sizeof (ClassicList));

    free (list_for_destruct);
    list_for_destruct = NULL;

    return LIST_STATUS_OK;
}

ClassicListMainItems *ClassicListStructArrayCtor (const ClassicList *const list_for_create_arrs) {

    assert (list_for_create_arrs);

    ClassicListMainItems *main_items_ptr = (ClassicListMainItems *) calloc (list_for_create_arrs -> capacity,
                                                                            sizeof (ClassicListMainItems));

    assert (main_items_ptr);

    return main_items_ptr;
}

enum ListStatus ClassicListStructArrayDtor (ClassicList *list_for_arr_destruct) {

    LIST_VERIFY (list_for_arr_destruct);

    ClassicListStructArrayClear (list_for_arr_destruct);

    free (list_for_arr_destruct -> mainItems);

    list_for_arr_destruct -> mainItems = NULL;

    return LIST_STATUS_OK;
}

enum ListStatus FillClassicList (ClassicList *const list_for_fill, size_t start_pos) {

    assert (list_for_fill);

    for (size_t i = list_for_fill -> capacity - 1; i > start_pos; i--)
        ClassicListFreeElem (list_for_fill, &(list_for_fill -> mainItems)[i]);

    ON_DEBUG (CLASSIC_LIST_DUMP (list_for_fill));

    return LIST_STATUS_OK;
}

enum ListStatus ClassicListConnectNeighbourElems (ClassicList *const list_for_connect_elems,
                                                  ClassicListMainItems *first_elem_ptr,
                                                  ClassicListMainItems *second_elem_ptr) {

    assert (list_for_connect_elems);

    first_elem_ptr -> next  = second_elem_ptr;

    second_elem_ptr -> prev = first_elem_ptr;

    ON_DEBUG (CLASSIC_LIST_DUMP (list_for_connect_elems));

    return LIST_STATUS_OK;
}

enum ListStatus ClassicListCreateDummyNode (ClassicList *const list_for_create_dummy_node) {

    assert (list_for_create_dummy_node);

    (list_for_create_dummy_node -> mainItems)[DUMMY_ELEM_POS].value = POISON;

    ClassicListMainItems *ptr_to_dummy = &(list_for_create_dummy_node -> mainItems)[DUMMY_ELEM_POS];

    ClassicListConnectNeighbourElems (list_for_create_dummy_node, ptr_to_dummy, ptr_to_dummy);    //TODO fix position of dummy node

    ON_DEBUG (CLASSIC_LIST_DUMP (list_for_create_dummy_node));

    return LIST_STATUS_OK;
}

enum ListStatus ClassicListStructArrayClear (ClassicList *const list_for_clear_struct) {

    LIST_VERIFY (list_for_clear_struct);

    size_t size_of_main_items = sizeof (ClassicListMainItems) * (list_for_clear_struct -> capacity);

    memset (list_for_clear_struct -> mainItems, 0, size_of_main_items);

    ON_DEBUG (CLASSIC_LIST_DUMP (list_for_clear_struct));

    return LIST_STATUS_OK;
}

unsigned int ClassicListVerify (const ClassicList *const list_to_verify) {

    unsigned int list_errors = 0;

    if (list_to_verify == NULL) {

        list_errors |= INVALID_LIST_POINTER;

        LogPrintListError ("INVALID_LIST_POINTER");
    }

    if ((list_to_verify -> list_size) > (list_to_verify -> capacity)) {

        list_errors |= INVALID_LIST_SIZE;

        LogPrintListError ("INVALID_LIST_SIZE");
    }

    if ((list_to_verify -> mainItems)[DUMMY_ELEM_POS].value != POISON) {

        list_errors |= DAMAGED_LIST_DUMMY_NODE;

        LogPrintListError ("DAMAGED_LIST_DUMMY_NODE");
    }

    ClassicListMainItems *first_elem_ptr = NULL;
    ClassicListMainItems *first_elem_next_ptr = NULL, *second_elem_prev_ptr = NULL;

    for (int64_t i = 0; i < (list_to_verify -> capacity); i++) {

        first_elem_ptr = &(list_to_verify -> mainItems)[i];

        first_elem_next_ptr = first_elem_ptr -> next;

        // if next is NULL and value is POISON then cell is free

        if (first_elem_next_ptr == NULL && first_elem_ptr -> value == POISON)
            continue;

        second_elem_prev_ptr = first_elem_next_ptr -> prev;

        if (first_elem_ptr != second_elem_prev_ptr) {

            list_errors |= DAMAGED_CONNECTION_LIST;

            LogPrintListError ("DAMAGED_CONNECTION_LIST");

            break;
        }
    }

    ClassicListMainItems *free_elem_ptr = ((list_to_verify -> controlItems).free);

    if (free_elem_ptr -> value != POISON) {

        list_errors |= DAMAGED_LIST_FREE_ELEM;

        LogPrintListError ("DAMAGED_LIST_FREE_ELEM");
    }

    return list_errors;
}

enum ListStatus ClassicListAddElemAfter (ClassicList *const list_for_add_elem,
                                         ClassicListMainItems *ptr_to_elem_in_list,
                                         ClassicListMainItems **inserted_ptr,
                                         const ClassicListElem_t add_value) {

    assert (ptr_to_elem_in_list);
    assert (inserted_ptr);

    LIST_VERIFY (list_for_add_elem);

    if ((list_for_add_elem -> controlItems).free == &(list_for_add_elem -> mainItems)[DUMMY_ELEM_POS])
        if (ClassicListIncreaseCapacity (list_for_add_elem) == LIST_STATUS_FAIL)
            return LIST_STATUS_FAIL;

    ClassicListMainItems *free_elem_ptr         = (list_for_add_elem -> controlItems).free;
    ClassicListMainItems *elem_in_list_next_ptr = ptr_to_elem_in_list -> next;

    free_elem_ptr -> value = add_value;

    (list_for_add_elem -> controlItems).free = free_elem_ptr -> prev;

    ClassicListConnectNeighbourElems (list_for_add_elem, ptr_to_elem_in_list, free_elem_ptr);
    ClassicListConnectNeighbourElems (list_for_add_elem, free_elem_ptr, elem_in_list_next_ptr);

    *inserted_ptr = free_elem_ptr;

    (list_for_add_elem -> list_size) += 1;

    ON_DEBUG (CLASSIC_LIST_DUMP (list_for_add_elem));

    return LIST_STATUS_OK;
}

enum ListStatus ClassicListRemoveElem (ClassicList *const list_for_remove_elem,
                                       ClassicListMainItems *elem_ptr_for_remove) {

    LIST_VERIFY (list_for_remove_elem);

    ClassicListMainItems *prev_of_ptr_remove = elem_ptr_for_remove -> prev;
    ClassicListMainItems *next_of_ptr_remove = elem_ptr_for_remove -> next;

    if (ClassicListFreeElem (list_for_remove_elem, elem_ptr_for_remove) == LIST_STATUS_FAIL)
        return LIST_STATUS_FAIL;

    ClassicListConnectNeighbourElems (list_for_remove_elem, prev_of_ptr_remove, next_of_ptr_remove);

    (list_for_remove_elem -> list_size) -= 1;

    ON_DEBUG (CLASSIC_LIST_DUMP (list_for_remove_elem));

    return LIST_STATUS_OK;
}

enum ListStatus ClassicListFreeElem (ClassicList *const list_for_free_elem,
                                     ClassicListMainItems *ptr_to_free_elem) {

    assert (list_for_free_elem);

    if (ptr_to_free_elem == &(list_for_free_elem -> mainItems)[DUMMY_ELEM_POS])
        return LIST_STATUS_FAIL;

    ptr_to_free_elem -> value = POISON;
    ptr_to_free_elem -> next  = NULL;
    ptr_to_free_elem -> prev  = (list_for_free_elem -> controlItems).free;

    (list_for_free_elem -> controlItems).free = ptr_to_free_elem;

    ON_DEBUG (CLASSIC_LIST_DUMP (list_for_free_elem));

    return LIST_STATUS_OK;
}

enum ListStatus ClassicListGetElem (const ClassicList *const list_for_get_elem,
                                    ClassicListMainItems *ptr_to_elem_in_list,
                                    ClassicListElem_t *ret_value) {

    assert (ret_value);

    LIST_VERIFY (list_for_get_elem);

    *ret_value = ptr_to_elem_in_list -> value;

    ON_DEBUG (CLASSIC_LIST_DUMP (list_for_get_elem));

    return LIST_STATUS_OK;
}

enum ListStatus ClassicListIncreaseCapacity (ClassicList *const list_for_increase_cap) {

    LIST_VERIFY (list_for_increase_cap);

    size_t prev_capacity = (list_for_increase_cap -> capacity);
    size_t new_capacity  = prev_capacity * 2;

    size_t size_of_all_elems_in_bytes = sizeof (ClassicListMainItems) * new_capacity;
    size_t size_of_new_elems_in_bytes = sizeof (ClassicListMainItems) * (new_capacity - prev_capacity);

    (list_for_increase_cap -> capacity) = new_capacity;

    ClassicListMainItems **ptr_to_classic_list_elems = &(list_for_increase_cap -> mainItems);

    *ptr_to_classic_list_elems = (ClassicListMainItems *) realloc (*ptr_to_classic_list_elems,
                                                                   size_of_all_elems_in_bytes);

    if (ptr_to_classic_list_elems == NULL)
        return LIST_STATUS_FAIL;

    memset (*ptr_to_classic_list_elems + prev_capacity, 0, size_of_new_elems_in_bytes);

    ClassicListMainItems *ptr_to_elem_in_list = NULL;

    for (size_t i = new_capacity - 1; i >= prev_capacity; i--) {

        ptr_to_elem_in_list = &(list_for_increase_cap -> mainItems)[i];
        ClassicListFreeElem (list_for_increase_cap, ptr_to_elem_in_list);
    }

    ON_DEBUG (CLASSIC_LIST_DUMP (list_for_increase_cap));

    return LIST_STATUS_OK;
}
