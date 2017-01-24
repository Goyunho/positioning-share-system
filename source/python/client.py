# client side source

# socket 과 select 모듈 임포트
from BasicNet import BasicNet
import time


# client class
class Client(BasicNet):
    def __init__(self, host, port, buffersize):
        super.__init__(self, host, port, buffersize)
        self.socket.connect(self.addr)

    def run(self):
        while True:
            try:
                self.socket.send(b'Hello, python : ') #문자를 보냄
                data=self.socket.recv(1024) #서버로 부터 정보를 받음
                print('Received',repr(data))
                time.sleep(2)
            except KeyboardInterrupt :
                return


if __name__ == '__main__':
    cli = Client('127.0.0.1', 9001, 255*32)
    print("connecting server...")
    try:
        cli.run()
    except Exception as e:
        print("Error " + e)
    else:
        cli.close()
