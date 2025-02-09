#pragma once

#include <memory>
#include <vector>

template <class T> class Subject;
template <class T> class Observer;

template <class T>
class Observer: public std::enable_shared_from_this<Observer<T>> {
	friend class Subject<T>;

	public:

		virtual ~Observer() = default;

		virtual void update(const T& data) = 0;


	private:

		void addSubject(std::weak_ptr<Subject<T>> subject) {
			mSubjects.push_back(subject);
		}

		void removeSubject(std::weak_ptr<Subject<T>> subject) {
			if (auto targetSubject = subject.lock()) {
				mSubjects.erase(
					std::remove_if(mSubjects.begin(), mSubjects.end(), [&targetSubject](const auto& weakSubject) {
						auto ptr = weakSubject.lock();
						return !ptr || ptr == targetSubject;
					}),
					mSubjects.end()
				);
			}
		}


	private:

		// Track subjects this observer is registered to for automatic cleanup
		std::vector<std::weak_ptr<Subject<T>>> mSubjects;

};

template <class T>
class Subject : public std::enable_shared_from_this<Subject<T>> {


	public:

		virtual ~Subject() {
			// Notify observers that this subject is being destroyed
			for (const auto& weakObs : mObservers)
				if (auto observer = weakObs.lock())
					observer->removeSubject(this->shared_from_this());
		}

		void registerObserver(std::shared_ptr<Observer<T>> observer) {
			if (!observer)
				return;

			auto it = std::find_if(mObservers.begin(), mObservers.end(), [&observer](const auto& weakObs) {
				auto ptr = weakObs.lock();
				return ptr && ptr == observer;
			});

			if (it == mObservers.end()) {
				mObservers.push_back(observer);
				observer->addSubject(this->shared_from_this());
			}
		}

		void removeObserver(std::shared_ptr<Observer<T>> observer) {
			if (!observer)
				return;

			mObservers.erase(
				std::remove_if(mObservers.begin(), mObservers.end(), [&observer](const auto& weakObs) {
					auto ptr = weakObs.lock();
					return !ptr || ptr == observer;
				}),
				mObservers.end()
			);
			observer->removeSubject(this->shared_from_this());
		}


	protected:

		virtual void notifyAll(const T& data) {
			mObservers.erase(
				std::remove_if(mObservers.begin(), mObservers.end(), [](const auto& weakObs) {
					return weakObs.expired();
				}),
				mObservers.end()
			);

			for (const auto& weakObs : mObservers)
				if (auto observer = weakObs.lock())
					observer->update(data);
		}


	protected:

		std::vector<std::weak_ptr<Observer<T>>> mObservers;

};






template <>
class Observer<void>: public std::enable_shared_from_this<Observer<void>> {
	friend class Subject<void>;

	public:

		virtual ~Observer() = default;

		virtual void update() = 0;


	private:

		void addSubject(std::weak_ptr<Subject<void>> subject) {
			mSubjects.push_back(subject);
		}

		void removeSubject(std::weak_ptr<Subject<void>> subject) {
			if (auto targetSubject = subject.lock()) {
				mSubjects.erase(
					std::remove_if(mSubjects.begin(), mSubjects.end(), [&targetSubject](const auto& weakSubject) {
						auto ptr = weakSubject.lock();
						return !ptr || ptr == targetSubject;
					}),
					mSubjects.end()
				);
			}
		}


	private:

		// Track subjects this observer is registered to for automatic cleanup
		std::vector<std::weak_ptr<Subject<void>>> mSubjects;

};


template <>
class Subject<void>: public std::enable_shared_from_this<Subject<void>> {

	public:

		virtual ~Subject() {
			// Notify observers that this subject is being destroyed
			for (const auto& weakObs : mObservers)
				if (auto observer = weakObs.lock())
					observer->removeSubject(this->shared_from_this());
		}

		void registerObserver(std::shared_ptr<Observer<void>> observer) {
			if (!observer)
				return;

			auto it = std::find_if(mObservers.begin(), mObservers.end(), [&observer](const auto& weakObs) {
				auto ptr = weakObs.lock();
				return ptr && ptr == observer;
			});

			if (it == mObservers.end()) {
				mObservers.push_back(observer);
				observer->addSubject(this->shared_from_this());
			}
		}

		void removeObserver(std::shared_ptr<Observer<void>> observer) {
			if (!observer)
				return;

			mObservers.erase(
				std::remove_if(mObservers.begin(), mObservers.end(), [&observer](const auto& weakObs) {
					auto ptr = weakObs.lock();
					return !ptr || ptr == observer;
				}),
				mObservers.end()
			);
			observer->removeSubject(this->shared_from_this());
		}


	protected:

		virtual void notifyAll() {
			mObservers.erase(
				std::remove_if(mObservers.begin(), mObservers.end(), [](const auto& weakObs) {
					return weakObs.expired();
				}),
				mObservers.end()
			);

			for (const auto& weakObs : mObservers)
				if (auto observer = weakObs.lock())
					observer->update();
		}


	protected:

		std::vector<std::weak_ptr<Observer<void>>> mObservers;

};


using SubjectNoParameters = Subject<void>;
using ObserverNoParameters = Observer<void>;
