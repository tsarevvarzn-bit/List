#include "..\Headers\List.h"

int main(){

    list_t* list_p = NULL;
    createList(&list_p, DEFAULT_LIST_SIZE);

    printListInConsole(list_p);
    addElemInTailOfList(list_p, 52);
    printListInConsole(list_p);
    addElemInTailOfList(list_p, 52);
    printListInConsole(list_p);
    addElemInTailOfList(list_p, 67);
    printListInConsole(list_p);
    // addElemInListAfterThat(list_p, 1, 98);
    // printListInConsole(list_p);
    deleteElemInList(list_p, 3);
    printListInConsole(list_p);

    // addElemInTailOfList(list_p, 67);
    // printListInConsole(list_p);
    // addElemInTailOfList(list_p, 67);
    // printListInConsole(list_p);
    // addElemInTailOfList(list_p, 67);
    // printListInConsole(list_p);
    // addElemInTailOfList(list_p, 67);
    // printListInConsole(list_p);

    // list_p->first_elem_p[3].idx_next = 10;
    // deleteElemInList(list_p, 8);
    printListInConsole(list_p);


    visualizeListWithGraphviz(list_p, "huinia.svg");
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

    list_p->size = list_size;
    list_p->first_elem_p = (elem_struct_t*) calloc(list_size + 1, sizeof(elem_struct_t));

    if(list_p->first_elem_p == NULL){

        printf(RED "ERROR: there is no way to allocate memory for list elements\n" DEFAULT);
        return RET_ERROR;
    }

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

    CHECK_LIST(list_p);

    return RET_CORRECT;
}

void printListInConsole(list_t const * const list_p){

    assert(list_p);

    printf("\nPrint list with "  GREEN  "%llu" DEFAULT " size, head idx: " YELLOW "%llu"   DEFAULT
                  ", tail idx: " VIOLET "%llu" DEFAULT ", free elem idx: " YELLOW "%llu\n" DEFAULT,
           list_p->size, list_p->first_elem_p[0].idx_next, list_p->first_elem_p[0].idx_prev, list_p->free_elem_idx);

    printf("N    PREV_IDX     ELEM_VALUE   NEXT_IDX\n\n");

    for(size_t i = 0; i <= list_p->size; i++){

        printf("%-4llu %-4llu         %-10d   %-4llu\n",
               i                                       ,
               list_p->first_elem_p[i].idx_prev        ,
               list_p->first_elem_p[i].value           ,
               list_p->first_elem_p[i].idx_next        );
    }
}

void visualizeListWithGraphviz(const list_t* const list_p, const char * const name_of_file){

    assert(list_p);
    assert(name_of_file);

    FILE* out_dot_file_p = fopen("DataForVisualization.dot", "w");

    addPreambleToVisualization     (list_p, out_dot_file_p);

    addElemsToVisualization        (list_p, out_dot_file_p);

    addSpecialEdgesToVisualization (list_p, out_dot_file_p);

    addElemEdgesToVisualization    (list_p, out_dot_file_p);

    addUnnormalEdgesToVisualization(list_p, out_dot_file_p);

    setElemslocationInVisualization(list_p, out_dot_file_p);

    fclose(out_dot_file_p);


    char str_for_system_call[MAX_LEN_OF_STR_FOR_SYSTEM_CALL] = "";

    sprintf(str_for_system_call, ".\\Graphviz\\bin\\dot.exe -Tsvg DataForVisualization.dot -o %s", name_of_file);

    int result = system(str_for_system_call);

    if(result == SYSTEM_CORRECT_RETURN){

        printf(GREEN "Correct visualization! Look for a visualization file named " YELLOW "\"%s\"\n" DEFAULT, name_of_file);

    }else{

        printf(RED "ERROR! Incorrect return of\n"                                                                                    \
                   "int result = system(%s);\n"       \
                   "result: %d" DEFAULT, str_for_system_call, result);
    }
}

void addPreambleToVisualization(const list_t* const list_p, FILE* const out_dot_file_p){

    assert(list_p);
    assert(out_dot_file_p);


    fprintf(out_dot_file_p, "digraph {\n\
    \n\
    graph [\n\
        rankdir=TB,\n\
        splines=true,\n\
        overlap=true,\n\
        nodesep=0.5,\n\
        ranksep=0.8,\n\
        bgcolor=\"#312d4b\"\n\
    ]\n\
    \n\
    edge [\n\
        penwidth=2,\n\
        arrowhead=\"vee\"\n\
    ]\n\
    \n\
    node [shape=plain]\n");
}

