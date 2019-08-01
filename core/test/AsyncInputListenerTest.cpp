#include <gtest/gtest.h>
#include "IInputPort_mock.h"
#include "AsyncInputListener.h"
#include <functional>

using testing::Return;
void ggg(){}
class AsyncInputListenerTest:public testing::Test{
public:
    IInputPort_mock input_mock;
    AsyncInputListener subject;
    AsyncInputListenerTest():subject(input_mock,std::bind(&IInputPort_mock::loggerFunction,&input_mock,std::placeholders::_1)){}
};
TEST(construciton,withoutLogger){
    IInputPort_mock input_mock;
    AsyncInputListener subject(input_mock);
    subject.StartListening();
    usleep(100);
    subject.StopListening();
}
TEST(destruction,stopListening){
    IInputPort_mock input_mock;
    EXPECT_CALL(input_mock,WaitForEvent(testing::_)).WillRepeatedly(Return(IInputPort::WaitResult::Timeout));
    EXPECT_CALL(input_mock,StopWaitingForEvent());
    {
        AsyncInputListener subject(input_mock);
        subject.StartListening();
        usleep(100);
    }
}
TEST_F(AsyncInputListenerTest, logger){
    EXPECT_CALL(input_mock,loggerFunction(testing::_)).Times(2);
    subject.StartListening();
    usleep(100);
    subject.StopListening();
}
