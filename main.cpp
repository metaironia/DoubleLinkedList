#include <stdio.h>
#include <assert.h>

//#include "fast_list_func.h"
//#include "fast_list_log.h"
#include "classic_list_func.h"
#include "classic_list_log.h"

int main (const int argc, const char *argv[]) {

    int list_capacity = 5;

//    FastList lst = {};
//fprintf (stderr,"lol");
//    FastListCtor (&lst, list_capacity);
//fprintf (stderr,"lol1");
//
//    FAST_LIST_DUMP (&lst);
//fprintf (stderr,"lol2");
//
//    size_t test_pos = DUMMY_ELEM_POS;
//
//    FastListAddElemAfter (&lst, test_pos, &test_pos, 10);
//    FastListAddElemAfter (&lst, test_pos, &test_pos, 13);
//    FastListAddElemAfter (&lst, test_pos, &test_pos, 20);
//    FastListAddElemAfter (&lst, test_pos, &test_pos, 167);
//
//    FastListRemoveElem (&lst, 3);
//
//    FastListGraphDump (&lst);
//
//    FastListDtor (&lst);
//fprintf (stderr,"lol3");

    ClassicList lst = {};

    ClassicListCtor (&lst, list_capacity);

    CLASSIC_LIST_DUMP (&lst);

    ClassicListMainItems *test_pos = &(lst.mainItems)[DUMMY_ELEM_POS];

    ClassicListAddElemAfter (&lst, test_pos, &test_pos, 10);

    ClassicListGraphDump (&lst);

    return 0;
}
