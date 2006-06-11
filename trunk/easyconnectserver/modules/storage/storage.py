from vcd import *
import base64 
from struct import *
class module:
  def __init__( self, string=None ):
    self.List = {}
    self.id = 0
    pass

  def create( self, string=None ):
    if( string == None ):
      return "No Parameter.\n"
    str = string.split(' ')   
    if str[0] == 'vcd':
      return self.CreateVCD(str[1:])
    return "No filetype given.\n"
     
  def store( self, string=None ):
    if( string == None ):
      return "No Param"
    str = string.split(' ')   
    Tmp = self.List[int(str[0])]
    #print Tmp
    
    #print unpack(len(str[1])*'b', str[1]) 
    if Tmp[0] == 'vcd':
      self.StoreVCD(base64.b64decode(str[1]), Tmp[1], Tmp[2] )
    
    return "\n"       

  def close( self, string=None ):
    if( string == None ):
      return "No Parameter.\n"
    str = string.split(' ')   
    if self.List.has_key(int(str[0])):
      del self.List[int(str[0])]
      return "closed"
    return "Failure, wrong Parameter?"
   
  def __introspection__(self):
    return ["create","store","retrieve"]


  def CreateVCD(self, ParamList):
    #print ParamList
    # Retrieve Path
    Path = ParamList[0]
    # Create new storage
    Store = VCDStorage(Path,ParamList[1],"4us")
    id = self.id
    self.id = self.id+1
    # Get Valuelist
    ValList = []

    for type in ParamList[2:]:
      tmp = type.split(':')
      if( tmp[1] == 'bit' ):
	Store.AddReg( tmp[0], 1 ) 
	ValList.append((tmp[0],1)) 
      if( tmp[1][3:] == 'int'):
	Store.AddReg( tmp[0], int(tmp[1][-3:]))
	ValList.append((tmp[0],int(tmp[1][-3:]))) 
      #print type
    self.List[id]=['vcd',Store,ValList]
    Store.CreateHeader()
    Values = {}
    for val in ValList:
      Values[val[0]]=0
    Store.SetStartValue( Values, 0 )
    return str(id)+'\n' 

  def StoreVCD(self, ParamList, Storage, ValList):
    Parameters= unpack(len(ParamList)*'b', ParamList) 
    Data = []
    Dict = {}   

    #TODO: Rewrite that more than binary is possible 
    for val in Parameters:
      bin = reversed(ToBin(val,8))
      j=0
      for bit in bin:
        Dict[ValList[j][0]]=bit 
	j+=1
	#print Dict
      Data.append(Dict)
      Dict={}
    Storage.AddValues( Data )
    return

def __introspection__():
  return ["module"] 

if __name__ == "__main__":
  print __introspection__()

