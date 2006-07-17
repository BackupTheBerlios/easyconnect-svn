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

/** \file request.h
 *  \brief The requests are a list of queues, containing requested commands.
 *  The request mechanism enables the system to create a queue for every connected
 *  client, and execute their commands one bye one. This prevents the system from executing
 *  commands just for one user. 
 *
 */

/** \typedef struct request Request 
 *  Use Request instead of struct request.
 */

/** \struct request
 *  \brief A Request is a representation of a valid command, waiting to be executed.
 *
 *  The Request struct is basically the information carrier of the command. It contains
 *  a reference to the stored function as well as the information who requested the execution.
 *  The string which triggerd the command is also stored.  
 *
 */
typedef struct request Request;
struct request
{
  //@{
  /// Func is a reference to the stored function. It contains the pointer to the function and the name of the command.
  NetFunction* Func;
  /// Data contains the Socket of the client and the original string with parameters if present.
  NetData* Data; 
  /// Send is a pointer to a valid Sender instance. This is necessary to return the result to the client.
  Sender* Send;
  //Request* Next;
  //@}	
};

/** \fn Request* Request_Init( NetFunction* Func, NetData* Data, Sender* Send )
 *  \brief Creates a new Request instance.
 *  This function reserves the memory for a new Request instance and fills it with the passed data.
 *  \Note It does not copy the passed Data instance. Don't destroy it.
 * 
 *  \param Func is a pointer to the appropriate NetFunction instance.
 *  \param Data is the NetData element the triggered the execution.
 *  \param Send is a valid Sender instance needed for sending back the result.
 *
 *  \return Handle to the new Request instance.
 *
 */
Request* Request_Init( NetFunction* Func, NetData* Data, Sender* Send );

/** \fn int Request_Execute( Request* Self )
 *  \brief Starts the execution, waits until completed and sends the result back to the requester.
 *
 *  The real call to the stored function happens here. It takes the funktion pointer of the stored
 *  command and passes it the parameter. After the function returns it adds the returned string
 *  to the queue of the Sender.
 *  \Note: This function will block until the called function returns.
 *
 *  \param Self is a pointer to a initialized Request instance.
 *
 *  \return -1 on error, 0 on success.
 *
 */
int Request_Execute( Request* Self );

/** \fn int Request_Destroy( Request* Self )
 *  \brief Removes a Request and frees it's memory.
 *
 *  \param Self is a pointer to a valid Request instance
 *
 *  \return -1 on error, 0 on success.
 *
 */
int Request_Destroy( Request* Self );

/*
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

*/

/** \typedef struct socketrequest SocketRequest 
 *  Use SocketRequest instead of struct socketrequest.
 */

/** \struct socketrequest
 *  \brief A SocketRequest is a list of commands from one client ordered in a queue.
 *
 *  The SocketRequest wraps the management of the Requests for one client. It contains
 *  the handle to the thread that belongs to this client and and queue with the waiting
 *  commands. The RequestList is built up on elements of SocketRequests. After the last
 *  Request is executed the Thread is closed and the SocketRequest destroyed. 
 *
 *
 */
typedef struct socketrequest SocketRequest;
struct socketrequest
{
  //@{
  /// Socket is the identification of the requester.
  int Socket;
  /// Thread is the handle to the running thread.
  pthread_t Thread;
  /// ReqQueue contains the waiting Request instances.
  TQueue* ReqQueue;
  /// Previous points to the previous SocketRequest in the list.
  SocketRequest* Previous;
  /// Next points to the next SocketRequest in the list.
  SocketRequest* Next;
  //@}	
};

/** \fn SocketRequest* SocketRequest_Init( int Socket )
 *  \brief Creates a new SocketRequest instance and thread.
 *
 *  This function creates an empty SocketRequest with a new thread
 *  that waits until the first Request is put in the queue.
 * 
 *  \param Socket is the identification of the client.
 *
 *  \return A newly created SocketRequest handle.
 *
 */
SocketRequest* SocketRequest_Init( int Socket );

/** \fn int SocketRequest_AddRequest( SocketRequest* Self, NetFunction* Func, NetData* Data, Sender* Send )
 *  \brief Creates a new Request and adds it to the queue.
 *
 *  This function creats a new Request instance and puts it on the bottom of the queue. The Data object
 *  must remain as it is, because this function does not copy it. 
 * 
 *  \param Self is a valid SocketRequest handle.
 *  \param Func is the corresponding NetFunction instance.
 *  \param Data is the received command with parameters.
 *  \param Send is a valid Sender handle.
 *
 *  \return -1 on error, 0 on success
 *
 */
int SocketRequest_AddRequest( SocketRequest* Self, NetFunction* Func, NetData* Data, Sender* Send );

/** \fn void SocketRequest_Execute( void* data )
 *  \brief This is the thread that is started at creation of a new SocketRequest.
 *
 *  \param data must be a pointer to the according SocketRequest instance.
 *
 */
void SocketRequest_Execute( void* data ); // data must be a SocketRequest

/** \fn
 *  \brief
 *
 *  \param
 *
 *  \return
 *
 */
int SocketRequest_Destroy( SocketRequest* Self );

/** \typedef struct requestlist RequestList
 *  Use RequestList instead of struct requestlist.
 */

/** \struct requestlist
 *  \brief This list contains all queues. Every client has it's own.
 * 
 *
 */
struct requestlist
{
  Sender* Send;

  SocketRequest* First;
  SocketRequest* Last;
  int Length;

  /*
  CliRequest* CliFirst;
  CliRequest* CliLast;
  int CliLength; 
  */

  // User Management Interface could be implemented here 
};
typedef struct requestlist RequestList;

/** \fn
 *  \brief
 *
 *  \param
 *
 *  \return
 *
 */
RequestList* RequestList_Init( Sender* Send );

/** \fn
 *  \brief
 *
 *  \param
 *
 *  \return
 *
 */
int RequestList_AddRequest( RequestList* Self, NetFunction* Func, NetData* Data );

/** \fn
 *  \brief
 *
 *  \param
 *
 *  \return
 *
 */
int RequestList_RemoveRequest( RequestList* Self, int Socket );

/** \fn
 *  \brief
 *
 *  \param
 *
 *  \return
 *
 */
int RequestList_Destroy();

/** \fn
 *  \brief
 *
 *  \param
 *
 *  \return
 *
 */
SocketRequest* RequestList_RawSocketExists( RequestList* Self, int Socket );
#endif
