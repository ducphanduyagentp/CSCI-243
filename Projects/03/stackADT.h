/// file: stackADT.h
/// description: interface for stack abstract data type
/// author: RIT CS Department

#ifndef _STACKADT_H_
#define _STACKADT_H_

#include <stdbool.h>

typedef struct stackStruct *StackADT;

StackADT stk_create( void );
void stk_destroy( StackADT stack );
void stk_clear( StackADT stack );
void stk_push( StackADT stack, void *data );
void *stk_pop( StackADT stack );
void *stk_top( StackADT stack );
bool stk_empty( StackADT stack );
bool stk_full( StackADT stack );

#endif
