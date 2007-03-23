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

  dictionary* Dict = iniparser_load( GeneralCfgFile );

  // Reading values from ini file
  Self->Ip = strdup( iniparser_getsecstring( Dict, NULL, "ip", "127.0.0.1" ) );
  Self->Error = strdup( iniparser_getsecstring( Dict, NULL, "default_error", "Command not found.\n" ));
  char* Newline = strstr(Self->Error, "\\n");
  while( Newline != NULL )
  {
    if( *( Newline-1 ) != '\\' )
    {
      *Newline = ' '; 
      *(Newline+1) = '\n';
    }
    Newline = strstr(Self->Error, "\\n");
  }
 
  Self->RawPort = iniparser_getint( Dict, "raw_port", 9090 );
  Self->CliPort = iniparser_getint( Dict, "cli_port", 9099 );
  Self->RawEnable = iniparser_getint( Dict, "raw_enable",1 );
  Self->CliEnable = iniparser_getint( Dict, "cli_enable",1 ); 
  

  iniparser_freedict(Dict);
  return Self;
}

int GeneralCfg_Destroy( GeneralCfg* Self )
{
  return 0;
}

char* DeviceCfg_GetField( char* FieldName, char* DeviceCfgPath )
{
  char* ret = NULL;
  char* RetStr = NULL;
  char* Newline = NULL;
  dictionary* Dict = iniparser_load( DeviceCfgPath );
  ret = iniparser_getsecstring( Dict, NULL, FieldName, NULL );
  if( ret != NULL )
  {
    RetStr = strdup( ret );
    Newline = strstr(RetStr, "\\n");
    while( Newline != NULL )
    {
      if( *( Newline-1 ) != '\\' )
      {
	*Newline = ' '; 
	*(Newline+1) = '\n';
      }
      Newline = strstr(RetStr, "\\n");
    }
  }
  iniparser_freedict(Dict);
  return RetStr;
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

char* DeviceCfg_GetLibType( char* DeviceCfgPath )
{
  char* ret = DeviceCfg_GetField( "libtype", DeviceCfgPath );
  if( ret == NULL )
    return strdup("c");
  
  return ret;
}

int DeviceFunctionCfg_GetNumberofFunctions( char* DeviceCfgPath )
{

//    CFG_STR("name", 0, CFGF_NONE),
    //CFG_STR("Type", 0, CFGF_NONE),
//    CFG_STR("description", 0, CFGF_NONE),
//    CFG_STR("init-string", 0, CFGF_NONE),
//    CFG_STR("libfile", 0, CFGF_NONE),
//    CFG_SEC("function", function_opts, CFGF_MULTI | CFGF_TITLE ),
//    CFG_END() 

  dictionary* Dict = iniparser_load( DeviceCfgPath );
  

  int Length = iniparser_getnsec( Dict );

  iniparser_freedict( Dict );
  return Length;

}

char* DeviceFunctionCfg_GetField( int FunctionNumber, char* FieldName, char* DeviceCfgPath )
{
  char* ret = NULL;
  char* Section = NULL;
  char* Newline;
  dictionary* Dict = iniparser_load( DeviceCfgPath );
  Section = iniparser_getsecname( Dict, FunctionNumber );  
  ret = iniparser_getsecstring( Dict, Section, FieldName, NULL ) ;
  if( ret != NULL )
  {
    Newline = strstr(ret, "\\n");
    while( Newline != NULL )
    {
      if( *( Newline-1 ) != '\\' )
      {
	*Newline = ' '; 
	*(Newline+1) = '\n';
      }
      Newline = strstr(ret, "\\n");
    }
    ret = strdup( ret );
  }
 
  iniparser_freedict( Dict );
  return ret; 

}
	
char* DeviceFunctionCfg_GetName( int FunctionNumber, char* DeviceFunctionCfgPath )
{
  return DeviceFunctionCfg_GetField( FunctionNumber, "name", DeviceFunctionCfgPath );
}

char* DeviceFunctionCfg_GetError( int FunctionNumber, char* DeviceFunctionCfgPath )
{
  char* Ret = DeviceFunctionCfg_GetField( FunctionNumber, "error", DeviceFunctionCfgPath );
  if( Ret == NULL )
    Ret = strdup("\n");

  return Ret;
}
	
char* DeviceFunctionCfg_GetHelp( int FunctionNumber, char* DeviceFunctionCfgPath )
{
  char* Ret = DeviceFunctionCfg_GetField( FunctionNumber, "help", DeviceFunctionCfgPath );
  if( Ret == NULL )
    Ret = strdup("\n");

  return Ret;
 
}


