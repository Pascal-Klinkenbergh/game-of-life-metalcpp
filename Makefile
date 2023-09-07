app: main.cc include/*.hpp shader
	clang++ -Wall -std=c++20 -Iinclude `pkg-config --libs sfml-graphics`  -framework Foundation -framework Metal -framework QuartzCore main.cc -o app

run: app
	./app


.PHONY: shader
shader: shader/*.metal
	xcrun metal shader/*.metal

.PHONY: format
format:
	clang-format -i *.cc include/*.hpp