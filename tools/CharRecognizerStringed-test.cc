
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
#include <fstream>
#include <boost/tokenizer.hpp>
#include <string>


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
	ofstream ofichier ("test-char.bin", ios::out | ios::app | ios::binary);
  character->saveString(ofichier, savePosition);
  character2->saveString(ofichier, savePosition);
	ofichier.close();



	ifstream ifichier ("test-char.bin", ios::in | ios::binary);
  string contenu;  // déclaration d'une chaîne qui contiendra la ligne lue
  getline(ifichier, contenu);  // on met dans "contenu" la ligne
  typedef boost::tokenizer<boost::char_separator<char>>tokenizer;
  boost::char_separator<char> sep("|");
  tokenizer tokens(contenu, sep);
  tokenizer::iterator tok_iter = tokens.begin();
  string dataLoaded = *tok_iter;
	const shared_ptr<CharRecognizer> characterLoaded = CharRecognizer::loadString(dataLoaded);
  dataLoaded = *(++tok_iter);
	const shared_ptr<CharRecognizer> characterLoaded2 = CharRecognizer::loadString(dataLoaded);
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
