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
 *  This software is part of the easyConnect system http://easyconnect.berlios.de
 *  written by: 
 *
 *  Moritz Eberl <meberl@vscope.de>
 */

#include <python2.4/Python.h>
#include <stdlib.h>

typedef struct pythonenv PythonEnv;
struct pythonenv
{
  PyThreadState* MainThreadState;
};

typedef struct pythonmodule PythonModule;
struct pythonmodule
{
  PythonEnv* Environment;
  PyThreadState* CurrentThreadState;

  // Module stuff
  char* ModuleName;
  PyObject* Module;
 
  // Class instance 
  PyObject* Instance;
  
  // Functionlist
  char** List;
  int ListLength;
};


/////////////////////////////////////////////////////////////////
// PythonEnv Functions
/////////////////////////////////////////////////////////////////

PythonEnv* PythonEnv_Init();
int PythonEnv_AddPath( PythonEnv* Self, char* Path );
int PythonEnv_Destroy( PythonEnv* Self );


/////////////////////////////////////////////////////////////////
// PythonModule Functions
/////////////////////////////////////////////////////////////////

PythonModule* PythonModule_Init( PythonEnv* Environment, char* ModuleName );

int PythonModule_EnterThread( PythonModule* Self );

int PythonModule_ImportFunctions( PythonModule* Self, char* Parameter );
char* PythonModule_ExecuteFunction( PythonModule* Self, char* Function, char* Parameter );

int PythonModule_LeaveThread( PythonModule* Self );

PyObject* PythonModule_CreateArgument( char* String );
PyObject* PythonModule_InstatiateClass( PythonModule* Self, char* Name, char* Parameter );

char* PythonModule_Callback( char* Callstring, int Socket, void* Parameter );

int PythonModule_Destroy( PythonModule* Self );

/*
/////////////////////////////////////////////////////////////////
// PythonFunction Functions
/////////////////////////////////////////////////////////////////


PythonFunction* PythonFunction_Init( char* FunctionName, PyObject* Func );
char* PythonFunction_Execute( PythonFunction* Self, char* Parameter );
int PythonFunction_Destroy( PythonFunction* Self );
*/
