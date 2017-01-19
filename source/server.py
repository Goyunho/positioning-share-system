# server side source

# socket 과 select 모듈 임포트
from basicNet import BasicNet


# Server class
class Server(BasicNet):
    def run(self):
        self.socket.bind(self.addr)
        self.socket.listen(1)
        conn, addr = self.socket.accept()
        print('Connected by', addr)
        try:
            while True:
                data=conn.recv(1024)
                if not data: break
                print("Received data from", addr, repr(data))
                conn.send(b"ok") #받은 데이터를 그대로 클라이언트에 전송
        except KeyboardInterrupt :
            return


if __name__ == '__main__':
    ser = Server('', 9000, 255*32)
    print("socket is ready.")
    print("run...")
    ser.run()
    ser.close()
    print("closed.")
