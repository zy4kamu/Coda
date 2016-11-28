/**
 * @file	Parse.cpp
 * @brief	Implementation of the methods in the corresponding header
 */

#include "Parse.h"
#include <iostream>
#include <utility>

using std::shared_ptr;
using std::wstring;
using std::vector;
using std::wcout;
using std::endl;

namespace Phoenix
{
Parse::Parse(const vector<wstring>& frameLabels, const vector<shared_ptr<ChartCell>>& _slotSequence)
{
    frameSequence.assign(frameLabels.begin(), frameLabels.end());
    slotSequence.assign(_slotSequence.begin(), _slotSequence.end());
}

const vector<wstring>& Parse::GetFrameSequence() const
{
    return frameSequence;
}

const vector<shared_ptr<ChartCell>>& Parse::GetSlotSequence() const
{
    return slotSequence;
}

void Parse::Print(const vector<wstring>& utteranceTokens)
{
    wstring FrameName= L"";
    for(size_t i = 0; i < frameSequence.size(); i++)
    {
        if(frameSequence[i] != FrameName)
        {
            FrameName = frameSequence[i];
            wcout << FrameName << L":" << endl;
        }
        wstring slot = printSlotRecursively(slotSequence[i]->GetMatch(), utteranceTokens);
        wcout << slot << endl;
    }
}

wstring Parse::ToString(const vector<wstring>& utteranceTokens)
{
    wstring FrameName= L"";
    wstring toReturn;
    for(size_t i = 0; i < frameSequence.size(); i++)
    {
        if(frameSequence[i] != FrameName)
        {
            FrameName = frameSequence[i];
            toReturn.append(FrameName + L":\n");
            //wcout << FrameName << L":" << endl;
        }
        wstring slot = printSlotRecursively(slotSequence[i]->GetMatch(), utteranceTokens);
        toReturn.append(slot + L"\n");
    }

    return toReturn;
}

wstring Parse::ToStringConciseNF(const vector<wstring>& utteranceTokens)
{
    wstring FrameName= L"";
    wstring toReturn;
    for(size_t i = 0; i < frameSequence.size(); i++)
    {
        if(frameSequence[i] != FrameName)
        {
            FrameName = frameSequence[i];
            wcout << FrameName << L":" << endl;
        }
        wstring slot = printConciseRecursivelyNF(slotSequence[i]->GetMatch());

        if(slot[0] == L'(')
        {
            slot = slot.substr(1, slot.size() - 2);
        }

        toReturn.append(slot + L"\n");
    }

    return toReturn;
}

wstring Parse::printSlotRecursively(const shared_ptr<MatchTree>& match, const vector<wstring>& words)
{
    wstring slot = match->GetNet()->GetName();

    int p1 = match->GetStartPosition();
    int p2 = match->GetEndPosition();

    if( match->HasChildren())
    {
        slot.append(L"(");

        p2 = (*(match->GetChildMatchesIterator()))->GetStartPosition();

        for(int i = p1; i < p2; i++)
        {
            slot.append(words[i]);
            slot.append(L" ");
        }

        for(auto child_p = match->GetChildMatchesIterator(); child_p != match->GetChildMatchesEndIterator(); child_p++)
        {
            slot.append(printSlotRecursively(*child_p, words));
        }

        slot.append(L")");
    }
    else
    {
        slot.append(L"(");
        for(int i = p1; i < p2; i++)
        {
            slot.append(words[i]);
            slot.append(L" ");
        }
        slot.append(words[p2]);
        slot.append(L")");
    }

    return slot;
}

void Parse::PrintConcise(const vector<wstring>& utteranceTokens)
{
    wstring FrameName= L"";
    for(size_t i = 0; i < frameSequence.size(); i++)
    {
        if(frameSequence[i] != FrameName)
        {
            FrameName = frameSequence[i];
            wcout << FrameName << L":" << endl;
        }
        wstring slot = printConciseRecursively(slotSequence[i]->GetMatch(), utteranceTokens);

    if(slot[0] == L'(')
    {
        slot = slot.substr(1, slot.size() - 2);
    }

        wcout << slot << endl;
    }
}

wstring Parse::printConciseRecursively(const shared_ptr<MatchTree>& match, const vector<wstring>& words)
{
    wstring slot = match->GetNet()->GetConciseName();

    int p1 = match->GetStartPosition();
    int p2 = match->GetEndPosition();

    if( match->HasChildren())
    {
        p2 = (*(match->GetChildMatchesIterator()))->GetStartPosition();

        auto last_p = match->GetChildMatchesEndIterator();
        last_p--;

            for(auto child_p = match->GetChildMatchesIterator(); child_p != match->GetChildMatchesEndIterator(); child_p++)
        {
            if((*child_p)->GetNet()->GetType() != AUX)
            {
                slot.append(L"(");
                slot.append(printConciseRecursively(*child_p, words));
                slot.append(L")");
            }
            else
            {
                slot.append(printConciseRecursively(*child_p, words));
            }

            if(child_p != last_p)
            {
                slot.append(L" ");
            }
        }

        //if(match->GetNet()->GetType() != AUX)
        //{
        //	slot.append(L")");
        //}
    }
    else
    {
        if(match->GetNet()->IsConceptLeaf())
        {
            slot.append(L" ");
            for(int i = p1; i < p2; i++)
            {
                slot.append(words[i]);
                slot.append(L" ");
            }
            slot.append(words[p2]);
        }
    }

    return slot;
}

void Parse::PrintNF()
{
    wstring FrameName= L"";
    for(size_t i = 0; i < frameSequence.size(); i++)
    {
        if(frameSequence[i] != FrameName)
        {
            FrameName = frameSequence[i];
            wcout << FrameName << L":" << endl;
        }
        wstring slot = printSlotRecursivelyNF(slotSequence[i]->GetMatch());
        wcout << slot << endl;
    }
}

wstring Parse::printSlotRecursivelyNF(const shared_ptr<MatchTree>& match)
{
    wstring slot = match->GetNet()->GetName();
    int p2 = match->GetEndPosition();
    if( match->HasChildren())
    {
        slot.append(L"(");
        p2 = (*(match->GetChildMatchesIterator()))->GetStartPosition();

        for(size_t i = 0; i < match->GetMatchedWordEntries().size(); i++)
        {
            slot.append(match->GetMatchedWordEntries()[i]);
            slot.append(L" ");
        }

        for(auto child_p = match->GetChildMatchesIterator(); child_p != match->GetChildMatchesEndIterator(); child_p++)
        {
            slot.append(printSlotRecursivelyNF(*child_p));
        }

        slot.append(L")");
    }
    else
    {
        slot.append(L"(");
        for(size_t i = 0; i < match->GetMatchedWordEntries().size(); i++)
        {
            slot.append(match->GetMatchedWordEntries()[i]);
            slot.append(L" ");
        }
        slot.append(match->GetMatchedWordEntries()[p2]);
        slot.append(L")");
    }

    return slot;
}

void Parse::PrintConciseNF()
{
    wstring FrameName= L"";
    for(size_t i = 0; i < frameSequence.size(); i++)
    {
        if(frameSequence[i] != FrameName)
        {
            FrameName = frameSequence[i];
            wcout << FrameName << L":" << endl;
        }
        wstring slot = printConciseRecursivelyNF(slotSequence[i]->GetMatch());

    if(slot[0] == L'(')
    {
        slot = slot.substr(1, slot.size() - 2);
    }

        wcout << slot << endl;
    }
}

wstring Parse::printConciseRecursivelyNF(const shared_ptr<MatchTree>& match)
{
    wstring slot = match->GetNet()->GetConciseName();
    if( match->HasChildren())
    {
        //int p1 = match->GetStartPosition();
        //int p2 = (*(match->GetChildMatchesIterator()))->GetStartPosition();

        auto last_p = match->GetChildMatchesEndIterator();
        last_p--;

            for(auto child_p = match->GetChildMatchesIterator(); child_p != match->GetChildMatchesEndIterator(); child_p++)
        {
            if((*child_p)->GetNet()->GetType() != AUX)
            {
                slot.append(L"(");
                slot.append(printConciseRecursivelyNF(*child_p));
                slot.append(L")");
            }
            else
            {
                slot.append(printConciseRecursivelyNF(*child_p));
            }

            if(child_p != last_p)
            {
                slot.append(L" ");
            }
        }
    }
    else
    {
        if(match->GetNet()->IsConceptLeaf())
        {
            slot.append(L" ");
            for(size_t i = 0; i < match->GetMatchedWordEntries().size() - 1; i++)
            {
                slot.append(match->GetMatchedWordEntries()[i]);
                slot.append(L" ");
            }
            slot.append(match->GetMatchedWordEntries()[match->GetMatchedWordEntries().size() - 1]);
        }
    }

    return slot;
}

bool Parse::IsFragmented() const
{
    wstring fl = frameSequence[0];

    for(size_t i = 0; i < slotSequence.size(); i++)
    {
        if(frameSequence[i] != fl)
        {
            return true;
        }
    }

    return false;
}

vector<wstring> Parse::GetUnparsedSequence(const vector<wstring>& words) const
{
    vector<bool> positions;
    for(int i = 0; i < words.size(); i++)
    {
        positions.push_back(true);
    }

    for(auto slot_p = slotSequence.begin(); slot_p != slotSequence.end(); slot_p++)
    {
        int a = (*slot_p)->GetStartPosition();
        int b = (*slot_p)->GetEndPosition();

        for(int i = a; i <= b; i++)
        {
            positions[i] = false;
        }
    }

    vector<wstring> toReturn;

    for(int i = 0; i < words.size(); i++)
    {
        if(positions[i])
        toReturn.push_back(words[i]);
    }

    return toReturn;
}

} //namespace Phoenix
