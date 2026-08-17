//spawn the base and tile entities ; enough to be render worthy
//perform the following tests:
//
//0 ) generate a sprite with known dimensions and see what happens to the tile indeces
//1 ) spawn large amount of entities and render (TEST OK DESIRED)
//	  check mainly for stack overflow 
//2 ) manipulate x-mov && y-mov entity across background. (TEST OK DESIRED)
//	  Check for gen. mem exception, stack overflow
//3 ) collision check (TEST FAIL DESIRED)
//	  Check for throw collsion exception. if thrown then success.	
//
//	  ALSO 
//	  this serves as a main cpp for the unit test. (testing harness)

//#include "engine_api.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <vector>

TEST_CASE("Sum of integers for a short vector", "[short]") {
	auto_integers = { 1, 2, 3, 4, 5, };
	REQUIRE(sum(integers) == 15);

}

/*
//layer/tilemap level testing
int tilemap_test( int . , char * ) {
	//call a sprite and see result. 
	//verify correctness and return 0 on success
	
	Layer test_layer = new L
	
	return 0;
}

//entities testing
int entities_stress_test() {
	
}
int edge_case_check() {
}
int collision_check() {
}

int main( int argc, char *argv[] ) {
	std:vector<int> integers ;
	for ( auto i = 1 ; i < argc ; i++ ) {
		integers.push_back(std:stoi(argv[i]));
	}
	auto sum = sum_integers(integers);

	std::cout << sum << std::endl;
}
*/
