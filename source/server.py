# server side source

# socket �� select ��� ����Ʈ
from basicNet import BasicNet


# Server class
class Server(BasicNet):
    def run(self):
        try:
            self.socket.listen(10)
            conn, addr = self.socket.accept()
            print('Connected by', addr)
            while True:
                data=conn.recv(1024)
                if not data: break
                self.socket.send(data+'ok') #���� �����͸� �״�� Ŭ���̾�Ʈ�� ����
        except KeyboardInterrupt :
            return


if __name__ == '__main__':
    ser = Server('', 9000, 255*32)
    print("socket is ready.")
    print("run...")
    ser.run()
    ser.close()
    print("closed.")
