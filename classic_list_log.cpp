#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "classic_list_log.h"
#include "classic_list_func.h"


FILE *LOG_FILE = CreateLogFile ("classic_list_log.txt");

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

enum ListStatus ClassicListDump (const ClassicList *list_for_dump, const char *file_called,
                                 const char *func_called,          const int line_called,
                                 const char *list_name) {

    assert (file_called);
    assert (func_called);
    assert (list_name);

    LOG_PRINT (LOG_FILE, "List[0x%p] \"%s\" from %s(%d) %s() \n", list_for_dump, list_name,
                          file_called, line_called, func_called);

    LOG_PRINT (LOG_FILE, "free = 0x%p\n",      (list_for_dump -> controlItems).free);
    LOG_PRINT (LOG_FILE, "size = %Iu\n",     (list_for_dump -> list_size));
    LOG_PRINT (LOG_FILE, "capacity = %Iu\n", (list_for_dump -> capacity));

    LOG_PRINT (LOG_FILE, "Elements:\n");

    PrintClassicListElem (list_for_dump);

    return LIST_STATUS_OK;
}

enum ListStatus PrintClassicListElem (const ClassicList *list_for_print) {

    assert (list_for_print);

    for (size_t i = 0; i < (list_for_print -> capacity); i++) {

        LOG_PRINT (LOG_FILE, "Element %Iu: \n",              i);
        LOG_PRINT (LOG_FILE, "value = " LIST_EL_FORMAT "\n", (list_for_print -> mainItems)[i].value);
        LOG_PRINT (LOG_FILE, "next = 0x%p\n",                  (list_for_print -> mainItems)[i].next);
        LOG_PRINT (LOG_FILE, "prev = 0x%p\n\n",                (list_for_print -> mainItems)[i].prev);
    }

    return LIST_STATUS_OK;
}

enum ListStatus LogPrintListError (const char *error_text) {

    assert (error_text);

    LOG_PRINT (LOG_FILE, "An error %s occurred. \n", error_text);

    return LIST_STATUS_OK;
}

enum ListStatus ClassicListGraphDump (const ClassicList *list_for_graph_dump) {

    LIST_VERIFY (list_for_graph_dump);

    FILE *graph_dump_file = fopen ("graph_dump_classic_list.dot", "w");

    if (graph_dump_file == NULL)
        return LIST_STATUS_FAIL;

    ClassicListDotFileBegin (graph_dump_file);

    ClassicListDotFileInfo (graph_dump_file, list_for_graph_dump);
    ClassicListDotFileOutputElems (graph_dump_file, list_for_graph_dump);

    ClassicListDotFileCenterElems (graph_dump_file, list_for_graph_dump);
    ClassicListDotFileDrawArrows (graph_dump_file, list_for_graph_dump);

    ClassicListDotFileEnd (graph_dump_file);


    fclose (graph_dump_file);
    graph_dump_file = NULL;

    return LIST_STATUS_OK;
}

enum ListStatus ClassicListDotFileBegin (FILE *dot_file) {

    assert (dot_file);

    fprintf(dot_file, "digraph G{\n"
                      "rankdir = LR;\n"
                      "graph [bgcolor = white];\n");

    return LIST_STATUS_OK;
}

enum ListStatus ClassicListDotFileEnd (FILE *dot_file) {

    assert (dot_file);

    fprintf(dot_file, "\n}\n");

    return LIST_STATUS_OK;
}

enum ListStatus ClassicListDotFileInfo (FILE *dot_file, const ClassicList *list_for_info) {

    assert (dot_file);
    assert (list_for_info);

    fprintf(dot_file, "info [shape = record, style = filled, fillcolor = \"yellow\","
                      "label = \"FREE: 0x%p | SIZE: %Iu | CAPACITY: %Iu\","
                      "fontcolor = \"black\", fontsize = 22];\n",
                      (list_for_info -> controlItems).free, list_for_info -> list_size,
                      list_for_info -> capacity);

    return LIST_STATUS_OK;
}

enum ListStatus ClassicListDotFileColorDummy (FILE *dot_file, const ClassicList *list_for_output_dummy) {

    assert (dot_file);
    assert (list_for_output_dummy);

