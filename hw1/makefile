all:
	mkdir -p bin
	g++ -std=c++17 src/validate_code.cpp src/*_hw1.cpp -o bin/validate_code
clean:
	@rm bin/*
test%:
	@./bin/validate_code inputs/test$(patsubst test%,%,$@)
leak_test%:
	@valgrind ./bin/validate_code inputs/test$(patsubst leak_test%,%,$@)
