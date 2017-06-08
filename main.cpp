#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include <string>
#include <set>

struct polynomialPart{
	int coefficient = 1;
	int power = 1;
	std::string toString() {
	  return std::to_string(coefficient) + "*x^" + std::to_string(power) + '\n';
	}
	std::string::iterator parse(std::string::iterator it) {		
		// parsing coefficient
		std::string coefString;
		while(isdigit(*it) || (*it == '-') || (*it == '+')) {		
		  coefString += *it;
		  it++;
		}			
		coefficient = atoi(coefString.c_str());
		if(!coefficient) {
		    coefficient = 1;
		}
		
		if( *it == '*') it++;
		if( *it == 'x') it++;
		if( *it == '^') { 
		// parsing power
			it++;
			std::string powerStr;
			power;
			while(isdigit(*it)) {
			powerStr = powerStr + *it;
			it++; }
			power = atoi(powerStr.c_str());
		}
		else {
			power = 1;
			return it;
		}		
		
		return it;
	}
};

std::string derivative (std::string polynomial){


	return (std::string)"";
}



int main (){
	std::string test1 = "-15*x^23+x+12*x^2";	
	
	std::string::iterator it = test1.begin();
	std::set <int, polynomialPart> polynomial;
	
	while(it != test1.end()){
		polynomialPart p;
		it = p.parse(it);
		std::cout << p.toString();	
		
	}
	
	
	return 0;
}
