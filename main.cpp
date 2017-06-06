#include <iostream>
#include <ctype.h>
#include <stdlib.h>

struct polynomialPart{
	int coefficient;
	int power;
	int parse(std::string polyString) {

		std::string::iterator it = polyString.begin();
		// parsing coefficient
		std::string coefString;
		int coef;
		while(isdigit(*it) || *it == '-') {
		  coefString += *it;
		  it++;
		}
		coef = atoi(coefString.c_str());
		std::cout << coef << std::endl;

		if( *it != '*' || *(++it) != 'x') {
		  std::cout << "Error!";
		  return -1;
		}

		it++;

		// parsing power
		if(*it == '^') {
		  it++;
		  std::string powerStr;
		  int power;
		  while(isdigit(*it) || *it == '-'){
		    powerStr = powerStr + *it;
		    it++;
		  }
		  power = atoi(powerStr.c_str());
		  std::cout << power;
		}
	}
};

std::string derivative (std::string polynomial){


	return (std::string)"";
}



int main (){
	std::string test1 = "-1*x^23+x";
	//std::cout << derivative(test1);
	polynomialPart p;
	p.parse(test1);
	return 0;
}
