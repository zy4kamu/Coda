/** 
 * @file    statcalc-enums.h
 * @brief	header file with enumeration types of others text domains
 *
 * .. invisible:
 *     _   _ _____ _     _____ _____
 *
 *    | | | |  ___| |   |  ___/  ___|
 *
 *    | | | | |__ | |   | |__ \ `--.
 *
 *    | | | |  __|| |   |  __| `--. \
 *
 *    \ \_/ / |___| |___| |___/\__/ /
 *
 *     \___/\____/\_____|____/\____/
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License
 *
 */


#ifndef _ENUMS_
#define _ENUMS_

#include <string>
#include <unordered_map>
#include "Tools.h"

using std::string;
using std::wstring;
using std::unordered_map;

namespace statcalc
{
/**
* Valid domains
*/
enum DomainTypes
{
   CONTENTS_ALL = 0,
   CONTENTS_PVOD,
   CONTENTS_EPG,
   CONTENTS_APP,
   TVCONTROL,
   WEATHER,
   STOCK,
   TRAFFIC,
   RESTAURANT,
   SPORT,
   WEBSEARCH,
   COMMON,
   TALK,
   COUNT_DOMAIN_TYPES
};

/**
* Valid domains
*/
enum GoogleCategory
{
    GC_COMMUNICATION = 0,
    GC_PHOTOGRAPHY,
    GC_BOOKS_AND_REFERENCE,
    GC_GAME_TRIVIA,
    GC_SPORTS,
    GC_GAME_EDUCATIONAL,
    GC_EDUCATION,
    GC_MEDIA_AND_VIDEO,
    GC_WEATHER,
    GC_COMICS,
    GC_DASH,
    GC_GAME_RACING,
    GC_SHOPPING,
    GC_ENTERTAINMENT,
    GC_TRAVEL_AND_LOCAL,
    GC_GAME_ACTION,
    GC_MUSIC_AND_AUDIO,
    GC_GAME_ROLE_PLAYING,
    GC_FINANCE,
    GC_GAME_FAMILY,
    GC_GAME_CASINO,
    GC_LIFESTYLE,
    GC_LIBRARIES_AND_DEMO,
    GC_GAME_WORD,
    GC_GAME_CASUAL,
    GC_GAME_BOARD,
    GC_GAME_SIMULATION,
    GC_MEDICAL,
    GC_PERSONALIZATION,
    GC_TOOLS,
    GC_BUSINESS,
    GC_GAME_STRATEGY,
    GC_GAME_ARCADE,
    GC_HEALTH_AND_FITNESS,
    GC_PRODUCTIVITY,
    GC_GAME_PUZZLE,
    GC_GAME_SPORTS,
    GC_TRANSPORTATION,
    GC_NEWS_AND_MAGAZINES,
    GC_GAME_MUSIC,
    GC_GAME_ADVENTURE,
    GC_GAME_CARD,
    GC_SOCIAL,
    GC_COUNT
};

/**
* Valid domains
*/
enum GoogleCategoryShort
{
    GCS_GAME = 0,
    GCS_COMMUNICATION,
    GCS_PHOTOGRAPHY,
    GCS_BOOKS_AND_REFERENCE,
    GCS_SPORTS,
    GCS_EDUCATION,
    GCS_MEDIA_AND_VIDEO,
    GCS_WEATHER,
    GCS_COMICS,
    GCS_DASH,
    GCS_SHOPPING,
    GCS_ENTERTAINMENT,
    GCS_TRAVEL_AND_LOCAL,
    GCS_MUSIC_AND_AUDIO,
    GCS_FINANCE,
    GCS_LIFESTYLE,
    GCS_LIBRARIES_AND_DEMO,
    GCS_GAME_SIMULATION,
    GCS_MEDICAL,
    GCS_PERSONALIZATION,
    GCS_TOOLS,
    GCS_BUSINESS,
    GCS_HEALTH_AND_FITNESS,
    GCS_PRODUCTIVITY,
    GCS_TRANSPORTATION,
    GCS_NEWS_AND_MAGAZINES,
    GCS_SOCIAL,
    GCS_COUNT
};

enum WikiCategory
{
    WC_RESEARCH = 0,
    WC_LIBRARY_SCIENCE,
    WC_CULTURE,
    WC_ARTS,
    WC_GEOGRAPHY,
    WC_PLACES,
    WC_HEALTH,
    WC_SELF_CARE,
    WC_HEALTHCARE_OCCUPATIONS,
    WC_HISTORY,
    WC_EVENTS,
    WC_MATHEMATICS,
    WC_ABSTRACTION,
    WC_SCIENCE,
    WC_NATURAL_SCIENCES,
    WC_NATURE,
    WC_PEOPLE,
    WC_PERSONAL_LIFE,
    WC_SELF,
    WC_PHILOSOPHY,
    WC_THOUGHT,
    WC_RELIGION,
    WC_BELIEF,
    WC_SOCIETY,
    WC_SOCIAL_SCIENCES,
    WC_TECHNOLOGY,
    WC_APPLIED_SCIENCES,
    WC_COUNT
};

enum WikiCategoryFull
{
    WCF_ABRAHAMIC_MYTHOLOGY = 0,
    WCF_ACTIVISM,
    WCF_ACTIVISTS,
    WCF_AEROSPACE_ENGINEERING,
    WCF_AESTHETICS,
    WCF_AFRICA,
    WCF_AGNOSTICISM,
    WCF_AGRICULTURE,
    WCF_AGRONOMY,
    WCF_AIR_SPORTS,
    WCF_ALGEBRA,
    WCF_ALLAH,
    WCF_ALTER_EGOS,
    WCF_ALTERNATIVE_MEDICINE,
    WCF_AMERICAN_FOOTBALL,
    WCF_ANALYSIS,
    WCF_ANALYSIS_OF_VARIANCE,
    WCF_ANIMALS,
    WCF_ANIMATION,
    WCF_ARCHAEOLOGY,
    WCF_ARCHITECTURE,
    WCF_ARITHMETIC,
    WCF_ARTIFICIAL_INTELLIGENCE,
    WCF_ARTS_AND_CRAFTS,
    WCF_ASIA,
    WCF_ASSOCIATION_FOOTBALL,
    WCF_ASTRONAUTS,
    WCF_ASTRONOMY,
    WCF_ATHEISM,
    WCF_ATMOSPHERIC_SCIENCES,
    WCF_ATTENTION,
    WCF_AUTOMATION,
    WCF_AUTOMOBILES,
    WCF_AUTO_RACING,
    WCF_AVIATION,
    WCF_AVIONICS,
    WCF_AYYAVAZHI,
    WCF_BASEBALL,
    WCF_BASKETBALL,
    WCF_BAYESIAN_STATISTICS,
    WCF_BEGINNERS_AND_NEWCOMERS,
    WCF_BIBLE,
    WCF_BILLIONAIRES,
    WCF_BIOLOGY,
    WCF_BOARD_GAMES,
    WCF_BOATING,
    WCF_BODYWEIGHT_EXERCISE,
    WCF_BOXING,
    WCF_BROADCASTING,
    WCF_BUDDHAS,
    WCF_BUDDHISM,
    WCF_BUDDHIST_MYTHOLOGY,
    WCF_BUSINESS,
    WCF_CANOEING,
    WCF_CAO_DAI,
    WCF_CARD_GAMES,
    WCF_CARDIOLOGY,
    WCF_CARTOGRAPHY,
    WCF_CATEGORICAL_DATA,
    WCF_CELEBRITY,
    WCF_CENSORSHIP_IN_THE_ARTS,
    WCF_CENTRAL_EUROPE,
    WCF_CHEMICAL_ENGINEERING,
    WCF_CHEMISTRY,
    WCF_CHILDREN,
    WCF_CHINESE_FOLK_RELIGION,
    WCF_CHRISTIANITY,
    WCF_CHRISTIAN_MYTHOLOGY,
    WCF_CITIES,
    WCF_CIVIL_ENGINEERING,
    WCF_CLASSES_OF_COMPUTERS,
    WCF_CLEANING,
    WCF_CLIMATE,
    WCF_CLINICAL_RESEARCH,
    WCF_CLOTHING,
    WCF_COGNITION,
    WCF_COGNITIVE_BIASES,
    WCF_COMICS,
    WCF_COMMUNICATION,
    WCF_COMMUNITIES,
    WCF_COMPANIES,
    WCF_COMPOSERS,
    WCF_COMPUTER_ARCHITECTURE,
    WCF_COMPUTER_ENGINEERING,
    WCF_COMPUTER_SCIENCE,
    WCF_COMPUTER_SECURITY,
    WCF_COMPUTING_AND_SOCIETY,
    WCF_CONFUCIANISM,
    WCF_CONSCIOUSNESS_STUDIES,
    WCF_CONSTRUCTION,
    WCF_CONSUMER_ELECTRONICS,
    WCF_CONTINENTS,
    WCF_CONTROL_THEORY,
    WCF_COUNTRIES,
    WCF_COVARIANCE_AND_CORRELATION,
    WCF_CRAFTS,
    WCF_CREATIVITY,
    WCF_CRICKET,
    WCF_CRIME,
    WCF_CRITICAL_THEORY,
    WCF_CULTURAL_ANTHROPOLOGY,
    WCF_CULTURAL_STUDIES,
    WCF_CYBORGS,
    WCF_CYCLING,
    WCF_DANCE,
    WCF_DATA,
    WCF_DATA_ANALYSIS,
    WCF_DECISION_THEORY,
    WCF_DEFECTORS,
    WCF_DEITIES,
    WCF_DEMOGRAPHICS,
    WCF_DEMONS,
    WCF_DESERTS,
    WCF_DESIGN,
    WCF_DESIGN_OF_EXPERIMENTS,
    WCF_DIETARY_SUPPLEMENTS,
    WCF_DIETETICS,
    WCF_DIGITAL_DIVIDE,
    WCF_DIGITAL_ELECTRONICS,
    WCF_DIGITAL_MEDIA,
    WCF_DOLLS,
    WCF_DRAWING,
    WCF_DRUIDRY,
    WCF_EARTH,
    WCF_EARTHQUAKE_ENGINEERING,
    WCF_ECOLOGY,
    WCF_ECONOMICS,
    WCF_EDUCATION,
    WCF_ELECTRICAL_COMPONENTS,
    WCF_ELECTRICAL_ENGINEERING,
    WCF_ELECTRONIC_DESIGN,
    WCF_ELECTRONICS_MANUFACTURING,
    WCF_EMBEDDED_SYSTEMS,
    WCF_EMOTION,
    WCF_EMPLOYMENT,
    WCF_ENDOCRINOLOGY,
    WCF_ENERGY,
    WCF_ENTERTAINMENT,
    WCF_ENVIRONMENT,
    WCF_ENVIRONMENTAL_ENGINEERING,
    WCF_EPIDEMIOLOGY,
    WCF_EPISTEMOLOGY,
    WCF_EQUATIONS,
    WCF_ERGONOMICS,
    WCF_ERROR,
    WCF_ETHICS,
    WCF_ETHNIC_GROUPS,
    WCF_EUROPE,
    WCF_EXERCISE_EQUIPMENT,
    WCF_EXERCISE_INSTRUCTORS,
    WCF_EXERCISE_PHYSIOLOGY,
    WCF_EXORCISM,
    WCF_FALUN_GONG,
    WCF_FAMILY,
    WCF_FESTIVALS,
    WCF_FILM,
    WCF_FIREFIGHTING,
    WCF_FIRE_PREVENTION,
    WCF_FISHING,
    WCF_FOLKLORE,
    WCF_FOOD_AND_DRINK,
    WCF_FOOD_CULTURE,
    WCF_FORENSICS,
    WCF_FORESTRY,
    WCF_FREE_SOFTWARE,
    WCF_GAMES,
    WCF_GASTROENTEROLOGY,
    WCF_GENDER,
    WCF_GENERALS,
    WCF_GEOGRAPHY,
    WCF_GEOLOGY,
    WCF_GEOMETRY,
    WCF_GEOPHYSICS,
    WCF_GERIATRICS,
    WCF_GERONTOLOGY,
    WCF_GLOBALIZATION,
    WCF_GOD,
    WCF_GOLF,
    WCF_GOVERNMENT,
    WCF_GYMNASTICS,
    WCF_HEADS_OF_STATE,
    WCF_HEALTH,
    WCF_HEALTH_BY_COUNTRY,
    WCF_HEALTH_LAW,
    WCF_HEALTH_PROMOTION,
    WCF_HEALTH_SCIENCES,
    WCF_HEALTH_STANDARDS,
    WCF_HEMATOLOGY,
    WCF_HEURISTICS,
    WCF_HIKING,
    WCF_HINDUISM,
    WCF_HINDU_MYTHOLOGY,
    WCF_HOBBIES,
    WCF_HOME,
    WCF_HORSE_RACING,
    WCF_HOSPITALS,
    WCF_HUMANITARIANS,
    WCF_HUMANS,
    WCF_ICE_HOCKEY,
    WCF_IMAGINATION,
    WCF_INCOME,
    WCF_INDUSTRIES,
    WCF_INDUSTRY,
    WCF_INFORMATION_SCIENCE,
    WCF_INFORMATION_SYSTEMS,
    WCF_INFORMATION_TECHNOLOGY,
    WCF_INFRASTRUCTURE,
    WCF_INNOVATORS,
    WCF_INTEGRATED_CIRCUITS,
    WCF_INTELLIGENCE,
    WCF_INTERNATIONAL_RELATIONS,
    WCF_INTERNET,
    WCF_INTERPERSONAL_RELATIONSHIPS,
    WCF_INVENTORS,
    WCF_ISLAM,
    WCF_ISLAMIC_MYTHOLOGY,
    WCF_JAINISM,
    WCF_JESUS,
    WCF_JEWISH_MYTHOLOGY,
    WCF_JUDAISM,
    WCF_LACROSSE,
    WCF_LAKES,
    WCF_LANDFORMS,
    WCF_LANGUAGES,
    WCF_LAW,
    WCF_LEARNING,
    WCF_LEGAL_CATEGORIES_OF_PEOPLE,
    WCF_LEISURE,
    WCF_LIFE,
    WCF_LIFE_EXTENSION,
    WCF_LINGUISTICS,
    WCF_LITERATURE,
    WCF_LOGIC,
    WCF_LOGIC_AND_STATISTICS,
    WCF_LOVE,
    WCF_MAGAZINES,
    WCF_MANUFACTURING,
    WCF_MAPS,
    WCF_MASS_MEDIA,
    WCF_MEASUREMENT,
    WCF_MECHANICAL_ENGINEERING,
    WCF_MEDIA_STUDIES,
    WCF_MEDICINE,
    WCF_MEMORY,
    WCF_MEN,
    WCF_MENTAL_HEALTH,
    WCF_METALWORKING,
    WCF_METAPHYSICS,
    WCF_MICROTECHNOLOGY,
    WCF_MICROWAVE_TECHNOLOGY,
    WCF_MIDDLE_EAST,
    WCF_MIDWIFERY,
    WCF_MILITARY,
    WCF_MILITARY_SCIENCE,
    WCF_MINERALS,
    WCF_MINING,
    WCF_MOBILE_WEB,
    WCF_MOLECULAR_ELECTRONICS,
    WCF_MONARCHS,
    WCF_MONEY,
    WCF_MORMONISM,
    WCF_MOTIVATION,
    WCF_MOUNTAINS,
    WCF_MULTIMEDIA,
    WCF_MULTIVARIATE_STATISTICS,
    WCF_MUSEOLOGY,
    WCF_MUSIC,
    WCF_MUSICAL_GROUPS,
    WCF_MUSICIANS,
    WCF_MYTHOLOGY,
    WCF_NANOTECHNOLOGY,
    WCF_NATURAL_RESOURCES,
    WCF_NEOPAGANISM,
    WCF_NEPHROLOGY,
    WCF_NETWORKS,
    WCF_NEUROLOGY,
    WCF_NEW_MEDIA_ART,
    WCF_NEWSPAPERS,
    WCF_NON_PARAMETRIC_STATISTICS,
    WCF_NOOTROPICS,
    WCF_NORTH_AMERICA,
    WCF_NUCLEAR_TECHNOLOGY,
    WCF_NUMBERS,
    WCF_NURSING,
    WCF_NUTRIENTS,
    WCF_NUTRITIONAL_ADVICE_PYRAMIDS,
    WCF_OBSTETRICS,
    WCF_OCCULT,
    WCF_OCCUPATIONAL_SAFETY_AND_HEALTH,
    WCF_OCEANIA,
    WCF_OCEANOGRAPHY,
    WCF_OCEANS,
    WCF_OLD_AGE,
    WCF_OLYMPIC_GAMES,
    WCF_ONCOLOGY,
    WCF_OPERA,
    WCF_OPERATING_SYSTEMS,
    WCF_OPHTHALMOLOGY,
    WCF_OPTOELECTRONICS,
    WCF_OPTOMETRY,
    WCF_ORAL_HYGIENE,
    WCF_ORGANIZATIONS,
    WCF_ORTHODONTICS,
    WCF_ORTHOPEDIC_SURGICAL_PROCEDURES,
    WCF_PAINTING,
    WCF_PARAMETRIC_STATISTICS,
    WCF_PARTIES,
    WCF_PATHOLOGY,
    WCF_PEACE,
    WCF_PEDIATRICS,
    WCF_PEOPLE_ASSOCIATED_WITH_WAR,
    WCF_PERCEPTION,
    WCF_PERSONAL_DEVELOPMENT,
    WCF_PERSONALITY,
    WCF_PETS,
    WCF_PHARMACEUTICAL_INDUSTRY,
    WCF_PHARMACEUTICALS_POLICY,
    WCF_PHARMACY,
    WCF_PHILOSOPHERS,
    WCF_PHILOSOPHY,
    WCF_PHOTOGRAPHERS,
    WCF_PHOTOGRAPHY,
    WCF_PHYSICS,
    WCF_PHYTOCHEMICALS,
    WCF_PILATES,
    WCF_PLANTS,
    WCF_PLUMBING,
    WCF_POETRY,
    WCF_POLITICAL_PEOPLE,
    WCF_POLITICAL_SCIENCE,
    WCF_POLITICIANS,
    WCF_POLITICS,
    WCF_POLLUTION,
    WCF_POPULAR_CULTURE,
    WCF_PRAYER,
    WCF_PRESIDENTS,
    WCF_PREVENTION,
    WCF_PRINCES,
    WCF_PRINCESSES,
    WCF_PROBLEM_SOLVING,
    WCF_PRODUCT_LIFECYCLE_MANAGEMENT,
    WCF_PROPHECY,
    WCF_PROTECTED_AREAS,
    WCF_PSYCHIATRY,
    WCF_PSYCHOLOGICAL_ADJUSTMENT,
    WCF_PSYCHOLOGY,
    WCF_PSYCHOMETRICS,
    WCF_PSYCHOTHERAPY,
    WCF_PUBLIC_ADMINISTRATION,
    WCF_PUBLICATIONS,
    WCF_PUBLIC_HEALTH,
    WCF_PUBLIC_TRANSPORT,
    WCF_PUBLISHING,
    WCF_PUPPETRY,
    WCF_PUZZLES,
    WCF_QUALITIES_OF_THOUGHT,
    WCF_QUANTUM_ELECTRONICS,
    WCF_QURAN,
    WCF_RADIO,
    WCF_RADIO_ELECTRONICS,
    WCF_RAIL_TRANSPORT,
    WCF_REAL_ESTATE,
    WCF_REAL_TIME_COMPUTING,
    WCF_REGIONS,
    WCF_REGRESSION_ANALYSIS,
    WCF_RELIGIOUS_ETHICS,
    WCF_RELIGIOUS_LAW,
    WCF_REVOLUTIONARIES,
    WCF_RHEUMATOLOGY,
    WCF_RIGHTS,
    WCF_RITUAL,
    WCF_RIVALRY,
    WCF_RIVERS,
    WCF_ROAD_TRANSPORT,
    WCF_ROBOTICS,
    WCF_ROLE_PLAYING_GAMES,
    WCF_RUGBY_LEAGUE,
    WCF_RUGBY_UNION,
    WCF_RUNNING,
    WCF_SAFETY,
    WCF_SAILING,
    WCF_SAMPLING,
    WCF_SATANISM,
    WCF_SCIENCE_AND_CULTURE,
    WCF_SCIENTISTS,
    WCF_SCIENTOLOGY,
    WCF_SCULPTURE,
    WCF_SEMICONDUCTORS,
    WCF_SETTLERS,
    WCF_SEXOLOGY,
    WCF_SEXUAL_HEALTH,
    WCF_SEXUALITY,
    WCF_SEXUAL_ORIENTATION,
    WCF_SHINTO,
    WCF_SHIPPING,
    WCF_SIGNAL_CABLES,
    WCF_SIKHISM,
    WCF_SKIING,
    WCF_SLAVES,
    WCF_SLEEP,
    WCF_SOCIAL_GROUPS,
    WCF_SOCIAL_PHILOSOPHY,
    WCF_SOCIAL_SCIENTISTS,
    WCF_SOCIAL_WORK,
    WCF_SOCIOLOGY,
    WCF_SOFTWARE,
    WCF_SOFTWARE_ENGINEERING,
    WCF_SOUND_TECHNOLOGY,
    WCF_SOUTH_AMERICA,
    WCF_SPACE,
    WCF_SPACEFLIGHT,
    WCF_SPIRITISM,
    WCF_SPIRITUALITY,
    WCF_SPORTS,
    WCF_STATISTICAL_THEORY,
    WCF_STOCHASTIC_PROCESSES,
    WCF_STORYTELLING,
    WCF_STRUCTURAL_ENGINEERING,
    WCF_SUBCULTURES,
    WCF_SUMMARY_STATISTICS,
    WCF_SURGERY,
    WCF_SURVEILLANCE,
    WCF_SURVIVAL_ANALYSIS,
    WCF_SWIMMING,
    WCF_SYSTEMS_ENGINEERING,
    WCF_SYSTEMS_THEORY,
    WCF_TAOISM,
    WCF_TECHNOLOGY_FORECASTING,
    WCF_TELECOMMUNICATIONS,
    WCF_TELEVISION,
    WCF_TENNIS,
    WCF_TENRIKYO,
    WCF_TERMINOLOGY,
    WCF_TERRITORIES,
    WCF_THEATRE,
    WCF_THEOLOGY,
    WCF_THEOREMS,
    WCF_TIME_SERIES_ANALYSIS,
    WCF_TOOLS,
    WCF_TOWNS,
    WCF_TRACK_AND_FIELD,
    WCF_TRADITIONS,
    WCF_TRIGONOMETRY,
    WCF_UNCERTAINTY_OF_NUMBERS,
    WCF_UNITARIAN_UNIVERSALISM,
    WCF_UNSOLVED_PROBLEMS_IN_COMPUTER_SCIENCE,
    WCF_UROLOGY,
    WCF_VEHICLES,
    WCF_VETERINARY_MEDICINE,
    WCF_VILLAGES,
    WCF_VITAMINS,
    WCF_WALKING,
    WCF_WAR,
    WCF_WATER_SPORTS,
    WCF_WATER_TECHNOLOGY,
    WCF_WATER_TRANSPORT,
    WCF_WEIGHT_TRAINING,
    WCF_WHITEWATER_SPORTS,
    WCF_WICCA,
    WCF_WOMEN,
    WCF_YOGA,
    WCF_ZOROASTRIANISM,
    WCF_COUNT
};

class MapWstring2Category : public unordered_map<wstring, int>
{
public:
	MapWstring2Category() : unordered_map<wstring, int>()
	{
        this->operator[](L"Abrahamic_mythology")  = WCF_ABRAHAMIC_MYTHOLOGY;
        this->operator[](L"Activism")  = WCF_ACTIVISM;
        this->operator[](L"Activists")  = WCF_ACTIVISTS;
        this->operator[](L"Aerospace_engineering")  = WCF_AEROSPACE_ENGINEERING;
        this->operator[](L"Aesthetics")  = WCF_AESTHETICS;
        this->operator[](L"Africa")  = WCF_AFRICA;
        this->operator[](L"Agnosticism")  = WCF_AGNOSTICISM;
        this->operator[](L"Agriculture")  = WCF_AGRICULTURE;
        this->operator[](L"Agronomy")  = WCF_AGRONOMY;
        this->operator[](L"Air_sports")  = WCF_AIR_SPORTS;
        this->operator[](L"Algebra")  = WCF_ALGEBRA;
        this->operator[](L"Allah")  = WCF_ALLAH;
        this->operator[](L"Alter_egos")  = WCF_ALTER_EGOS;
        this->operator[](L"Alternative_medicine")  = WCF_ALTERNATIVE_MEDICINE;
        this->operator[](L"American_football")  = WCF_AMERICAN_FOOTBALL;
        this->operator[](L"Analysis")  = WCF_ANALYSIS;
        this->operator[](L"Analysis_of_variance")  = WCF_ANALYSIS_OF_VARIANCE;
        this->operator[](L"Animals")  = WCF_ANIMALS;
        this->operator[](L"Animation")  = WCF_ANIMATION;
        this->operator[](L"Archaeology")  = WCF_ARCHAEOLOGY;
        this->operator[](L"Architecture")  = WCF_ARCHITECTURE;
        this->operator[](L"Arithmetic")  = WCF_ARITHMETIC;
        this->operator[](L"Artificial_intelligence")  = WCF_ARTIFICIAL_INTELLIGENCE;
        this->operator[](L"Arts_and_crafts")  = WCF_ARTS_AND_CRAFTS;
        this->operator[](L"Asia")  = WCF_ASIA;
        this->operator[](L"Association_football")  = WCF_ASSOCIATION_FOOTBALL;
        this->operator[](L"Astronauts")  = WCF_ASTRONAUTS;
        this->operator[](L"Astronomy")  = WCF_ASTRONOMY;
        this->operator[](L"Atheism")  = WCF_ATHEISM;
        this->operator[](L"Atmospheric_sciences")  = WCF_ATMOSPHERIC_SCIENCES;
        this->operator[](L"Attention")  = WCF_ATTENTION;
        this->operator[](L"Automation")  = WCF_AUTOMATION;
        this->operator[](L"Automobiles")  = WCF_AUTOMOBILES;
        this->operator[](L"Auto_racing")  = WCF_AUTO_RACING;
        this->operator[](L"Aviation")  = WCF_AVIATION;
        this->operator[](L"Avionics")  = WCF_AVIONICS;
        this->operator[](L"Ayyavazhi")  = WCF_AYYAVAZHI;
        this->operator[](L"Baseball")  = WCF_BASEBALL;
        this->operator[](L"Basketball")  = WCF_BASKETBALL;
        this->operator[](L"Bayesian_statistics")  = WCF_BAYESIAN_STATISTICS;
        this->operator[](L"Beginners_and_newcomers")  = WCF_BEGINNERS_AND_NEWCOMERS;
        this->operator[](L"Bible")  = WCF_BIBLE;
        this->operator[](L"Billionaires")  = WCF_BILLIONAIRES;
        this->operator[](L"Biology")  = WCF_BIOLOGY;
        this->operator[](L"Board_games")  = WCF_BOARD_GAMES;
        this->operator[](L"Boating")  = WCF_BOATING;
        this->operator[](L"Bodyweight_exercise")  = WCF_BODYWEIGHT_EXERCISE;
        this->operator[](L"Boxing")  = WCF_BOXING;
        this->operator[](L"Broadcasting")  = WCF_BROADCASTING;
        this->operator[](L"Buddhas")  = WCF_BUDDHAS;
        this->operator[](L"Buddhism")  = WCF_BUDDHISM;
        this->operator[](L"Buddhist_mythology")  = WCF_BUDDHIST_MYTHOLOGY;
        this->operator[](L"Business")  = WCF_BUSINESS;
        this->operator[](L"Canoeing")  = WCF_CANOEING;
        this->operator[](L"Cao_Dai")  = WCF_CAO_DAI;
        this->operator[](L"Card_games")  = WCF_CARD_GAMES;
        this->operator[](L"Cardiology")  = WCF_CARDIOLOGY;
        this->operator[](L"Cartography")  = WCF_CARTOGRAPHY;
        this->operator[](L"Categorical_data")  = WCF_CATEGORICAL_DATA;
        this->operator[](L"Celebrity")  = WCF_CELEBRITY;
        this->operator[](L"Censorship_in_the_arts")  = WCF_CENSORSHIP_IN_THE_ARTS;
        this->operator[](L"Central_Europe")  = WCF_CENTRAL_EUROPE;
        this->operator[](L"Chemical_engineering")  = WCF_CHEMICAL_ENGINEERING;
        this->operator[](L"Chemistry")  = WCF_CHEMISTRY;
        this->operator[](L"Children")  = WCF_CHILDREN;
        this->operator[](L"Chinese_folk_religion")  = WCF_CHINESE_FOLK_RELIGION;
        this->operator[](L"Christianity")  = WCF_CHRISTIANITY;
        this->operator[](L"Christian_mythology")  = WCF_CHRISTIAN_MYTHOLOGY;
        this->operator[](L"Cities")  = WCF_CITIES;
        this->operator[](L"Civil_engineering")  = WCF_CIVIL_ENGINEERING;
        this->operator[](L"Classes_of_computers")  = WCF_CLASSES_OF_COMPUTERS;
        this->operator[](L"Cleaning")  = WCF_CLEANING;
        this->operator[](L"Climate")  = WCF_CLIMATE;
        this->operator[](L"Clinical_research")  = WCF_CLINICAL_RESEARCH;
        this->operator[](L"Clothing")  = WCF_CLOTHING;
        this->operator[](L"Cognition")  = WCF_COGNITION;
        this->operator[](L"Cognitive_biases")  = WCF_COGNITIVE_BIASES;
        this->operator[](L"Comics")  = WCF_COMICS;
        this->operator[](L"Communication")  = WCF_COMMUNICATION;
        this->operator[](L"Communities")  = WCF_COMMUNITIES;
        this->operator[](L"Companies")  = WCF_COMPANIES;
        this->operator[](L"Composers")  = WCF_COMPOSERS;
        this->operator[](L"Computer_architecture")  = WCF_COMPUTER_ARCHITECTURE;
        this->operator[](L"Computer_engineering")  = WCF_COMPUTER_ENGINEERING;
        this->operator[](L"Computer_science")  = WCF_COMPUTER_SCIENCE;
        this->operator[](L"Computer_security")  = WCF_COMPUTER_SECURITY;
        this->operator[](L"Computing_and_society")  = WCF_COMPUTING_AND_SOCIETY;
        this->operator[](L"Confucianism")  = WCF_CONFUCIANISM;
        this->operator[](L"Consciousness_studies")  = WCF_CONSCIOUSNESS_STUDIES;
        this->operator[](L"Construction")  = WCF_CONSTRUCTION;
        this->operator[](L"Consumer_electronics")  = WCF_CONSUMER_ELECTRONICS;
        this->operator[](L"Continents")  = WCF_CONTINENTS;
        this->operator[](L"Control_theory")  = WCF_CONTROL_THEORY;
        this->operator[](L"Countries")  = WCF_COUNTRIES;
        this->operator[](L"Covariance_and_correlation")  = WCF_COVARIANCE_AND_CORRELATION;
        this->operator[](L"Crafts")  = WCF_CRAFTS;
        this->operator[](L"Creativity")  = WCF_CREATIVITY;
        this->operator[](L"Cricket")  = WCF_CRICKET;
        this->operator[](L"Crime")  = WCF_CRIME;
        this->operator[](L"Critical_theory")  = WCF_CRITICAL_THEORY;
        this->operator[](L"Cultural_anthropology")  = WCF_CULTURAL_ANTHROPOLOGY;
        this->operator[](L"Cultural_studies")  = WCF_CULTURAL_STUDIES;
        this->operator[](L"Cyborgs")  = WCF_CYBORGS;
        this->operator[](L"Cycling")  = WCF_CYCLING;
        this->operator[](L"Dance")  = WCF_DANCE;
        this->operator[](L"Data")  = WCF_DATA;
        this->operator[](L"Data_analysis")  = WCF_DATA_ANALYSIS;
        this->operator[](L"Decision_theory")  = WCF_DECISION_THEORY;
        this->operator[](L"Defectors")  = WCF_DEFECTORS;
        this->operator[](L"Deities")  = WCF_DEITIES;
        this->operator[](L"Demographics")  = WCF_DEMOGRAPHICS;
        this->operator[](L"Demons")  = WCF_DEMONS;
        this->operator[](L"Deserts")  = WCF_DESERTS;
        this->operator[](L"Design")  = WCF_DESIGN;
        this->operator[](L"Design_of_experiments")  = WCF_DESIGN_OF_EXPERIMENTS;
        this->operator[](L"Dietary_supplements")  = WCF_DIETARY_SUPPLEMENTS;
        this->operator[](L"Dietetics")  = WCF_DIETETICS;
        this->operator[](L"Digital_divide")  = WCF_DIGITAL_DIVIDE;
        this->operator[](L"Digital_electronics")  = WCF_DIGITAL_ELECTRONICS;
        this->operator[](L"Digital_media")  = WCF_DIGITAL_MEDIA;
        this->operator[](L"Dolls")  = WCF_DOLLS;
        this->operator[](L"Drawing")  = WCF_DRAWING;
        this->operator[](L"Druidry")  = WCF_DRUIDRY;
        this->operator[](L"Earth")  = WCF_EARTH;
        this->operator[](L"Earthquake_engineering")  = WCF_EARTHQUAKE_ENGINEERING;
        this->operator[](L"Ecology")  = WCF_ECOLOGY;
        this->operator[](L"Economics")  = WCF_ECONOMICS;
        this->operator[](L"Education")  = WCF_EDUCATION;
        this->operator[](L"Electrical_components")  = WCF_ELECTRICAL_COMPONENTS;
        this->operator[](L"Electrical_engineering")  = WCF_ELECTRICAL_ENGINEERING;
        this->operator[](L"Electronic_design")  = WCF_ELECTRONIC_DESIGN;
        this->operator[](L"Electronics_manufacturing")  = WCF_ELECTRONICS_MANUFACTURING;
        this->operator[](L"Embedded_systems")  = WCF_EMBEDDED_SYSTEMS;
        this->operator[](L"Emotion")  = WCF_EMOTION;
        this->operator[](L"Employment")  = WCF_EMPLOYMENT;
        this->operator[](L"Endocrinology")  = WCF_ENDOCRINOLOGY;
        this->operator[](L"Energy")  = WCF_ENERGY;
        this->operator[](L"Entertainment")  = WCF_ENTERTAINMENT;
        this->operator[](L"Environment")  = WCF_ENVIRONMENT;
        this->operator[](L"Environmental_engineering")  = WCF_ENVIRONMENTAL_ENGINEERING;
        this->operator[](L"Epidemiology")  = WCF_EPIDEMIOLOGY;
        this->operator[](L"Epistemology")  = WCF_EPISTEMOLOGY;
        this->operator[](L"Equations")  = WCF_EQUATIONS;
        this->operator[](L"Ergonomics")  = WCF_ERGONOMICS;
        this->operator[](L"Error")  = WCF_ERROR;
        this->operator[](L"Ethics")  = WCF_ETHICS;
        this->operator[](L"Ethnic_groups")  = WCF_ETHNIC_GROUPS;
        this->operator[](L"Europe")  = WCF_EUROPE;
        this->operator[](L"Exercise_equipment")  = WCF_EXERCISE_EQUIPMENT;
        this->operator[](L"Exercise_instructors")  = WCF_EXERCISE_INSTRUCTORS;
        this->operator[](L"Exercise_physiology")  = WCF_EXERCISE_PHYSIOLOGY;
        this->operator[](L"Exorcism")  = WCF_EXORCISM;
        this->operator[](L"Falun_Gong")  = WCF_FALUN_GONG;
        this->operator[](L"Family")  = WCF_FAMILY;
        this->operator[](L"Festivals")  = WCF_FESTIVALS;
        this->operator[](L"Film")  = WCF_FILM;
        this->operator[](L"Firefighting")  = WCF_FIREFIGHTING;
        this->operator[](L"Fire_prevention")  = WCF_FIRE_PREVENTION;
        this->operator[](L"Fishing")  = WCF_FISHING;
        this->operator[](L"Folklore")  = WCF_FOLKLORE;
        this->operator[](L"Food_and_drink")  = WCF_FOOD_AND_DRINK;
        this->operator[](L"Food_culture")  = WCF_FOOD_CULTURE;
        this->operator[](L"Forensics")  = WCF_FORENSICS;
        this->operator[](L"Forestry")  = WCF_FORESTRY;
        this->operator[](L"Free_software")  = WCF_FREE_SOFTWARE;
        this->operator[](L"Games")  = WCF_GAMES;
        this->operator[](L"Gastroenterology")  = WCF_GASTROENTEROLOGY;
        this->operator[](L"Gender")  = WCF_GENDER;
        this->operator[](L"Generals")  = WCF_GENERALS;
        this->operator[](L"Geography")  = WCF_GEOGRAPHY;
        this->operator[](L"Geology")  = WCF_GEOLOGY;
        this->operator[](L"Geometry")  = WCF_GEOMETRY;
        this->operator[](L"Geophysics")  = WCF_GEOPHYSICS;
        this->operator[](L"Geriatrics")  = WCF_GERIATRICS;
        this->operator[](L"Gerontology")  = WCF_GERONTOLOGY;
        this->operator[](L"Globalization")  = WCF_GLOBALIZATION;
        this->operator[](L"God")  = WCF_GOD;
        this->operator[](L"Golf")  = WCF_GOLF;
        this->operator[](L"Government")  = WCF_GOVERNMENT;
        this->operator[](L"Gymnastics")  = WCF_GYMNASTICS;
        this->operator[](L"Heads_of_state")  = WCF_HEADS_OF_STATE;
        this->operator[](L"Health")  = WCF_HEALTH;
        this->operator[](L"Health_by_country")  = WCF_HEALTH_BY_COUNTRY;
        this->operator[](L"Health_law")  = WCF_HEALTH_LAW;
        this->operator[](L"Health_promotion")  = WCF_HEALTH_PROMOTION;
        this->operator[](L"Health_sciences")  = WCF_HEALTH_SCIENCES;
        this->operator[](L"Health_standards")  = WCF_HEALTH_STANDARDS;
        this->operator[](L"Hematology")  = WCF_HEMATOLOGY;
        this->operator[](L"Heuristics")  = WCF_HEURISTICS;
        this->operator[](L"Hiking")  = WCF_HIKING;
        this->operator[](L"Hinduism")  = WCF_HINDUISM;
        this->operator[](L"Hindu_mythology")  = WCF_HINDU_MYTHOLOGY;
        this->operator[](L"Hobbies")  = WCF_HOBBIES;
        this->operator[](L"Home")  = WCF_HOME;
        this->operator[](L"Horse_racing")  = WCF_HORSE_RACING;
        this->operator[](L"Hospitals")  = WCF_HOSPITALS;
        this->operator[](L"Humanitarians")  = WCF_HUMANITARIANS;
        this->operator[](L"Humans")  = WCF_HUMANS;
        this->operator[](L"Ice_hockey")  = WCF_ICE_HOCKEY;
        this->operator[](L"Imagination")  = WCF_IMAGINATION;
        this->operator[](L"Income")  = WCF_INCOME;
        this->operator[](L"Industries")  = WCF_INDUSTRIES;
        this->operator[](L"Industry")  = WCF_INDUSTRY;
        this->operator[](L"Information_science")  = WCF_INFORMATION_SCIENCE;
        this->operator[](L"Information_systems")  = WCF_INFORMATION_SYSTEMS;
        this->operator[](L"Information_technology")  = WCF_INFORMATION_TECHNOLOGY;
        this->operator[](L"Infrastructure")  = WCF_INFRASTRUCTURE;
        this->operator[](L"Innovators")  = WCF_INNOVATORS;
        this->operator[](L"Integrated_circuits")  = WCF_INTEGRATED_CIRCUITS;
        this->operator[](L"Intelligence")  = WCF_INTELLIGENCE;
        this->operator[](L"International_relations")  = WCF_INTERNATIONAL_RELATIONS;
        this->operator[](L"Internet")  = WCF_INTERNET;
        this->operator[](L"Interpersonal_relationships")  = WCF_INTERPERSONAL_RELATIONSHIPS;
        this->operator[](L"Inventors")  = WCF_INVENTORS;
        this->operator[](L"Islam")  = WCF_ISLAM;
        this->operator[](L"Islamic_mythology")  = WCF_ISLAMIC_MYTHOLOGY;
        this->operator[](L"Jainism")  = WCF_JAINISM;
        this->operator[](L"Jesus")  = WCF_JESUS;
        this->operator[](L"Jewish_mythology")  = WCF_JEWISH_MYTHOLOGY;
        this->operator[](L"Judaism")  = WCF_JUDAISM;
        this->operator[](L"Lacrosse")  = WCF_LACROSSE;
        this->operator[](L"Lakes")  = WCF_LAKES;
        this->operator[](L"Landforms")  = WCF_LANDFORMS;
        this->operator[](L"Languages")  = WCF_LANGUAGES;
        this->operator[](L"Law")  = WCF_LAW;
        this->operator[](L"Learning")  = WCF_LEARNING;
        this->operator[](L"Legal_categories_of_people")  = WCF_LEGAL_CATEGORIES_OF_PEOPLE;
        this->operator[](L"Leisure")  = WCF_LEISURE;
        this->operator[](L"Life")  = WCF_LIFE;
        this->operator[](L"Life_extension")  = WCF_LIFE_EXTENSION;
        this->operator[](L"Linguistics")  = WCF_LINGUISTICS;
        this->operator[](L"Literature")  = WCF_LITERATURE;
        this->operator[](L"Logic")  = WCF_LOGIC;
        this->operator[](L"Logic_and_statistics")  = WCF_LOGIC_AND_STATISTICS;
        this->operator[](L"Love")  = WCF_LOVE;
        this->operator[](L"Magazines")  = WCF_MAGAZINES;
        this->operator[](L"Manufacturing")  = WCF_MANUFACTURING;
        this->operator[](L"Maps")  = WCF_MAPS;
        this->operator[](L"Mass_media")  = WCF_MASS_MEDIA;
        this->operator[](L"Measurement")  = WCF_MEASUREMENT;
        this->operator[](L"Mechanical_engineering")  = WCF_MECHANICAL_ENGINEERING;
        this->operator[](L"Media_studies")  = WCF_MEDIA_STUDIES;
        this->operator[](L"Medicine")  = WCF_MEDICINE;
        this->operator[](L"Memory")  = WCF_MEMORY;
        this->operator[](L"Men")  = WCF_MEN;
        this->operator[](L"Mental_health")  = WCF_MENTAL_HEALTH;
        this->operator[](L"Metalworking")  = WCF_METALWORKING;
        this->operator[](L"Metaphysics")  = WCF_METAPHYSICS;
        this->operator[](L"Microtechnology")  = WCF_MICROTECHNOLOGY;
        this->operator[](L"Microwave_technology")  = WCF_MICROWAVE_TECHNOLOGY;
        this->operator[](L"Middle_East")  = WCF_MIDDLE_EAST;
        this->operator[](L"Midwifery")  = WCF_MIDWIFERY;
        this->operator[](L"Military")  = WCF_MILITARY;
        this->operator[](L"Military_science")  = WCF_MILITARY_SCIENCE;
        this->operator[](L"Minerals")  = WCF_MINERALS;
        this->operator[](L"Mining")  = WCF_MINING;
        this->operator[](L"Mobile_Web")  = WCF_MOBILE_WEB;
        this->operator[](L"Molecular_electronics")  = WCF_MOLECULAR_ELECTRONICS;
        this->operator[](L"Monarchs")  = WCF_MONARCHS;
        this->operator[](L"Money")  = WCF_MONEY;
        this->operator[](L"Mormonism")  = WCF_MORMONISM;
        this->operator[](L"Motivation")  = WCF_MOTIVATION;
        this->operator[](L"Mountains")  = WCF_MOUNTAINS;
        this->operator[](L"Multimedia")  = WCF_MULTIMEDIA;
        this->operator[](L"Multivariate_statistics")  = WCF_MULTIVARIATE_STATISTICS;
        this->operator[](L"Museology")  = WCF_MUSEOLOGY;
        this->operator[](L"Music")  = WCF_MUSIC;
        this->operator[](L"Musical_groups")  = WCF_MUSICAL_GROUPS;
        this->operator[](L"Musicians")  = WCF_MUSICIANS;
        this->operator[](L"Mythology")  = WCF_MYTHOLOGY;
        this->operator[](L"Nanotechnology")  = WCF_NANOTECHNOLOGY;
        this->operator[](L"Natural_resources")  = WCF_NATURAL_RESOURCES;
        this->operator[](L"Neopaganism")  = WCF_NEOPAGANISM;
        this->operator[](L"Nephrology")  = WCF_NEPHROLOGY;
        this->operator[](L"Networks")  = WCF_NETWORKS;
        this->operator[](L"Neurology")  = WCF_NEUROLOGY;
        this->operator[](L"New_media_art")  = WCF_NEW_MEDIA_ART;
        this->operator[](L"Newspapers")  = WCF_NEWSPAPERS;
        this->operator[](L"Non-parametric_statistics")  = WCF_NON_PARAMETRIC_STATISTICS;
        this->operator[](L"Nootropics")  = WCF_NOOTROPICS;
        this->operator[](L"North_America")  = WCF_NORTH_AMERICA;
        this->operator[](L"Nuclear_technology")  = WCF_NUCLEAR_TECHNOLOGY;
        this->operator[](L"Numbers")  = WCF_NUMBERS;
        this->operator[](L"Nursing")  = WCF_NURSING;
        this->operator[](L"Nutrients")  = WCF_NUTRIENTS;
        this->operator[](L"Nutritional_advice_pyramids")  = WCF_NUTRITIONAL_ADVICE_PYRAMIDS;
        this->operator[](L"Obstetrics")  = WCF_OBSTETRICS;
        this->operator[](L"Occult")  = WCF_OCCULT;
        this->operator[](L"Occupational_safety_and_health")  = WCF_OCCUPATIONAL_SAFETY_AND_HEALTH;
        this->operator[](L"Oceania")  = WCF_OCEANIA;
        this->operator[](L"Oceanography")  = WCF_OCEANOGRAPHY;
        this->operator[](L"Oceans")  = WCF_OCEANS;
        this->operator[](L"Old_age")  = WCF_OLD_AGE;
        this->operator[](L"Olympic_Games")  = WCF_OLYMPIC_GAMES;
        this->operator[](L"Oncology")  = WCF_ONCOLOGY;
        this->operator[](L"Opera")  = WCF_OPERA;
        this->operator[](L"Operating_systems")  = WCF_OPERATING_SYSTEMS;
        this->operator[](L"Ophthalmology")  = WCF_OPHTHALMOLOGY;
        this->operator[](L"Optoelectronics")  = WCF_OPTOELECTRONICS;
        this->operator[](L"Optometry")  = WCF_OPTOMETRY;
        this->operator[](L"Oral_hygiene")  = WCF_ORAL_HYGIENE;
        this->operator[](L"Organizations")  = WCF_ORGANIZATIONS;
        this->operator[](L"Orthodontics")  = WCF_ORTHODONTICS;
        this->operator[](L"Orthopedic_surgical_procedures")  = WCF_ORTHOPEDIC_SURGICAL_PROCEDURES;
        this->operator[](L"Painting")  = WCF_PAINTING;
        this->operator[](L"Parametric_statistics")  = WCF_PARAMETRIC_STATISTICS;
        this->operator[](L"Parties")  = WCF_PARTIES;
        this->operator[](L"Pathology")  = WCF_PATHOLOGY;
        this->operator[](L"Peace")  = WCF_PEACE;
        this->operator[](L"Pediatrics")  = WCF_PEDIATRICS;
        this->operator[](L"People_associated_with_war")  = WCF_PEOPLE_ASSOCIATED_WITH_WAR;
        this->operator[](L"Perception")  = WCF_PERCEPTION;
        this->operator[](L"Personal_development")  = WCF_PERSONAL_DEVELOPMENT;
        this->operator[](L"Personality")  = WCF_PERSONALITY;
        this->operator[](L"Pets")  = WCF_PETS;
        this->operator[](L"Pharmaceutical_industry")  = WCF_PHARMACEUTICAL_INDUSTRY;
        this->operator[](L"Pharmaceuticals_policy")  = WCF_PHARMACEUTICALS_POLICY;
        this->operator[](L"Pharmacy")  = WCF_PHARMACY;
        this->operator[](L"Philosophers")  = WCF_PHILOSOPHERS;
        this->operator[](L"Philosophy")  = WCF_PHILOSOPHY;
        this->operator[](L"Photographers")  = WCF_PHOTOGRAPHERS;
        this->operator[](L"Photography")  = WCF_PHOTOGRAPHY;
        this->operator[](L"Physics")  = WCF_PHYSICS;
        this->operator[](L"Phytochemicals")  = WCF_PHYTOCHEMICALS;
        this->operator[](L"Pilates")  = WCF_PILATES;
        this->operator[](L"Plants")  = WCF_PLANTS;
        this->operator[](L"Plumbing")  = WCF_PLUMBING;
        this->operator[](L"Poetry")  = WCF_POETRY;
        this->operator[](L"Political_people")  = WCF_POLITICAL_PEOPLE;
        this->operator[](L"Political_science")  = WCF_POLITICAL_SCIENCE;
        this->operator[](L"Politicians")  = WCF_POLITICIANS;
        this->operator[](L"Politics")  = WCF_POLITICS;
        this->operator[](L"Pollution")  = WCF_POLLUTION;
        this->operator[](L"Popular_culture")  = WCF_POPULAR_CULTURE;
        this->operator[](L"Prayer")  = WCF_PRAYER;
        this->operator[](L"Presidents")  = WCF_PRESIDENTS;
        this->operator[](L"Prevention")  = WCF_PREVENTION;
        this->operator[](L"Princes")  = WCF_PRINCES;
        this->operator[](L"Princesses")  = WCF_PRINCESSES;
        this->operator[](L"Problem_solving")  = WCF_PROBLEM_SOLVING;
        this->operator[](L"Product_lifecycle_management")  = WCF_PRODUCT_LIFECYCLE_MANAGEMENT;
        this->operator[](L"Prophecy")  = WCF_PROPHECY;
        this->operator[](L"Protected_areas")  = WCF_PROTECTED_AREAS;
        this->operator[](L"Psychiatry")  = WCF_PSYCHIATRY;
        this->operator[](L"Psychological_adjustment")  = WCF_PSYCHOLOGICAL_ADJUSTMENT;
        this->operator[](L"Psychology")  = WCF_PSYCHOLOGY;
        this->operator[](L"Psychometrics")  = WCF_PSYCHOMETRICS;
        this->operator[](L"Psychotherapy")  = WCF_PSYCHOTHERAPY;
        this->operator[](L"Public_administration")  = WCF_PUBLIC_ADMINISTRATION;
        this->operator[](L"Publications")  = WCF_PUBLICATIONS;
        this->operator[](L"Public_health")  = WCF_PUBLIC_HEALTH;
        this->operator[](L"Public_transport")  = WCF_PUBLIC_TRANSPORT;
        this->operator[](L"Publishing")  = WCF_PUBLISHING;
        this->operator[](L"Puppetry")  = WCF_PUPPETRY;
        this->operator[](L"Puzzles")  = WCF_PUZZLES;
        this->operator[](L"Qualities_of_thought")  = WCF_QUALITIES_OF_THOUGHT;
        this->operator[](L"Quantum_electronics")  = WCF_QUANTUM_ELECTRONICS;
        this->operator[](L"Quran")  = WCF_QURAN;
        this->operator[](L"Radio")  = WCF_RADIO;
        this->operator[](L"Radio_electronics")  = WCF_RADIO_ELECTRONICS;
        this->operator[](L"Rail_transport")  = WCF_RAIL_TRANSPORT;
        this->operator[](L"Real_estate")  = WCF_REAL_ESTATE;
        this->operator[](L"Real-time_computing")  = WCF_REAL_TIME_COMPUTING;
        this->operator[](L"Regions")  = WCF_REGIONS;
        this->operator[](L"Regression_analysis")  = WCF_REGRESSION_ANALYSIS;
        this->operator[](L"Religious_ethics")  = WCF_RELIGIOUS_ETHICS;
        this->operator[](L"Religious_law")  = WCF_RELIGIOUS_LAW;
        this->operator[](L"Revolutionaries")  = WCF_REVOLUTIONARIES;
        this->operator[](L"Rheumatology")  = WCF_RHEUMATOLOGY;
        this->operator[](L"Rights")  = WCF_RIGHTS;
        this->operator[](L"Ritual")  = WCF_RITUAL;
        this->operator[](L"Rivalry")  = WCF_RIVALRY;
        this->operator[](L"Rivers")  = WCF_RIVERS;
        this->operator[](L"Road_transport")  = WCF_ROAD_TRANSPORT;
        this->operator[](L"Robotics")  = WCF_ROBOTICS;
        this->operator[](L"Role-playing_games")  = WCF_ROLE_PLAYING_GAMES;
        this->operator[](L"Rugby_league")  = WCF_RUGBY_LEAGUE;
        this->operator[](L"Rugby_union")  = WCF_RUGBY_UNION;
        this->operator[](L"Running")  = WCF_RUNNING;
        this->operator[](L"Safety")  = WCF_SAFETY;
        this->operator[](L"Sailing")  = WCF_SAILING;
        this->operator[](L"Sampling")  = WCF_SAMPLING;
        this->operator[](L"Satanism")  = WCF_SATANISM;
        this->operator[](L"Science_and_culture")  = WCF_SCIENCE_AND_CULTURE;
        this->operator[](L"Scientists")  = WCF_SCIENTISTS;
        this->operator[](L"Scientology")  = WCF_SCIENTOLOGY;
        this->operator[](L"Sculpture")  = WCF_SCULPTURE;
        this->operator[](L"Semiconductors")  = WCF_SEMICONDUCTORS;
        this->operator[](L"Settlers")  = WCF_SETTLERS;
        this->operator[](L"Sexology")  = WCF_SEXOLOGY;
        this->operator[](L"Sexual_health")  = WCF_SEXUAL_HEALTH;
        this->operator[](L"Sexuality")  = WCF_SEXUALITY;
        this->operator[](L"Sexual_orientation")  = WCF_SEXUAL_ORIENTATION;
        this->operator[](L"Shinto")  = WCF_SHINTO;
        this->operator[](L"Shipping")  = WCF_SHIPPING;
        this->operator[](L"Signal_cables")  = WCF_SIGNAL_CABLES;
        this->operator[](L"Sikhism")  = WCF_SIKHISM;
        this->operator[](L"Skiing")  = WCF_SKIING;
        this->operator[](L"Slaves")  = WCF_SLAVES;
        this->operator[](L"Sleep")  = WCF_SLEEP;
        this->operator[](L"Social_groups")  = WCF_SOCIAL_GROUPS;
        this->operator[](L"Social_philosophy")  = WCF_SOCIAL_PHILOSOPHY;
        this->operator[](L"Social_scientists")  = WCF_SOCIAL_SCIENTISTS;
        this->operator[](L"Social_work")  = WCF_SOCIAL_WORK;
        this->operator[](L"Sociology")  = WCF_SOCIOLOGY;
        this->operator[](L"Software")  = WCF_SOFTWARE;
        this->operator[](L"Software_engineering")  = WCF_SOFTWARE_ENGINEERING;
        this->operator[](L"Sound_technology")  = WCF_SOUND_TECHNOLOGY;
        this->operator[](L"South_America")  = WCF_SOUTH_AMERICA;
        this->operator[](L"Space")  = WCF_SPACE;
        this->operator[](L"Spaceflight")  = WCF_SPACEFLIGHT;
        this->operator[](L"Spiritism")  = WCF_SPIRITISM;
        this->operator[](L"Spirituality")  = WCF_SPIRITUALITY;
        this->operator[](L"Sports")  = WCF_SPORTS;
        this->operator[](L"Statistical_theory")  = WCF_STATISTICAL_THEORY;
        this->operator[](L"Stochastic_processes")  = WCF_STOCHASTIC_PROCESSES;
        this->operator[](L"Storytelling")  = WCF_STORYTELLING;
        this->operator[](L"Structural_engineering")  = WCF_STRUCTURAL_ENGINEERING;
        this->operator[](L"Subcultures")  = WCF_SUBCULTURES;
        this->operator[](L"Summary_statistics")  = WCF_SUMMARY_STATISTICS;
        this->operator[](L"Surgery")  = WCF_SURGERY;
        this->operator[](L"Surveillance")  = WCF_SURVEILLANCE;
        this->operator[](L"Survival_analysis")  = WCF_SURVIVAL_ANALYSIS;
        this->operator[](L"Swimming")  = WCF_SWIMMING;
        this->operator[](L"Systems_engineering")  = WCF_SYSTEMS_ENGINEERING;
        this->operator[](L"Systems_theory")  = WCF_SYSTEMS_THEORY;
        this->operator[](L"Taoism")  = WCF_TAOISM;
        this->operator[](L"Technology_forecasting")  = WCF_TECHNOLOGY_FORECASTING;
        this->operator[](L"Telecommunications")  = WCF_TELECOMMUNICATIONS;
        this->operator[](L"Television")  = WCF_TELEVISION;
        this->operator[](L"Tennis")  = WCF_TENNIS;
        this->operator[](L"Tenrikyo")  = WCF_TENRIKYO;
        this->operator[](L"Terminology")  = WCF_TERMINOLOGY;
        this->operator[](L"Territories")  = WCF_TERRITORIES;
        this->operator[](L"Theatre")  = WCF_THEATRE;
        this->operator[](L"Theology")  = WCF_THEOLOGY;
        this->operator[](L"Theorems")  = WCF_THEOREMS;
        this->operator[](L"Time_series_analysis")  = WCF_TIME_SERIES_ANALYSIS;
        this->operator[](L"Tools")  = WCF_TOOLS;
        this->operator[](L"Towns")  = WCF_TOWNS;
        this->operator[](L"Track_and_field")  = WCF_TRACK_AND_FIELD;
        this->operator[](L"Traditions")  = WCF_TRADITIONS;
        this->operator[](L"Trigonometry")  = WCF_TRIGONOMETRY;
        this->operator[](L"Uncertainty_of_numbers")  = WCF_UNCERTAINTY_OF_NUMBERS;
        this->operator[](L"Unitarian_Universalism")  = WCF_UNITARIAN_UNIVERSALISM;
        this->operator[](L"Unsolved_problems_in_computer_science")  = WCF_UNSOLVED_PROBLEMS_IN_COMPUTER_SCIENCE;
        this->operator[](L"Urology")  = WCF_UROLOGY;
        this->operator[](L"Vehicles")  = WCF_VEHICLES;
        this->operator[](L"Veterinary_medicine")  = WCF_VETERINARY_MEDICINE;
        this->operator[](L"Villages")  = WCF_VILLAGES;
        this->operator[](L"Vitamins")  = WCF_VITAMINS;
        this->operator[](L"Walking")  = WCF_WALKING;
        this->operator[](L"War")  = WCF_WAR;
        this->operator[](L"Water_sports")  = WCF_WATER_SPORTS;
        this->operator[](L"Water_technology")  = WCF_WATER_TECHNOLOGY;
        this->operator[](L"Water_transport")  = WCF_WATER_TRANSPORT;
        this->operator[](L"Weight_training")  = WCF_WEIGHT_TRAINING;
        this->operator[](L"Whitewater_sports")  = WCF_WHITEWATER_SPORTS;
        this->operator[](L"Wicca")  = WCF_WICCA;
        this->operator[](L"Women")  = WCF_WOMEN;
        this->operator[](L"Yoga")  = WCF_YOGA;
        this->operator[](L"Zoroastrianism")  = WCF_ZOROASTRIANISM;
    }

};

wstring domainEnumToWstring(enum DomainTypes i_domain);
enum DomainTypes domainStringToEnum(string& i_domain);
enum DomainTypes domainWstringToEnum(const wstring &i_domain);

enum GoogleCategory googlecategoryWstringToEnum(const wstring &category);
enum GoogleCategoryShort googlecategoryshortWstringToEnum(const wstring &category);

enum WikiCategory wikicategoryWstringToEnum(const wstring &category);
enum WikiCategoryFull wikicategoryfullWstringToEnum(const wstring &category);

Tools::TrieDictionary<vector<wstring>, int> patentLabeltoTrie(const wstring& category);
vector<int> patentLabeltoVec(const wstring& category);
extern MapWstring2Category MAP;

}
#endif //_ENUMS_
