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



#include "config.h"

GeneralCfg* GeneralCfg_Init( char* GeneralCfgFile )
{
  if( GeneralCfgFile == NULL )
  {
    return NULL;
  }

  // Check if config file is present
  FILE* FP = fopen( GeneralCfgFile, "r" );
  if( FP == NULL )
  {
    return NULL;
  }
  fclose( FP );

  
  GeneralCfg* Self = (GeneralCfg*) malloc( sizeof( GeneralCfg ) );
 
  char* Ip = NULL;
  char* Error = NULL;
  
  cfg_opt_t opts[] =
  {
    CFG_STR("ip","127.0.0.1", CFGF_NONE),
    CFG_INT("raw_port", 9090, CFGF_NONE),
    CFG_INT("raw_enable", 1, CFGF_NONE),
    CFG_INT("cli_port", 9099, CFGF_NONE),
    CFG_INT("cli_enable", 1, CFGF_NONE), 
    CFG_STR("default_error","Command not found.", CFGF_NONE),
    CFG_END() 
  };

  cfg_t* cfg;
  cfg = cfg_init( opts, CFGF_NONE );
  cfg_parse( cfg, GeneralCfgFile );
  
  Ip = cfg_getstr( cfg, "ip" );
  Self->Ip = (char*) malloc( sizeof(char) * strlen( Ip )+1);
  strcpy( Self->Ip, Ip ); 
 
  Error = cfg_getstr( cfg, "default_error" );
  Self->Error = (char*) malloc( sizeof(char) * (strlen( Error )+1) );
  strcpy( Self->Error, Error );
  
  Self->RawPort = cfg_getint( cfg, "raw_port" );
  Self->RawEnable = cfg_getint( cfg, "raw_enable" );
  Self->CliEnable = cfg_getint( cfg, "cli_enable" ); 
  Self->CliPort = cfg_getint( cfg, "cli_port" );
  
  cfg_free_value( opts ); 
  cfg_free(cfg);
  return Self;
}

int GeneralCfg_Destroy( GeneralCfg* Self )
{
  return 0;
}

char* DeviceCfg_GetField( char* FieldName, char* DeviceCfgPath )
{
  cfg_opt_t function_opts[] =
  {
    CFG_STR("name", 0, CFGF_NONE),
    CFG_STR("help", 0, CFGF_NONE),
    CFG_STR("error", 0, CFGF_NONE),
    CFG_END() 
  };

  cfg_opt_t opts[] =
  {
    CFG_STR("name", 0, CFGF_NONE),
    //CFG_STR("Type", 0, CFGF_NONE),
    CFG_STR("description", 0, CFGF_NONE),
    CFG_STR("init-string", 0, CFGF_NONE),
    CFG_STR("libfile", 0, CFGF_NONE),
    CFG_SEC("function", function_opts, CFGF_MULTI | CFGF_TITLE ),
    CFG_END() 
  };

  cfg_t* cfg;
  cfg_t* fn;
  cfg = cfg_init( opts, CFGF_NONE );
  cfg_parse( cfg, DeviceCfgPath );
  
  //fn = cfg_gettsec( cfg, "function", FunctionName );

  char* field = cfg_getstr( cfg, FieldName );
  char* NewField = NULL;
  if( field != NULL )
  {
    NewField = (char*) malloc( sizeof(char) * strlen( field )+1);
    strcpy( NewField, field ); 
  }

  cfg_free_value( opts ); 
  cfg_free(cfg);

  return NewField;
}

char* DeviceCfg_GetName( char* DeviceCfgPath )
{
  return DeviceCfg_GetField( "name", DeviceCfgPath );
}

char* DeviceCfg_GetInitString( char* DeviceCfgPath )
{
  return DeviceCfg_GetField( "init-string", DeviceCfgPath );
}

char* DeviceCfg_GetDescription( char* DeviceCfgPath )
{
  return DeviceCfg_GetField( "description", DeviceCfgPath );
}

char* DeviceCfg_GetLibPath( char* DeviceCfgPath )
{
  return DeviceCfg_GetField( "libfile", DeviceCfgPath );
}

int DeviceFunctionCfg_GetNumberofFunctions( char* DeviceCfgPath )
{
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
    CFG_STR("name", 0, CFGF_NONE),
    //CFG_STR("Type", 0, CFGF_NONE),
    CFG_STR("description", 0, CFGF_NONE),
    CFG_STR("init-string", 0, CFGF_NONE),
    CFG_STR("libfile", 0, CFGF_NONE),
    CFG_SEC("function", function_opts, CFGF_MULTI | CFGF_TITLE ),
    CFG_END() 
  };

  cfg_t* cfg;
  cfg_t* fn;
  cfg = cfg_init( opts, CFGF_NONE );
  cfg_parse( cfg, DeviceCfgPath );
  
  //fn = cfg_gettsec( cfg, "function", FunctionName );

  int Length = cfg_size( cfg, "function" ); 
  cfg_free_value( opts ); 
  cfg_free(cfg);

  return Length;

}

char* DeviceFunctionCfg_GetField( int FunctionNumber, char* FieldName, char* DeviceCfgPath )
{
  cfg_opt_t function_opts[] =
  {
    CFG_STR("name", 0, CFGF_NONE),
    CFG_STR("help", 0, CFGF_NONE),
    CFG_STR("error", 0, CFGF_NONE),
    CFG_END() 
  };

  cfg_opt_t opts[] =
  {
    CFG_STR("name", 0, CFGF_NONE),
    //CFG_STR("Type", 0, CFGF_NONE),
    CFG_STR("description", 0, CFGF_NONE),
    CFG_STR("init-string", 0, CFGF_NONE),
    CFG_STR("libfile", 0, CFGF_NONE),
    CFG_SEC("function", function_opts, CFGF_MULTI | CFGF_TITLE ),
    CFG_END() 
  };

  cfg_t* cfg;
  cfg_t* fn;
  cfg = cfg_init( opts, CFGF_NONE );
  cfg_parse( cfg, DeviceCfgPath );
  
  fn = cfg_getnsec( cfg, "function", FunctionNumber );

  char* field = cfg_getstr( fn, FieldName );
  char* NewField = NULL;
  if( field != NULL )
  {
    NewField = (char*) malloc( sizeof(char) * strlen( field )+1);
    strcpy( NewField, field ); 
  }

  cfg_free_value( opts ); 
  cfg_free(cfg);

  return NewField;

}
	
char* DeviceFunctionCfg_GetName( int FunctionNumber, char* DeviceFunctionCfgPath )
{
  return DeviceFunctionCfg_GetField( FunctionNumber, "name", DeviceFunctionCfgPath );
}

char* DeviceFunctionCfg_GetError( int FunctionNumber, char* DeviceFunctionCfgPath )
{
  return DeviceFunctionCfg_GetField( FunctionNumber, "error", DeviceFunctionCfgPath );
}
	
char* DeviceFunctionCfg_GetHelp( int FunctionNumber, char* DeviceFunctionCfgPath )
{
  return DeviceFunctionCfg_GetField( FunctionNumber, "help", DeviceFunctionCfgPath );
}


