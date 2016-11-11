list=$(ls -d -v $PWD/src/*/*.cpp)
for i in $list
do
	cp "$i" ./temp_cnv_file
	python CodeToCyrillic.py ./temp_cnv_file "$i"
done
list=$(ls -d -v $PWD/src/*/*.h)
for i in $list
do
	cp "$i" ./temp_cnv_file
	python CodeToCyrillic.py ./temp_cnv_file "$i"
done
rm ./temp_cnv_file
