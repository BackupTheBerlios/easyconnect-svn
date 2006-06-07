from vcd import *
 
class module:
  def __init__( self, string=None ):
    #self.st = Storage(string) 
    pass

  def create( self, string=None ):
    if( string == None ):
      return "No Param"
    str = string.split(' ')   
    print str
    return
 
  def store( self, string=None ):
    if( string == None ):
      return "No Param"
    self.st.Save( string ) 
    return "saved"

  def __introspection__(self):
    return ["create","store","retrieve"]


def __introspection__():
  return ["module"] 

if __name__ == "__main__":
  print __introspection__()

