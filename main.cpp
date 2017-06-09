#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include <string>
#include <map>

struct polynomialPart{
	int coefficient = 1;
	int power = 1;
	
	std::string toString() {
	  return std::to_string(coefficient) + "*x^" + std::to_string(power) + '\n';
	}
	
	void derivative() {
		coefficient *= power;
		power--;
	}
	
	void add(const polynomialPart &p){
		if(p.power == power){
			coefficient +=p.coefficient;
		}
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
	std::string test1 = "-15*x^23+x+12*x^2+16*x^23";	
	
	std::string::iterator it = test1.begin();
	std::map <int, polynomialPart> polynomial;
	
	while(it != test1.end()){
		polynomialPart p;
		it = p.parse(it);
		//p.derivative();		
		auto existingPart = polynomial.find(p.power);
		if(existingPart !=  polynomial.end()){
			existingPart->second.add(p);
		} else {
			polynomial[p.power] = p;
		}
		std::cout << p.toString();	
		
	}
	printf("Polynomial: \n");
	for(auto &x : polynomial){
		std::cout << x.second.toString();
	}
	
	
	return 0;
}
