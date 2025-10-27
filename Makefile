build_cmake: 
	cmake -S . -B build

build_app: 
	cmake --build build

run:
	./build/bin/try_box2d

clean:
	rm -rf build