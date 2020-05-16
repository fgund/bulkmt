#ifndef IOBSERVABLE_H
#define IOBSERVABLE_H

#include <vector>
#include "IObserver.h"

template<typename T>
struct IObservable
{
	IObservable() = default;
	virtual void AddObserver(std::shared_ptr<IObserver<T> > observer) {
		observers.push_back(observer);
	}
	void RemoveObserver(std::shared_ptr<IObserver<T> > observer) {
		auto iter = std::find(observers.begin(), observers.end(), observer);
        if(iter != std::end(observers)) {
            observers.erase(iter);
        }
	}
	void Notify(T result) {
		for (auto& o : observers) {
			o->Update(result);
		}
	}
private:
	std::vector<std::shared_ptr<IObserver<T> > > observers;
};


#endif //IOBSERVABLE_H
