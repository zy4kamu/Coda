/** 
 * @file	Frame.cpp
 * @brief	Implementation of the methods in the corresponding header
 */

#include "Frame.h"

using std::shared_ptr;
using std::vector;
using std::wstring;

namespace Phoenix {

Frame::Frame(wstring _frameName)
{
	frameName = _frameName;
}

int Frame::GetNumberOfSlots() const
{
	return slots.size();
}

bool Frame::DoesHaveSlot(const shared_ptr<GrammarNet>& slot) const
{
	for(vector<shared_ptr<GrammarNet>>::const_iterator np = slots.begin(); np != slots.end(); np++)
	{
		if(slot == *np)
		{
			return true;
		}
	}
	return false;
}

const wstring& Frame::GetFrameName() const
{
	return frameName;
}

void Frame::AddSlot(const shared_ptr<Phoenix::GrammarNet>& net, bool isTenseSlot=false)
{
	if(isTenseSlot)
	{
		tenseSlotName = net->GetName();
	}
	slots.push_back(net);
}

vector<shared_ptr<GrammarNet>>::const_iterator Frame::GetNetIterator() const
{
	return slots.begin();
}

vector<shared_ptr<GrammarNet>>::const_iterator Frame::GetNetEndIterator() const
{
	return slots.end();
}

std::wstring Frame::GetTenseSlotName() const
{
	return tenseSlotName;
}

}
