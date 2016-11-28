/** 
 * @file    statcalc-enums.h
 * @brief	header file with enumeration types of others text domains
 */

#include <string>
#include <unordered_map>
#include "statcalc-enums.h"

using std::string;
using std::wstring;
using std::unordered_map;
using namespace statcalc;

enum WikiCategoryFull
statcalc::wikicategoryfullWstringToEnum(const wstring &category)
{
    if (MAP.count(category))
    {
        return (enum WikiCategoryFull)MAP[category];
    }
    else
    {
        return WCF_COUNT;
    }
}

enum DomainTypes
statcalc::domainStringToEnum(string &i_domain)
{
    if (i_domain == "TVCONTROL")
        return TVCONTROL;
    else if (i_domain == "CONTENTS_APP")
        return CONTENTS_APP;
    else if (i_domain == "CONTENTS_EPG")
        return CONTENTS_EPG;
    else if (i_domain == "CONTENTS_PVOD")
        return CONTENTS_PVOD;
    else if (i_domain == "SPORT")
        return SPORT;
    else if (i_domain == "TRAFFIC")
        return TRAFFIC;
    else if (i_domain == "RESTAURANT")
        return RESTAURANT;
    else if (i_domain == "WEATHER")
        return WEATHER;
    else if (i_domain == "STOCK")
        return STOCK;
    else
        return COUNT_DOMAIN_TYPES;

}

enum DomainTypes
statcalc::domainWstringToEnum(const wstring &i_domain)
{
    if (i_domain == L"TVCONTROL")
        return TVCONTROL;
    else if (i_domain == L"CONTENTS_APP")
        return CONTENTS_APP;
    else if (i_domain == L"CONTENTS_EPG")
        return CONTENTS_EPG;
    else if (i_domain == L"CONTENTS_PVOD")
        return CONTENTS_PVOD;
    else if (i_domain == L"SPORT")
        return SPORT;
    else if (i_domain == L"TRAFFIC")
        return TRAFFIC;
    else if (i_domain == L"RESTAURANT")
        return RESTAURANT;
    else if (i_domain == L"WEATHER")
        return WEATHER;
    else if (i_domain == L"STOCK")
        return STOCK;
    else
        return COUNT_DOMAIN_TYPES;

}

enum WikiCategory
statcalc::wikicategoryWstringToEnum(const wstring &category)
{
    if (category == L"Research")
        return WC_RESEARCH;
    else if (category == L"Library_science")
        return WC_LIBRARY_SCIENCE;
    else if (category == L"Culture")
        return WC_CULTURE;
    else if (category == L"Arts")
        return WC_ARTS;
    else if (category == L"Geography")
        return WC_GEOGRAPHY;
    else if (category == L"Places")
        return WC_PLACES;
    else if (category == L"Health")
        return WC_HEALTH;
    else if (category == L"Self_care")
        return WC_SELF_CARE;
    else if (category == L"Healthcare_occupations")
        return WC_HEALTHCARE_OCCUPATIONS;
    else if (category == L"History")
        return WC_HISTORY;
    else if (category == L"Events")
        return WC_EVENTS;
    else if (category == L"Mathematics")
        return WC_MATHEMATICS;
    else if (category == L"Abstraction")
        return WC_ABSTRACTION;
    else if (category == L"Science")
        return WC_SCIENCE;
    else if (category == L"Natural_sciences")
        return WC_NATURAL_SCIENCES;
    else if (category == L"Nature")
        return WC_NATURE;
    else if (category == L"People")
        return WC_PEOPLE;
    else if (category == L"Personal_life")
        return WC_PERSONAL_LIFE;
    else if (category == L"Self")
        return WC_SELF;
    else if (category == L"Philosophy")
        return WC_PHILOSOPHY;
    else if (category == L"Thought")
        return WC_THOUGHT;
    else if (category == L"Religion")
        return WC_RELIGION;
    else if (category == L"Belief")
        return WC_BELIEF;
    else if (category == L"Society")
        return WC_SOCIETY;
    else if (category == L"Social_sciences")
        return WC_SOCIAL_SCIENCES;
    else if (category == L"Technology")
        return WC_TECHNOLOGY;
    else if (category == L"Applied_sciences")
        return WC_APPLIED_SCIENCES;
    else
        return WC_COUNT;
}


