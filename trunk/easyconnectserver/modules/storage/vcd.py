class Storage:
  def __init__(self, Name ):
    self.Name = Name
  
  def Save( self, string ):
    File = open( self.Name, "a" )
    File.write( string )
    File.write( "\n" )
    File.close()

if __name__ == "__main__":
  st = Storage( "Save.txt" )
  st.Save("WTF")

