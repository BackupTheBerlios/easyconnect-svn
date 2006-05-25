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

#include "netdata.h"

NetData* NetData_Init( char* Data, int Socket, int Type )
{
  if( Data == NULL )
    return NULL;
  
  NetData* Self = ( NetData* ) malloc( sizeof( NetData ));
  Self->Socket = Socket;
  Self->Type = Type;

  Self->Data = (char*) malloc( sizeof( char ) * ( strlen( Data ) + 1) ) ;
  strcpy( Self->Data, Data );

  return Self;
}

int NetData_Destroy( NetData* Self )
{
  if( Self == NULL )
  {
    return -1;
  }
  
  if( Self->Data != NULL )
  {
    free( Self->Data );
  }
  free( Self );

  return 0;
}

