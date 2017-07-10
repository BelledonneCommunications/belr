
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




int main(int argc, char *argv[]){
  // WHAT IS PARSER CONTEXT BASER : objet necessaire pour l'utilisation de feed
  // WHAT IS STRING input : string a comparer (partant du principe que )
  // WHAT IS pos : position du caractère (je sais pas ou maybe dans la grammaire ? maybe dans le fichier de test ? )

	shared_ptr<CharRecognizer> caractere = Foundation::charRecognizer('a', false);
	shared_ptr<CharRecognizer> caractere2 = Foundation::charRecognizer('b', false);


  if (caractere->feed(NULL, "a", 0) == 1) {
		cout << "first success" << endl;
  }
  else{
    cout << "fail" << endl;
  }

	if (caractere2->feed(NULL, "b", 0) == 1) {
		cout << "first success" << endl;
	}
	else{
		cout << "fail" << endl;
	}
  //waaht is feed ?

	//no viable conversion from 'belr::CharRecognizer' to 'const std::shared_ptr<CharRecognizer>'

//SAVE AND LOAD
  caractere->Save("test.txt");
	shared_ptr<CharRecognizer> caractere_loaded = caractere->Load("test.txt");

	caractere2->Save("test.txt");
	shared_ptr<CharRecognizer> caractere2_loaded = caractere->Load("test.txt");


	if(caractere->equal(caractere_loaded)){
    cout << "second success" << endl;
  }
  else{
    cout << "fail" << endl;
  }

	if (caractere_loaded->feed(NULL, "a",0)){
		cout << "third success" << endl;
  }
  else{
    cout << "fail" << endl;
  }

	if(caractere2->equal(caractere2_loaded)){
		cout << "second success" << endl;
	}
	else{
		cout << "fail" << endl;
	}

	if (caractere2_loaded->feed(NULL, "b",0)){
		cout << "third success" << endl;
	}
	else{
		cout << "fail" << endl;
	}
}
