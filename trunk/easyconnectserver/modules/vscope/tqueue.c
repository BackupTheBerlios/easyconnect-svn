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
 */

#include "tqueue.h"

TQueue* TQueue_Init()
{
  TQueue* Self = ( TQueue* ) malloc( sizeof( TQueue ));
  Self->Length = 0;
  Self->First = NULL;
  Self->Last = NULL;
  
  pthread_mutex_init( &Self->QueueLock, NULL );

  pthread_mutex_init( &Self->QueueCondLock, NULL );
  pthread_cond_init( &Self->QueueCond, NULL );
  
  return Self;
}

// This function adds an element at the bottom of the list
// Blocks until it can write
int TQueue_AddElement( TQueue* Self, void* Element )
{
  if( Self == NULL || Element == NULL )
    return -1;

  // Locking Queue for writing  
  pthread_mutex_lock( &Self->QueueLock );
  
  
  // Create Element
  TQueue_Node* TempNode = ( TQueue_Node* ) malloc( sizeof( TQueue_Node ) );
	
  if( TempNode == NULL ) 
  {
    return -1;
  }
  
  // Save old last element
  // Copy the new element on the bottom
  TQueue_Node* Last = NULL;
  if( Self->Last != NULL )
  {
    Last = Self->Last;
    Last->Next = TempNode; 
  }
	
  if( Self->First == NULL )
    Self->First = TempNode;
	
  TempNode->Element = Element;
  Self->Length++;
  TempNode->Next = NULL;
  Self->Last = TempNode;
  
  // Signal waiting threads
  pthread_cond_signal( &Self->QueueCond );
  
  pthread_mutex_unlock( &Self->QueueLock );
  return 0;
  
}

// This function returns an element from the top of the list
void* TQueue_GetElement( TQueue* Self )
{
  if( Self == NULL )
    return NULL;
 
  // First lock to get number of elements 
  pthread_mutex_lock( &Self->QueueLock );
  int Length = Self->Length;
  pthread_mutex_unlock( &Self->QueueLock );
  
  if( Length == 0 )
  {
    // Wait if no element available
    pthread_mutex_lock( &Self->QueueCondLock );
    pthread_cond_wait( &Self->QueueCond, &Self->QueueCondLock );
    pthread_mutex_unlock( &Self->QueueCondLock );
  }
 
  // Test if just wakup call
  if( Self->Length == 0 )
    return NULL; 
  
  //Second lock to get the element
  pthread_mutex_lock( &Self->QueueLock );
  TQueue_Node* TempNode;
  void* Element;
  TempNode = Self->First; 
  Self->First = TempNode->Next;
  Self->Length--;
  if(Self->Length == 0)
    Self->Last = NULL;
  
  Element = TempNode->Element;
  free(TempNode);
	
  pthread_mutex_unlock( &Self->QueueLock );
  return Element;
}


// Trying to get an element returs NULL if emty
void* TQueue_TryGetElement( TQueue* Self )
{
  if( Self == NULL || Self->First == NULL )
    return NULL;
	 
  // First lock to get number of elements 
  pthread_mutex_lock( &Self->QueueLock );
  int Length = Self->Length;
  pthread_mutex_unlock( &Self->QueueLock );
 
  // Return if nothing was found 
  if( Length == 0 )
  {
    return NULL;
  }
  

  // Get element if something is there
  pthread_mutex_lock( &Self->QueueLock );
  TQueue_Node* TempNode;
  void* Element;
  TempNode = Self->First; 
  Self->First = TempNode->Next;
  Self->Length--;
  if(Self->Length == 0)
    Self->Last = NULL;
  
  Element = TempNode->Element;
  free(TempNode);
	
  pthread_mutex_unlock( &Self->QueueLock );
  return Element;
}


int TQueue_Destroy( TQueue* Self )
{
  if(Self->Length > 0)
    return -1;

  pthread_mutex_destroy( &Self->QueueLock );
  pthread_mutex_destroy( &Self->QueueCondLock );
  pthread_cond_destroy( &Self->QueueCond );
  
  free(Self);
  return 0;
}


int TQueue_WakeQueue( TQueue* Self )
{
  pthread_cond_signal( &Self->QueueCond );
  return 0;
}
