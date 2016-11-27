#!/bin/sh

# choose build options
build_path=""
build_option=""
if [ "$1" = "-d" ]; then
	build_path="../build-debug"
	build_option="Debug"
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

# create output folder
if [ $build_option = "Release" ]; then
    output_folder=/opt/coda/
    if [ -d $output_folder ]; then
        rm -rf $output_folder
    fi
    mkdir $output_folder
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
if [ $build_option = "Release" ]; then
config_folder=$output_folder'config/'
bin_folder=$output_folder'bin/'
lib_folder=$output_folder'lib/'
cmake -DCMAKE_BUILD_TYPE:STRING="$build_option" \
      -DCONFIG_ROOT_DIR:STRING="$config_folder" \
      -DEXECUTABLE_OUTPUT_PATH:STRING="$bin_folder" \
      -DLIBRARY_OUTPUT_PATH:STRING="$lib_folder" \
      ../Coda
else
cmake -DCMAKE_BUILD_TYPE:STRING="$build_option" \
      ../Coda
fi
make -j $num_proc

# copy libraries to output folder
if [ $build_option = "Release" ]; then
    output_lib_folder=$output_folder'lib/'
    if [ ! -d $output_lib_folder ]; then
        echo 'creating '$output_lib_folder' ...'
        mkdir -p $output_lib_folder
    fi
    ls bin | grep .so | xargs -I {} cp bin/{} $output_lib_folder
fi

# copy configs to output folder
cd -
if [ $build_option = "Release" ]; then
    ln -s `pwd`/config $output_folder'config'
fi
