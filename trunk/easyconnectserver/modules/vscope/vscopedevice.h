/*
An API for the vscopedevice
Copyright (C) 2006 Benedikt Sauter <sauter@ixbat.de> 

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <usb.h>



#define MODE_NONE	  0x00
#define MODE_COUNTER	  0x01
#define MODE_LOGIC	  0x02
#define MODE_1CHANNELAD	  0x03


#define STATE_DONOTHING	  0x01
#define STATE_RUNNING	  0x02
#define STATE_TRIGGER	  0x03


#define CMD_SETMODE	  0x01
#define CMD_STARTSCOPE	  0x02
#define CMD_STOPSCOPE	  0x03
#define CMD_GETSCOPEMODE  0x04
#define CMD_GETSCOPESTATE 0x05
#define CMD_GETFIFOLOAD	  0x06


typedef struct vscope VScope;

struct vscope {
  struct usb_dev_handle *vscope_handle;
//  struct usb_device *vscope_device;
};

VScope* openVScope();
int closeVScope(VScope* self);

int sendVScopeCommand(VScope* self,char *command);

int readVScopeData(VScope* self, char* data,int length);

int readVScopeResults(VScope* self,char *data);


void SetVScopeMode(VScope* self,char state);

void StartVScope(VScope* self);
void StopVScope(VScope* self);

int GetVScopeState(VScope* self);
int GetVScopeMode(VScope* self);
int GetVScopeFIFOLoad(VScope* self);


