#include <iostream>
#include <map>
#include <string.h>



//poor man's logging

#ifdef DEBUG
#define LOG1( text ) {\
		std::cout << (text) << std::endl;\
	}

#define LOG2( text, number ) {\
		std::cout << (text) << " :" << (number) << std::endl;\
	}


#define LOG3( text1, number, text2 ) {\
		std::cout << (text1) << ':' << (number) << (text2) << std::endl;\
	}

#define PRINT_ADDRESSES() {\
	for(auto it = sizes.begin(); it != sizes.end(); it++){\
		std::cout << "Size:" << it->second << std::endl;\
	}\
}
#else
#define LOG1( text ) {\
	}

#define LOG2( text, number ) {\
	}


#define LOG3( text1, number, text2 ) {\
	}

#define PRINT_ADDRESSES() {\
	}
#endif

class SmallAllocator {
public:
	SmallAllocator() {
		memoryBuffer = (char*)malloc(initialBufferSize);
		currentAllocatedSize = initialBufferSize;		
		LOG3("Initial allocated size", currentAllocatedSize, " bytes");
	}
	
	~SmallAllocator() {
		
	}
	
        void *Alloc(unsigned int Size) {		
		LOG3("Allocated ", Size, " bytes");
		//std::cout << Size << " bytes allocated" << std::endl;
		char *buffer = ReallocateByteBuffer(Size);                
		sizes.insert(std::pair<void*, unsigned int>((void*)buffer, Size));
		PRINT_ADDRESSES();
		return (void*)buffer;
        };
        
	void *ReAlloc(void *Pointer, unsigned int Size) {
		char *buffer;
		auto oldPointer = sizes.find(Pointer);
		if(oldPointer != sizes.end()){
			unsigned int oldSize = oldPointer->second;
			char *oldBuffer = (char*)oldPointer->first;
			buffer = ReallocateByteBuffer(Size);
			memcpy(buffer, oldBuffer, oldSize);
			sizes.erase(oldPointer->first);
			sizes.insert(std::pair<void*, unsigned int>((void*)buffer, Size));
		}
		LOG1("Reallocated");
                return (void*)buffer;
        };
        
	void Free(void *Pointer) {
		auto oldPointer = sizes.find(Pointer);
		freeSpaces.insert(std::pair<void*, unsigned int>(oldPointer->first, oldPointer->second));
		sizes.erase(oldPointer->first);
		PRINT_ADDRESSES();
        };
	
	
	
private:
	char* FindEmptyMemoryInBuffer(unsigned int Size) {
		for(auto it = freeSpaces.begin(); it != freeSpaces.end(); it++){
			if(it->second == Size){				
				LOG2("Size found", it->second);
				char *buffer = (char*)it->first;
				sizes.insert(std::pair<void*, unsigned int>(it->first, it->second));
				freeSpaces.erase(it->first);
			} else if(it->second > Size){
				LOG2("Size found", it->second);
				char *buffer = (char*)it->first;
				int newFreeLength = Size - it->second;
				sizes.insert(std::pair<void*, unsigned int>(it->first, Size));
				freeSpaces.insert(std::pair<void*, unsigned int>((char*)it->first + Size, newFreeLength));
				freeSpaces.erase(it->first);
			}
			
		}
	}
	char* ReallocateByteBuffer(unsigned int Size) {
				
	
		if (lastAllocatedByteOffset + Size >= currentAllocatedSize) {
			memoryBuffer = (char*)realloc(memoryBuffer, currentAllocatedSize + Size + 1024);			
			currentAllocatedSize += (Size + 1024);			
			LOG3("New allocator's size", currentAllocatedSize, " bytes");
		}
		char *result = memoryBuffer + lastAllocatedByteOffset;		
		lastAllocatedByteOffset += Size;
		return result;
	}
	
	std::map<void*, unsigned int> sizes;
	std::map<void*, unsigned int> freeSpaces;
	const int initialBufferSize = 1024;
	int currentAllocatedSize=0;
	char *memoryBuffer;
	int lastAllocatedByteOffset = 0;
};

int main(int argc, char **argv) {

	SmallAllocator A1;
	int * A1_P1 = (int *) A1.Alloc(sizeof(int));
	A1_P1 = (int *) A1.ReAlloc(A1_P1, 2 * sizeof(int));
	A1.Free(A1_P1);
	SmallAllocator A2;
	int * A2_P1 = (int *) A2.Alloc(10 * sizeof(int));
	for(unsigned int i = 0; i < 10; i++) A2_P1[i] = i;
	for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 1" << std::endl;
	int * A2_P2 = (int *) A2.Alloc(10 * sizeof(int));
	for(unsigned int i = 0; i < 10; i++) A2_P2[i] = -1;
	for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 2" << std::endl;
	for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 3" << std::endl;
	A2_P1 = (int *) A2.ReAlloc(A2_P1, 20 * sizeof(int));
	for(unsigned int i = 10; i < 20; i++) A2_P1[i] = i;
	for(unsigned int i = 0; i < 20; i++) if(A2_P1[i] != i) std::cout << "ERROR 4" << std::endl;
	for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 5" << std::endl;
	A2_P1 = (int *) A2.ReAlloc(A2_P1, 5 * sizeof(int));
	for(unsigned int i = 0; i < 5; i++) if(A2_P1[i] != i) std::cout << "ERROR 6" << std::endl;
	for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 7" << std::endl;
	A2.Free(A2_P1);
	A2.Free(A2_P2);
}