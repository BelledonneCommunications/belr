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

  cout << "DEBUG : grammar parsed no seg fault yet" << endl;
  if (!grammar){
  cerr<<"Could not build grammar from basicgrammar.txt"<<endl;
  return -1;
  }


//  cout << "************DEBUG original grammar*******************"<< endl;
  //grammar->debugGrammar();
//  cout << "************DONE DEBUG original grammar*******************"<< endl;

  if(remove("grammarDump.bin")){cerr << "Problem removing previous dump" << endl;}
  //Save grammar
  grammar->createGrammarDump("grammarDump.bin");

  cout << "DEBUG : grammar dump created no seg fault yet" << endl;


  //Load grammar
  start = std::chrono::high_resolution_clock::now();
  shared_ptr<Grammar> loadedGram = Grammar::loadVectRulesMap("grammarDump.bin");
  finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsedSecond = finish - start;

  cout << "DEBUG : grammar dump loaded no seg fault yet" << endl;
  //cout << "************DEBUG loaded grammar*******************"<< endl;
  //loadedGram->debugGrammar();
  //Verification and output of the results (Match , indvidual parsing time and optimisation ratio)
  bool isItOk = false;
  isItOk = grammar->equal(loadedGram);
  string isItAMatch = isItOk ? "MATCH" : "NO MATCH";
  cout << "Grammars loaded ,compare result : " << isItAMatch << endl;
  cout << "Elapsed time for initial parsing :" << elapsedFirst.count() << " VS second parsing :" << elapsedSecond.count() << endl;
  cout << "Optimisation ratio : " << (elapsedFirst.count() / elapsedSecond.count())*100 << "%"<< endl;
/*
  cout << "*********************ORIGINAL GRAMMAR BUMP******************" << endl;
  grammar->debugGrammar();
  cout << "*********************LOADED GRAMMAR BUMP******************" << endl;
  loadedGram->debugGrammar();
*/
}
