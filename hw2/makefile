all:
	mkdir -p bin
	g++ -std=c++17 src/validate_code.cpp src/f.cpp src/*_hw2.cpp -o bin/validate_code
clean:
	@rm bin/*
test%:
	@./bin/validate_code $(patsubst test%,%,$@) 0
leak_test%:
	@valgrind ./bin/validate_code $(patsubst test%,%,$@) 0
