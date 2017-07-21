/*
	belcard-loop-tester.cpp
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








static void char_loop(void) {
  long savePosition = 0;

  //Loop with one CharRecognizer
  shared_ptr<Loop> loopCar = Foundation::loop();
  shared_ptr<CharRecognizer> characterLoop1 = Foundation::charRecognizer('a', false);
  loopCar->setRecognizer(characterLoop1, 0, 3);
  BC_ASSERT_EQUAL(loopCar->feed(NULL, "a", 0) ,1, int, "%d");

    ofstream ofichier ("test-loop.bin", ios::out | ios::app);
    loopCar->save(ofichier, savePosition);
    ofichier.close();

    ifstream ifichier ("test-loop.bin", ios::in);
    string content;
    getline(ifichier, content);

    std::vector<std::string> result = split(content, ' ');
    vector<string>::const_iterator it = result.begin();
    const shared_ptr<Loop> loopCarLoaded = Loop::loadVect(it);
    ifichier.close();
    remove("test-loop.bin");


    BC_ASSERT_TRUE(loopCar->equal(loopCarLoaded));

    BC_ASSERT_EQUAL(loopCarLoaded->feed(NULL, "a", 0) ,1, int, "%d");

}
static void xsel_loop(void) {
  long savePosition = 0;


  //Loop with one ExclusiveSelector
  shared_ptr<Loop> loopExSel = Foundation::loop();
  shared_ptr<Selector> exSelector1 = Foundation::selector(true);
  shared_ptr<CharRecognizer> characterExSel1 = Foundation::charRecognizer('a', false);
  shared_ptr<CharRecognizer> characterExSel2 = Foundation::charRecognizer('b', false);
  shared_ptr<CharRecognizer> characterExSel3 = Foundation::charRecognizer('c', false);
  shared_ptr<CharRecognizer> characterExSel4 = Foundation::charRecognizer('d', false);
  exSelector1->addRecognizer(characterExSel1);
  exSelector1->addRecognizer(characterExSel2);
  exSelector1->addRecognizer(characterExSel3);
  exSelector1->addRecognizer(characterExSel4);
  loopExSel->setRecognizer(exSelector1, 0, -1);


      BC_ASSERT_EQUAL(loopExSel->feed(NULL, "a", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(loopExSel->feed(NULL, "b", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(loopExSel->feed(NULL, "c", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(loopExSel->feed(NULL, "d", 0) ,1, int, "%d");

      ofstream ofichier ("test-loop.bin", ios::out | ios::app);
      loopExSel->save(ofichier, savePosition);
      ofichier.close();

      ifstream ifichier ("test-loop.bin", ios::in);
      string content;
      getline(ifichier, content);

      std::vector<std::string> result = split(content, ' ');
      vector<string>::const_iterator i = result.begin();

      const shared_ptr<Loop> loopExSelLoaded = Loop::loadVect(i);
      ifichier.close();
      remove("test-loop.bin");


      BC_ASSERT_TRUE(loopExSel->equal(loopExSelLoaded));

      BC_ASSERT_EQUAL(loopExSelLoaded->feed(NULL, "a", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(loopExSelLoaded->feed(NULL, "b", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(loopExSelLoaded->feed(NULL, "c", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(loopExSelLoaded->feed(NULL, "d", 0) ,1, int, "%d");

}
static void sel_loop(void) {
  long savePosition = 0;


  //Loop with one Selector
  shared_ptr<Loop> loopSel = Foundation::loop();
  shared_ptr<Selector> selector1 = Foundation::selector(false);
  shared_ptr<CharRecognizer> characterSel1 = Foundation::charRecognizer('a', false);
  shared_ptr<CharRecognizer> characterSel2 = Foundation::charRecognizer('b', false);
  shared_ptr<CharRecognizer> characterSel3 = Foundation::charRecognizer('c', false);
  shared_ptr<CharRecognizer> characterSel4 = Foundation::charRecognizer('d', false);
  selector1->addRecognizer(characterSel1);
  selector1->addRecognizer(characterSel2);
  selector1->addRecognizer(characterSel3);
  selector1->addRecognizer(characterSel4);
  loopSel->setRecognizer(selector1, 0, 3);


      BC_ASSERT_EQUAL(loopSel->feed(NULL, "a", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(loopSel->feed(NULL, "b", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(loopSel->feed(NULL, "c", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(loopSel->feed(NULL, "d", 0) ,1, int, "%d");

      ofstream ofichier ("test-loop.bin", ios::out | ios::app);
      loopSel->save(ofichier, savePosition);
      ofichier.close();

      ifstream ifichier ("test-loop.bin", ios::in);
      string content;
      getline(ifichier, content);

      std::vector<std::string> result = split(content, ' ');
      vector<string>::const_iterator i = result.begin();

      const shared_ptr<Loop> loopSelLoaded = Loop::loadVect(i);
      ifichier.close();
      remove("test-loop.bin");


      BC_ASSERT_TRUE(loopSel->equal(loopSelLoaded));

      BC_ASSERT_EQUAL(loopSelLoaded->feed(NULL, "a", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(loopSelLoaded->feed(NULL, "b", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(loopSelLoaded->feed(NULL, "c", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(loopSelLoaded->feed(NULL, "d", 0) ,1, int, "%d");

}
static void loop_loop(void) {
  long savePosition = 0;


  //Selector with a loop of one character
    shared_ptr<Loop> loopLoop = Foundation::loop();
  	shared_ptr<Loop> loopCar = Foundation::loop();
  	shared_ptr<CharRecognizer> characterLoop2 = Foundation::charRecognizer('a', false);
  	loopCar->setRecognizer(characterLoop2, 0, 3);
    loopLoop->setRecognizer(loopCar, 0, 3);

        BC_ASSERT_EQUAL(loopLoop->feed(NULL, "a", 0) ,1, int, "%d");

        ofstream ofichier ("test-loop.bin", ios::out | ios::app);
        loopLoop->save(ofichier, savePosition);
        ofichier.close();

        ifstream ifichier ("test-loop.bin", ios::in);
        string content;
        getline(ifichier, content);

        std::vector<std::string> result = split(content, ' ');
        vector<string>::const_iterator i = result.begin();

        const shared_ptr<Loop> loopLoopLoaded = Loop::loadVect(i);
        ifichier.close();
        remove("test-loop.bin");


        BC_ASSERT_TRUE(loopLoop->equal(loopLoopLoaded));

        BC_ASSERT_EQUAL(loopLoopLoaded->feed(NULL, "a", 0) ,1, int, "%d");

}
static void sequence_loop(void) {
  long savePosition = 0;


  //Loop with one Sequence

  shared_ptr<Loop> loopSequence = Foundation::loop();
  shared_ptr<Sequence> sequence1 = Foundation::sequence();
  shared_ptr<CharRecognizer> characterSequence1 = Foundation::charRecognizer('a', false);
  shared_ptr<CharRecognizer> characterSequence2 = Foundation::charRecognizer('b', false);
  shared_ptr<CharRecognizer> characterSequence3 = Foundation::charRecognizer('c', false);
  shared_ptr<CharRecognizer> characterSequence4 = Foundation::charRecognizer('d', false);
  sequence1->addRecognizer(characterSequence1);
  sequence1->addRecognizer(characterSequence2);
  sequence1->addRecognizer(characterSequence3);
  sequence1->addRecognizer(characterSequence4);
  loopSequence->setRecognizer(sequence1, 0 ,3);


        BC_ASSERT_EQUAL(loopSequence->feed(NULL, "abcd", 0) ,4, int, "%d");

        ofstream ofichier ("test-loop.bin", ios::out | ios::app);
        loopSequence->save(ofichier, savePosition);
        ofichier.close();

        ifstream ifichier ("test-loop.bin", ios::in);
        string content;
        getline(ifichier, content);

        std::vector<std::string> result = split(content, ' ');
        vector<string>::const_iterator i = result.begin();

        const shared_ptr<Loop> loopSequenceLoaded = Loop::loadVect(i);
        ifichier.close();
        remove("test-loop.bin");


        BC_ASSERT_TRUE(loopSequence->equal(loopSequenceLoaded));

        BC_ASSERT_EQUAL(loopSequenceLoaded->feed(NULL, "abcd", 0) ,4, int, "%d");

}
static void literal_loop(void) {
  long savePosition = 0;


  //Loop with one Literal
  shared_ptr<Loop> loopLiteral = Foundation::loop();
  const shared_ptr<Literal> stringBasic = dynamic_pointer_cast<Literal>(Utils::literal("abcd"));
  loopLiteral->setRecognizer(stringBasic,0,3);


        BC_ASSERT_EQUAL(loopLiteral->feed(NULL, "abcd", 0) ,4, int, "%d");

        ofstream ofichier ("test-loop.bin", ios::out | ios::app);
        loopLiteral->save(ofichier, savePosition);
        ofichier.close();

        ifstream ifichier ("test-loop.bin", ios::in);
        string content;
        getline(ifichier, content);

        std::vector<std::string> result = split(content, ' ');
        vector<string>::const_iterator i = result.begin();

        const shared_ptr<Loop> loopLiteralLoaded = Loop::loadVect(i);
        ifichier.close();
        remove("test-loop.bin");


        BC_ASSERT_TRUE(loopLiteral->equal(loopLiteralLoaded));

        BC_ASSERT_EQUAL(loopLiteralLoaded->feed(NULL, "abcd", 0) ,4, int, "%d");

}

static void xchar_loop(void) {
  long savePosition = 0;


  //Loop with one CharRange
  shared_ptr<Loop> loopCharRange = Foundation::loop();
  const shared_ptr<CharRange> characRange = dynamic_pointer_cast<CharRange>(Utils::char_range(97, 100));
  loopCharRange->setRecognizer(characRange, 0, 3);
/*********************BIZARRE QUE ça SOIT un 3 **************************/
        BC_ASSERT_EQUAL(loopCharRange->feed(NULL, "abcd", 0) ,3, int, "%d");

        ofstream ofichier ("test-loop.bin", ios::out | ios::app);
        loopCharRange->save(ofichier, savePosition);
        ofichier.close();

        ifstream ifichier ("test-loop.bin", ios::in);
        string content;
        getline(ifichier, content);

        std::vector<std::string> result = split(content, ' ');
        vector<string>::const_iterator i = result.begin();

        const shared_ptr<Loop> loopCharRangeLoaded = Loop::loadVect(i);
        ifichier.close();
        remove("test-loop.bin");


        BC_ASSERT_TRUE(loopCharRange->equal(loopCharRangeLoaded));

        BC_ASSERT_EQUAL(loopCharRangeLoaded->feed(NULL, "abcd", 0) ,3, int, "%d");



}
static void recp_loop(void) {
  long savePosition = 0;



  //Loop with one Recognizer Pointer
  shared_ptr<Loop> loopRecPointer = Foundation::loop();
  shared_ptr<RecognizerPointer> recognizptrCar = make_shared<RecognizerPointer>();
  shared_ptr<CharRecognizer> characterRecognizerPtr2 = Foundation::charRecognizer('a', false);
  recognizptrCar->setPointed(characterRecognizerPtr2);
  loopRecPointer->setRecognizer(recognizptrCar,0,3);
  loopRecPointer->feed(NULL, "a", 0);
      BC_ASSERT_EQUAL(loopRecPointer->feed(NULL, "a", 0) ,1, int, "%d");

      ofstream ofichier ("test-loop.bin", ios::out | ios::app);
      loopRecPointer->save(ofichier, savePosition);
      ofichier.close();

      ifstream ifichier ("test-loop.bin", ios::in);
      string content;
      getline(ifichier, content);

      std::vector<std::string> result = split(content, ' ');
      vector<string>::const_iterator i = result.begin();

      const shared_ptr<Loop> loopRecPointerLoaded = Loop::loadVect(i);
      ifichier.close();
      remove("test-loop.bin");


      BC_ASSERT_TRUE(loopRecPointer->equal(loopRecPointerLoaded));
      cout << loopRecPointerLoaded->feed(NULL, "a", 0) << endl;
      BC_ASSERT_EQUAL(loopRecPointerLoaded->feed(NULL, "a", 0) ,1, int, "%d");


}


static test_t tests[] = {
	TEST_NO_TAG("CHARRECOGNIZERIN", char_loop),
	TEST_NO_TAG("EXCLUSIVESSELECTORIN", xsel_loop),
	TEST_NO_TAG("SELECTORIN", sel_loop),
	TEST_NO_TAG("LOOPIN", loop_loop),
	TEST_NO_TAG("SEQUENCEIN", sequence_loop),
	TEST_NO_TAG("LITERALIN", literal_loop),
  TEST_NO_TAG("CHARRANGEIN", xchar_loop),
  TEST_NO_TAG("RECOGNIZERPOINTERIN", recp_loop),

};

test_suite_t recognizer_loop_test_suite = {
	"Loop",
	NULL,
	NULL,
	NULL,
	NULL,
	sizeof(tests) / sizeof(tests[0]),
	tests
};
