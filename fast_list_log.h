#ifndef LIST_LOG_H
#define LIST_LOG_H


#include <stdio.h>

#include "fast_list_func.h"


#define LIST_NAME(list)           #list

#define FAST_LIST_DUMP(list)      FastListDump (list, __FILE__, __func__, __LINE__, LIST_NAME (list))

#define LOG_PRINT(LOG_FILE, ...)  do {                                \
                                      fprintf(LOG_FILE, __VA_ARGS__); \
                                      fflush (LOG_FILE);              \
                                                                      \
                                  } while (0)

#define LIST_EL_FORMAT            "%d"

FILE *CreateLogFile (const char *log_file_name);

void CloseLogFile (void);

enum ListStatus FastListDump (const FastList *list_for_dump, const char *file_called,
                              const char *func_called,       const int line_called,
                              const char *list_name);

enum ListStatus PrintFastListElem (const FastList *list_for_print);

enum ListStatus LogPrintListError (const char *error_text);

#endif
