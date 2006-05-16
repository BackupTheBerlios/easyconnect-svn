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

Function* Function_Init( char* Name, char* Error, char* Help,
			 char* Param, char* ParamConvert,
			 char* (*Callback)(char*, int, void*), void* Argument )
{
  if( Name == NULL )
  {
    return NULL;
  }
  Function* Self = (Function*) malloc( sizeof( Function ));

  Self->Name = (char*) malloc( sizeof(char) * strlen( Name )+1);
  strcpy( Self->Name, Name );

  if( Error != NULL )
  {
    Self->Error = (char*) malloc( sizeof(char) * strlen( Error )+1);
    strcpy( Self->Error, Error );
  } else
  {
    Self->Error = NULL;
  }

  if( Help != NULL )
  {
    Self->Help = (char*) malloc( sizeof(char) * strlen( Help )+1);
    strcpy( Self->Help, Help );
  } else
  {
    Self->Help = NULL;
  }

  if( Param != NULL )
  {
    Self->Param = (char*) malloc( sizeof(char) * strlen( Param )+1);
    strcpy( Self->Param, Param );
  } else
  {
    Self->Param = NULL;
  }

  if( ParamConvert != NULL )
  {
    Self->ParamConvert = (char*) malloc( sizeof(char) * strlen( ParamConvert )+1);
    strcpy( Self->ParamConvert, ParamConvert );
  } else
  {
    Self->Param = NULL;
  }

  Self->Callback = Callback;
  Self->Argument = Argument;
  
  Self->Next = NULL;
  Self->Previous = NULL;
  
  return Self;

}

int Function_Destroy( Function* Self )
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
  if( Self->Param != NULL )
  {
    free( Self->Param );
  } 
  if( Self->ParamConvert != NULL )
  {
    free( Self->ParamConvert );
  }
  free( Self );
  return 0;
}



FunctionList* FunctionList_Init()
{
  FunctionList* Self = (FunctionList*) malloc( sizeof( FunctionList ));
  Self->First = NULL;
  Self->Last = NULL;
  Self->Length = 0;
  return Self;
}

int FunctionList_AddFunction( FunctionList* Self, Function* Func )
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

int FunctionList_RegisterAllFunctions( FunctionList* Self, TcpCliServer* Server )
{
  if( Self == NULL || Server == NULL )
  {
    return -1;
  }
  
  FunctionList_GenerateList( Self );
  int i = 0;
  Function* Tmp = Self->First;
  for( i = 0; i < Self->Length; i++ )
  {
    TcpCliServer_RegisterFunction( Server, Tmp->Name, Tmp->Callback, Tmp );
    Tmp = Tmp->Next;
  }
  return 0;
}

int FunctionList_GenerateList( FunctionList* Self )
{
  if( Self == NULL )
  {
    return -1;
  }

  Function* Tmp = Self->First;
  int StringLength = 0;
  int i;
  for( i = 0; i < Self->Length; i++ )
  {
    StringLength += strlen(Tmp->Name)+1;
    Tmp = Tmp->Next; 
  }
  
  Self->List = (char*) malloc( sizeof(char*) * StringLength + 1 );

  Self->List[0] = 0;
  Tmp = Self->First;
  
  for( i = 0; i < Self->Length; i++ )
  {
    strcat( Self->List, Tmp->Name );
    strcat( Self->List, "\n" );
    Tmp = Tmp->Next; 
  }  
  Self->List[StringLength] = '\0';
  return 0; 
}


Function* FunctionList_GetElementByName( FunctionList* Self, char* FunctionName )
{
  if( Self == NULL )
  {
    return NULL;
  }
  int i;
  Function* Tmp = Self->First;
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


int FunctionList_Destroy( FunctionList* Self )
{
  if( Self == NULL )
  {
    return -1;
  }
  Function* Tmp;  
  int i;
  for( i = 0; i < Self->Length; i++ )
  { 
    Tmp = Self->Last;
    Self->Last = Tmp->Previous;
    Function_Destroy( Tmp ); 
  }
  return 0;
}
