#ifndef LIST_LOG_H
#define LIST_LOG_H


#include <stdio.h>

#include "classic_list_func.h"


#define LIST_NAME(list)           #list

#define CLASSIC_LIST_DUMP(list)   ClassicListDump (list, __FILE__, __func__, __LINE__, LIST_NAME (list))

#define LOG_PRINT(LOG_FILE, ...)  do {                                \
                                      fprintf(LOG_FILE, __VA_ARGS__); \
                                      fflush (LOG_FILE);              \
                                                                      \
                                  } while (0)

#define LIST_EL_FORMAT            "%d"

FILE *CreateLogFile (const char *log_file_name);

void CloseLogFile (void);

enum ListStatus ClassicListDump (const ClassicList *list_for_dump, const char *file_called,
                                 const char *func_called,          const int line_called,
                                 const char *list_name);

enum ListStatus PrintClassicListElem (const ClassicList *list_for_print);

enum ListStatus LogPrintListError (const char *error_text);

//---------------------------------- Graph dump -------------------------------------------------------------

enum ListStatus ClassicListGraphDump (const ClassicList *list_for_graph_dump);

enum ListStatus ClassicListDotFileBegin (FILE *dot_file);

enum ListStatus ClassicListDotFileEnd (FILE *dot_file);

enum ListStatus ClassicListDotFileInfo (FILE *dot_file, const ClassicList *list_for_info);

enum ListStatus ClassicListDotFileColorElem (FILE *dot_file_for_color,
                                             const ClassicList *list_for_choose_color,
                                             const size_t index);

enum ListStatus ClassicListDotFileOutputElems (FILE *dot_file, const ClassicList *list_for_output_elems);

enum ListStatus ClassicListDotFileDrawArrows (FILE *dot_file_for_arrows,
                                              const ClassicList *list_for_draw_arrows);

enum ListStatus ClassicListDotFileCenterElems (FILE * dot_file_for_center,
                                               const ClassicList *list_for_center_elems);

enum ListStatus ClassicListDotFileColorDummy (FILE *dot_file, const ClassicList *list_for_output_dummy);


#endif
