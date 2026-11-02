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

    size_t         num_of_elems ;
    elem_struct_t* first_elem_p ;
    size_t         head_idx     ;//начало
    size_t         tail_idx     ;//конец
    size_t         free_elem_idx;
};

const size_t DEFAULT_LIST_SIZE = 5     ;
const size_t POISON            = 675242;

enum return_codes{

    RET_CORRECT = 0,
    RET_ERROR   = 1
};

return_codes createList      (list_t** const         list_p_p, size_t list_size);
void         printList       (list_t   const * const list_p);
return_codes addElemInList   (list_t*  const         list_p, size_t idx_prev, int value);
void         deleteElemInList(list_t*  const         list_p, size_t idx_del);
return_codes checkAndResizeUp(list_t*  const         list_p);

#endif
