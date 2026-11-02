#include "..\Headers\List.h"

int main(){

    list_t* list_p = NULL;
    createList(&list_p, DEFAULT_LIST_SIZE);

    printList(list_p);
    addElemInList(list_p, list_p->tail_idx, 52);
    printList(list_p);
    addElemInList(list_p, list_p->tail_idx, 52);
    printList(list_p);
    addElemInList(list_p, list_p->tail_idx, 67);
    printList(list_p);
    addElemInList(list_p, 1, 98);
    printList(list_p);
    deleteElemInList(list_p, 3);
    printList(list_p);

    addElemInList(list_p, list_p->tail_idx, 67);
    printList(list_p);
    addElemInList(list_p, list_p->tail_idx, 67);
    printList(list_p);
    addElemInList(list_p, list_p->tail_idx, 67);
    printList(list_p);
    addElemInList(list_p, list_p->tail_idx, 67);
    printList(list_p);
    addElemInList(list_p, list_p->tail_idx, 67);
    printList(list_p);
    addElemInList(list_p, list_p->tail_idx, 67);
    printList(list_p);
    addElemInList(list_p, list_p->tail_idx, 67);
    printList(list_p);
    addElemInList(list_p, list_p->tail_idx, 67);
    printList(list_p);
}

return_codes createList(list_t** const list_p_p, size_t list_size){

    assert(list_p_p);
    assert(*list_p_p == NULL);
    assert(list_size != 0);

    list_t* list_p = NULL;

    list_p = (list_t*) calloc(1, sizeof(list_t));

    if(list_p == NULL){

        printf(RED "ERROR: there is no way to allocate memory for list struct\n" DEFAULT);
        return RET_ERROR;
    }

    list_p->num_of_elems = list_size;
    list_p->first_elem_p = (elem_struct_t*) calloc(list_size + 1, sizeof(elem_struct_t));

    if(list_p->first_elem_p == NULL){

        printf(RED "ERROR: there is no way to allocate memory for list elements\n" DEFAULT);
        return RET_ERROR;
    }

    list_p->head_idx      = 1;
    list_p->tail_idx      = 0;
    list_p->free_elem_idx = 1;

    list_p->first_elem_p[0].idx_prev   = 0;
    list_p->first_elem_p[0].value = POISON;
    list_p->first_elem_p[0].idx_next   = 0;

    for(size_t i = 1; i <= list_size; i++){

        list_p->first_elem_p[i].idx_prev = i - 1;

        list_p->first_elem_p[i].idx_next = i + 1;

        list_p->first_elem_p[i].value = POISON;
    }

    list_p->first_elem_p[list_size].idx_next = 0;

    *list_p_p = list_p;

    return RET_CORRECT;
}

void printList(list_t const * const list_p){

    assert(list_p);

    printf("\nPrint list with "  GREEN  "%llu" DEFAULT " size, head idx: " YELLOW "%llu"   DEFAULT
                  ", tail idx: " VIOLET "%llu" DEFAULT ", free elem idx: " YELLOW "%llu\n" DEFAULT,
           list_p->num_of_elems, list_p->head_idx, list_p->tail_idx, list_p->free_elem_idx);

    printf("N    PREV_IDX     ELEM_VALUE   NEXT_IDX\n\n");


    for(size_t i = 0; i <= list_p->num_of_elems; i++){

        printf("%-4llu %-4llu         %-10d   %-4llu\n",
               i                                       ,
               list_p->first_elem_p[i].idx_prev        ,
               list_p->first_elem_p[i].value           ,
               list_p->first_elem_p[i].idx_next        );
    }
}

