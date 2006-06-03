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

#include "pythonmodule.h"

PythonModule* PythonModule_Init()
{
  PythonModule* Self = (PythonModule*) malloc( sizeof( PythonModule ) );
  
  Py_Initialize();
  PyEval_InitThreads();
  Self->MainThreadState = PyThreadState_Get();

  PyEval_ReleaseLock();
  return Self;
}

int PythonModule_AddPath( PythonModule* Self, char* Path )
{
  char* Command = (char*) malloc( sizeof( char ) * 
				    ( strlen( Path ) +
				    26 ));
  sprintf( Command, "import sys\nsys.path += '%s'", Path );
  PyRun_SimpleString( Command );
  return 0;
	
}


int PythonModule_Destroy( PythonModule* Self )
{
  PyEval_AcquireLock();
  PyThreadState_Swap(Self->MainThreadState); 
  Py_Finalize();
  free( Self );
  return 0;
}



PythonEnv* PythonEnv_Init( PythonModule* Master, char* ModuleName )
{
  PyObject* pName;
  
  PythonEnv* Self = (PythonEnv*) malloc( sizeof(PythonEnv) );
  Self->Module=NULL;
  Self->ModuleName = (char*) malloc( sizeof(char) * strlen(ModuleName)+1 );
  strcpy( Self->ModuleName, ModuleName );

  Self->Master = Master;
  
  PyEval_AcquireLock(); 
  Self->CurrentThreadState = PyThreadState_New( Self->Master->MainThreadState->interp );
  PyThreadState_Swap( Self->CurrentThreadState );

  pName = PyString_FromString(Self->ModuleName);

  Self->Module = PyImport_Import(pName);
  
  if( Self->Module == NULL )
  {
    PyErr_Print();
    PyThreadState_Swap(NULL); 
    PyEval_ReleaseLock();
    PythonEnv_Destroy( Self );
    return NULL;
  }

  Py_INCREF(Self->Module);
  Py_DECREF(pName);
 
  PyThreadState_Swap(NULL); 
  PyEval_ReleaseLock();
  return Self;
}


int PythonEnv_ImportFunctions( PythonEnv* Self )
{
  char* FuncName;
  PyObject* pFunc;
  PyObject* Ret;
  PyObject* pClass;
  FuncName = (char*) malloc( sizeof(char) * ( strlen( Self->ModuleName ) +
						    strlen( ".__introspection__" ) +
						    1
						   ));
  sprintf( FuncName, "%s%s\0", Self->ModuleName, ".__introspection__");
  printf("%s\n", FuncName );
  pFunc = PyObject_GetAttrString( Self->Module, FuncName ); 
  free( FuncName );

  if( pFunc && PyCallable_Check( pFunc ) )
  {

    Py_DECREF(pFunc);
    Ret = PyObject_CallObject(pFunc, NULL);
    if( PyString_Check( Ret ) )
    {
      pClass = PyObject_GetAttr( Self->Module, Ret );  
    }

  }
  return -1;
}



char* PythonEnv_ExecuteFunction( PythonEnv* Self, char* Function )
{
  PyObject* pFunc;
  PyObject* Module = Self->Module;
  Py_INCREF( Module );
  PyEval_AcquireLock(); 
  PyThreadState_Swap( Self->CurrentThreadState );

  pFunc = PyObject_GetAttrString( Module, Function ); 
  Py_DECREF( Module );

  if( pFunc && PyCallable_Check( pFunc ) )
  {
    Py_DECREF(pFunc);
    PyObject_CallObject(pFunc, NULL);
  }
  else
  {
    PyErr_Print();
  }
  PyObject_Print( Self->Module, stderr, 0 );
  PyThreadState_Swap(NULL); 
  PyEval_ReleaseLock();

  return NULL;
}

int PythonEnv_Destroy( PythonEnv* Self )
{
  if( Self != NULL )  
  {
    PyEval_AcquireLock();
  
    PyThreadState_Swap(NULL);
    PyThreadState_Clear(Self->CurrentThreadState);
    PyThreadState_Delete(Self->CurrentThreadState);

    PyEval_ReleaseLock();
  }

  free( Self );
}



