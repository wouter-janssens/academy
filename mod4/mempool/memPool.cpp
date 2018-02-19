#ifndef _MEMPOOL_C_
#define _MEMPOOL_C_
#include <iostream>



		//array of tuples, dynamically allocated(TODO: increase size when necessary, now always 10). tuple contains address of memory chunk, second is size, third is allocated/not allocates (true/false)
	//tuple<void*, unsigned int, bool> memArray;
	//	pointer to mem, size, taken
template<class T> void MemPool<T>::create_MemPool(unsigned int poolSize){
	this->totalMem = (T*)malloc(poolSize*sizeof(T));
	//std::cout << "mem address totalmem is "<<this->totalMem<<"\n";
	//this->memArray = malloc(DEFAUL_MEMARRAY_SIZE*sizeof(tuple<void*, unsigned int, bool>));
	
	tuple<T*, unsigned int, bool> mem_chunk = make_tuple(this->totalMem, poolSize, false);
	//std::cout << "memory allocated\n";

	memArray[0] = mem_chunk;

	

}
//create mempool of default size 1024 byte
template<class T> MemPool<T>::MemPool(){
	this->create_MemPool(DEFAULT_POOL_SIZE);
	this->nr_Of_Chunks = 1;
}
//create mempool of size bytes
template<class T> MemPool<T>::MemPool(int size){
	create_MemPool(size);
	this->nr_Of_Chunks = 1;
}


//dtor
template<class T> MemPool<T>::~MemPool(){
	std::cout << "mem address totalmem is "<<this->totalMem<<"\n";
	//release memory used by this mempool
	//delete this->totalMem;
	free(this->totalMem);
}
//void create_Mempool(unsigned int size);
//allocate size bytes in pool and return pointer to first
template<class T> T* MemPool<T>::pool_Malloc(unsigned int size){
	T* ptr = NULL;
	for(int i = 0; i<nr_Of_Chunks; i++){

		if(std::get<2>(memArray[i])){
			//std::cout << "mem chunck in mem array already taken\n";
		}else{ //chunk is free
			std::cout << "emppty mem chumk found\n";
			int chunk_size =std::get<1>(memArray[i]); 
			if(chunk_size == size){
				std::cout<< "exactly right, chunk is "<<chunk_size<<" and new size is "<<size<<"\n";
				std::get<2>(memArray[i]) = true;
				ptr = std::get<0>(memArray[i]) + chunk_size-size;
				return ptr;
			}
			else if(chunk_size > size){
				std::cout<< "big enough, chunk is "<<chunk_size<<" and new size is "<<size<<"\n";
				std::get<1>(memArray[i]) = chunk_size - size;
				ptr = std::get<0>(memArray[i]) + chunk_size-size;
				break;
			}else{
				std::cout << "too small\n";
			}
		}
	}
	if(ptr == NULL){
		throw Pool_Empty_Exception();
	}else{
		
		tuple<T*, unsigned int, bool> mem_chunk = make_tuple(ptr, size, true);
		//std::cout << "memory allocated\n";
		memArray[this->nr_Of_Chunks] = mem_chunk;
		this->nr_Of_Chunks++;
			
	}
	return ptr;
}
//will find the pointer in its array. throws Pool_Invalid_address_Exception if not found
template<class T> void MemPool<T>::pool_Free(void* ptr){
	bool found =false;
	for(int i = 0; i<nr_Of_Chunks; i++){
		if(std::get<0>(memArray[i]) == ptr){
			std::get<2>(memArray[i]) = false;
			found = true;
			break;
		}


		
	}
	if(!found){
		

		throw Pool_Invalid_Address_Exception();
	}
	
}	
//try to reduce number of chunks as much as possible (join adjacent ckunks)
template<class T> void MemPool<T>::reformat(){
	//check for every memory chunk if it is free amd next one is free
	for(int i = 0; i < nr_Of_Chunks; i++){
		//look for subsequent chunk
		for(int j = 0; j<nr_Of_Chunks-1; j++)
			if(i != j && std::get<0>(memArray[i]) == (std::get<0>(memArray[j])+std::get<1>(memArray[j]))){
					if((!std::get<2>(memArray[i])) &&(!std::get<2>(memArray[j]))){ 
					//adjacent and free chunks, concat and shorten array
				//	std::cout << i << " comes after "<< j << "\n";
					std::get<1>(memArray[j]) += std::get<1>(memArray[i]);
					memArray[i] = memArray[nr_Of_Chunks - 1];
					nr_Of_Chunks--;
					i--;
					break;	
				}
			}

		//this->printMemPool();	
	}


}

template<class T> void MemPool<T>::printTuple(tuple<T*, unsigned int , bool> tup) const{
	std::cout << std::get<0>(tup) << "\t";
	std::cout << "size: " << std::get<1>(tup) << " taken:\t";
	std::cout << std::get<2>(tup) << "\n";
}

template<class T> void MemPool<T>::printMemPool() const{
	for(int i = 0; i < nr_Of_Chunks; i++){
		this->printTuple(memArray[i]);


	}
}


#endif
