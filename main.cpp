#include <stdio.h>
#include <assert.h>

#include "fast_list_func.h"
#include "fast_list_log.h"

int main (const int argc, const char *argv[]) {

    int list_capacity = 5;

    FastList lst = {};
fprintf (stderr,"lol");
    FastListCtor (&lst, list_capacity);
fprintf (stderr,"lol1");

    FAST_LIST_DUMP (&lst);
fprintf (stderr,"lol2");

    size_t test_pos = DUMMY_ELEM_POS;

    FastListAddElemAfter (&lst, test_pos, &test_pos, 10);


    FastListDtor (&lst);
fprintf (stderr,"lol3");

    return 0;
}
