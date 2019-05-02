from PyQt5.QtWidgets import *
from PyQt5 import QtCore, QtGui
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtCore import QSettings
import sys
import serial
import serial.tools.list_ports
import  time
import pyqtgraph as pg
import pyqtgraph.exporters
import math
import numpy as np
import random
import collections
serial_port =  serial.Serial()

class Serial_RX(QtCore.QThread):
    Serial_signal = pyqtSignal()

    serial_display = ''
    serial_buffer = ''
    timer = time.clock()
    def __init__(self,parent):
        QtCore.QThread.__init__(self)
        self.parent = parent
    def __del__(self):
        self.wait()
    def run(self):
        while True:
            if serial_port.is_open:


                error = False
                data_to_update = False
                try:
                    if serial_port.inWaiting()>0:

                        #print(delta_time)
                        bytesToRead = serial_port.inWaiting()
                        data = serial_port.read(bytesToRead)
                        self.serial_buffer = ''

                        #if self.mode == 'ASCII':

                        self.serial_buffer += data.decode("utf-8")


                        self.serial_display = self.serial_buffer
                        data_to_update = True

                except Exception as e:
                    print('read error',e)
                    error = True
                if not error and data_to_update:
                    self.Serial_signal.emit()


class Serial_TX(QtCore.QThread):
    data_to_send = ''
    def __init__(self,data_to_send):
        QtCore.QThread.__init__(self)
        self.data_to_send = data_to_send
    def __del__(self):
        self.wait()
    def run(self):
        try:
            serial_port.write(self.data_to_send.encode())
        except:
            print('send error')
