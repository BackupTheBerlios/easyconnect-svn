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
  Self->Description = NULL;
  
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

int Module_SetGeneralModule( Module* Self )
{
  if( Self->Type != MT_NONE )
  {
    return -1;
  }
  Self->Type = MT_GENERAL;
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
  if( Self->Name )
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

Module* ModuleList_GetModule( ModuleList* Self, char* ModuleName )
{
  if( Self == NULL )
  {
    return NULL;
  }
  int i;
  Module* Tmp = Self->First;
  for( i = 0; i < Self->Length; i++ )
  {
    if( strcmp( Tmp->Name, ModuleName ) == 0 )
    {
      return Tmp;
    }
    Tmp = Tmp->Next;
  }
  return NULL;
}

ModFunction* ModuleList_GetFunction( ModuleList* Self, char* ModName, char* FuncName )
{
  if( Self == NULL )
    return NULL;

  Module* Mod;
  ModFunction* Ret;
  char* Tmp;
  int Length = 0;

  if( ModName == NULL )
  {
    Tmp = strchr(FuncName, '.');
    if( Tmp == NULL )
      return NULL;
    Length = strlen(FuncName)-strlen(Tmp);
    ModName = (char*)malloc( sizeof(char) * (Length+1));
    strncpy( ModName, FuncName, Length );
    ModName[Length] = '\0';
    FuncName = ++Tmp;
  }
  Mod = ModuleList_GetModule( Self, ModName );
  Ret = ModFunctionList_GetFunction( Mod->FuncList, FuncName );
  if( Length != 0 )
    free( ModName );
  
  return Ret; 
}

ModFunction* ModuleList_GetFunctionFromCallstring( ModuleList* Self, char* Callstring )
{
  ModFunction* Ret;
  char* End = strchr( Callstring, ' ');
  int Length = 0;
  char* Call = Callstring;
  if( End != NULL )
  {
    Length = strlen(Callstring) - strlen(End);
    Call = (char*)malloc(sizeof(char)*(Length +1));
    strncpy(Call, Callstring, Length );
    Call[Length]='\0';
  }
  Ret = ModuleList_GetFunction( Self, NULL, Call );
  if( Length == 0 )
    free(Call); 
  
  return Ret; 
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

   
  if( strcmp( LibType, "c" ) == 0 )
  {
    if( LibPath == NULL )
    {
      Module_Destroy( New );
      return -1;
    }
    Module_SetCModule( New, CModule_Init( LibPath, ModName, InitString ));  
  } else if( strcmp( LibType, "python" ) == 0 )
  {
    if( LibPath == NULL )
    {
      Module_Destroy( New );
      return -1;
    }
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
      length = strlen( Tmp ) - strlen( ModName );
      ModName = malloc( sizeof(char) * ( length+1));
      strncpy( ModName, Tmp, length );
      ModName[length] = '\0';
      //ModName = strndup( Tmp, strlen( Tmp ) - strlen( ModName ) );		      
    }	      
    PythonEnv_AddPath( Self->PyEnv, FullPath );
    Module_SetPythonModule( New, PythonModule_Init( Self->PyEnv, ModName ));
    PythonModule_ImportFunctions( New->PyMod, InitString);
  }else if( strcmp( LibType, "general" ) == 0 ) 
  {
    Module_SetGeneralModule( New );    
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

  return 0;  
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

  ModuleList_GenerateList( Self );
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
	case MT_GENERAL:
	  TcpCliServer_RegisterFunction( Server, TmpFunc->Name, GeneralCallback , Self ); 
	  
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
 
  if( Self->FunctionList )
  {
    free( Self->FunctionList );
  }
  free( Self );
  return 0; 
}


char* GeneralCallback( char* Callstring, int Socket, void* Parameter )
{
  ModuleList* ModList = (ModuleList*) Parameter;
  ModFunction* FuncArg;
  char* ret = NULL;
  // TODO: Search for the first non blank string not for the first blank
  // Maybe in tcpcliserver
  char* Argument = strchr( Callstring, ' ' );
  char* End;
  if( Argument == NULL )
    End = Callstring+strlen(Callstring);  
  else
  {
    End = Argument;
    Argument++;
  }
  char* FuncName = (char*) malloc( sizeof(char) * (strlen(Callstring)-strlen(End)+1) );
  strncpy( FuncName, Callstring, strlen(Callstring)-strlen(End) );
  FuncName[strlen(Callstring)-strlen(End)] = '\0';
  FuncArg = ModuleList_GetFunction( ModList, "general", FuncName ); 
  if( strcmp( FuncName, "Help" ) == 0)
  {
    ret = HelpFunction( ModList, FuncArg, Argument );
  }else if( strcmp( FuncName, "ListFunctions" ) == 0)
  {
    ret = strdup(ModList->FunctionList);
  }
  
  free(FuncName); 
  return ret; 
  
}


char* HelpFunction( ModuleList* ModList, ModFunction* Func,  char* Parameter )
{
  char* ret;
  char* End;
  int j = 0;
  ModFunction* ArgFunc;

  // TODO: Search for the first non blank string not for the first blank
  if( Parameter != NULL && strlen(Parameter) != 0 )
  {
    char* End = Parameter + strlen(Parameter);
    for( j = 0; Parameter+j < End && Parameter[j] == ' '; j++ );
        Parameter = Parameter + j;
    if( Parameter != End )
    {     
	
      ArgFunc = ModuleList_GetFunction( ModList, NULL, Parameter );  
      if( ArgFunc == NULL )
      {
	return strdup(Func->Error);
      }
      ret = strdup(ArgFunc->Help);
      return ret;
    }  
  }
  ret = strdup(Func->Help);
  return ret;
}


int ModuleList_GenerateList( ModuleList* Self )
{
  int i, j, length = 0;
  char* List;
  Module* TmpMod = Self->First;
  ModFunction* TmpFunc;
  for( i = 0; i < Self->Length; i++ ) 
  {
    TmpFunc = TmpMod->FuncList->First;
    for( j = 0; j < TmpMod->FuncList->Length; j++ )
    {
      if( TmpMod->Type != MT_GENERAL )
	length += strlen(TmpMod->Name)+ strlen(TmpFunc->Name)+2; 
      else
	length += strlen(TmpFunc->Name)+1; 
	
      TmpFunc = TmpFunc->Next;  
    } 
    TmpMod = TmpMod->Next; 
  }
  
  List = (char*) malloc( sizeof(char)* (length +1));
  List[0] = '\0';
  TmpMod = Self->First;
  for( i = 0; i < Self->Length; i++ ) 
  {
    TmpFunc = TmpMod->FuncList->First;
    for( j = 0; j < TmpMod->FuncList->Length; j++ )
    {
      if( TmpMod->Type != MT_GENERAL )
      {
	strcat( List, TmpMod->Name );
	strcat( List, "." );
      }
      strcat( List, TmpFunc->Name );
      strcat( List, "\n" );
      
      TmpFunc = TmpFunc->Next;  
    } 
    TmpMod = TmpMod->Next; 
  }
  Self->FunctionList = List;
  return 0;
}

 
/*
char* ConnectFunction( char* Callstring, int Socket, void* Func )
{
  Function* Tmp = (Function*) Func;
  Function* ToCall;
  FunctionList* List = (FunctionList*) Tmp->Argument;
  
  
  // Split the callstring
  //printf("%s\n", Callstring); 

  int Length;
  int retLength;
  char* Argument;
  char* ArgEnd;
  char* ArgString;
  char* FuncName;
  char* FuncNameEnd;
  char* ret = NULL;
   
  Argument = strchr( Callstring, ' ' );
  if( Argument == NULL )
  {
    return Tmp->Error;
  }
  for( ; *Argument == ' '; Argument++ );

  //printf("%s\n", Argument); 
  ArgEnd = strchr( Argument, '>' );
  if( ArgEnd == NULL )
  {
    return Tmp->Error;
  }

  while( Argument != NULL )
  {
    Length = strlen( Argument ) - strlen( ArgEnd );
    if( ret != NULL )
      retLength = strlen(ret);
    else
      retLength = 0;
	    
    ArgString = malloc( sizeof(char) * ( Length + 2 + retLength ));  
    strncpy( ArgString, Argument, Length );
    ArgString[Length] = ' ';
    ArgString[Length+1] = '\0';
    if( retLength > 0 )
    {
      strcat( ArgString, ret );
    }

    FuncNameEnd = strchr( ArgString, ' ' );
    if( FuncNameEnd != NULL )
    {
      Length = strlen( ArgString ) - strlen( FuncNameEnd );
      FuncName = malloc( sizeof(char) * ( Length + 1 ));  
      strncpy( FuncName , ArgString, Length );
      FuncName[Length] = '\0';
    } else
    {
      FuncName = ArgString;
    }
   
    Function* ToCall = FunctionList_GetElementByName( List, FuncName  );
    free( FuncName );
    if( ToCall == NULL || ToCall->Dev == NULL )
    {
      ret = NULL;
      break;
    }
    ret = CallFunction( ArgString, ToCall->Dev ); 
    printf( "%s return: %s\n", ArgString, ret );
    free( ArgString );
    if( *ArgEnd != '\0' )
    {
      Argument = ArgEnd + 1;
      for( ; *Argument == ' '; Argument++ );
      ArgEnd = strchr( Argument, '>' ); 
      if( ArgEnd == NULL )
      {
	ArgEnd = Argument + strlen(Argument);
      }
    }
    else
    {
      Argument = NULL;
    }
    
  }
  return ret; 
}
*/

