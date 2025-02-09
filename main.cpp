#include <iostream>
#include "Observer.h"

void simpleExample() {

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


void complexExample() {
	struct Temperature {
		float value;
	};

	class TemperatureSensor: public Subject<Temperature> {
		public:
			void setTemperature(float temp) {
				notifyAll(Temperature { temp });
			}
	};

	class Display: public Observer<Temperature> {
		public:
			void update(const Temperature& data) override {
				std::cout << "Temperature: " << data.value << "C\n";
			}
	};

	auto sensor = std::make_shared<TemperatureSensor>();
	auto display = std::make_shared<Display>();

	sensor->registerObserver(display);
	sensor->setTemperature(25.0f);
}

int main() {

	simpleExample();

	complexExample();

}