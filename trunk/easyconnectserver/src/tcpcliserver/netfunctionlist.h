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

#ifndef __NETFUNCTIONLIST_H
#define __NETFUNCTIONLIST_H

#include <stdlib.h>
#include <string.h>

/** \file netfunctionlist.h
 *  \brief Contains the list and management of the stored commands and functions.
 *  
 *  A NetFunction is a representation of a command how it is stored after registration.
 *  The functions are put in a double linked list. Elements can be added at any moment.
 *  It is not sorted, which means that it will become slow with an increasing number of elements. 
 */

/** \typedef typedef struct netfunction NetFunction 
 *  Use NetFunction instead of struct netfunction.
 */

/** \struct netfunction
 *  \brief A representation of a stored command.
 *
 *  NetFunction contains the needed information for one stored command. 
 *  They are the pointer to the function, the name of the command, which triggers the function,
 *  and the Parameter which was supplied. 
 *
 */
typedef struct netfunction NetFunction;
struct netfunction
{
  ///@{	
  /// Callback is the pointer to the function which will be executed when the command is regognized. 
  char* ( *Callback )( char*, int, void* );
  /// Parameter is the user set value which will be passed to the stored function.
  void* Parameter;
  /// Command is the stored string. This is the identifier for the stored function pointer.
  char* Command;

  /// Index stores the numeric position in the queue. 
  int Index;
  /// Previous is a pointer to the previous element in the list.
  NetFunction* Previous;
  /// Next is a pointer to the next element in the list.
  NetFunction* Next;
  ///@}
};


/** \fn NetFunction* NetFunction_Init( char* Command, char*( *Callback )( char*, int, void* ), void* Parameter )
 *  \brief Initializes a NetFunction instance.
 *
 *  Use this function to construct a new NetFunction object. The returned handle
 *  can be linked into a double linked list. The index is set to 0 on creation.
 *
 *  \param Command is the string which triggers the execution of the function.
 *  \param Callback is a pointer to the function which should be executed. 
 *  \param Parameter a void pointer chosen by the user. It can point to arbitrary data.
 *
 * \return A initialized NetFunction instance.
 */
NetFunction* NetFunction_Init( char* Command, 
			 char*( *Callback )( char*, int, void* ), 
			 void* Parameter );

/** \fn void NetFunction_Destroy( NetFunction* Self )
 *  \brief This function deinitalizes a NetFunction instance.
 *
 *  Use this function if you want to remove a NetFunction object. It frees the
 *  reserved memory.
 *
 *  \param Self is a NetFunction handle.
 *
 */
void NetFunction_Destroy( NetFunction* Self );

/** \typedef typedef struct netfunctionlist NetFunctionList 
 *  Use NetFunctionList instead of struct netfunctionlist.
 */

/** \struct netfunctionlist
 *  \brief This is the list containing the stored commands and respective functionpointers.
 *
 *  The whole list management is done with this object. It basically wraps the adding, removing,
 *  searching and simple retrieving of elements. A list after initalization is naturally empty.
 *  The Length attribute increases with every usage of NetFunctionList_RegisterFunction(). 
 *  It is not advised to try to add or remove elements manually. Future optimization could include
 *  a sorted adding of elements, as this would happen in most cases once at the startup of an
 *  application.
 *  \Note Do not use two NetFunctions with the same command. The same function pointer can be used.
 *
 */
typedef struct netfunctionlist NetFunctionList;
struct netfunctionlist
{
  //{@
  /// First points to the first object in the list.
  NetFunction* First;
  /// Last points to the last object in the list.
  NetFunction* Last;
  /// Length indicates how long the list is currently.
  int Length;  
  //}@
};

/** \fn NetFunctionList* NetFunctionList_Init();
 *  \brief Initializes a new FunctionList.
 *
 *  This function creates a new and empty NetFunctionList.
 *
 *  \return Handle to NetFunctionList instance.
 *
 */
NetFunctionList* NetFunctionList_Init();


/** \fn int NetFunctionList_RegisterFunction( NetFunctionList* Self, char* Command, char*( *Callback )( char*, int, void* ), void* Parameter )
 *  \brief This function creates a new NetFunction and adds it to the list.
 *
 *  Use this function to add a new command to the list. You need to supply a unique name, by which
 *  the function will be called. Also a pointer to the function that should be called is also needed.
 *  The additional parameter is not mandatory and can be set to NULL.
 * 
 *  \param Self is a valid NetFunctionList handle.
 *  \param Command is a zero terminated string containing the command.
 *  \param Callback is the pointer to the function that is called if the command is recognized.
 *  \param Parameter is a optional void pointer that will be passed to the callback.
 *
 *  \return -1 if an error occoured, 0 if successfull
 *
 */
int NetFunctionList_RegisterFunction( NetFunctionList* Self, 
				   char* Command,
				   char*( *Callback )( char*, int, void* ),
				   void* Parameter );
/** \fn int NetFunctionList_UnRegisterFunction( NetFunctionList* Self, char* Command )
 *  \brief Removes a NetFunction from the list.
 *
 *  To remove a NetFunction from the list you have to supply the unique command. It is then
 *  unlinked and destroyed.
 *
 *  \param Self is a valid NetFunctionList handle.
 *  \param Command is a zero terminated string containing the command. 
 * 
 *  \return -1 on error, 0 on success
 */
int NetFunctionList_UnRegisterFunction( NetFunctionList* Self, char* Command );

/** \fn NetFunction* NetFunctionList_FindFunction( NetFunctionList* Self, char* Command )
 *  \brief Get the handle of a NetFunction.
 *
 *  To access the single functions, you have to supply the associated command to this function.
 *  It then walks through the list and finds the first NetFunction with the same Command string.
 *  The search works like a linear search and is not very efficient. 
 *  Here is a point where optimazation may do some good.
 *
 *  \param Self is a valid NetFunctionList handle.
 *  \param Command is the name of the wanted NetFunction.
 *
 *  \return A NetFunction pointer to the appropriate instance or NULL if the element is not present.
 */ 
NetFunction* NetFunctionList_FindFunction( NetFunctionList* Self, char* Command );


//char* NetFunctionList_LikeFunctions( NetFunctionList* Self, char* Command );

#endif
