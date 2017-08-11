  /*
  	belcard-recognizerpointer-tester.cpp
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


  static void char_recognizerpointer(void) {
    long savePosition = 0;

        std::list<std::shared_ptr<RecognizerPointer>> mRecognizerPointersLoaded;
        std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItBegin ;
        std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItEnd ;


    //RecognizerPointer with one CharRecognizer
  		shared_ptr<RecognizerPointer> recognizptr = make_shared<RecognizerPointer>();
      recognizptr->setName("@rule");
  		shared_ptr<CharRecognizer> caractere2 = Foundation::charRecognizer('a', false);
  	  recognizptr->setPointed(caractere2);
      mRecognizerPointersLoaded.push_back(recognizptr);
      rcptrItEnd = (mRecognizerPointersLoaded.end());
      rcptrItBegin = (mRecognizerPointersLoaded.begin());

      BC_ASSERT_EQUAL(recognizptr->feed(NULL, "a", 0) ,1, int, "%d");


      ofstream ofichier ("test-recp.bin", ios::out | ios::app);
  	  recognizptr->save(ofichier, savePosition);
      ofichier.close();
      ifstream ifichier ("test-recp.bin", ios::in);
      string content;
      getline(ifichier, content);
      std::vector<std::string> result = split(content, ' ');
      vector<string>::const_iterator i = result.begin();
      ifichier.close();
      const shared_ptr<RecognizerPointer> recognizptrLoaded = RecognizerPointer::loadVect(i, rcptrItBegin, rcptrItEnd);
      remove("test-recp.bin");


      BC_ASSERT_TRUE(recognizptr->equal(recognizptrLoaded));

      BC_ASSERT_EQUAL(recognizptrLoaded->feed(NULL, "a", 0) ,1, int, "%d");


  }
  static void sel_recognizerpointer(void) {
    long savePosition = 0;

    std::list<std::shared_ptr<RecognizerPointer>> mRecognizerPointersLoaded;
    std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItBegin ;
    std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItEnd ;


    	//RecognizerPointer with a selector of 4 characters
    		shared_ptr<RecognizerPointer> recognizptr = make_shared<RecognizerPointer>();
        recognizptr->setName("@rule");
    		shared_ptr<Selector> selector1 = Foundation::selector(false);
    		shared_ptr<CharRecognizer> characterSel1 = Foundation::charRecognizer('a', false);
    	  shared_ptr<CharRecognizer> characterSel2 = Foundation::charRecognizer('b', false);
    	  shared_ptr<CharRecognizer> characterSel3 = Foundation::charRecognizer('c', false);
    	  shared_ptr<CharRecognizer> characterSel4 = Foundation::charRecognizer('d', false);
    	  selector1->addRecognizer(characterSel1);
    	  selector1->addRecognizer(characterSel2);
    	  selector1->addRecognizer(characterSel3);
    	  selector1->addRecognizer(characterSel4);
    		recognizptr->setPointed(selector1);
        mRecognizerPointersLoaded.push_back(recognizptr);
        rcptrItEnd = (mRecognizerPointersLoaded.end());
        rcptrItBegin = (mRecognizerPointersLoaded.begin());


        BC_ASSERT_EQUAL(recognizptr->feed(NULL, "a", 0) ,1, int, "%d");
        BC_ASSERT_EQUAL(recognizptr->feed(NULL, "b", 0) ,1, int, "%d");
        BC_ASSERT_EQUAL(recognizptr->feed(NULL, "c", 0) ,1, int, "%d");
        BC_ASSERT_EQUAL(recognizptr->feed(NULL, "d", 0) ,1, int, "%d");

        ofstream ofichier ("test-recp.bin", ios::out | ios::app);
    	  recognizptr->save(ofichier, savePosition);
        ofichier.close();

        ifstream ifichier ("test-recp.bin", ios::in);
        string content;
        getline(ifichier, content);
        std::vector<std::string> result = split(content, ' ');
        vector<string>::const_iterator i = result.begin();
        ifichier.close();

        const shared_ptr<RecognizerPointer> recognizptrLoaded = RecognizerPointer::loadVect(i, rcptrItBegin, rcptrItEnd);
        remove("test-recp.bin");


        BC_ASSERT_TRUE(recognizptr->equal(recognizptrLoaded));

        BC_ASSERT_EQUAL(recognizptrLoaded->feed(NULL, "a", 0) ,1, int, "%d");
        BC_ASSERT_EQUAL(recognizptrLoaded->feed(NULL, "b", 0) ,1, int, "%d");
        BC_ASSERT_EQUAL(recognizptrLoaded->feed(NULL, "c", 0) ,1, int, "%d");
        BC_ASSERT_EQUAL(recognizptrLoaded->feed(NULL, "d", 0) ,1, int, "%d");

  }
  static void xsel_recognizerpointer(void) {
    long savePosition = 0;
    std::list<std::shared_ptr<RecognizerPointer>> mRecognizerPointersLoaded;
    std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItBegin ;
    std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItEnd ;


    //RecognizerPointer with a selector of 4 characters
      shared_ptr<RecognizerPointer> recognizptr = make_shared<RecognizerPointer>();
      recognizptr->setName("@rule");
      shared_ptr<Selector> selector1 = Foundation::selector(true);
      shared_ptr<CharRecognizer> characterSel1 = Foundation::charRecognizer('a', false);
      shared_ptr<CharRecognizer> characterSel2 = Foundation::charRecognizer('b', false);
      shared_ptr<CharRecognizer> characterSel3 = Foundation::charRecognizer('c', false);
      shared_ptr<CharRecognizer> characterSel4 = Foundation::charRecognizer('d', false);
      selector1->addRecognizer(characterSel1);
      selector1->addRecognizer(characterSel2);
      selector1->addRecognizer(characterSel3);
      selector1->addRecognizer(characterSel4);
      recognizptr->setPointed(selector1);
      mRecognizerPointersLoaded.push_back(recognizptr);
      rcptrItEnd = (mRecognizerPointersLoaded.end());
      rcptrItBegin = (mRecognizerPointersLoaded.begin());


      BC_ASSERT_EQUAL(recognizptr->feed(NULL, "a", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(recognizptr->feed(NULL, "b", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(recognizptr->feed(NULL, "c", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(recognizptr->feed(NULL, "d", 0) ,1, int, "%d");

      ofstream ofichier ("test-recp.bin", ios::out | ios::app);
      recognizptr->save(ofichier, savePosition);
      ofichier.close();

      ifstream ifichier ("test-recp.bin", ios::in);
      string content;
      getline(ifichier, content);
      std::vector<std::string> result = split(content, ' ');
      vector<string>::const_iterator i = result.begin();
      ifichier.close();

      const shared_ptr<RecognizerPointer> recognizptrLoaded = RecognizerPointer::loadVect(i, rcptrItBegin, rcptrItEnd);
      remove("test-recp.bin");


      BC_ASSERT_TRUE(recognizptr->equal(recognizptrLoaded));

      BC_ASSERT_EQUAL(recognizptrLoaded->feed(NULL, "a", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(recognizptrLoaded->feed(NULL, "b", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(recognizptrLoaded->feed(NULL, "c", 0) ,1, int, "%d");
      BC_ASSERT_EQUAL(recognizptrLoaded->feed(NULL, "d", 0) ,1, int, "%d");
  }

static void loop_recognizerpointer(void) {
    long savePosition = 0;

    std::list<std::shared_ptr<RecognizerPointer>> mRecognizerPointersLoaded;
    std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItBegin ;
    std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItEnd ;

    //RecognizerPointer with a loop of one character

  		shared_ptr<RecognizerPointer> recognizptr = make_shared<RecognizerPointer>();
      recognizptr->setName("@rule");
  		shared_ptr<Loop> loopCar = Foundation::loop();
  		shared_ptr<CharRecognizer> characterLoop2 = Foundation::charRecognizer('a', false);
  		loopCar->setRecognizer(characterLoop2, 0, 3);
  		recognizptr->setPointed(loopCar);
      mRecognizerPointersLoaded.push_back(recognizptr);
      rcptrItEnd = (mRecognizerPointersLoaded.end());
      rcptrItBegin = (mRecognizerPointersLoaded.begin());


      BC_ASSERT_EQUAL(recognizptr->feed(NULL, "a", 0) ,1, int, "%d");


      ofstream ofichier ("test-recp.bin", ios::out | ios::app);
      recognizptr->save(ofichier, savePosition);
      ofichier.close();

      ifstream ifichier ("test-recp.bin", ios::in);
      string content;
      getline(ifichier, content);
      std::vector<std::string> result = split(content, ' ');
      vector<string>::const_iterator i = result.begin();
      ifichier.close();

      const shared_ptr<RecognizerPointer> recognizptrLoaded = RecognizerPointer::loadVect(i, rcptrItBegin, rcptrItEnd);
      remove("test-recp.bin");


      BC_ASSERT_TRUE(recognizptr->equal(recognizptrLoaded));

      BC_ASSERT_EQUAL(recognizptrLoaded->feed(NULL, "a", 0) ,1, int, "%d");

  }


static void sequence_recognizerpointer(void) {
    long savePosition = 0;
    std::list<std::shared_ptr<RecognizerPointer>> mRecognizerPointersLoaded;
    std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItBegin ;
    std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItEnd ;


    //RecognizerPointer with a sequence of 4 characters
  		shared_ptr<RecognizerPointer> recognizptr = make_shared<RecognizerPointer>();
      recognizptr->setName("@rule");
  		shared_ptr<Sequence> sequence1 = Foundation::sequence();
  		shared_ptr<CharRecognizer> characterSequence1 = Foundation::charRecognizer('a', false);
  		shared_ptr<CharRecognizer> characterSequence2 = Foundation::charRecognizer('b', false);
  		shared_ptr<CharRecognizer> characterSequence3 = Foundation::charRecognizer('c', false);
  		shared_ptr<CharRecognizer> characterSequence4 = Foundation::charRecognizer('d', false);
  		sequence1->addRecognizer(characterSequence1);
  		sequence1->addRecognizer(characterSequence2);
  		sequence1->addRecognizer(characterSequence3);
  		sequence1->addRecognizer(characterSequence4);
  		recognizptr->setPointed(sequence1);
      mRecognizerPointersLoaded.push_back(recognizptr);
      rcptrItEnd = (mRecognizerPointersLoaded.end());
      rcptrItBegin = (mRecognizerPointersLoaded.begin());


          BC_ASSERT_EQUAL(recognizptr->feed(NULL, "abcd", 0) ,4, int, "%d");

          ofstream ofichier ("test-recp.bin", ios::out | ios::app);
          recognizptr->save(ofichier, savePosition);
          ofichier.close();

          ifstream ifichier ("test-recp.bin", ios::in);
          string content;
          getline(ifichier, content);
          std::vector<std::string> result = split(content, ' ');
          vector<string>::const_iterator i = result.begin();
          ifichier.close();

          const shared_ptr<RecognizerPointer> recognizptrLoaded = RecognizerPointer::loadVect(i, rcptrItBegin, rcptrItEnd);
          remove("test-recp.bin");


          BC_ASSERT_TRUE(recognizptr->equal(recognizptrLoaded));

          BC_ASSERT_EQUAL(recognizptrLoaded->feed(NULL, "abcd", 0) ,4, int, "%d");

  }
  static void literal_recognizerpointer(void) {
    long savePosition = 0;

    std::list<std::shared_ptr<RecognizerPointer>> mRecognizerPointersLoaded;
    std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItBegin ;
    std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItEnd ;


    	// Selector with a Literal
    		shared_ptr<RecognizerPointer> recognizptr = make_shared<RecognizerPointer>();
        recognizptr->setName("@rule");
    		const shared_ptr<Literal> stringBasic = dynamic_pointer_cast<Literal>(Utils::literal("abcd"));
    		recognizptr->setPointed(stringBasic);
        mRecognizerPointersLoaded.push_back(recognizptr);
        rcptrItEnd = (mRecognizerPointersLoaded.end());
        rcptrItBegin = (mRecognizerPointersLoaded.begin());


          BC_ASSERT_EQUAL(recognizptr->feed(NULL, "abcd", 0) ,4, int, "%d");

          ofstream ofichier ("test-recp.bin", ios::out | ios::app);
          recognizptr->save(ofichier, savePosition);
          ofichier.close();

          ifstream ifichier ("test-recp.bin", ios::in);
          string content;
          getline(ifichier, content);
          std::vector<std::string> result = split(content, ' ');
          vector<string>::const_iterator i = result.begin();
          ifichier.close();

          const shared_ptr<RecognizerPointer> recognizptrLoaded = RecognizerPointer::loadVect(i, rcptrItBegin, rcptrItEnd);
          remove("test-recp.bin");


          BC_ASSERT_TRUE(recognizptr->equal(recognizptrLoaded));

          BC_ASSERT_EQUAL(recognizptrLoaded->feed(NULL, "abcd", 0) ,4, int, "%d");

  }
static void xchar_recognizerpointer(void) {
    long savePosition = 0;
    std::list<std::shared_ptr<RecognizerPointer>> mRecognizerPointersLoaded;
    std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItBegin ;
    std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItEnd ;


    //Selector with a CharRange
  		shared_ptr<RecognizerPointer> recognizptr = make_shared<RecognizerPointer>();
      recognizptr->setName("@rule");
  		const shared_ptr<CharRange> charRange = dynamic_pointer_cast<CharRange>(Utils::char_range(97, 100));
  		recognizptr->setPointed(charRange);
      mRecognizerPointersLoaded.push_back(recognizptr);
      rcptrItEnd = (mRecognizerPointersLoaded.end());
      rcptrItBegin = (mRecognizerPointersLoaded.begin());


      BC_ASSERT_EQUAL(recognizptr->feed(NULL, "abcd", 0) ,1, int, "%d");

      ofstream ofichier ("test-recp.bin", ios::out | ios::app);
      recognizptr->save(ofichier, savePosition);
      ofichier.close();

      ifstream ifichier ("test-recp.bin", ios::in);
      string content;
      getline(ifichier, content);
      std::vector<std::string> result = split(content, ' ');
      vector<string>::const_iterator i = result.begin();
      ifichier.close();

      const shared_ptr<RecognizerPointer> recognizptrLoaded = RecognizerPointer::loadVect(i, rcptrItBegin, rcptrItEnd);
      remove("test-recp.bin");


          BC_ASSERT_TRUE(recognizptr->equal(recognizptrLoaded));

          BC_ASSERT_EQUAL(recognizptrLoaded->feed(NULL, "abcd", 0) ,1, int, "%d");



  }
static void recp_recognizerpointer(void) {
    long savePosition = 0;

    std::list<std::shared_ptr<RecognizerPointer>> mRecognizerPointersLoaded;
    std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItBegin ;
    std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItEnd ;

    shared_ptr<RecognizerPointer> recognizptr = make_shared<RecognizerPointer>();
    shared_ptr<RecognizerPointer> recognizptrCar = make_shared<RecognizerPointer>();
    recognizptr->setName("@rule");
    recognizptrCar->setName("@ruleCar");

    shared_ptr<CharRecognizer> characterRecognizerPtr2 = Foundation::charRecognizer('a', false);
    recognizptrCar->setPointed(characterRecognizerPtr2);
    recognizptr->setPointed(recognizptrCar);
    mRecognizerPointersLoaded.push_back(recognizptr);
    mRecognizerPointersLoaded.push_back(recognizptrCar);
    rcptrItEnd = (mRecognizerPointersLoaded.end());
    rcptrItBegin = (mRecognizerPointersLoaded.begin());


    BC_ASSERT_EQUAL(recognizptr->feed(NULL, "a", 0) ,1, int, "%d");

    ofstream ofichier ("test-recp.bin", ios::out | ios::app);
    recognizptr->save(ofichier, savePosition);
    ofichier.close();

    ifstream ifichier ("test-recp.bin", ios::in);
    string content;
    getline(ifichier, content);
    std::vector<std::string> result = split(content, ' ');
    vector<string>::const_iterator i = result.begin();
    const shared_ptr<RecognizerPointer> recognizptrLoaded = RecognizerPointer::loadVect(i, rcptrItBegin, rcptrItEnd);
    remove("test-recp.bin");
    ifichier.close();


    BC_ASSERT_TRUE(recognizptr->equal(recognizptrLoaded));

    BC_ASSERT_EQUAL(recognizptrLoaded->feed(NULL, "a", 0) ,1, int, "%d");

  }


  static test_t tests[] = {
  	TEST_NO_TAG("CHARRECOGNIZERIN", char_recognizerpointer),
  	TEST_NO_TAG("EXCLUSIVESSELECTORIN", xsel_recognizerpointer),
  	TEST_NO_TAG("SELECTORIN", sel_recognizerpointer),
  	TEST_NO_TAG("LOOPIN", loop_recognizerpointer),
  	TEST_NO_TAG("SEQUENCEIN", sequence_recognizerpointer),
  	TEST_NO_TAG("LITERALIN", literal_recognizerpointer),
    TEST_NO_TAG("CHARRANGEIN", xchar_recognizerpointer),
    TEST_NO_TAG("RECOGNIZERPOINTERIN", recp_recognizerpointer),

  };

  test_suite_t recognizer_recognizerpointer_test_suite = {
  	"RecognizerPointer",
  	NULL,
  	NULL,
  	NULL,
  	NULL,
  	sizeof(tests) / sizeof(tests[0]),
  	tests
  };
