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


#ifndef __MODULELIST_H
#define __MODULELIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "functionlist.h"
#include "config.h"
#include "cmodule.h"
#include "tcpcliserver/tcpcliserver.h"
#include "pythonmodule.h"

enum ModuleTypes { MT_NONE, MT_C, MT_PYTHON, MT_GENERAL };


typedef struct module Module;
struct module
{
  char* Name;
  char* Description;
  int Type;
  
  CModule* CMod;
  PythonModule* PyMod; 

  ModFunctionList* FuncList;
	
  Module* Next;
  Module* Previous;
  
};

typedef struct modulelist ModuleList;
struct modulelist
{
  PythonEnv* PyEnv;
  Module* First;
  Module* Last;
  int Length;
 
  char* FunctionList; 
};



Module* Module_Init( char* Name, char* Description );
int Module_SetPythonModule( Module* Self, PythonModule* PyMod );
int Module_SetCModule( Module* Self, CModule* CMod );
int Module_SetGeneralModule( Module* Self );
int Module_LoadFunctions( Module* Self, char* Path );
char* Module_ExecuteFunction( Module* Self, char* Function, char* Parameter );
int Module_Destroy( Module* Self );

// returns new instance
ModuleList* ModuleList_Init( );

// returns reference
Module* ModuleList_GetModule( ModuleList* Self, char* ModuleName );
int ModuleList_GenerateList( ModuleList* Self );
// returns reference
// You dont have to provide a Modulename if you got a func name like 'Mod.Func'
ModFunction* ModuleList_GetFunction( ModuleList* Self, char* ModName, char* FuncName );

// returns reference
ModFunction* ModuleList_GetFunctionFromCallstring( ModuleList* Self, char* Callstring );
int ModuleList_AddAllFromDir( ModuleList* Self, char* Path );
int ModuleList_AddFromPath( ModuleList* Self, char* Path );
int ModuleList_Add( ModuleList* Self, Module* Mod );
int ModuleList_RegisterAll( ModuleList* Self, TcpCliServer* Server );
char* ModuleList_CallFunction( ModuleList* Self, char* Callstring );
int ModuleList_Destroy( ModuleList* Self );

char* GeneralCallback( char* Callstring, int Socket, void* Parameter );
char* ModuleList_GetHelpString( ModuleList* Self, char* Parameter );
char* ConnectFunction( ModuleList* Self, char* Argument );
#endif
