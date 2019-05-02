import serial
import serial.tools.list_ports

import time
import winsound
serial_port = serial.Serial()
serial_port.timeout = 1
localtime = time.localtime(time.time())
print ("Local current time :", localtime)
print(type(localtime.tm_hour))
def time_stamp():

    localtime = time.localtime(time.time())
    return str(localtime.tm_hour)+':'+str(localtime.tm_min) + ':' + str(localtime.tm_sec)+ ' '
print(time_stamp())
def rx_thread_fc():
    while True:
        #print('recieve')
        #time.sleep(1)
        if serial_port.is_open:
            if serial_port.inWaiting() > 0:
                data = serial_port.read(serial_port.inWaiting())
                print(data)
def get_port_list():
    port_list = []
    for i in serial.tools.list_ports.comports(include_links=False):
        port_list.append(i.device)
    return port_list

while True:
    connect_err_flag = False
    print("Select port");
    port_list = get_port_list()
    for port_i in range(len(port_list)):
        print('[',port_i,']',port_list[port_i])
    while True:
        input_err_flag = False
        try:
            port_select = int(input())
        except:
            print("invalid number")
            input_err_flag = True
        if not input_err_flag:
            break
    serial_port.baudrate = 115200
    serial_port.port = port_list[port_select]

    try:
        serial_port.open()
    except:
        print('Serial error')
        connect_err_flag = True
    if not connect_err_flag:
        break
print("Serial connection establish")

#rx_thread = threading.Thread(target=rx_thread_fc)
#rx_thread.start()
while True:
    line = serial_port.readline()
    print(time_stamp() + 'RX:',line)
    if line == b'RST\r\n':
        print('Start testing')
        break
winsound.PlaySound("SystemExit", winsound.SND_ALIAS)
pos = 0
iterations = 0
arduino_rst_flag = False
while not arduino_rst_flag:
    iterations += 1

    print('==============='+' iteration = ',iterations,'===============')
    if pos == 0:
        pos = 5
    else:
        pos = 0
    serial_port.write(('G1X'+str(pos)+'Y'+str(pos)+'W'+str(pos)+'F20\r\n').encode())
    line = ''
    start_time = time.time()
    while not arduino_rst_flag:

        if serial_port.inWaiting()>0:
            line = serial_port.readline()
            print(time_stamp() +'RX:',line)
            if line == b'DONE\r\n':
                print('responce time',(time.time() - start_time),'sec')
                break
            elif line == b'RST\r\n':
                print('Arduino reset after ', iterations)
                arduino_rst_flag = True
        if time.time() - start_time>5:
            print('Arduino no responding')
            winsound.PlaySound("SystemExit", winsound.SND_ALIAS)
            data_to_send = input()
            data_to_send += '\r\n'
            serial_port.write(data_to_send.encode())
            start_time = time.time()




print('Program terminated')
winsound.PlaySound("SystemExit", winsound.SND_ALIAS)
