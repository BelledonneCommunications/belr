
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


using namespace belr;
using namespace std;


long savePosition = 0;
long loadPosition = 0;

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

//Sequence with CharRecognizer
	shared_ptr<Sequence> sequence1 = Foundation::sequence();
	shared_ptr<CharRecognizer> caractere1 = Foundation::charRecognizer('a', false);
  shared_ptr<CharRecognizer> caractere2 = Foundation::charRecognizer('b', false);
  shared_ptr<CharRecognizer> caractere3 = Foundation::charRecognizer('c', false);
  shared_ptr<CharRecognizer> caractere4 = Foundation::charRecognizer('d', false);
  sequence1->addRecognizer(caractere1);
  sequence1->addRecognizer(caractere2);
  sequence1->addRecognizer(caractere3);
  sequence1->addRecognizer(caractere4);

//Sequence with a Selector
	shared_ptr<Sequence> sequence2 = Foundation::sequence();
	shared_ptr<Selector> selector1 = Foundation::selector(false);
	shared_ptr<CharRecognizer> characterSel1 = Foundation::charRecognizer('a', false);
  shared_ptr<CharRecognizer> characterSel2 = Foundation::charRecognizer('b', false);
  shared_ptr<CharRecognizer> characterSel3 = Foundation::charRecognizer('c', false);
  shared_ptr<CharRecognizer> characterSel4 = Foundation::charRecognizer('d', false);
  selector1->addRecognizer(characterSel1);
  selector1->addRecognizer(characterSel2);
  selector1->addRecognizer(characterSel3);
  selector1->addRecognizer(characterSel4);
	sequence2->addRecognizer(selector1);

//Sequence with an ExclusiveSelector
	shared_ptr<Sequence> sequence3 = Foundation::sequence();
	shared_ptr<Selector> selector2 = Foundation::selector(true);
	shared_ptr<CharRecognizer> characterExSel1 = Foundation::charRecognizer('a', false);
	shared_ptr<CharRecognizer> characterExSel2 = Foundation::charRecognizer('b', false);
	shared_ptr<CharRecognizer> characterExSel3 = Foundation::charRecognizer('c', false);
	shared_ptr<CharRecognizer> characterExSel4 = Foundation::charRecognizer('d', false);
	selector2->addRecognizer(characterSel1);
	selector2->addRecognizer(characterSel2);
	selector2->addRecognizer(characterSel3);
	selector2->addRecognizer(characterSel4);
	sequence3->addRecognizer(selector2);

//Sequence with Loop
	shared_ptr<Sequence> sequence4 = Foundation::sequence();
	shared_ptr<Loop> loopCar = Foundation::loop();
	shared_ptr<CharRecognizer> characterLoop2 = Foundation::charRecognizer('a', false);
	loopCar->setRecognizer(characterLoop2, 0, 3);
	sequence4->addRecognizer(loopCar);


//Sequence with a CharRange
	shared_ptr<Sequence> sequence5 = Foundation::sequence();
	const shared_ptr<CharRange> charRange = dynamic_pointer_cast<CharRange>(Utils::char_range(97, 100));
	sequence5->addRecognizer(charRange);

//Sequence with a Literal
	shared_ptr<Sequence> sequence6 = Foundation::sequence();
	const shared_ptr<Literal> stringBasic = dynamic_pointer_cast<Literal>(Utils::literal("abcd"));
	sequence6->addRecognizer(stringBasic);

