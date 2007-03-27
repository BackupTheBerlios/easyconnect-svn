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
#include "tcpcliserver.h"

TcpCliServer* TcpCliServer_Init()
{
  TcpCliServer* Self = (TcpCliServer*) malloc( sizeof( TcpCliServer ));
  
  Self->InQueue = TQueue_Init();
  Self->OutQueue = TQueue_Init();

  Self->RawNet = NULL;
  Self->CliNet = NULL;

  Self->Send = Sender_Init( Self->OutQueue );
  Self->Disp = Disposer_Init( Self->InQueue, Self->Send );
	  
  return Self;
}

int TcpCliServer_InitCli( TcpCliServer* Self, char* Ip, int Port, int BufferSize )
{
  if( Self == NULL || Ip == NULL )
  {
    return -1;
  }
  
  Self->CliNet = Network_Init( Ip, Port, CLI_MODE, BufferSize, Self->InQueue );
  if( Self->RawNet != NULL)
  {
    return 0;
  } else
  {
    return -1;
  }
  
}

int TcpCliServer_InitRaw( TcpCliServer* Self, char* Ip, int Port, int BufferSize )
{
  if( Self == NULL || Ip == NULL )
  {
    return -1;
  }
  
  Self->RawNet = Network_Init( Ip, Port, RAW_MODE, BufferSize, Self->InQueue );
  if( Self->RawNet != NULL)
  {
    return 0;
  } else
  {
    return -1;
  }
  
}

int TcpCliServer_RegisterFunction( TcpCliServer* Self, char* Command,
				   char*( *Callback )(char*, int, void* ),
				   void* Parameter )
{
  if( Self == NULL || Command == NULL )
  {
    return -1;
  }
  
  return Disposer_RegisterFunction( Self->Disp, Command, Callback, Parameter );
}	

int TcpCliServer_SetDefaultError( TcpCliServer* Self, char* ErrorMsg )
{
  if( Self == NULL || ErrorMsg == NULL )
  {
    return -1;
  }

  Disposer_SetDefaultErrorMsg( Self->Disp, ErrorMsg );
  
  return 0;
}
      
void TcpCliServer_Destroy( TcpCliServer* Self )
{
  if( Self != NULL )
  {
    free( Self );
  }
  
  return;
}
