
#include "belr/grammarbuilder.h"
#include "belr/abnf.h"
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


		//Create two charrecognizers
	const shared_ptr<CharRange> stringBasic = dynamic_pointer_cast<CharRange>(Utils::char_range(97, 100));

	cout << "*********FEEDING THE ORIGINAL RECOGNIZERS*********" << endl;

	//verification avec feed
  if ((stringBasic->feed(NULL, "a", 0) == 1) && (stringBasic->feed(NULL, "b", 0) == 1) && (stringBasic->feed(NULL, "c", 0) == 1) && (stringBasic->feed(NULL, "d", 0) == 1))	cout << "SUCESS : CharRange feed successfull" << endl;
  else cout << "FAILED : CharRange feed gone wrong" << endl;



//SAVE AND LOAD
	ofstream ofichier ("test-char_range.bin", ios::out | ios::app);
  stringBasic->save(ofichier, savePosition);
	ofichier.close();

	ifstream ifichier("test-char_range.bin", ios::in);
  string contenu;  // déclaration d'une chaîne qui contiendra la ligne lue
  getline(ifichier, contenu);  // on met dans "contenu" la ligne

  std::vector<std::string> result = split(contenu, ' ');
  vector<string>::const_iterator i = result.begin();

	const shared_ptr<CharRange> stringBasicLoaded = CharRange::loadVect(i);
	ifichier.close();

//verification du save and mload
		cout << "*********VERIFIYING IF RECOGNIZERS MATCH*********" << endl;

		if(*stringBasic.get() == *stringBasicLoaded.get()) cout << "SUCESS : CharRange matches loaded" << endl;
  	else cout << "FAILED : CharRange doesnt match loaded" << endl;
    

		cout << "*********FEEDING THE LOADED RECOGNIZERS*********" << endl;


	if ((stringBasicLoaded->feed(NULL, "a",0) == 1) && (stringBasicLoaded->feed(NULL, "b",0) == 1) && (stringBasicLoaded->feed(NULL, "c",0) == 1) && (stringBasicLoaded->feed(NULL, "d",0) == 1)){
		cout << "SUCESS : loaded charrange feed successfull" << endl;
  }
  else{
    cout << "FAILED : loaded charrange feed gone wrong" << endl;
  }


}
