#include <iostream>
#include "Observer.h"

void example1() {

	class Model: public SimpleSubject {
		public:
			void applyChanges() {
				notifyAll();
			}
	};

	class View: public simpleObserver {
		public:
			void update() override {
				std::cout << "View: updated\n";
			}
	};

	auto model = std::make_shared<Model>();
	auto view = std::make_shared<View>();

	model->registerObserver(view);
	model->applyChanges();


}


void example2() {
	
	class TemperatureSensor: public Subject<float> {
		public:
			void setTemperature(float temp) {
				notifyAll(temp);
			}
	};

	class Display: public Observer<float> {
		public:
			void update(const float& val) override {
				std::cout << "Temperature: " << val << "C\n";
			}
	};

	auto sensor = std::make_shared<TemperatureSensor>();
	auto display = std::make_shared<Display>();

	sensor->registerObserver(display);
	sensor->setTemperature(25.0f);
}

void example3() {
    class WeatherStation : public Subject<float, float, std::string> {
		public:
			void setWeather(float temperature, float humidity, const std::string& condition) {
				notifyAll(temperature, humidity, condition);
			}
    };

    class WeatherDisplay : public Observer<float, float, std::string> {
		public:
			void update(const float& temperature, const float& humidity, const std::string& condition) override {
				std::cout << "Weather Update: \n";
				std::cout << "Temperature: " << temperature << "C\n";
				std::cout << "Humidity: " << humidity << "%\n";
				std::cout << "Condition: " << condition << "\n";
			}
    };

    auto station = std::make_shared<WeatherStation>();
    auto display = std::make_shared<WeatherDisplay>();

    station->registerObserver(display);

    station->setWeather(25.5f, 60.0f, "Sunny");
    station->setWeather(18.3f, 75.0f, "Cloudy");
}


void example4() {
	struct SensorData {
		float temperature;
		float humidity;
	};

	class Sensor: public Subject<SensorData> {
		public:
			void setTemperature(float t) {
				temp = t;
				notifyAll({ temp, hum });
			}
			void setHumidity(float h) {
				hum = h;
				notifyAll({ temp, hum });
			}
		private:
			float temp { 0.0f };
			float hum { 0.0f };
	};

	class Display: public Observer<SensorData> {
		public:
			void update(const SensorData& data) override {
				std::cout << "Temperature: " << data.temperature << "C\n";
				std::cout << "Humidity: " << data.humidity << "C\n";
			}
	};

	auto sensor = std::make_shared<Sensor>();
	auto display = std::make_shared<Display>();

	sensor->registerObserver(display);
	sensor->setTemperature(420.0f);
	sensor->setHumidity(69.0f);
}

int main() {

	example1();
	std::cout << "\n";

	example2();
	std::cout << "\n";
	
	example3();
	std::cout << "\n";
	
	example4();
	std::cout << "\n";


}