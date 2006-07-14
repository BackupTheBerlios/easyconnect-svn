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

/** \file sender.h
 * \brief Contains the wrapper for sending data.
 *
 * The Sender queues the data and sends it to the according connection/socket.
 * The data is represented by a NetData object, containing the Socket identifier, the type of the message
 * and the message as 0 terminated string. 
 * The Sender works asynchronous, this means a thread will be created on initalization.
 * 
 */

/** \typedef typedef struct sender Sender 
 *  Use Sender instead of sender.
 */
typedef struct sender Sender;

/** \struct sender
 *  \brief This is the sender struct. 
 *  Use Sender instead of struct sender. 
 *  A initialized instance contains the thread handle and the queue handle.
 *
 *  Pass a instance to this struct to the functions. Always use the Sender_Init function to create
 *  a instance. 
 *
 *  There are two possibilites to add data to the Queue. You can pass the 
 *  data, the socket and the type to the Sender_AddMessage() function, or you create a NetData 
 *  instance and pass it to Sender_AddNetData().
 *  
 *  \Attention The Sender will try to destroy the NetData. So be
 *  sure that you don't use it after passing it to the
 *  Sender. Dont't destroy it afterwards either!
 *  The message string will be copied if you use the Sender_AddMessage() function.
 *  
 *  The Sender_Destroy function will terminate the thread and free allocated memory, so
 *  use it to deinitalize the instance if you don't need it anymore. The queue will not be
 *  destroyed. 
 */
struct sender
{
  //@{
  /// The handle to the threadsafe queue.
  TQueue* Out;
  /// The handle for the sending thread.
  pthread_t SendThread;
  //@}
};

/** \fn Sender* Sender_Init( TQueue* OutQueue )
 *  \brief Initalization function for the Sender object.
 *
 *  Use this function to create a Sender instance. It reserves 
 *  the needed memory and creates a new thread with the Sender_Thread function.
 *  
 *  \param OutQueue is the handle to an existing threadsafe queue.
 *
 *  \return The return value is a pointer to a sender instance. If something went wrong
 *  it returns NULL.
 *
 */
Sender* Sender_Init( TQueue* OutQueue );

/** \fn int Sender_AddMessage( Sender* Self, char* Msg, int Socket, int Type )
 *  \brief Puts a message on the end of the queue. 
 *
 *  As the parameter list shows, this function enables you to add a message without the usage of a
 *  NetData object. The Message will be put on the bottom of the queue. If the
 *  queue was emty the thread will be woken up.
 *  \Attention You have to free the Msg string. It will be copied by Sender_AddMessage!
 *  
 *  \param Self is a pointer to a initalized Sender instance.
 *
 *  \param Msg is a 0 terminated String, containing the message you want to be sent.
 *  \param Socket is the identifier for the recipient.
 *  \param Type is used to determine how the recipient is connected. Can be CLI_MODE or RAW_MODE. 
 *
 *  \return The result is -1 on failure, 0 on success.
 *
 *
 */
int Sender_AddMessage( Sender* Self, char* Msg, int Socket, int Type );

/** \fn int Sender_AddNetData( Sender* Self, NetData* Data )
 *  \brief Adds a NetData object to the bottom of the queue.
 *
 *  This functions adds a NetData object to the bottom of the Queue.
 *  It needs a initalized object. 
 *  \Attention The NetData will be destroyed after sending. If you want to
 *  use it afterwards, pass a copy.
 *
 *  \param Self is a pointer to a initalized Sender instance.
 *  \param Data is a pointer to a NetData instance containing the Data.
 */  
int Sender_AddNetData( Sender* Self, NetData* Data );
	
void Sender_Thread( void* data );

void Sender_Destroy( Sender* Self );

#endif
