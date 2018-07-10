/// file: stackADT.c
/// description: implementation for stack abstract data type
/// author: RIT CS Department

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <assert.h>

#define	STACK_ALLOC_UNIT	5

struct stackStruct {
	void **contents;
	size_t capacity;
	size_t num;
};

#include "stackADT.h"

StackADT stk_create( void ) {
	StackADT new;

	new = (StackADT) malloc( sizeof(struct stackStruct) );
	if( new != 0 ) {
		new->contents = 0;
		new->capacity = 0;
		new->num = 0;
	}

	return( new );
}

void stk_destroy( StackADT stack ) {
	assert( stack != 0 );

	if( stack->contents != 0 ) {
		free( stack->contents );
	}

	free( stack );
}

void stk_clear( StackADT stack ) {
	if( stack->contents != 0 ) {
		free( stack->contents );
		stack->contents = 0;
	}
	stack->capacity = 0;
	stack->num = 0;
}

void stk_push( StackADT stack, void *data ) {

	if( stack->contents == 0 ) {
		stack->contents = malloc( sizeof(void *) * STACK_ALLOC_UNIT );
		assert( stack->contents != 0 );
		stack->capacity = STACK_ALLOC_UNIT;
	}
	if( stack->num >= stack->capacity ) {
		void *tmp;
		tmp = realloc( stack->contents,
			sizeof(void *) * (stack->capacity + STACK_ALLOC_UNIT) );
		assert( tmp != 0 );
		stack->contents = tmp;
		stack->capacity += STACK_ALLOC_UNIT;
	}

	stack->contents[stack->num] = data;
	stack->num += 1;
}

void *stk_pop( StackADT stack ) {
	int n;
	void *data;

	assert( stack->num > 0 );

	n = stack->num -= 1;
	data = stack->contents[n];

	return( data );
}

void *stk_top( StackADT stack ) {
	int n;
	void *data;

	assert( stack->num > 0 );

	n = stack->num - 1;
	data = stack->contents[n];

	return( data );
}

bool stk_empty( StackADT stack ) {
	return( stack->num == 0 );
}

bool stk_full( StackADT stack ) {
	(void)stack; // quiet. shh!
	return( false );
}