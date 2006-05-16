
#include "cl.h"

Cl* Cl_Init(  )
{
  Cl* Self = (Cl*) malloc( sizeof(Cl) );
  
  Self->Hist = History_Init();
  
  Self->Decoration = (char*) malloc( sizeof(char) * (strlen(">")+1) );
  Self->Decoration = strcpy( Self->Decoration, ">" );
  Self->Decoration[1] = '\0';

  //State = WAITING;

  return Self;
  
}

int Cl_SetDecoration( Cl* Self, char* Decoration )
{
  if( Self == NULL || Decoration == NULL )
  {
    return -1;
  }
  
  if( Self->Decoration != NULL )
  {
    free( Self->Decoration );
  }

  Self->Decoration = (char*) malloc( sizeof(char) * ( strlen( Decoration ) + 1 ) );
  Self->Decoration = strcpy( Self->Decoration, Decoration );
  
  return 0; 
}


int Cl_SetReturnCallback( Cl* Self, void(*Callback)(int, char**,void*), void* Parameter );
int Cl_SetTabCallback( Cl* Self, void(*Callback)(char*,void*), void* Parameter );
 
int Cl_RegisterFunction( Cl* Self, char* Command, void(*Callback)(char*,void*), void* Parameter );
int Cl_UnRegisterFunction( Cl* Self, char* Command );

char* Cl_AddChar( Cl* Self, char* c )
{
  if( Self == NULL || c == NULL )
  {
    return NULL;
  }  
  int i;
   
  //printf("%s",c); 
  int Len = strlen(c);
  for( i = 0; i < Len; i++ )
  {
    if( c[i] == '\n' || c[i] == '\r' )
    {
      History_ConfirmCurrent( Self->Hist );    
      continue;
    }
    History_AddCharToCurrent( Self->Hist, c[i] ); 
  }
  return c;  
}







int Cl_Destroy( Cl* Self );


