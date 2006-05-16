
from wxPyPlot import *
from wx import Timer
import sys
sys.path.append( "../../../easyconnectlibs/python/")
from easyconnectlib import *

class AppFrame( wx.wxFrame ):
  def __init__( self, parent, id, title ):
    wx.wxFrame.__init__( self, parent, id, title, 
			 wx.wxPyDefaultPosition, wx.wxSize( 600, 400 ))

    self.mainmenu = wx.wxMenuBar()

    menu = wx.wxMenu()
    menu.Append( 200, 'Start', 'Start VScope Demo' )
    wx.EVT_MENU( self, 200, self.OnDemoStart )
  
    self.mainmenu.Append( menu, 'Demo' )
   
    self.SetMenuBar( self.mainmenu )
 
    self.client = PlotCanvas( self )
    
    TIMER_ID = 100
    self.timer = Timer( self, TIMER_ID )
    wx.EVT_TIMER( self, TIMER_ID, self.OnTimer )
    self.curve = []
    self.elements = 0
    self.line = PolyLine( self.curve, legend='Demo', colour='black' )
    self.client.Draw(PlotGraphics([self.line]), (0, 600), (0, 100))

    self.width = 600
    self.current = 0

  def OnDemoStart( self, event ):
    #INIT THE easyConnect OBJECT
    self.Connection = easyConnect( ip = "80.190.146.252")

    self.timer.Start( 40 )
  
  def OnTimer( self, event ):
    #GETTING THE DATA FROM THE SERVER
    val =self.Connection.Call("CpuWatcher.GetCurrentLoad").replace('.','').replace("\n",'')

    value = int(val)


    if( self.elements <= self.width ):
      self.elements += 1
      self.curve.append([self.current, value])
    else:
      #print self.current
      self.curve[self.current] = [self.current, value]

    self.line = PolyLine( self.curve, legend='Demo', colour='black' )
    self.client.Draw(PlotGraphics([self.line]),(0, 600), (0, 100))
      
    self.current += 1
    if( self.current > self.width ):
      self.current = 0


class VScope( wx.wxApp ):
  def OnInit(self):
    frame = AppFrame( wx.NULL, -1, "pyCpuMon :: A easyConnect Demo" )
    frame.Show( True )
    self.SetTopWindow( frame )
    return True 


if __name__ == '__main__':
  App = VScope(0)
  App.MainLoop() 
  

