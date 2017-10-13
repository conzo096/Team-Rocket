#pragma once
#pragma once

#include <memory>

// Singleton is so boiler plate we can define a template
// for it.

template<typename T>
class Singleton
{
public:
	// This call needs to be inline and static
	inline static T& Get()
	{
		// This *guarantees* that instance is only
		// created once.  If your constructor is not
		// empty this template won't work.
	//	static T instance;
	//	return instance;

		// Static memory is not good if you have big objects.
		// Consider using a static unique pointer.  Code commented
		// out below.
		 static std::unique_ptr<T> instance(new T());
		// Dereference the pointer to get the data.
		 return *instance.get();
	}
};