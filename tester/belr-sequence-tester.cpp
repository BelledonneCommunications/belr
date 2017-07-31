/*
	belcard-sequence-tester.cpp
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


static void char_sequence(void) {
  long savePosition = 0;
  std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
  std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
  std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;
  rcptrItEnd = (mRecognizerPointersLoaded.end());
  rcptrItBegin = (mRecognizerPointersLoaded.begin());

  //Selector with 4 characters
  	shared_ptr<Sequence> sequence = Foundation::sequence();
  	shared_ptr<CharRecognizer> characterSel1 = Foundation::charRecognizer('a', false);
    shared_ptr<CharRecognizer> characterSel2 = Foundation::charRecognizer('b', false);
    shared_ptr<CharRecognizer> characterSel3 = Foundation::charRecognizer('c', false);
    shared_ptr<CharRecognizer> characterSel4 = Foundation::charRecognizer('d', false);
    sequence->addRecognizer(characterSel1);
    sequence->addRecognizer(characterSel2);
    sequence->addRecognizer(characterSel3);
    sequence->addRecognizer(characterSel4);

    BC_ASSERT_EQUAL(sequence->feed(NULL, "a", 0) ,-1, int, "%d");
    BC_ASSERT_EQUAL(sequence->feed(NULL, "b", 0) ,-1, int, "%d");
    BC_ASSERT_EQUAL(sequence->feed(NULL, "c", 0) ,-1, int, "%d");
    BC_ASSERT_EQUAL(sequence->feed(NULL, "d", 0) ,-1, int, "%d");

    ofstream ofichier ("test-seq.bin", ios::out | ios::app);
    sequence->save(ofichier, savePosition);
    ofichier.close();

    ifstream ifichier ("test-seq.bin", ios::in);
    string content;
    getline(ifichier, content);
    std::vector<std::string> result = split(content, ' ');
    vector<string>::const_iterator i = result.begin();
    const shared_ptr<Sequence> sequenceLoaded = Sequence::loadVect(i, rcptrItBegin, rcptrItEnd);
    ifichier.close();
    remove("test-seq.bin");


    BC_ASSERT_TRUE(sequence->equal(sequenceLoaded));

    BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "a", 0) ,-1, int, "%d");
    BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "b", 0) ,-1, int, "%d");
    BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "c", 0) ,-1, int, "%d");
    BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "d", 0) ,-1, int, "%d");


}
static void xsel_sequence(void) {
  long savePosition = 0;
  std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
  std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
  std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;
  rcptrItEnd = (mRecognizerPointersLoaded.end());
  rcptrItBegin = (mRecognizerPointersLoaded.begin());


  //Selector with an Exclusive Selector inside
  shared_ptr<Sequence> sequence = Foundation::sequence();
  shared_ptr<Selector> ExclusiveSelector1 = Foundation::selector(true);
  shared_ptr<CharRecognizer> characterExclusiveSelector1 = Foundation::charRecognizer('a', false);
  shared_ptr<CharRecognizer> characterExclusiveSelector2 = Foundation::charRecognizer('b', false);
  shared_ptr<CharRecognizer> characterExclusiveSelector3 = Foundation::charRecognizer('c', false);
  shared_ptr<CharRecognizer> characterExclusiveSelector4 = Foundation::charRecognizer('d', false);
  ExclusiveSelector1->addRecognizer(characterExclusiveSelector1);
  ExclusiveSelector1->addRecognizer(characterExclusiveSelector2);
  ExclusiveSelector1->addRecognizer(characterExclusiveSelector3);
  ExclusiveSelector1->addRecognizer(characterExclusiveSelector4);
  sequence->addRecognizer(ExclusiveSelector1);


      BC_ASSERT_EQUAL(sequence->feed(NULL, "a", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(sequence->feed(NULL, "b", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(sequence->feed(NULL, "c", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(sequence->feed(NULL, "d", 0) ,1, int, "%d");

      ofstream ofichier ("test-seq.bin", ios::out | ios::app);
      sequence->save(ofichier, savePosition);
      ofichier.close();

      ifstream ifichier ("test-seq.bin", ios::in);
      string content;
      getline(ifichier, content);

      std::vector<std::string> result = split(content, ' ');
      vector<string>::const_iterator i = result.begin();

      const shared_ptr<Sequence> sequenceLoaded = Sequence::loadVect(i,rcptrItBegin ,rcptrItEnd);
      ifichier.close();
      remove("test-seq.bin");


      BC_ASSERT_TRUE(sequence->equal(sequenceLoaded));

      BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "a", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "b", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "c", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "d", 0) ,1, int, "%d");

}
static void sel_sequence(void) {
  long savePosition = 0;

  std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
  std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
  std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;
  rcptrItEnd = (mRecognizerPointersLoaded.end());
  rcptrItBegin = (mRecognizerPointersLoaded.begin());

  //Selector with an Exclusive Selector inside
  shared_ptr<Sequence> sequence = Foundation::sequence();
  shared_ptr<Selector> ExclusiveSelector1 = Foundation::selector(true);
  shared_ptr<CharRecognizer> characterExclusiveSelector1 = Foundation::charRecognizer('a', false);
  shared_ptr<CharRecognizer> characterExclusiveSelector2 = Foundation::charRecognizer('b', false);
  shared_ptr<CharRecognizer> characterExclusiveSelector3 = Foundation::charRecognizer('c', false);
  shared_ptr<CharRecognizer> characterExclusiveSelector4 = Foundation::charRecognizer('d', false);
  ExclusiveSelector1->addRecognizer(characterExclusiveSelector1);
  ExclusiveSelector1->addRecognizer(characterExclusiveSelector2);
  ExclusiveSelector1->addRecognizer(characterExclusiveSelector3);
  ExclusiveSelector1->addRecognizer(characterExclusiveSelector4);
  sequence->addRecognizer(ExclusiveSelector1);


      BC_ASSERT_EQUAL(sequence->feed(NULL, "a", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(sequence->feed(NULL, "b", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(sequence->feed(NULL, "c", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(sequence->feed(NULL, "d", 0) ,1, int, "%d");

      ofstream ofichier ("test-seq.bin", ios::out | ios::app);
      sequence->save(ofichier, savePosition);
      ofichier.close();

      ifstream ifichier ("test-seq.bin", ios::in);
      string content;
      getline(ifichier, content);

      std::vector<std::string> result = split(content, ' ');
      vector<string>::const_iterator i = result.begin();

      const shared_ptr<Sequence> sequenceLoaded = Sequence::loadVect(i, rcptrItBegin, rcptrItEnd);
      ifichier.close();
      remove("test-seq.bin");


      BC_ASSERT_TRUE(sequence->equal(sequenceLoaded));

      BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "a", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "b", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "c", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "d", 0) ,1, int, "%d");

}
static void loop_sequence(void) {
  long savePosition = 0;

  std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
  std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
  std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;
  rcptrItEnd = (mRecognizerPointersLoaded.end());
  rcptrItBegin = (mRecognizerPointersLoaded.begin());

  //Selector with a loop of one character
  	shared_ptr<Sequence> sequence = Foundation::sequence();
  	shared_ptr<Loop> loopCar = Foundation::loop();
  	shared_ptr<CharRecognizer> characterLoop2 = Foundation::charRecognizer('a', false);
  	loopCar->setRecognizer(characterLoop2, 0, 3);
  	sequence->addRecognizer(loopCar);

        BC_ASSERT_EQUAL(sequence->feed(NULL, "a", 0) ,1, int, "%d");

        ofstream ofichier ("test-seq.bin", ios::out | ios::app);
        sequence->save(ofichier, savePosition);
        ofichier.close();

        ifstream ifichier ("test-seq.bin", ios::in);
        string content;
        getline(ifichier, content);

        std::vector<std::string> result = split(content, ' ');
        vector<string>::const_iterator i = result.begin();

        const shared_ptr<Sequence> sequenceLoaded = Sequence::loadVect(i, rcptrItBegin, rcptrItEnd);
        ifichier.close();
        remove("test-seq.bin");


        BC_ASSERT_TRUE(sequence->equal(sequenceLoaded));

        BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "a", 0) ,1, int, "%d");

}
static void sequence_sequence(void) {
  long savePosition = 0;
  std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
  std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
  std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;
  rcptrItEnd = (mRecognizerPointersLoaded.end());
  rcptrItBegin = (mRecognizerPointersLoaded.begin());


  //Selector with a sequence of 4 characters
  	shared_ptr<Sequence> sequence = Foundation::sequence();
  	shared_ptr<Sequence> sequence1 = Foundation::sequence();
  	shared_ptr<CharRecognizer> characterSequence1 = Foundation::charRecognizer('a', false);
  	shared_ptr<CharRecognizer> characterSequence2 = Foundation::charRecognizer('b', false);
  	shared_ptr<CharRecognizer> characterSequence3 = Foundation::charRecognizer('c', false);
  	shared_ptr<CharRecognizer> characterSequence4 = Foundation::charRecognizer('d', false);
  	sequence1->addRecognizer(characterSequence1);
  	sequence1->addRecognizer(characterSequence2);
  	sequence1->addRecognizer(characterSequence3);
  	sequence1->addRecognizer(characterSequence4);
  	sequence->addRecognizer(sequence1);


        BC_ASSERT_EQUAL(sequence->feed(NULL, "abcd", 0) ,4, int, "%d");

        ofstream ofichier ("test-seq.bin", ios::out | ios::app);
        sequence->save(ofichier, savePosition);
        ofichier.close();

        ifstream ifichier ("test-seq.bin", ios::in);
        string content;
        getline(ifichier, content);

        std::vector<std::string> result = split(content, ' ');
        vector<string>::const_iterator i = result.begin();

        const shared_ptr<Sequence> sequenceLoaded = Sequence::loadVect(i, rcptrItBegin, rcptrItEnd);
        ifichier.close();
        remove("test-seq.bin");


        BC_ASSERT_TRUE(sequence->equal(sequenceLoaded));

        BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "abcd", 0) ,4, int, "%d");

}
static void literal_sequence(void) {
  long savePosition = 0;

  std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
  std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
  std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;
  rcptrItEnd = (mRecognizerPointersLoaded.end());
  rcptrItBegin = (mRecognizerPointersLoaded.begin());

  // Selector with a Literal
  	shared_ptr<Sequence> sequence = Foundation::sequence();
  	const shared_ptr<Literal> stringBasic = dynamic_pointer_cast<Literal>(Utils::literal("abcd"));
  	sequence->addRecognizer(stringBasic);

        BC_ASSERT_EQUAL(sequence->feed(NULL, "abcd", 0) ,4, int, "%d");

        ofstream ofichier ("test-seq.bin", ios::out | ios::app);
        sequence->save(ofichier, savePosition);
        ofichier.close();

        ifstream ifichier ("test-seq.bin", ios::in);
        string content;
        getline(ifichier, content);

        std::vector<std::string> result = split(content, ' ');
        vector<string>::const_iterator i = result.begin();

        const shared_ptr<Sequence> sequenceLoaded = Sequence::loadVect(i, rcptrItBegin, rcptrItEnd);
        ifichier.close();
        remove("test-seq.bin");


        BC_ASSERT_TRUE(sequence->equal(sequenceLoaded));

        BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "abcd", 0) ,4, int, "%d");

}
static void xchar_sequence(void) {
  long savePosition = 0;

  std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
  std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
  std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;
  rcptrItEnd = (mRecognizerPointersLoaded.end());
  rcptrItBegin = (mRecognizerPointersLoaded.begin());

  //Selector with a CharRange
  	shared_ptr<Sequence> sequence = Foundation::sequence();
  	const shared_ptr<CharRange> charRange1 = dynamic_pointer_cast<CharRange>(Utils::char_range(97, 100));
    const shared_ptr<CharRange> charRange2 = dynamic_pointer_cast<CharRange>(Utils::char_range(98, 101));
    const shared_ptr<CharRange> charRange3 = dynamic_pointer_cast<CharRange>(Utils::char_range(99, 102));
  	sequence->addRecognizer(charRange1);
    sequence->addRecognizer(charRange2);
    sequence->addRecognizer(charRange3);
//********************* POURQOUI YA UN -1 NORMAL ? *************************
        BC_ASSERT_EQUAL(sequence->feed(NULL, "a", 0) ,-1, int, "%d");
        BC_ASSERT_EQUAL(sequence->feed(NULL, "b", 0) ,-1, int, "%d");
        BC_ASSERT_EQUAL(sequence->feed(NULL, "c", 0) ,-1, int, "%d");
        BC_ASSERT_EQUAL(sequence->feed(NULL, "d", 0) ,-1, int, "%d");

        ofstream ofichier ("test-seq.bin", ios::out | ios::app);
        sequence->save(ofichier, savePosition);
        ofichier.close();

        ifstream ifichier ("test-seq.bin", ios::in);
        string content;
        getline(ifichier, content);

        std::vector<std::string> result = split(content, ' ');
        vector<string>::const_iterator i = result.begin();

        const shared_ptr<Sequence> sequenceLoaded = Sequence::loadVect(i,rcptrItBegin, rcptrItEnd);
        ifichier.close();
        remove("test-seq.bin");


        BC_ASSERT_TRUE(sequence->equal(sequenceLoaded));

        BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "a", 0) ,-1, int, "%d");
        BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "b", 0) ,-1, int, "%d");
        BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "c", 0) ,-1, int, "%d");
        BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "d", 0) ,-1, int, "%d");



}
static void recp_sequence(void) {
  long savePosition = 0;


  std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
  std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
  std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;

  //Selector with a RecognizerPointer
  shared_ptr<Sequence> sequence = Foundation::sequence();
  shared_ptr<RecognizerPointer> recognizptrCar = make_shared<RecognizerPointer>();
  recognizptrCar->setName("@rule");
  shared_ptr<CharRecognizer> characterRecognizerPtr2 = Foundation::charRecognizer('a', false);
  recognizptrCar->setPointed(characterRecognizerPtr2);
  sequence->addRecognizer(recognizptrCar);
  mRecognizerPointersLoaded.push_back(recognizptrCar);
  rcptrItEnd = (mRecognizerPointersLoaded.end());
  rcptrItBegin = (mRecognizerPointersLoaded.begin());

      BC_ASSERT_EQUAL(sequence->feed(NULL, "a", 0) ,1, int, "%d");

      ofstream ofichier ("test-seq.bin", ios::out | ios::app);
      sequence->save(ofichier, savePosition);
      ofichier.close();

      ifstream ifichier ("test-seq.bin", ios::in);
      string content;
      getline(ifichier, content);

      std::vector<std::string> result = split(content, ' ');
      vector<string>::const_iterator i = result.begin();

      const shared_ptr<Sequence> sequenceLoaded = Sequence::loadVect(i, rcptrItBegin, rcptrItEnd);
      ifichier.close();
      remove("test-seq.bin");


      BC_ASSERT_TRUE(sequence->equal(sequenceLoaded));

      BC_ASSERT_EQUAL(sequenceLoaded->feed(NULL, "a", 0) ,1, int, "%d");


}


static test_t tests[] = {
	TEST_NO_TAG("CHARRECOGNIZERIN", char_sequence),
	TEST_NO_TAG("EXCLUSIVESSELECTORIN", xsel_sequence),
	TEST_NO_TAG("SELECTORIN", sel_sequence),
	TEST_NO_TAG("LOOPIN", loop_sequence),
	TEST_NO_TAG("SEQUENCEIN", sequence_sequence),
	TEST_NO_TAG("LITERALIN", literal_sequence),
  TEST_NO_TAG("CHARRANGEIN", xchar_sequence),
  TEST_NO_TAG("RECOGNIZERPOINTERIN", recp_sequence),

};

test_suite_t recognizer_sequence_test_suite = {
	"Sequence",
	NULL,
	NULL,
	NULL,
	NULL,
	sizeof(tests) / sizeof(tests[0]),
	tests
};
