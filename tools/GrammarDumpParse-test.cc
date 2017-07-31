


#include "belr/grammarbuilder.hh"
#include "belr/abnf.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <cstring>
#include <cstdlib>

using namespace belr;
using namespace std;

int main(int argc, char *argv[]){
	const char *file=NULL,*message_file=NULL;
	int rules_first=0;
	int i;
	int repeat_count=1;
	if (argc<2){
		cerr<<argv[0]<< " [--repeat <count>] <grammarfile-to-load> - test an abnf and instanciate the parser"<<endl;
		cerr<<argv[0]<< " [--repeat <count>] <grammarfile-to-load> <input file to parse> <entry rule> [rule1] [rule2]..."<<endl;
		return -1;
	}
	for(i=1;i<argc;++i){
		if (strcmp(argv[i],"--repeat")==0){
			++i;
			if (i<argc){
				repeat_count=atoi(argv[i]);
			}
		}else{
			file=argv[i];
			++i;
			if (i<argc)
				message_file=argv[i];
			break;
		}
	}
	rules_first=i+1;

  auto start = std::chrono::high_resolution_clock::now();
	ABNFGrammarBuilder builder;
	shared_ptr<Grammar> grammar=make_shared<Grammar>(file);
	grammar->include(make_shared<CoreRules>());
	grammar = builder.createFromAbnfFile(file,grammar);
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsedFirst = finish - start;



  if(remove("grammarDump.bin")){cerr << "Problem removing previous dump" << endl;}
  //Save grammar
  grammar->createGrammarDump("grammarDump.bin");

  cout << "SUCCESS : Grammar dump created" << endl;


  //Load grammar
  start = std::chrono::high_resolution_clock::now();
  shared_ptr<Grammar> grammarReloaded = Grammar::loadVectRulesMap("grammarDump.bin");
  finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsedSecond = finish - start;

  cout << "SUCCESS : Grammar dump loaded " << endl;
  //cout << "************DEBUG loaded grammar*******************"<< endl;
  //loadedGram->debugGrammar();
  //Verification and output of the results (Match , indvidual parsing time and optimisation ratio)
  bool isItOk = false;
  isItOk = grammar->equal(grammarReloaded);
  string isItAMatch = isItOk ? "MATCH" : "NO MATCH";
  cout << "Grammars loaded ,compare result : " << isItAMatch << endl;
  cout << "Elapsed time for initial parsing :" << elapsedFirst.count() << " VS second parsing :" << elapsedSecond.count() << endl;
  cout << "Optimisation ratio : " << (elapsedFirst.count() / elapsedSecond.count())*100 << "%"<< endl;
	cout << "First grammar completion : <" << grammar->isComplete() << ">" << endl;
	cout << "Second grammar completion : <" << grammarReloaded->isComplete() << ">" << endl;

	grammarReloaded->isComplete();

cout << "************ Original grammar file parse *******************"<< endl;
//PARSING A FILE WITH THE ORIGINAL GRAMMAR
if (message_file){
  ifstream istr(message_file);
  if (!istr.is_open()){
    cerr<<"Could not open "<<message_file<<endl;
    return -1;
  }
  stringstream str;
  str<<istr.rdbuf();
  DebugParser parser(grammar);
  list<string> rules;
  for(int i=rules_first; i<argc; ++i){
    rules.push_back(argv[i]);
  }
  parser.setObservedRules(rules);
  size_t parsed;
  shared_ptr<DebugElement> ret;
  auto t_start = std::chrono::high_resolution_clock::now();
  for(int r=0;r<repeat_count;++r){
    ret=parser.parseInput(argv[rules_first],str.str(),&parsed);
  }
  auto t_end = std::chrono::high_resolution_clock::now();
  if (parsed<str.str().size()){
    cerr<<"Parsing ended prematuraly at pos "<<parsed<<endl;
  }else{
    cout<<"Parsing done in "<<std::chrono::duration<double, std::milli>(t_end-t_start).count()/repeat_count<<" milliseconds"<<endl;
  }
  if (ret){
    ret->tostream(0,cout);
  }else{
    cerr<<"Parsing failed."<<endl;
    return -1;
  }
}

cout << "************Loaded grammar file parse *******************"<< endl;
//PARSING A FILE WITH THE LOADED GRAMMAR
if (message_file){
  ifstream istr(message_file);
  if (!istr.is_open()){
    cerr<<"Could not open "<<message_file<<endl;
    return -1;
  }
  stringstream str;
  str<<istr.rdbuf();
  DebugParser parser(grammarReloaded);
  list<string> rules;
  for(int i=rules_first; i<argc; ++i){
    rules.push_back(argv[i]);
  }
  parser.setObservedRules(rules);
  size_t parsed;
  shared_ptr<DebugElement> ret;
  auto t_start = std::chrono::high_resolution_clock::now();
  for(int r=0;r<repeat_count;++r){
    ret=parser.parseInput(argv[rules_first],str.str(),&parsed);
  }
  auto t_end = std::chrono::high_resolution_clock::now();
  if (parsed<str.str().size()){
    cerr<<"Parsing ended prematuraly at pos "<<parsed<<endl;
  }else{
    cout<<"Parsing done in "<<std::chrono::duration<double, std::milli>(t_end-t_start).count()/repeat_count<<" milliseconds"<<endl;
  }
  if (ret){
    ret->tostream(0,cout);
  }else{
    cerr<<"Parsing failed."<<endl;
    return -1;
  }
}
	return 0;
};
