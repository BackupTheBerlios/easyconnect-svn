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

#ifndef __NETWORK_H
#define __NETWORK_H

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <pthread.h>
#include "tqueue.h"
#include "netdata.h"


      
/** \struct network
 *  \brief This object abstracts the TCP socket functionality.
 *
 *  
 */
typedef struct network Network;
struct network
{
  //@{
  char* Ip;
  int Port;
  
  int BuffSize;
  char* Buffer;

  int Type; 

  int SockFD;

  struct sockaddr_in Address;

  pthread_t* Listener;
  
  TQueue* InQueue; // Thread safe queue

  //@}
};

Network* Network_Init( char* Ip, int Port, int Type, int BuffSize, TQueue* Queue );

void Network_CliListener( void* data );

void Network_RawListener( void* data );


#endif
