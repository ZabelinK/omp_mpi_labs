
app:
	rm -rf ./bin
	mkdir ./bin
	c++ -std=c++14 -fopenmp -I ./include  -L ./lib  -o ./bin/test_app ./test_app/test.cpp -l MyMath

static_lib_normal: lib_prep
	c++ -std=c++14 -o ./lib/libMyMath.o -c ./lib_src/normal.cpp
	ar rcs ./lib/libMyMath.a ./lib/libMyMath.o

static_lib_optimized: lib_prep
	c++ -std=c++14 -O2 -ftree-vectorize -fopt-info-vec-optimized -o ./lib/libMyMath.o -c ./lib_src/normal.cpp
	ar rcs ./lib/libMyMath.a ./lib/libMyMath.o

static_lib_vectorized: lib_prep
	c++ -std=c++14 -O2 -ftree-vectorize -fopt-info-vec-optimized -o ./lib/libMyMath.o -c ./lib_src/vectorized.cpp
	ar rcs ./lib/libMyMath.a ./lib/libMyMath.o

static_lib_aligned: lib_prep
	c++ -std=c++14 -O2 -ftree-vectorize -fopt-info-vec-optimized -o ./lib/libMyMath.o -c ./lib_src/vectorized_alligned.cpp
	ar rcs ./lib/libMyMath.a ./lib/libMyMath.o

static_lib_parallel: lib_prep
	c++ -std=c++14 -O2 -fopenmp -ftree-vectorize -fopt-info-vec-optimized -o ./lib/libMyMath.o -c ./lib_src/vectorized_alligned_parallel.cpp
	ar rcs ./lib/libMyMath.a ./lib/libMyMath.o

shared_lib_normal: lib_prep
	c++ -std=c++14 -fPIC -o ./lib/libMyMath.o -c ./lib_src/normal.cpp
	c++ -shared ./lib/libMyMath.o  -o ./lib/libMyMath.so
	sudo cp ./lib/libMyMath.so /usr/lib
	sudo chmod 755 /usr/lib/libMyMath.so


shared_lib: lib_prep
	c++ -std=c++14 -O2 -fPIC -fopenmp -ftree-vectorize -fopt-info-vec-optimized -o ./lib/libMyMath.o -c ./lib_src/vectorized_alligned_parallel.cpp
	c++ -shared -fopenmp ./lib/libMyMath.o  -o ./lib/libMyMath.so
	sudo cp ./lib/libMyMath.so /usr/lib
	sudo chmod 755 /usr/lib/libMyMath.so

lib_prep:
	rm -rf ./lib
	mkdir ./lib
