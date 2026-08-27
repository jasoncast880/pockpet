#include "tester.hpp"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
	std::vector<int> integers;
	for( auto i = 1 ; i < argc ; i++ ) {
		integers.push_back(std::stoi(argv[i]));
	}
	auto sum = sum_integers(integers); 
	std::cout << sum << std::endl;
}
