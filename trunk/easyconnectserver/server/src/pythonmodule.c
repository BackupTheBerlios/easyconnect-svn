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

PythonEnv* PythonEnv_Init()
{
  PythonEnv* Self = (PythonEnv*) malloc( sizeof( PythonEnv ) );
  
  Py_Initialize();
  PyEval_InitThreads();
  Self->MainThreadState = PyThreadState_Get();

  PyEval_ReleaseLock();
  return Self;
}

int PythonEnv_AddPath( PythonEnv* Self, char* Path )
{
  if( Path == NULL )
    return -1;
  char* Command = (char*) malloc( sizeof( char ) * 
				    ( strlen( Path ) +
				    26 ));
  PyRun_SimpleString("import sys");
		  
  sprintf( Command, "sys.path.append('%s')", Path );
  PyRun_SimpleString( Command );
  return 0;
	
}


int PythonEnv_Destroy( PythonEnv* Self )
{
  PyEval_AcquireLock();
  PyThreadState_Swap(Self->MainThreadState); 
  Py_Finalize();
  free( Self );
  return 0;
}


PythonModule* PythonModule_Init( PythonEnv* Environment, char* ModuleName )
{
  PyObject* pName;
  
  PythonModule* Self = (PythonModule*) malloc( sizeof(PythonModule) );
  Self->Module=NULL;
  Self->List = NULL;
  Self->ListLength = 0;
  Self->ModuleName = (char*) malloc( sizeof(char) * strlen(ModuleName)+1 );
  strcpy( Self->ModuleName, ModuleName );

  Self->Environment = Environment;
  
  PyEval_AcquireLock(); 
  Self->CurrentThreadState = PyThreadState_New( Self->Environment->MainThreadState->interp );
  PyThreadState_Swap( Self->CurrentThreadState );

  pName = PyString_FromString(Self->ModuleName);

  Self->Module = PyImport_Import(pName);
  
  if( Self->Module == NULL )
  {
    PyErr_Print();
    PythonModule_LeaveThread( Self );
    PythonModule_Destroy( Self );
    return NULL;
  }

  Py_INCREF(Self->Module);
  Py_DECREF(pName);

  PythonModule_LeaveThread( Self ); 
  return Self;
}

int PythonModule_EnterThread( PythonModule* Self )
{
  PyEval_AcquireLock(); 
  PyThreadState_Swap( Self->CurrentThreadState );
  return 0;
}

int PythonModule_LeaveThread( PythonModule* Self )
{
  PyThreadState_Swap(NULL); 
  PyEval_ReleaseLock();
  return 0;	
}

PyObject* PythonModule_CreateArgument( char* String )
{
  PyObject* Param; 
  if( String == NULL )
    Param = PyTuple_New(0);
  else
    Param = PyTuple_Pack(1,PyString_FromString(String));
  return Param; 
}

PyObject* PythonModule_InstatiateClass( PythonModule* Self, char* Name, char* Parameter )
{
  if( Name == NULL || Self == NULL )
    return NULL;

  PyObject* Instance;
  PyObject* Class;

  Class = PyObject_GetAttrString( Self->Module, Name );  
  if( !Class || !PyClass_Check( Class ) )
  {
    PyErr_Print();
    PythonModule_LeaveThread( Self );   
    return NULL;  
  }
  Instance = PyInstance_New( Class, PythonModule_CreateArgument( Parameter ), NULL );
  if( !Instance || !PyInstance_Check( Instance) )
  {
    PyErr_Print();
    PythonModule_LeaveThread( Self );   
    return NULL;  
  }
  return Instance;
}


int PythonModule_ImportFunctions( PythonModule* Self, char* Parameter)
{
  if( Self == NULL )
    return -1;
  PyObject* pFunc;
  PyObject* Ret;
  PyObject* RetString;
  PyObject* pClass;
  char* FuncName;
  char* ClassName;
  int Size, i;
  PyObject* Module = Self->Module;
  Py_INCREF( Module );

  PythonModule_EnterThread( Self );
  
  pFunc = PyObject_GetAttrString( Module, "__introspection__" ); 
  if( !pFunc || !PyCallable_Check( pFunc ) )
  {
    PyErr_Print();
    PythonModule_LeaveThread( Self );   
    return -1;
  }

  Ret = PyObject_CallFunction(pFunc, NULL );
  Py_DECREF(pFunc);
  if( !Ret || !PyList_Check( Ret ) )
  {
    PyErr_Print();
    PythonModule_LeaveThread( Self );   
    return -1;
  }
  RetString = PyList_GetItem( Ret, 0 );
  Py_INCREF( RetString );
  Py_DECREF( Ret );
  
  Self->Instance = PythonModule_InstatiateClass( Self, PyString_AsString( RetString ), Parameter ); 
  Py_DECREF( RetString );
  
  Ret = PyObject_CallMethod( Self->Instance, "__introspection__", NULL );
  if( !Ret || !PyList_Check( Ret ) )
  {
    PyErr_Print();
    PythonModule_LeaveThread( Self );   
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
  
  PythonModule_LeaveThread( Self );
  return 0;
}

char* PythonModule_ExecuteFunction( PythonModule* Self, char* Function, char* Parameter )
{
  PythonModule_EnterThread( Self );
  
  PyObject* Ret;
  char* RetString = NULL;
  
  Ret = PyObject_CallMethod( Self->Instance, Function, "s", Parameter );
  if( !Ret || !PyString_Check( Ret ) )
  {
    PyErr_Print();
    PythonModule_LeaveThread( Self );   
    return NULL;
  } 
  RetString = strdup( PyString_AsString( Ret ) ); 
  Py_DECREF( Ret );
  PythonModule_LeaveThread( Self ); 
  return RetString;
}

char* PythonModule_Callback( char* Callstring, int Socket, void* Parameter )
{
  PythonModule* Self = (PythonModule*) Parameter;
  char* End;
  char* Function;
  char* Argument;
  char* Tmp;
  int j;
  int length;

  Argument = strchr( Callstring, '.' );
  Argument++;
 
  
  End = strchr( Argument, ' ' ); 
  if( End == NULL )
  {
    Function = Argument;
    Argument = NULL;
    //printf("Function = %s\nArgument is null\n",Function );
  }else
  {
    length = strlen(Argument) - strlen(End);
    Function = (char*) malloc( sizeof(char) * ( length+1));
    strncpy( Function, Argument, length);
    Function[length] = '\0';

    Tmp = End;
    End = Tmp + strlen(Tmp);
    for(j = 0; Tmp+j < End && Tmp[j] == ' '; j++ );
    Argument = Tmp + j;

    //printf("Function = %s\nArgument = %s\n",Function, Argument );
  }

  return PythonModule_ExecuteFunction( Self, Function, Argument ); 

}

int PythonModule_Destroy( PythonModule* Self )
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


