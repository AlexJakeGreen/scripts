/*
 * cvar.c - набор инструкций по обслуживанию cvar-ов - динамических переменных,
 * которые также доступны пользователю и могут быть изменены в процессе работы
 * программы 
 */
#include <stdio.h>
#include <stdlib.h>
#include "cvar.h"
#include "common.h"

/* "Предыдущий" cvar */
cvar_t  *cvar_vars;

/* Максимальное количество консольных переменных */
#define MAX_CVARS 31
/* память выделяем всю и сейчас */
cvar_t  cvar_indexes[MAX_CVARS];
/* текущий индекс */
int     cvar_numIndexes = 0;



/*
==============================
Cvar_FindVal
Поиск cvar по имени
==============================
*/
cvar_t *Cvar_FindVar(char *var_name)
{
    cvar_t  *var;
    for(var = cvar_vars; var; var = var->next)
    {
        if(var_name == var->name)return var;
    };
    return NULL;
};


/*
==============================
Cvar_Set
Добавление/Изменение cvar
==============================
*/
cvar_t  *Cvar_Set(char *var_name, char *value)
{
    cvar_t  *var;

    var = Cvar_FindVar(var_name);
    if(var)
    {
        var->value = value;     //Если уже существует - только изменим value
    }
    else
    {
        if(cvar_numIndexes > MAX_CVARS) Comm_Error(1, "MAX_CVARS"); // Лимит cvar-ов

        var = &cvar_indexes[cvar_numIndexes];  //создадим новый элемент и заполним все поля
        cvar_numIndexes++;
        var->name = var_name;
        var->value = value;
        var->defaultValue = value;             //первичное значение
        var->next = cvar_vars;
        cvar_vars = var;
    };
    return var;
};


/* Функции дебага */

/*
==============================
Cvar_PrintAll
Печатаем все сохраненные консольные переменные
==============================
*/
void Cvar_PrintAll(void)
{
    cvar_t *var;
    for(var = cvar_vars; var; var = var->next)
    {
        printf("Var: \"%s\" = \"%s\"\n", var->name, var->value);
    };
};
