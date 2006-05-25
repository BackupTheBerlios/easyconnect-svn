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


#include "devicelist.h"

Device* Device_Init( char* LibFilePath, char* Name, char* Type,
		     char* Description, char* InitString )
{
  if( LibFilePath == NULL || Name == NULL )
  {
    return NULL;
  } 
  
  Device* Self = (Device*) malloc( sizeof( Device ) );
  
  Self->Name = (char*) malloc( sizeof( char )* (strlen( Name )+1)  );
  strcpy( Self->Name , Name );

  if( Type != NULL )
  {
    Self->Type = (char*) malloc( sizeof( char )* (strlen( Type )+1)  );
    strcpy( Self->Type , Type );
  }
    
  Self->Description = (char*) malloc( sizeof( char )* (strlen( Description )+1)  );
  strcpy( Self->Description , Description );

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

char* Device_Communicate( Device* Self, int argc, char** argv )
{
  if( Self == NULL )
  {
    return NULL;
  }	  
  return Self->Communicate( Self->Handle, argc, argv );
}

//int Device_AddFunctions( Device* Self, FunctionList* List ); 

int Device_Destroy( Device* Self )
{
  if( Self == NULL )
  {
    return -1;
  }	  
  Self->Destroy( Self->Handle );
  
  dlclose( Self->LibHandle );

  free( Self->Name );
  free( Self->Type );
  free( Self->Description );
  free( Self );
  return 0;
}



///////////////////////////////////////////////////////////////
// 
// DeviceList
//
///////////////////////////////////////////////////////////////


DeviceList* DeviceList_Init( )
{
  DeviceList* Self = (DeviceList*) malloc( sizeof( DeviceList ) );

  Self->First = NULL;
  Self->Last = NULL;

  Self->Length = 0;

  return Self;

}



int DeviceList_Add( DeviceList* Self, Device* Dev )
{
  if( Dev == NULL )
  {
    return -1;
  }

  if( Self->Length == 0 )
  {
    Self->First = Dev;
    Self->Last = Dev;
  } else
  {
    Self->Last->Next = Dev;
    Dev->Previous = Self->Last;
    Self->Last = Dev;
  }
  Self->Length++;
}

int DeviceList_Destroy( DeviceList* Self )
{
  if( Self == NULL )
  {
    return -1;
  }
  
  int i;
  Device* Tmp = Self->Last;
  for( i = 0; i < Self->Length; i++ )
  {
    Self->Last = Tmp->Previous;
    Device_Destroy( Tmp );
    Tmp = Self->Last;
  }
 
  free( Self );
  return 0; 
  
}


