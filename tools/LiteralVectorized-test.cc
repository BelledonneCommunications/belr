
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
	ofstream ofichier ("test-lit.bin", ios::out | ios::app);
  chaine->save(ofichier ,savePosition);
  ofichier.close();

  ifstream ifichier ("test-lit.bin", ios::in);
  string contenu;  // déclaration d'une chaîne qui contiendra la ligne lue
  getline(ifichier, contenu);  // on met dans "contenu" la ligne

  std::vector<std::string> result = split(contenu, ' ');
  vector<string>::const_iterator i = result.begin();

	const shared_ptr<Literal> chaine_loaded = Literal::loadVect(i);
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
