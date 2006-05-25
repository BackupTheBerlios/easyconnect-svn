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

#include "network.h"

Network* Network_Init( char* Ip, int Port, int Type, int BuffSize, TQueue* Queue )
{
  Network* Self = (Network*) malloc( sizeof( Network ) );
  Self->Type = Type;
  Self->Port = Port;
  Self->BuffSize = BuffSize;
  Self->Buffer = (char*) malloc( sizeof( char )* BuffSize ); 
 
  Self->Ip = (char*) malloc( sizeof( char ) * ( strlen( Ip ) + 1 ) ); 
  strcpy( Self->Ip, Ip );
  
  Self->Listener = (pthread_t*) malloc( sizeof (pthread_t) );
  Self->InQueue = Queue;
  
  socklen_t AddressLength = sizeof( struct sockaddr_in );
  const int x = 1;

  if(( Self->SockFD = socket( PF_INET, SOCK_STREAM, 0 )) < 0 )
  {
    // ERROR: with socket
    // printf(" ERROR: %s\n", strerror(errno));
    return NULL;
  }

  Self->Address.sin_family = AF_INET;
  Self->Address.sin_port = htons( Self->Port );

  memset( &Self->Address.sin_addr, 0, sizeof( Self->Address.sin_addr ));

  setsockopt( Self->SockFD, SOL_SOCKET, SO_REUSEADDR, &x, sizeof( int ));

  if( bind( Self->SockFD, (struct sockaddr*) &Self->Address, sizeof( Self->Address )))
  {
    // ERROR: at binding
    // printf(" ERROR: %s\n", strerror(errno));
    return NULL;
  }
  if( listen( Self->SockFD, SOMAXCONN ) != 0 ) 
  {
    // ERROR: at listen() 
    // printf(" ERROR: %s\nExit listen Thread!\n", strerror(errno));
    return;
  }   

  if( Self->Type == CLI_MODE )
  {
  
    pthread_create( Self->Listener, NULL, (void*)&Network_CliListener, (void*) Self );
    
  } else if( Self->Type == RAW_MODE )
  {
  
    pthread_create( Self->Listener, NULL, (void*)&Network_RawListener, (void*) Self );
    
  } else
  {
    // Not right type
    return NULL;
  }
  
  
}

void Network_CliListener( void* data )
{
  Network* Self = ( Network* ) data;
  int ConnFD;
  int i, n, retval;
  
  NetData* Incoming;
  socklen_t AddrLength = sizeof( struct sockaddr_in );

  int FdMax = Self->SockFD;
  fd_set master;
  fd_set fdlist;
 
  struct sockaddr_in RemoteAdress;
    
  FD_ZERO( &fdlist );
 
  FD_SET( Self->SockFD, &master );

  if( listen( Self->SockFD, SOMAXCONN ) != 0 ) 
  {
      // ERROR: at listen() 
      // printf(" ERROR: %s\nExit listen Thread!\n", strerror(errno));
      return;
  }  

  while( 1 ) // While thread is running
  {
    fdlist = master;

    retval = select( FdMax+1 , &fdlist, NULL, NULL, NULL);
    pthread_testcancel(); 
    if( retval == -1 )
    {
      // ERROR: at select()
      // printf ....
    } 
    // Look which socket was selected
    for( i = 0; i <= FdMax; i++ )
    {
      // Because it has to be in the fdlist if it was selected
      if( FD_ISSET( i, &fdlist ))
      {
	// New Connection
	if( i == Self->SockFD )
	{
	  ConnFD = accept( Self->SockFD,
			   (struct sockaddr*) &Self->Address,
			   &AddrLength);
     
	  FD_SET(ConnFD, &master);
	  FdMax = ConnFD;
	  char* negotiate = "\xFF\xFB\x03"
		            "\xFF\xFB\x01"
			    "\xFF\xFD\x03"
			    "\xFF\xFD\x01";
  	 
	  send( ConnFD, negotiate, strlen(negotiate), 0 ); 
	  
	} else
	{ // received data:
	  n = recv( i, Self->Buffer, Self->BuffSize, 0 );
	   
	  // no data received
	  if( n <= 0 )
	  {
	    // close connection
		  
	    //printf("Client Disconnected.\n");
	    close( i );
	    FD_CLR( i, &master );
	  } else
	  {
	    // This is a dirty hack for telnet
	    // TODO: catch the first strings after connection and do NOT
	    // echo them 
	    if( n > 3 )
	      continue;

	    Self->Buffer[n] = 0;
	    //send( i, Self->Buffer, n, 0 );
	    Incoming = NetData_Init( Self->Buffer, i, CLI_MODE );
	    // Add Data to queue
	    TQueue_AddElement( Self->InQueue, (void*) Incoming );

	    //printf("%s\n",Self->Buffer);
	  }
	}
      }
    }
  }

  return;
}


void Network_RawListener( void* data )
{
  Network* Self = ( Network* ) data;
  int ConnFD;
  int i, n, retval;
  
  NetData* Incoming;
  socklen_t AddrLength = sizeof( struct sockaddr_in );

  int FdMax = Self->SockFD;
  fd_set master;
  fd_set fdlist;
 
  struct sockaddr_in RemoteAdress;
    
  FD_ZERO( &fdlist );
  FD_ZERO( &master );
 
  FD_SET( Self->SockFD, &master );

  if( listen( Self->SockFD, SOMAXCONN ) != 0 ) 
  {
      // ERROR: at listen() 
      // printf(" ERROR: %s\nExit listen Thread!\n", strerror(errno));
      return;
  }  

  while( 1 ) // While thread is running
  {
    fdlist = master;

    retval = select( FdMax+1 , &fdlist, NULL, NULL, NULL);

    pthread_testcancel(); 

    if( retval == -1 )
    {
      // ERROR: at select()
      // printf ....
    } 
    // Look which socket was selected
    for( i = 0; i <= FdMax; i++ )
    {
      // Because it has to be in the fdlist if it was selected
      if( FD_ISSET( i, &fdlist ))
      {
	// New Connection
	if( i == Self->SockFD )
	{
	  ConnFD = accept( Self->SockFD,
			   (struct sockaddr*) &Self->Address,
			   &AddrLength);
     
	  FD_SET(ConnFD, &master);
	  FdMax = ConnFD;
	  
	  // Here be welcome message!
	
	} else
	{ // received data:
	  n = recv( i, Self->Buffer, Self->BuffSize, 0 );
	  
	  // no data received
	  if( n <= 0 )
	  {
	    // close connection
		  
	    //printf("Client Disconnected.\n");
	    close( i );
	    FD_CLR( i, &master );
	  } else
	  {
	    
	    if( Self->Buffer[n-1] == '\r' || Self->Buffer[n-1] == '\n' )
	      Self->Buffer[n-1] = 0;
	    
	    if( Self->Buffer[n-2] == '\r' || Self->Buffer[n-2] == '\n' )
	      Self->Buffer[n-2] = 0;
	    
	    Self->Buffer[n] = 0;
	    Incoming = NetData_Init( Self->Buffer, i, RAW_MODE );
	    // Add Data to queue
	    TQueue_AddElement( Self->InQueue, (void*) Incoming );
	    //printf("%s",Self->Buffer);
	  }
	}
      }
    }
  }

  return;
}

int Network_Destroy( Network* Self )
{
  if( Self == NULL )
  {
    return -1;
  }
  
  pthread_cancel( *Self->Listener );
  // TODO: Are there more sockets to close?
  close( Self->SockFD );
  free( Self->Buffer );
  free( Self );
}
