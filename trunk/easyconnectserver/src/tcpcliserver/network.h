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


/** \file network.h
 *  \brief Contains the functionality for accepting connections and receiving data.
 *  
 *  This object is a wrapper of the socket.h functionality. It binds to a given
 *  IP and port and waits for incoming connections. This works asynchronous as it
 *  starts a new thread. This means there is just one thread for all incoming data.
 *  This should be enough, because we just get commands.
 *  
 */

      
/** \struct network
 *  \brief This object abstracts the socket functionality.
 *  Use Netword instead of struct network.
 *
 *  The Network object contains the handle of the thread which accept 
 *  the connection and data. It stores the received data in a threadsafe
 *  queue to keep the order of the packages.
 *  
 *  
 */
typedef struct network Network;
struct network
{
  //@{
  /// The Ip stored as zero terminated string.
  char* Ip;
  /// The port where network is bound.
  int Port;
  
  /// Size of the receive buffer.
  int BuffSize;
  /// Pointer to the receive buffer.
  char* Buffer;

  /// Type of the instance. Possible types are RAW_MODE and CLI_MODE.
  int Type; 

  /// Current Socket.
  int SockFD;

  /// Network address
  struct sockaddr_in Address;

  /// Handle to listening thread.
  pthread_t* Listener;
 
  /// Handle of the queue for incoming data. 
  TQueue* InQueue; 

  //@}
};


/** \fn Network* Network_Init( char* Ip, int Port, int Type, int BuffSize, TQueue* Queue )
 *  \brief Use this function to instatiate a Network object.
 *
 *  To create a Network object you have to use this Function. It tries to bind to the given 
 *  ip and port. The type determines which thread it starts. 
 *  CLI_MODE creates a Network instance that works as comandline interface. Use this
 *  if you want to connect yourself with the server via telnet.
 *  RAW_MODE passes the data right trough, without bells and whistles. This is used for
 *  application interfaces.
 *
 *  \param Ip is the ip address as zero terminated string.
 *  \param Port is the port.
 *  \param Type can be RAW_MODE or CLI_MODE 
 *  \param BuffSize is the size of the receive buffer.
 *  \param Queue is the handle to a initialized TQueue. It is used as queue for incoming data.
 *  
 *  \return Handle for the Network instance.
 *
 */
Network* Network_Init( char* Ip, int Port, int Type, int BuffSize, TQueue* Queue );


/** \fn void Network_CliListener( void* data )
 *  \brief This is the thread for the command line listener.
 *
 *  This thread is started in Network_Init if Type is passed as CLI_MODE.
 *  
 */
void Network_CliListener( void* data );

/** \fn void Network_RawListener( void* data )
 *  \brief This is the thread for the raw data listener.
 *
 *  This thread is started in Network_Init if Type is passed as RAW_MODE.
 *  
 */
void Network_RawListener( void* data );

/** \fn int Network_Destroy( Network* Self )
 *  \brief Deinitalizes the Network instance.
 *
 *  Use this function to stop the listener and kill all active connections.
 *  It stops the thread and frees remaining memory. It does not destroy the queue.
 *  Do not use the handle after this function call.  
 *  \param Self is a handle of a initialized Network instance.
 *  \return 0 if all went ok, -1 if a error occured.
 *
 */
int Network_Destroy( Network* Self );

#endif
