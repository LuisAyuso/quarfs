//
//  q-tests.h
//  quarfs-xCode
//
//  Created by Luis Ayuso on 10/05/14.
//
//

#pragma once
#include <iostream>
#include <vector>

#define TEST_CLASS_NAME_(test_case_name, test_name) \
   test_case_name##_##test_name##_Test

#define TEST(name,group) \
struct TEST_CLASS_NAME_(name,group) : public TestItem{\
    TEST_CLASS_NAME_(name,group)(){}\
    void operator()();\
};\
static int instance_name_group = TestManager::createTest<TEST_CLASS_NAME_(name,group)>();\
void TEST_CLASS_NAME_(name,group)::operator()()

struct TestItem{
    TestItem(){}
    TestItem(const std::string& name, const std::string& group ){

    }
    virtual void operator()(){
        
    }
};

class TestManager{

    TestManager(){
        
    }
    
    std::vector<std::shared_ptr<TestItem>> tests;
    
public:
    
    void addTest(const TestItem& t){
    }
    
    void operator()(){
        for (auto x : tests)
            (*x)();
    }
    
    static TestManager& getInstance(){
        static TestManager mgr;
        return mgr;
    }
    
    template<typename  T>
    static int createTest(){
        getInstance().tests.push_back(std::shared_ptr<TestItem>(new T()));
        return 1;
    }
};


//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

#define EXPECT_EQ(a,b) assert(a==b)


//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////



int main(){
    TestManager::getInstance()();
}
