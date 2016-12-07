/** 
 * @file	FilledFrame.cpp
 * @brief	FilledFrame class implementation
 */

#include "FilledFrame.h"
#include "tagsetConstants.h"

using std::shared_ptr;
using std::vector;

namespace Phoenix
{

FilledFrame::FilledFrame(const shared_ptr<Frame>& semantic_frame, const vector<shared_ptr<IMatch>>& all_slots)
{
	frame = semantic_frame;
	slots = all_slots;

	tense = PRESENT;

	for(auto slot_p = all_slots.begin(); slot_p != all_slots.end(); slot_p++)
	{
		if(!(*slot_p)->IsExternal())
		{
			shared_ptr<MatchTree> slot = std::dynamic_pointer_cast<ChartCell>((*slot_p))->GetMatch();
			if(slot->GetNet()->GetName() == semantic_frame->GetTenseSlotName())
			{
				std::unordered_map<std::wstring, std::wstring> feats = slot->GetFeatures();
				if(feats.find(PrivateTagset::CAT_TIME) == feats.end())
				{
					tense = PRESENT;
				}
				else if(feats[PrivateTagset::CAT_TIME] == SynTagRusTagset::TIME_PAST)
				{
					tense = PAST;
				}
				else if(feats[PrivateTagset::CAT_TIME] == SynTagRusTagset::TIME_FUTURE)
				{
					tense = FUTURE;
				}
			}
		}
	}
}

shared_ptr<Frame> FilledFrame::GetSemanticFrame() const
{
	return frame;
}

vector<shared_ptr<IMatch>>::const_iterator FilledFrame::GetSlotIterator() const
{
	return slots.begin();
}

vector<shared_ptr<IMatch>>::const_iterator FilledFrame::GetSlotEndIterator() const
{
	return slots.end();
}

Tense FilledFrame::GetTense() const
{
	return tense;
}

} //namespace Phoenix

