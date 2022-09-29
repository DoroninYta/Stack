#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>




#define StackCtor(stk, size)  StackCtor_(stk, size, &#stk[0] + (#stk[0] == '&'), __FILE__,  __PRETTY_FUNCTION__, __LINE__)
#define POISON -429
//#define ASSERTED_OK ==0||printf("asserted of line %d\n", __LINE__);
#define CODE 11022003
#define PHONE "89001409398"
#define verificator(stk) verificator_(stk, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define StackPush(stk, value) StackPush_(stk,value, __LINE__)
#define StackResize(stk, count_symbols, mode) StackResize_(stk, count_symbols, mode, __LINE__)
#define StackPop(stk) StackPop_(stk, __LINE__)
#define StackUse(stk) StacakUse_(stk, __LINE__)






typedef int Elem_t;

struct Stack_info
{
    const char * stack_name = 0;
    const char * file_name = 0;
    const char * function_name = 0;
    int line = 0;
};


struct protector
{
    Elem_t left_side = CODE;
    Elem_t * memory = 0;
    Elem_t right_side = CODE;
};


struct Stack
{   const char protect_Stack_begin[12] = PHONE;
    Elem_t * data = 0;
    int size = 0;
    int capacity = 0;
    struct protector safe;
    struct Stack_info st_main;
    struct Stack_info st_fun;
    unsigned long long int hash = 0;
    const char protect_Stack_end[12] = PHONE;
};


struct attacker
{
    int LM[1] = {0};
    struct Stack stk;
    int RM[1] = {0};
}xxx;

int StackCtor_(Stack * stk, int count, const char * stack_name, const char * func_name, const char * file_name, int line);
int StackPush_(Stack * stk, Elem_t value, const int line);
int StackResize_(Stack * stk, int count_symbols, const int mode, const int line);
Elem_t StackPop_(Stack * stk, const int line);
int StackUse_(Stack * stk, const int line);
int StackStatus(Stack * stk);
char * find_errors(Stack * stk);
int errors_reader(Stack * stk, char * string);
int verificator_(Stack * stk, const char * file_name,const char * fun_name, const int line);
int file_write_realloc( const int count_memory);
void master(attacker * xxx);
int poison_input(Stack * stk);
int Stack_calloc_canary(Stack * stk);
void memory_distribution(Stack * stk);
int Stack_realloc_canary(Stack * stk);
unsigned long long int  hash(const Stack * stk);
int fun_info(Stack * stk, const char * file_name,const char * fun_name, const int line);



//what am i need to do
 /*добавить с помощью дефайнов инициализацию stk.st_fun при вызове функций, использующих verificator и добавить канарейки и
   хэш, исправить иниц
   st_main , исправить StackStatus, использовать хуеву тучу дефайнов
   Destructor, условная компиляция проект готов.

*/

int main()
{

    Stack stk1 = {};

    int i =0;
    while(i < stk1.capacity)
    {
        printf("1 %d\n", stk1.data[i]);
        i++;
    }

    StackCtor(&stk1, 5);

    StackPush(&stk1, 1);
    StackPush(&stk1, 2);
    StackPush(&stk1, 3);

    StackPush(&stk1, 3);
    StackPush(&stk1, 4);
    StackPush(&stk1, 5);
    StackPush(&stk1, 6);

    //StackPop(&stk1);
    //StackPop(&stk1);
    //StackPop(&stk1);

    StackStatus(&stk1);





    return 0;
}

                           // Stack_info info
int StackCtor_(Stack * stk, int count, const char * stack_name, const char * func_name, const char * file_name, int line)
{
    if (stk == NULL)
        return 1;

    stk->capacity = count;
    Stack_calloc_canary(stk);

    if (stk->data == NULL)
        return 1;
    assert(stk->data);

    stk->st_main.function_name = func_name;
    stk->st_main.file_name = file_name;
    stk->st_main.stack_name = stack_name;
    stk->st_main.line = line;

    stk->size = 0;

    poison_input(stk);

    stk->hash = hash(stk);

    verificator(stk);
    return 0;
}


