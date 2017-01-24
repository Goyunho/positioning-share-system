# server�� client �������� ����Ǵ� �κ�
# socket �� select ��� ����Ʈ
from socket import *
#from select import *
import sys
#from time import ctime

# Server class
class BasicNet():
    host = None
    port = None
    buffersize = 256*32
    addr = (host, port)
    def __init__(self, host, port, buffersize):
        self.host = host
        self.port = port
        self.buffersize = buffersize
        try:
            self.socket = socket(AF_INET, SOCK_STREAM)
        except Exception as e:
            print(e)
            self.close()
        else:
            self.addr = (host, port)


    def run(self):
        try:
            pass
        except KeyboardInterrupt :
            return

    def close(self):
        del self

    def __del__(self):
        self.socket.close()
