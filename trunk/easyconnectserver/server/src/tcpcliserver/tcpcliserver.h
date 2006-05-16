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

#ifndef __TCPCLISERVER_H_
#define __TCPCLISERVER_H_

#include "tqueue.h"
#include "network.h"
#include "sender.h"
#include "disposer.h"



typedef struct tcpcliserver TcpCliServer;
struct tcpcliserver
{
  TQueue*   InQueue; 
  TQueue*   OutQueue; 
  Network*  CliNet;
  Network*  RawNet;
  Sender*   Send;
  Disposer* Disp; 
  
};


TcpCliServer* TcpCliServer_Init();

int TcpCliServer_InitCli( TcpCliServer* Self, char* Ip, int Port, int BufferSize );
int TcpCliServer_InitRaw( TcpCliServer* Self, char* Ip, int Port, int BufferSize );

int TcpCliServer_RegisterFunction( TcpCliServer* Self, char* Command, char*( *Callback )(char*, int, void* ), void* Parameter );

int TcpCliServer_SetDefaultError( TcpCliServer* Self, char* ErrorMsg );

void TcpCliServer_Destroy( TcpCliServer* Self );

#endif
