/*
	belcard-exclusiveselector-tester.cpp
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


static void char_xselector(void) {
  long savePosition = 0;


      std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
      std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
      std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;
      rcptrItEnd = (mRecognizerPointersLoaded.end());
      rcptrItBegin = (mRecognizerPointersLoaded.begin());

  //Selector with 4 characters
  	shared_ptr<Selector> selector = Foundation::selector(true);
  	shared_ptr<CharRecognizer> characterSel1 = Foundation::charRecognizer('a', false);
    shared_ptr<CharRecognizer> characterSel2 = Foundation::charRecognizer('b', false);
    shared_ptr<CharRecognizer> characterSel3 = Foundation::charRecognizer('c', false);
    shared_ptr<CharRecognizer> characterSel4 = Foundation::charRecognizer('d', false);
    selector->addRecognizer(characterSel1);
    selector->addRecognizer(characterSel2);
    selector->addRecognizer(characterSel3);
    selector->addRecognizer(characterSel4);

    BC_ASSERT_EQUAL(selector->feed(NULL, "a", 0) ,1, int, "%d");
    BC_ASSERT_EQUAL(selector->feed(NULL, "b", 0) ,1, int, "%d");
    BC_ASSERT_EQUAL(selector->feed(NULL, "c", 0) ,1, int, "%d");
    BC_ASSERT_EQUAL(selector->feed(NULL, "d", 0) ,1, int, "%d");

    ofstream ofichier ("test-sel.bin", ios::out | ios::app);
    selector->save(ofichier, savePosition);
    ofichier.close();

    ifstream ifichier ("test-sel.bin", ios::in);
    string content;
    getline(ifichier, content);
    std::vector<std::string> result = split(content, ' ');
    vector<string>::const_iterator i = result.begin();
    const shared_ptr<Selector> selectorLoaded = ExclusiveSelector::loadVect(i, rcptrItBegin, rcptrItEnd);
    ifichier.close();
    remove("test-sel.bin");


    BC_ASSERT_TRUE(selector->equal(selectorLoaded));

    BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "a", 0) ,1, int, "%d");
    BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "b", 0) ,1, int, "%d");
    BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "c", 0) ,1, int, "%d");
    BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "d", 0) ,1, int, "%d");


}
static void xsel_xselector(void) {
  long savePosition = 0;


      std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
      std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
      std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;
      rcptrItEnd = (mRecognizerPointersLoaded.end());
      rcptrItBegin = (mRecognizerPointersLoaded.begin());
  //Selector with an Exclusive Selector inside
  shared_ptr<Selector> selector = Foundation::selector(true);
  shared_ptr<Selector> ExclusiveSelector1 = Foundation::selector(true);
  shared_ptr<CharRecognizer> characterExclusiveSelector1 = Foundation::charRecognizer('a', false);
  shared_ptr<CharRecognizer> characterExclusiveSelector2 = Foundation::charRecognizer('b', false);
  shared_ptr<CharRecognizer> characterExclusiveSelector3 = Foundation::charRecognizer('c', false);
  shared_ptr<CharRecognizer> characterExclusiveSelector4 = Foundation::charRecognizer('d', false);
  ExclusiveSelector1->addRecognizer(characterExclusiveSelector1);
  ExclusiveSelector1->addRecognizer(characterExclusiveSelector2);
  ExclusiveSelector1->addRecognizer(characterExclusiveSelector3);
  ExclusiveSelector1->addRecognizer(characterExclusiveSelector4);
  selector->addRecognizer(ExclusiveSelector1);


      BC_ASSERT_EQUAL(selector->feed(NULL, "a", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(selector->feed(NULL, "b", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(selector->feed(NULL, "c", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(selector->feed(NULL, "d", 0) ,1, int, "%d");

      ofstream ofichier ("test-sel.bin", ios::out | ios::app);
      selector->save(ofichier, savePosition);
      ofichier.close();

      ifstream ifichier ("test-sel.bin", ios::in);
      string content;
      getline(ifichier, content);

      std::vector<std::string> result = split(content, ' ');
      vector<string>::const_iterator i = result.begin();

      const shared_ptr<Selector> selectorLoaded = ExclusiveSelector::loadVect(i, rcptrItBegin, rcptrItEnd);
      ifichier.close();
      remove("test-sel.bin");


      BC_ASSERT_TRUE(selector->equal(selectorLoaded));

      BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "a", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "b", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "c", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "d", 0) ,1, int, "%d");

}
static void sel_xselector(void) {
  long savePosition = 0;

      std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
      std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
      std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;
      rcptrItEnd = (mRecognizerPointersLoaded.end());
      rcptrItBegin = (mRecognizerPointersLoaded.begin());

  //Selector with an Exclusive Selector inside
  shared_ptr<Selector> selector = Foundation::selector(true);
  shared_ptr<Selector> Selector1 = Foundation::selector(false);
  shared_ptr<CharRecognizer> characterSelector1 = Foundation::charRecognizer('a', false);
  shared_ptr<CharRecognizer> characterSelector2 = Foundation::charRecognizer('b', false);
  shared_ptr<CharRecognizer> characterSelector3 = Foundation::charRecognizer('c', false);
  shared_ptr<CharRecognizer> characterSelector4 = Foundation::charRecognizer('d', false);
  Selector1->addRecognizer(characterSelector1);
  Selector1->addRecognizer(characterSelector2);
  Selector1->addRecognizer(characterSelector3);
  Selector1->addRecognizer(characterSelector4);
  selector->addRecognizer(Selector1);


      BC_ASSERT_EQUAL(selector->feed(NULL, "a", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(selector->feed(NULL, "b", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(selector->feed(NULL, "c", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(selector->feed(NULL, "d", 0) ,1, int, "%d");

      ofstream ofichier ("test-sel.bin", ios::out | ios::app);
      selector->save(ofichier, savePosition);
      ofichier.close();

      ifstream ifichier ("test-sel.bin", ios::in);
      string content;
      getline(ifichier, content);

      std::vector<std::string> result = split(content, ' ');
      vector<string>::const_iterator i = result.begin();

      const shared_ptr<Selector> selectorLoaded = ExclusiveSelector::loadVect(i, rcptrItBegin, rcptrItEnd);
      ifichier.close();
      remove("test-sel.bin");


      BC_ASSERT_TRUE(selector->equal(selectorLoaded));

      BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "a", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "b", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "c", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "d", 0) ,1, int, "%d");

}
static void loop_xselector(void) {
  long savePosition = 0;

      std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
      std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
      std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;
      rcptrItEnd = (mRecognizerPointersLoaded.end());
      rcptrItBegin = (mRecognizerPointersLoaded.begin());

  //Selector with a loop of one character
  	shared_ptr<Selector> selector = Foundation::selector(true);
  	shared_ptr<Loop> loopCar = Foundation::loop();
  	shared_ptr<CharRecognizer> characterLoop2 = Foundation::charRecognizer('a', false);
  	loopCar->setRecognizer(characterLoop2, 0, 3);
  	selector->addRecognizer(loopCar);

        BC_ASSERT_EQUAL(selector->feed(NULL, "a", 0) ,1, int, "%d");

        ofstream ofichier ("test-sel.bin", ios::out | ios::app);
        selector->save(ofichier, savePosition);
        ofichier.close();

        ifstream ifichier ("test-sel.bin", ios::in);
        string content;
        getline(ifichier, content);

        std::vector<std::string> result = split(content, ' ');
        vector<string>::const_iterator i = result.begin();

        const shared_ptr<Selector> selectorLoaded = ExclusiveSelector::loadVect(i, rcptrItBegin, rcptrItEnd);
        ifichier.close();
        remove("test-sel.bin");


        BC_ASSERT_TRUE(selector->equal(selectorLoaded));

        BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "a", 0) ,1, int, "%d");

}
static void sequence_xselector(void) {
  long savePosition = 0;


      std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
      std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
      std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;
      rcptrItEnd = (mRecognizerPointersLoaded.end());
      rcptrItBegin = (mRecognizerPointersLoaded.begin());
  //Selector with a sequence of 4 characters
  	shared_ptr<Selector> selector = Foundation::selector(true);
  	shared_ptr<Sequence> sequence1 = Foundation::sequence();
  	shared_ptr<CharRecognizer> characterSequence1 = Foundation::charRecognizer('a', false);
  	shared_ptr<CharRecognizer> characterSequence2 = Foundation::charRecognizer('b', false);
  	shared_ptr<CharRecognizer> characterSequence3 = Foundation::charRecognizer('c', false);
  	shared_ptr<CharRecognizer> characterSequence4 = Foundation::charRecognizer('d', false);
  	sequence1->addRecognizer(characterSequence1);
  	sequence1->addRecognizer(characterSequence2);
  	sequence1->addRecognizer(characterSequence3);
  	sequence1->addRecognizer(characterSequence4);
  	selector->addRecognizer(sequence1);


        BC_ASSERT_EQUAL(selector->feed(NULL, "abcd", 0) ,4, int, "%d");

        ofstream ofichier ("test-sel.bin", ios::out | ios::app);
        selector->save(ofichier, savePosition);
        ofichier.close();

        ifstream ifichier ("test-sel.bin", ios::in);
        string content;
        getline(ifichier, content);

        std::vector<std::string> result = split(content, ' ');
        vector<string>::const_iterator i = result.begin();

        const shared_ptr<Selector> selectorLoaded = ExclusiveSelector::loadVect(i, rcptrItBegin, rcptrItEnd);
        ifichier.close();
        remove("test-sel.bin");


        BC_ASSERT_TRUE(selector->equal(selectorLoaded));

        BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "abcd", 0) ,4, int, "%d");

}
static void literal_xselector(void) {
  long savePosition = 0;


      std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
      std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
      std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;
      rcptrItEnd = (mRecognizerPointersLoaded.end());
      rcptrItBegin = (mRecognizerPointersLoaded.begin());
  // Selector with a Literal
  	shared_ptr<Selector> selector = Foundation::selector(true);
  	const shared_ptr<Literal> stringBasic = dynamic_pointer_cast<Literal>(Utils::literal("abcd"));
  	selector->addRecognizer(stringBasic);

        BC_ASSERT_EQUAL(selector->feed(NULL, "abcd", 0) ,4, int, "%d");

        ofstream ofichier ("test-sel.bin", ios::out | ios::app);
        selector->save(ofichier, savePosition);
        ofichier.close();

        ifstream ifichier ("test-sel.bin", ios::in);
        string content;
        getline(ifichier, content);

        std::vector<std::string> result = split(content, ' ');
        vector<string>::const_iterator i = result.begin();

        const shared_ptr<Selector> selectorLoaded = ExclusiveSelector::loadVect(i, rcptrItBegin, rcptrItEnd);
        ifichier.close();
        remove("test-sel.bin");


        BC_ASSERT_TRUE(selector->equal(selectorLoaded));

        BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "abcd", 0) ,4, int, "%d");

}
static void xchar_xselector(void) {
  long savePosition = 0;

      std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
      std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
      std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;
      rcptrItEnd = (mRecognizerPointersLoaded.end());
      rcptrItBegin = (mRecognizerPointersLoaded.begin());

  //Selector with a CharRange
  	shared_ptr<Selector> selector = Foundation::selector(true);
  	const shared_ptr<CharRange> charRange1 = dynamic_pointer_cast<CharRange>(Utils::char_range(97, 100));
    const shared_ptr<CharRange> charRange2 = dynamic_pointer_cast<CharRange>(Utils::char_range(98, 101));
    const shared_ptr<CharRange> charRange3 = dynamic_pointer_cast<CharRange>(Utils::char_range(99, 102));
  	selector->addRecognizer(charRange1);
    selector->addRecognizer(charRange2);
    selector->addRecognizer(charRange3);

        BC_ASSERT_EQUAL(selector->feed(NULL, "a", 0) ,1, int, "%d");
        BC_ASSERT_EQUAL(selector->feed(NULL, "b", 0) ,1, int, "%d");
        BC_ASSERT_EQUAL(selector->feed(NULL, "c", 0) ,1, int, "%d");
        BC_ASSERT_EQUAL(selector->feed(NULL, "d", 0) ,1, int, "%d");

        ofstream ofichier ("test-sel.bin", ios::out | ios::app);
        selector->save(ofichier, savePosition);
        ofichier.close();

        ifstream ifichier ("test-sel.bin", ios::in);
        string content;
        getline(ifichier, content);

        std::vector<std::string> result = split(content, ' ');
        vector<string>::const_iterator i = result.begin();

        const shared_ptr<Selector> selectorLoaded = ExclusiveSelector::loadVect(i, rcptrItBegin, rcptrItEnd);
        ifichier.close();
        remove("test-sel.bin");


        BC_ASSERT_TRUE(selector->equal(selectorLoaded));

        BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "a", 0) ,1, int, "%d");
        BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "b", 0) ,1, int, "%d");
        BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "c", 0) ,1, int, "%d");
        BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "d", 0) ,1, int, "%d");



}
static void recp_xselector(void) {
  long savePosition = 0;


      std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
      std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
      std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;


  //Selector with a RecognizerPointer
  shared_ptr<Selector> selector = Foundation::selector(true);
  shared_ptr<RecognizerPointer> recognizptrCar = make_shared<RecognizerPointer>();
  recognizptrCar->setName("@rule");
  shared_ptr<CharRecognizer> characterRecognizerPtr2 = Foundation::charRecognizer('a', false);
  recognizptrCar->setPointed(characterRecognizerPtr2);
  selector->addRecognizer(recognizptrCar);
  mRecognizerPointersLoaded.push_back(recognizptrCar);
  rcptrItEnd = (mRecognizerPointersLoaded.end());
  rcptrItBegin = (mRecognizerPointersLoaded.begin());

      BC_ASSERT_EQUAL(selector->feed(NULL, "a", 0) ,1, int, "%d");

      ofstream ofichier ("test-sel.bin", ios::out | ios::app);
      selector->save(ofichier, savePosition);
      ofichier.close();

      ifstream ifichier ("test-sel.bin", ios::in);
      string content;
      getline(ifichier, content);

      std::vector<std::string> result = split(content, ' ');
      vector<string>::const_iterator i = result.begin();

      const shared_ptr<Selector> selectorLoaded = ExclusiveSelector::loadVect(i, rcptrItBegin, rcptrItEnd);
      ifichier.close();
      remove("test-sel.bin");


      BC_ASSERT_TRUE(selector->equal(selectorLoaded));

      BC_ASSERT_EQUAL(selectorLoaded->feed(NULL, "a", 0) ,1, int, "%d");


}


static test_t tests[] = {
	TEST_NO_TAG("CHARRECOGNIZERIN", char_xselector),
	TEST_NO_TAG("EXCLUSIVESSELECTORIN", xsel_xselector),
	TEST_NO_TAG("SELECTORIN", sel_xselector),
	TEST_NO_TAG("LOOPIN", loop_xselector),
	TEST_NO_TAG("SEQUENCEIN", sequence_xselector),
	TEST_NO_TAG("LITERALIN", literal_xselector),
  TEST_NO_TAG("CHARRANGEIN", xchar_xselector),
  TEST_NO_TAG("RECOGNIZERPOINTERIN", recp_xselector),

};

test_suite_t recognizer_exclusiveselector_test_suite = {
	"ExclusiveSelector",
	NULL,
	NULL,
	NULL,
	NULL,
	sizeof(tests) / sizeof(tests[0]),
	tests
};
