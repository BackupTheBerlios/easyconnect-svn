#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// linux usb subsystem
#include <usb.h>


typedef struct vscope Vscope;
struct vscope 
{
  struct usb_dev_handle *vscope_handle;
};

char* ReadChannel( Vscope* Self, int Channel);

/* Init Function:
 * If you need to store information you can use the Device_Init() function
 * to store any stuff you like. Just define yourself a struct and initialize it.
 * The void pointer you return will be passed to the Communicate and Destroy function.
 */
void* Init( char* InitString )
{
  Vscope* New = (Vscope*) malloc( sizeof( Vscope ) );

  unsigned char located = 0;
  struct usb_bus *bus;
  struct usb_device *dev;
  usb_init();

  usb_find_busses();
  usb_find_devices();

  for (bus = usb_busses; bus; bus = bus->next)
  {
    for (dev = bus->devices; dev; dev = dev->next)
    { 
      if (dev->descriptor.idVendor == 0x0400)
      { 
        located++;
        New->vscope_handle = usb_open(dev);
      }
    }
  }

  usb_set_configuration(New->vscope_handle,1);
  usb_claim_interface(New->vscope_handle,0);
  usb_set_altinterface(New->vscope_handle,0);

  return (void*) New;
}

char* Communicate( void* DeviceHandle, int argc, char** argv )
{
  if( DeviceHandle == NULL ) // || String == NULL )
  {
    return NULL;
  }
  Vscope* Tmp = (Vscope*) DeviceHandle;
  
  // This should be read from the config file
  // Anyway compare the name of the called function
  // with yours, so you can determine which function
  // needs to be called
  
  if( strcmp( argv[0], "readchannel" ) == 0 )
  {
    int channel = 0;

    channel = atoi(argv[1]);

    return ReadChannel( Tmp, channel );
    
  }
}

int Destroy( void* DeviceHandle )
{
  if( DeviceHandle == NULL )
  {
    return -1;
  }
  Vscope* Tmp = (Vscope*) DeviceHandle;
  usb_close(Tmp->vscope_handle);

  free( Tmp );
  return 0;
}


char* ReadChannel( Vscope* Self, int Channel )
{
  unsigned char receive_data[2];

  char* Ret = (char*) malloc( sizeof( char) * 3 );

  char send_data[1];
  send_data[0] = (char)Channel;

  usb_bulk_write(Self->vscope_handle,2,send_data,1,1);
 
  usb_bulk_read(Self->vscope_handle,0x83,receive_data,2,1);

  sprintf(Ret,"%i\n",((int)receive_data[1]*255)+(int)receive_data[0]);

  return Ret;
}
