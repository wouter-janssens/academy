#include <cppunit/TestFixture.h>
#include "memPool.hpp"
#include <cppunit/TestSuite.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>



class memPoolTest: public CppUnit::TestFixture{
	private: 
		MemPool<int> testPool;
	public:
		void setUp(){
		
		}
		void tearDown(){
		
		}
		memPoolTest(){
		
		}
		
		void testCtor(){
			MemPool<int> testPool;
			//check if there is one chunk
			CPPUNIT_ASSERT(testPool.nr_Of_Chunks == 1);	
			//check if chunck of memory of correct size (default) is ade
			CPPUNIT_ASSERT(std::get<1>(testPool.memArray[0]) == (DEFAULT_POOL_SIZE));
			//check if the one chunk is free
			CPPUNIT_ASSERT(std::get<2>(testPool.memArray[0]) == false);
		}
		void testMalloc(){
			MemPool<int> testPool;
			int* myInts = testPool.pool_Malloc(3);	
			int* myInts2 = testPool.pool_Malloc(2);	

			//check if there is one chunk
			CPPUNIT_ASSERT(testPool.nr_Of_Chunks == 3);	

			//check if chunck of memory of correct size (default) is ade
			CPPUNIT_ASSERT(std::get<1>(testPool.memArray[0]) == (DEFAULT_POOL_SIZE-5));
			CPPUNIT_ASSERT(std::get<1>(testPool.memArray[1]) == 3);
			CPPUNIT_ASSERT(std::get<1>(testPool.memArray[2]) == 2);

			//check if the one chunk is free
			CPPUNIT_ASSERT(std::get<2>(testPool.memArray[0]) == false);
			CPPUNIT_ASSERT(std::get<2>(testPool.memArray[1]) == true);
			CPPUNIT_ASSERT(std::get<2>(testPool.memArray[2]) == true);

			int* myInts3 = testPool.pool_Malloc(DEFAULT_POOL_SIZE - 2 - 3);
			CPPUNIT_ASSERT(testPool.nr_Of_Chunks == 3);	
			CPPUNIT_ASSERT(std::get<1>(testPool.memArray[0]) == DEFAULT_POOL_SIZE-2-3);
			CPPUNIT_ASSERT(std::get<2>(testPool.memArray[0]) == true);

			CPPUNIT_ASSERT_THROW_MESSAGE("",testPool.pool_Malloc(1),Pool_Empty_Exception);
		}
		void testFree(){
			MemPool<int> testPool;
			int* myInts = testPool.pool_Malloc(3);	
			int* myInts2 = testPool.pool_Malloc(2);	
			int* myInts3 = testPool.pool_Malloc(DEFAULT_POOL_SIZE - 2 - 3);
			CPPUNIT_ASSERT_THROW_MESSAGE("",testPool.pool_Malloc(1),Pool_Empty_Exception);
			testPool.pool_Free(myInts2);
			int* myInts4 = testPool.pool_Malloc(1);
			CPPUNIT_ASSERT(testPool.nr_Of_Chunks == 4);




		}
		void testReformat(){
			MemPool<int> testPool;
			int* myInts = testPool.pool_Malloc(3);	
			int* myInts2 = testPool.pool_Malloc(2);	
			int* myInts3 = testPool.pool_Malloc(5);
			int* myInts4 = testPool.pool_Malloc(8);	
			int* myInts5 = testPool.pool_Malloc(3);	
			int* myInts6 = testPool.pool_Malloc(10);

			CPPUNIT_ASSERT(testPool.nr_Of_Chunks == 7);
			int* myInts7;
		        CPPUNIT_ASSERT_THROW_MESSAGE("", myInts7 = testPool.pool_Malloc(10), Pool_Empty_Exception);
			
			testPool.pool_Free(myInts4);
			testPool.pool_Free(myInts5);
		        CPPUNIT_ASSERT_THROW_MESSAGE("", myInts7 = testPool.pool_Malloc(10), Pool_Empty_Exception);
			CPPUNIT_ASSERT(testPool.nr_Of_Chunks == 7);
			testPool.reformat();
			CPPUNIT_ASSERT(testPool.nr_Of_Chunks == 6);
		        myInts7 = testPool.pool_Malloc(10);
			CPPUNIT_ASSERT(testPool.nr_Of_Chunks == 7);
			
			
		}
		

		static CppUnit::TestSuite *suite(){
			CppUnit::TestSuite* my_suite = new CppUnit::TestSuite("memory pool test");
			my_suite -> addTest(new CppUnit::TestCaller<memPoolTest>("testCtor", &memPoolTest::testCtor));
			my_suite -> addTest(new CppUnit::TestCaller<memPoolTest>("testMalloc", &memPoolTest::testMalloc));
			my_suite -> addTest(new CppUnit::TestCaller<memPoolTest>("testFree", &memPoolTest::testFree));
			my_suite -> addTest(new CppUnit::TestCaller<memPoolTest>("testReformat", &memPoolTest::testReformat));
			return my_suite;
		}
};




int main(){
	CppUnit::TextUi::TestRunner my_runner;
	my_runner.addTest(memPoolTest::suite());
	my_runner.run();
	return 0;	
	
}
