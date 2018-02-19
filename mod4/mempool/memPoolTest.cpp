#include "memPool.hpp"


int main(){
	std::cout << "pool constructor:\n";
	MemPool<int> my_pool;
	//char* intArr = my_pool.pool_Malloc(10*sizeof(int));
	std::cout << "pool malloc:\n";
	
	std::cout << "pool free:\n";
	int* ptrarr[11];
	std::cout << "allocate ints\n";
	for(int i = 0; i < 11; i++){
		try{
		//	std::cout << "allocate " << i << "st element\n";
			ptrarr[i] = my_pool.pool_Malloc(4);
			for(int j = 0; j<4; j++){
				*(ptrarr[i]+j) = i*4+j;
			}
		}
		catch(Pool_Empty_Exception e){
	//		std::cout << "pool is empty, exception thrown, " << i << " set to null\n";
			ptrarr[i] = NULL;

		}
	}

	for(int i = 0;i < 9; i++){
	//	std::cout << i << ": ";
//		std::cout << "\n";
		if(ptrarr[i] == NULL){
			std::cout << "found null pointer\n";
			continue;
		}
//		std::cout << "will loop, ptr is "<<ptrarr[i] << "\n";
		for(int j = 0; j<4; j++){
			std::cout << *(ptrarr[i]+j)<< " ";
		}
//		std::cout << "\n";

	}

	
	for(int i = 0;i < 11; i++){
//		std::cout << "freeing "<<i << "\n";
		try{
			my_pool.pool_Free((ptrarr[i]));
		}catch(Pool_Invalid_Address_Exception e){
//			std::cout << "invalid free at i is " << i << " not free'd\n";
		}
	}
	my_pool.reformat();
	bool thrown = false;
	try{
		my_pool.pool_Free((ptrarr[3]+1));
	}catch(Pool_Invalid_Address_Exception e){
		thrown = true;
	}
	if(!thrown){
		std::cout << "no exception thrown\n";
	}
	std::cout << "\nprint mempool:\n";
	my_pool.printMemPool();

}
