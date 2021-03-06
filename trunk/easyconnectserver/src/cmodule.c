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


#include "cmodule.h"

CModule* CModule_Init( char* LibFilePath, char* Name, char* InitString )
{
  if( LibFilePath == NULL || Name == NULL )
  {
    return NULL;
  } 
  
  CModule* Self = (CModule*) malloc( sizeof( CModule ) );
  
  Self->Name = strdup( Name ); 

  Self->LibHandle = dlopen( LibFilePath, RTLD_LAZY );
  if( Self->LibHandle == NULL )
  {
    return NULL;
  }  
  
  void* (*InitCustomDevice)( char* InitString );
  const char* error_msg;
  
  InitCustomDevice = dlsym( Self->LibHandle, "Init" );
  error_msg = dlerror();
  if( error_msg )
  {
    fprintf( stderr, "Error locating 'Init' in %s\n%s\n", LibFilePath, error_msg );
    return NULL;
  }
       	
  Self->Handle =(*InitCustomDevice)( InitString ); 
  
  Self->Communicate = dlsym( Self->LibHandle, "Communicate" );
  if( error_msg )
  {
    fprintf( stderr, "Error locating 'Communicate' in %s\n%s\n", LibFilePath, error_msg );
    return NULL;
  }

  Self->Destroy = dlsym( Self->LibHandle, "Destroy" );
  if( error_msg )
  {
    fprintf( stderr, "Error locating 'Destroy' in %s\n%s\n", LibFilePath, error_msg );
    return NULL;
  }   
 
  return Self; 
}

char* CModule_Communicate( CModule* Self, int argc, char** argv )
{
  if( Self == NULL )
  {
    return NULL;
  }	  
  return Self->Communicate( Self->Handle, argc, argv );
}

char* CModule_ExecuteFunction( CModule* Self, char* Function, char* Parameter )
{
  // Count the arguments
  int NumberofArgs = 1;
  char* Tmp = Parameter;
  while( Tmp != NULL && *Tmp != '\0' )
  {
    Tmp++;
    if( *Tmp == ' ' )
      continue;

    NumberofArgs++;
    Tmp = strchr( Tmp, ' ' );
  }
  
  char** argv = (char**) malloc( sizeof( char*) * NumberofArgs );
  int argc = NumberofArgs;
  
  argv[0] = strdup(Function);
  char* Begin = Parameter;
  char* End = Parameter+ strlen(Parameter);
  int i = 0;
  int j = 0;
  int Length = 0;
  Tmp = Begin;
  for( i = 1; i < NumberofArgs; i++ )
  {
    for(j = 0; Tmp+j < End && Tmp[j] != ' '; j++ );
    Tmp = Tmp + j;
    
    Length = strlen( Begin ) - strlen( Tmp );
    argv[i] = (char*) malloc( sizeof( char) * ( Length +1 ) );
    strncpy( argv[i], Begin, Length );
    argv[i][Length] = '\0';
 
    for( j = 0; Tmp+j < End && Tmp[j] == ' '; j++ ); 
    Tmp = Tmp + j;
    Begin = Tmp;
  }
  
  char* Ret = Self->Communicate( Self->Handle, argc, argv );  


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

char* CModule_Callback( char* Callstring, int Socket, void* Parameter )
{
  CModule* Self = (CModule*) Parameter;
  Callstring = strchr( Callstring, '.' ) +1;
  char* Ret;  
  char* FunctionEnd = strchr( Callstring, ' ' );
  if( FunctionEnd == NULL )
    FunctionEnd = Callstring + strlen( Callstring);
  int length = strlen(Callstring)-strlen(FunctionEnd);
  char* Function = (char*) malloc( sizeof(char) * (length+1)); 
  strncpy( Function, Callstring, length );
  Function[length] = '\0';  
  for(;*FunctionEnd != '\0' && *FunctionEnd == ' '; FunctionEnd++);
  Ret = CModule_ExecuteFunction( Self, Function, FunctionEnd); 
  free(Function);
  return Ret;
}

int CModule_Destroy( CModule* Self )
{
  if( Self == NULL )
  {
    return -1;
  }	  
  Self->Destroy( Self->Handle );
  
  dlclose( Self->LibHandle );

  free( Self->Name );
  free( Self );
  return 0;
}

