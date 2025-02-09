# Observer Pattern in C++
This repository implements the **Observer Pattern** in C++ with automatic memory management using `std::weak_ptr` and `std::shared_ptr`. It provides a flexible way to manage observers and subjects, including both typed and untyped (void) observers, while automatically cleaning up expired (dead) observers.

## Features
- **Observer Pattern**: Implements the classic observer pattern where subjects notify observers of updates.
- **Automatic Cleanup**: Uses `std::weak_ptr` to avoid memory leaks and ensures observers that have expired (i.e., the `shared_ptr` managing them is no longer valid) are automatically removed from the subject's list of observers.
- **Support for `void` Specialization**: Handles both typed observers (e.g., `Observer<int>`) and the special `Observer<void>`, where no data is passed to the observers.
- **Efficient Memory Management**: Observers are stored as weak pointers, so the subject won't extend the observer's lifetime unnecessarily. Expired observers are cleaned up automatically during each notification cycle.

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
- A specialization of `Observer<void>` and `Subject<void>` is provided for cases where no data needs to be passed to observers.

## Usage Example

### 1. Observer with no parameters

```cpp
#include "Observer.h"

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
```

### 2. Observer with parameters

```cpp
#include "Observer.h"

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
```

## Features

- **Memory Safe**: Automatically clears expired observers using weak pointers.
- **Flexible**: Can handle both typed and untyped observers.
- **Efficient**: Avoids duplicates in the observer list and removes expired references when notifying observers.

## Requirements

- C++11 or later

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
