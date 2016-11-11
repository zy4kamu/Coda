# -*- coding: utf-8 -*-
import socket

NUM_BUFFER_SIZE = 8
END_CONNECTION_STRING = 'end session'

class FixedSizeProcessClient:
    def __init__(self, file, messageSize=256, maxTrials=-1):
        self.messageSize = messageSize
        self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        notConnected = True
        trial = 0
        while (notConnected):
            try:
                trial += 1
                if (maxTrials != -1 and trial > maxTrials):
                    raise "Process client connection error"
                self.sock.connect(file)
                notConnected = False
            except:
                pass

    def Send(self, message):
        self.sock.send(bytes(message.encode('utf-8')))

    def Get(self):
        return self.sock.recv(self.messageSize)

    def Close(self):
        self.Send(END_CONNECTION_STRING)

class FloatSizeProcessClient:
    def __init__(self, file, maxTrials=-1):
        self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        notConnected = True
        trial = 0
        while (notConnected):
            try:
                trial += 1
                if (maxTrials != -1 and trial > maxTrials):
                    raise "Process client connection error"
                self.sock.connect(file)
                notConnected = False
            except:
                pass

    def Send(self, message):
        try:
            encoded = message.encode('utf-8')
            length = str(len(encoded)).encode('utf-8')
            while(len(length) < NUM_BUFFER_SIZE):
                length = '0' + length
            self.sock.send(bytes(length))
            self.sock.send(bytes(encoded))
        except (AttributeError):
            self.sock.close()
            raise

    def Get(self):
        try:
            size = self.sock.recv(NUM_BUFFER_SIZE)
            size = int(size.strip('\0'))
            return self.sock.recv(size)
        except (AttributeError):
            self.sock.close()
            raise

    def Close(self):
        self.Send(END_CONNECTION_STRING)

if __name__ == "__main__":
    client = FloatSizeProcessClient("/tmp/socket")
    for i in range(10):
        client.Send("Hello")
        responce = client.Get()
        print responce
    client.Close()