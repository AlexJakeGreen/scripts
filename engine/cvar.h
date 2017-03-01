/*
 * cvar.c - набор инструкций по обслуживанию cvar-ов - динамических переменных,
 * которые также доступны пользователю и могут быть изменены в процессе работы
 * программы
 */
#ifndef CVAR_H_INCLUDED
#define CVAR_H_INCLUDED

typedef struct cvar_s
{
    char *name;
    char *value;
    char *defaultValue;
    int  flags;
    struct cvar_s *next;
} cvar_t;


cvar_t *Cvar_FindVar(char *var_name);
cvar_t  *Cvar_Set(char *var_name, char *value);
void Cvar_PrintAll(void);


#endif // CVAR_H_INCLUDED
