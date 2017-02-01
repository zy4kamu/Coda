#!/bin/sh

# choose build options
main_folder="/opt/coda/"
build_path=""
build_option=""
output_folder=""
if [ "$1" = "-d" ]; then
	build_path="../coda-build-debug"
	build_option="Debug"
    output_folder=$main_folder"debug/"
else
	build_path="../coda-build-release"
	build_option="Release"
    output_folder=$main_folder"release/"
fi

# create build path
if [ -d "$build_path" ]; then
	echo "path exists..."
else
	echo "path doesn't exist... creating..."
	mkdir $build_path
fi

# create output folder
if [ $build_option = "Release" ]; then
    if [ -d $output_folder ]; then
        rm -rf $output_folder
    fi
    mkdir -p $output_folder
    if [ $? -ne 0 ]; then
        echo "couldn't access $output_folder, check permissions ... "
        exit 0
    fi
fi

# choose number of processors to build
num_proc=`nproc`
if [ $num_proc -gt 1 ]; then
    num_proc=$((`nproc` - 1))
fi

# build
cd $build_path
config_folder=$main_folder"config"
bin_folder=$output_folder"bin/"
lib_folder=$output_folder"lib/"
cmake -DCMAKE_BUILD_TYPE:STRING="$build_option" \
      -DCONFIG_ROOT_DIR:STRING="$config_folder"/ \
      -DCMAKE_RUNTIME_OUTPUT_DIRECTORY:STRING="$bin_folder" \
      -DCMAKE_LIBRARY_OUTPUT_DIRECTORY:STRING="$lib_folder" \
      ../Coda
make -j $num_proc

# copy configs to output folder
cd -
if [ ! -d $config_folder ]; then
    sudo ln -s `pwd`/config $config_folder
fi
