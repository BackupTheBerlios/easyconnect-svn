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

#include <signal.h>
#include "config.h"
#include "tcpcliserver/tcpcliserver.h"
#include "modulelist.h"

void sigfunc(int sig)
{
  if(sig != SIGINT)
  return;
  exit (EXIT_FAILURE);
}

char* DoNothing( char* Callstring, int Socket, void* Param )
{
  return NULL;
}

int main( int argv, char** argc )
{
  char* Ignore[] = {"\004"};
  int size = 1;
  int i;
  signal(SIGINT, sigfunc);
  GeneralCfg* Tmp = GeneralCfg_Init( "../../config/easyconnect.conf" );
  //Function* Func = Function_Init( "../config/functions.cfg", "help" );
  
  ModuleList* List = ModuleList_Init();
  // Add General functions
  ModuleList_AddFromPath( List, "../../config/ecfunc.conf" );

  ModuleList_AddFromPath( List, "../../config/devices/cpu-watcher.conf" );
  ModuleList_AddFromPath( List, "../../config/devices/storage.conf" );
  ModuleList_AddFromPath( List, "../../config/devices/vscope.conf" );
  
  TcpCliServer* New = TcpCliServer_Init();
  TcpCliServer_SetDefaultError( New, Tmp->Error ); 
  for( i = 0; i < size; i++ )
  {
    TcpCliServer_RegisterFunction( New, Ignore[i], DoNothing, NULL ); 
  }

  
  ModuleList_RegisterAll( List, New );
  //RegisterFunctions( New, "../config/functions.cfg" );
  
  TcpCliServer_InitRaw( New, Tmp->Ip, Tmp->RawPort, 1000 );
  
  while(1)
  {
    sleep(1000);
  }
  return 0;
}
