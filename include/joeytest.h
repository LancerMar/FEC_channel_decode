#include <iostream>

#define UNIT_TEST_HEADER(STRING) std::cout<<" ### [UNIT_TEST] : "<<STRING<<" ####"<<std::endl;
#define ENTER() std::cout<<std::endl;
#define UNIT_TEST_SUM(STRING) std::cout<<"\r\n ##### [UNIT_TEST_SUM] : "<<STRING<<" #####"<<std::endl;

struct UTEST_result{
    int total_case = 0;             // all test case count
    int failed_case = 0;            // failed test case count
    int success_case = 0;           // success test case count  
    
    void print(){
        std::cout<< "[UNIT_TEST_count] : "<< total_case <<std::endl;
        std::cout<< "[PASS] UNIT test count : "<< failed_case<<std::endl;
        std::cout<< "[FAIL] UNIT test count : "<< success_case<<std::endl;
    }
};