int StackPush_(Stack * stk, Elem_t value, const int line)
{

    verificator(stk);

    stk->st_main.line = line;

    if (!(stk->data[0] == 0 && stk->size == 0))
        stk->size++;

    if (stk->size >= stk->capacity)
    {
        stk->safe.memory[stk->capacity + 1] = POISON;
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


int StackResize_(Stack * stk, int count_symbols, const int mode, const int line)
{
    verificator(stk);

    stk->st_main.line = line;

    Stack_realloc_canary(stk);

    file_write_realloc(count_symbols);

    if (mode == 0)
        poison_input(stk);

    verificator(stk);

    return 0;
}


Elem_t StackPop_(Stack * stk, const int line)
{
    verificator(stk);

    stk->st_main.line = line;

    Elem_t value = stk->data[stk->size];

    stk->data[stk->size] = POISON;

    stk->size--;

    if (stk->size * 4 >= stk->capacity)
    {
        stk->safe.memory[stk->capacity + 1] = POISON;

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


int StackUse_(Stack * stk, const int line)
{
    verificator(stk);

    stk->st_main.line = line;

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
    if (stk == NULL)
        return 1;

    printf("Stack %s\ncapacity = %d   size = %d\n\n", stk->st_main.stack_name, stk->capacity, stk->size);

    printf("%s at %s(%d)\n\nStack[%p](", stk->st_fun.function_name, stk->st_fun.file_name, stk->st_fun.line,  stk);

    if (stk == NULL)
        printf("ERROR poiters is free");
    else
        printf("ok");

    printf(") \"%s\" at %s at %s(%d)\n\n", stk->st_main.stack_name, stk->st_main.function_name,
                                           stk->st_main.file_name, stk->st_main.line);

    int i = 0;
    while (i < stk->capacity)
    {
        printf("stk.data[%d] = %d ", i, stk->data[i]);

        if (stk->data[i] == POISON)
            printf("(POISON)");

        printf("\n");
        i++;
    }

    printf("\nHASH %x\n", stk->hash);

    return 0;
}


char * find_errors(Stack * stk)
{
    assert(stk);
    int error_place = 0;
    char array_with_errors[7] = {0};

    if (stk == NULL)
        array_with_errors[error_place] = '1';      //error_place++?
    error_place++;

    if (stk->data == NULL)
        array_with_errors[error_place] = '1';
    error_place++;

    if (stk->size > stk-> capacity)
        array_with_errors[error_place] = '1';
    error_place++;

    if (stk->size < 0)
        array_with_errors[error_place] = '1';
    error_place++;

    if (stk->safe.memory[0] != CODE || stk->safe.memory[stk->capacity + 1] != CODE)
        array_with_errors[error_place] = '1';
    error_place++;

    if (strcmp(stk->protect_Stack_begin, PHONE) != 0 || strcmp(stk->protect_Stack_end, PHONE) != 0)
        array_with_errors[error_place] = '1';
    error_place++;

    if (hash(stk) != stk->hash)
        array_with_errors[error_place] = '1';
    error_place++;

    return array_with_errors;
}


int errors_reader(Stack * stk, char * string)
{
    assert(stk);

    int count_of_mistakes = 0;

    if (string[0] == 1)
    {
        printf("pointer on %s = NULL\n", stk->st_main.stack_name);
        count_of_mistakes++;
    }

    if (string[1] == 1)
    {
        printf("pointer on %s->data = NULL\n", stk->st_main.stack_name);
        count_of_mistakes++;
    }

    if (string[2] == 1)
    {
        printf("In Stack %s size > capacity\n", stk->st_main.stack_name);
        count_of_mistakes++;
    }

    if (string[3] == 1)
    {
        printf("In Stack %s size < 0\n", stk->st_main.stack_name);
        count_of_mistakes++;
    }

    if (string[4] == 1)
    {
        printf("canary in struct Stack %s data was changed", stk->st_main.stack_name);
        count_of_mistakes++;
    }

    if (string[5] == 1)
    {
        printf("canary in struct Stack %s was changed", stk->st_main.stack_name);
        count_of_mistakes++;
    }

    if (string[6] == 1)
    {
        printf("hash data was changed");
        count_of_mistakes++;
    }

    if (count_of_mistakes > 0)
    {
        printf("count of mistakes = %d\n", count_of_mistakes);
        StackStatus(stk);
        abort();
        return 1;
    }

    return 0;

}


int verificator_(Stack * stk, const char * file_name,const char * fun_name, const int line)
{
    fun_info(stk, file_name, fun_name, line);

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


/*int Stack_calloc_memory_distribution(Stack * stk)               ВОПРОС ДЕД!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
    int count_memory = sizeof(stk->protect_Stack_begin) + sizeof(stk->protect_Stack_end) + sizeof(Elem_t) * stk->capacity;

    char * memory = (char *) calloc (count_memory, sizeof(memory[0]));

    stk->protect_Stack_begin = memory;

    stk->data = Elem_t(memory + sizeof(stk->protect_Stack_begin));

    stk->protect_Stack_end = memory + + sizeof(stk->protect_Stack_begin) + sizeof(Elem_t) * stk->capacity;

}*/


int Stack_calloc_canary(Stack * stk)
{
    stk->safe.memory = (Elem_t *) calloc (stk->capacity + 2, sizeof(stk->safe.memory[0]));
    assert(stk->safe.memory);

    memory_distribution(stk);

    return 0;
}


void memory_distribution(Stack * stk)
{
    stk->safe.memory[0] = stk->safe.left_side;
    stk->data = stk->safe.memory + 1;
    stk->safe.memory[stk->capacity + 1] = stk->safe.right_side;
}


int Stack_realloc_canary(Stack * stk)
{
    Elem_t * realloc_checker = (Elem_t *) realloc (stk->safe.memory, sizeof(Elem_t) * (stk->capacity + 2));


    if (realloc_checker == NULL)
        return 1;
    assert(realloc_checker);

    stk->safe.memory = realloc_checker;

    memory_distribution(stk);

    return 0;
}


unsigned long long int  hash(const Stack * stk)
{

    unsigned long long int h = 0;
    Elem_t  c = *stk->data;
    int count_operation = 0;

    while(count_operation < stk->capacity)
    {
        c = *(stk->data + count_operation);

        if (count_operation % 2 == 0)
            h = h * 3 + c;

        else
            h = h / 2 + c;

        count_operation++;
    }

    return h;
}


int fun_info(Stack * stk, const char * file_name,const char * fun_name, const int line)
{
    stk->st_fun.stack_name = stk->st_main.stack_name;
    stk->st_fun.file_name = file_name;
    stk->st_fun.function_name = fun_name;
    stk->st_fun.line = line;

    return 0;
}


