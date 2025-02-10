#pragma once

#include <memory>
#include <vector>
#include <algorithm>

template <class T, class... U> class Subject;
template <class T, class... U> class Observer;

template <class T, class... U>
class Observer {

	public:

		virtual ~Observer() = default;

		virtual void update(const T&, const U&...) = 0;

};

template <class T, class... U>
class Subject: public std::enable_shared_from_this<Subject<T, U...>> {
	friend class Subject<void>;

	using _Observer = Observer<T, U...>;
	using _WeakObserver = std::weak_ptr<_Observer>;
	using _SharedObserver = std::shared_ptr<_Observer>;


	public:

		void registerObserver(std::shared_ptr<Observer<T, U...>> observer) {
			mObservers.addReference(observer);
		}

		void removeObserver(std::shared_ptr<Observer<T, U...>> observer) {
			mObservers.removeReference(observer);
		}


	protected:

		virtual void notifyAll(const T& data, const U&... other) {
			mObservers.clearExpired();
			mObservers.callOnValidRefs(&_Observer::update, data, other...);
		}


	private:

		template <class _T>
		struct AutoCleanup {
			
			using _WrappedType = std::weak_ptr<_T>;

			public:

				bool addReference(_WrappedType ref) {
					if (auto target = ref.lock()) {
						auto it = std::find_if(mReferences.begin(), mReferences.end(), [&target](const auto& weakRef) {
							auto ptr = weakRef.lock();
							return ptr && ptr == target;
											   });

						if (it == mReferences.end()) {
							mReferences.push_back(target);
							return true;
						}
					}

					return false;
				}

				bool removeReference(_WrappedType object) {
					if (auto targetObject = object.lock()) {
						const std::size_t oldSize = mReferences.size();
						mReferences.erase(
							std::remove_if(mReferences.begin(), mReferences.end(), [&targetObject](const auto& weakObject) {
								auto ptr = weakObject.lock();
								return !ptr || ptr == targetObject;
										   }),
							mReferences.end()
						);

						return mReferences.size() < oldSize;
					}

					return false;
				}

				void clearExpired() {
					mReferences.erase(
						std::remove_if(mReferences.begin(), mReferences.end(), [](const auto& weakObs) {
							return weakObs.expired();
									   }),
						mReferences.end()
					);
				}


				template <class F, class... Args>
				void callOnValidRefs(F&& func, Args&&... args) {
					for (const auto& weakRef : mReferences)
						if (auto ref = weakRef.lock())
							(ref.get()->*func)(std::forward<Args>(args)...);
				}


				auto begin() { return mReferences.begin(); }
				auto end() { return mReferences.end(); }
				auto begin() const { return mReferences.begin(); }
				auto end() const { return mReferences.end(); }


			private:

				std::vector<_WrappedType> mReferences;

		};


	public:
		
		AutoCleanup<_Observer> mObservers;

};

template <>
class Observer<void> {

	public:

		virtual ~Observer() = default;

		virtual void update() = 0;

};


template <>
class Subject<void>: public std::enable_shared_from_this<Subject<void>> {
	using _Observer = Observer<void>;

	public:

		void registerObserver(std::shared_ptr<Observer<void>> observer) {
			mObservers.addReference(observer);
		}

		void removeObserver(std::shared_ptr<Observer<void>> observer) {
			mObservers.removeReference(observer);
		}


	protected:

		virtual void notifyAll() {
			mObservers.clearExpired();
			mObservers.callOnValidRefs(&Observer<void>::update);
		}


	protected:

		// Dummy template argument to access AutoCleanup
		Subject<int>::AutoCleanup<_Observer> mObservers;

};


using SimpleSubject = Subject<void>;
using SimpleObserver = Observer<void>;
