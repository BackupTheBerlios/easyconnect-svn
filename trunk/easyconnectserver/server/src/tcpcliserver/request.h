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
#ifndef __REQUEST_H
#define __REQUEST_H

#include <pthread.h>
#include "netdata.h"
#include "netfunctionlist.h"
#include "sender.h"

typedef struct request Request;
struct request
{
  NetFunction* Func;
  NetData* Data; 

  Sender* Send;
  
  Request* Next;
	
};

Request* Request_Init( NetFunction* Func, NetData* Data, Sender* Send );
int Request_Execute( Request* Self );
int Request_Destroy( Request* Self );

struct clirequest
{
  char* CallString;
  
  Sender* Send;

  int Open;  
  Request* Next;
	
};
typedef struct clirequest CliRequest;

CliRequest* CliRequest_Init( Sender* Send, NetData* Data );
int CliRequest_AddNetData( CliRequest* Self, NetData* Data );
int CliRequest_Complete( CliRequest* Self );
int CliRequest_Destroy( CliRequest* Self );


typedef struct socketrequest SocketRequest;
struct socketrequest
{
  int Socket;
  
  pthread_t Thread;

  TQueue* ReqQueue;
  
  SocketRequest* Previous;
  SocketRequest* Next;
	
};

SocketRequest* SocketRequest_Init( int Socket );
int SocketRequest_AddRequest( SocketRequest* Self, NetFunction* Func, NetData* Data, Sender* Send );

// Removes the first request
int SocketRequest_RemoveRequest( SocketRequest* Self );
Request* SocketRequest_GetFirstRequest( SocketRequest* Self );
int SocketRequest_GetLength( SocketRequest* Self );
void SocketRequest_Execute( void* data ); // data must be a SocketRequest
int SocketRequest_Destroy( SocketRequest* Self );


struct requestlist
{
  Sender* Send;

  SocketRequest* First;
  SocketRequest* Last;
  int Length;

  CliRequest* CliFirst;
  CliRequest* CliLast;
  int CliLength; 
  
  // User Management Interface could be implemented here 
};
typedef struct requestlist RequestList;

RequestList* RequestList_Init( Sender* Send );
int RequestList_AddRequest( RequestList* Self, NetFunction* Func, NetData* Data );
int RequestList_RemoveRequest( RequestList* Self, int Socket );
int RequestList_Destroy();
SocketRequest* RequestList_RawSocketExists( RequestList* Self, int Socket );
#endif
