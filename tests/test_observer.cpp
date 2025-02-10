#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include "Observer.h"

// Test case 1: Simple void notification
class TestSimpleObserver: public SimpleObserver {
	public:
	void update() override {
		callCount++;
	}
	int callCount = 0;
};

class TestSimpleSubject: public SimpleSubject {
	public:
	void notify() {
		notifyAll();
	}
};

// Test case 2: Observer with data
class WeatherObserver: public Observer<float, std::string> {
	public:
	void update(const float& temp, const std::string& condition) override {
		lastTemp = temp;
		lastCondition = condition;
	}
	float lastTemp = 0.0f;
	std::string lastCondition;
};

class WeatherStation: public Subject<float, std::string> {
	public:
	void updateWeather(float temp, const std::string& condition) {
		notifyAll(temp, condition);
	}
};

// Test 1: Simple notification
TEST(ObserverPatternTest, SimpleNotification) {
	auto subject = std::make_shared<TestSimpleSubject>();
	auto observer1 = std::make_shared<TestSimpleObserver>();
	auto observer2 = std::make_shared<TestSimpleObserver>();

	subject->registerObserver(observer1);
	subject->registerObserver(observer2);

	subject->notify();
	EXPECT_EQ(observer1->callCount, 1);
	EXPECT_EQ(observer2->callCount, 1);

	subject->removeObserver(observer1);
	subject->notify();
	EXPECT_EQ(observer1->callCount, 1);
	EXPECT_EQ(observer2->callCount, 2);
}

// Test 2: Observer with data
TEST(ObserverPatternTest, ObserverWithData) {
	auto station = std::make_shared<WeatherStation>();
	auto observer1 = std::make_shared<WeatherObserver>();
	auto observer2 = std::make_shared<WeatherObserver>();

	station->registerObserver(observer1);
	station->registerObserver(observer2);

	station->updateWeather(25.5f, "Sunny");
	EXPECT_EQ(observer1->lastTemp, 25.5f);
	EXPECT_EQ(observer1->lastCondition, "Sunny");
	EXPECT_EQ(observer2->lastTemp, 25.5f);
	EXPECT_EQ(observer2->lastCondition, "Sunny");
}

// Test 3: Automatic cleanup of expired observers
TEST(ObserverPatternTest, AutomaticCleanupOfExpiredObservers) {
	auto subject = std::make_shared<TestSimpleSubject>();
	{
		auto observer = std::make_shared<TestSimpleObserver>();
		subject->registerObserver(observer);
	}
	// This should not crash and indicate successful completion
	subject->notify();
}

// Main function to run all tests
int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
