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

#include "generalfunctions.h"

char* HelpFunction( char* Callstring, int Socket, void* Func )
{
  Function* Tmp = (Function*) Func;
  Function* ArgFunc;
  FunctionList* List = (FunctionList*) Tmp->Argument;
  
  char* Argument = strchr( Callstring, ' ' );
  
  if( Argument == NULL )
  {  
    char* ret = (char*) malloc( strlen(Tmp->Help)+1 );
    strcpy( ret, Tmp->Help );
    strcat( ret, "\0" );
    return ret;
  }
  else
  {
    Argument++;
    ArgFunc = FunctionList_GetElementByName( List, Argument );  
    if( ArgFunc == NULL )
    {
      return NULL;
    }
    char* ret = (char*) malloc( strlen(ArgFunc->Help)+1 );
    strcpy( ret, ArgFunc->Help );
    strcat( ret, "\0" );
    return ret;
  }
}

char* ListFunctionsFunction( char* Callstring, int Socket, void* Func )
{
  Function* Tmp = (Function*) Func;
  FunctionList* List = (FunctionList*) Tmp->Argument;
  char* ret = (char*) malloc( strlen(List->List)+1 );
  strcpy( ret, List->List );
  strcat( ret, "\0" );
  return ret;
 
}

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


Function* GetFunctionFromFile( char* FileName, char* FunctionIdentifier, 
			       char* (*Callback)(char*, int, void*), void* Argument )
{
  if( FileName == NULL )
  {
    return NULL;
  }

  // Check if config file is present
  FILE* FP = fopen( FileName, "r" );
  if( FP == NULL )
  {
    return NULL;
  }
  fclose( FP );

  dictionary* Dict = iniparser_load( FileName );
 
  char* Name = iniparser_getsecstring( Dict, FunctionIdentifier, "name", FunctionIdentifier );
  
  char* Help = iniparser_getsecstring( Dict, FunctionIdentifier, "help", NULL );

  char* Error = iniparser_getsecstring( Dict, FunctionIdentifier, "error", NULL );

  Function* Func = Function_Init( Name, Error, Help, Callback, Argument );
  
  
  iniparser_freedict(Dict);
  return Func; 
}


int AddGeneralFunctionsToList( FunctionList* List, char* FunctionsFile )
{
  FunctionList_AddFunction( List, GetFunctionFromFile( FunctionsFile, "help", HelpFunction, List ));
    
  FunctionList_AddFunction( List, GetFunctionFromFile( FunctionsFile, "listfunctions", ListFunctionsFunction, List ));
  
  FunctionList_AddFunction( List, GetFunctionFromFile( FunctionsFile, "connect", ConnectFunction, List ));
    
}	


