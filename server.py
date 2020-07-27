import socket
import main

HEADER_SIZE = 10
PORT = 1234

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((socket.gethostname(), PORT))

s.listen(5)


def get_message(message):
    header = str(len(message))
    header += ' ' * (HEADER_SIZE - len(header))
    return bytes(header + message, 'utf-8')


while True:
    client_socket, address = s.accept()
    print(f'Connection from {address} has been established')
    client_socket.send(get_message('Connection to the server has been established'))

    header = int(client_socket.recv(HEADER_SIZE).decode('utf-8'))
    result = client_socket.recv(header).decode('utf-8')

    recognized_text = main.__main__(result)
    print(recognized_text)
    client_socket.send(get_message(recognized_text))
    client_socket.close()
