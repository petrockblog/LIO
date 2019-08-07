#include <gtest/gtest.h>
#include <DebouncedListener.h>
#include "IInputPort_mock.h"
#include <thread>

using namespace std;
using testing::Return;

ACTION_P2(waitForEventBehave,retVal,duration){
    this_thread::sleep_for(duration);
    return retVal;
}
class DebouncedListenerTest : public testing::Test{
public:
    IInputPort_mock input_mock;
    DebouncedListener subject;
    DebouncedListenerTest():
    subject(input_mock,
            bind(&IInputPort_mock::onCallback,&input_mock),
            bind(&IInputPort_mock::offCallback,&input_mock),
            10ms){
    }

    // Test interface
protected:
    virtual void SetUp() override;
};

void DebouncedListenerTest::SetUp(){
    ON_CALL(input_mock,WaitForEvent(testing::_)).WillByDefault(waitForEventBehave(IInputPort::WaitResult::Timeout,chrono::milliseconds(1)));
}

TEST_F(DebouncedListenerTest,onCallback){
    EXPECT_CALL(input_mock,WaitForEvent(testing::_)).WillRepeatedly(waitForEventBehave(IInputPort::WaitResult::EventOccurred,chrono::milliseconds(1)));
    EXPECT_CALL(input_mock,Read()).WillRepeatedly(Return(true));
    EXPECT_CALL(input_mock,onCallback()).Times(1);
    EXPECT_CALL(input_mock,offCallback()).Times(0);
    this_thread::sleep_for(100ms);

}
TEST_F(DebouncedListenerTest,offCallback){
    EXPECT_CALL(input_mock,WaitForEvent(testing::_)).WillRepeatedly(waitForEventBehave(IInputPort::WaitResult::EventOccurred,chrono::milliseconds(1)));
    EXPECT_CALL(input_mock,Read()).WillRepeatedly(Return(true));
    EXPECT_CALL(input_mock,offCallback()).Times(0);
    this_thread::sleep_for(100ms);
    EXPECT_CALL(input_mock,Read()).WillRepeatedly(Return(false));
    EXPECT_CALL(input_mock,offCallback()).Times(1);
    this_thread::sleep_for(100ms);

}
