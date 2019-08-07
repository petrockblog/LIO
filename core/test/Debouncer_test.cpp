#include <gmock/gmock.h>
#include <Debouncer.h>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <thread>
#include "IDeboucerStrategy_mock.h"

using namespace std;

class Debounce_test : public ::testing::Test{
private:
    mutex m;
    condition_variable cnd;
    atomic<bool> exit;
public:
    Debouncer subject;
    IDeboucerStrategy_mock strategy;
    MOCK_METHOD0(notifyCalled,void());
    // Sets up the test fixture.
    thread *t;
    virtual void SetUp()override{
        subject.setDebounceInterval(100ms);
        subject.setStrategy(&strategy);
        exit=false;
        t=new thread(&Debounce_test::run,this);
    }
    virtual void TearDown()override{
        exit=true;
        cnd.notify_all();
        t->join();
        delete t;
    }
private:
    void run(){
        while(!exit){
            unique_lock<mutex> lck(m);
            cnd.wait(lck);
            if(exit)
                break;
            notifyCalled();
        }
    }

};

TEST_F(Debounce_test, partialOff){
    EXPECT_CALL(strategy,handleOffEvent()).Times(0);
    EXPECT_CALL(strategy,handleOnEvent()).Times(0);
    subject.off();
    this_thread::sleep_for(50ms);
}
TEST_F(Debounce_test, totalOff){
    EXPECT_CALL(strategy,handleOffEvent()).Times(0);
    EXPECT_CALL(strategy,handleOnEvent()).Times(0);
    subject.off();
    this_thread::sleep_for(500ms);
}
TEST_F(Debounce_test, partialOn){
    EXPECT_CALL(strategy,handleOffEvent()).Times(0);
    EXPECT_CALL(strategy,handleOnEvent()).Times(0);
    subject.off();
    this_thread::sleep_for(50ms);
    subject.on();
    this_thread::sleep_for(50ms);
}
TEST_F(Debounce_test, bounceOn){
    EXPECT_CALL(strategy,handleOffEvent()).Times(0);
    EXPECT_CALL(strategy,handleOnEvent()).Times(0);
    subject.off();
    this_thread::sleep_for(50ms);
    subject.on();
    this_thread::sleep_for(20ms);
    subject.off();
    this_thread::sleep_for(20ms);
    subject.on();
    this_thread::sleep_for(20ms);
}
TEST_F(Debounce_test, switchedOn){
    EXPECT_CALL(strategy,handleOffEvent()).Times(0);
    EXPECT_CALL(strategy,handleOnEvent()).Times(1);
    subject.off();
    this_thread::sleep_for(20ms);
    subject.on();
    this_thread::sleep_for(20ms);
    subject.off();
    this_thread::sleep_for(20ms);
    subject.on();
    this_thread::sleep_for(120ms);
}
TEST_F(Debounce_test, switchedOnAndOff){
    EXPECT_CALL(strategy,handleOffEvent()).Times(1);
    EXPECT_CALL(strategy,handleOnEvent()).Times(1);

    subject.on();
    this_thread::sleep_for(120ms);
    subject.off();
    this_thread::sleep_for(20ms);
    subject.on();
    this_thread::sleep_for(20ms);
    subject.off();
    this_thread::sleep_for(120ms);
}

