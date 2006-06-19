
import socket

class easyConnect(object):
  def __init__( self, ip="127.0.0.1", port=9090 ):
    self.s = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
    self.s.connect(( ip,port ))
    
  def Call( self, command ):
    self.s.send( command )
    i = 1
    while i:
      j = self.s.recv( 1000 )
      if strlen(j):
	ret+=j
      else:
	i =0

    return ret  


  def ListMethods( self ):
    return self.Call( "ListFunctions" )    


  def MethodHelp( self, method ):
    return self.Call( "Help "+method )

  def __del__(self):
    self.s.close()
