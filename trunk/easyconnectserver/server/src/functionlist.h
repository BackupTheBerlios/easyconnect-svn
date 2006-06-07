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

#include "tcpcliserver/tcpcliserver.h"

typedef struct modfunction ModFunction;
struct modfunction
{
  char* Name;
  char* Error;
  char* Help;

  ModFunction* Next;
  ModFunction* Previous;
};

ModFunction* ModFunction_Init( char* Name, char* Error, char* Help );
int ModFunction_Destroy( ModFunction* Self );

typedef struct modfunctionlist ModFunctionList;
struct modfunctionlist
{
  ModFunction* First;
  ModFunction* Last;

  int Length;
};

ModFunctionList* ModFunctionList_Init( );

int ModFunctionList_AddFunction( ModFunctionList* Self,  ModFunction* Func );

ModFunction* ModFunctionList_GetElementByName( ModFunctionList* Self, char* FunctionName );

int ModFunctionList_Destroy( ModFunctionList* Self );

#endif
