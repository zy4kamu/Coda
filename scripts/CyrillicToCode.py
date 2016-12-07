#!/usr/local/bin/python 
# -*- coding: utf-8 
import codecs
import re
import sys

def CyrrilicToCode(iname,oname):
	f = codecs.open(iname, encoding='utf-8')
	f2 = codecs.open(oname, encoding='utf-8', mode='w+')
	flag = 0
	for line in f:
		if line[len(line)-2:len(line)]=="\r\n":
			line = line[0:-2]
		if line[len(line)-1:len(line)]=="\n":
			line = line[0:-1]
		commentary = ""
		reg = re.compile(ur"([а-яА-ЯёЁ]+)")
		res = reg.findall(line)
		if res:
			flag = 1
			commentary = "//here was cyrrilic symbols: "+",".join(res)
			for i in range(0,len(res)):
				s = repr(res[i])
				s = s[2:-1]
				line = reg.sub(s,line,1)
			line = line	+ commentary+"\n"
		else:
			line = line+"\n"
		f2.write(line)
	if flag:
		print iname
	f.close()
	f2.close()
			
		
if len(sys.argv)!=3:
	print "Bad input format. Too few arguments"
else:
	CyrrilicToCode(sys.argv[1], sys.argv[2])
	
