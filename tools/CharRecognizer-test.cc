
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


int main(int argc, char *argv[]){


		//Create two charrecognizers
	shared_ptr<CharRecognizer> character = Foundation::charRecognizer('b', false);
	shared_ptr<CharRecognizer> character2 = Foundation::charRecognizer('a', false);

	cout << "*********FEEDING THE ORIGINAL RECOGNIZERS*********" << endl;

	//verification avec feed
  if (character->feed(NULL, "b", 0) == 1) cout << "SUCESS : first feed successfull" << endl;
  else cout << "FAILED : first feed gone wrong" << endl;

	if (character2->feed(NULL, "a", 0) == 1) cout << "SUCESS : second feed successfull" << endl;
	else cout << "FAILED : second feed gone wrong" << endl;


//SAVE AND LOAD
	ofstream ofichier ("test-char.txt", ios::out | ios::app);
  character->save(ofichier, savePosition);
	ofichier.close();

	ifstream ifichier ("test-char.txt", ios::in);
	const shared_ptr<CharRecognizer> characterLoaded = CharRecognizer::load(ifichier, loadPosition);
	ifichier.close();

	ofichier = ofstream("test-char.txt", ios::out | ios::app);

	character2->save(ofichier, savePosition);
	ofichier.close();

	ifichier = ifstream("test-char.txt", ios::in);
	const shared_ptr<CharRecognizer> characterLoaded2 = CharRecognizer::load(ifichier, loadPosition);
	ifichier.close();

	cout << "*********VERIFIYING IF RECOGNIZERS MATCH*********" << endl;

	if(character->equal(characterLoaded)) cout << "SUCESS : first character matches loaded" << endl;
  else cout << "FAILED : first character doesnt match loaded" << endl;

	if(character2->equal(characterLoaded2)) cout << "SUCESS : second character matches loaded" << endl;
	else cout << "FAILED : second character doesnt match loaded" << endl;

	cout << "*********FEEDING THE LOADED RECOGNIZERS*********" << endl;

	if (characterLoaded->feed(NULL, "b",0)) cout << "SUCESS : first loaded character feed successfull" << endl;
  else cout << "FAILED : first loaded character feed gone wrong" << endl;

	if (characterLoaded2->feed(NULL, "a",0)) cout << "SUCESS : second loaded character feed successfull" << endl;
	else cout << "FAILED : second loaded character feed gone wrong" << endl;

}
