#include <iostream>

class StringPointer {

public:
	StringPointer(std::string *p):pointer(p){
				
	}
	
	/*std::string* operator(){
		return pointer;
	}*/
	
	std::string operator*() {
		return *pointer;
	}
	
	std::string* operator->() {
		return pointer;
	}	
private:
	std::string *pointer;
};

int main (int argc, char **argv){
	std::string s1 = "Hello, world!";

	StringPointer sp1(&s1);
	StringPointer sp2(NULL);

	std::cout << sp1->length() << std::endl;
	std::cout << *sp1 << std::endl;
	std::cout << sp2->length() << std::endl;
	std::cout << *sp2 << std::endl;
}