wstring
statcalc::domainEnumToWstring(DomainTypes i_domain)
{
    if (i_domain == TVCONTROL)
        return L"TVCONTROL";
    else if (i_domain == CONTENTS_APP)
        return L"CONTENTS_APP";
    else if (i_domain == CONTENTS_EPG)
        return L"CONTENTS_EPG";
    else if (i_domain == CONTENTS_PVOD)
        return L"CONTENTS_PVOD";
    else if (i_domain == SPORT)
        return L"SPORT";
    else if (i_domain == TRAFFIC)
        return L"TRAFFIC";
    else if (i_domain == RESTAURANT)
        return L"RESTAURANT";
    else if (i_domain == WEATHER)
        return L"WEATHER";
    else if (i_domain == STOCK)
        return L"STOCK";
    else
        return L"Uknown domain";
}

enum GoogleCategoryShort
statcalc::googlecategoryshortWstringToEnum(const wstring &category)
{
    if (category == L"/store/apps/category/COMMUNICATION")
        return GCS_COMMUNICATION;
    else if (category == L"/store/apps/category/PHOTOGRAPHY")
        return GCS_PHOTOGRAPHY;
    else if (category == L"/store/apps/category/BOOKS_AND_REFERENCE")
        return GCS_BOOKS_AND_REFERENCE;
    else if (category == L"/store/apps/category/GAME_TRIVIA")
        return GCS_GAME;
    else if (category == L"/store/apps/category/SPORTS")
        return GCS_SPORTS;
    else if (category == L"/store/apps/category/GAME_EDUCATIONAL")
        return GCS_GAME;
    else if (category == L"/store/apps/category/EDUCATION")
        return GCS_EDUCATION;
    else if (category == L"/store/apps/category/MEDIA_AND_VIDEO")
        return GCS_MEDIA_AND_VIDEO;
    else if (category == L"/store/apps/category/WEATHER")
        return GCS_WEATHER;
    else if (category == L"/store/apps/category/COMICS")
        return GCS_COMICS;
    else if (category == L"#")
        return GCS_DASH;
    else if (category == L"/store/apps/category/GAME_RACING")
        return GCS_GAME;
    else if (category == L"/store/apps/category/SHOPPING")
        return GCS_SHOPPING;
    else if (category == L"/store/apps/category/ENTERTAINMENT")
        return GCS_ENTERTAINMENT;
    else if (category == L"/store/apps/category/TRAVEL_AND_LOCAL")
        return GCS_TRAVEL_AND_LOCAL;
    else if (category == L"/store/apps/category/GAME_ACTION")
        return GCS_GAME;
    else if (category == L"/store/apps/category/MUSIC_AND_AUDIO")
        return GCS_MUSIC_AND_AUDIO;
    else if (category == L"/store/apps/category/GAME_ROLE_PLAYING")
        return GCS_GAME;
    else if (category == L"/store/apps/category/FINANCE")
        return GCS_FINANCE;
    else if (category == L"/store/apps/category/GAME_FAMILY")
        return GCS_GAME;
    else if (category == L"/store/apps/category/GAME_CASINO")
        return GCS_GAME;
    else if (category == L"/store/apps/category/LIFESTYLE")
        return GCS_LIFESTYLE;
    else if (category == L"/store/apps/category/LIBRARIES_AND_DEMO")
        return GCS_LIBRARIES_AND_DEMO;
    else if (category == L"/store/apps/category/GAME_WORD")
        return GCS_GAME;
    else if (category == L"/store/apps/category/GAME_CASUAL")
        return GCS_GAME;
    else if (category == L"/store/apps/category/GAME_BOARD")
        return GCS_GAME;
    else if (category == L"/store/apps/category/GAME_SIMULATION")
        return GCS_GAME;
    else if (category == L"/store/apps/category/MEDICAL")
        return GCS_MEDICAL;
    else if (category == L"/store/apps/category/PERSONALIZATION")
        return GCS_PERSONALIZATION;
    else if (category == L"/store/apps/category/TOOLS")
        return GCS_TOOLS;
    else if (category == L"/store/apps/category/BUSINESS")
        return GCS_BUSINESS;
    else if (category == L"/store/apps/category/GAME_STRATEGY")
        return GCS_GAME;
    else if (category == L"/store/apps/category/GAME_ARCADE")
        return GCS_GAME;
    else if (category == L"/store/apps/category/HEALTH_AND_FITNESS")
        return GCS_HEALTH_AND_FITNESS;
    else if (category == L"/store/apps/category/PRODUCTIVITY")
        return GCS_PRODUCTIVITY;
    else if (category == L"/store/apps/category/GAME_PUZZLE")
        return GCS_GAME;
    else if (category == L"/store/apps/category/GAME_SPORTS")
        return GCS_GAME;
    else if (category == L"/store/apps/category/TRANSPORTATION")
        return GCS_TRANSPORTATION;
    else if (category == L"/store/apps/category/NEWS_AND_MAGAZINES")
        return GCS_NEWS_AND_MAGAZINES;
    else if (category == L"/store/apps/category/GAME_MUSIC")
        return GCS_GAME;
    else if (category == L"/store/apps/category/GAME_ADVENTURE")
        return GCS_GAME;
    else if (category == L"/store/apps/category/GAME_CARD")
        return GCS_GAME;
    else if (category == L"/store/apps/category/SOCIAL")
        return GCS_SOCIAL;
    else
        return GCS_COUNT;
}


