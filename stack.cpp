#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#define StackCtor(stk, size)  StackCtor_(stk, size, &#stk[0] + (#stk[0] == '&'), __FILE__,  __PRETTY_FUNCTION__)
#define POISON -429
#define ASSERTED_OK ==0||printf("asserted of line %d\n", __LINE__);

typedef int Elem_t;

struct Stack_info
{
    const char * stack_name;
    const char * file_name;
    const char * function_name;
    int line;
};


struct Stack
{   char protect_Stack_begin[10] = "111111111";
    Elem_t * data;
    int size ;
    int capacity;
    struct Stack_info st;
    char protect_Stack_end[10] = "111111111";
};

struct attacker
{
    int LM[1] = {0};
    struct Stack stk;
    int RM[1] = {0};
};


int StackCtor_(Stack * stk, int count, const char * stack_name, const char * func_name, const char * file_name);
int StackPush(Stack * stk, Elem_t value);
int StackResize(Stack * stk, int count_symbols,const int mode);
Elem_t StackPop(Stack * stk);
int StackUse(Stack * stk);
char * find_errors(Stack * stk);
int errors_reader(Stack * stk, char * string);
int StackStatus(Stack * stk);
int verificator(Stack * stk);
int file_write_realloc( const int count_memory);
void master(attacker * xxx);
int poison_input(Stack * stk);

//what am i need to do
//ASSERTED_OK
//hash and another protect:
//destructor
int main()
{

    Stack stk1 = {};

    StackCtor(&stk1, 5)  ASSERTED_OK

    StackPush(&stk1, 1)  ASSERTED_OK
    StackPush(&stk1, 2)  ASSERTED_OK
    StackPush(&stk1, 3)  ASSERTED_OK

    //StackPush(&stk1, 3)  ASSERTED_OK
    //StackPush(&stk1, 4)  ASSERTED_OK
   // StackPush(&stk1, 5)  ASSERTED_OK
    //StackPush(&stk1, 6)  ASSERTED_OK

    //StackPop(&stk1);
    //StackPop(&stk1);
    //StackPop(&stk1);

    //printf("begin %s\nend %s\n", stk1.protect_Stack_begin, stk1.protect_Stack_end);

    StackStatus(&stk1);


    return 0;
}

                           // Stack_info info
int StackCtor_(Stack * stk, int count, const char * stack_name, const char * func_name, const char * file_name)
{
    if (stk == NULL)
        return 1;

    stk->data = (Elem_t *) calloc (count, sizeof(stk->data[0]));

    if (stk->data == NULL)
        return 1;
    assert(stk->data);

    stk->st.function_name = func_name;
    stk->st.file_name = file_name;
    stk->st.stack_name = stack_name;

    stk->size = 0;
    stk->capacity = count;

    poison_input(stk);

    verificator(stk);
    return 0;
}



int StackPush(Stack * stk, Elem_t value)
{
    verificator(stk);

    if (!(stk->data[0] == 0 && stk->size == 0))
        stk->size++;

    if (stk->size >= stk->capacity)
    {
        if (stk->capacity < 10)
            stk->capacity = 10;

        else
            stk->capacity = stk->capacity * 2;
        StackResize(stk, stk->capacity, 0);
    }

    stk->data[stk->size] = value;

    if (verificator(stk) == 1)
        return 1;

    return 0;
}

int StackResize(Stack * stk, int count_symbols, const int mode)
{
    verificator(stk);

    Elem_t * realloc_cheker = (Elem_t *)realloc(stk->data, count_symbols * sizeof(stk->data[0]));
    assert(realloc_cheker);

    stk->data = realloc_cheker;

    file_write_realloc(count_symbols);

    if (mode == 0)
        poison_input(stk);


    if (verificator(stk) == 1)
        return 1;

    return 0;
}

Elem_t StackPop(Stack * stk)
{
    verificator(stk);

    Elem_t value = stk->data[stk->size];

    stk->data[stk->size] = POISON;

    stk->size--;

    if (stk->size * 4 >= stk->capacity)
    {
        if (stk->capacity <= 10)
            stk->capacity = 10;

        else
            stk->capacity = (stk->capacity)/2;

        StackResize(stk, stk->capacity, 1);
    }

    if (verificator(stk) == 1)
        return 1;

    return value;
}

