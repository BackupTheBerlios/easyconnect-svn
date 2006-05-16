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


#include "devicefunctions.h"



char* CommunicateCallback( char* Callstring, int Socket, void* Parameter )
{
  Function* Func = (Function*) Parameter;
  Device* Dev = Func->Dev;
  
  char* Argument = strchr( Callstring, '.' );
  Argument++;
  //printf( "Argument: %s\n",Argument );

  // Count the arguments
  int NumberofArgs = 1;
  char* Tmp = strchr( Argument, ' ' );
  while( Tmp != NULL )
  {
    Tmp++;
    if( *Tmp == '\0' )
      break;
    if( *Tmp == ' ' )
      continue;

    NumberofArgs++;
    Tmp = strchr( Tmp, ' ' );
  }
  
  char** argv = (char**) malloc( sizeof( char*) * NumberofArgs );
  int argc = NumberofArgs;
  
  char* Begin = Argument;
  char* End = Argument+ strlen(Argument);
  int i = 0;
  int j = 0;
  int Length = 0;
  Tmp = Begin;
  for( i = 0; i < NumberofArgs; i++ )
  {
    for(j = 0; Tmp+j < End && Tmp[j] != ' '; j++ );
    Tmp = Tmp + j;
    
    //printf("Begin: %s, Tmp: %s, End: %s\n", Begin, Tmp, End );

    Length = strlen( Begin ) - strlen( Tmp );
    argv[i] = (char*) malloc( sizeof( char) * ( Length +1 ) );
    strncpy( argv[i], Begin, Length );
    argv[i][Length] = '\0';
 
    for( j = 0; Tmp+j < End && Tmp[j] == ' '; j++ ); 
    Tmp = Tmp + j;
    Begin = Tmp;
  }
   
  //for( j = 0; j < NumberofArgs; j++ )
  //  printf("Arg %i: %s\n", j, argv[j] );
  
  //printf("\n");
  char* Ret = Dev->Communicate( Dev->Handle, argc, argv );  
  
  /*
  if( Ret != NULL )
  {
    Ret = (char*) realloc( Ret,strlen(Ret)+3 );
    Ret[strlen(Ret)] = '\n';
    Ret[strlen(Ret)+1] = '\n';
    Ret[strlen(Ret)+2] = '\0';
  }
  */

  if( argv != NULL )
  {
    for( i = 0; i < argc; i++ )
    {
      if( argv[i] != NULL )
      {
	free( argv[i] );
      }
    }
    free( argv );
  }
  return Ret;
}



int AddDeviceFromPath( DeviceList* List, char* DeviceCfgPath, FunctionList* FuncList )
{
  if( List == NULL || DeviceCfgPath == NULL || FuncList == NULL )
  {
    return -1;
  }  
  char* DeviceName = DeviceCfg_GetName( DeviceCfgPath );

  Device* TmpDev = Device_Init( DeviceCfg_GetLibPath( DeviceCfgPath ),
			     DeviceName,
		             NULL,
			     DeviceCfg_GetDescription( DeviceCfgPath ),
			     DeviceCfg_GetInitString( DeviceCfgPath ) );
  
  
  DeviceList_Add( List, TmpDev );
  
  int Length = DeviceFunctionCfg_GetNumberofFunctions( DeviceCfgPath );
  int i;
  char* FunctionName;
  char* NewName;
  Function* TmpFunc;
  for( i = 0; i < Length; i++ )
  {
    // Change FunctionName to DeviceName.FunctionName
    FunctionName = DeviceFunctionCfg_GetName( i, DeviceCfgPath );
    NewName = malloc( sizeof(char) * ( strlen( FunctionName ) + strlen( DeviceName ) + 2 ) );
    sprintf( NewName, "%s.%s", DeviceName, FunctionName );
    TmpFunc = Function_Init( NewName ,
			  DeviceFunctionCfg_GetError( i, DeviceCfgPath ),
			  DeviceFunctionCfg_GetHelp( i, DeviceCfgPath ),NULL,NULL,
			  CommunicateCallback, TmpFunc );
    TmpFunc->Dev = TmpDev;
    FunctionList_AddFunction( FuncList, TmpFunc );
    
  }
  return Length;
  
}
