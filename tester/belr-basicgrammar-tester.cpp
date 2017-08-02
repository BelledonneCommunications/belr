/*
	belr-basicgrammar-tester.cpp
	Copyright (C) 2015  Belledonne Communications SARL

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "belr-tester.hpp"

using namespace::std;
using namespace::belr;


static void basicgrammar_save(void) {

  string grammarToParse = bc_tester_res("grammars/basicgrammar.txt");

  ABNFGrammarBuilder builder;

  //Read grammar put it in object grammar
  auto start = std::chrono::high_resolution_clock::now();
  shared_ptr<Grammar> grammar=builder.createFromAbnfFile(grammarToParse, make_shared<CoreRules>());
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsedFirst = finish - start;

  BC_ASSERT_FALSE(!grammar);
  
  //Save grammar
  string grammarDump = bc_tester_file("grammarDump.bin");
  grammar->createGrammarDump(grammarDump);

  //Load grammar
  start = std::chrono::high_resolution_clock::now();
  shared_ptr<Grammar> loadedGram = Grammar::loadVectRulesMap(grammarDump);
  finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsedSecond = finish - start;

  remove(grammarDump.c_str());

  BC_ASSERT_FALSE(!loadedGram);

  BC_ASSERT_TRUE(grammar->equal(loadedGram));

  BC_ASSERT_TRUE(elapsedFirst.count() > elapsedSecond.count());

//  cout << "Optimisation ratio : " << (elapsedFirst.count() / elapsedSecond.count())*100 << "%"<< endl;

}



static test_t tests[] = {
	TEST_NO_TAG("BASICGRAMMAR", basicgrammar_save),
};

test_suite_t grammar_simple_test_suite = {
	"BasicGrammar Save and Load",
	NULL,
	NULL,
	NULL,
	NULL,
	sizeof(tests) / sizeof(tests[0]),
	tests
};
