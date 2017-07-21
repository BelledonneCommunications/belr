
  /*
  	belr-literal-tester.cpp
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


  static void char4_literal(void) {
    long savePosition = 0;

    const shared_ptr<Literal> chaine = dynamic_pointer_cast<Literal>(Utils::literal("abcd"));

    BC_ASSERT_EQUAL(chaine->feed(NULL, "abcd", 0) ,4, int, "%d");

    ofstream ofichier ("test-lit.bin", ios::out | ios::app);
    chaine->save(ofichier ,savePosition);
    ofichier.close();

    ifstream ifichier ("test-lit.bin", ios::in);
    string content;
    getline(ifichier, content);

    std::vector<std::string> result = split(content, ' ');
    vector<string>::const_iterator i = result.begin();

  	const shared_ptr<Literal> chaine_loaded = Literal::loadVect(i);
    ifichier.close();

    remove("test-lit.bin");

  	BC_ASSERT_TRUE(chaine->equal(chaine_loaded));
    BC_ASSERT_EQUAL(chaine_loaded->feed(NULL, "abcd",0) ,4,int ,"%d");




  }



  static test_t tests[] = {
  	TEST_NO_TAG("4CHAR", char4_literal),
  };

  test_suite_t recognizer_literal_test_suite = {
  	"Literal",
  	NULL,
  	NULL,
  	NULL,
  	NULL,
  	sizeof(tests) / sizeof(tests[0]),
  	tests
  };
