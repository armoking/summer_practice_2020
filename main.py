import os
from PIL import Image

PATH_TO_IMAGE = 'photo2.jpg'
PATH_TO_RESULT = 'result.txt'
PATH_TO_SOURCE = 'source.txt'
PATH_TO_PARSER = '/Parser/main.exe'
PATH_TO_CNN = 'Recognizer/parse_image.py'
PATH_TO_ANSWER = 'answer.txt'

# idea:
# 1. Client send image to Server
# 2. Server recognize image and transform it in text
# 3. Server send text to Client


def __main__(data):
    output = open(PATH_TO_SOURCE, 'w')
    output.write(data)
    output.close()

    print('Image has been read')
    os.system('.\\Parser\\main.exe')

    print('Image has been parsed')
    os.system('python ./Recognizer/parse_image.py')
    print('Recognition finished')
    return open(PATH_TO_ANSWER).read()
