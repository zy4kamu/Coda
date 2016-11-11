#include "CommandInterpreter.h"
#include <memory>
#include <iostream>

using std::shared_ptr;
using std::vector;
using std::wstring;

namespace Phoenix
{

//for each possible command we provide processing function
CommandInterpreter::CommandInterpreter()
{
	functions[L"[Mute_On]"] = &CommandInterpreter::muteOn;
	functions[L"[Mute_Off]"] = &CommandInterpreter::muteOff;
	functions[L"[Set_Volume]"] = &CommandInterpreter::setVolume;
	functions[L"[Volume_Up]"] = &CommandInterpreter::volumeUp;
	functions[L"[Volume_Down]"] = &CommandInterpreter::volumeDown;
	functions[L"[Set_Channel]"] = &CommandInterpreter::setChannel;
	functions[L"[Channel_Up]"] = &CommandInterpreter::channelUp;
	functions[L"[Channel_Down]"] = &CommandInterpreter::channelDown;
	functions[L"[Power_Off]"] = &CommandInterpreter::poweroff;
}

void CommandInterpreter::Execute(const FilledFrame& targetFrame)
{
	normalizer.ClearStr();

	if(targetFrame.GetSemanticFrame()->GetFrameName() != L"RusTVControl")
	{
		throw ENoCommandFrame(targetFrame.GetSemanticFrame()->GetFrameName().c_str());
	}

	shared_ptr<MatchTree> comType;

	for(auto slot_p = targetFrame.GetSlotIterator(); slot_p != targetFrame.GetSlotEndIterator(); slot_p++)
	{
		if((*slot_p)->GetNetName() != L"[TVControl]")
		{
			throw ENoCommandFrame(targetFrame.GetSemanticFrame()->GetFrameName().c_str());
		}

		comType = *(std::dynamic_pointer_cast<ChartCell>(*slot_p)->GetMatch()->GetChildMatchesIterator());

		if(comType->GetNet()->GetName() != L"[Command_Type]")
		{
			throw ENoCommandFrame(targetFrame.GetSemanticFrame()->GetFrameName().c_str());
		}

		break;
	}

	shared_ptr<MatchTree> command = *(comType->GetChildMatchesIterator());

	if(functions.find(command->GetNet()->GetName()) != functions.end())
	{
        std::wcout << (this->*(functions[command->GetNet()->GetName()]))(command) << std::endl;
	}
	else
	{
		throw ENoCommandFrame(targetFrame.GetSemanticFrame()->GetFrameName().c_str());
	}

}

wstring CommandInterpreter::muteOn(shared_ptr<MatchTree> command)
{
	return L"Mute on";
}

wstring CommandInterpreter::muteOff(shared_ptr<MatchTree> command)
{
	return L"Mute off";
}

wstring CommandInterpreter::setVolume(shared_ptr<MatchTree> command)
{
	shared_ptr<MatchTree> newLevel = getNumberArg(command);

	vector<wstring> words = newLevel->GetMatchedWordEntries();

	wstring timeexp = L"";

	for(auto word_p = words.begin(); word_p != words.end(); word_p++)
	{
		timeexp = timeexp + *word_p + L" ";
	}

	return L"Volume level is set to: " + normalizer.NormalizeStr(timeexp);
}

wstring CommandInterpreter::volumeUp(shared_ptr<MatchTree> command)
{
	shared_ptr<MatchTree> newLevel = getNumberArg(command);

	vector<wstring> words = newLevel->GetMatchedWordEntries();

	wstring timeexp = L"";

	for(auto word_p = words.begin(); word_p != words.end(); word_p++)
	{
		timeexp = timeexp + *word_p + L" ";
	}

	return L"Volume up " + normalizer.NormalizeStr(timeexp) + L" points";
}

wstring CommandInterpreter::volumeDown(shared_ptr<MatchTree> command)
{
	shared_ptr<MatchTree> newLevel = getNumberArg(command);

	vector<wstring> words = newLevel->GetMatchedWordEntries();

	wstring timeexp = L"";

	for(auto word_p = words.begin(); word_p != words.end(); word_p++)
	{
		timeexp = timeexp + *word_p + L" ";
	}

	return L"Volume down by " + normalizer.NormalizeStr(timeexp) + L" points";
}

wstring CommandInterpreter::setChannel(shared_ptr<MatchTree> command)
{
	shared_ptr<MatchTree> newLevel = getNumberArg(command);

	vector<wstring> words = newLevel->GetMatchedWordEntries();

	wstring timeexp = L"";

	for(auto word_p = words.begin(); word_p != words.end(); word_p++)
	{
		timeexp = timeexp + *word_p + L" ";
	}

	return L"Turn to channel " + normalizer.NormalizeStr(timeexp);
}

wstring CommandInterpreter::channelUp(shared_ptr<MatchTree> command)
{
	shared_ptr<MatchTree> newLevel = getNumberArg(command);

	vector<wstring> words = newLevel->GetMatchedWordEntries();

	wstring timeexp = L"";

	for(auto word_p = words.begin(); word_p != words.end(); word_p++)
	{
		timeexp = timeexp + *word_p + L" ";
	}

	return L"Turn to " + normalizer.NormalizeStr(timeexp) + L"channels up";
}

wstring CommandInterpreter::channelDown(shared_ptr<MatchTree> command)
{
	shared_ptr<MatchTree> newLevel = getNumberArg(command);

	vector<wstring> words = newLevel->GetMatchedWordEntries();

	wstring timeexp = L"";

	for(auto word_p = words.begin(); word_p != words.end(); word_p++)
	{
		timeexp = timeexp + *word_p + L" ";
	}

	return L"Turn to " + normalizer.NormalizeStr(timeexp) + L" channels down";
}

wstring CommandInterpreter::poweroff(shared_ptr<MatchTree> command)
{
	return L"Poweroff";
}

shared_ptr<MatchTree> CommandInterpreter::getNumberArg(shared_ptr<MatchTree> cmd)
{
	for(auto net_p = cmd->GetChildMatchesIterator(); net_p != cmd->GetChildMatchesEndIterator(); net_p++)
	{
		if((*net_p)->GetNet()->GetName() == L"[Number]")
		{
			return *net_p;
		}
	}

	return shared_ptr<MatchTree>();
}

} //namespace Phoenix
