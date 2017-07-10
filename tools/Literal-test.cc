
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


const shared_ptr<Literal> chaine = dynamic_pointer_cast<Literal>(Utils::literal("abcd"));

  cout << "*********FEEDING THE ORIGINAL RECOGNIZERS*********" << endl;

	//verification avec feed
  if (chaine->feed(NULL, "abcd", 0) == 4) {
		cout << "SUCESS : Literal feed successfull" << endl;
  }
  else{
    cout << "FAILED : Literal feed gone wrong" << endl;
  }



//SAVE AND LOAD
	ofstream ofichier ("test-lit.txt", ios::out | ios::app);
  chaine->save(ofichier ,savePosition);
  ofichier.close();
  ifstream ifichier ("test-lit.txt", ios::in);
	const shared_ptr<Literal> chaine_loaded = Literal::load(ifichier, loadPosition);
  ifichier.close();

//verification du save and mload
cout << "*********VERIFIYING IF RECOGNIZERS MATCH*********" << endl;

		if(chaine->equal(chaine_loaded)){
    cout << "SUCESS : Literal matches loaded" << endl;
  }
  else{
    cout << "FAILED : Literal doesnt match loaded" << endl;
  }

  cout << "*********FEEDING THE LOADED RECOGNIZERS*********" << endl;


	if (chaine_loaded->feed(NULL, "abcd",0) == 4){
		cout << "SUCESS : Loaded Literal feed successfull" << endl;
  }
  else{
    cout << "FAILED : Loaded Literal feed gone wrong" << endl;
  }



}
