
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "event_bus.h"

class TestEvent : public Event {
 public:
  TestEvent(std::string msg) : _msg(msg) {}

  std::string GetMessage() { return _msg; }

 private:
  std::string _msg;
};

class TestListener : public EventHandler<TestEvent> {
 public:
  std::string message = "";

  virtual void OnEvent(TestEvent &e) override {
    // Ignore the event if it's already been canceled
    if (e.GetCanceled()) {
      return;
    }

    // Cancel the event if the event message is "cancel"
    if (e.GetMessage() == "cancel") {
      e.SetCanceled(true);
      message = "canceled";
      return;
    }

    message = e.GetMessage();
  }
};

TEST_CASE("Eventhandler got canceled", "[eventtest]") {
  TestListener testListener;
  EventRegistration *eventReg;
  eventReg = EventBus::AddHandler<TestEvent>(testListener);

  std::string msg = "cancel";
  TestEvent testEvent(msg);
  EventBus::FireEvent(testEvent);

  REQUIRE(testEvent.GetCanceled() == true);
  REQUIRE(testListener.message == "canceled");
  eventReg->RemoveHandler();
}

TEST_CASE("Eventhandler not got canceled", "[eventtest]") {
  TestListener testListener;
  EventRegistration *eventReg;
  eventReg = EventBus::AddHandler<TestEvent>(testListener);

  std::string msg = "testmessage";
  TestEvent testEvent(msg);
  EventBus::FireEvent(testEvent);

  REQUIRE(testListener.message == msg);
  eventReg->RemoveHandler();
}

TEST_CASE("Eventbus fires for multiple eventhandlers", "[eventtest]") {
  TestListener testListener;
  TestListener testListener2;
  EventRegistration *eventReg;
  EventRegistration *eventReg2;
  eventReg = EventBus::AddHandler<TestEvent>(testListener);
  eventReg2 = EventBus::AddHandler<TestEvent>(testListener2);

  std::string msg = "testmessage";
  TestEvent testEvent(msg);
  EventBus::FireEvent(testEvent);

  REQUIRE(testListener.message == msg);
  REQUIRE(testListener2.message == msg);
  eventReg->RemoveHandler();
  eventReg2->RemoveHandler();
}

TEST_CASE("Eventbus doesn't fire after removing eventhandler", "[eventtest]") {
  TestListener testListener;
  EventRegistration *eventReg;
  eventReg = EventBus::AddHandler<TestEvent>(testListener);

  std::string msg = "testmessage";
  TestEvent testEvent(msg);
  EventBus::FireEvent(testEvent);

  REQUIRE(testListener.message == msg);

  eventReg->RemoveHandler();

  std::string msg2 = "testmessage2";
  TestEvent testEvent2(msg2);
  EventBus::FireEvent(testEvent2);

  REQUIRE(testListener.message != msg2);
  REQUIRE(testListener.message == msg);
}