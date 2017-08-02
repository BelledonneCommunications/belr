
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


	//Loop with one CharRecognizer
 	shared_ptr<Loop> loopCar = Foundation::loop();
	shared_ptr<CharRecognizer> characterLoop1 = Foundation::charRecognizer('a', false);
  loopCar->setRecognizer(characterLoop1, 0, 3);

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

	//Loop with one CharRange
	shared_ptr<Loop> loopCharRange = Foundation::loop();
	const shared_ptr<CharRange> characRange = dynamic_pointer_cast<CharRange>(Utils::char_range(97, 100));
	loopCharRange->setRecognizer(characRange, 0, 3);

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


	//Loop with one Literal
	shared_ptr<Loop> loopLiteral = Foundation::loop();
	const shared_ptr<Literal> stringBasic = dynamic_pointer_cast<Literal>(Utils::literal("abcd"));
	loopLiteral->setRecognizer(stringBasic,0,3);

	//Loop with one Recognizer Pointer
	shared_ptr<Loop> loopRecPointer = Foundation::loop();
	shared_ptr<RecognizerPointer> recognizptrCar = make_shared<RecognizerPointer>();
  recognizptrCar->setName("@rule");
	shared_ptr<CharRecognizer> characterRecognizerPtr2 = Foundation::charRecognizer('a', false);
	recognizptrCar->setPointed(characterRecognizerPtr2);
	loopRecPointer->setRecognizer(recognizptrCar,0,3);
  mRecognizerPointersLoaded.push_back(recognizptrCar);
  rcptrItEnd = (mRecognizerPointersLoaded.end());
  rcptrItBegin = (mRecognizerPointersLoaded.begin());

	cout << "*********FEEDING THE ORIGINAL RECOGNIZERS*********" << endl;

  if (loopCar->feed(NULL, "a", 0) == 1) cout << "SUCESS : 4 CharacterRecognize Loop feed successfull" << endl;
  else cout << "FAILED : 4 CharacterRecognize Loop feed gone wrong" << endl;
  //PROBLEM
  if ((loopSel->feed(NULL, "a", 0) == 1) && (loopSel->feed(NULL, "b", 0) == 1) && (loopSel->feed(NULL, "c", 0) == 1) && (loopSel->feed(NULL, "d", 0) == 1)) cout << "SUCESS : Selector Loop feed successfull" << endl;
  else cout << "FAILED :  Selector Loop feed gone wrong" << endl;
  if ((loopExSel->feed(NULL, "a", 0) == 1) && (loopExSel->feed(NULL, "b", 0) == 1) && (loopExSel->feed(NULL, "c", 0) == 1) && (loopExSel->feed(NULL, "d", 0) == 1)) cout << "SUCESS : Exclusive Selector loop feed successfull" << endl;
  else cout << "FAILED : Exclusive Selector Loop feed gone wrong" << endl;
  //PROBLEM
  if ((loopCharRange->feed(NULL, "a", 0) == 1) && (loopCharRange->feed(NULL, "b", 0) == 1) && (loopCharRange->feed(NULL, "c", 0) == 1) && (loopCharRange->feed(NULL, "d", 0) == 1)) cout << "SUCESS : CharRange Loop feed successfull" << endl;
  else cout << "FAILED : CharRange Loop feed gone wrong" << endl;
  if (loopSequence->feed(NULL, "abcd", 0) == 4)cout << "SUCESS : Sequence Loop feed successfull" << endl;
  else cout << "FAILED : Sequence Loop feed gone wrong" << endl;
  if (loopLiteral->feed(NULL, "abcd", 0) == 4)cout << "SUCESS : Literal Loop feed successfull" << endl;
  else cout << "FAILED : Literal Loop feed gone wrong" << endl;
  if (loopRecPointer->feed(NULL, "a", 0) == 1)cout << "SUCESS : RecognizerPointer Loop feed successfull" << endl;
  else cout << "FAILED : RecognizerPointer Loop feed gone wrong" << endl;

  remove("test-loop.bin");

