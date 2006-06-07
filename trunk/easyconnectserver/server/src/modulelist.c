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


#include "modulelist.h"

Module* Module_Init( char* Name, char* Description )
{
  if( Name == NULL )
  {
    return NULL;
  } 
  
  Module* Self = (Module*) malloc( sizeof( Module ) );
  
  Self->Name = strdup( Name ); 
  if( Description != NULL )
  {	  
    Self->Description = strdup( Description ); 
  }

  Self->Type = MT_NONE;
  Self->CMod = NULL;
  Self->PyMod = NULL;
  
  Self->Next = NULL;
  Self->Previous = NULL;

  return Self; 
}

int Module_SetPythonModule( Module* Self, PythonModule* PyMod )
{
  if( Self->Type != MT_NONE )
  {
    return -1;
  }
  Self->Type = MT_PYTHON;
  Self->PyMod = PyMod;
  return 0;
}

int Module_SetCModule( Module* Self, CModule* CMod )
{
  if( Self->Type != MT_NONE )
  {
    return -1;
  }
  Self->Type = MT_C;
  Self->CMod = CMod;
  return 0;
}

int Module_LoadFunctions( Module* Self, char* Path )
{
  Self->FuncList = ModFunctionList_Init( );

  int Length = DeviceFunctionCfg_GetNumberofFunctions( Path );
  int i;
  char* FunctionName;
  char* Help;
  char* Error;
  ModFunction* TmpFunc;
  for( i = 0; i < Length; i++ )
  {
    FunctionName = DeviceFunctionCfg_GetName( i, Path );
    Error = DeviceFunctionCfg_GetError( i, Path );
    Help = DeviceFunctionCfg_GetHelp( i, Path );

    ModFunctionList_AddFunction( Self->FuncList, ModFunction_Init( FunctionName, Error, Help ) );
    free( FunctionName );
    if( Help != NULL )
      free( Help );
    if( Error != NULL )
      free( Error );
    
  }
  return Length;
}


int Module_Destroy( Module* Self )
{
  if( Self == NULL )
  {
    return -1;
  }
  
  switch(Self->Type)
  {
    case MT_C:
      CModule_Destroy( Self->CMod );
      break;
    case MT_PYTHON:
      PythonModule_Destroy( Self->PyMod );
      break;
    case MT_NONE:
      break;
    default:
      break;
  }
  free( Self->Name );
  if( Self->Description )
  {
    free( Self->Description );
  }
  free( Self );
  return 0; 
}






///////////////////////////////////////////////////////////////
// 
// ModuleList
//
///////////////////////////////////////////////////////////////


ModuleList* ModuleList_Init( )
{
  ModuleList* Self = (ModuleList*) malloc( sizeof( ModuleList ) );
  Self->PyEnv = PythonEnv_Init();

  Self->First = NULL;
  Self->Last = NULL;

  Self->Length = 0;
  return Self;
}

int ModuleList_AddAllFromDir( ModuleList* Self, char* Path )
{
  return -1;
}

int ModuleList_AddFromPath( ModuleList* Self, char* Path )
{
  if( Self == NULL || Path == NULL )
  {
    return -1;
  }

  char* ModName = DeviceCfg_GetName( Path );
  char* ModDesc = DeviceCfg_GetDescription( Path );
  Module* New = Module_Init( ModName, ModDesc );

  char* LibPath = DeviceCfg_GetLibPath( Path );
  char* InitString = DeviceCfg_GetInitString( Path );
  char* LibType = DeviceCfg_GetLibType( Path );

  if( LibPath == NULL )
  {
    Module_Destroy( New );
    return -1;
  } 
  if( strcmp( LibType, "c" ) == 0 )
  {
    Module_SetCModule( New, CModule_Init( LibPath, ModName, InitString ));  
  } else if( strcmp( LibType, "python" ) == 0 )
  {
    char* ModName = NULL;
    char* FullPath = NULL;
    char* Tmp = strrchr( LibPath, '/' );
    Tmp++;
    int length;
    // This case is just if the module is in the same folder
    if( Tmp  == NULL )
    {
      FullPath = strdup(".");
      ModName = strstr( LibPath, ".py" );
      length =  strlen( LibPath) - strlen( ModName );
      Tmp = malloc( sizeof(char) * (length + 1) );
      strncpy( Tmp, ModName, length ); 
      Tmp[length] = '\0';
    }
    else
    {
      length = strlen( LibPath ) - strlen( Tmp ) ;
      FullPath = malloc( sizeof(char) * (length+1));
      strncpy( FullPath, LibPath, length );
      FullPath[length];

      ModName = strstr( Tmp, ".py" );
      ModName = strndup( Tmp, strlen( Tmp ) - strlen( ModName ) );		      
    }	      
    PythonEnv_AddPath( Self->PyEnv, FullPath );
    Module_SetPythonModule( New, PythonModule_Init( Self->PyEnv, ModName ));
    PythonModule_ImportFunctions( New->PyMod, InitString);
  }else
  {
    free( LibPath );
    free( LibType );
    Module_Destroy( New );
  }
 
  Module_LoadFunctions( New, Path ); 
  ModuleList_Add( Self, New );
  free( ModName );
  if( ModDesc )
    free( ModDesc );

  if( LibPath )
    free( LibPath );
  if( InitString )
    free( InitString );
  if( LibType )
    free( LibType );
      	
}

int ModuleList_Add( ModuleList* Self, Module* Mod )
{
  if( Mod == NULL )
  {
    return -1;
  }

  if( Self->Length == 0 )
  {
    Self->First = Mod;
    Self->Last = Mod;
  } else
  {
    Self->Last->Next = Mod;
    Mod->Previous = Self->Last;
    Self->Last = Mod;
  }
  Self->Length++;
}

int ModuleList_RegisterAll( ModuleList* Self, TcpCliServer* Server )
{
  int i, j;
  Module* TmpMod = Self->First;
  ModFunction* TmpFunc;
  char* RegName;
  
  for( i = 0; i < Self->Length; i++ ) 
  {
    TmpFunc = TmpMod->FuncList->First;
    for( j = 0; j < TmpMod->FuncList->Length; j++ )
    {
      RegName = (char*) malloc( sizeof(char) * (strlen(TmpFunc->Name) + strlen(TmpMod->Name) +2));
      sprintf( RegName, "%s.%s", TmpMod->Name, TmpFunc->Name );
      
      switch( TmpMod->Type )
      {
	case MT_C:
	  TcpCliServer_RegisterFunction( Server, RegName, CModule_Callback , TmpMod->CMod ); 
	  break;
	case MT_PYTHON:
	  TcpCliServer_RegisterFunction( Server, RegName, PythonModule_Callback , TmpMod->PyMod ); 
	  break;
	default:
	  break;
      }	  
      free(RegName);
      
      TmpFunc = TmpFunc->Next;  
    } 
    TmpMod = TmpMod->Next; 
  }
  return 0;
}


int ModuleList_Destroy( ModuleList* Self )
{
  if( Self == NULL )
  {
    return -1;
  }
  
  int i;
  Module* Tmp = Self->Last;
  for( i = 0; i < Self->Length; i++ )
  {
    Self->Last = Tmp->Previous;
    Module_Destroy( Tmp );
    Tmp = Self->Last;
  }
 
  free( Self );
  return 0; 
}