void addElemsToVisualization(const list_t* const list_p, FILE* const out_dot_file_p){

    assert(list_p);
    assert(out_dot_file_p);

    for(size_t elem_idx = 0; elem_idx <= list_p->size; elem_idx++){

        fprintf(out_dot_file_p,
            "\t%llu [label=<\n"
            "        <TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLSPACING=\"0\" BGCOLOR=\"#9975bf\">\n"
            "            <TR><TD COLSPAN=\"3\">idx = %llu</TD></TR>\n"
            "            <TR>\n"
            "                <TD PORT=\"prev\">prev = %llu</TD>\n",
            elem_idx, elem_idx, list_p->first_elem_p[elem_idx].idx_prev);

        if (list_p->first_elem_p[elem_idx].value == POISON) {

            fprintf(out_dot_file_p,
            "                <TD BGCOLOR=\"yellow\">value = <B>POISON</B></TD>\n");

        } else {

            fprintf(out_dot_file_p,
            "                <TD>value = %d</TD>\n",
            list_p->first_elem_p[elem_idx].value);
        }

        fprintf(out_dot_file_p,
            "                <TD PORT=\"next\">next = %llu</TD>\n"
            "            </TR>\n"
            "        </TABLE>\n"
            "    >];\n",
            list_p->first_elem_p[elem_idx].idx_next);
    }
}

void addElemEdgesToVisualization(const list_t* const list_p, FILE* const out_dot_file_p){

    assert(list_p);
    assert(out_dot_file_p);

    fprintf(out_dot_file_p, "\tedge [\n\
        color=\"#0e6fbe\",\n\
        constraint=false,\n\
        penwidth=2\n\
    ];\n");

    for(size_t elem_idx = 1; elem_idx <= list_p->size; elem_idx++){

        if(isElemNextEdgeCorrect(list_p, elem_idx) != true){

            fprintf(out_dot_file_p, "\tedge [\n\
                color=\"#be0e0e\",\n\
                constraint=false,\n\
                penwidth=2\n\
                ]   ;\n");

        }else if(elem_idx == 0 || list_p->first_elem_p[elem_idx].idx_next == 0){

            fprintf(out_dot_file_p, "\tedge [\n\
                color=\"#b5be0e\",\n\
                constraint=false,\n\
                penwidth=2\n\
                ];\n");

        }

        fprintf(out_dot_file_p,
                "%llu:next:e -> %llu:prev:w;\n",
                elem_idx, list_p->first_elem_p[elem_idx].idx_next);

        fprintf(out_dot_file_p, "\tedge [\n\
                color=\"#0e6fbe\",\n\
                constraint=false,\n\
                penwidth=2\n\
                ];\n");
    }
}

void addSpecialEdgesToVisualization(const list_t* const list_p, FILE* const out_dot_file_p){

    assert(list_p);
    assert(out_dot_file_p);

    //  Рисуем head, tail и first free
    fprintf(out_dot_file_p, "    node [\n\
        shape=oval,\n\
        style=filled,\n\
        fillcolor=\"#75b1bf\"\n\
        weight=100\n\
    ]\n\
    edge [color=\"#54c282\"];\n\
    head -> %llu:s;\n\
    tail -> %llu:s;\n\
    \"first free\" -> %llu:s\n",
    list_p->first_elem_p[0].idx_next, list_p->first_elem_p[0].idx_prev, list_p->free_elem_idx);

    // Отдельно рисуем нулевой элемент

    fprintf(out_dot_file_p, "\tedge [\n\
        color=\"#b5be0e\",\n\
        constraint=false,\n\
        penwidth=2\n\
    ];\n");

    fprintf(out_dot_file_p,
            "0:next:e -> %llu:prev:w;\n",
            list_p->first_elem_p[0].idx_next);

    // Теперь табличку INFO

    fprintf(out_dot_file_p,
            "edge [\n\
                penwidth=2,\n\
                    arrowhead=\"vee\"\n\
            ]\n\
            \n\
             node [shape=plain]\n""INFO [label=<\n\
                <TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLSPACING=\"0\" BGCOLOR=\"#9975bf\">\n\
                    <TR><TD COLSPAN=\"4\">INFO</TD></TR>\n\
                    <TR>\n\
                        <TD>size = %llu</TD>\n\
                        <TD>head idx = %llu</TD>\n\
                        <TD>tail idx = %llu</TD>\n\
                        <TD>first free idx = %llu</TD>\n\
                    </TR>\n\
                </TABLE>\n\
            >];", list_p->size, list_p->first_elem_p[0].idx_next, list_p->first_elem_p[0].idx_prev, list_p->free_elem_idx);
}

