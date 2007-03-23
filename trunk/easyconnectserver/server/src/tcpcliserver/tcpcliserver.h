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

/** \file tcpcliserver.h
 *  \brief Contains the main interface for the TcpCliServer.
 *
 *  The TcpCliServer is the main wrapper. The functions defined here are the public interface
 *  that should be used. First initialize the TcpCliServer to get a instance. 
 *  Then you can register your functions with the server and set the default error. 
 *  After that Initialize one or both interfaces, Cli or Raw, with the ip and the port.
 *  The server should now be up and running. 
 *
 */

/** typedef struct tcpcliserver TcpCliServer
 * Use TcpCliServer instead of struct tcpcliserver.
 */

/** \struct tcpcliserver
 *  \brief This is the main handle for the TcpCliServer.
 *
 *  This struct contains the whole information needed for the TcpCliServer. On initialization it
 *  initializes its queues and the disposer.
 *  Then the functions can be registered, and the Network instances initialized.
 *  There are three possibilities to use the server. 
 *  
 *  First you can use it as pure data server. Bringing up the raw network with TcpCliServer_InitRaw()
 *  lets other written programs connect with realtive ease to the server. They just need to separete their
 *  commands with a newline (\n)
 *  
 *  Second, just use the commandline with TcpCliServer_InitCli(). This enables user to connect to the server
 *  with telnet. They get a history and auto completion for comfortable use.
 *
 *  The third possiblity is to use both mechanisms. The only drawback is that you have to use a different
 *  port for both interfaces. Using the same will fail the operation.
 *  
 *
 */
typedef struct tcpcliserver TcpCliServer;
struct tcpcliserver
{
  //@{
  /// This is the queue which contains incoming mesasges.
  TQueue*   InQueue; 
  /// This queue contains outgoing messages.
  TQueue*   OutQueue; 
  /// This Network instance manages the commandline interface connections from telnet.
  Network*  CliNet;
  /// This Network instance manages the raw data from connected clients.
  Network*  RawNet;
  /// This Sender instance sends all outgoing messages.
  Sender*   Send;
  /// The Disposer matches the incoming messages with stored functions and executes them if one was found.
  Disposer* Disp; 
  //@}
};

/** \fn TcpCliServer* TcpCliServer_Init()
 *  \brief Creats a new TcpCliServer instance. 
 *
 *  Call this function to gain a handle to a new TcpCliServer. It is completly unspoiled. The network interface
 *  is not brought up, this means is has not yet bound itself to a specific ip or port. The functionlist is 
 *  empty and the error message not set.
 *
 *  \return A handle to a new TcpCliServer.
 */
TcpCliServer* TcpCliServer_Init();

/** \fn int TcpCliServer_InitCli( TcpCliServer* Self, char* Ip, int Port, int BufferSize )
 *  \brief This function brings the command line interface up.
 *
 *  Call this function to enable the command line. After this function call, you can use telnet to communicate
 *  with the server. 
 *  \Note Currently under developement and not working.
 * 
 *  \param Self is a valid TcpCliServer handle.
 *  \param Ip is a zero terminated string containing the net adress to which the server should bind this interface.
 *  \param Port is the port number which should be used.
 *  \param BufferSize is the size of the receive buffer.
 *
 *  \return -1 on error, 0 on success
 */
int TcpCliServer_InitCli( TcpCliServer* Self, char* Ip, int Port, int BufferSize );

/** \fn int TcpCliServer_InitRaw( TcpCliServer* Self, char* Ip, int Port, int BufferSize )
 *  \brief This function brings the raw interface up.
 *
 *  To activate the raw data interface call this function. You can use it parallel to TcpCliServer_InitCli() as long as you
 *  use a different port. 
 *
 *  \param Self is a valid TcpCliServer handle.
 *  \param Ip is a zero terminated string containing the net adress to which the server should bind this interface.
 *  \param Port is the port number which should be used.
 *  \param BufferSize is the size of the receive buffer.
 *
 *  \return -1 on error, 0 on success
 */
int TcpCliServer_InitRaw( TcpCliServer* Self, char* Ip, int Port, int BufferSize );

/** \fn int TcpCliServer_RegisterFunction( TcpCliServer* Self, char* Command, char*( *Callback )(char*, int, void* ), void* Parameter )
 *  \brief Pass a pointer to a function and a command you want to trigger the execution of that function.
 *
 *  This function makes the TcpCliServer aware of your function and the triggering command. The first parameter of the callback
 *  is the string which caused the function call, the second is the socket, and the third is a parameter you can specify. 
 *  Its the Parameter you pass to this function. 
 *  
 *  e.g. You Registerd a function called "Run" pointing to the function "char* ExecuteFunction( char*, int, void* )".
 *  As parameter you passed a pointer to your struct "Environment".
 *  After connecting, a client with the socket number 3 submits a string "Run Thing" the function ExecuteFunction() will be called
 *  with parameters like this ExecuteFunction("Run Thing", 3, &Environment).
 *  The returning result "Went ok.\n" from that callback will be sent back to the client.
 *  
 *  As you can see from this example, to process the submitted parameters you just have to parse the submitted string
 *  in parameter number one of your callback.
 *
 *  \param Self is a valid TcpCliServer instance
 *  \param Command is a zero terminated string containing the command.
 *
 *  \return -1 on error, 0 on success
 */
int TcpCliServer_RegisterFunction( TcpCliServer* Self, char* Command, char*( *Callback )(char*, int, void* ), void* Parameter );

/** \fn int TcpCliServer_SetDefaultError( TcpCliServer* Self, char* ErrorMsg )
 *  \brief Sets the string which will be returned if no function was found.
 *
 *  After checking all stored commands the server would return NULL if the submitted was not found.
 *  This means the connected user will not get any feedback if his command is still processing or 
 *  it was not found in the first place. To counter this behaviour you can set a default error with
 *  this function.
 *
 *  \param Self is a valid TcpCliServer instance.
 *  \param ErrorMsg is a zero terminated string containing the error message that should be returned as default.
 *
 *  \return -1 on error, 0 on success
 */
int TcpCliServer_SetDefaultError( TcpCliServer* Self, char* ErrorMsg );

/** \fn void TcpCliServer_Destroy( TcpCliServer* Self )
 *  \brief Use this function to deinitialize the server.
 *
 *  This function stops the network interfaces, deletes all waiting requests, removing stored commands and frees
 *  the memory that was allocated. Every started thread will be canceled.
 *
 *  \param Self is a valid TcpCliServer instance.
 */
void TcpCliServer_Destroy( TcpCliServer* Self );

#endif
