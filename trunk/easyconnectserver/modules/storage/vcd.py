import datetime

def ToBin(x, count=8):
        """
        Integer to binary
        Count is number of bits
        """
        return "b"+"".join(map(lambda y:str((x>>y)&1), range(count-1, -1, -1)))

class VCDStorage:
  def __init__(self, Filepath = None, Name = None, Timescale="1ns" ):
    if( Filepath is None or Name is None ):
      raise "Failure: Storage.__init__\n Name or Filepath was not given."

    self.Filepath = Filepath
    self.File = open( Filepath, "w" )
    self.Name = Name
    self.Timescale = Timescale
    self.Header = 0
    self.Values = []
    self.ValDict = {} 
    self.CurrentSymbol = 33 
    self.Current = 0

  def AddWire( self, Name, Bits ):
    self.AddVar("wire", Name, Bits )

  def AddReal( self, Name, Bits ):
    self.AddVar("real", Name, Bits )

  def AddReg( self, Name, Bits ):
    self.AddVar("reg", Name, Bits )

  def AddVar( self, Type, Name, Bits ):
    Symbol = '%c'%self.CurrentSymbol
    self.CurrentSymbol += 1
    self.ValDict[Name] = Symbol 
    Symbol = ' '+Symbol+' '

    if( Bits > 1 ):
      BitString = " ["+str(Bits-1)+"]" 
      BitString = BitString.replace("]",":0]")
    else:
      BitString = ""

    String = Type+" "+str(Bits)+Symbol+Name+BitString
    self.Values.append( self.CreateField( "var", String ) ) 
  
  def CreateField( self, Fieldname, Value ):
    String = '$' + Fieldname + ' ' + str(Value) + ' $end\n' 
    return String 

  def CreateHeader( self ):
    Header = ''
    Header += self.CreateField( "date", str(datetime.datetime.now()) )
    Header += self.CreateField( "version", "VScope "+self.Name )
    Header += self.CreateField( "timescale", self.Timescale )
    Header += self.CreateField( "scope module", self.Name ) 
    for var in self.Values:
      Header += var    
    Header += self.CreateField( "upscope", "" ) 
    Header += self.CreateField( "enddefinitions", "" ) 
    self.WriteString(Header) 
    self.Header = 1

  def SetStartValue( self, StartValue, StartTime=0):
    String = '#'+str(StartTime)+'\n'
    String += '$dumpvars\n'
    for key in StartValue.keys():
      String += str(StartValue[key]) + " " + self.ValDict[key]+"\n"
    
    String += '$end\n'
    self.WriteString( String )
    self.Current += 1

  def AddValues( self, ValList ):
    String = ""

    for Value in ValList:
      String += '#'+str(self.Current)+'\n' 
      for key in Value.keys():
	String += str(Value[key]) + " " + self.ValDict[key]+"\n"
      self.Current += 1

    self.WriteString( String )
 

  def WriteString( self, String ):
    self.File.write( String )

  def __del__(self):
    self.File.close()

if __name__ == "__main__":
  st = VCDStorage( "Save.txt", "Test"  )
  st.AddReg( "Logik", 8 )
  st.CreateHeader()
  st.SetStartValue( {"Logik": ToBin(0) }, 0 )
  st.AddValues( [{"Logik":ToBin(10)}, {"Logik":ToBin(20)},{"Logik":ToBin(30)},{"Logik":ToBin(40)},{"Logik": ToBin(50) }])

 
