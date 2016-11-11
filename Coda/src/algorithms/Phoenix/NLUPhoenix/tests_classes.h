/**
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

#if 0

class GrammarTest
{
public:
	GrammarTest(shared_ptr<Grammar> G)
	{
		g = G;
	}

	bool Test1()
	{
		shared_ptr<GrammarNet> net = g->findNet(L"[main_device]");
		shared_ptr<GrammarNode> start = net->GetStartNode();
		unordered_set<wstring> possibleFeats;
		possibleFeats.insert(L"���");
		possibleFeats.insert(L"���");
		for(auto arc_p = start->GetArcIterator(); arc_p != start->GetArcEndIterator(); arc_p++)
		{
			if((*arc_p)->GetType() != TWORD)
			{
				return false;
			}

			if (!DataTools::set_equal(std::dynamic_pointer_cast<WordArc>(*arc_p)->GetFeatures(), possibleFeats)) return false;
		}

		return true;
	}

	bool Test2()
	{
		shared_ptr<GrammarNet> net = g->findNet(L"[action_finish]");
		shared_ptr<GrammarNode> start = net->GetStartNode();
		unordered_set<wstring> possibleFeats;
		possibleFeats.insert(L"���");
		possibleFeats.insert(L"���");
		possibleFeats.insert(L"������.��");
		possibleFeats.insert(L"������.1-�.��");
		unordered_set<wstring> possibleFeats2;
		possibleFeats2.insert(L"=");
		int i = 0, j = 0;
		for(auto arc_p = start->GetArcIterator(); arc_p != start->GetArcEndIterator(); arc_p++)
		{
			if((*arc_p)->GetType() != TWORD)
			{
				return false;
			}
			unordered_set<wstring> ret = std::dynamic_pointer_cast<WordArc>(*arc_p)->GetFeatures();
			if (DataTools::set_equal(ret, possibleFeats)) i++;
			else if(DataTools::set_equal(ret, possibleFeats2)) j++;
			else return false;
		}

		return i==4 && j == 3;
	}

	bool Test3()
	{
		shared_ptr<GrammarNet> net = g->findNet(L"[remote_control]");
		shared_ptr<GrammarNode> start = net->GetStartNode();
		unordered_set<wstring> possibleFeats;
		int i = 0, j = 0;
		for(auto arc_p = start->GetArcIterator(); arc_p != start->GetArcEndIterator(); arc_p++)
		{
			if((*arc_p)->GetType() != TWORD)
			{
				return false;
			}

			if (!DataTools::set_equal(std::dynamic_pointer_cast<WordArc>(*arc_p)->GetFeatures(), possibleFeats)) return false;
		}

		return true;
	}

	shared_ptr<GrammarNet> GetNet(wstring netName)
	{
		return g->findNet(netName);
	}

private:
	shared_ptr<Grammar> g;
};

class ParserTest
{
public:
	ParserTest(PhoenixParser * _parser)
	{
		parser = _parser;
	}

	bool test1()
	{
		wstring utterance = L"���������";

		parser->createWordSequence(utterance);

		unordered_map<wstring, wstring> r4;
		r4[PrivateTagset::CAT_POS] = SynTagRusTagset::POS_VERB;
		r4[PrivateTagset::CAT_ASPECT] = SynTagRusTagset::A_PERF;
		r4[PrivateTagset::CAT_MODE] = SynTagRusTagset::M_IMPER;
		r4[PrivateTagset::CAT_NUM] = SynTagRusTagset::N_SING;
		r4[PrivateTagset::CAT_PERSON] = SynTagRusTagset::P_2;

		return parser->dis_words[0].GetAllFeatures() == r4;
	}

	bool test2()
	{
		wstring utterance = L"���������";

		parser->createWordSequence(utterance);

		GrammarTest gt(parser->gram);

		shared_ptr<GrammarNet> net = gt.GetNet(L"[action_turn_off]");

		shared_ptr<GrammarNode> start = net->GetStartNode();

		for(auto arc_p = start->GetArcIterator(); arc_p != start->GetArcEndIterator(); arc_p++)
		{
			if((*arc_p)->GetType() != TWORD)
			{
				return false;
			}
			if(parser->matchWord(std::dynamic_pointer_cast<WordArc>(*arc_p)->GetArgument(),std::dynamic_pointer_cast<WordArc>(*arc_p)->GetFeatures(), 1))
			{
				return true;
			}
		}

		return false;
	}

	bool test3()
	{
		wstring utterance = L"��������";

		parser->createWordSequence(utterance);

		GrammarTest gt(parser->gram);

		shared_ptr<GrammarNet> net = gt.GetNet(L"[action_turn_off]");

		shared_ptr<GrammarNode> start = net->GetStartNode();

		for(auto arc_p = start->GetArcIterator(); arc_p != start->GetArcEndIterator(); arc_p++)
		{
			if((*arc_p)->GetType() != TWORD)
			{
				return false;
			}
			if(parser->matchWord(std::dynamic_pointer_cast<WordArc>(*arc_p)->GetArgument(),std::dynamic_pointer_cast<WordArc>(*arc_p)->GetFeatures(), 1))
			{
				return true;
			}
		}

		return false;
	}

	bool test4()
	{
		wstring utterance = L"��������";

		parser->createWordSequence(utterance);

		GrammarTest gt(parser->gram);

		shared_ptr<GrammarNet> net = gt.GetNet(L"[action_turn_off]");

		shared_ptr<GrammarNode> start = net->GetStartNode();

		for(auto arc_p = start->GetArcIterator(); arc_p != start->GetArcEndIterator(); arc_p++)
		{
			if((*arc_p)->GetType() != TWORD)
			{
				return false;
			}
			if(parser->matchWord(std::dynamic_pointer_cast<WordArc>(*arc_p)->GetArgument(),std::dynamic_pointer_cast<WordArc>(*arc_p)->GetFeatures(), 1))
			{
				return true;
			}
		}

		return false;
	}

private:
	PhoenixParser * parser;
};

TEST(SynTagRusTagInterpreterTest, Basic)
{
	vector<wstring> t1;
	t1.push_back(L"NUM");

	vector<wstring> t2;
	t2.push_back(L"PART");

	vector<wstring> t3;
	t3.push_back(L"CONJ");

	vector<wstring> t4;
	t4.push_back(L"PR");

	unordered_map<wstring, wstring> r1;
	r1[PrivateTagset::CAT_POS] = SynTagRusTagset::POS_NUMR;

	unordered_map<wstring, wstring> r2;
	r2[PrivateTagset::CAT_POS] = SynTagRusTagset::POS_PRCL;

	unordered_map<wstring, wstring> r3;
	r3[PrivateTagset::CAT_POS] = SynTagRusTagset::POS_CONJ;

	unordered_map<wstring, wstring> r4;
	r4[PrivateTagset::CAT_POS] = SynTagRusTagset::POS_PREP;

	EXPECT_EQ(true, SynTagRusTagInterpreter::GetInstance().ProcessTags(t1) == r1);
	EXPECT_EQ(true, SynTagRusTagInterpreter::GetInstance().ProcessTags(t2) == r2);
	EXPECT_EQ(true, SynTagRusTagInterpreter::GetInstance().ProcessTags(t3) == r3);
	EXPECT_EQ(true, SynTagRusTagInterpreter::GetInstance().ProcessTags(t4) == r4);
}

TEST(SynTagRusTagInterpreterTest, NamesAndAdv)
{
	vector<wstring> t1;
	t1.push_back(L"S");
	t1.push_back(L"��");
	t1.push_back(L"���");
	t1.push_back(L"���");
	t1.push_back(L"����");

	vector<wstring> t2;
	t2.push_back(L"A");
	t2.push_back(L"��");
	t2.push_back(L"���");
	t2.push_back(L"���");
	t2.push_back(L"����");
	vector<wstring> t3;
	t3.push_back(L"A");
	t3.push_back(L"��");
	t3.push_back(L"��");
	t3.push_back(L"����");
	vector<wstring> t4;
	t4.push_back(L"NUM");
	t4.push_back(L"��");
	t4.push_back(L"���");
	t4.push_back(L"���");
	t4.push_back(L"����");

	vector<wstring> t5;
	t5.push_back(L"ADV");
	t5.push_back(L"����");
	t5.push_back(L"����");

	unordered_map<wstring, wstring> r1;
	r1[PrivateTagset::CAT_POS] = SynTagRusTagset::POS_NOUN;
	r1[PrivateTagset::CAT_NUM] = SynTagRusTagset::N_SING;
	r1[PrivateTagset::CAT_GEN] = SynTagRusTagset::G_MASC;
	r1[PrivateTagset::CAT_CASE] = SynTagRusTagset::C_GEN1;
	r1[PrivateTagset::CAT_ANIM] = SynTagRusTagset::ANIM_N;

	unordered_map<wstring, wstring> r2;
	r2[PrivateTagset::CAT_POS] = SynTagRusTagset::POS_ADJ;
	r2[PrivateTagset::CAT_NUM] = SynTagRusTagset::N_SING;
	r2[PrivateTagset::CAT_GEN] = SynTagRusTagset::G_MASC;
	r2[PrivateTagset::CAT_CASE] = SynTagRusTagset::C_ACC;
	r2[PrivateTagset::CAT_ANIM] = SynTagRusTagset::ANIM_N;
	r2[PrivateTagset::CAT_SHORT] = SynTagRusTagset::D_FULL;
	r2[PrivateTagset::CAT_DEG] = SynTagRusTagset::DEG_POSITIVE;

	unordered_map<wstring, wstring> r3;
	r3[PrivateTagset::CAT_POS] = SynTagRusTagset::POS_ADJ;
	r3[PrivateTagset::CAT_NUM] = SynTagRusTagset::N_SING;
	r3[PrivateTagset::CAT_SHORT] = SynTagRusTagset::D_SHORT;
	r3[PrivateTagset::CAT_GEN] = SynTagRusTagset::G_NEUT;

	unordered_map<wstring, wstring> r4;
	r4[PrivateTagset::CAT_POS] = SynTagRusTagset::POS_NUMR;
	r4[PrivateTagset::CAT_NUM] = SynTagRusTagset::N_SING;
	r4[PrivateTagset::CAT_GEN] = SynTagRusTagset::G_MASC;
	r4[PrivateTagset::CAT_CASE] = SynTagRusTagset::C_ACC;
	r4[PrivateTagset::CAT_ANIM] = SynTagRusTagset::ANIM_N;

	unordered_map<wstring, wstring> r5;
	r5[PrivateTagset::CAT_POS] = SynTagRusTagset::POS_ADVB;
	r5[PrivateTagset::CAT_DEG] = SynTagRusTagset::DEG_COMP;

	EXPECT_EQ(true, SynTagRusTagInterpreter::GetInstance().ProcessTags(t1) == r1);
	EXPECT_EQ(true, SynTagRusTagInterpreter::GetInstance().ProcessTags(t2) == r2);
	EXPECT_EQ(true, SynTagRusTagInterpreter::GetInstance().ProcessTags(t3) == r3);
	EXPECT_EQ(true, SynTagRusTagInterpreter::GetInstance().ProcessTags(t4) == r4);
	EXPECT_EQ(true, SynTagRusTagInterpreter::GetInstance().ProcessTags(t5) == r5);
}

TEST(SynTagRusTagInterpreterTest, Verb)
{
	vector<wstring> t1;
	t1.push_back(L"V");
	t1.push_back(L"�����");
	t1.push_back(L"�����");
	t1.push_back(L"������");
	t1.push_back(L"��");
	t1.push_back(L"3-�");

	vector<wstring> t2;
	t2.push_back(L"V");
	t2.push_back(L"�����");
	t2.push_back(L"����");
	t2.push_back(L"������");
	t2.push_back(L"��");
	t2.push_back(L"���");
	t2.push_back(L"���");
	t2.push_back(L"��");

	vector<wstring> t3;
	t3.push_back(L"V");
	t3.push_back(L"���");
	t3.push_back(L"���");

	vector<wstring> t4;
	t4.push_back(L"V");
	t4.push_back(L"���");
	t4.push_back(L"���");
	t4.push_back(L"��");
	t4.push_back(L"2-�");

	unordered_map<wstring, wstring> r1;
	r1[PrivateTagset::CAT_POS] = SynTagRusTagset::POS_VERB;
	r1[PrivateTagset::CAT_ASPECT] = SynTagRusTagset::A_IMPRF;
	r1[PrivateTagset::CAT_MODE] = SynTagRusTagset::M_INDC;
	r1[PrivateTagset::CAT_TENSE] = SynTagRusTagset::TN_NONPAST;
	r1[PrivateTagset::CAT_NUM] = SynTagRusTagset::N_SING;
	r1[PrivateTagset::CAT_PERSON] = SynTagRusTagset::P_3;
	r1[PrivateTagset::CAT_TIME] = SynTagRusTagset::TIME_PRESENT;
	r1[PrivateTagset::CAT_VOICE] = SynTagRusTagset::V_ACT;
	r1[PrivateTagset::CAT_VERB_FORM] = SynTagRusTagset::VT_FIN;

	unordered_map<wstring, wstring> r2;
	r2[PrivateTagset::CAT_POS] = SynTagRusTagset::POS_VERB;
	r2[PrivateTagset::CAT_ASPECT] = SynTagRusTagset::A_IMPRF;
	r2[PrivateTagset::CAT_VERB_FORM] = SynTagRusTagset::VT_PRT;
	r2[PrivateTagset::CAT_TENSE] = SynTagRusTagset::TN_NONPAST;
	r2[PrivateTagset::CAT_NUM] = SynTagRusTagset::N_SING;
	r2[PrivateTagset::CAT_GEN] = SynTagRusTagset::G_MASC;
	r2[PrivateTagset::CAT_CASE] = SynTagRusTagset::C_ACC;
	r2[PrivateTagset::CAT_ANIM] = SynTagRusTagset::ANIM_Y;
	r2[PrivateTagset::CAT_TIME] = SynTagRusTagset::TIME_PRESENT;
	r2[PrivateTagset::CAT_VOICE] = SynTagRusTagset::V_ACT;

	unordered_map<wstring, wstring> r3;
	r3[PrivateTagset::CAT_POS] = SynTagRusTagset::POS_VERB;
	r3[PrivateTagset::CAT_ASPECT] = SynTagRusTagset::A_PERF;
	r3[PrivateTagset::CAT_VERB_FORM] = SynTagRusTagset::VT_INF;

	unordered_map<wstring, wstring> r4;
	r4[PrivateTagset::CAT_POS] = SynTagRusTagset::POS_VERB;
	r4[PrivateTagset::CAT_ASPECT] = SynTagRusTagset::A_PERF;
	r4[PrivateTagset::CAT_MODE] = SynTagRusTagset::M_IMPER;
	r4[PrivateTagset::CAT_NUM] = SynTagRusTagset::N_SING;
	r4[PrivateTagset::CAT_PERSON] = SynTagRusTagset::P_2;

	SynTagRusTagInterpreter::GetInstance().ProcessTags(vector<wstring>());

	EXPECT_EQ(true, SynTagRusTagInterpreter::GetInstance().ProcessTags(t1) == r1);
	EXPECT_EQ(true, SynTagRusTagInterpreter::GetInstance().ProcessTags(t2) == r2);
	EXPECT_EQ(true, SynTagRusTagInterpreter::GetInstance().ProcessTags(t3) == r3);
	EXPECT_EQ(true, SynTagRusTagInterpreter::GetInstance().ProcessTags(t4) == r4);
}

TEST(ReadGrammar, ConstraintFeaturesFromNet)
{
	wstring dict_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\base.dic";
	wstring grammar_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\RUS.net";
	wstring frames_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\forms";
	shared_ptr<Grammar> G = shared_ptr<Grammar>(new Grammar(dict_file, grammar_file, frames_file));
	GrammarTest gTest(G);
	EXPECT_EQ(true, gTest.Test1());
}

TEST(ReadGrammar, ConstraintFeaturesFromNetAndPrivate)
{
	wstring dict_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\base.dic";
	wstring grammar_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\RUS.net";
	wstring frames_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\forms";
	shared_ptr<Grammar> G = shared_ptr<Grammar>(new Grammar(dict_file, grammar_file, frames_file));
	GrammarTest gTest(G);
	EXPECT_EQ(true, gTest.Test2());
}

TEST(ReadGrammar, NetWithNoConstriants)
{
	wstring dict_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\base.dic";
	wstring grammar_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\RUS.net";
	wstring frames_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\forms";
	shared_ptr<Grammar> G = shared_ptr<Grammar>(new Grammar(dict_file, grammar_file, frames_file));
	GrammarTest gTest(G);
	EXPECT_EQ(true, gTest.Test3());
}

TEST(CreateWords, Disambiguate)
{
	wstring dict_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\base.dic";
	wstring grammar_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\RUS.net";
	wstring frames_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\forms";
	shared_ptr<Grammar> G = shared_ptr<Grammar>(new Grammar(dict_file, grammar_file, frames_file));
	shared_ptr<SlotTrellis> GS = shared_ptr<SlotTrellis>(new SlotTrellis(G));

	PhoenixParser p(G, GS);
	ParserTest pt(&p);

	EXPECT_EQ(true, pt.test1());
}

TEST(MatchWord, MatchWithFeatures)
{
	wstring dict_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\base.dic";
	wstring grammar_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\RUS.net";
	wstring frames_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\forms";
	shared_ptr<Grammar> G = shared_ptr<Grammar>(new Grammar(dict_file, grammar_file, frames_file));
	shared_ptr<SlotTrellis> GS = shared_ptr<SlotTrellis>(new SlotTrellis(G));

	PhoenixParser p(G, GS);
	ParserTest pt(&p);

	EXPECT_EQ(true, pt.test2());
	EXPECT_EQ(false, pt.test3());
	EXPECT_EQ(false, pt.test4());
}

TEST(ParseTest, ParsingCommands)
{
	wstring dict_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\base.dic";
	wstring grammar_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\RUS.net";
	wstring frames_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\CompileGrammar\\forms";
	shared_ptr<Grammar> G = shared_ptr<Grammar>(new Grammar(dict_file, grammar_file, frames_file));
	shared_ptr<SlotTrellis> GS = shared_ptr<SlotTrellis>(new SlotTrellis(G));

	PhoenixParser parser(G, GS);

	wstring target = L"���������";

	ParseResult ret_code = parser.ParseLine(target);
	EXPECT_EQ(SUCCESS, ret_code);
	parser.Reset();

	target = L"����������";
	ret_code = parser.ParseLine(target);
	EXPECT_EQ(SUCCESS, ret_code);
	parser.Reset();

	target = L"����������";
	ret_code = parser.ParseLine(target);
	EXPECT_EQ(NO_PARSE, ret_code);
	parser.Reset();

	target = L"��������";
	ret_code = parser.ParseLine(target);
	EXPECT_EQ(NO_PARSE, ret_code);
	parser.Reset();

	target = L"";
	ret_code = parser.ParseLine(target);
	EXPECT_EQ(NO_PARSE, ret_code);
	parser.Reset();

	target = L" ";
	ret_code = parser.ParseLine(target);
	EXPECT_EQ(NO_PARSE, ret_code);
	parser.Reset();
}

TEST(ParseTest, ParsingQuestions)
{
	wstring dict_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\PhoenixCompiler\\base.dic";
	wstring grammar_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\PhoenixCompiler\\RUS.net";
	wstring frames_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\PhoenixCompiler\\forms";
	shared_ptr<Grammar> G = shared_ptr<Grammar>(new Grammar(dict_file, grammar_file, frames_file));
	shared_ptr<SlotTrellis> GS = shared_ptr<SlotTrellis>(new SlotTrellis(G));

	PhoenixParser parser(G, GS);

	wstring target = L"��� ���������� �� ������� ������";

	ParseResult ret_code = parser.ParseLine(target);
	ASSERT_EQ(SUCCESS, ret_code);

	ASSERT_EQ(1, parser.GetNumberOfParses());

	shared_ptr<Parse> prs = parser.GetAllParses()[0];

	ASSERT_EQ(false, prs->IsFragmented());

	wstring frameName = prs->GetFrameSequence()[0];

	shared_ptr<Frame> f = G->GetFrameByName(frameName);

	FilledFrame ff(f, prs->GetSlotSequence());

	unordered_set<wstring> testNets;

	testNets.insert(L"[Topic]");
	testNets.insert(L"[Channel]");
	testNets.insert(L"[Query]");

	for(auto slot_p = ff.GetSlotIterator(); slot_p != ff.GetSlotEndIterator(); slot_p++)
	{
		ASSERT_EQ(false, testNets.find((*slot_p)->GetNet()->GetName()) == testNets.end());
		testNets.erase((*slot_p)->GetNet()->GetName());
	}
	EXPECT_EQ(true, testNets.empty());
}

class QueryAssemblerTest
{
public:
	bool Test1()
	{
		sqlite3 * db;

		int rc = sqlite3_open("afisha.db", &db);

		if(rc)
		{
			throw -1;
		}

		QueryAssembler::GetInstance().SetResource(db);

		wstring a = QueryAssembler::GetInstance().getQueryTemplateFromDB(L"RusTVQuery", db);

		sqlite3_close(db);

		unordered_map<wstring, wstring> b = QueryAssembler::GetInstance().parseTemplate(a);

		if (b.find(L"[Topic]") == b.end())
		{
			throw -1;
		}

		return b.empty();
	}
};

#if 0

TEST(NetEvaluationTest, SimpleEvaluation)
{
	wstring dict_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\PhoenixCompiler\\base.dic";
	wstring grammar_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\PhoenixCompiler\\RUS.net";
	wstring frames_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\PhoenixCompiler\\forms";
	shared_ptr<Grammar> G = shared_ptr<Grammar>(new Grammar(dict_file, grammar_file, frames_file));
	shared_ptr<SlotTrellis> GS = shared_ptr<SlotTrellis>(new SlotTrellis(G));

	PhoenixParser parser(G, GS);

	wstring target = L"��� ���������� �� ������� ������";

	ParseResult ret_code = parser.ParseLine(target);
	ASSERT_EQ(SUCCESS, ret_code);

	shared_ptr<Parse> prs = parser.GetAllParses()[0];

	wstring frameName = prs->GetFrameSequence()[0];

	shared_ptr<Frame> f = G->GetFrameByName(frameName);

	FilledFrame ff(f, prs->GetSlotSequence());

	wstring res;
	NetEvaluator::SetResource(L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\NLUPhoenix\\net_interp");

	vector<wstring> values;
	values.push_back(L"what");
	values.push_back(L"schedule");
	values.push_back(L"������ �����");
	int i = 0;

	for(auto slot_p = ff.GetSlotIterator(); slot_p != ff.GetSlotEndIterator(); slot_p++)
	{
		res = NetEvaluator::GetInstance().Evaluate((*slot_p)->GetMatch()).at(L"VALUE");
		EXPECT_EQ(true, values[i] == res);
		i++;
	}
}

#endif

TEST(QueryAssemblerTest, GetTemplate)
{

	QueryAssemblerTest qat;
	EXPECT_EQ(false, qat.Test1());

}

TEST(QueryAssemblerTest, GetQuery)
{
	wstring dict_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\PhoenixCompiler\\base.dic";
	wstring grammar_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\PhoenixCompiler\\RUS.net";
	wstring frames_file = L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\PhoenixCompiler\\forms";
	shared_ptr<Grammar> G = shared_ptr<Grammar>(new Grammar(dict_file, grammar_file, frames_file));
	shared_ptr<SlotTrellis> GS = shared_ptr<SlotTrellis>(new SlotTrellis(G));

	PhoenixParser parser(G, GS);

	wstring target = L"��� ���������� �� ������� ������";

	ParseResult ret_code = parser.ParseLine(target);
	ASSERT_EQ(SUCCESS, ret_code);

	shared_ptr<Parse> prs = parser.GetAllParses()[0];

	wstring frameName = prs->GetFrameSequence()[0];

	shared_ptr<Frame> f = G->GetFrameByName(frameName);

	FilledFrame ff(f, prs->GetSlotSequence());

	sqlite3 * db;

	int rc = sqlite3_open("afisha.db", &db);

	if(rc)
	{
		throw -1;
	}

	QueryAssembler::GetInstance().SetResource(db);

	NetEvaluator evaluator(L"C:\\Users\\m.kudinov\\Documents\\PhoenixExperimental\\NLUPhoenix\\NLUPhoenix\\net_interp");

	wstring a = QueryAssembler::GetInstance().CreateQuery(ff, evaluator);

	sqlite3_close(db);

	EXPECT_EQ(wstring(L"SELECT * FROM schedule WHERE 1=1 AND channel_name = '������ �����'"), a);
}

#endif

