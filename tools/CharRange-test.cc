
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
	const shared_ptr<CharRange> stringBasic = dynamic_pointer_cast<CharRange>(Utils::char_range(97, 100));

	cout << "*********FEEDING THE ORIGINAL RECOGNIZERS*********" << endl;

	//verification avec feed
  if ((stringBasic->feed(NULL, "a", 0) == 1) && (stringBasic->feed(NULL, "b", 0) == 1) && (stringBasic->feed(NULL, "c", 0) == 1) && (stringBasic->feed(NULL, "d", 0) == 1))	cout << "SUCESS : CharRange feed successfull" << endl;
  else cout << "FAILED : CharRange feed gone wrong" << endl;



//SAVE AND LOAD
	ofstream ofichier ("test-char_range.txt", ios::out | ios::app);
  stringBasic->save(ofichier, savePosition);
	ofichier.close();

	ifstream ifichier("test-char_range.txt", ios::in);
	const shared_ptr<CharRange> stringBasicLoaded = CharRange::load(ifichier, loadPosition);
	ifichier.close();

//verification du save and mload
		cout << "*********VERIFIYING IF RECOGNIZERS MATCH*********" << endl;

		if(stringBasic->equal(stringBasicLoaded)) cout << "SUCESS : CharRange matches loaded" << endl;
  	else cout << "FAILED : CharRange doesnt match loaded" << endl;

		cout << "*********FEEDING THE LOADED RECOGNIZERS*********" << endl;


	if ((stringBasicLoaded->feed(NULL, "a",0) == 1) && (stringBasicLoaded->feed(NULL, "b",0) == 1) && (stringBasicLoaded->feed(NULL, "c",0) == 1) && (stringBasicLoaded->feed(NULL, "d",0) == 1)){
		cout << "SUCESS : loaded charrange feed successfull" << endl;
  }
  else{
    cout << "FAILED : loaded charrange feed gone wrong" << endl;
  }


}
