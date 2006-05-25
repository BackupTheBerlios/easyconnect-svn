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

#include <Python.h>
#include <stdlib.h>

typedef struct pythonenv PythonEnv;
struct pythonenv
{
  PyObject* MainModule;
  PyObject* MainDict;
  PyObject** Functions;  
};


int PythonModule_Init();
int PythonModule_Destroy();


PythonEnv* PythonEnv_Init();
int PythonEnv_LoadFromFile( PythonEnv* Self, char* Path );
int PythonEnv_ExecuteFunction( PythonEnv* Self, int FnNumber );
int PythonEnv_Destroy( PythonEnv* Self );



