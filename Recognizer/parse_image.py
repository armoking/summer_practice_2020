from keras.models import load_model
import numpy as np

filename = 'result.txt'
answerPath = 'answer.txt'
alphabet = list('0123456789аАбБвВгГдДежзийклмнопПрРстТуУфхцчшщъыьэюя')


if __name__ == '__main__':
	answer = ''	
	inputData = []
	index = 0
	inp = open(filename).read().split('\n')	
	letters_count = int(inp[index])
	index += 1
	
	cnn = load_model('Recognizer/main_cnn.h5')
	
	
	for _ in range(letters_count):
		
		n, m = list(map(int, inp[index].split()))
		index += 1
		
		cnt = [[0 for _ in range(32)] for _ in range(32)]
		arr = [[0 for _ in range(32)] for _ in range(32)]
		
		data = []
		black_pixels = 0
		for x in range(n):
			s = list(inp[index])
			index += 1
			for i in range(m):
				if s[i] == '0':
					s[i] = 0
				else:
					black_pixels += 1
					s[i] = 255
			data.append(s)
	
		for x in range(n):
			for y in range(m):
				val = data[x][y]
				x_index = int(x * 32 / n)
				y_index = int(y * 32 / m)
				cnt[x_index][y_index] += 1
				arr[x_index][y_index] += val

		for x in range(32):
			for y in range(32):
				arr[x][y] //= max(1, cnt[x][y])

		prediction = cnn.predict(np.array(arr).reshape((1, 32, 32, 1))).reshape(51)
		max_probability = 0
		current_result = '?'
		for i in range(51):
			if prediction[i] > max_probability:
				max_probability = prediction[i]
				current_result = alphabet[i]
		if black_pixels == 0:
			answer += ' '
		else:
			answer += current_result
	
	output = open(answerPath, 'w')
	output.write(answer)
	output.close()
	
	print("""***IMAGE HAS BEEN RECOGNIZED***""")
	print(answer)