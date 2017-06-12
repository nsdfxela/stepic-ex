#include <iostream>
#include <map>
#include <string.h>
#define DEBUG

#define LOG1( text ) {\
		std::cout << (text) << std::endl;\
	}


#define LOG3( text1, number, text2 ) {\
		std::cout << (text1) << ':' << (number) << (text2) << std::endl;\
	}

#define PRINT_ADDRESSES() {\
	for(auto it = sizes.begin(); it != sizes.end(); it++){\
		std::cout << "Size:" << it->second << std::endl;\
	}\
}

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
			sizes.remove(oldPointer->first);
			sizes.insert(std::pair<void*, unsigned int>((void*)buffer, Size));
		}
		LOG1("Reallocated");
                return (void*)buffer;
        };
        
	void Free(void *Pointer) {
                //return free(Pointer);
        };
	
private: 
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
	const int initialBufferSize = 1024;
	int currentAllocatedSize=0;
	char *memoryBuffer;
	int lastAllocatedByteOffset = 0;
};

int main(int argc, char **argv) {

	SmallAllocator A1;
	int * A1_P1 = (int *) A1.Alloc(sizeof(int)*1000);
	A1_P1 = (int *) A1.ReAlloc(A1_P1, 2 * sizeof(int));
	A1.Free(A1_P1);
	return 0;
}