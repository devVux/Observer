# Observer Pattern in C++

This repository implements the **Observer Pattern** in C++ with automatic memory management using `std::weak_ptr` and `std::shared_ptr`. It provides a flexible way to manage observers and subjects, including both typed and untyped (void) observers, while automatically cleaning up expired (dead) observers.

## Requirements
- C++11 or later

## Features
- **Memory Safe**: Automatically clears expired observers using weak pointers.
- **Flexible**: Can handle both typed and untyped observers.
- **Efficient**: Avoids duplicates in the observer list and removes expired references when notifying observers.

## How it Works

### AutoCleanup Class
- Manages a list of weak references (`std::weak_ptr`) to observers.
- Handles the addition and removal of observers while preventing duplicates.
- Provides an automatic cleanup of expired (dead) references to ensure no dangling pointers are accessed.

### Observer Class
- Defines the interface for observer objects. It has a pure virtual `update` method that is called when the subject notifies the observer.
- Supports both typed observers (e.g., `Observer<int>`) and the specialized `Observer<void>` for cases when no data needs to be passed.

### Subject Class
- Holds a list of observers and notifies them when an update occurs.
- Handles registering and removing observers and automatically cleans up expired references before sending updates.

### `void` Specialization
- A specialization of `Observer<void>` and `Subject<void>` (namely `SimpleObserver` and `SimpleSubject`) is provided for cases where no data needs to be passed to observers.

## Usage Example

### 1. **Observer with No Parameters**

This example demonstrates the most basic use case where the observer does not require any parameters for the `update()` method.

<details>
<summary>Click to expand code example</summary>

```cpp
#include "Observer.h"

class Model : public SimpleSubject {
	public:
		void applyChanges() {
			notifyAll();
		}
};

class View : public SimpleObserver {
	public:
		void update() override {
			std::cout << "View: updated\n";
		}
};

auto model = std::make_shared<Model>();
auto view = std::make_shared<View>();

model->registerObserver(view);
model->applyChanges();
```

#### Output:
```
View: updated
```

</details>

### 2. **Observer with One Parameter (Temperature)**

In this example, the observer reacts to a change in temperature, which is passed as an argument to the update method.

<details>
<summary>Click to expand code example</summary>

```cpp
#include "Observer.h"

class TemperatureSensor : public Subject<float> {
	public:
		void setTemperature(float temp) {
			notifyAll(temp);
		}
};

class Display : public Observer<float> {
	public:
		void update(const float& val) override {
			std::cout << "Temperature: " << val << "C\n";
		}
};

auto sensor = std::make_shared<TemperatureSensor>();
auto display = std::make_shared<Display>();

sensor->registerObserver(display);
sensor->setTemperature(25.0f);
```

#### Output:
```
Temperature: 25C
```

</details>

### 3. **Observer with Multiple Parameters (float, float, std::string)**

This example demonstrates the observer pattern with multiple parameters: temperature, humidity, and weather condition. 
The observer handles all these parameters in the update method.

<details>
<summary>Click to expand code example</summary>

```cpp
#include "Observer.h"

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
```

#### Output:
```
Weather Update: 
Temperature: 25.5C
Humidity: 60%
Condition: Sunny

Weather Update: 
Temperature: 18.3C
Humidity: 75%
Condition: Cloudy
```

</details>

### 4. **Observer with Struct Parameters (Sensor Data)**

This example demonstrates how to use the observer pattern with a more complex data type, such as a struct, to store multiple related properties like temperature and humidity.

<details>
<summary>Click to expand code example</summary>

```cpp
#include "Observer.h"

struct SensorData {
	float temperature;
	float humidity;
};

class Sensor : public Subject<SensorData> {
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

class Display : public Observer<SensorData> {
	public:
		void update(const SensorData& data) override {
			std::cout << "Temperature: " << data.temperature << "C\n";
			std::cout << "Humidity: " << data.humidity << "%\n";
		}
};

auto sensor = std::make_shared<Sensor>();
auto display = std::make_shared<Display>();

sensor->registerObserver(display);
sensor->setTemperature(420.0f);
sensor->setHumidity(69.0f);
```

#### Output:
```
Temperature: 420C
Humidity: 69%
```

</details>

---

## Conclusion

The Observer design pattern is an essential pattern for handling dynamic updates between objects in a loosely coupled way. With the examples provided, you can see how to:

1. Use simple observers without parameters.
2. Pass parameters to observers when needed.
3. Handle multiple parameters in the observer's `update` method.
4. Use complex data structures like structs for more organized data flow.

These examples should serve as a solid foundation for implementing the Observer pattern in C++ in various scenarios. Feel free to modify the code to suit your application's needs!

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
