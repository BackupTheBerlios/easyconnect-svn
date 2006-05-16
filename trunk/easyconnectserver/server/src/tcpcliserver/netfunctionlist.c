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

#include "netfunctionlist.h"



NetFunction* NetFunction_Init( char* Command, 
			 char*( *Callback )( char*, int, void* ),
			 void* Parameter )
{
  NetFunction* Self = (NetFunction*) malloc( sizeof( NetFunction ));
  Self->Command = (char*) malloc( sizeof( char ) * ( strlen( Command ) +1 ) );
  strcpy( Self->Command, Command );
  Self->Parameter = Parameter;
  Self->Callback = Callback;

  Self->Index = 0;
  Self->Next = NULL;
  Self->Previous = NULL;
  return Self;
}



NetFunctionList* NetFunctionList_Init()
{
  NetFunctionList* Self = (NetFunctionList*) malloc( sizeof( NetFunctionList ));
  Self->Length = 0;
  Self->First = NULL;
  Self->Last = NULL;
  return Self;
}

int NetFunctionList_RegisterFunction( NetFunctionList* Self, 
				   char* Command,
				   char*( *Callback )( char*, int, void* ),
				   void* Parameter )
{
  NetFunction* Tmp = Self->Last;
  Self->Last = NetFunction_Init( Command, Callback, Parameter );
  Self->Last->Next = NULL;
  Self->Last->Index = 0;
  if( Tmp != NULL )
  {
    Self->Last->Previous = Tmp;
    Tmp->Next = Self->Last;
    Self->Last->Index = Tmp->Index++;
  }
  Self->Length++;
  if( Self->First == NULL )
  {
    Self->First = Self->Last;
  }
  return Self->Length;
}

int NetFunctionList_UnRegisterFunction( NetFunctionList* Self, char* Command )
{
  // Find function and unlink it
  return 0;
}

NetFunction* NetFunctionList_FindFunction( NetFunctionList* Self, char* Command )
{
  NetFunction* Tmp = Self->First;
  int i;
  for( i = 0; i < Self->Length; i++ )
  {
    if( strcmp( Command, Tmp->Command ) == 0 )
    {
      return Tmp;
    }
    Tmp = Tmp->Next;    
  }
  return NULL;
}

char* NetFunctionList_LikeFunctions( NetFunctionList* Self, char* Command )
{
  // Here comes the wizardry with regular expressions 
}
