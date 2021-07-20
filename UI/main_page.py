# -*- coding: utf-8 -*-

try:
    from PySide import QtCore
    from PySide import QtWidgets
    from PySide import QtQuick
except:
    from PyQt5.QtCore import pyqtSlot as Slot
    from PyQt5 import QtCore
    from PyQt5 import QtWidgets
    from PyQt5 import QtQuick
    import sys

class main_page(QtWidgets.QMainWindow):
    def __init__(self):
        QtWidgets.QMainWindow.__init__(self)
        super(Ui, self).__init__() # Call the inherited classes __init__ method
        uic.loadUi('basic.ui', self) # Load the .ui file
        self.show()
        pass

