#include <gtest/gtest.h>
#include "IInputPort_mock.h"
#include "AsyncInputListener.h"
#include <functional>
#include <condition_variable>

using testing::Return;
using namespace std;

class AsyncInputListenerTest:public testing::Test{
public:
    IInputPort_mock input_mock;
    AsyncInputListener subject;
    AsyncInputListenerTest():subject(input_mock,bind(&IInputPort_mock::loggerFunction,&input_mock,std::placeholders::_1)){
        subject.SetOnCallback(bind(&IInputPort_mock::onCallback,&input_mock));
        subject.SetOffCallback(bind(&IInputPort_mock::offCallback,&input_mock));
    }
};
TEST(construciton,initWithoutLogger){

    IInputPort_mock input_mock;
    AsyncInputListener subject(input_mock);
    subject.StartListening();
    this_thread::sleep_for(1ms);
    subject.StopListening();
}
TEST(destruction,stopListening){
    IInputPort_mock input_mock;
    condition_variable cv;
    unique_lock<mutex> l;
    EXPECT_CALL(input_mock,WaitForEvent(testing::_)).WillRepeatedly(Return(IInputPort::WaitResult::Timeout));
    {
        AsyncInputListener subject(input_mock);
        EXPECT_CALL(input_mock,StopWaitingForEvent()).Times(2);
        subject.StartListening();
        this_thread::sleep_for(1ms);
    }
}
TEST_F(AsyncInputListenerTest, logger){
    EXPECT_CALL(input_mock,loggerFunction(testing::_)).Times(2);
    subject.StartListening();
    this_thread::sleep_for(1ms);
    subject.StopListening();
}
TEST_F(AsyncInputListenerTest,TurnOnCallback){
    EXPECT_CALL(input_mock,WaitForEvent(testing::_)).WillRepeatedly(Return(IInputPort::WaitResult::EventOccurred));
    EXPECT_CALL(input_mock,Read()).WillRepeatedly(Return(true));
    EXPECT_CALL(input_mock,onCallback()).Times(testing::AtLeast(1));
    EXPECT_CALL(input_mock,offCallback()).Times(0);
    subject.StartListening();
    this_thread::sleep_for(1ms);
    subject.StopListening();
}
TEST_F(AsyncInputListenerTest,TurnOffCallback){
    EXPECT_CALL(input_mock,WaitForEvent(testing::_)).WillRepeatedly(Return(IInputPort::WaitResult::EventOccurred));
    EXPECT_CALL(input_mock,Read()).WillRepeatedly(Return(false));
    EXPECT_CALL(input_mock,offCallback()).Times(testing::AtLeast(1));
    EXPECT_CALL(input_mock,onCallback()).Times(0);
    subject.StartListening();
    this_thread::sleep_for(1ms);
    subject.StopListening();
}
TEST_F(AsyncInputListenerTest,ExitSignal){
    EXPECT_CALL(input_mock,WaitForEvent(testing::_)).WillOnce(Return(IInputPort::WaitResult::EventOccurred))
                                                    .WillOnce(Return(IInputPort::WaitResult::Exit));
    EXPECT_CALL(input_mock,Read()).WillRepeatedly(Return(false));
    EXPECT_CALL(input_mock,offCallback()).Times(1);
    EXPECT_CALL(input_mock,onCallback()).Times(0);
    subject.StartListening();
    this_thread::sleep_for(1ms);
}
TEST_F(AsyncInputListenerTest,restartListening){
    EXPECT_CALL(input_mock,WaitForEvent(testing::_)).WillOnce(Return(IInputPort::WaitResult::Exit));
    EXPECT_CALL(input_mock,Read()).WillRepeatedly(Return(false));
    EXPECT_CALL(input_mock,offCallback()).Times(0);
    EXPECT_CALL(input_mock,onCallback()).Times(0);
    subject.StartListening();
    this_thread::sleep_for(10ms);
    EXPECT_CALL(input_mock,WaitForEvent(testing::_)).WillRepeatedly(Return(IInputPort::WaitResult::EventOccurred));
    EXPECT_CALL(input_mock,Read()).WillRepeatedly(Return(false));
    EXPECT_CALL(input_mock,offCallback()).Times(testing::AtLeast(1));
    EXPECT_CALL(input_mock,onCallback()).Times(0);
    subject.StartListening();
    this_thread::sleep_for(1ms);
    subject.StopListening();
}
TEST_F(AsyncInputListenerTest,Timeout){
    EXPECT_CALL(input_mock,WaitForEvent(testing::_)).WillRepeatedly(Return(IInputPort::WaitResult::Timeout));
    EXPECT_CALL(input_mock,Read()).Times(0);
    EXPECT_CALL(input_mock,offCallback()).Times(0);
    EXPECT_CALL(input_mock,onCallback()).Times(0);
    subject.StartListening();
    this_thread::sleep_for(1ms);
    subject.StopListening();


}
