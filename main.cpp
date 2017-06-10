#define DEBUG

#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include <string>
#include <map>

struct polynomialPart{
	int coefficient = 0;
	int power = 0;
	bool zero = false;
	
	std::string toStringDebug(){
		return "coefficient " + std::to_string(coefficient) + " power " + std::to_string(power) + "\n";
	}
	std::string toString(bool addEndline = false) {
	
		std::string varPart;	
		std::string coeffPart;
		if(zero) {return "";}
		if(power == 1){
			varPart = "x";
		} else if (power != 0) {
			varPart = "x^" + std::to_string(power);
		}
		
		if(coefficient == 1){
			if(power == 0){
				coeffPart = "1";
			} else {
			   coeffPart = "";
			}
		} else {
			
			coeffPart = std::to_string(coefficient);
			if(power != 0){
			    coeffPart += "*";
			}
		}
		
	
		return coeffPart + varPart + (addEndline ? "\n" : "");
	}
	
	void derivative() {
		if(power == 0){
		    zero = true;
		    return;
		}
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
		if(coefString == "-") {
			coefString += '1';
		}
		
		coefficient = atoi(coefString.c_str());		
		if(!coefficient) {
		    coefficient = 1;
		}
		
		if( *it == '*') it++;
		if( *it == 'x') { it++; power = 1;} 
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
			return it;
		}		
		
		return it;
	}
};

std::string derivative (std::string polynomial){

	std::string::iterator it = polynomial.begin();
	std::map <int, polynomialPart> polynomialMap;
	
	while(it != polynomial.end()){
		polynomialPart p;
		it = p.parse(it);
		//p.derivative();		
		auto existingPart = polynomialMap.find(p.power);
		if(existingPart !=  polynomialMap.end()){
			existingPart->second.add(p);
		} else {
			polynomialMap[p.power] = p;
		}
		#ifdef DEBUG
		std::cout << p.toStringDebug();	
		#endif
	}
	
	std::string result;
	
	int i = 0;
	for(auto it = polynomialMap.rbegin(); it != polynomialMap.rend(); it++){
		auto x = *it;
		x.second.derivative();
		#ifdef DEBUG
		std::cout << x.second.toStringDebug();
		#endif
		if(i++ && x.second.coefficient > 0 && !x.second.zero){
			result += "+";
		}
		result += x.second.toString();
		
	}	
	return result;
}



int main (){
	//std::string test1 = "-15*x^23+x+12*x^2+16*x^23";	
	std::string test1 = "x+x+x+x+x+x+x+x+x+x";
	//std::string test1 = "x^10000+x+1";
	std::cout << derivative(test1);
	
	
	return 0;
}
