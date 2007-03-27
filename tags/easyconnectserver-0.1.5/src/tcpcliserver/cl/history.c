
#include "history.h"

Entry* Entry_Init( char* entry )
{
  Entry* Self = (Entry*) malloc( sizeof( Entry ) );

  Self->Next = NULL;
  Self->Previous = NULL;

  Self->Entry = NULL;
  Self->EntryLen = 0;
  
  Self->WorkingLen = 11;

  if( strlen( entry ) +1 > Self->WorkingLen )
  {
    Self->WorkingLen = strlen( entry ) + 6 ;
  }
  
  Self->Working = (char*) malloc( sizeof(char) * Self->WorkingLen );
  strcpy( Self->Working, entry );
  return Self;
	
}

Entry* Entry_Copy( Entry* Tmp )
{
  Entry* Self = (Entry*) malloc( sizeof( Tmp ) );

  Self->Next = NULL;
  Self->Previous = NULL;

  Self->WorkingLen = Tmp->WorkingLen;
  
  Self->Working = (char*) malloc( sizeof(char) * Self->WorkingLen );
  strcpy( Self->Working, Tmp->Entry );
  return Self;

}

int Entry_Add( Entry* Self, char c )
{
  if( Self == NULL )
  {
    return -1;
  }
  
  if( Self->Working == NULL )
  {
    if( strlen( Self->Entry ) +2  > Self->EntryLen )
    {
      Self->WorkingLen += 5;
    }
    Self->Working = (char*) malloc( sizeof(char) * Self->WorkingLen );
    strcpy( Self->Working, Self->Entry );
  }
  
  if( strlen( Self->Working ) +2 > Self->WorkingLen )
  {
    Self->WorkingLen += 6;
    Self->Working = (char*) realloc( Self->Working, sizeof(char)*Self->WorkingLen );
  }
  int tmplen = strlen(Self->Working);
  Self->Working[tmplen] = c;
  Self->Working[tmplen+1] = '\0';  
  return 0;
  
   
}
	
int Entry_AddPos( Entry* Self, char c, int pos )
{
  return -1;
}


int Entry_Confirm( Entry* Self )
{
  if( Self == NULL )
  {
    return -1;
  }
  if( Self->Entry != NULL )
  {
    free( Self->Entry );
  }
  Self->Entry = Self->Working;
  Self->EntryLen = Self->WorkingLen;
  Self->Working = NULL;
  Self->WorkingLen = 0;
  return 0;
}

int Entry_Undo( Entry* Self )
{
  if( Self == NULL )
  {
    return -1;
  }
  if( Self->Working != NULL )
  {
    free( Self->Working );
  }
  Self->WorkingLen = 0;
  return 0;
}

char* Entry_Get( Entry* Self )
{
  if( Self == NULL )
  {
    return NULL;
  }
  if( Self->Working == NULL )
  {
    return Self->Entry;
  } else
    return Self->Working;
}

int Entry_Destroy( Entry* Self )
{
  if( Self == NULL )
  {
    return -1;
  }	    
  if( Self->Entry != NULL )
  {
    free( Self->Entry );
  }
  free( Self );
  
  return 0;
}


////////////////////////////////
//
//  History
//  
////////////////////////////////


History* History_Init(  )
{
  History* Self = (History*) malloc( sizeof(History) );
  Self->First = Entry_Init( "" );
  Self->Last = Self->First;

  Self->Current = Self->First;

  Self->Length = 1;
  
  return Self;
}

int History_AddNew( History *Self, char* entry )
{
  if( Self == NULL )
  {
    return -1;
  }
  Entry* New = Entry_Init( entry );
  Self->Last->Next = New;
  Self->Last = New;
  Self->Length++;
  return 0;
}

int History_AddCharToCurrent( History* Self, char c )
{
  return Entry_Add( Self->Current, c );
}

int History_AddCharToCurrentPos( History* Self, char c, int pos )
{
  return Entry_AddPos( Self->Current, c, pos );
}

char* History_GetCurrent( History* Self )
{
  if( Self == NULL )
  {
    return NULL;
  }
  return Entry_Get( Self->Current );
}

char* History_ShiftBack( History* Self )
{
  if( Self == NULL )
  {
    return NULL;
  } 

  if( Self->Current->Previous != NULL )
  {
    Self->Current = Self->Current->Previous; 
  }
  
  return Entry_Get( Self->Current );
}

char* History_GetNext( History* Self )
{
  if( Self == NULL )
  {
    return NULL;
  } 

  if( Self->Current->Next != NULL )
  {
    Self->Current = Self->Current->Next; 
  }
  
  return Entry_Get( Self->Current );
}

int History_ConfirmCurrent( History* Self )
{
  if( Self == NULL )
  {
    return -1;
  }
  
  if( Self->Current->Next == NULL )
  {
    // We are at the bottom
    Entry_Confirm( Self->Current );
    Self->Current->Next = Entry_Init( "" );
    Self->Last = Self->Current->Next;
    Self->Current = Self->Current->Next; 
    Self->Length++;
  } else
  {
    Entry* Tmp = Entry_Copy( Self->Current );
    Tmp->Previous = Self->Last->Previous;
    Self->Last->Previous->Next = Tmp;
    Entry_Destroy( Self->Last );
    Self->Last = Tmp;
    Entry_Undo( Self->Current );
    Entry_Confirm( Self->Last );
    Self->Current->Next = Entry_Init( "" );
    Self->Last = Self->Current->Next;
    Self->Current = Self->Current->Next; 
    Self->Length++;
  }
  return 0;
}

int History_Destroy( History* Destroy )
{
  return -1;	
}


