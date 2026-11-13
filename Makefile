CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Iinclude

test: build/test_bs
	./build/test_bs

build/test_bs: src/bs.cpp tests/test_bs.cpp include/bs.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) src/bs.cpp tests/test_bs.cpp -o $@

build/price: src/bs.cpp src/main.cpp include/bs.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) src/bs.cpp src/main.cpp -o $@

clean:
	rm -rf build
