# server side source

# socket 과 select 모듈 임포트
from BasicNet import BasicNet
import sqlite3

BUFFERSIZE = 1024

def slice_list(llist, slice_index:list):
    if len(slice_index) == 0:
        return [llist]
    elif slice_index[0] == None:
        return [llist]
    elif slice_index[0] > len(llist):
        return [llist]
    return [llist[:slice_index[0]]] + slice_list(llist[slice_index[0]:], slice_index[1:])

# Server class
class Server(BasicNet):
    def __init__(self, host, port, buffersize):
        super().__init__(host, port, buffersize)
        self.socket.bind(self.addr)

    def run(self):
        self.socket.listen(1)
        conn, addr = self.socket.accept()
        print('Connected by', addr)
        try:
            data=conn.recv(1024)
            if not data: return
            print("Received data from", addr, repr(data))
            #conn.send(data_process(data))
            data_process(conn, data)
        except KeyboardInterrupt :
            return

def data_process(conn, data):
    datastruct = {
        'sign': 1,
        'ID' : 4,
        'filename' : 256
    }
    info = slice_list(data, [1,4,256])
    sign = info[0]
    ID = info[1]
    filename = info[2]
    print('sign : ', repr(sign))
    print('ID : ', repr(ID))
    print('filename : ', repr(filename))
    if sign == b'0': # save file
        saveFileProcess(conn, ID, filename)
    elif sign == b'1': # send file
        sendFileProcess(conn, ID, filename)
    else:
        conn.send(b'1') # 1: datastruct error!


def saveFileProcess(conn, ID, filename):
    conn.send(b'0') # ok sign
    data = conn.recv(BUFFERSIZE)
    if not data :
        conn.send(b'3')
        return
    try:
        with open("./files/"+str(int.from_bytes(ID, 'big'))+"_"+str(filename.split(b'\x00')[0]), "wb") as f:
            while data:
                f.write(data)
                conn.send(b'0')
                data = conn.recv(BUFFERSIZE)
    except Exception as e:
        print(e)
        conn.send(b'2') # save process error!
    else:
        conn.send(b'0')

def sendFileProcess(conn, ID, filename):
    with open("./files/"+str(int.from_bytes(ID, 'big'))+"_"+str(filename.split(b'\x00')[0]), 'wb') as f:
        while True:
            block = f.read(16)
            if not block : break
            conn.send(block)

if __name__ == '__main__':
    ser = Server('', 9000, 1024*4)
    print("socket is ready.")
    print("run...")
    while True:
        try:
            ser.run()
        except KeyboardInterrupt :
            break
    ser.close()
    print("closed.")
