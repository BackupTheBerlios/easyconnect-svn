#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// linux usb subsystem
#include <usb.h>
#include "vscopedevice.h" 
#include "tqueue.h"
#include "base64.h"

#define TEST
enum 
{
  INTERNAL, ONLINE
};

void *EndlessDataCollection(void* DeviceHandle);

typedef struct devicestate DeviceState;
struct devicestate 
{
  VScope    *vscope;
  TQueue    *data;
  pthread_t thread;
  int	    thread_id;
  
  pthread_mutex_t mut;

  int Running;
  int RecordMode;
  int DataCount;
  char Interval;
};

typedef struct vsdata VSData;
struct vsdata
{
  char* Data;
  int Len;
};

/* Init Function:
 * If you need to store information you can use the Device_Init() function
 * to store any stuff you like. Just define yourself a struct and initialize it.
 * The void pointer you return will be passed to the Communicate and Destroy function.
 */
void* Init( char* InitString )
{
  DeviceState* New = (DeviceState*) malloc( sizeof( DeviceState ) );

  #ifndef TEST
  New->vscope = openVScope();
  #endif
  New->data = TQueue_Init();
  pthread_mutex_init (&New->mut,NULL);

  return (void*) New;
}

char* Communicate( void* DeviceHandle, int argc, char** argv )
{
  if( DeviceHandle == NULL ) // || String == NULL )
  {
    return NULL;
  }
  DeviceState* Tmp = (DeviceState*) DeviceHandle;
  
  // This should be read from the config file
  // Anyway compare the name of the called function
  // with yours, so you can determine which function
  // needs to be called
  
  if( strcmp( argv[0], "start" ) == 0 )
  {
    
    //if( Tmp->Running == 1)
    //  return strdup( "Still Running!" );

    //Tmp->Running = 1;
    Tmp->DataCount = atoi(argv[1]);
    if( strcmp(argv[3], "online")==0 )
      Tmp->RecordMode = ONLINE;
    if( strcmp(argv[3], "internal")==0 )
      Tmp->RecordMode = INTERNAL ;
    
    if( !strcmp( argv[2], "5us" ) )
      Tmp->Interval = SAMPLERATE_5US;
    if( !strcmp( argv[2], "10us" ) )
      Tmp->Interval = SAMPLERATE_10US;   
    if( !strcmp( argv[2], "50us" ) )
      Tmp->Interval = SAMPLERATE_50US;
    if( !strcmp( argv[2], "100us" ) )
      Tmp->Interval = SAMPLERATE_100US;
    if( !strcmp( argv[2], "1ms" ) )
      Tmp->Interval = SAMPLERATE_1MS;
    if( !strcmp( argv[2], "10ms" ) )
      Tmp->Interval = SAMPLERATE_10MS;
    if( !strcmp( argv[2], "100ms" ) )
      Tmp->Interval = SAMPLERATE_100MS;
    
    Tmp->thread_id = pthread_create(&Tmp->thread, NULL, EndlessDataCollection, (void*)Tmp);
    return NULL;
  }
  if( strcmp( argv[0], "settrigger" ) == 0 )
  {
    if(argc < 3)
      return strdup("Wrong Arguments!/n");
	      
    int i;
    #ifndef TEST
    DeActivateTrigger( Tmp->vscope );
    #endif
    if(strcmp( argv[1], "edge") == 0 )
    {
      for( i = 0; i < 8; i++ )
      {
        if( argv[2][i] == '0' )
	{
	  #ifndef TEST
	  ActivateEdgeTrigger(Tmp->vscope, i, 0);
	  #endif
	}
        if( argv[2][i] == '1' )
	{
	  #ifndef TEST
	  ActivateEdgeTrigger(Tmp->vscope, i, 1);
	  #endif
	}
      }
      return NULL;
    }
    if(strcmp( argv[1], "value") == 0 )
    {
      char Pattern = 0;
      for( i = 0; i < 8; i++ )
      {
	Pattern = Pattern << 1;
        if( argv[2][i] == '1' )
	  Pattern &= 0x01;
      }
#ifndef TEST
      ActivatePatternTrigger();
#endif
    }
     
	  
  } if( strcmp( argv[0], "getdata" ) == 0 )
  {
    int i, j;
    char* Ret;
    VSData* TmpData = NULL;
    TmpData = (VSData*) TQueue_TryGetElement( Tmp->data );
    if( TmpData == NULL )
      return strdup("no data!\n");
    
    Ret =(char*) encode_b64((unsigned char*)TmpData->Data, TmpData->Len);
    free( TmpData->Data );
    free( TmpData);
    return Ret;

    /*
    char test[20000];
    int k;
    for(k=0;k<20000;k++)
      test[k]=(char)k;
    return (char*) encode_b64((unsigned char*)test, 20000);
    //return 
    */
  }
  return NULL;
}

int Destroy( void* DeviceHandle )
{
  if( DeviceHandle == NULL )
  {
    return -1;
  }
  DeviceState* Tmp = (DeviceState*) DeviceHandle;
#ifndef TEST
  closeVScope(Tmp->vscope);
#endif
  free( Tmp );
  return 0;
}


void *EndlessDataCollection(void* DeviceHandle)
{
  DeviceState* Tmp = (DeviceState*) DeviceHandle;
  VSData* Data = (VSData*)malloc(sizeof(VSData)); 
  Data->Data = (char*)malloc(sizeof(char)*Tmp->DataCount);
  pthread_mutex_lock(&Tmp->mut);
  #ifndef TEST
  Recording(Tmp->vscope, Tmp->Interval, Tmp->DataCount, Data->Data ); 
  #endif
  #ifdef TEST
  int i = 0;
  char j = 0;
  for( i = 0; i < Tmp->DataCount; i++ )
  {
    Data->Data[i] = j;
    j+=1;
  }
  Data->Len = Tmp->DataCount;
  #endif
  pthread_mutex_unlock(&Tmp->mut);

  TQueue_AddElement(Tmp->data,Data);
    
  return;
}




