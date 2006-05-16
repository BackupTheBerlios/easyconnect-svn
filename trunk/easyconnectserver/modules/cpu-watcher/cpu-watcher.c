#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct cpuwatcher CpuWatcher;
struct cpuwatcher
{
  char* FilePath;
};

char* GetCurrLoad( CpuWatcher* Self );
char* GetAvgLoad( CpuWatcher* Self, int Interv);

/* Init Function:
 * If you need to store information you can use the Device_Init() function
 * to store any stuff you like. Just define yourself a struct and initialize it.
 * The void pointer you return will be passed to the Communicate and Destroy function.
 */
void* Init( char* InitString )
{
  CpuWatcher* New = (CpuWatcher*) malloc( sizeof( CpuWatcher ) );
  New->FilePath = (char*) malloc( sizeof( char ) * ( strlen( InitString )+1) );
  strcpy( New->FilePath, InitString );
  //printf("The Cpu Watcher is watching you\n");
  return (void*) New;
}

char* Communicate( void* DeviceHandle, int argc, char** argv )
{
  if( DeviceHandle == NULL ) // || String == NULL )
  {
    return NULL;
  }
  CpuWatcher* Tmp = (CpuWatcher*) DeviceHandle;
  
  // This should be read from the config file
  // Anyway compare the name of the called function
  // with yours, so you can determine which function
  // needs to be called
  if( strcmp( argv[0], "GetCurrentLoad" ) == 0 )
  {
    return GetCurrLoad( Tmp );  
  } else
  if( strcmp( argv[0], "GetAvgLoad" ) == 0 )
  {
    int param = 0;
    if( strcmp( argv[1], "0" ) == 0 )
      param = 0;
    else if ( strcmp( argv[1], "1" ) == 0 )
      param = 1;

    return GetAvgLoad( Tmp, param );
  }
  
}

int Destroy( void* DeviceHandle )
{
  if( DeviceHandle == NULL )
  {
    return -1;
  }
  CpuWatcher* Tmp = (CpuWatcher*) DeviceHandle;
  free( Tmp->FilePath );
  free( Tmp );
  return 0;
}


char* GetCurrLoad( CpuWatcher* Self )
{
  char* FileString = (char*) malloc( sizeof(char) * 256 );
  char* Ret = (char*) malloc( sizeof( char) * 6 );
  
  FILE* fp = NULL;
  fp = fopen(Self->FilePath, "r");
  if( fp == NULL )
  {
    return NULL;
  }

  fgets( FileString, 256, fp );
  fclose(fp);

  strncpy( Ret, FileString, 4 );
  Ret[4] = '\n';
  Ret[5] = '\0';
  free( FileString );
  return Ret;

}

char* GetAvgLoad( CpuWatcher* Self, int Interv )
{
  char* FileString = (char*) malloc( sizeof(char) * 256 );
  char* Ret = (char*) malloc( sizeof( char) * 6 );
  
  FILE* fp = NULL;
  fp = fopen(Self->FilePath, "r");
  if( fp == NULL )
  {
    return NULL;
  }

  fgets( FileString, 256, fp );
  fclose(fp);
 
  if( Interv == 1 ) 
    FileString += 10;
  else
    FileString += 5;
  
  strncpy( Ret, FileString, 4 );
  Ret[4] = '\n';
  Ret[5] = '\0';
  free( FileString );
  return Ret;

}

