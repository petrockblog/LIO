#include <gmock/gmock.h>
#include <Debouncer.h>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <thread>
using namespace std;

class Debounce_test : public ::testing::Test{
private:
    mutex m;
    condition_variable cnd;
        atomic<bool> exit;
public:
    Debouncer subject;
    MOCK_METHOD0(notifyCalled,void());
    // Sets up the test fixture.
    thread *t;
    virtual void SetUp()override{
        subject.setSampleInterval(std::chrono::duration<uint64_t,std::milli>(100));
        subject.setNotifyer(&cnd);
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
    EXPECT_CALL(*this,notifyCalled()).Times(0);
    subject.off();
    usleep(100000);
}
TEST_F(Debounce_test, totalOff){
    EXPECT_CALL(*this,notifyCalled()).Times(0);
    subject.off();
    usleep(500000);
}
TEST_F(Debounce_test, partialOn){
    EXPECT_CALL(*this,notifyCalled()).Times(0);
    subject.off();
    usleep(100000);
    subject.on();
    usleep(100000);
}
TEST_F(Debounce_test, bounceOn){
    EXPECT_CALL(*this,notifyCalled()).Times(0);
    subject.off();
    usleep(50000);
    subject.on();
    usleep(20000);
    subject.off();
    usleep(20000);
    subject.on();
    usleep(20000);
}
TEST_F(Debounce_test, sitchedOn){
    EXPECT_CALL(*this,notifyCalled()).Times(1);
    subject.off();
    usleep(20000);
    subject.on();
    usleep(20000);
    subject.off();
    usleep(20000);
    subject.on();
    usleep(200000);
}