return_codes addElemInList(list_t* const list_p, size_t idx_prev, int value){

    assert(list_p);

    if(checkAndResizeUp(list_p) == RET_ERROR){

        return RET_ERROR;
    }

    size_t new_elem_idx = list_p->free_elem_idx;

    //Корректируем список свободных элементов
    list_p->free_elem_idx = list_p->first_elem_p[new_elem_idx].idx_next;
    list_p->first_elem_p[list_p->free_elem_idx].idx_prev = 0;

    //Заполняем наш новый элемент
    list_p->first_elem_p[new_elem_idx].idx_prev = idx_prev;
    list_p->first_elem_p[new_elem_idx].value    = value;
    list_p->first_elem_p[new_elem_idx].idx_next = list_p->first_elem_p[idx_prev].idx_next;

    //Исправляем новых соседей
    list_p->first_elem_p[idx_prev].idx_next = new_elem_idx;
    list_p->first_elem_p[list_p->first_elem_p[new_elem_idx].idx_next].idx_prev = new_elem_idx;

    //Если это новый конец
    if(idx_prev == list_p->tail_idx){

        list_p->tail_idx = new_elem_idx;

        list_p->first_elem_p[new_elem_idx].idx_next = 0;
    }

    //Если это новое начало
    if(idx_prev == 0){

        list_p->head_idx = new_elem_idx;

        list_p->first_elem_p[new_elem_idx].idx_prev = 0;
    }

    //Возвращаем нулевой элемент
    list_p->first_elem_p[0].idx_prev = 0;
    list_p->first_elem_p[0].idx_next = 0;

    return RET_CORRECT;
}

void deleteElemInList(list_t* const list_p, size_t idx_del){

    assert(list_p);
    assert(idx_del != 0);


    //Исправляем старых соседей
    size_t idx_old_prev = list_p->first_elem_p[idx_del].idx_prev;
    size_t idx_old_next = list_p->first_elem_p[idx_del].idx_next;

    list_p->first_elem_p[idx_old_prev].idx_next = idx_old_next;
    list_p->first_elem_p[idx_old_next].idx_prev = idx_old_prev;

    //Заполняем удаляемый элемент
    list_p->first_elem_p[idx_del].idx_prev = 0;
    list_p->first_elem_p[idx_del].value    = POISON;
    list_p->first_elem_p[idx_del].idx_next = list_p->free_elem_idx;

    //Корректируем список свободных элементов
    list_p->first_elem_p[list_p->free_elem_idx].idx_prev = idx_del;
    list_p->free_elem_idx = idx_del;

    //Если это был конец
    if(idx_del == list_p->tail_idx){

        list_p->tail_idx = idx_old_prev;
    }

    //Если было начало
    if(idx_del == list_p->head_idx){

        list_p->head_idx = idx_old_next;
    }

    //Возвращаем нулевой элемент
    list_p->first_elem_p[0].idx_prev = 0;
    list_p->first_elem_p[0].idx_next = 0;
}

return_codes checkAndResizeUp(list_t* const list_p){

    assert(list_p);

    if(list_p->first_elem_p[list_p->free_elem_idx].idx_next != 0){

        return RET_CORRECT;
    }

    printf("We need to resize up\n");
    printList(list_p);

    elem_struct_t* new_memory_block_p = (elem_struct_t*) realloc(list_p->first_elem_p, sizeof(elem_struct_t) * (list_p->num_of_elems * 2 + 1));

    if(new_memory_block_p == NULL){

        printf(RED "ERROR: no way to reallocate memory for list to add a new element to it\n" DEFAULT);

        return RET_ERROR;
    }

    list_p->first_elem_p = new_memory_block_p;

    printf("Allocated new memory block\n");
    printList(list_p);

    list_p->first_elem_p[list_p->free_elem_idx].idx_next = list_p->num_of_elems + 1;

    printf("Update free elem idx\n");
    printList(list_p);

    for(size_t i = list_p->num_of_elems + 1; i <= list_p->num_of_elems * 2; i++){

        list_p->first_elem_p[i].idx_prev = i - 1;

        list_p->first_elem_p[i].idx_next = i + 1;

        list_p->first_elem_p[i].value = POISON;
    }

    list_p->first_elem_p[list_p->num_of_elems * 2].idx_next = 0;

    list_p->num_of_elems *= 2;


    printf("Update all new memory\n");
    printList(list_p);

    return RET_CORRECT;
}
