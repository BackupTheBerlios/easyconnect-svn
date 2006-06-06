#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// linux usb subsystem
#include <usb.h>
#include "vscopedevice.h" 
#include "tqueue.h"


void *EndlessDataCollection(void* DeviceHandle);

typedef struct devicestate DeviceState;
struct devicestate 
{
  VScope    *vscope;
  TQueue    *data;
  pthread_t thread;
  int	    thread_id;
};

/* Init Function:
 * If you need to store information you can use the Device_Init() function
 * to store any stuff you like. Just define yourself a struct and initialize it.
 * The void pointer you return will be passed to the Communicate and Destroy function.
 */
void* Init( char* InitString )
{
  DeviceState* New = (DeviceState*) malloc( sizeof( DeviceState ) );

  New->vscope = openVScope();
  New->data = TQueue_Init();

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
     SetVScopeMode(Tmp->vscope,MODE_COUNTER);
     StartVScope(Tmp->vscope);
     Tmp->thread_id = pthread_create(&Tmp->thread, NULL, EndlessDataCollection, (void*)Tmp);
  }
  if( strcmp( argv[0], "stop" ) == 0 )
  {
     StopVScope(Tmp->vscope);
     pthread_cancel(Tmp->thread_id);
  }
}

int Destroy( void* DeviceHandle )
{
  if( DeviceHandle == NULL )
  {
    return -1;
  }
  DeviceState* Tmp = (DeviceState*) DeviceHandle;
  closeVScope(Tmp->vscope);
  free( Tmp );
  return 0;
}


void *EndlessDataCollection(void* DeviceHandle)
{
  char buffer[20000]; 
  DeviceState* Tmp = (DeviceState*) DeviceHandle;
  while(1)
  {
    //sleep(1)
    //printf("thread running\n");
    readVScopeData(Tmp->vscope, buffer, 20000);
    //TQueue_AddElement(Tmp->data,strdup(buffer));
  }
}
