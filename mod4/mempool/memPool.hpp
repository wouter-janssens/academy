#ifndef _MEMPOOL_H_
#define _MEMPOOL_H_

#include <iostream>
#include <stdexcept>
#include <exception>
#include <tuple>
#include <array>
const unsigned int DEFAULT_MEMARRAY_SIZE = 100;
const unsigned int DEFAULT_POOL_SIZE = 32;

using namespace std;
class Pool_Invalid_Address_Exception: public exception{};
class Pool_Empty_Exception: public exception{};
template <class T>
class MemPool{
	private:
		//array of tuples, dynamically allocated(TODO: increase size when necessary, now always 10). tuple contains address of memory chunk, second is size, third is allocated/not allocates (true/false)
		array<tuple<T*, unsigned int, bool>,DEFAULT_MEMARRAY_SIZE> memArray;
		//size in number of T ocjects
		void create_MemPool(unsigned int size);
		T* totalMem;
		int nr_Of_Chunks;
		friend class memPoolTest;
	public:
		//create mempool of default size 1024 byte
		MemPool();
		//create mempool of size bytes
		MemPool(int size);

		//remove all of the allocated mem
		~MemPool();
		
		//allocate memory of size T objects in pool and return pointer to first
		T* pool_Malloc(unsigned int size);
		
		void reformat();
		//will find the pointer in its array. throws Pool_Invalid_address_Exception if not found
		void pool_Free(void* ptr);
		void printMemPool() const;
		void printTuple(tuple<T*, unsigned int, bool> tup) const;
};


#include "memPool.cpp"


#endif
