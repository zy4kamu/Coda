#!/bin/sh

# choose build options
build_path=""
build_option=""
if [ "$1" = "-d" ]; then
	build_path="../build-debug"
	build_option="Debug"
elif [ "$1" = "-r" ]; then
	build_path="../build-release"
	build_option="Release"
else
	build_path="../build"
	build_option="Release"
fi

# create build path
if [ -d "$build_path" ]; then
	echo 'path exists...'
else
	echo "path doesn't exist... creating..."
	mkdir $build_path
fi

# choose numuber of processors to build
num_proc=`nproc`
if [ $num_proc -gt 1 ]; then
    num_proc=$((`nproc` - 1))
fi

# build
cd $build_path
cmake -DCMAKE_BUILD_TYPE:STRING="$build_option" ../Coda
make -j $num_proc

# copy libraries to python folder
python_lib_folder=../Coda/python/lib
if [ ! -d $python_lib_folder ]; then
    echo 'creating '$python_lib_folder' ...'
    mkdir -p $python_lib_folder
fi
ls bin | grep .so | xargs -I {} cp bin/{} $python_lib_folder
