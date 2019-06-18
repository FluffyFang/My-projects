#Author: Alan Althoff
#Assignment #3
#
#Features:
#Uses a database to store phone numbers
#Search function is case insensitive and utilizes partial matching
#Search can also be called by pressing enter in search box
#Edit, delete, or add addresses
#Simplistic interface

from tkinter import *
import sqlite3

conn = sqlite3.connect('phoneBook.db')
c = conn.cursor()
c.execute('CREATE TABLE IF NOT EXISTS phoneBook (first text, last text, number text, autokey INTEGER PRIMARY KEY AUTOINCREMENT)')

def searchDBEnter(event):
	searchDB()

#Looks for value in main window search box and displays it in the results listbox
def searchDB():
	results.delete(0, END)
	resultsKey.delete(0, END)
	input = (searchBox.get().strip() + '%',)
	notFound = True
	
	c.execute('SELECT * FROM phoneBook WHERE first LIKE ?', input)
	result = c.fetchall()
	for i in result:
		results.insert (END, i[0] + " " + i[1] + ": " + i[2])
		resultsKey.insert (END, i[3])
		notFound = False
		
	c.execute('SELECT * FROM phoneBook WHERE last LIKE ?', input)
	result = c.fetchall()
	alreadyEntered = list(resultsKey.get(0, END))
	for i in result:
		if i[3] not in alreadyEntered:
			results.insert (END, i[0] + " " + i[1] + ": " + i[2])
			resultsKey.insert (END, i[3])
			notFound = False
		
	c.execute('SELECT * FROM phoneBook WHERE number LIKE ?', input)
	alreadyEntered = list(resultsKey.get(0, END))
	for i in result:
		if i[3] not in alreadyEntered:
			results.insert (END, i[0] + " " + i[1] + ": " + i[2])
			resultsKey.insert (END, i[3])
			notFound = False
		
	if notFound:
		results.insert (END, "Name not found")
	
def exitProgram():
	window.destroy()
	
#Populate the listbox in the main window with all addresses in database
def displayAll():
	results.delete(0, END)
	resultsKey.delete(0, END)
	c.execute('SELECT * FROM phoneBook')
	theList = c.fetchall()
	for i in theList:
		results.insert (END, i[0] + " " + i[1] + ": " + i[2])
		resultsKey.insert (END, i[3])
	
#creates the window for editing an address; handles all scripts for that window
def editWindow():
	def backOneWindow():
		subWindow.destroy()
		
	#Deletes the address selected on main window from database and returns to main window
	def deleteDB():
		c.execute('DELETE FROM phoneBook WHERE autokey=?', editKey)
		conn.commit()
		displayAll()
		backOneWindow()
	
	#Same as deleteDB, but adds the edited information into the database
	def editEntry():
		editInput = (firstText.get(), lastText.get(), numberText.get(), None,)
		c.execute('DELETE FROM phoneBook WHERE autokey=?', editKey)
		c.execute('INSERT INTO phoneBook VALUES (?,?,?,?)', editInput)
		conn.commit()
		displayAll()
		backOneWindow()
		
	editKey = (resultsKey.get(results.curselection()),)
	c.execute('SELECT * FROM phoneBook WHERE autokey=?', editKey)
	entryToEdit = c.fetchone()
	
	#The "window"
	subWindow = Label()
	subWindow.place(relheight = 1, relwidth = 1)
	
	#Buttons
	add = Button(subWindow, text ="Delete Entry", command = deleteDB)
	add.place(height = 140, width = 170, relx = 1, x = -180, y = 280)
	saveChange = Button(subWindow, text ="Save changes", command = editEntry)
	saveChange.place(height = 50, width = 235, x = 10, rely = 1, y = -60)
	goBack = Button(subWindow, text ="Back to search", command = backOneWindow)
	goBack.place(height = 50, width = 235, x = -245, relx = 1, rely = 1, y = -60)
	
	#Entry boxes
	firstText = Entry(subWindow)
	firstText.insert(END, entryToEdit[0])
	firstText.place(height = 20, width = -200, x = 10, relwidth = 1, y = 280)
	lastText = Entry(subWindow)
	lastText.insert(END, entryToEdit[1])
	lastText.place(height = 20, width = -200, x = 10, relwidth = 1, y = 340)
	numberText = Entry(subWindow)
	numberText.insert(END, entryToEdit[2])
	numberText.place(height = 20, width = -200, x = 10, relwidth = 1, y = 400)
	
	#Labels
	firstLabel = Label(subWindow, text = "First Name")
	firstLabel.place(x = 10, y = 250)
	lastLabel = Label(subWindow, text = "Last Name")
	lastLabel.place(x = 10, y = 310)
	numberLabel = Label(subWindow, text = "Phone number")
	numberLabel.place(x = 10, y = 370)
	
