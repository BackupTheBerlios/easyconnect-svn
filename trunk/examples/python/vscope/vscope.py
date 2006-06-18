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
	self.patterngroup.grid(row=0,column=0 ,padx=10, pady=10)

	label = Label( self.patterngroup, text="Channel")
	label.grid(row=0, column=0)	

	self.ValueList = []
	for i in range(8):
	  
	  self.ValueList.append( StringVar() )
	  self.ValueList[i].set(" ")	
	  Drop = DropDown(self.patterngroup, self.ValueList[i], [" ","H","L"]) 
	  Drop.grid(row=0,column=i+1)
	  label = Label(self.patterngroup, text=str(i))
	  label.grid(row=1,column=i+1)

	self.patternstartbutton = Button(self.patterngroup, text="Start Condition", command=self.ValueAsStart)
	self.patternstartbutton.grid(row=2, column =0, columnspan=4, sticky=E)

	self.patternstartbutton = Button(self.patterngroup, text="Stop Condition", command=self.ValueAsStop, state=DISABLED )
	self.patternstartbutton.grid(row=2, column =4, columnspan=5, sticky=W)


	#######################
	# Edge pattern section

	self.edgegroup = LabelFrame(self.triggergroup, width=30, height=30, text="Edge Pattern", padx=5, pady=5 )
	self.edgegroup.grid(row=1,column=0 ,padx=10, pady=10)

	label = Label( self.edgegroup, text="Channel")
	label.grid(row=0, column=0)	

	self.EdgeList = []
	for i in range(8):
	  
	  self.EdgeList.append( StringVar() )
	  self.EdgeList[i].set(" ")	
	  Drop = DropDown(self.edgegroup, self.EdgeList[i], [" ","R","F"]) 
	  Drop.grid(row=0,column=i+1)
	  label = Label(self.edgegroup, text=str(i))
	  label.grid(row=1,column=i+1)


	self.edgestartbutton = Button(self.edgegroup, text="Start Condition", command=self.EdgeAsStart)
	self.edgestartbutton.grid(row=2, column =0, columnspan=4)

	self.edgestartbutton = Button(self.edgegroup, text="Stop Condition", command=self.EdgeAsStop, state=DISABLED )
	self.edgestartbutton.grid(row=2, column =4, columnspan=5)


	###########################
	# Snapshot section

	self.conditiongroup = LabelFrame(self.triggergroup, text="Condition",  padx=5, pady=5 )
	self.conditiongroup.grid(row=2,column=0 ,padx=10, pady=10)
	
	label = Label( self.conditiongroup, text="Start Condition:")	
	label.grid( row=0,column=0)
	
	self.startcond = Label( self.conditiongroup, text="None")	
	self.startcond.grid( row=1,column=0)

	separator = Frame(self.conditiongroup, width=2, height=20, bd=1, relief=SUNKEN)
	separator.grid(row=0, column=1, padx=5, pady=5)

	label = Label( self.conditiongroup, text="Stop Condition:")	
	label.grid( row=0,column=2)

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
	vallist.append( i.get())
      return vallist
     
    def PatternButton(self):
      pass
 
    def ValueAsStart(self):
      print "Value as Start "+str(self.GetPattern(self.ValueList))

    def ValueAsStop(self):
      print "Value as Stop "+str(self.GetPattern(self.ValueList))

    def EdgeAsStart(self):
      print "Edge as Start "+str(self.GetPattern(self.EdgeList))

    def EdgeAsStop(self):
      print "Edge as Stop "+str(self.GetPattern(self.EdgeList))


if __name__ == '__main__':

  root = Tk()

  app = App(root)

  root.mainloop()