//Sequence with a RecognizerPointer
	shared_ptr<Sequence> sequence7 = Foundation::sequence();
	shared_ptr<RecognizerPointer> recognizptrCar = make_shared<RecognizerPointer>();
	shared_ptr<CharRecognizer> characterRecognizerPtr2 = Foundation::charRecognizer('a', false);
	recognizptrCar->setPointed(characterRecognizerPtr2);
	sequence7->addRecognizer(recognizptrCar);

	cout << "*********FEEDING THE ORIGINAL RECOGNIZERS*********" << endl;




	if( sequence1->feed(NULL, "abcd", 0) == 4) cout << "SUCESS : 4 CharRecognizer Sequence feed successfull" << endl;
	else cout << "FAILED : 4 CharRecognizer Sequence feed gone wrong" << endl;
	if( (sequence2->feed(NULL, "a", 0) == 1) && (sequence2->feed(NULL, "b", 0) == 1) && (sequence2->feed(NULL, "c", 0) == 1) && (sequence2->feed(NULL, "d", 0) == 1)) cout << "SUCESS : Selector Sequence feed successfull" << endl;
  else cout << "FAILED : Selector Sequence feed gone wrong" << endl;
	if( (sequence3->feed(NULL, "a", 0) == 1) && (sequence3->feed(NULL, "b", 0) == 1) && (sequence3->feed(NULL, "c", 0) == 1) && (sequence3->feed(NULL, "d", 0) == 1)) cout << "SUCESS : ExclusiveSelector Sequence feed successfull" << endl;
  else cout << "FAILED : ExclusiveSelector Sequence feed gone wrong" << endl;
	if( sequence4->feed(NULL, "a", 0) == 1) cout << "SUCESS : Loop Sequence feed successfull" << endl;
	else cout << "FAILED : Loop Sequence feed gone wrong" << endl;
	if( (sequence5->feed(NULL, "a", 0) == 1) && (sequence5->feed(NULL, "b", 0) == 1) && (sequence5->feed(NULL, "c", 0) == 1) && (sequence5->feed(NULL, "d", 0) == 1)) cout << "SUCESS : CharRange Sequence feed successfull" << endl;
	else cout << "FAILED : CharRange Sequence feed gone wrong" << endl;
	if( sequence6->feed(NULL, "abcd", 0) == 4) cout << "SUCESS : Literal Sequence feed successfull" << endl;
	else cout << "FAILED : Literal Sequence feed gone wrong" << endl;
	if( sequence7->feed(NULL, "a", 0) == 1) cout << "SUCESS : RecognizerPointer Sequence feed successfull" << endl;
	else cout << "FAILED : RecognizerPointer Sequence feed gone wrong" << endl;





  //SAVE AND LOAD
	ofstream ofichier ("test-seq.bin", ios::out | ios::app);
  sequence1->save(ofichier, savePosition);
	sequence2->save(ofichier, savePosition);
	sequence3->save(ofichier, savePosition);
  sequence4->save(ofichier, savePosition);
	sequence5->save(ofichier, savePosition);
	sequence6->save(ofichier, savePosition);
	sequence7->save(ofichier, savePosition);

	ofichier.close();

	ifstream ifichier ("test-seq.bin", ios::in);
  string contenu;  // déclaration d'une chaîne qui contiendra la ligne lue
  getline(ifichier, contenu);  // on met dans "contenu" la ligne

  std::vector<std::string> result = split(contenu, ' ');
  vector<string>::const_iterator i = result.begin();
	const shared_ptr<Sequence> sequenceLoaded1 = Sequence::loadVect(i);
	const shared_ptr<Sequence> sequenceLoaded2 = Sequence::loadVect(i);
	const shared_ptr<Sequence> sequenceLoaded3 = Sequence::loadVect(i);
	const shared_ptr<Sequence> sequenceLoaded4 = Sequence::loadVect(i);
	const shared_ptr<Sequence> sequenceLoaded5 = Sequence::loadVect(i);
	const shared_ptr<Sequence> sequenceLoaded6 = Sequence::loadVect(i);
	const shared_ptr<Sequence> sequenceLoaded7 = Sequence::loadVect(i);

	ifichier.close();


	cout << "*********VERIFIYING IF RECOGNIZERS MATCH*********" << endl;


	if(sequence1->equal(sequenceLoaded1)) cout << "SUCESS : original 4 CharRecognizer Sequence matches loaded" << endl;
  else cout << "FAILED : original sequence does not match" << endl;
	if(sequence2->equal(sequenceLoaded2)) cout << "SUCESS : original Selector Sequence matches loaded" << endl;
  else cout << "FAILED : original sequence does not match" << endl;
	if(sequence3->equal(sequenceLoaded3)) cout << "SUCESS : original ExclusiveSelector Sequence matches loaded" << endl;
  else cout << "FAILED : original sequence does not match" << endl;
	if(sequence4->equal(sequenceLoaded4)) cout << "SUCESS : original Loop Sequence matches loaded" << endl;
  else cout << "FAILED : original sequence does not match" << endl;
	if(sequence5->equal(sequenceLoaded5)) cout << "SUCESS : original CharRange Sequence matches loaded" << endl;
  else cout << "FAILED : original sequence does not match" << endl;
	if(sequence6->equal(sequenceLoaded6)) cout << "SUCESS : original Literal Sequence matches loaded" << endl;
  else cout << "FAILED : original sequence does not match" << endl;
	if(sequence7->equal(sequenceLoaded7)) cout << "SUCESS : original RecognizerPointer Sequence matches loaded" << endl;
  else cout << "FAILED : original sequence does not match" << endl;



	cout << "*********FEEDING THE LOADED RECOGNIZERS*********" << endl;


	if( (sequenceLoaded1->feed(NULL, "abcd", 0) == 4)) cout << "SUCESS : 4 CharRecognizer Loaded Sequence feed successfull" << endl;
	else cout << "FAILED : 4 CharRecognizer Loaded Sequence feed gone wrong" << endl;
	if( (sequenceLoaded2->feed(NULL, "a", 0) == 1) && (sequenceLoaded2->feed(NULL, "b", 0) == 1) && (sequenceLoaded2->feed(NULL, "c", 0) == 1) && (sequenceLoaded2->feed(NULL, "d", 0) == 1)) cout << "SUCESS : Selector Loaded Sequence feed successfull" << endl;
	else cout << "FAILED : Selector Loaded Sequence feed gone wrong" << endl;
	if( (sequenceLoaded3->feed(NULL, "a", 0) == 1) && (sequenceLoaded3->feed(NULL, "b", 0) == 1) && (sequenceLoaded3->feed(NULL, "c", 0) == 1) && (sequenceLoaded3->feed(NULL, "d", 0) == 1)) cout << "SUCESS : ExclusiveSelector Loaded Sequence feed successfull" << endl;
	else cout << "FAILED : ExclusiveSelector Loaded Sequence feed gone wrong" << endl;
	if( sequenceLoaded4->feed(NULL, "a", 0) == 1) cout << "SUCESS : Loopp Loaded Sequence feed successfull" << endl;
	else cout << "FAILED : Loop Loaded Sequence feed gone wrong" << endl;
	if( (sequenceLoaded5->feed(NULL, "a", 0) == 1) && (sequenceLoaded5->feed(NULL, "b", 0) == 1) && (sequenceLoaded5->feed(NULL, "c", 0) == 1) && (sequenceLoaded5->feed(NULL, "d", 0) == 1)) cout << "SUCESS : CharRange Loaded Sequence feed successfull" << endl;
	else cout << "FAILED : CharRange Loaded Sequence feed gone wrong" << endl;
	if( sequenceLoaded6->feed(NULL, "abcd", 0) == 4) cout << "SUCESS : Literal Loaded feed successfull" << endl;
	else cout << "FAILED : Literal Loaded Sequence feed gone wrong" << endl;
	if( sequenceLoaded7->feed(NULL, "a", 0) == 1) cout << "SUCESS : RecognizerPointer Loaded Sequence feed successfull" << endl;
	else cout << "FAILED : RecognizerPointer Loaded Sequence feed gone wrong" << endl;


}