void setElemslocationInVisualization(const list_t* const list_p, FILE* const out_dot_file_p){

    assert(list_p);
    assert(out_dot_file_p);

    // Задаем горизонтальный строгий порядок по индексам

    fprintf(out_dot_file_p, "\nedge [\n\
        style=invis,\n\
        constraint=true,\n\
        weight=100\n\
    ];\n");

    fprintf(out_dot_file_p, "1");

    for(size_t i = 2; i <= list_p->size; i++){

        fprintf(out_dot_file_p, " -> %llu", i);
    }
    fprintf(out_dot_file_p, "\n");

    //  Head, tail first free снизу

    fprintf(out_dot_file_p, "{rank=same; ");

    for(size_t i = 1; i <= list_p->size; i++){

        fprintf(out_dot_file_p, "%llu;", i);
    }

    fprintf(out_dot_file_p, "}\n");
    fprintf(out_dot_file_p, "{rank=sink; head; tail;\"first free\"}");
    fprintf(out_dot_file_p, "{rank=source; 0; INFO}");
    fprintf(out_dot_file_p, "\n}");

}

void addUnnormalEdgesToVisualization(const list_t* const list_p, FILE* const out_dot_file_p){

    assert(list_p);
    assert(out_dot_file_p);

    for(size_t elem_idx = 1; elem_idx <= list_p->size; elem_idx++){

        if(isElemPrevEdgeCorrect(list_p, elem_idx) != true){

            fprintf(out_dot_file_p, "\tedge [\n\
                color=\"#be0e0e\",\n\
                constraint=false,\n\
                penwidth=2\n\
                ]   ;\n");
            fprintf(out_dot_file_p,
                "%llu:next:e -> %llu:prev:w;\n",
                list_p->first_elem_p[elem_idx].idx_prev, elem_idx);

        }
    }
}
return_codes addElemInListAfterThat(list_t* const list_p, size_t idx_prev, int value){

    assert(list_p);

    CHECK_LIST(list_p);

    if(list_p->first_elem_p[idx_prev].value == POISON && idx_prev != 0){

        printf(RED "ERROR: incorrect addElemInListAfterThat with idx_prev = %llu - POISON element, not a part of list\n" DEFAULT, idx_prev);
        return RET_ERROR;
    }

    if(checkAndResizeUp(list_p) == RET_ERROR){

        return RET_ERROR;
    }

    size_t new_elem_idx = list_p->free_elem_idx;

    // Корректируем список свободных элементов
    list_p->free_elem_idx = list_p->first_elem_p[new_elem_idx].idx_next;
    list_p->first_elem_p[list_p->free_elem_idx].idx_prev = 0;

    // Заполняем наш новый элемент
    list_p->first_elem_p[new_elem_idx].idx_prev = idx_prev;
    list_p->first_elem_p[new_elem_idx].value    = value;
    list_p->first_elem_p[new_elem_idx].idx_next = list_p->first_elem_p[idx_prev].idx_next;

    // Исправляем новых соседей
    list_p->first_elem_p[idx_prev].idx_next = new_elem_idx;
    list_p->first_elem_p[list_p->first_elem_p[new_elem_idx].idx_next].idx_prev = new_elem_idx;

    CHECK_LIST(list_p);

    return RET_CORRECT;
}

return_codes addElemInListBeforeThat(list_t* const list_p, size_t idx_next, int value){

    assert(list_p);

    CHECK_LIST(list_p);

    if(list_p->first_elem_p[idx_next].value == POISON && idx_next != 0){

        printf(RED "ERROR: incorrect addElemInListBeforeThat with idx_next = %llu - POISON element, not a part of list\n" DEFAULT, idx_next);
        return RET_ERROR;
    }

    if(checkAndResizeUp(list_p) == RET_ERROR){

        return RET_ERROR;
    }

    size_t new_elem_idx = list_p->free_elem_idx;

    // Корректируем список свободных элементов
    list_p->free_elem_idx = list_p->first_elem_p[new_elem_idx].idx_next;
    list_p->first_elem_p[list_p->free_elem_idx].idx_prev = 0;

    // Заполняем наш новый элемент
    list_p->first_elem_p[new_elem_idx].idx_prev = list_p->first_elem_p[idx_next].idx_prev;
    list_p->first_elem_p[new_elem_idx].value    = value;
    list_p->first_elem_p[new_elem_idx].idx_next = idx_next;

    // Исправляем новых соседей
    list_p->first_elem_p[idx_next].idx_prev = new_elem_idx;
    list_p->first_elem_p[list_p->first_elem_p[new_elem_idx].idx_prev].idx_next = new_elem_idx;

    CHECK_LIST(list_p);

    return RET_CORRECT;
}