class main_widget(QWidget):
    x_plt_arr =  np.array([])
    y_plt_arr = np.array([])
    def __init__(self, parent,settings):
        self.settings = settings
        super().__init__(parent)
        self.setupUI()



    def get_port_list(self):
        port_list = []
        for i in serial.tools.list_ports.comports(include_links=False):
            port_list.append(i.device)
        return port_list
    def update_port_list(self):
        print('update_port_list')
        self.Port_select.clear()
        self.Port_select.addItems(self.get_port_list())
        self.Port_select.update()
    def serial_connect(self):

        serial_port.port = str(self.Port_select.currentText())

        serial_port.baudrate = int(self.Buad_select.currentText())
        print(serial_port)
        try:
            serial_port.open()
        except:
            print('Serial error')
            self.serial_error_dialog()
            return
        self.connection_update()
        print('Connected')
        self.serial_log_clear()
        self.settings.setValue('last_connect_port', serial_port.port)
        self.settings.setValue('last_connect_baud', str(serial_port.baudrate))

    def serial_disconnect(self):
        serial_port.close()
        self.connection_update()
        print('Disconnected')

    def serial_log_update(self):
        self.Serial_log.insertPlainText(self.Serial_RX_Thread.serial_display)
        self.Serial_log.verticalScrollBar().setValue(self.Serial_log.verticalScrollBar().maximum())

    def serial_log_clear(self):
        self.Serial_log.setPlainText('')
        self.x_plt_arr = np.array([])
        self.y_plt_arr = np.array([])
    def connection_update(self):
        Serial_Open = serial_port.is_open
        print("Serial Open = ",Serial_Open)


        self.disconnect_button.setEnabled(Serial_Open)
        self.connect_button.setEnabled(not Serial_Open)
        self.Port_select.setEnabled(not Serial_Open)
        self.port_refresh_button.setEnabled(not Serial_Open)
        self.Buad_select.setEnabled(not Serial_Open)

    def serial_send(self):
        if serial_port.is_open:
            data_to_send = self.text_for_send.text()
            if self.CR.isChecked():
                data_to_send += '\r'
            if self.NL.isChecked():
                data_to_send += '\n'
            self.Serial_TX_Thread = Serial_TX(data_to_send)
            self.Serial_TX_Thread.start()
            print('send : ' + data_to_send)

            self.text_for_send.setText('')
    def serial_error_dialog(self):
        serial_error_msg = QMessageBox()
        serial_error_msg.setIcon(QMessageBox.Warning)
        serial_error_msg.setText("Error openning serial port "+serial_port.port)
        #serial_error_msg.setInformativeText("This is additional information")
        serial_error_msg.setWindowTitle("Warning message")

        serial_error_msg.setStandardButtons(QMessageBox.Ok )
        serial_error_msg.exec_()
    def serial_setting_groupBox(self):
        serial_setting = QGroupBox('Serial port setting')
        Hlayout = QHBoxLayout(self)
        l1 = QLabel()
        l1.setText('Port')
        l2 = QLabel()
        l2.setText('Baud rate')
        self.port_refresh_button = QPushButton('Refresh', self)
        self.port_refresh_button.clicked.connect(self.update_port_list)

        self.connect_button = QPushButton('Connect', self)
        self.connect_button.clicked.connect(self.serial_connect)
        self.disconnect_button = QPushButton('Disconnect', self)
        # self.disconnect_button.setEnabled(False)
        self.disconnect_button.clicked.connect(self.serial_disconnect)

        self.Port_select = QComboBox()
        port_list = self.get_port_list()
        self.Port_select.addItems(port_list)

        last_connect_port = self.settings.value('last_connect_port', type=str)
        if last_connect_port in port_list:
            self.Port_select.setCurrentIndex(port_list.index(last_connect_port))

        self.Buad_select = QComboBox()
        baud_list = ['300', '600', '1200', '2400', '4800', '9600', '14400', '19200', '28800', '38400', '57600', '115200']
        self.Buad_select.addItems(baud_list)
        last_connect_baud = self.settings.value('last_connect_baud', type=str)
        if last_connect_baud in baud_list:
            self.Buad_select.setCurrentIndex(baud_list.index(last_connect_baud))


        H_Spacer1 = QSpacerItem(150, 10, QSizePolicy.Expanding)
        Hlayout.addWidget(l1)
        Hlayout.addWidget(self.Port_select)
        Hlayout.addWidget(self.port_refresh_button)
        Hlayout.addWidget(l2)
        Hlayout.addWidget(self.Buad_select)
        Hlayout.addWidget(self.connect_button)
        Hlayout.addWidget(self.disconnect_button)
        Hlayout.addItem(H_Spacer1)
        # Vlayout.addLayout(Hlayout_0)
        serial_setting.setLayout(Hlayout)
        return serial_setting



    def Sending_console(self):
        Vlayout = QVBoxLayout(self)
        Hlayout_1 = QHBoxLayout(self)
        self.text_for_send = QLineEdit()
        self.text_for_send_change_signal = pyqtSignal()
        self.text_for_send.returnPressed.connect(self.serial_send)

        #self.send_button = QPushButton('Send', self)
        #self.send_button.clicked.connect(self.serial_send)
        Hlayout_1.addWidget(self.text_for_send)

        Vlayout.addLayout(Hlayout_1)

        ################################
        Hlayout_2 = QHBoxLayout(self)
        l3 = QLabel()
        l3.setText('Line ending')
        self.CR = QCheckBox("<CR>")
        self.NL = QCheckBox("<NL>")
        H_Spacer2 = QSpacerItem(150, 10, QSizePolicy.Expanding)

        Hlayout_2.addWidget(l3)
        Hlayout_2.addWidget(self.CR)
        Hlayout_2.addWidget(self.NL)
        Hlayout_2.addItem(H_Spacer2)
        Vlayout.addLayout(Hlayout_2)
        Sending_console_widget = QWidget()
        Sending_console_widget.setLayout(Vlayout)
        return Sending_console_widget
    def setupUI(self):

        main_Vlayout = QVBoxLayout(self)
        ################################

        main_Vlayout.addWidget(self.serial_setting_groupBox())

        ################################


        self.Serial_log = QPlainTextEdit()

        self.Serial_log.setReadOnly(True)

        main_Vlayout.addWidget(self.Serial_log)

        ################################

        main_Vlayout.addWidget(self.Sending_console())

        ################################
        self.connection_update()
        self.setLayout(main_Vlayout)


        self.Serial_RX_Thread = Serial_RX(self)
        self.Serial_RX_Thread.start()
        self.Serial_RX_Thread.Serial_signal.connect(self.serial_log_update)


class main_window(QMainWindow):
    def __init__(self,settings ):
        self.settings = settings
        super(QMainWindow, self).__init__()
        #QMainWindow.__init__(self)
        self.initUI()
    def initUI(self):
        self.setGeometry(300, 300, 500, 600)
        self.setWindowTitle("Serial Monitor")
        self.setWindowIcon(QtGui.QIcon('py_logo.png'))

        self.main_widget = main_widget(self,self.settings)
        self.setCentralWidget( self.main_widget)




    def closeEvent(self, event):

        if serial_port.is_open:
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Information)

            msg.setText("Are you sure you want to quit?")
            msg.setInformativeText("Serial port still connected.")
            msg.setWindowTitle("MessageBox demo")
            msg.setWindowTitle("Warning message")

            msg.setStandardButtons(QMessageBox.Ok | QMessageBox.Cancel)


            retval = msg.exec_()
            if retval == QMessageBox.Ok:
                print('Exit conform')
                app = QtGui.QApplication.instance()
                app.closeAllWindows()
            else:
                event.ignore()

        else:
            app = QtGui.QApplication.instance()
            app.closeAllWindows()

def main():
    app = QApplication(sys.argv)
    settings = QSettings('Meng\'s Lab', 'Serial Monitor')
    w = main_window(settings)
    w.show()
    sys.exit(app.exec_())



if __name__ == '__main__':
    main()