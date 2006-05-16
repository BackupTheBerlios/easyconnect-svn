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

#ifndef __NETDATA_H
#define __NETDATA_H

#include <string.h>
#include <stdlib.h>

/** \file netdata.h
 *  \brief Contains the NetData struct.
 */


/** \enum ConnMode 
 *  Describes the two possible modes. 
 *  Cli = CLI_MODE
 *  Raw = RAW_MODE
 */
enum ConnMode { CLI_MODE, RAW_MODE };

/** \typedef typedef struct netdata NetData
 *  Use NetData instead of struct netdata.
 
 * \struct netdata
 * \brief This object is used to transport
 * information as strings from and to the network. 
*/
typedef struct netdata NetData;
struct netdata
{
  //@{
  /// Contains a zero terminated string
  char* Data;
  /// The socket, used as target or source
  int Socket;
  /// Can be either CLI_MODE or RAW_MODE
  int Type;
  //@}
};

/** \fn NetData* NetData_Init( char* Data, int Socket, int Type )
 *  \brief Initialize function for NetData. Allocates memory for the object.
 *  \param Data should be a zero terminated string. The "payload" so to speak.
 *  \param Socket is the sender or receiver, depends on the direction.
 *  \param Type should be either CLI_MODE or RAW_MODE
 *  \return A pointer to the initialized NetData object. 
 * 
 */
NetData* NetData_Init( char* Data, int Socket, int Type );

/** \fn NetData* NetData_Destroy( NetData* Self )
 *  \brief Destroy function for NetData. Removes object from memory.
 *  \param Self is a pointer to a NetData object. 
 *  \return An integer which is zero if successful 
 * 
 */
int NetData_Destroy( NetData* Self );

#endif
