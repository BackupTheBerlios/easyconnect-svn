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
 *  Author:
 *  Moritz Eberl <meberl@vscope.de>
 * 
 */
 
#ifndef _TQUEUE_H
#define _TQUEUE_H

#include <stdlib.h>
#include <pthread.h>

typedef struct node__ TQueue_Node;
struct node__
{
  TQueue_Node* Next;
  void* Element;	
};


/** \file tqueue.h
 * \brief Contains the Threadsafe Queue.
 * 
 * The TQueue is a Threadsafe queue. Every writing and reading action is
 * protected by a Mutex. AddElement puts a new element at the end of the queue
 * and sends a signal to the condition variable of the queue.
 * GetElement removes the first entry from the queue and returns it.
 * If the queue is empty it waits on the condition variable until
 * a new element arrives.
 * The queue stores void pointers, so you have to cast it back on the old type
 * after retrieving it.
 */


/** \typedef typedef struct tqueue TQueue
 * Use TQueue instead of struct tqueue.
 */
 
/** \struct tqueue
 * \brief Use TQueue instead of struct tqueue.
 * This is the queue object. It is needed for the whole queue management. 
 * 
 * The queue functions need a pointer to a initialized tqueue object. This 
 * means that you should always use a pointer on a queue which was returned
 * by the proper Init function. If you do not need the queue anymore be sure 
 * to call the Destroy function, which not just frees the memory, but also
 * removes the mutexes properly. Note: Remove the elements from the queue first,
 * or destroy will fail.
 */
typedef struct tqueue TQueue;	
struct tqueue
{
  //{@
  /// Pointer to the first element in the queue. 
  TQueue_Node* First;
  /// Pointer to the last element in the queue.
  TQueue_Node* Last;
  /// Number of elements in the queue.
  int Length;

  /// Mutex to lock the queue.
  pthread_mutex_t QueueLock;

  /// Mutex for the condition variable
  pthread_mutex_t QueueCondLock;
   
  /// Condition variable on which the GetElement function waits.
  pthread_cond_t QueueCond;
  //@} 
};


/** \fn TQueue* TQueue_Init()
 *  \brief Initialize function for the TQueue object. 
 *  
 *  This function is used to create a new queue. It reserves
 *  the memory for a new TQueue instance, sets all values to zero
 *  and initializes the mutexes and the condition variable.
 *  You need to call this function before working with a queue.
 *  
 *  \return A pointer to a TQueue instance.
 */
TQueue* TQueue_Init();

/** \fn int TQueue_AddElement( TQueue* Self, void* Element )
 *  \brief Adds an element to the bottom of the Queue.
 *
 *  This function will lock the queue, create a new node for the
 *  given element pointer and add it to the bottom of the queue.
 *  Then it increases the length counter.
 *  \Attention This function blocks until it gained the lock of
 *  the queue.
 *
 *  \param Self is a pointer to a valid queue.
 *  \param Element is a void pointer to the element which is to 
 *  be added to the queue. 
 *
 *  \return It returns 0 if successful. If return value is -1, either the 
 *  Parameters were NULL or the allocation for the node failed.
 *  
 */
int TQueue_AddElement( TQueue* Self, void* Element );

/** \fn void* TQueue_GetElement( TQueue* Self )
 *  \brief This function returns first element and waits if none is available.
 *
 *  First it will check for the length of the queue. If zero it blocks until
 *  an element is added. If greater than zero it removes the element from
 *  the queue, decreases the counter, and returns the void pointer to the
 *  stored element.
 *  \Attention If you do not want the function to block, use the TryGetElement
 *  function instead.
 *
 *  \param Self is the queue from which you want to get the first element.
 *
 *  \return Void pointer to the stored element. You have to cast it back to
 *  the type it was before it was added to the queue. 
 * 
 */
void* TQueue_GetElement( TQueue* Self );

/** \fn void* TQueue_TryGetElement( TQueue* Self )
 *  \brief This function returns first element and returns if none is available.
 *
 *  First it will check for the length of the queue. If zero it returns NULL.
 *  If greater than zero it removes the element from the queue, decreases the
 *  counter, and returns the void pointer to the stored element.
 *  \Attention If you want the function to block, use the GetElement
 *  function instead.
 *
 *  \param Self is the queue from which you want to get the first element.
 *
 *  \return Void pointer to the stored element. You have to cast it back to
 *  the type it was before it was added to the queue. 
 * 
 */
void* TQueue_TryGetElement( TQueue* Self );


/** \fn void* TQueue_Destroy( TQueue* Self )
 *  \brief This function removes a instanciated TQueue object.
 *  
 *  This function should be called when the queue is not needed any more.
 *  It will be removed from memory. Afterwards the passed pointer is invalid.
 *  \Attention Do not try to remove a queue with a length greater zero. It will fail.
 *   
 *  \param Self is the queue you want to be removed.
 *
 *  \return The integer is zero if successful, -1 if not. 
 */
int TQueue_Destroy( TQueue* Self );

int TQueue_WakeQueue( TQueue* Self );

#endif /* _TQUEUE_H */