    fprintf(dot_file, "fillcolor = gray, color = black, ");

    return LIST_STATUS_OK;
}

enum ListStatus ClassicListDotFileColorElem (FILE *dot_file_for_color, const ClassicList *list_for_choose_color,
                                             const size_t index) {

    assert (dot_file_for_color);
    assert (list_for_choose_color);

    if ((list_for_choose_color -> mainItems)[index].value == POISON)
        fprintf (dot_file_for_color, "fillcolor = \"crimson\", color = black,");

    else
        fprintf (dot_file_for_color, "fillcolor = \"lightgreen\", color = darkgreen,");

    return LIST_STATUS_OK;
}


enum ListStatus ClassicListDotFileOutputElems (FILE *dot_file, const ClassicList *list_for_output_elems) {

    assert (dot_file);
    assert (list_for_output_elems);

    for (size_t i = 0; i < (list_for_output_elems -> capacity); i++) {

        fprintf (dot_file, "%x [shape=Mrecord, style=filled, ", &(list_for_output_elems -> mainItems)[i]);

        if (i != DUMMY_ELEM_POS)
            ClassicListDotFileColorElem (dot_file, list_for_output_elems, i);

        else
            ClassicListDotFileColorDummy (dot_file, list_for_output_elems);

        fprintf (dot_file, " label=\" ");

        if ((list_for_output_elems -> mainItems)[i].value == POISON) {

            fprintf(dot_file, "index: %d | value: POISON| next: 0x%p| prev: 0x%p\" ];\n",
                              i,
                              (list_for_output_elems -> mainItems)[i].next,
                              (list_for_output_elems -> mainItems)[i].prev);
        }

        else {

            fprintf(dot_file, "index: %d | value: " LIST_EL_FORMAT "| next: 0x%p| prev: 0x%p\" ];\n",
                              i,
                              (list_for_output_elems -> mainItems)[i].value,
                              (list_for_output_elems -> mainItems)[i].next,
                              (list_for_output_elems -> mainItems)[i].prev);
        }
    }

    return LIST_STATUS_OK;
}

enum ListStatus ClassicListDotFileDrawArrows (FILE *dot_file_for_arrows,
                                              const ClassicList *list_for_draw_arrows) {

    assert (dot_file_for_arrows);
    assert (list_for_draw_arrows);

    for (size_t i = 1; i < (list_for_draw_arrows -> capacity); i++) {

        if ((list_for_draw_arrows -> mainItems)[i].next  == NULL &&
            (list_for_draw_arrows -> mainItems)[i].value == POISON)

            fprintf (dot_file_for_arrows, "%x -> %x [weight = 0, color = \"red\"];\n",
                                         &(list_for_draw_arrows -> mainItems)[i],
                                         (list_for_draw_arrows -> mainItems)[i].prev);

        else {

            fprintf (dot_file_for_arrows, "%x -> %x [weight = 0, color = \"blue\"];\n",
                                          &(list_for_draw_arrows -> mainItems)[i],
                                          (list_for_draw_arrows -> mainItems)[i].next);

            fprintf (dot_file_for_arrows, "%x -> %x [weight = 0, color = \"green\"];\n",
                                          &(list_for_draw_arrows -> mainItems)[i],
                                          (list_for_draw_arrows -> mainItems)[i].prev);
        }
    }

    return LIST_STATUS_OK;
}

enum ListStatus ClassicListDotFileCenterElems (FILE *dot_file_for_center,
                                               const ClassicList *list_for_center_elems) {

    assert (dot_file_for_center);
    assert (list_for_center_elems);

    fprintf (dot_file_for_center, "info -> %x [color = \"white\", style = invis];\n",
                                  &(list_for_center_elems -> mainItems)[DUMMY_ELEM_POS]);

    for (size_t i = 0; i < (list_for_center_elems -> capacity) - 1; i++) {

        fprintf (dot_file_for_center, "%x -> %x [color = \"white\", style = invis];\n",
                                      &(list_for_center_elems -> mainItems)[i],
                                      &(list_for_center_elems -> mainItems)[i + 1]);
    }

    return LIST_STATUS_OK;
}
