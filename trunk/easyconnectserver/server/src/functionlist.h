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


#ifndef __FUNCTIONLIST_H
#define __FUNCTIONLIST_H

#include "devicelist.h"
#include "tcpcliserver/tcpcliserver.h"

typedef struct function Function;
struct function
{
  char* Name;
	    
  char* Error;
  char* Help;
  char* Param;
  char* ParamConvert;  

  Device* Dev;  

  char* (*Callback)(char*, int, void*);
  void* Argument;
  
  Function* Next;
  Function* Previous;
};

Function* Function_Init( char* Name, char* Error, char* Help,
			 char* Param, char* ParamConvert,
			 char* (*Callback)(char*, int, void*), void* Argument );

int Function_Destroy( Function* Self );


typedef struct functionlist FunctionList;
struct functionlist
{
  Function* First;
  Function* Last;

  int Length;

  char* List;
};

FunctionList* FunctionList_Init( );

int FunctionList_AddFunction( FunctionList* Self,  Function* Func );

int FunctionList_RegisterAllFunctions( FunctionList* Self, TcpCliServer* Server );

int FunctionList_GenerateList( FunctionList* Self );

Function* FunctionList_GetElementByName( FunctionList* Self, char* FunctionName );

int FunctionList_Destroy( FunctionList* Self );



#endif
