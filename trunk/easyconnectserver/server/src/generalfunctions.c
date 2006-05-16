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

  cfg_opt_t function_opts[] =
  {
    CFG_STR("name", 0, CFGF_NONE),
    CFG_STR("help", 0, CFGF_NONE),
    CFG_STR("error", 0, CFGF_NONE),
    CFG_STR("parameter", 0, CFGF_NONE),
    CFG_STR("parameter_convert", 0, CFGF_NONE),
    CFG_END() 
  };

  cfg_opt_t opts[] =
  {
    CFG_SEC("function", function_opts, CFGF_MULTI | CFGF_TITLE ),
    CFG_END() 
  };

  cfg_t* cfg;
  cfg_t* fn;
  cfg = cfg_init( opts, CFGF_NONE );
  cfg_parse( cfg, FileName );
  
  fn = cfg_gettsec( cfg, "function", FunctionIdentifier );

  char* Name = cfg_getstr( fn, "name" );
  char* Help = cfg_getstr( fn, "help" );
  char* Error = cfg_getstr( fn, "error" );
  char* Param = cfg_getstr( fn, "parameter" );
  char* ParamConvert = cfg_getstr( fn, "parameter_convert" );

  Function* Func = Function_Init( Name, Error, Help, Param, ParamConvert, Callback, Argument );
  
  
  cfg_free_value( opts ); 
  cfg_free(cfg);
 
  return Func; 
}


int AddGeneralFunctionsToList( FunctionList* List, char* FunctionsFile )
{
  FunctionList_AddFunction( List, GetFunctionFromFile( FunctionsFile, "help", HelpFunction, List ));
    
  FunctionList_AddFunction( List, GetFunctionFromFile( FunctionsFile, "list_functions", ListFunctionsFunction, List ));
    
}	
