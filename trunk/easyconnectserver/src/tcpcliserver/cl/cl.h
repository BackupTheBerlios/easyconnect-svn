
#ifndef __CL_H_
#define __CL_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "history.h"


enum States { WAITING, // State zero
	      NEW_ENTRY, // State one, user is entering a command
	      HISTORY // State two, Looking at entries in history
	    };
	      
typedef struct cl Cl;
struct cl
{
  History* Hist;

  void* Parameter;
  void (*Callback)( char*, void* );

  char* Decoration;
  //enum States State;
    
};


Cl* Cl_Init(  );

// With this function you can set the returned prompt.
// It will be sent back after every request (\n)
// Default is >, how boring.
int Cl_SetDecoration( Cl* Self, char* Decoration );

// There are two different interfaces to work with libcl
// either you register all your commands and your specific callbacks
// or you register one callback which will be called if the user presses enter.
// I will elaborate the pros and cons:
// First Method:
// It should be used if just the command line is used as input.
// If you plan to use your commands and callbacks somewhere else the method two
// would be better. Still Interested, well okay, you can use RegisterFunction to
// bind a command to a function. The passed parameters will be ignored. 
// This also means you dont have to write your own autocompletion handler.
// libcl will try to complete any input followed by a tab.
// If there is more than one possibility it will list them. 
//
// Second Method:
// 


int Cl_SetReturnCallback( Cl* Self, void(*Callback)(int, char**,void*), void* Parameter );
int Cl_SetTabCallback( Cl* Self, void(*Callback)(char*,void*), void* Parameter );
 
int Cl_RegisterFunction( Cl* Self, char* Command, void(*Callback)(char*,void*), void* Parameter );
int Cl_UnRegisterFunction( Cl* Self, char* Command );

char* Cl_AddChar( Cl* Self, char* c );

int Cl_Destroy( Cl* Self );

#endif
