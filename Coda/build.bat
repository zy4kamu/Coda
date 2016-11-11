echo "%1%"
if "%1%"=="d" (
	echo "BUILD DEBUG"
	mkdir "build_debug"
	cd "build_debug"
	cmake -G "Visual Studio 10" .. -DCMAKE_BUILD_TYPE=Debug
	cd ..
) else (
	if "%1%"=="r" (
		echo "BUILD RELEASE"		
		mkdir "build_release"
		cd "build_release"
		cmake -G "Visual Studio 10" .. -DCMAKE_BUILD_TYPE=Release
		cd ..
	) else (
		echo "BUILD DEBUG"
		mkdir "build_debug"
		cd "build_debug"
		cmake -G "Visual Studio 10" .. -DCMAKE_BUILD_TYPE=Debug
		cd ..

		echo "BUILD RELEASE"		
		mkdir "build_release"
		cd "build_release"
		cmake -G "Visual Studio 10" .. -DCMAKE_BUILD_TYPE=Release
		cd ..
	)
)
