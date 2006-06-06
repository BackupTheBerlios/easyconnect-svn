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
  Self->List = NULL;
  Self->ListLength = 0;
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
    PythonEnv_LeaveThread( Self );
    PythonEnv_Destroy( Self );
    return NULL;
  }

  Py_INCREF(Self->Module);
  Py_DECREF(pName);

  PythonEnv_LeaveThread( Self ); 
  return Self;
}

int PythonEnv_EnterThread( PythonEnv* Self )
{
  PyEval_AcquireLock(); 
  PyThreadState_Swap( Self->CurrentThreadState );
  return 0;
}

int PythonEnv_LeaveThread( PythonEnv* Self )
{
  PyThreadState_Swap(NULL); 
  PyEval_ReleaseLock();
  return 0;	
}

PyObject* PythonEnv_CreateArgument( char* String )
{
  PyObject* Param; 
  if( String == NULL )
    Param = PyTuple_New(0);
  else
    Param = PyTuple_Pack(1,PyString_FromString(String));
  return Param; 
}

PyObject* PythonEnv_InstatiateClass( PythonEnv* Self, char* Name, char* Parameter )
{
  if( Name == NULL || Self == NULL )
    return NULL;

  PyObject* Instance;
  PyObject* Class;

  Class = PyObject_GetAttrString( Self->Module, Name );  
  if( !Class || !PyClass_Check( Class ) )
  {
    PyErr_Print();
    PythonEnv_LeaveThread( Self );   
    return NULL;  
  }
  Instance = PyInstance_New( Class, PythonEnv_CreateArgument( Parameter ), NULL );
  if( !Instance || !PyInstance_Check( Instance) )
  {
    PyErr_Print();
    PythonEnv_LeaveThread( Self );   
    return NULL;  
  }
  return Instance;
}


int PythonEnv_ImportFunctions( PythonEnv* Self, char* Parameter)
{
  PyObject* pFunc;
  PyObject* Ret;
  PyObject* RetString;
  PyObject* pClass;
  char* FuncName;
  char* ClassName;
  int Size, i;
  PyObject* Module = Self->Module;
  Py_INCREF( Module );

  PythonEnv_EnterThread( Self );
  
  pFunc = PyObject_GetAttrString( Module, "__introspection__" ); 
  if( !pFunc || !PyCallable_Check( pFunc ) )
  {
    PyErr_Print();
    PythonEnv_LeaveThread( Self );   
    return -1;
  }

  Ret = PyObject_CallFunction(pFunc, NULL );
  Py_DECREF(pFunc);
  if( !Ret || !PyList_Check( Ret ) )
  {
    PyErr_Print();
    PythonEnv_LeaveThread( Self );   
    return -1;
  }
  RetString = PyList_GetItem( Ret, 0 );
  Py_INCREF( RetString );
  Py_DECREF( Ret );
  
  Self->Instance = PythonEnv_InstatiateClass( Self, PyString_AsString( RetString ), Parameter ); 
  Py_DECREF( RetString );
  
  Ret = PyObject_CallMethod( Self->Instance, "__introspection__", NULL );
  if( !Ret || !PyList_Check( Ret ) )
  {
    PyErr_Print();
    PythonEnv_LeaveThread( Self );   
    return -1;
  }
  
   
  //PyObject_Print( Ret, stderr, 0 );
  Size = PyList_Size(Ret);
  Self->List = (char**) malloc( sizeof(char*) * Size );
  for( i = 0; i < Size; i++ )
  {
    RetString = PyList_GetItem( Ret, i );
    Self->List[i] = strdup( PyString_AsString( RetString ));
  }
  Self->ListLength = Size;
  
  PythonEnv_LeaveThread( Self );
  return 0;
}

char* PythonEnv_ExecuteFunction( PythonEnv* Self, char* Function, char* Parameter )
{
  PythonEnv_EnterThread( Self );
  
  PyObject* Ret;
  char* RetString = NULL;
  
  Ret = PyObject_CallMethod( Self->Instance, Function, "s", Parameter );
  if( !Ret || !PyString_Check( Ret ) )
  {
    PyErr_Print();
    PythonEnv_LeaveThread( Self );   
    return NULL;
  } 
  RetString = strdup( PyString_AsString( Ret ) ); 
  Py_DECREF( Ret );
  PythonEnv_LeaveThread( Self ); 
  return RetString;
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


