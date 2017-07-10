#include "belr/grammarbuilder.hh"
#include "belr/abnf.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <cstring>

#include "belr/parser-impl.cc" //this file is to be included only in the file where the parser is instanciated, for template instanciation.

using namespace::belr;
using namespace::std;


int main(int argc, char *argv[]){


  ABNFGrammarBuilder builder;
  cout << "DEBUG : no seg fault yet : builder created" << endl;

shared_ptr<Grammar> grammar=builder.createFromAbnfFile("basicgrammar.txt", make_shared<CoreRules>());
//Read grammar put it in object grammar
cout << "DEBUG : no seg fault yet : grammar parse done" << endl;

if (!grammar){
  cerr<<"Could not build grammar from basicgrammar.txt"<<endl;
  return -1;
}

ofstream ofichier ("test-grammar.txt", ios::out | ios::app);
cout << "DEBUG : no seg fault yet : file opened" << endl;
grammar->saveRulesMap(ofichier);
cout << "DEBUG : no seg fault yet : map saved" << endl;

ofichier.close();
//call save function on the object created

//call load fucntion on a new object
ifstream ifichier ("test-grammar.txt", ios::in);
std::map<std::string,std::shared_ptr<Recognizer>> basicRules = Grammar::loadRulesMap(ifichier);
ifichier.close();

for(map<string,shared_ptr<Recognizer>>::iterator it= basicRules.begin(); it != basicRules.end() ;it++){
    cout << "DEBUG : read key : " << it->first << endl;
}

}

//
