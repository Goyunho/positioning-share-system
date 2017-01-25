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
        while True:
            try:
                data=conn.recv(1024)
                if not data: break
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
    info = slice_list(repr(data), [1,4,256])
    sign = info[0]
    ID = info[1]
    filename = info[2]
    if sign == 0x00: # save file
        saveFileProcess(conn, ID, filename)
    elif sign == 0x01: # send file
        sendFileProcess(conn, ID, filename)
    else:
        conn.send(0) # 0: datastruct error!


def saveFileProcess(conn, ID, filename):
    conn.send(b'ok')
    data = conn.recv(BUFFERSIZE)
    if not data :
        conn.send(b'no data')
        return
    try:
        with open("".join("./files/", ID, "_", filename), "wb") as f:
            while data:
                f.write(data)
                conn.send(b'ok')
                data = conn.recv(BUFFERSIZE)
    except:
        conn.send(('error! ' + Exception).encode())
    else:
        conn.send(('ok').encode())

def sendFileProcess(conn, ID, filename):
    with open("".join("./files/", ID, "_", filename), wb) as f:
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
