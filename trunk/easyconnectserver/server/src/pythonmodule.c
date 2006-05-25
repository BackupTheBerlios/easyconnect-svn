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

int PythonModule_Init()
{
  Py_Initialize();
  return 0;
}

int PythonModule_Destroy()
{
  Py_Finalize();
  return 0;
}

PythonEnv* PythonEnv_Init()
{
  PythonEnv* Self = (PythonEnv*) malloc( sizeof(PythonEnv) );

  Self->MainModule = PyImport_AddModule("__main__");
  Py_INCREF( Self->MainModule );
  
  PyObject* main_dict = PyModule_GetDict(main_module);
  Self->MainDict = PyDict_Copy(main_dict);
  Py_INCREF( Self->MainDict );

  Self->Functions = NULL;
  return Self;
}

int PythonEnv_LoadFromFile( PythonEnv* Self, char* Path )
{

}

int PythonEnv_ExecuteFunction( PythonEnv* Self, int FnNumber )
{
}

int PythonEnv_Destroy( PythonEnv* Self )
{
  // TODO: Decref for functions
  Py_DECREF( Self->MainModule );
  Py_DECREF( Self->MainDict );
  free( Self );
}



