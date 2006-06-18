#! /usr/bin/python

from Tkinter import *
import sys
sys.path.append( "../../../easyconnectlibs/python/")
from easyconnectlib import *
from dropdown import *

class scope:
  def __init__( self, FilePath=None, Name="Test" ):
    if( FilePath == None ):
      return
    self.Connection = easyConnect()

  def Store( self):
    return

  def Close( self ):
    return self.Connection.Call("storage.close "+str(self.id))


class App:

    def __init__(self, master):


	self.triggergroup = LabelFrame(master, text="Trigger", padx=5, pady=5)
	self.triggergroup.grid(row=0,column=0 ,padx=10, pady=10, sticky=W )

	#######################
	# Value pattern section

	self.patterngroup = LabelFrame(self.triggergroup, text="Value Pattern", padx=5, pady=5 )
	self.patterngroup.grid(row=0,column=0 ,columnspan=2, padx=10, pady=10)

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
	self.edgegroup.grid(row=1,column=0 ,columnspan=2, padx=10, pady=10)

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
	self.conditiongroup.grid(row=2,column=0, columnspan=2, padx=10, pady=10)
	
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

	self.numberofvalues= StringVar()
	self.numberofvalues.set('1000')
	entry = Entry( self.conditiongroup, textvariable=self.numberofvalues, bg='white', width=10)
	entry.grid( row=2, column=2)

	label = Label( self.triggergroup, text="Intervall:")
	label.grid(row=3,column=0, sticky=E)
	self.intervall = StringVar()
	self.intervall.set("100ms")	
	Drop = DropDown(self.triggergroup, self.intervall, ["100ms","10ms","1ms","100us","50us","10us","5us"]) 
	Drop.grid( row=3,column=1, sticky=W)

	Button(self.triggergroup, width=20, text="Start!").grid(row=4,column=0, columnspan=2, pady=10)
	

	###########################
	# Snapshot section

	self.snapgroup = LabelFrame(master, text="Snapshot",  padx=5, pady=5 )
	self.snapgroup.grid(row=0,column=1 ,padx=10, pady=10)
	
	self.snapnow = Button(self.snapgroup, text="Take Snapshot")
	self.snapnow.grid(row=0,column=0)


	#############################
	# Target section

	self.targetgroup = LabelFrame(master, text="File", padx=5, pady=5)
	self.targetgroup.grid(row=1,column=0,columnspan=2 ,padx=10, pady=10)
        self.target = Entry(self.targetgroup,width=60, bg='white')
        self.target.grid(row=0,column=0)
	self.targetbutton = Button(self.targetgroup, text='Select Directory')
	self.targetbutton.grid(row=0,column=1)

    def hi(self):
      pass


    def GetPattern(self,CBList):
      vallist =[]
      for i in CBList:
	j = i.get()
	if j == ' ':
	  j = '.'
	vallist.append(j)
      return vallist
     
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
