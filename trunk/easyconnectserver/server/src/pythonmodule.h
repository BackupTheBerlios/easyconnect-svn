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


typedef struct pythonfunction PythonFunction;
struct pythonfunction
{
  char* FunctionName;
  PyObject* Func; 
};

typedef struct pythonmodule PythonModule;
struct pythonmodule
{
  PyThreadState* MainThreadState;
};

typedef struct pythonenv PythonEnv;
struct pythonenv
{
  char* ModuleName;
  PythonModule* Master;
  PyThreadState* CurrentThreadState;
  PyObject* Module;
  
  PyObject* Instance;
  
  char** List;
  int ListLength;
};


/////////////////////////////////////////////////////////////////
// PythonModule Functions
/////////////////////////////////////////////////////////////////

PythonModule* PythonModule_Init();
int PythonModule_AddPath( PythonModule* Self, char* Path );
int PythonModule_Destroy( PythonModule* Self );


/////////////////////////////////////////////////////////////////
// PythonEnv Functions
/////////////////////////////////////////////////////////////////

PythonEnv* PythonEnv_Init( PythonModule* Master, char* ModuleName );

int PythonEnv_EnterThread( PythonEnv* Self );

int PythonEnv_ImportFunctions( PythonEnv* Self, char* Parameter );
char* PythonEnv_ExecuteFunction( PythonEnv* Self, char* Function, char* Parameter );

int PythonEnv_LeaveThread( PythonEnv* Self );

PyObject* PythonEnv_CreateArgument( char* String );
PyObject* PythonEnv_InstatiateClass( PythonEnv* Self, char* Name, char* Parameter );

int PythonEnv_Destroy( PythonEnv* Self );

/////////////////////////////////////////////////////////////////
// PythonFunction Functions
/////////////////////////////////////////////////////////////////


PythonFunction* PythonFunction_Init( char* FunctionName, PyObject* Func );
char* PythonFunction_Execute( PythonFunction* Self, char* Parameter );
int PythonFunction_Destroy( PythonFunction* Self );

