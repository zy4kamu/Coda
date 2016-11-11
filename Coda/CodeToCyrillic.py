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
		commentary = ""
		reg = re.compile(ur"(\\u04[1234][0-9a-f]|\\u04[05]1)")
		res = reg.findall(line)
		if res:
			flag = 1
			commentary = "//here was cyrrilic symbols: "+",".join(res)
			for i in range(0,len(res)):
				l = res[i].decode('unicode_escape','strict')
				line = reg.sub(l,line,1)
			line = re.sub("//here was cyrrilic symbols:.*","",line)	
		f2.write(line)
	if flag:
		print iname
	f.close()
	f2.close()
			
		
if len(sys.argv)!=3:
	print "Bad input format. Too few arguments"
else:
	CyrrilicToCode(sys.argv[1], sys.argv[2])
	
