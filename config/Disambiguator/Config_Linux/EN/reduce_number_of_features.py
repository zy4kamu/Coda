import operator
import os
from subprocess import call

if __name__ == "__main__":
	dic = {}
	trancated = {}
	with open('train.txt')  as reader:
		for line in reader:
			line = line.strip()
			if (len(line) > 0):
				features = line.split(' ')[1].split(',')
				for feature in features:
					if feature in dic:
						dic[feature] += 1
					else:
						dic[feature] = 1
		items = dic.items()
		print len(items)
		items = [item for item in items if item[1] > 50]
		print len(items)
		for item in items:
			trancated[item[0]] = item[1]
	
	with open('train1.txt', 'w') as writer:		
		with open('train.txt')  as reader:
			for line in reader:
				line = line.strip()
				if (len(line) > 0):
					splitted = line.split(' ')
					features = splitted[1].split(',')
					features = [f for f in features if f in trancated]
					line = splitted[0] + ' ' + ','.join(features) + ' ' + splitted[2]
					writer.write(line)
					writer.write('\n')
				else:
					writer.write('\n')
					
	with open('test1.txt', 'w') as writer:		
		with open('test.txt')  as reader:
			for line in reader:
				line = line.strip()
				if (len(line) > 0):
					splitted = line.split(' ')
					features = splitted[1].split(',')
					features = [f for f in features if f in trancated]
					line = splitted[0] + ' ' + ','.join(features) + ' ' + splitted[2]
					writer.write(line)
					writer.write('\n')
				else:
					writer.write('\n')
					
	call(['rm', 'train.txt'])
	call(['mv', 'train1.txt', 'train.txt'])
	call(['rm', 'test.txt'])
	call(['mv', 'test1.txt', 'test.txt'])
				
		
