#ifndef LIST_FUNC_H
#define LIST_FUNC_H


#include <stdint.h>

#define LIST_VERIFY(list)   do {                                     \
                                if (ClassicListVerify (list) != 0) { \
                                                                     \
                                    CLASSIC_LIST_DUMP (list);        \
                                    return LIST_STATUS_FAIL;         \
                                }                                    \
                            } while (0)

#define DUMMY_ELEM_POS      0

#define IS_ELEM_TYPE_FLOAT  0

#define DEBUG


#if IS_ELEM_TYPE_FLOAT

    #define POISON          NAN
#else

    #define POISON          0x50DEAD
#endif


#ifdef DEBUG

    #define ON_DEBUG(...)   __VA_ARGS__
#else

    #define ON_DEBUG(...)
#endif


typedef int ClassicListElem_t;

struct ClassicListMainItems {

    ClassicListElem_t value;

    ClassicListMainItems *prev;
    ClassicListMainItems *next;
};

struct ClassicListControlItems {

    ClassicListMainItems *free;
};

struct ClassicList {

    ClassicListControlItems controlItems;

    ClassicListMainItems *mainItems;

    size_t capacity;
    size_t list_size;
};

enum ListStatus {

    LIST_STATUS_OK,
    LIST_STATUS_FAIL
};

enum ListErrors {

    INVALID_LIST_POINTER,
    INVALID_LIST_SIZE,
    DAMAGED_LIST_DUMMY_NODE,
    INVALID_LIST_ELEM_NEXT,
    DAMAGED_CONNECTION_LIST,
    DAMAGED_LIST_FREE_ELEM
};

enum ListStatus ClassicListCtor (ClassicList *created_list, const size_t list_capacity);

enum ListStatus ClassicListDtor (ClassicList *list_for_destruct);

ClassicListMainItems *ClassicListStructArrayCtor (const ClassicList *const list_for_create_arrs);

enum ListStatus ClassicListStructArrayDtor (ClassicList *list_for_arrs_destruct);

enum ListStatus ClassicListStructArrayClear (ClassicList *const list_for_clear_struct);

enum ListStatus FillClassicList (ClassicList *const list_for_fill, size_t start_pos);

enum ListStatus ClassicListConnectNeighbourElems (ClassicList *const list_for_connect_elems,
                                                  ClassicListMainItems *first_elem_ptr,
                                                  ClassicListMainItems *second_elem_ptr);

enum ListStatus ClassicListCreateDummyNode (ClassicList *const list_for_create_dummy_node);

unsigned int ClassicListVerify (const ClassicList *const list_to_verify);

enum ListStatus ClassicListAddElemAfter (ClassicList *const list_for_add_elem,
                                         ClassicListMainItems *ptr_to_elem_in_list,
                                         ClassicListMainItems **inserted_ptr,
                                         const ClassicListElem_t add_value);

enum ListStatus ClassicListRemoveElem (ClassicList *const list_for_remove_elem,
                                       ClassicListMainItems *elem_ptr_for_remove);

enum ListStatus ClassicListFreeElem (ClassicList *const list_for_free_elem,
                                     ClassicListMainItems *ptr_to_free_elem);

enum ListStatus ClassicListGetElem (const ClassicList *const list_for_get_elem,
                                    ClassicListMainItems *ptr_to_elem_in_list,
                                    ClassicListElem_t *ret_value);

enum ListStatus ClassicListIncreaseCapacity (ClassicList *const list_for_increase_cap);

#endif