#creates the window for adding an address; handles all scripts for that window
def addWindow():
	def backOneWindow():
		displayAll()
		subWindow.destroy()
		
	#Adds the entry to the database
	def addToDB():
		addInput = (firstText.get(), lastText.get(), numberText.get(), None,)
		c.execute('INSERT INTO phoneBook VALUES (?,?,?,?)', addInput)
		addGhost.insert(END, "Added " + addInput[0] + " " + addInput[1] + ": " + addInput[2])
		conn.commit()
		
	#The "window"
	subWindow = Label()
	subWindow.place(relheight = 1, relwidth = 1)
	
	#Buttons
	add = Button(subWindow, text ="Add to address book", command = addToDB)
	add.place(height = 140, width = 170, relx = 1, x = -180, y = 40)
	goBack = Button(subWindow, text ="Back to search", command = backOneWindow)
	goBack.place(height = 50, width = 235, x = 10, rely = 1, y = -60)
	quit2 = Button(subWindow, text ="Quit program", command = exitProgram)
	quit2.place(height = 50, width = 235, x = -245, relx = 1, rely = 1, y = -60)
	
	#Entry boxes
	firstText = Entry(subWindow)
	firstText.place(height = 20, width = -200, x = 10, relwidth = 1, y = 40)
	lastText = Entry(subWindow)
	lastText.place(height = 20, width = -200, x = 10, relwidth = 1, y = 100)
	numberText = Entry(subWindow)
	numberText.place(height = 20, width = -200, x = 10, relwidth = 1, y = 160)
	
	#Labels
	firstLabel = Label(subWindow, text = "First Name")
	firstLabel.place(x = 10, y = 10)
	lastLabel = Label(subWindow, text = "Last Name")
	lastLabel.place(x = 10, y = 70)
	numberLabel = Label(subWindow, text = "Phone number")
	numberLabel.place(x = 10, y = 130)
	
	#Listbox. Mostly here to let users know the address was accepted without a confirm button
	addGhost = Listbox(subWindow)
	addGhost.place(height = -260, relheight = 1, width = -20, relwidth = 1, x = 10, y = 190)
	
   
#The main window!
window = Tk()
window.minsize(width = 500, height = 500)

#Buttons
search = Button(window, text ="Search", command = searchDB)
search.place(height = 20, width = 100, x = 390, y = 30)
gotoAddMenu = Button(window, text ="Add address", command = addWindow)
gotoAddMenu.place(height = 50, width = 235, x = 255, y = 400)
gotoEditMenu = Button(window, text ="Edit selected address", command = editWindow)
gotoEditMenu.place(height = 50, width = 235, x = 10, y = 400)
quit = Button(window, text ="Exit", command = exitProgram)
quit.place(height = 20, width = 100, x = 200, y = 470)

#Entry box
searchBox = Entry(window)
searchBox.bind('<Return>', searchDBEnter)
searchBox.place(height = 20, width = 370, x = 10, y = 30)

#Label
searchLabel = Label(window, text = "Name or number to look for:")
searchLabel.place(x = 10)

#Listbox
scrollbar = Scrollbar(window)
results = Listbox(window, yscrollcommand = scrollbar.set)
results.place(height = 300, width = 460, x = 10, y = 70)
scrollbar.place(height = 300, x = 470, y = 70)
scrollbar.config(command = results.yview)
resultsKey = Listbox(window)

#start with all addresses displayed
displayAll()

#create the window
window.mainloop()