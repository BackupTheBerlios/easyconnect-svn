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

#ifndef __SENDER_H
#define __SENDER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <pthread.h>

#include "tqueue.h"
#include "netdata.h"

struct sender
{
  TQueue* Out;
  pthread_t SendThread;
};
typedef struct sender Sender;

Sender* Sender_Init( TQueue* OutQueue );

int Sender_AddMessage( Sender* Self, char* Msg, int Socket, int Type );

int Sender_AddNetData( Sender* Self, NetData* Data );
	
void Sender_Thread( void* data );

void Sender_Destroy( Sender* Self );

#endif
