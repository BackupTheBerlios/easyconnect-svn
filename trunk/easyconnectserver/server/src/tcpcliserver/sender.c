
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

#include "sender.h"

Sender* Sender_Init( TQueue* OutQueue )
{
  Sender* Self = (Sender*) malloc( sizeof( Sender ));
  Self->Out = OutQueue;
  int i = pthread_create( &Self->SendThread, NULL, (void*) &Sender_Thread, (void*) Self );

  return Self;
}

int Sender_AddMessage( Sender* Self, char* Msg, int Socket, int Type )
{
  return TQueue_AddElement( Self->Out, (void*) NetData_Init( Msg, Socket, Type ));
}

int Sender_AddNetData( Sender* Self, NetData* Data )
{
  return TQueue_AddElement( Self->Out, (void*) Data);
}

void Sender_Thread( void* data )
{
  Sender* Self = (Sender*) data;
  NetData* Outgoing = NULL;
  while(1)
  {
    Outgoing =(NetData*) TQueue_GetElement( Self->Out );
    // pthread_testcancel();
   
    //printf("To Send: %s To Socket: %i \n", Outgoing->Data, Outgoing->Socket ); 
    send( Outgoing->Socket, Outgoing->Data, strlen(Outgoing->Data), 0 );
    NetData_Destroy( Outgoing );	    
  }
}
