list=$(ls -d -v $PWD/src/*/*.cpp)
for i in $list
do
	python CyrillicToCode.py "$i" ./temp_cnv_file
	cp ./temp_cnv_file "$i"
done
list=$(ls -d -v $PWD/src/*/*.h)
for i in $list
do
	python CyrillicToCode.py "$i" ./temp_cnv_file
	cp ./temp_cnv_file "$i"
done
rm ./temp_cnv_file
