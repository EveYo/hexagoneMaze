#include <iostream>

#include "cs221util/catch.hpp"
#include "quack.h"
#include "queue.h"
#include "stack.h"
using namespace std;

TEST_CASE("stack::basic functions", "[weight=1][part=stack]") {
    // cout << "Testing Stack..." << endl;
    Stack<int> intStack;
    vector<int> result;
    vector<int> expected;
    for (int i = 10; i > 0; i--) {
        expected.push_back(i);
    }
    for (int i = 1; i <= 10; i++) {
        intStack.push(i);
    }
    while (!intStack.isEmpty()) {
        result.push_back(intStack.pop());
    }
   
    REQUIRE(result == expected);
}

TEST_CASE("queue::basic functions", "[weight=1][part=queue]") {
    Queue<int> intQueue;
    vector<int> result;
    vector<int> expected;
    for (int i = 1; i <= 10; i++) {
        expected.push_back(i);
    }

    for (int i = 1; i <= 10; i++) {
        intQueue.enqueue(i);
    }
    while (!intQueue.isEmpty()) {
        result.push_back(intQueue.dequeue());
    }
    REQUIRE(result == expected);
}

 
TEST_CASE("quack:: resize pushPopRL","[weight=1][part=quack]" ){
    Quack<int> intQ;
    vector<int> result;
    int rS=0;

    vector<int> expected ={2,3};
    int eS = 2;

    for (int i = 1; i <= 4; i++) {
        intQ.pushR(i);
    }
    intQ.popR();
    intQ.popL();
    while(!intQ.isEmpty()){
        rS ++;
        result.push_back(intQ.popL());
    }
    REQUIRE(result == expected);
    REQUIRE(rS == eS);
}

TEST_CASE("quack:: resize pushPopLR","[weight=1][part=quack]" ){
    Quack<int> intQ;
    vector<int> result;
    int rS=0;

    vector<int> expected ={2,3};
    int eS = 2;

    for (int i = 1; i <= 4; i++) {
        intQ.pushR(i);
    }
    intQ.popL();
    intQ.popR();
    while(!intQ.isEmpty()){
        rS ++;
        result.push_back(intQ.popL());
    }
    REQUIRE(result == expected);
    REQUIRE(rS == eS);
}
