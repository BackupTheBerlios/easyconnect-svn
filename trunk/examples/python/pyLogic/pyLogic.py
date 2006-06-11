#! /usr/bin/python
import sys
sys.path.append( "../../../easyconnectlibs/python/")
from easyconnectlib import *

class scope:
  def __init__( self, FilePath=None, Name="Test" ):
    if( FilePath == None ):
      return
    self.Connection = easyConnect()
    ret = self.Connection.Call("storage.create vcd "+FilePath+" "+Name+" Bit0:bit Bit1:bit Bit2:bit Bit3:bit Bit4:bit Bit5:bit Bit6:bit Bit7:bit")
    self.id = int(ret)

  def Store( self):
    self.Connection.Call("Connect vscope.data > storage.store "+str(self.id))
    return

  def Close( self ):
    return self.Connection.Call("storage.close "+str(self.id))

if __name__ == '__main__':
  Tmp = scope(sys.argv[1], sys.argv[2]) 
  Tmp.Store()
  Tmp.Close()