//SAVE AND LOAD
	ofstream ofichier ("test-loop.bin", ios::out | ios::app);
  loopCar->save(ofichier, savePosition);
	loopSel->save(ofichier, savePosition);
	loopExSel->save(ofichier, savePosition);
  loopSequence->save(ofichier, savePosition);
  loopLiteral->save(ofichier, savePosition);
  loopCharRange->save(ofichier, savePosition);
	loopRecPointer->save(ofichier, savePosition);
	ofichier.close();


	ifstream ifichier ("test-loop.bin", ios::in);
  string contenu;  // déclaration d'une chaîne qui contiendra la ligne lue
  getline(ifichier, contenu);  // on met dans "contenu" la ligne

  std::vector<std::string> result = split(contenu, ' ');
  vector<string>::const_iterator i = result.begin();

	const shared_ptr<Loop> loopCarLoaded = Loop::loadVect(i, rcptrItBegin, rcptrItEnd);
	const shared_ptr<Loop> loopSelLoaded = Loop::loadVect(i, rcptrItBegin, rcptrItEnd);
	const shared_ptr<Loop> loopExSelLoaded = Loop::loadVect(i, rcptrItBegin, rcptrItEnd);
	const shared_ptr<Loop> loopSequenceLoaded = Loop::loadVect(i, rcptrItBegin, rcptrItEnd);
	const shared_ptr<Loop> loopLiteralLoaded = Loop::loadVect(i, rcptrItBegin, rcptrItEnd);
  const shared_ptr<Loop> loopCharRangeLoaded = Loop::loadVect(i, rcptrItBegin, rcptrItEnd);
  const shared_ptr<Loop> loopRecPointerLoaded = Loop::loadVect(i, rcptrItBegin, rcptrItEnd);

	ifichier.close();

	cout << "*********VERIFIYING IF RECOGNIZERS MATCH*********" << endl;


		if(loopCar->equal(loopCarLoaded)) cout << "SUCESS : Character Loop matches loaded" << endl;
  	else cout << "FAILED : first character doesnt match loaded" << endl;
	  if(loopSel->equal(loopSelLoaded)) cout << "SUCESS :  Selector Loop matches loaded" << endl;
		else cout << "FAILED : first character doesnt match loaded" << endl;
		if(loopExSel->equal(loopExSelLoaded)) cout << "SUCESS : ExclusiveSelector Loop matches loaded" << endl;
		else cout << "FAILED : first character doesnt match loaded" << endl;
    //cout << " no seg fault in exsel test" << endl;
		if(loopCharRange->equal(loopCharRangeLoaded)) cout << "SUCESS : CharRange Loop matches loaded" << endl;
		else cout << "FAILED : first character doesnt match loaded" << endl;
		if(loopSequence->equal(loopSequenceLoaded)) cout << "SUCESS : Sequence Loop matches loaded" << endl;
		else cout << "FAILED : first character doesnt match loaded" << endl;
		if(loopLiteral->equal(loopLiteralLoaded)) cout << "SUCESS : Literal Loop matches loaded" << endl;
		else cout << "FAILED : first character doesnt match loaded" << endl;
		if(loopRecPointer->equal(loopRecPointerLoaded)) cout << "SUCESS : Recognizer Pointer Loop matches loaded" << endl;
		else cout << "FAILED : Recognizer Pointer Loop doesnt match loaded" << endl;


		cout << "*********FEEDING THE LOADED RECOGNIZERS*********" << endl;

    if (loopCarLoaded->feed(NULL, "a", 0) == 1) cout << "SUCESS : 4 CharacterRecognize Loop feed successfull" << endl;
    else cout << "FAILED : 4 CharacterRecognize Loop feed gone wrong" << endl;
    //PROBLEM
    if ((loopSelLoaded->feed(NULL, "a", 0) == 1) && (loopSelLoaded->feed(NULL, "b", 0) == 1) && (loopSelLoaded->feed(NULL, "c", 0) == 1) && (loopSelLoaded->feed(NULL, "d", 0) == 1))cout << "SUCESS : Selector Loop feed successfull" << endl;
    else cout << "FAILED :  Selector Loop feed gone wrong" << endl;
    if ((loopExSelLoaded->feed(NULL, "a", 0) == 1) && (loopExSelLoaded->feed(NULL, "b", 0) == 1) && (loopExSelLoaded->feed(NULL, "c", 0) == 1) && (loopExSelLoaded->feed(NULL, "d", 0) == 1))cout << "SUCESS : Exclusive Selector loop feed successfull" << endl;
    else cout << "FAILED : Exclusive Selector Loop feed gone wrong" << endl;
    //PROBLEM
    if ((loopCharRangeLoaded->feed(NULL, "a", 0) == 1) && (loopCharRangeLoaded->feed(NULL, "b", 0) == 1) && (loopCharRangeLoaded->feed(NULL, "c", 0) == 1) && (loopCharRangeLoaded->feed(NULL, "d", 0) == 1)) cout << "SUCESS : CharRange Loop feed successfull" << endl;
    else cout << "FAILED : CharRange Loop feed gone wrong" << endl;
    if (loopSequenceLoaded->feed(NULL, "abcd", 0) == 4)cout << "SUCESS : Sequence Loop feed successfull" << endl;
    else cout << "FAILED : Sequence Loop feed gone wrong" << endl;
    if (loopLiteralLoaded->feed(NULL, "abcd", 0) == 4)cout << "SUCESS : Literal Loop feed successfull" << endl;
    else cout << "FAILED : Literal Loop feed gone wrong" << endl;
    cout << loopRecPointerLoaded->feed(NULL, "a", 0) << endl;
    if (loopRecPointerLoaded->feed(NULL, "a", 0) == 1)cout << "SUCESS : RecognizerPointer Loop feed successfull" << endl;
    else cout << "FAILED : RecognizerPointer Loop feed gone wrong" << endl;


}
