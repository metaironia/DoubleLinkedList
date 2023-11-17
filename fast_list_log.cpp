#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "fast_list_log.h"
#include "fast_list_func.h"


FILE *LOG_FILE = CreateLogFile ("list_log.txt");

FILE *CreateLogFile (const char *log_file_name) {

    assert (log_file_name);

    FILE *log_file_ptr = fopen (log_file_name, "w");

    assert (log_file_ptr);

    atexit (CloseLogFile);
    return log_file_ptr;
}

void CloseLogFile (void) {

    if (LOG_FILE)
        fclose (LOG_FILE);

    LOG_FILE = NULL;
}

enum ListStatus FastListDump (const FastList *list_for_dump, const char *file_called,
                              const char *func_called,       const int line_called,
                              const char *list_name) {

    LOG_PRINT (LOG_FILE, "List[0x%p] \"%s\" from %s(%d) %s() \n", list_for_dump, list_name,
                          file_called, line_called, func_called);

    LOG_PRINT (LOG_FILE, "free = %Iu\n", (list_for_dump -> controlItems).free);

    LOG_PRINT (LOG_FILE, "Elements:\n");

    PrintFastListElem (list_for_dump);

    return LIST_STATUS_OK;
}

enum ListStatus PrintFastListElem (const FastList *list_for_print) {

    assert (list_for_print);

    for (size_t i = 0; i < (list_for_print -> capacity); i++) {

        LOG_PRINT (LOG_FILE, "Element %Iu: \n",              i);
        LOG_PRINT (LOG_FILE, "value = " LIST_EL_FORMAT "\n", (list_for_print -> mainItems)[i].value);
        LOG_PRINT (LOG_FILE, "next = %d\n",                  (list_for_print -> mainItems)[i].next);
        LOG_PRINT (LOG_FILE, "prev = %d\n\n",                (list_for_print -> mainItems)[i].prev);
    }

    return LIST_STATUS_OK;
}

enum ListStatus LogPrintListError (const char *error_text) {

    assert (error_text);

    LOG_PRINT (LOG_FILE, "An error %s occurred. \n", error_text);

    return LIST_STATUS_OK;
}
