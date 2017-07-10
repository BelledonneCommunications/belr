#include "belr/grammarbuilder.hh"
#include "belr/abnf.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <cstring>
#include <chrono>

#include "belr/parser-impl.cc" //this file is to be included only in the file where the parser is instanciated, for template instanciation.

using namespace::belr;
using namespace::std;


int main(int argc, char *argv[]){

  string grammarToParse;

	if (argc<2){
		cerr<<argv[0]<<" <grammar file to parse>"<<endl;
		return -1;
	}
	grammarToParse = argv[1];

  ABNFGrammarBuilder builder;

//Read grammar put it in object grammar
  auto start = std::chrono::high_resolution_clock::now();
  shared_ptr<Grammar> grammar=builder.createFromAbnfFile(grammarToParse, make_shared<CoreRules>());
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsedFirst = finish - start;


if (!grammar){
  cerr<<"Could not build grammar from basicgrammar.txt"<<endl;
  return -1;
}
  cout << "DEBUG : no seg fault yet" << endl;
  if(remove("grammarDump.txt")){cerr << "Problem removing previous dump" << endl;}
  grammar->createGrammarDump("grammarDump.txt");

  cout << "DEBUG : no seg fault yet" << endl;
  //Load grammar
  start = std::chrono::high_resolution_clock::now();
  shared_ptr<Grammar> loadedGram = Grammar::createLoadedGrammar("grammarDump.txt");
  finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsedSecond = finish - start;

  bool isItOk = false;
  isItOk = grammar->equal(loadedGram);
  cout << "Grammars loaded ,compare result : " << isItOk << endl;
  cout << "Elapsed time for initial parsing :" << elapsedFirst.count() << " VS second parsing :" << elapsedSecond.count() << endl;
}

//
