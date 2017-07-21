  /*
  	belr-charrange-tester.cpp
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


  static void char4_charrange(void) {
    long savePosition = 0;

    //Create two charrecognizers
    const shared_ptr<CharRange> stringBasic = dynamic_pointer_cast<CharRange>(Utils::char_range(97, 100));

    BC_ASSERT_EQUAL(stringBasic->feed(NULL, "a", 0), 1, int, "%d");
    BC_ASSERT_EQUAL(stringBasic->feed(NULL, "b", 0), 1, int, "%d");
    BC_ASSERT_EQUAL(stringBasic->feed(NULL, "c", 0), 1, int, "%d");
    BC_ASSERT_EQUAL(stringBasic->feed(NULL, "d", 0), 1, int, "%d");


    //SAVE AND LOAD
    ofstream ofichier ("test-char_range.bin", ios::out | ios::app);
    stringBasic->save(ofichier, savePosition);
    ofichier.close();

    ifstream ifichier("test-char_range.bin", ios::in);
    string contenu;  // déclaration d'une chaîne qui contiendra la ligne lue
    getline(ifichier, contenu);  // on met dans "contenu" la ligne

    std::vector<std::string> result = split(contenu, ' ');
    vector<string>::const_iterator i = result.begin();

    const shared_ptr<CharRange> stringBasicLoaded = CharRange::loadVect(i);
    ifichier.close();
    remove("test-char_range.bin");

    BC_ASSERT_TRUE(stringBasic->equal(stringBasicLoaded));


    BC_ASSERT_EQUAL(stringBasicLoaded->feed(NULL, "a",0), 1, int, "%d");
    BC_ASSERT_EQUAL(stringBasicLoaded->feed(NULL, "b",0), 1, int, "%d");
    BC_ASSERT_EQUAL(stringBasicLoaded->feed(NULL, "c",0), 1, int, "%d");
    BC_ASSERT_EQUAL(stringBasicLoaded->feed(NULL, "d",0), 1, int, "%d");



  }



  static test_t tests[] = {
  	TEST_NO_TAG("4CHAR", char4_charrange),
  };

  test_suite_t recognizer_charrange_test_suite = {
  	"CharRange",
  	NULL,
  	NULL,
  	NULL,
  	NULL,
  	sizeof(tests) / sizeof(tests[0]),
  	tests
  };