int StackUse(Stack * stk)
{
    verificator(stk);

    Elem_t value = 0;
    char string[20] = {0};

    while (1)
    {
        scanf("%s %d", string, &value);

        printf("you input: %s\n", string);
        if (strcmp(string, "end") == 0)
         {
            printf("using end");
            return 0;                               //how to make another stop?
        }

        else if (strcmp(string, "push") == 0)
        {
            printf("using StackPush\n");
            StackPush(stk, value);
            printf("data[%d] = %d\n",stk->size-1, stk->data[stk->size-1]);
        }
        else if (strcmp(string, "pop") == 0)
        {
            printf("using StackPop\n");
            value = 0;
            value = StackPop(stk);
            printf("<%d>\n", value);
        }

        else
            printf("incorrect command for Stack");

        printf("you input: %s %d\n", string, value);

        int i = stk->size;
        printf("size = %d", stk->size);
        while(i >=0)
        {
            printf("stack status: %d\n", stk->data[i]);
            i--;
        }
    }

    return 1;
}


int StackStatus(Stack * stk)
{
    verificator(stk);

    printf("Stack %s\ncapacity = %d   size = %d\n\n", stk->st.stack_name, stk->capacity, stk->size);

    printf("%s at %s(%d)\n\nStack[%p](", __PRETTY_FUNCTION__, __FILE__, __LINE__, stk);

    if (stk == NULL)
        printf("ERROR poiters is free");
    else
        printf("ok");

    printf(") \"%s\" at %s at %s(%d)\n\n", stk->st.stack_name, stk->st.function_name, stk->st.file_name, stk->st.line);


    int i = 0;
    while (i < stk->capacity)
    {
        printf("stk.data[%d] = %d ", i, stk->data[i]);

        if (stk->data[i] == POISON)
            printf("(POISON)");

        printf("\n");
        i++;
    }

    return 0;
}

char* find_errors(Stack * stk)
{
    assert(stk);
    int error_place = 0;
    char array_with_errors[5] = {0};

    if (stk == NULL)
        array_with_errors[error_place] = 1;      //error_place++?
    error_place++;

    if (stk->data == NULL)
        array_with_errors[error_place] = 1;
    error_place++;

    if (stk->size > stk-> capacity)
        array_with_errors[error_place] = 1;
    error_place++;

    if (stk->size < 0)
        array_with_errors[error_place] = 1;
    error_place++;

    return array_with_errors;
}



int errors_reader(Stack * stk, char * string)
{
    assert(stk);

    int count_of_mistakes = 0;

    if (string[0] == 1)
    {
        printf("pointer on %s = NULL\n", stk->st.stack_name);
        count_of_mistakes++;
    }

    if (string[1] == 1)
    {
        printf("pointer on %s->data = NULL\n", stk->st.stack_name);
        count_of_mistakes++;
    }

    if (string[2] == 1)
    {
        printf("In Stack %s size > capacity\n", stk->st.stack_name);
        count_of_mistakes++;
    }

    if (string[3] == 1)
    {
        printf("In Stack %s size < 0\n", stk->st.stack_name);
        count_of_mistakes++;
    }

    if (count_of_mistakes > 0)
    {
        StackStatus(stk);
        abort();
        return 1;
    }

    return 0;

}


int verificator(Stack * stk)
{
    if (errors_reader(stk, find_errors(stk)) == 1)
        return 1;

    return 0;
}

int file_write_realloc( const int count_elements)
{
    FILE * stream = fopen("realloc_work.txt", "w");
    fprintf(stream, "realloc was used. Memory changed to %d\n", count_elements);
    return 0;
}

void master(attacker * xxx)
{
    xxx->LM[1] = 0;
    xxx->RM[-1] = 0;
}

int poison_input(Stack * stk)
{
    int i = stk->size + 1;

    while (i < stk->capacity)
    {
        stk->data[i] = POISON;
        i++;
    }

    return 0;
}
