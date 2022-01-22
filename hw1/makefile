all:
	g++ -std=c++17 src/validate_code.cpp src/*_hw1.cpp -o bin/validate_code
clean:
	@rm bin/*
test%:
	@./bin/validate_code inputs/test$(patsubst test%,%,$@)