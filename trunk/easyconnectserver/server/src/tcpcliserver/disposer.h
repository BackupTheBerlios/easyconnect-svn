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
//#include "cl/cl.h"

/**  \file disposer.h
 *   \brief matches and executes queued commands.
 *
 *   The Disposer is a asynchronous execution facility. It stores 
 *   registered commands and respective function pointer in a 
 *   NetFunctionList. 
 *   A thread gets the commands from the queue, vergleicht them with
 *   the stored commands, and if found it puts them in the RequestList.
 *   The execution of the functions in the RequestList works asynchronous.
 *   The Disposer doesn't have to wait until the command is executed. 
 *   
 *
 */

/** \typedef typedef struct disposer Disposer
 *  Use Disposer instead of struct disposer.
 */

/**  \struct disposer
 *   \brief The Disposer matches incoming command with stored ones.
 *
 *   Use Disposer_Init() to create a new Disposer instance.
 *   To connect it properly to a network, it needs the queue where
 *   the data from the network is put. It also needs a handle of a
 *   initialized Sender instance. The thread will be started during
 *   the initialization.
 *   Use the Disposer_RegisterFunction() to store a command with the
 *   appropriate function pointer to make the function be callable.
 *   The function must have a parameter list like this, char*, int, void*.
 *   The char pointer is a zero terminated string. It contains the complete string
 *   which triggered the execution, until the first zero. Parameter will be
 *   included as long as they where not after a terminating zero. They
 *   have to be parsed by the called function.
 *   The int is the socket that sent the data. 
 *   The void pointer is a parameter that can be chosen at registering.
 *   It is the same pointer as the fourth parameter of the Dispoer_Register Function.
 *   The return value must be a valid zero terminated string or NULL. 
 *   This string will be sent back to the caller. 
 *   
 *   The default message if the command was not found can be chosen with Disposer_SetDefaultErrorMsg().
 *   
 *
 */
typedef struct disposer Disposer;
struct disposer 
{
  //@{
  /// Contains the stored commands and respective function pointer.
  NetFunctionList* FuncList;
  /// Contains the queue with the matched commands. They are queued per socket.
  RequestList* ReqList;
  /// The incoming data from the network stored in a queue.
  TQueue* InQueue;
  /// The Sender instance. Needed to return the results.
  Sender* Send;
  /// The handle of the thread that matches the incoming commands with the stored ones.
  pthread_t Thread;

  //Cl* CommandLine;
  
  /// The default error message as zero terminated string.
  char* ErrorMsg;
  //@}
};

/** \fn Disposer* Disposer_Init( TQueue* InQueue, Sender* Send )
 *  \brief This function initalizes a Disposer. Use it to properly create one.
 *
 *  Use this function to initialize a Disposer. It returns a valid handle. Afterwards you can
 *  use it to register functions and set the error msg.
 * 
 *  \param InQueue is a threadsafe queue. It should be the same one as the one from the Network instance.
 *  \param Send is a handle of a initialized Sender object.
 *  \return A initialized Disposer instance with a started and waiting thread.
 */
Disposer* Disposer_Init( TQueue* InQueue, Sender* Send );	

/** \fn int Disposer_RegisterFunction( Disposer* Self, char* Command, char*( *Callback )( char*, int, void* ), void* Parameter )
 *  \brief Registers a command and it's according function pointer.
 *
 *  This function stores the string that represents the command with the pointer to the function in a RequestList object. Additionaly you can store a parameter with it. /Attention The disposer is case sensitiv, so the received command must match the stored one exactly. 
 * 
 *  \param Self is a valid Disposer handle.
 *  
 *  \param Command is zero terminated string containing the command.
 *  
 *  \param Callback is the pointer to the function you want to be called. 
 *  Return value must be a char pointer.
 *  The parameters must be char pointer, int and void pointer.
 *  The first is the string that triggered the call.
 *  The second is the socket that sent the command.
 *  The third is the choosable parameter. 
 *  
 *  \param Parameter is a pointer to an arbitrary object. You can use this to pass your data to
 *  the callback.
 *  
 *
 *  \return If the operation was successfull it returns -1, if not it returns zero.
 */
int Disposer_RegisterFunction( Disposer* Self, char* Command, char*( *Callback )( char*, int, void* ), void* Parameter );

/** \fn int Disposer_SetDefaultErrorMsg( Disposer* Self, char* ErrorMsg )
 *  \brief Stores a new default error message.
 *
 *  Sets the message that is sent back if the specified command was not found.
 *  If no default error message is set, nothing is sent back.
 *  It makes a copy of the supplied string, so you can safely free your string 
 *  after passing.
 *
 *  \param Self is a valid Disposer handle.
 *  \param ErrorMsg is a zero terminated string containing the new error message.
 *  
 *
 *  \return If the operation was successfull it returns -1, if not it returns zero.
 */
int Disposer_SetDefaultErrorMsg( Disposer* Self, char* ErrorMsg );

/** \fn int Disposer_HandleCli( Disposer* Self, NetData* Received )
 *  \brief Handles special commandline operations like sending a prompt.
 *
 *  This function wraps the commandline functionality. Currently it is not working.
 *  It is normaly used internally in the Disposer thread.
 *
 *  \param Self is a valid Disposer handle.
 *  \param Received is the NetData object that was received from the network.
 *  
 *
 *  \return Returns -1 on Error, 0 on success.
 */
int Disposer_HandleCli( Disposer* Self, NetData* Received );

/** \fn int Disposer_Echo( Disposer* Self, int Socket, char* Data )
 *  \brief Echoes back the sent character. Used for the commandline interface.
 *
 *  This function sends the receieved data right back to the sender.
 *  The local echo of the telnet client is turned off, so we have full control
 *  over the output. This also means that we have to echo the sent data back.
 *  It is normaly used internally in the Disposer thread.
 *
 *  \param Self is a valid Disposer handle.
 *  \param Socket is the socket to which we echo back.
 *  \param Data is the zero terminated string which contains the sent data. 
 *
 *  \return Returns -1 on Error, 0 on success.
 */
int Disposer_Echo( Disposer* Self, int Socket, char* Data );

/** \fn void Disposer_Thread( void* data )
 *  \brief The thread does the matching of the incoming and stored commands.
 *
 *  On startup it will wait until the first element is put in the queue. 
 *  The list with the stored commands will be empty on beginning, nothing
 *  will happen if a command is received.
 *  This function is not called normaly, it is used to start the thread.
 *
 *  \param data is a pointer to a Disposer casted to void*. 
 * 
 */
void Disposer_Thread( void* data );

/** \fn char* GetCommand( char* String )
 *  \brief Filters the command from a received string. 
 *
 *  This function takes a string is a format like "Command Foo Bar" and 
 *  returns a copy of the command, in this case "Command".
 * 
 *  \return A copy of the command.
 */
char* GetCommand( char* String );

#endif
