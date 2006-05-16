/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  
 *  This software is part of the vscope system http://vscope.de
 *  written by: 
 *
 *  Moritz Eberl <meberl@vscope.de>
 *  
 *
 */

#include "disposer.h"

Disposer* Disposer_Init( TQueue* InQueue, Sender* Send )
{
  Disposer* Self = (Disposer*) malloc( sizeof( Disposer ));
  Self->ErrorMsg = NULL;
  Self->Send = Send;
  Self->InQueue = InQueue;
  Self->FuncList = NetFunctionList_Init();
  Self->ReqList = RequestList_Init( Send );
  //Self->CommandLine = Cl_Init();
  
	  
  pthread_create( &Self->Thread, NULL, (void*) &Disposer_Thread, (void*) Self );
  
  return Self;
}

int Disposer_RegisterFunction( Disposer* Self,
			       char* Command,
			       char*( *Callback )( char*, int, void* ),
			       void* Parameter )
{
  return NetFunctionList_RegisterFunction( Self->FuncList, Command, Callback, Parameter );
}

int Disposer_SetDefaultErrorMsg( Disposer* Self, char* ErrorMsg )
{
  if( Self->ErrorMsg != NULL )
  {
    free( Self->ErrorMsg );
  }
  Self->ErrorMsg = (char*) malloc( strlen( ErrorMsg ) * sizeof( char ));
  strcpy( Self->ErrorMsg, ErrorMsg );
  return 0;
}

int Disposer_HandleCli( Disposer* Self, NetData* Received )
{
  //Disposer_Echo( Self, Received->Socket, Cl_AddChar( Self->CommandLine, Received->Data ));
  
   

	
  return 0;
}

int Disposer_Echo( Disposer* Self, int Socket, char* Data )
{
  return Sender_AddMessage( Self->Send, Data, Socket, CLI_MODE );
}

void Disposer_Thread( void* data )
{
  Disposer* Self = (Disposer*) data;

  NetData* Received = NULL;
  char* Command;
  NetFunction* Func;
  
  while(1)
  {
    // Wait for data
    Received = (NetData*) TQueue_GetElement( Self->InQueue );
    pthread_testcancel();
    
    
    // If in CLI Mode echo char and save until a \n is sent 
    if( Received->Type == CLI_MODE )
    {
      Disposer_HandleCli( Self, Received );
       
      continue;
    }
    // If in Raw Mode take command and 
    if( Received->Type == RAW_MODE )
    {
      Command = GetCommand( Received->Data );
      Func = NetFunctionList_FindFunction( Self->FuncList, Command );
      free( Command );
      if( Func != NULL )
      {
	RequestList_AddRequest( Self->ReqList, Func, Received );      
      } else
      {
        // Here we would send a std error message back
	Sender_AddMessage( Self->Send, Self->ErrorMsg, Received->Socket, Received->Type );
	NetData_Destroy( Received );
      }
    }
    
  }
}


char* GetCommand( char* String )
{
  if( String == NULL )
  {
    return NULL; 
  }

  char* Command = NULL;
  char* Space = strchr( String, ' ' );
  int Stringlength = 0;
  if( Space != NULL )
  {
    Stringlength = strlen( String ) - strlen( Space );
    Command = (char*) malloc( ( Stringlength  + 1 )  * sizeof( char ));
    strncpy( Command, String, Stringlength );
    Command[Stringlength] = '\0'; 
    return Command;
  }else 
  {
    Command = (char*) malloc(  sizeof( char ) * ( strlen( String ) + 1 ) );
    strcpy( Command, String );
    return Command;
  }
  return NULL;
}