return_codes deleteElemInList(list_t* const list_p, size_t idx_del){

    assert(list_p);

    if(idx_del == 0 || list_p->first_elem_p[idx_del].value == POISON){

        printf(RED "ERROR: incorrect idx to delete: %llu\n" DEFAULT, idx_del);
        return RET_ERROR;
    }

    CHECK_LIST(list_p);

    // Исправляем старых соседей
    size_t idx_old_prev = list_p->first_elem_p[idx_del].idx_prev;
    size_t idx_old_next = list_p->first_elem_p[idx_del].idx_next;

    list_p->first_elem_p[idx_old_prev].idx_next = idx_old_next;
    list_p->first_elem_p[idx_old_next].idx_prev = idx_old_prev;

    // Заполняем удаляемый элемент
    list_p->first_elem_p[idx_del].idx_prev = 0;
    list_p->first_elem_p[idx_del].value    = POISON;
    list_p->first_elem_p[idx_del].idx_next = list_p->free_elem_idx;

    // Корректируем список свободных элементов
    list_p->first_elem_p[list_p->free_elem_idx].idx_prev = idx_del;
    list_p->free_elem_idx = idx_del;

    CHECK_LIST(list_p);

    return RET_CORRECT;
}

return_codes checkAndResizeUp(list_t* const list_p){

    assert(list_p);

    CHECK_LIST(list_p);

    if(list_p->first_elem_p[list_p->free_elem_idx].idx_next != 0){

        return RET_CORRECT;
    }

    elem_struct_t* new_memory_block_p = (elem_struct_t*) realloc(list_p->first_elem_p, sizeof(elem_struct_t) * (list_p->size * 2 + 1));

    if(new_memory_block_p == NULL){

        printf(RED "ERROR: no way to reallocate memory for list to add a new element to it\n" DEFAULT);

        return RET_ERROR;
    }

    list_p->first_elem_p = new_memory_block_p;

    printf("Allocated new memory block\n");

    list_p->first_elem_p[list_p->free_elem_idx].idx_next = list_p->size + 1;


    for(size_t i = list_p->size + 1; i <= list_p->size * 2; i++){

        list_p->first_elem_p[i].idx_prev = i - 1;

        list_p->first_elem_p[i].idx_next = i + 1;

        list_p->first_elem_p[i].value = POISON;
    }

    list_p->first_elem_p[list_p->size * 2].idx_next = 0;

    list_p->size *= 2;

    CHECK_LIST(list_p);

    return RET_CORRECT;
}

return_codes addElemInTailOfList(list_t* const list_p, int value){

    assert(list_p);

    return addElemInListAfterThat(list_p, list_p->first_elem_p[0].idx_prev, value);
}

return_codes addElemInHeadOfList(list_t* const list_p, int value){

    assert(list_p);

    return addElemInListBeforeThat(list_p, list_p->first_elem_p[0].idx_next, value);
}

bool isElemNextEdgeCorrect(const list_t* const list_p, size_t idx){

    assert(list_p);

    if(list_p->first_elem_p[idx].idx_next == 0 || (list_p->first_elem_p[list_p->first_elem_p[idx].idx_next].idx_prev == idx && list_p->first_elem_p[idx].idx_next != idx)){

        return true;

    }else{

        printf("INCORRECT NEXT IDX: %llu\n", idx);
        return false;
    }
}

bool isElemPrevEdgeCorrect(const list_t* const list_p, size_t idx){

    assert(list_p);

    if((list_p->first_elem_p[idx].idx_prev == 0 && list_p->first_elem_p[idx].value == POISON)|| (list_p->first_elem_p[list_p->first_elem_p[idx].idx_prev].idx_next == idx && list_p->first_elem_p[idx].idx_prev != idx)){

        return true;

    }else{

        printf("INCORRECT PREV IDX: %llu\n", idx);
        return false;
    }
}

bool isListCorrect(const list_t* const list_p){

    assert(list_p);

    for(size_t idx = 0; idx <= list_p->size; idx++){

        if(isElemNextEdgeCorrect(list_p, idx) == 0 || isElemPrevEdgeCorrect(list_p, idx) == 0){

            printf(RED "Incorrect element with idx: %llu" DEFAULT, idx);
            return RET_ERROR;
        }
    }

    return RET_CORRECT;
}