enum GoogleCategory
statcalc::googlecategoryWstringToEnum(const wstring &category)
{
    if (category == L"/store/apps/category/COMMUNICATION")
        return GC_COMMUNICATION;
    else if (category == L"/store/apps/category/PHOTOGRAPHY")
        return GC_PHOTOGRAPHY;
    else if (category == L"/store/apps/category/BOOKS_AND_REFERENCE")
        return GC_BOOKS_AND_REFERENCE;
    else if (category == L"/store/apps/category/GAME_TRIVIA")
        return GC_GAME_TRIVIA;
    else if (category == L"/store/apps/category/SPORTS")
        return GC_SPORTS;
    else if (category == L"/store/apps/category/GAME_EDUCATIONAL")
        return GC_GAME_EDUCATIONAL;
    else if (category == L"/store/apps/category/EDUCATION")
        return GC_EDUCATION;
    else if (category == L"/store/apps/category/MEDIA_AND_VIDEO")
        return GC_MEDIA_AND_VIDEO;
    else if (category == L"/store/apps/category/WEATHER")
        return GC_WEATHER;
    else if (category == L"/store/apps/category/COMICS")
        return GC_COMICS;
    else if (category == L"#")
        return GC_DASH;
    else if (category == L"/store/apps/category/GAME_RACING")
        return GC_GAME_RACING;
    else if (category == L"/store/apps/category/SHOPPING")
        return GC_SHOPPING;
    else if (category == L"/store/apps/category/ENTERTAINMENT")
        return GC_ENTERTAINMENT;
    else if (category == L"/store/apps/category/TRAVEL_AND_LOCAL")
        return GC_TRAVEL_AND_LOCAL;
    else if (category == L"/store/apps/category/GAME_ACTION")
        return GC_GAME_ACTION;
    else if (category == L"/store/apps/category/MUSIC_AND_AUDIO")
        return GC_MUSIC_AND_AUDIO;
    else if (category == L"/store/apps/category/GAME_ROLE_PLAYING")
        return GC_GAME_ROLE_PLAYING;
    else if (category == L"/store/apps/category/FINANCE")
        return GC_FINANCE;
    else if (category == L"/store/apps/category/GAME_FAMILY")
        return GC_GAME_FAMILY;
    else if (category == L"/store/apps/category/GAME_CASINO")
        return GC_GAME_CASINO;
    else if (category == L"/store/apps/category/LIFESTYLE")
        return GC_LIFESTYLE;
    else if (category == L"/store/apps/category/LIBRARIES_AND_DEMO")
        return GC_LIBRARIES_AND_DEMO;
    else if (category == L"/store/apps/category/GAME_WORD")
        return GC_GAME_WORD;
    else if (category == L"/store/apps/category/GAME_CASUAL")
        return GC_GAME_CASUAL;
    else if (category == L"/store/apps/category/GAME_BOARD")
        return GC_GAME_BOARD;
    else if (category == L"/store/apps/category/GAME_SIMULATION")
        return GC_GAME_SIMULATION;
    else if (category == L"/store/apps/category/MEDICAL")
        return GC_MEDICAL;
    else if (category == L"/store/apps/category/PERSONALIZATION")
        return GC_PERSONALIZATION;
    else if (category == L"/store/apps/category/TOOLS")
        return GC_TOOLS;
    else if (category == L"/store/apps/category/BUSINESS")
        return GC_BUSINESS;
    else if (category == L"/store/apps/category/GAME_STRATEGY")
        return GC_GAME_STRATEGY;
    else if (category == L"/store/apps/category/GAME_ARCADE")
        return GC_GAME_ARCADE;
    else if (category == L"/store/apps/category/HEALTH_AND_FITNESS")
        return GC_HEALTH_AND_FITNESS;
    else if (category == L"/store/apps/category/PRODUCTIVITY")
        return GC_PRODUCTIVITY;
    else if (category == L"/store/apps/category/GAME_PUZZLE")
        return GC_GAME_PUZZLE;
    else if (category == L"/store/apps/category/GAME_SPORTS")
        return GC_GAME_SPORTS;
    else if (category == L"/store/apps/category/TRANSPORTATION")
        return GC_TRANSPORTATION;
    else if (category == L"/store/apps/category/NEWS_AND_MAGAZINES")
        return GC_NEWS_AND_MAGAZINES;
    else if (category == L"/store/apps/category/GAME_MUSIC")
        return GC_GAME_MUSIC;
    else if (category == L"/store/apps/category/GAME_ADVENTURE")
        return GC_GAME_ADVENTURE;
    else if (category == L"/store/apps/category/GAME_CARD")
        return GC_GAME_CARD;
    else if (category == L"/store/apps/category/SOCIAL")
        return GC_SOCIAL;
    else
        return GC_COUNT;
}

vector<int> statcalc::patentLabeltoVec(const wstring& category)
{
    string str;
    Tools::ConvertWstringToString(category,str);
    Tools::DocumentPlain doc;
    doc.load(str);
    vector<int> label_vec;
    vector<wstring> splitted;
    for (size_t i = 0;i < doc.content().size(); i++)
    {
        splitted = Tools::Split(doc.content()[i],L"|",true);
        for (size_t j = 0; j < splitted.size(); j++)
        {
            label_vec.push_back(std::stoi(splitted[j]));
        }
    }
    std::sort(label_vec.begin(),label_vec.end());
    std::vector<int>::iterator it;
    it = std::unique(label_vec.begin(),label_vec.end());
    label_vec.resize( std::distance(label_vec.begin(),it) );
    return label_vec;
}

Tools::TrieDictionary<vector<wstring>, int> statcalc::patentLabeltoTrie(const wstring& category)
{
    string str;
    Tools::ConvertWstringToString(category,str);
    Tools::DocumentPlain doc;
    doc.load(str);
    Tools::TrieDictionary<vector<wstring>, int> trie(0);
    for (auto i = 0;i < doc.content().size(); i++)
    {
        trie[Tools::Split(doc.content()[i],L"|")] = 1;
    }
    return trie;
}
