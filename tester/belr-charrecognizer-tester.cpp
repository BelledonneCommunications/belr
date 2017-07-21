/*
	belr-charrecognizer-tester.cpp
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


extern std::vector<std::string> split(const std::string &s, char delim);


static void char2_charrecognizer(void) {
	long savePosition = 0;

shared_ptr<CharRecognizer> character = Foundation::charRecognizer('b', false);
shared_ptr<CharRecognizer> character2 = Foundation::charRecognizer('a', false);


//feeding the original recognizer
BC_ASSERT_EQUAL(character->feed(NULL, "b", 0), 1, int, "%d");
BC_ASSERT_EQUAL(character2->feed(NULL, "a", 0), 1, int, "%d");

//SAVE
ofstream ofichier ("test-char.bin", ios::out | ios::app | ios::binary);
character->save(ofichier, savePosition);
character2->save(ofichier, savePosition);
ofichier.close();

//LOAD
ifstream ifichier ("test-char.bin", ios::in | ios::binary);
string content;
getline(ifichier, content);
std::vector<std::string> result = split(content, ' ');
vector<string>::const_iterator i = result.begin();
const shared_ptr<CharRecognizer> characterLoaded = CharRecognizer::loadVect(i);
const shared_ptr<CharRecognizer> characterLoaded2 = CharRecognizer::loadVect(i);
ifichier.close();
remove("test-char.bin");
//verify if the two recognizers are equal
BC_ASSERT_TRUE(character->equal(characterLoaded));
BC_ASSERT_TRUE(character2->equal(characterLoaded2));

//feeding the loaded recognizer
BC_ASSERT_EQUAL(characterLoaded->feed(NULL, "b", 0), 1, int, "%d");
BC_ASSERT_EQUAL(characterLoaded2->feed(NULL, "a", 0), 1, int, "%d");

}



static test_t tests[] = {
	TEST_NO_TAG("2CHAR", char2_charrecognizer),
};

test_suite_t recognizer_char_test_suite = {
	"CharRecognizer",
	NULL,
	NULL,
	NULL,
	NULL,
	sizeof(tests) / sizeof(tests[0]),
	tests
};
