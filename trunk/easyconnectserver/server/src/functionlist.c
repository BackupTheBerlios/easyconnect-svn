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


#include "functionlist.h"

ModFunction* ModFunction_Init( char* Name, char* Error, char* Help )
{
  if( Name == NULL )
  {
    return NULL;
  }
  
  ModFunction* Self = (ModFunction*) malloc( sizeof( ModFunction ));
  
  Self->Name = strdup( Name );
  
  if( Error != NULL )
  {
    Self->Error = strdup( Error );
  } else
  {
    Self->Error = NULL;
  }

  if( Help != NULL )
  {
    Self->Help = strdup( Help );
  } else
  {
    Self->Help = NULL;
  }
  
  Self->Next = NULL;
  Self->Previous = NULL;
  return Self;
}

int ModFunction_Destroy( ModFunction* Self )
{
  if( Self == NULL )
  {
    return -1;
  }
  if( Self->Help != NULL )
  {
    free( Self->Help );
  }
  if( Self->Error != NULL )
  {
    free( Self->Error );
  }
  if( Self->Name != NULL )
  {
    free( Self->Name );
  }

  free( Self );
  return 0;
}



ModFunctionList* ModFunctionList_Init()
{
  ModFunctionList* Self = (ModFunctionList*) malloc( sizeof( ModFunctionList ));
  Self->First = NULL;
  Self->Last = NULL;
  Self->Length = 0;
  return Self;
}

int ModFunctionList_AddFunction( ModFunctionList* Self, ModFunction* Func )
{
  if( Self == NULL )
  {
    return -1;
  }

  if( Self->Length == 0 )
  {
    Self->First = Func;
    Self->Last = Func;
  }
  else
  {
    Self->Last->Next = Func;
    Func->Previous = Self->Last->Next;
    Self->Last = Func;
  }

  Self->Length++;
  return Self->Length;
}


ModFunction* ModFunctionList_GetFunction( ModFunctionList* Self, char* FunctionName )
{
  if( Self == NULL )
  {
    return NULL;
  }
  int i;
  ModFunction* Tmp = Self->First;
  for( i = 0; i < Self->Length; i++ )
  {
    if( strcmp( Tmp->Name, FunctionName ) == 0 )
    {
      return Tmp;
    }
    Tmp = Tmp->Next;
  }
  return NULL;
}


int ModFunctionList_Destroy( ModFunctionList* Self )
{
  if( Self == NULL )
  {
    return -1;
  }
  ModFunction* Tmp;  
  int i;
  for( i = 0; i < Self->Length; i++ )
  { 
    Tmp = Self->Last;
    Self->Last = Tmp->Previous;
    ModFunction_Destroy( Tmp ); 
  }
  return 0;
}
