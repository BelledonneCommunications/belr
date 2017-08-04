#include "belr/grammarbuilder.h"
#include "belr/abnf.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <cstring>


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

shared_ptr<Grammar> grammar=builder.createFromAbnfFile(grammarToParse, make_shared<CoreRules>());
//Read grammar put it in object grammar

if (!grammar){
  cerr<<"Could not build grammar from basicgrammar.txt"<<endl;
  return -1;
}
  grammar->debugGrammar();
}

//
