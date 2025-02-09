#include <iostream>
#include "Observer.h"

int main() {

    struct Temperature {
        float value;
    };
    
    class TemperatureSensor : public Subject<Temperature> {
    public:
        void setTemperature(float temp) {
            notifyAll(Temperature{temp});
        }
    };
    
    class Display : public Observer<Temperature> {
    public:
        void update(const Temperature& data) override {
            std::cout << "Temperature: " << data.value << "°C\n";
        }
    };
    
    auto sensor = std::make_shared<TemperatureSensor>();
    auto display = std::make_shared<Display>();
    
    sensor->registerObserver(display);
    sensor->setTemperature(25.0f);


}