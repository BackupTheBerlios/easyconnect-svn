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

typedef struct netfunction NetFunction;
struct netfunction
{
  char* ( *Callback )( char*, int, void* );
  void* Parameter;
  char* Command;

  int Index;
  NetFunction* Previous;
  NetFunction* Next;
};


NetFunction* NetFunction_Init( char* Command, 
			 char*( *Callback )( char*, int, void* ), 
			 void* Parameter );


typedef struct netfunctionlist NetFunctionList;
struct netfunctionlist
{
  NetFunction* First;
  NetFunction* Last;
  int Length;  
};

NetFunctionList* NetFunctionList_Init();

int NetFunctionList_RegisterFunction( NetFunctionList* Self, 
				   char* Command,
				   char*( *Callback )( char*, int, void* ),
				   void* Parameter );

int NetFunctionList_UnRegisterFunction( NetFunctionList* Self, char* Command );

NetFunction* NetFunctionList_FindFunction( NetFunctionList* Self, char* Command );

char* NetFunctionList_LikeFunctions( NetFunctionList* Self, char* Command );

#endif
