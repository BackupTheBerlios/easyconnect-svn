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


#ifndef __CMODULE_H
#define __CMODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>

typedef struct cmodule CModule;
struct cmodule
{
  void* LibHandle; 
  void* Handle;
  
  char* (*Communicate)( void* , int, char** );
  int (*Destroy)( void* );
  
  char* Name;
};

CModule* CModule_Init( char* LibFilePath, char* Name, char* InitString );

char* CModule_Communicate( CModule* Self, int argc, char** argv );

char* CModule_ExecuteFunction( CModule* Self, char* Function, char* Parameter );

char* CModule_Callback( char* Callstring, int Socket, void* Parameter );

int CModule_Destroy( CModule* Self );


#endif
