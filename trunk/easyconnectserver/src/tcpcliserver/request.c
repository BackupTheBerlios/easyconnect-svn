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
#include "request.h"


Request* Request_Init( NetFunction* Func, NetData* Data, Sender* Send )
{
  Request* Self = (Request*) malloc( sizeof(Request) );
  Self->Send = Send;
  Self->Func = Func;
  Self->Data = Data;
  //Self->Next = NULL;

  return Self;
}

int Request_Execute( Request* Self )
{
  // We have to check if the Functiondata is still present
  if( Self->Func == NULL )
  {
    return -1;
  }
  char* Ret;
  // Execute Function
  Ret = Self->Func->Callback( Self->Data->Data, Self->Data->Socket, Self->Func->Parameter );
  if( Ret != NULL )
  {
    Sender_AddMessage( Self->Send, Ret, Self->Data->Socket, RAW_MODE );
  }
  if( Ret == NULL )
    Sender_AddMessage( Self->Send, "\n", Self->Data->Socket, RAW_MODE );
  
  return 0;
}


int Request_Destroy( Request* Self )
{
  NetData_Destroy(Self->Data);
  free(Self);
  return 0;
}



// -------------------------------------------------------------
//
// SocketRequest
//
// -------------------------------------------------------------



SocketRequest* SocketRequest_Init( int Socket )
{
  SocketRequest* Self = (SocketRequest*) malloc( sizeof( SocketRequest ));
  Self->Socket = Socket;
  Self->ReqQueue = TQueue_Init();
  
  Self->Next =NULL;

  pthread_create( &Self->Thread, NULL, (void*)&SocketRequest_Execute, (void*) Self ); 
  
  return Self;

}

int SocketRequest_AddRequest( SocketRequest* Self, NetFunction* Func, NetData* Data, Sender* Send )
{
  return TQueue_AddElement( Self->ReqQueue, (void*) Request_Init( Func, Data, Send ) );
}

void SocketRequest_Execute( void* data )
{
  SocketRequest* Self = ( SocketRequest* ) data;
  Request* Tmp;
  while(1)
  {
    Tmp = (Request*) TQueue_GetElement( Self->ReqQueue ); 
    pthread_testcancel();
    Request_Execute( Tmp );
    Request_Destroy( Tmp );
  } 
}

int SocketRequest_Destroy( SocketRequest* Self )
{
  pthread_cancel( Self->Thread );
  TQueue_WakeQueue( Self->ReqQueue );
  free( Self ); 
}



// --------------------------------------------------------------
//
// RequestList
//
// --------------------------------------------------------------

RequestList* RequestList_Init( Sender* Send )
{
  RequestList* Self = ( RequestList* ) malloc( sizeof( RequestList ));

  Self->Send = Send;
  Self->First = NULL;
  Self->Last = NULL;
  Self->Length = 0;

  /*
  Self->CliFirst = NULL;
  Self->CliLast = NULL;
  Self->CliLength = 0;
  */
  return Self;
}

int RequestList_AddRequest( RequestList* Self, NetFunction* Func, NetData* Data )
{
  if( Data->Type == CLI_MODE )
  {
    
  }
  else if( Data->Type == RAW_MODE )
  {
    // Check if socket is in list, if not add
    SocketRequest* Tmp = RequestList_RawSocketExists( Self, Data->Socket );
    if( Tmp == NULL )
    {
      Tmp = SocketRequest_Init( Data->Socket );
      if( Self->Length == 0 )
      {
	Self->First = Tmp;
	Self->Last = Tmp;
	Self->Length++;
      } else
      {
	Self->Last->Next = Tmp;
	Self->Length++;
      }
    }
    // Add request to socketrequest
    SocketRequest_AddRequest( Tmp, Func, Data, Self->Send );
      
  }
  
}

int RequestList_Destroy(RequestList* Self)
{
  return 0;
}

SocketRequest* RequestList_RawSocketExists( RequestList* Self, int Socket )
{
  int i;
  SocketRequest* Tmp = Self->First;
  for( i = 0; i < Self->Length; i++ )
  {
    if( Tmp->Socket == Socket )
    {
      return Tmp;
    }
    if( Tmp->Next != NULL )
    {
      Tmp = Tmp->Next;
    } else break;
  }
  return NULL;
}
