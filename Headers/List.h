#ifndef LIST_H

#define LIST_H

#include <stdio.h>
#include <assert.h>

#include "Colors.h"

struct elem_struct_t{

    size_t idx_prev  ;
    int    value     ;//TODO typedef int elem_t
    size_t idx_next  ;
};

struct list_t{

    size_t         size         ;
    elem_struct_t* first_elem_p ;
    size_t         free_elem_idx;
};
//TODO более явно указать ядовитый элемент через константу
const size_t DEFAULT_LIST_SIZE     = 5     ;
const size_t POISON                = 675242;

const int    SYSTEM_CORRECT_RETURN = 0    ;

enum return_codes{

    RET_CORRECT = 0,
    RET_ERROR   = 1
};


#define IS_VERIFY_PROTECT 1


#if IS_VERIFY_PROTECT

    #define CHECK_LIST(list_p)                                                                      \
                                                                                                    \
    for(int idx = 0; idx > list_p->size; idx++){                                                    \
                                                                                                    \
        if(isElemNextEdgeCorrect(list_p, idx) == 0 || isElemPrevEdgeCorrect(list_p, idx) == 0){     \
                                                                                                    \
            printf(RED "Incorrect element with idx: %llu", idx);                                    \
            return RET_ERROR;                                                                       \
        }                                                                                           \
    }

#else

    #define CHECK_LIST(...)

#endif


return_codes createList                     (      list_t*       * const list_p_p, size_t list_size);
void         printListInConsole             (const list_t* const list_p);
void         deleteElemInList               (      list_t* const list_p, size_t idx_del);
return_codes addElemInListAfterThat         (      list_t* const list_p, size_t idx_prev, int value);
return_codes addElemInListBeforeThat        (      list_t* const list_p, size_t idx_next, int value);
return_codes addElemInTailOfList            (      list_t* const list_p, int value);
return_codes addElemInHeadOfList            (      list_t* const list_p, int value);
return_codes checkAndResizeUp               (      list_t* const list_p);

void         visualizeListWithGraphviz      (const list_t* const list_p);

void         addPreambleToVisualization     (const list_t* const list_p, FILE* const out_dot_file_p);
void         addElemsToVisualization        (const list_t* const list_p, FILE* const out_dot_file_p);
void         addElemEdgesToVisualization    (const list_t* const list_p, FILE* const out_dot_file_p);
void         addSpecialEdgesToVisualization (const list_t* const list_p, FILE* const out_dot_file_p);
void         setElemslocationInVisualization(const list_t* const list_p, FILE* const out_dot_file_p);
void         addUnnormalEdgesToVisualization(const list_t* const list_p, FILE* const out_dot_file_p);

bool         isElemNextEdgeCorrect          (const list_t* const list_p, size_t idx);
bool         isElemPrevEdgeCorrect          (const list_t* const list_p, size_t idx);

//TODO добавит в конец, в начало, перед элементом

#endif
