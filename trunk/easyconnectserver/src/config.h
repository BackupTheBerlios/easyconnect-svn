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


#ifndef __CONFIG_H_
#define __CONFIG_H_

#include "iniparser/iniparser.h"
#include <stdlib.h>
#include <string.h>

typedef struct generalcfg GeneralCfg;
struct generalcfg
{
  char* Ip;
  char* Error;
  
  // Raw:
  short RawEnable;
  int RawPort;

  // CLI:
  short CliEnable;
  int CliPort;

  char* FunctionCfgFile;
};

GeneralCfg* GeneralCfg_Init( char* GeneralCfgFile );
int GeneralCfg_Destroy( GeneralCfg* Self );

char* DeviceCfg_GetField( char* FieldName, char* DeviceCfgPath );  

char* DeviceCfg_GetName( char* DeviceCfgPath );
char* DeviceCfg_GetInitString( char* DeviceCfgPath );
char* DeviceCfg_GetDescription( char* DeviceCfgPath );
char* DeviceCfg_GetLibPath( char* DeviceCfgPath );
char* DeviceCfg_GetLibType( char* DeviceCfgPath );

int DeviceFunctionCfg_GetNumberofFunctions( char* DeviceCfgPath );

char* DeviceFunctionCfg_GetField( int FunctionNumber, char* FieldName, char* DeviceCfgPath );  
char* DeviceFunctionCfg_GetName( int FunctionNumber, char* DeviceFunctionCfgPath );
char* DeviceFunctionCfg_GetError( int FunctionNumber, char* DeviceFunctionCfgPath );
char* DeviceFunctionCfg_GetHelp( int FunctionNumber, char* DeviceFunctionCfgPath );

#endif
