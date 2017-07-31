
#include "belr/grammarbuilder.hh"
#include "belr/abnf.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <memory>
//#include <chrono>


using namespace belr;
using namespace std;

long savePosition = 0;
long loadPosition = 0;


std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded;
std::list<std::shared_ptr<Recognizer>> mRecognizerPointers;

std::list<shared_ptr<Recognizer>>::iterator rcptrItBegin ;
std::list<shared_ptr<Recognizer>>::iterator rcptrItEnd ;


template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}


int main(int argc, char *argv[]){


	//RecognizerPointer with one CharRecognizer
		shared_ptr<RecognizerPointer> recognizptr1 = make_shared<RecognizerPointer>();
		shared_ptr<CharRecognizer> caractere2 = Foundation::charRecognizer('a', false);
	  recognizptr1->setPointed(caractere2);
    recognizptr1->setName("@rule1");
    mRecognizerPointers.push_back(recognizptr1);


	//RecognizerPointer with a selector of 4 characters
		shared_ptr<RecognizerPointer> recognizptr2 = make_shared<RecognizerPointer>();
		shared_ptr<Selector> selector1 = Foundation::selector(false);
		shared_ptr<CharRecognizer> characterSel1 = Foundation::charRecognizer('a', false);
	  shared_ptr<CharRecognizer> characterSel2 = Foundation::charRecognizer('b', false);
	  shared_ptr<CharRecognizer> characterSel3 = Foundation::charRecognizer('c', false);
	  shared_ptr<CharRecognizer> characterSel4 = Foundation::charRecognizer('d', false);
	  selector1->addRecognizer(characterSel1);
	  selector1->addRecognizer(characterSel2);
	  selector1->addRecognizer(characterSel3);
	  selector1->addRecognizer(characterSel4);
		recognizptr2->setPointed(selector1);
    recognizptr2->setName("@rule2");

    mRecognizerPointers.push_back(recognizptr2);


	//RecognizerPointer with a loop of one character

		shared_ptr<RecognizerPointer> recognizptr3 = make_shared<RecognizerPointer>();
		shared_ptr<Loop> loopCar = Foundation::loop();
		shared_ptr<CharRecognizer> characterLoop2 = Foundation::charRecognizer('a', false);
		loopCar->setRecognizer(characterLoop2, 0, 3);
		recognizptr3->setPointed(loopCar);
    recognizptr3->setName("@rule3");

    mRecognizerPointers.push_back(recognizptr3);


	//RecognizerPointer with a sequence of 4 characters
		shared_ptr<RecognizerPointer> recognizptr4 = make_shared<RecognizerPointer>();
		shared_ptr<Sequence> sequence1 = Foundation::sequence();
		shared_ptr<CharRecognizer> characterSequence1 = Foundation::charRecognizer('a', false);
		shared_ptr<CharRecognizer> characterSequence2 = Foundation::charRecognizer('b', false);
		shared_ptr<CharRecognizer> characterSequence3 = Foundation::charRecognizer('c', false);
		shared_ptr<CharRecognizer> characterSequence4 = Foundation::charRecognizer('d', false);
		sequence1->addRecognizer(characterSequence1);
		sequence1->addRecognizer(characterSequence2);
		sequence1->addRecognizer(characterSequence3);
		sequence1->addRecognizer(characterSequence4);
		recognizptr4->setPointed(sequence1);
    recognizptr4->setName("@rule4");

    mRecognizerPointers.push_back(recognizptr4);


	//Selector with a CharRange
		shared_ptr<RecognizerPointer> recognizptr5 = make_shared<RecognizerPointer>();
		const shared_ptr<CharRange> charRange = dynamic_pointer_cast<CharRange>(Utils::char_range(97, 100));
		recognizptr5->setPointed(charRange);
    recognizptr5->setName("@rule5");

    mRecognizerPointers.push_back(recognizptr5);


	// Selector with a Literal
		shared_ptr<RecognizerPointer> recognizptr6 = make_shared<RecognizerPointer>();
		const shared_ptr<Literal> stringBasic = dynamic_pointer_cast<Literal>(Utils::literal("abcd"));
		recognizptr6->setPointed(stringBasic);
    recognizptr6->setName("@rule6");

    mRecognizerPointers.push_back(recognizptr6);



	//Selector with an Exclusive Selector inside
	shared_ptr<RecognizerPointer> recognizptr7 = make_shared<RecognizerPointer>();
	shared_ptr<Selector> ExclusiveSelector1 = Foundation::selector(true);
	shared_ptr<CharRecognizer> characterExclusiveSelector1 = Foundation::charRecognizer('a', false);
	shared_ptr<CharRecognizer> characterExclusiveSelector2 = Foundation::charRecognizer('b', false);
	shared_ptr<CharRecognizer> characterExclusiveSelector3 = Foundation::charRecognizer('c', false);
	shared_ptr<CharRecognizer> characterExclusiveSelector4 = Foundation::charRecognizer('d', false);
	ExclusiveSelector1->addRecognizer(characterExclusiveSelector1);
	ExclusiveSelector1->addRecognizer(characterExclusiveSelector2);
	ExclusiveSelector1->addRecognizer(characterExclusiveSelector3);
	ExclusiveSelector1->addRecognizer(characterExclusiveSelector4);
	recognizptr7->setPointed(ExclusiveSelector1);
  recognizptr7->setName("@rule7");

  mRecognizerPointers.push_back(recognizptr7);




	//verification avec feed
		cout << "*********FEEDING THE ORIGINAL RECOGNIZERS*********" << endl;
		if (recognizptr1->feed(NULL, "a", 0) == 1)cout << "SUCESS : CharacterRecognizer RecognizerPointer feed successfull" << endl;
		else cout << "FAILED : CharacterRecognizer RecognizerPointer feed gone wrong" << endl;
		if ((recognizptr2->feed(NULL, "a", 0) == 1) && (recognizptr2->feed(NULL, "b", 0) == 1) && (recognizptr2->feed(NULL, "c", 0) == 1) && (recognizptr2->feed(NULL, "d", 0) == 1))cout << "SUCESS : Selector RecognizerPointer feed successfull" << endl;
		else cout << "FAILED :  Selector RecognizerPointer feed gone wrong" << endl;
		if (recognizptr3->feed(NULL, "a", 0) == 1) cout << "SUCESS : Loop RecognizerPointer feed successfull" << endl;
		else cout << "FAILED : Loop RecognizerPointer feed gone wrong" << endl;
		if (recognizptr4->feed(NULL, "abcd", 0) == 4)cout << "SUCESS : Sequence RecognizerPointer feed successfull" << endl;
		else cout << "FAILED : Sequence RecognizerPointer feed gone wrong" << endl;
		if ((recognizptr5->feed(NULL, "a", 0) == 1) && (recognizptr5->feed(NULL, "b", 0) == 1) && (recognizptr5->feed(NULL, "c", 0) == 1) && (recognizptr5->feed(NULL, "d", 0) == 1))cout << "SUCESS : Literal RecognizerPointer feed successfull" << endl;
		else cout << "FAILED : CharRange RecognizerPointer feed gone wrong" << endl;
		if (recognizptr6->feed(NULL, "abcd", 0) == 4)cout << "SUCESS : Literal RecognizerPointer feed successfull" << endl;
		else cout << "FAILED : Literal RecognizerPointer feed gone wrong" << endl;
		if ((recognizptr7->feed(NULL, "a", 0) == 1) && (recognizptr7->feed(NULL, "b", 0) == 1) && (recognizptr7->feed(NULL, "c", 0) == 1) && (recognizptr7->feed(NULL, "d", 0) == 1))cout << "SUCESS : ExclusiveSelector RecognizerPointer feed successfull" << endl;
		else cout << "FAILED : ExclusiveSelector RecognizerPointer feed gone wrong" << endl;

    rcptrItBegin = (mRecognizerPointers.begin());

    rcptrItEnd = (mRecognizerPointers.end());

    cout << "NO PROBLEMO HERMANO" << endl;
    remove("test-recon.bin");

	  //SAVE AND LOAD
		ofstream ofichier ("test-recon.bin", ios::out | ios::app);
	  recognizptr1->save(ofichier, savePosition);
		recognizptr2->save(ofichier, savePosition);
		recognizptr3->save(ofichier, savePosition);
		recognizptr4->save(ofichier, savePosition);
		recognizptr5->save(ofichier, savePosition);
		recognizptr6->save(ofichier, savePosition);
		recognizptr7->save(ofichier, savePosition);
		ofichier.close();

		ifstream ifichier ("test-recon.bin", ios::in);
    string contenu;  // déclaration d'une chaîne qui contiendra la ligne lue
    getline(ifichier, contenu);  // on met dans "contenu" la ligne

    std::vector<std::string> result = split(contenu, ' ');
    vector<string>::const_iterator i = result.begin();

		const shared_ptr<RecognizerPointer> recognizptrLoaded1 = RecognizerPointer::loadVect(i, rcptrItBegin, rcptrItEnd);
		const shared_ptr<RecognizerPointer> recognizptrLoaded2 = RecognizerPointer::loadVect(i, rcptrItBegin, rcptrItEnd);
		const shared_ptr<RecognizerPointer> recognizptrLoaded3 = RecognizerPointer::loadVect(i, rcptrItBegin, rcptrItEnd);
		const shared_ptr<RecognizerPointer> recognizptrLoaded4 = RecognizerPointer::loadVect(i, rcptrItBegin, rcptrItEnd);
		const shared_ptr<RecognizerPointer> recognizptrLoaded5 = RecognizerPointer::loadVect(i, rcptrItBegin, rcptrItEnd);
		const shared_ptr<RecognizerPointer> recognizptrLoaded6 = RecognizerPointer::loadVect(i, rcptrItBegin, rcptrItEnd);
		const shared_ptr<RecognizerPointer> recognizptrLoaded7 = RecognizerPointer::loadVect(i, rcptrItBegin, rcptrItEnd);

		ifichier.close();

		cout << "*********VERIFIYING IF RECOGNIZERS MATCH*********" << endl;

		if(recognizptr1->equal(recognizptrLoaded1))  cout << "SUCESS : Character RecognizerPointer matches loaded" << endl;
	  else cout << "FAILED : Character RecognizerPointer does not match" << endl;
		if(recognizptr2->equal(recognizptrLoaded2))  cout << "SUCESS : Selector RecognizerPointer matches loaded" << endl;
	  else cout << "FAILED : Selector RecognizerPointer does not match" << endl;
		if(recognizptr3->equal(recognizptrLoaded3))  cout << "SUCESS : Loop RecognizerPointer matches loaded" << endl;
	  else cout << "FAILED : Loop RecognizerPointer does not match" << endl;
		if(recognizptr4->equal(recognizptrLoaded4))  cout << "SUCESS : Sequence RecognizerPointer matches loaded" << endl;
	  else cout << "FAILED : Sequence RecognizerPointer does not match" << endl;
		if(recognizptr5->equal(recognizptrLoaded5))  cout << "SUCESS : CharRange RecognizerPointer matches loaded" << endl;
	  else cout << "FAILED : CharRange RecognizerPointer does not match" << endl;
    if(recognizptr6->equal(recognizptrLoaded6))  cout << "SUCESS : Literal RecognizerPointer matches loaded" << endl;
	  else cout << "FAILED : Literal RecognizerPointer does not match" << endl;
		if(recognizptr7->equal(recognizptrLoaded7))  cout << "SUCESS : ExclusiveSelector RecognizerPointer matches loaded" << endl;
	  else cout << "FAILED : ExclusiveSelector RecognizerPointer does not match" << endl;


		cout << "*********FEEDING THE LOADED RECOGNIZERS*********" << endl;


		if (recognizptrLoaded1->feed(NULL, "a", 0) == 1)cout << "SUCESS : CharacterRecognizer RecognizerPointer feed successfull" << endl;
		else cout << "FAILED : CharacterRecognizer RecognizerPointer feed gone wrong" << endl;
		if ((recognizptrLoaded2->feed(NULL, "a", 0) == 1) && (recognizptrLoaded2->feed(NULL, "b", 0) == 1) && (recognizptrLoaded2->feed(NULL, "c", 0) == 1) && (recognizptrLoaded2->feed(NULL, "d", 0) == 1))cout << "SUCESS : Selector RecognizerPointer feed successfull" << endl;
		else cout << "FAILED :  Selector RecognizerPointer feed gone wrong" << endl;
		if (recognizptrLoaded3->feed(NULL, "a", 0) == 1)cout << "SUCESS : Loop RecognizerPointer feed successfull" << endl;
		else cout << "FAILED : Loop RecognizerPointer feed gone wrong" << endl;
		if (recognizptrLoaded4->feed(NULL, "abcd", 0) == 4)cout << "SUCESS : Sequence RecognizerPointer feed successfull" << endl;
		else cout << "FAILED : Sequence RecognizerPointer feed gone wrong" << endl;
		if ((recognizptrLoaded5->feed(NULL, "a", 0) == 1) && (recognizptrLoaded5->feed(NULL, "b", 0) == 1) && (recognizptrLoaded5->feed(NULL, "c", 0) == 1) && (recognizptrLoaded5->feed(NULL, "d", 0) == 1))cout << "SUCESS : CharRange RecognizerPointer feed successfull" << endl;
		else cout << "FAILED : CharRange RecognizerPointer feed gone wrong" << endl;
    if (recognizptrLoaded6->feed(NULL, "abcd", 0) == 4)cout << "SUCESS : Literal RecognizerPointer feed successfull" << endl;
		else cout << "FAILED : Literal RecognizerPointer feed gone wrong" << endl;
		if ((recognizptrLoaded7->feed(NULL, "a", 0) == 1) && (recognizptrLoaded7->feed(NULL, "b", 0) == 1) && (recognizptrLoaded7->feed(NULL, "c", 0) == 1) && (recognizptrLoaded7->feed(NULL, "d", 0) == 1))cout << "SUCESS : ExclusiveSelector RecognizerPointer feed successfull" << endl;
		else cout << "FAILED : ExclusiveSelector RecognizerPointer feed gone wrong" << endl;



}
