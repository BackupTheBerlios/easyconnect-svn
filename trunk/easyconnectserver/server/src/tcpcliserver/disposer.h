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

#ifndef __DISPOSER_H
#define __DISPOSER_H

#include <pthread.h>
#include <string.h>

#include "tqueue.h"
#include "sender.h"
#include "netfunctionlist.h"
#include "request.h"
#include "cl/cl.h"

typedef struct disposer Disposer;
struct disposer 
{
  NetFunctionList* FuncList;
  RequestList* ReqList;
  TQueue* InQueue;
  Sender* Send;
  pthread_t Thread;
  
  // Thread list
  // returnvalues from functions

  Cl* CommandLine;
  
  char* ErrorMsg;
};


Disposer* Disposer_Init( TQueue* InQueue, Sender* Send );	

int Disposer_RegisterFunction( Disposer* Self, char* Command, char*( *Callback )( char*, int, void* ), void* Parameter );

int Disposer_SetDefaultErrorMsg( Disposer* Self, char* ErrorMsg );

int Disposer_HandleCli( Disposer* Self, NetData* Received );

int Disposer_Echo( Disposer* Self, int Socket, char* Data );

void Disposer_Thread( void* data );

void Disposer_StarterThread( void* data );

// filters the command from a string 
// Format: "Command Param1 wurbl foo bar"
char* GetCommand( char* String );

#endif
