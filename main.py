import sys
#from PySide2.QtWebEngineWidgets import QWebEngineView
from PySide2.QtCore import QUrl
from PySide2.QtUiTools import QUiLoader
from PySide2.QtWidgets import QApplication, QPushButton, QLineEdit
from PySide2.QtCore import QFile, QObject
from PySide2.QtWebEngineWidgets import QWebEngineView
 
class Form(QObject):
 
    def __init__(self, ui_file, parent=None):
        super(Form, self).__init__(parent)
        ui_file = QFile(ui_file)
        ui_file.open(QFile.ReadOnly)
 	
        loader = QUiLoader()
        self.window = loader.load(ui_file)
        ui_file.close()
 	
        self.host = self.window.findChild(QLineEdit, 'userText')
	self.pw = self.window.findChild(QLineEdit, 'passwordText')
	
        btn = self.window.findChild(QPushButton, 'submitButton')
        btn.clicked.connect(self.button_handler)
        self.window.show()

    def loadMap(self):
	print("load map here!")
	
	self.browser = QWebEngineView()
	self.browser.load(QUrl('https://www.google.com/maps'))
	self.browser.show()	
 
    def button_handler(self):
        if self.host.text() == "host" and self.pw.text() == "pass":
		print('Success!')
		self.loadMap()
	else:
		print('Invalid login!')
 
if __name__ == '__main__':
    app = QApplication(sys.argv)
    form = Form('login.ui')
    sys.exit(app.exec_())
