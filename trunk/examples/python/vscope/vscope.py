#! /usr/bin/python

from Tkinter import *
import sys
sys.path.append( "../../../easyconnectlibs/python/")
from easyconnectlib import *
from dropdown import *
import vcd

class scope:
  def __init__( self, Ip, Port ):
    if( FilePath == None ):
      return
    self.Connection = easyConnect(ip=Ip, port=Port)

  def SetTrigger(self, TriggerType, Pattern ):
    return 
  
  def StartRecord( self, numval, interval, online ):
    return

  def GetOneValue( self ):
    return

class App:

    def __init__(self, master):


	self.triggergroup = LabelFrame(master, text="Trigger", padx=5, pady=5)
	self.triggergroup.grid(row=0,column=0 ,padx=10, pady=10, sticky=W )

	#######################
	# Value pattern section

	self.patterngroup = LabelFrame(self.triggergroup, text="Value Pattern", padx=5, pady=5 )
	self.patterngroup.grid(row=0,column=0 ,columnspan=3, padx=10, pady=10)

	label = Label( self.patterngroup, text="Channel")
	label.grid(row=0, column=0)	

	self.ValueList = []
	for i in range(8):
	  
	  self.ValueList.append( StringVar() )
	  self.ValueList[i].set(" ")	
	  Drop = DropDown(self.patterngroup, self.ValueList[i], [" ","H","L"]) 
	  Drop.grid(row=0,column=i+1)
	  label = Label(self.patterngroup, text=str(i+1))
	  label.grid(row=1,column=i+1)

	self.patternstartbutton = Button(self.patterngroup, text="Start Condition", command=self.ValueAsStart)
	self.patternstartbutton.grid(row=2, column =0, columnspan=4, sticky=E)

	self.patternstartbutton = Button(self.patterngroup, text="Stop Condition", command=self.ValueAsStop, state=DISABLED )
	self.patternstartbutton.grid(row=2, column =4, columnspan=5, sticky=W)


	#######################
	# Edge pattern section

	self.edgegroup = LabelFrame(self.triggergroup,  text="Edge Pattern", padx=5, pady=5 )
	self.edgegroup.grid(row=1,column=0 ,columnspan=3, padx=10, pady=10)

	label = Label( self.edgegroup, text="Channel")
	label.grid(row=0, column=0)	

	self.EdgeList = []
	for i in range(8):
	  
	  self.EdgeList.append( StringVar() )
	  self.EdgeList[i].set(" ")	
	  Drop = DropDown(self.edgegroup, self.EdgeList[i], [" ","R","F"]) 
	  Drop.grid(row=0,column=i+1)
	  label = Label(self.edgegroup, text=str(i+1))
	  label.grid(row=1,column=i+1)


	self.edgestartbutton = Button(self.edgegroup, text="Start Condition", command=self.EdgeAsStart)
	self.edgestartbutton.grid(row=2, column =0, columnspan=4)

	self.edgestartbutton = Button(self.edgegroup, text="Stop Condition", command=self.EdgeAsStop, state=DISABLED )
	self.edgestartbutton.grid(row=2, column =4, columnspan=5)


	###########################
	# condition section

	self.conditiongroup = LabelFrame(self.triggergroup, text="Condition",  padx=5, pady=5 )
	self.conditiongroup.grid(row=2,column=0, columnspan=3, padx=10, pady=10)
	
	label = Label( self.conditiongroup, text="Start Condition:")	
	label.grid( row=0,column=0)
	
	self.startcondtype = StringVar()
	self.startcondtype.set("None")
	label = Label( self.conditiongroup, textvariable=self.startcondtype)	
	label.grid( row=1,column=0)
	
	self.startcond = StringVar()
	self.startcond.set("None")
	label = Label( self.conditiongroup, textvariable=self.startcond)	
	label.grid( row=2,column=0)


	separator = Frame(self.conditiongroup, width=2, height=60, bd=1, relief=SUNKEN)
	separator.grid(row=0, column=1, rowspan=3, padx=5, pady=5)

	label = Label( self.conditiongroup, text="Stop Condition:")	
	label.grid( row=0,column=2)

	label = Label( self.conditiongroup, text="Number of Values:")	
	label.grid( row=1,column=2)

	self.numval= StringVar()
	self.numval.set('1000')
	self.numvalentry = Entry( self.conditiongroup, textvariable=self.numval, bg='white', width=10)
	self.numvalentry.grid( row=2, column=2)

	label = Label( self.triggergroup, text="Interval:")
	label.grid(row=3,column=0, sticky=E)
	self.intervall = StringVar()
	self.intervall.set("100ms")	
	Drop = DropDown(self.triggergroup, self.intervall, ["100ms","10ms","1ms","100us","50us","10us","5us"]) 
	Drop.grid( row=3,column=1, sticky=W)

	self.memory = IntVar()
	self.memory.set(1)
	Box=Checkbutton(self.triggergroup, variable=self.memory, text="Online", command=self.OnOffline).grid(row=3,column=2)


	Button(self.triggergroup, width=20, text="Start!").grid(row=4,column=0, columnspan=3, pady=10)
	

	###########################
	# Snapshot section

	self.snapgroup = LabelFrame(master, text="Snapshot",  padx=5, pady=5 )
	self.snapgroup.grid(row=0,column=1 ,padx=10, pady=10)
	
	snapnow = Button(self.snapgroup, text="Take Snapshot", width=20).grid(row=0,column=0, columnspan=2, pady=5)
	Listbox(self.snapgroup, bg='White', width=35, height=21).grid(row=1,column=0, columnspan=2)
	save = Button(self.snapgroup, text="Save").grid(row=2,column=0, pady=10)
	delsnap = Button(self.snapgroup, text="Delete").grid(row=2,column=1)


	#############################
	# Target section
	
	
	self.targetgroup = LabelFrame(master, text="File", padx=5, pady=5)
	self.targetgroup.grid(row=1,column=0,columnspan=2 )
        self.target = StringVar()
	self.target.set('./new.vcd')
	Entry(self.targetgroup,width=60, bg='white', textvariable=self.target).grid(row=0,column=0)
	self.targetbutton = Button(self.targetgroup, state=DISABLED, text='Select Directory')
	self.targetbutton.grid(row=0,column=1)

	self.servergroup = LabelFrame(master, text="Server", padx=5, pady=5)
	self.servergroup.grid(row=2,column=0,columnspan=2,padx=5, pady=10)
	Label(self.servergroup, text='IP:' ).grid(row=0,column=0)
        self.ip = StringVar()
	self.ip.set('127.0.0.1')
	Entry(self.servergroup,width=20, textvariable=self.ip, bg='white').grid(row=0,column=1)
	self.port = StringVar()
	self.port.set('9090')
	
	Label(self.servergroup, text='Port:' ).grid(row=0,column=2)
	Entry(self.servergroup,width=10, textvariable=self.port, bg='white').grid(row=0,column=3)
	#self.serverbutton = Button(self.servergroup, text='Select Directory')
	#self.serverbutton.grid(row=0,column=1)



    def GetPattern(self,CBList):
      vallist =[]
      for i in CBList:
	j = i.get()
	if j == ' ':
	  j = '.'
	vallist.append(j)
      return vallist
    
    def OnOffline(self):
      if self.memory.get() == 0:
	self.numval.set('1000')
	self.numvalentry.config(state=DISABLED)      
      if self.memory.get() == 1:
	self.numvalentry.config(state=NORMAL)      

    def PatternButton(self):
      pass
 
    def ValueAsStart(self):
      self.startcondtype.set("Value Pattern")     
      list = self.GetPattern(self.ValueList)
      self.startcond.set( ''.join(list))
	
 
    def ValueAsStop(self):
      print "Value as Stop "+str(self.GetPattern(self.ValueList))

    def EdgeAsStart(self):
      self.startcondtype.set("Edge Pattern")     
      list = self.GetPattern(self.EdgeList)
      self.startcond.set( ''.join(list))

    def EdgeAsStop(self):
      print "Edge as Stop "+str(self.GetPattern(self.EdgeList))


if __name__ == '__main__':

  root = Tk()

  app = App(root)

  root.mainloop()
