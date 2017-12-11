#include "Resource.h"
#include "Targetable.h"
// This resource takes x amount from the resource. Is there parameters that need to be considered?
int Resource::RetrieveResource()
{
	// Unit takes 250 from me.
	int takeAmount = 250;
	value -= 250;
	mut.lock();
	std::cout << "Retrieving resource. " << value << std::endl;
	std::cout << "Pretending to reduce resource." << std::endl;
	if (value < 0)
	{
		takeAmount = takeAmount + value;
		value = 0;
		// if it false delete - even better give it a targetable component.
		GetParent()->GetComponent<Targetable>().SetHealth(0);
		GetParent()->GetComponent<Targetable>().Update(0);
	}
	mut.unlock();
	return takeAmount;
}