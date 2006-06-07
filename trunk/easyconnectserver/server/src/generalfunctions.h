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

#ifndef __GENERALFUNCTIONS_H
#define __GENERALFUNCTIONS_H

//#include <confuse.h>
#include "iniparser/iniparser.h"
#include "functionlist.h"
#include <stdlib.h>
#include <stdio.h>

char* HelpFunction( char* Callstring, int Socket, void* Func );
char* ListFunctionsFunction( char* Callstring, int Socket, void* Func );
char* ConnectFunction( char* Callstring, int Socket, void* Func );

//Function* GetFunctionFromFile( char* FileName, char* FunctionIdentifier,
//			       char* (*Callback)(char*, int, void*), void* Argument);

//int AddGeneralFunctionsToList( FunctionList* List, char* FunctionsFile );

#endif
