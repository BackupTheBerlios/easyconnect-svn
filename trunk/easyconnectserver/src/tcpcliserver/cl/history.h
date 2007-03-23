
#ifndef __HISTORY_H
#define __HISTORY_H

#include <string.h>
#include <stdlib.h>

typedef struct entry Entry;
struct entry
{
  char* Entry;	
  int EntryLen;
  
  char* Working;
  int WorkingLen;
  
  Entry* Next;
  Entry* Previous;
  
};

Entry* Entry_Init( char* Entry );
Entry* Entry_Copy( Entry* Tmp );

int Entry_Add( Entry* Self, char c );
int Entry_AddPos( Entry* Self, char c, int pos );

int Entry_Confirm( Entry* Self );
int Entry_Undo( Entry* Self );

char* Entry_Get( Entry* Self );

int Entry_Destroy( Entry* Self );

//////////////////////////////////////////////////

typedef struct history History;
struct history
{
  Entry* First;
  Entry* Last;

  Entry* Current;

  int Length;

};

History* History_Init( );

//int History_AddNew( History* Self, char* Entry );
int History_AddCharToCurrent( History* Self, char c );
int History_AddCharToCurrentPos( History* Self, char c, int pos );

char* History_GetCurrent( History* Self );
char* History_ShiftBack( History* Self );
char* History_ShiftForward( History* Self );

int History_ConfirmCurrent( History* Self );

int History_Destroy( History* Destroy );

#endif
