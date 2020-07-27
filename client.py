import socket
from PIL import Image

HEADER_SIZE = 10
PORT = 1234

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((socket.gethostname(), PORT))


def get_message(text):
    header = str(len(text))
    header += ' ' * (HEADER_SIZE - len(header))
    return bytes(header + text, 'utf-8')


msg = s.recv(HEADER_SIZE)
message_size = int(msg.decode('utf-8'))
print('Message size is %d' % message_size)
msg = s.recv(message_size)
print(msg.decode('utf-8'))


def get_data(path_to_image):
    image = Image.open(path_to_image)
    w, h = image.size
    pixel_map = image.load()
    colors = ['%d %d\n' % (w, h)]
    for x in range(w):
        for y in range(h):
            r, g, b = pixel_map[x, y]
            color = (r << 16) ^ (g << 8) ^ b
            colors.append(str(color))
        colors.append('\n')
    print(w, h)
    return ' '.join(colors)


data = get_data('C:\\Users\\User\\Desktop\\github\\summer_practice_2020\\photo5.jpg')
message = get_message(data)

while True:
    s.send(message)
    try:
        header = s.recv(HEADER_SIZE)
        header = int(header.decode('utf-8'))
        result = s.recv(header * 2).decode('utf-8')
        print(result)
        break
    except Exception:
        pass