/*
 * belr.cpp
 * Copyright (C) 2017  Belledonne Communications SARL
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <bctoolbox/logging.h>
#include "belr/parser.h"
#include "belr/belr.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <cstring>
#include <string.h>

using namespace std;

using namespace belr;

// =============================================================================


TransitionMap::TransitionMap(){
	for(size_t i=0;i<sizeof(mPossibleChars)/sizeof(bool);++i)
		mPossibleChars[i]=false;
}

bool TransitionMap::intersect(const TransitionMap* other){
	for(size_t i=0;i<sizeof(mPossibleChars)/sizeof(bool);++i){
		if (mPossibleChars[i] && other->mPossibleChars[i]) return true;
	}
	return false;
}

bool TransitionMap::intersect(const TransitionMap *other, TransitionMap *result){
	bool ret=false;
	for(size_t i=0;i<sizeof(mPossibleChars)/sizeof(bool);++i){
		bool tmp=mPossibleChars[i] && other->mPossibleChars[i];
		result->mPossibleChars[i]=tmp;
		if (tmp) ret=true;
	}
	return ret;
}

void TransitionMap::merge(const TransitionMap* other){
	for(size_t i=0;i<sizeof(mPossibleChars)/sizeof(bool);++i){
		if (other->mPossibleChars[i]) mPossibleChars[i]=true;
	}
}

void Recognizer::setName(const std::string& name){
	static unsigned int id_base=0;
	mName=name;
	/* every recognizer that is given a name is given also a unique id*/
	mId=++id_base;
}

void Recognizer::setOnlyName(const std::string& name){
	mName= name;
}

void Recognizer::setId(int Id){
	mId= Id;
}

const string &Recognizer::getName()const{
	return mName;
}

size_t Recognizer::feed(const shared_ptr<ParserContextBase> &ctx, const string &input, size_t pos){
	size_t match;
	ParserLocalContext hctx;
	//cout << "DEBUG : recognizer name " << mName << endl;
 	if (ctx) ctx->beginParse(hctx, shared_from_this());
	match=_feed(ctx, input, pos);

	if (match!=string::npos && match>0){
		if (0 && mName.size()>0){
			string matched=input.substr(pos,match);
			cout<<"Matched recognizer '"<<mName<<"' with sequence '"<<matched<<"'."<<endl;
		}
	}
	if (ctx) ctx->endParse(hctx, input, pos, match);
	return match;
}

bool Recognizer::getTransitionMap(TransitionMap* mask){
	bool ret=_getTransitionMap(mask);
	if (0 /*!mName.empty()*/){
		cout<<"TransitionMap after "<<mName<<endl;
		for(int i=0;i<256;++i){
			if (mask->mPossibleChars[i]) cout<<(char)i;
		}
		cout<<endl;
	}
	return ret;
}


bool Recognizer::_getTransitionMap(TransitionMap* mask){
	string input;
	input.resize(2,'\0');
	for(int i=0;i<256;++i){
		input[0]=i;
		if (feed(nullptr,input,0)==1)
			mask->mPossibleChars[i]=true;
	}
	return true;
}

void Recognizer::optimize(){
	optimize(0);
}

void Recognizer::optimize(int recursionLevel){
	if (recursionLevel!=0 && mId!=0) return; /*stop on rule except at level 0*/
	_optimize(++recursionLevel);
}


CharRecognizer::CharRecognizer(int to_recognize, bool caseSensitive) : mToRecognize(to_recognize), mCaseSensitive(caseSensitive){
	if (::tolower(to_recognize)==::toupper(to_recognize)){
		/*not a case caseSensitive character*/
		mCaseSensitive=true;/*no need to perform a case insensitive match*/
	}else if (!caseSensitive){
		mToRecognize=::tolower(mToRecognize);
	}
}

size_t CharRecognizer::_feed(const shared_ptr<ParserContextBase> &ctx, const string &input, size_t pos){
	int c = (unsigned char)input[pos];
	if (mCaseSensitive){
		return c == mToRecognize ? 1 : string::npos;
	}
	return ::tolower(c) == mToRecognize ? 1 : string::npos;
}

void CharRecognizer::_optimize(int recursionLevel){

}

/**
    Saves Char regognizer attributes in a file
		write format is : TAG <mName> <mId> <mCaseSensitive> <mToRecognize>
		TAG is "CR"
    @param std:: ofstream out stream of the file.
					long : save position inside the file
    @return void
*/

void CharRecognizer::save(std::ofstream& outFile, long &savePos){
	outFile.seekp(savePos, ios::beg);

		if(outFile){
			outFile << "CR" << " " << mName << " " << mId << " " << mCaseSensitive  << " " <<	mToRecognize << " ";
			savePos = outFile.tellp();
		}
		else{
			cerr << "ERROR writing to file : output stream unavailable" << endl;
		}
}

/**
    Creates a CharRecognizer from parsed data

    @param std::vector<string>::const_iterator : iterator over a string vector representing the parsed line from the file
		elements are parsed with space as separator
    @return shared_ptr<CharRecognizer>
*/
shared_ptr<CharRecognizer> CharRecognizer::loadVect(std::vector<string>::const_iterator &inIter){

	if (*(inIter) == "CR"){
		string mNameReloaded =  *(++inIter);
		int mIdReloaded = stoi(*(++inIter));
		bool mCaseSensitive_loaded = stoi(*(++inIter));
		int mToRecognize_loaded = stoi(*(++inIter));
		++inIter;
		auto crRecon =  make_shared<CharRecognizer>(mToRecognize_loaded , mCaseSensitive_loaded);
		crRecon->mName = mNameReloaded;
		crRecon->mId = mIdReloaded;
		return crRecon;
	}
	else
		cerr << "ERROR reading file : no CHAR RECOGNIZER tag found" << endl;
	return NULL;
}


/**
  Verifies if the CharRecognizer is equal to the one passed in parameter
	returns true if that's the case
	@return bool
*/
bool CharRecognizer::equal(const shared_ptr<Recognizer> &CR){
	//Using dynamic cast to have acces to the attributes of the CharRecognizer object
	shared_ptr<CharRecognizer> CR_local = dynamic_pointer_cast<CharRecognizer>(CR);
	return((CR_local->mToRecognize == mToRecognize) && (CR_local->mCaseSensitive == mCaseSensitive) && mName == CR_local->getName() && mId == CR_local->getId()) ? true : false;
}

bool CharRecognizer::operator==(Recognizer &CR){
	return this->equal(CR.shared_from_this());
}

/**
  function used to set the right pointed element for the recognizer pointers
*/
void CharRecognizer::linkPointer(std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd){
	//nothing to do
}

shared_ptr<Selector> Selector::addRecognizer(const shared_ptr<Recognizer> &r){
	mElements.push_back(r);
	return static_pointer_cast<Selector> (shared_from_this());
}

bool Selector::_getTransitionMap(TransitionMap* mask){
    for (auto it=mElements.begin(); it!=mElements.end(); ++it){
	    (*it)->getTransitionMap(mask);
    }
    return true;
}

size_t Selector::_feedExclusive(const shared_ptr<ParserContextBase> &ctx, const string &input, size_t pos){
	size_t matched=0;

	for (auto it=mElements.begin(); it!=mElements.end(); ++it){
		matched=(*it)->feed(ctx, input, pos);
		if (matched!=string::npos && matched>0) {
			return matched;
		}
	}
	return string::npos;
}

size_t Selector::_feed(const shared_ptr<ParserContextBase> &ctx, const string &input, size_t pos){
	if (mIsExclusive) return _feedExclusive(ctx, input, pos);

	size_t matched=0;
	size_t bestmatch=0;
	shared_ptr<HandlerContextBase> bestBranch;

	for (auto it=mElements.begin(); it!=mElements.end(); ++it){
		shared_ptr<HandlerContextBase> br;
		if (ctx) br=ctx->branch();
		matched=(*it)->feed(ctx, input, pos);
		if (matched!=string::npos && matched>bestmatch) {
			bestmatch=matched;
			if (bestBranch) ctx->removeBranch(bestBranch);
			bestBranch=br;
		}else{
			if (ctx)
				ctx->removeBranch(br);
		}
	}
	if (bestmatch==0) return string::npos;
	if (ctx && bestmatch!=string::npos){
		ctx->merge(bestBranch);
	}
	return bestmatch;
}

void Selector::_optimize(int recursionLevel){
	for (auto it=mElements.begin(); it!=mElements.end(); ++it){
		(*it)->optimize(recursionLevel);
	}
	TransitionMap *all=nullptr;
	bool intersectionFound=false;
	for (auto it=mElements.begin(); it!=mElements.end() && !intersectionFound; ++it){
		TransitionMap *cur=new TransitionMap();
		(*it)->getTransitionMap(cur);
		if (all){
			if (cur->intersect(all)){
				intersectionFound=true;
			}
			all->merge(cur);
			delete cur;
		}else all=cur;
	}
	if (all) delete all;
	if (!intersectionFound){
		//cout<<"Selector '"<<getName()<<"' is exclusive."<<endl;
		mIsExclusive=true;
	}
}
/**
    Saves Selector attributes in a file
		write format is : TAG mName mId mIsExclusive numberOfElements Element1 Element2 Element3
		each Element is a recognizer meaning it has got its own format
		TAG is "SEL"
    @param std:: ofstream out stream of the file.
		long : save position inside the output file
    @return void
*/
void Selector::save(std::ofstream& outFile, long &savePos){
	outFile.seekp(savePos, ios::beg);

		if(outFile){
			outFile << "SEL" << " " << mName << " " << mId << " "<< mIsExclusive  << " ";
			savePos = outFile.tellp();
			int recognizerListSize = 0;
			//the number of elements in the recognizer list is needed when loading the Selector afterwards
			for(list<shared_ptr<Recognizer>>::iterator it = mElements.begin(); it!=mElements.end(); ++it){
				recognizerListSize++ ;
			}
			outFile << recognizerListSize << " ";
			for(list<shared_ptr<Recognizer>>::iterator it = mElements.begin(); it!=mElements.end() ; ++it){
						(*it)->save(outFile, savePos);

 			}
		}
		else{
			cerr << "ecriture dans le fichier impossibe" << endl;
		}
}

/**
    Creates a Selector from parsed data

    @param std::vector<string>::const_iterator &inIter: iterator over a string vector representing the parsed line from the file
		elements are parsed with space as separator
						std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin : iterator set on the first element of the recognizers pointers list
						std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd : iterator set on the last element of the recognizers pointers list
						these two parameters are used to go through the recognizers pointers list and look for a match incase a RECP tag is found and then set it correctly in the lodaded recognizer
    @return shared_ptr<Selector>
*/
shared_ptr<Selector> Selector::loadVect(std::vector<string>::const_iterator &inIter, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd){
	shared_ptr<Selector> selector_reloaded = NULL;
	if(*inIter == "SEL"){
		string mNameReloaded =  *(++inIter);
		int mIdReloaded = stoi(*(++inIter));
		bool mIsExclusive_loaded = stoi(*(++inIter));
		int recognizerListSize = stoi(*(++inIter));
		selector_reloaded = Foundation::selector(0);
		selector_reloaded->mIsExclusive = mIsExclusive_loaded;
		selector_reloaded->mName = mNameReloaded;
		selector_reloaded->mId = mIdReloaded;
		string insideTAG = *(++inIter);
		for(int i=0; i<recognizerListSize; i++){
			insideTAG = *(inIter);
		  if (insideTAG == "CR") {selector_reloaded->addRecognizer(CharRecognizer::loadVect(inIter));}
      else if(insideTAG == "SEQ") {selector_reloaded->addRecognizer(Sequence::loadVect(inIter, rcptrIterBegin, rcptrIterEnd));}
      else if(insideTAG == "LOOP") {selector_reloaded->addRecognizer(Loop::loadVect(inIter, rcptrIterBegin, rcptrIterEnd));}
      else if(insideTAG == "XCR") {selector_reloaded->addRecognizer(CharRange::loadVect(inIter));}
      else if(insideTAG == "LIT") {selector_reloaded->addRecognizer(Literal::loadVect(inIter));}
      else if(insideTAG == "XSEL") {selector_reloaded->addRecognizer(ExclusiveSelector::loadVect(inIter, rcptrIterBegin, rcptrIterEnd));}
      else if(insideTAG == "SEL") {selector_reloaded->addRecognizer(Selector::loadVect(inIter, rcptrIterBegin, rcptrIterEnd));}
      else if(insideTAG == "RECP") { selector_reloaded->addRecognizer(RecognizerPointer::loadVect(inIter, rcptrIterBegin, rcptrIterEnd));}
      else std::cerr << "ERROR : Tag inside selector not defined, found <" << insideTAG << "> instead"<< '\n';
		}
	}
	else
		cerr << "ERROR : No SELECTOR tag, found <" << *inIter << "> instead"<< endl;
	return selector_reloaded;
}

/**
	Verifies if two selectors are equal
	compares names , id, and the list of the recognizers
	@param : const std::shared_ptr<Recognizer>
*/
bool Selector::equal(const std::shared_ptr<Recognizer> &SEL){

	const shared_ptr<Selector> SEL_local = dynamic_pointer_cast<Selector>(SEL);
	list<shared_ptr<Recognizer>>::iterator i_local = SEL_local->mElements.begin();
	list<shared_ptr<Recognizer>>::iterator i = mElements.begin();
	bool condition = true;

	while (condition == true && i!=mElements.end()){
		if (!(*i)->equal(*i_local)) {
			condition = false;
		}
		advance(i,1);
		advance(i_local,1);

	}
	return(condition && (SEL_local->mIsExclusive == mIsExclusive) && (mName == SEL_local->getName()) && (mName == SEL_local->getName())) ? true : false;
}

/**
	Verifies if two selectors are equal
	compares names , id, and the list of the recognizers
	@param : belr::Recognizer
*/
bool Selector::operator==(Recognizer &SEL){
	return this->equal(SEL.shared_from_this());
}

/**
function used to set the right pointed element for the recognizer pointers
*/
void Selector::linkPointer(std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd){
	for(std::list<std::shared_ptr<Recognizer>>::const_iterator it = mElements.begin() ;it!=mElements.end();it++)
		(*it)->linkPointer(rcptrIterBegin, rcptrIterEnd);

}

ExclusiveSelector::ExclusiveSelector() {
	mIsExclusive = true;
}

size_t ExclusiveSelector::_feed(const shared_ptr<ParserContextBase> &ctx, const string &input, size_t pos){
	return Selector::_feedExclusive(ctx, input, pos);
}

/**
    Saves ExclusiveSelector attributes in a file
		write format is : TAG mName mId mIsExclusive exRecognizerListSize Element1 Element2 ...
		TAG is "XSEL"
    @param std:: ofstream out stream of the file.
					long : save position inside the output file
    @return void
*/
void ExclusiveSelector::save(std::ofstream& outFile, long &savePos){
		outFile.seekp(savePos, ios::beg);
		if(outFile){
			outFile << "XSEL" << " " << mName << " " << mId << " "<< mIsExclusive  << " ";
			int exRecognizerListSize = 0;
			for(list<shared_ptr<Recognizer>>::iterator it = mElements.begin(); it!=mElements.end(); ++it){
				exRecognizerListSize++ ;
			}
			outFile << exRecognizerListSize << " ";
			for(list<shared_ptr<Recognizer>>::iterator it = mElements.begin(); it!=mElements.end() ; ++it){
					(*it)->save(outFile, savePos);
 			}
		}
		else{
			cerr << "ERROR : file stream unavailable" << endl;
		}
}


/**
    Creates an ExclusiveSelector from parsed data

    @param std::vector<string>::const_iterator &inIter: iterator over a string vector representing the parsed line from the file
		elements are parsed with space as separator
						std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin : iterator set on the first element of the recognizers pointers list
						std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd : iterator set on the last element of the recognizers pointers list
						these two parameters are used to go through the recognizers pointers list and look for a match incase a RECP tag is found and then set it correctly in the lodaded recognizer
    @return shared_ptr<Selector>
*/
shared_ptr<ExclusiveSelector> ExclusiveSelector::loadVect(std::vector<string>::const_iterator &inIter, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd){
	shared_ptr<ExclusiveSelector> selector_reloaded = NULL;
	if(*inIter == "XSEL"){
		string mNameReloaded =  *(++inIter);
		int mIdReloaded = stoi(*(++inIter));
		bool mIsExclusive_loaded = stoi(*(++inIter));
		int recognizerListSize = stoi(*(++inIter));
		selector_reloaded = dynamic_pointer_cast<ExclusiveSelector>(Foundation::selector(mIsExclusive_loaded));
		selector_reloaded->mName = (mNameReloaded);
		selector_reloaded->mId = (mIdReloaded) ;
		string insideTAG = *(++inIter);
		for(int i=0; i<recognizerListSize; i++){
			insideTAG = *inIter;
			if (insideTAG == "CR") selector_reloaded->addRecognizer(CharRecognizer::loadVect(inIter));
      else if(insideTAG == "SEQ") selector_reloaded->addRecognizer(Sequence::loadVect(inIter, rcptrIterBegin, rcptrIterEnd));
      else if(insideTAG == "LOOP") selector_reloaded->addRecognizer(Loop::loadVect(inIter, rcptrIterBegin, rcptrIterEnd));
      else if(insideTAG == "XCR") selector_reloaded->addRecognizer(CharRange::loadVect(inIter));
      else if(insideTAG == "LIT") selector_reloaded->addRecognizer(Literal::loadVect(inIter));
      else if(insideTAG == "XSEL")selector_reloaded->addRecognizer(ExclusiveSelector::loadVect(inIter, rcptrIterBegin, rcptrIterEnd));
      else if(insideTAG == "SEL") selector_reloaded->addRecognizer(Selector::loadVect(inIter, rcptrIterBegin, rcptrIterEnd));
      else if(insideTAG == "RECP") selector_reloaded->addRecognizer(RecognizerPointer::loadVect(inIter, rcptrIterBegin, rcptrIterEnd));
      else std::cerr << "ERROR : Tag inside exclusive selector not defined, found <"<< insideTAG << "> instead" << '\n';
		}
	}
	else{
		cerr << "ERROR : No EXCLUSIVE SELECTOR tag, found <> instead"<<  endl;
	}
	return dynamic_pointer_cast<ExclusiveSelector>(selector_reloaded);
}

/**
	Verifies if two exclusive selecors are equal
	checks name, id, exclusivity, and recognizers list
*/
bool ExclusiveSelector::equal(const std::shared_ptr<Recognizer> &SEL){

	const shared_ptr<ExclusiveSelector> SEL_local = dynamic_pointer_cast<ExclusiveSelector>(SEL);
	list<shared_ptr<Recognizer>>::iterator i_local = SEL_local->mElements.begin();
	list<shared_ptr<Recognizer>>::iterator i = mElements.begin();
	bool condition = true;

	while (condition == true && i!=mElements.end()){
		if (!(*i)->equal(*i_local)) condition = false;
		advance(i,1);
		advance(i_local,1);
	}
	return(condition && (SEL_local->mIsExclusive == mIsExclusive) && (mName == SEL_local->getName()) && (mId == SEL_local->getId())) ? true : false;
}

bool ExclusiveSelector::operator==(Recognizer &SEL){
	return this->equal(SEL.shared_from_this());
}

/**
function used to set the right pointed element for the recognizer pointers
*/
void ExclusiveSelector::linkPointer(std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd){
	for(std::list<std::shared_ptr<Recognizer>>::const_iterator it = mElements.begin() ;it!=mElements.end();it++)
		(*it)->linkPointer(rcptrIterBegin, rcptrIterEnd);
}

shared_ptr<Sequence> Sequence::addRecognizer(const shared_ptr<Recognizer> &element){
	mElements.push_back(element);
	return static_pointer_cast<Sequence>( shared_from_this());
}

bool Sequence::_getTransitionMap(TransitionMap* mask){
	bool isComplete=false;
	for (auto it=mElements.begin(); it!=mElements.end(); ++it){
		if ((*it)->getTransitionMap(mask)) {
			isComplete=true;
			break;
		}
	}
	return isComplete;
}


size_t Sequence::_feed(const shared_ptr<ParserContextBase> &ctx, const string &input, size_t pos){
	size_t matched=0;
	size_t total=0;

	for (auto it=mElements.begin(); it!=mElements.end(); ++it){
		matched=(*it)->feed(ctx, input, pos);
		if (matched==string::npos){
			return string::npos;
		}
		pos+=matched;
		total+=matched;
	}
	return total;
}

void Sequence::_optimize(int recursionLevel){
	for (auto it=mElements.begin(); it!=mElements.end(); ++it)
		(*it)->optimize(recursionLevel);
}

/**
    Saves Sequence attributes in a file
		write format is : TAG mName mId sequenceListSize Element1 Element2 ...
		TAG is "SEQ"
    @param std:: ofstream out stream of the file.
					long : save position inside the file
    @return void
*/
void Sequence::save(std::ofstream& outFile, long &savePos){

		outFile.seekp(savePos, ios::beg);
		if(outFile){
			outFile << "SEQ" << " " << mName << " "<< mId << " ";
			int sequenceListSize = 0;
			for(list<shared_ptr<Recognizer>>::iterator it = mElements.begin(); it!=mElements.end(); ++it){
				sequenceListSize++ ;
			}
			outFile << sequenceListSize << " ";
			for(list<shared_ptr<Recognizer>>::iterator it = mElements.begin(); it!=mElements.end() ; ++it){
				(*it)->save(outFile, savePos);
 			}
		}
		else{
			cerr << "ERROR : file stream unavailable" << endl;
		}
}

/**
    Creates a Sequence from parsed data

    @param std::vector<string>::const_iterator : iterator over a string vector representing the parsed line from the file
		elements are parsed with space as separator
					std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin : iterator set on the first element of the recognizers pointers list
					std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd : iterator set on the last element of the recognizers pointers list
		these two parameters are used to go through the recognizers pointers list and look for a match incase a RECP tag is found and then set it correctly in the lodaded recognizer

    @return shared_ptr<Sequence>
*/
shared_ptr<Sequence> Sequence::loadVect(std::vector<string>::const_iterator &inIter, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd){
	shared_ptr<Sequence> sequence_reloaded = NULL;

	if(*inIter == "SEQ"){
		string mNameReloaded =  *(++inIter);
		int mIdReloaded = stoi(*(++inIter));
		int recognizerListSize = stoi(*(++inIter));
		sequence_reloaded = make_shared<Sequence>();
		sequence_reloaded->mName = mNameReloaded;
		sequence_reloaded->mId = mIdReloaded;
		string insideTAG = *(++inIter);
		for(int i=0; i<recognizerListSize; i++){
			insideTAG = *(inIter);

			if (insideTAG == "CR") sequence_reloaded->addRecognizer(CharRecognizer::loadVect(inIter));
      else if(insideTAG == "SEQ") sequence_reloaded->addRecognizer(Sequence::loadVect(inIter, rcptrIterBegin, rcptrIterEnd));
      else if(insideTAG == "LOOP") sequence_reloaded->addRecognizer(Loop::loadVect(inIter, rcptrIterBegin, rcptrIterEnd));
      else if(insideTAG == "XCR") sequence_reloaded->addRecognizer(CharRange::loadVect(inIter));
      else if(insideTAG == "LIT") sequence_reloaded->addRecognizer(Literal::loadVect(inIter));
      else if(insideTAG == "XSEL")sequence_reloaded->addRecognizer(ExclusiveSelector::loadVect(inIter, rcptrIterBegin, rcptrIterEnd));
      else if(insideTAG == "SEL") sequence_reloaded->addRecognizer(Selector::loadVect(inIter, rcptrIterBegin, rcptrIterEnd));
      else if(insideTAG == "RECP") sequence_reloaded->addRecognizer(RecognizerPointer::loadVect(inIter, rcptrIterBegin, rcptrIterEnd));
      else std::cerr << "ERROR : inside sequence Type not recognized found <"<< insideTAG << "> instead" << '\n';
		}
	}
	else
		cerr << "ERROR : No SEQUENCE tag found" << endl;
	return (sequence_reloaded);
}



bool Sequence::equal(const std::shared_ptr<Recognizer> &SEQ){

	const shared_ptr<Sequence> SEQ_local = dynamic_pointer_cast<Sequence>(SEQ);
	list<shared_ptr<Recognizer>>::iterator i_local = SEQ_local->mElements.begin();
	list<shared_ptr<Recognizer>>::iterator i = mElements.begin();
	bool condition = true;
	if (mName != SEQ_local->getName()) condition = false;
	if(mId != SEQ_local->getId()) condition = false;

	while (condition == true && i!=mElements.end()){
		if (!(*i)->equal(*i_local)) {
			condition = false;
		}
		advance(i,1);
		advance(i_local,1);
	}
	return(condition) ? true : false;
}

bool Sequence::operator==(Recognizer &SEQ){
	return this->equal(SEQ.shared_from_this());

}

/**
function used to set the right pointed element for the recognizer pointers
*/
void Sequence::linkPointer(std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd){
	for(std::list<std::shared_ptr<Recognizer>>::const_iterator it = mElements.begin() ;it!=mElements.end();it++)
		(*it)->linkPointer(rcptrIterBegin, rcptrIterEnd);
}

shared_ptr<Loop> Loop::setRecognizer(const shared_ptr<Recognizer> &element, int min, int max){
	mMin=min;
	mMax=max;
	mRecognizer=element;
	return static_pointer_cast<Loop>(shared_from_this());
}

size_t Loop::_feed(const shared_ptr<ParserContextBase> &ctx, const string &input, size_t pos){
	size_t matched=0;
	size_t total=0;
	int repeat;

	for(repeat=0;mMax!=-1 ? repeat<mMax : true;repeat++){
		matched=mRecognizer->feed(ctx, input, pos);
		if (matched==string::npos) break;
		total+=matched;
		pos+=matched;
		if (input[pos]=='\0') break;
	}
	if (repeat<mMin) return string::npos;
	return total;
}

bool Loop::_getTransitionMap(TransitionMap* mask){
	mRecognizer->getTransitionMap(mask);
	return mMin!=0; //we must say to upper layer that this loop recognizer is allowed to be optional by returning FALSE
}

void Loop::_optimize(int recursionLevel){
	mRecognizer->optimize(recursionLevel);
}

/**
    Saves Loop attributes in a file
		write format is : TAG mName mId mMin mMax mRecognizer
		TAG is "LOOP"
    @param std:: ofstream out stream of the file.
					long : save position inside the file
    @return void
*/
void Loop::save(std::ofstream& outFile, long &savePos){
	outFile.seekp(savePos, ios::beg);
	if(outFile){
		outFile << "LOOP" << " " << mName <<  " " << mId << " "<< mMin << " " << mMax << " ";
		mRecognizer->save(outFile, savePos);
	}
	else{
		cerr << "ERROR : file stream unavailable" << endl;
	}
}

/**
    Creates a Loop from parsed data

    @param std::vector<string>::const_iterator : iterator over a string vector representing the parsed line from the file
		elements are parsed with space as separator
					std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin : iterator set on the first element of the recognizers pointers list
					std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd : iterator set on the last element of the recognizers pointers list
these two parameters are used to go through the recognizers pointers list and look for a match incase a RECP tag is found and then set it correctly in the lodaded recognizer

    @return shared_ptr<Loop>
*/
shared_ptr<Loop> Loop::loadVect(std::vector<string>::const_iterator &inIter, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd){

	shared_ptr<Loop> loop_reloaded = NULL;

	if(*inIter == "LOOP"){
		string mNameReloaded =  *(++inIter);
		int mIdReloaded = stoi(*(++inIter));
    int mMin_loaded =  stoi(*(++inIter));
    int mMax_loaded =  stoi(*(++inIter));
    loop_reloaded = make_shared<Loop>();
		loop_reloaded->mName = (mNameReloaded);
		loop_reloaded->mId = mIdReloaded;
		string insideTAG = *(++inIter);
		if (insideTAG == "CR") loop_reloaded->setRecognizer(CharRecognizer::loadVect(inIter), mMin_loaded, mMax_loaded);
		else if(insideTAG == "XSEL") loop_reloaded->setRecognizer(ExclusiveSelector::loadVect(inIter, rcptrIterBegin, rcptrIterEnd), mMin_loaded, mMax_loaded);
		else if(insideTAG == "SEL") loop_reloaded->setRecognizer(Selector::loadVect(inIter, rcptrIterBegin, rcptrIterEnd), mMin_loaded, mMax_loaded);
		else if(insideTAG == "SEQ") loop_reloaded->setRecognizer(Sequence::loadVect(inIter, rcptrIterBegin, rcptrIterEnd), mMin_loaded, mMax_loaded);
    else if(insideTAG == "LOOP") loop_reloaded->setRecognizer(Loop::loadVect(inIter, rcptrIterBegin, rcptrIterEnd), mMin_loaded, mMax_loaded);
    else if(insideTAG == "XCR") loop_reloaded->setRecognizer(CharRange::loadVect(inIter), mMin_loaded, mMax_loaded);
    else if(insideTAG == "LIT") loop_reloaded->setRecognizer(Literal::loadVect(inIter), mMin_loaded, mMax_loaded);
    else if(insideTAG == "RECP") loop_reloaded->setRecognizer(RecognizerPointer::loadVect(inIter, rcptrIterBegin, rcptrIterEnd), mMin_loaded, mMax_loaded);
		else std::cerr << "ERROR : tag inside Loop not recognized ,found <" << insideTAG << "> instead" << endl;
	}
	else
		cerr << "ERROR : No LOOP tag, found <" << *inIter << "> instead" << endl;
	return loop_reloaded;
}


/**
	Verifies if two Loop recognizers are equal
*/
bool Loop::equal(const std::shared_ptr<Recognizer> &LOP){
	const shared_ptr<Loop> LOP_local = dynamic_pointer_cast<Loop>(LOP);
	return((LOP_local->mMin == mMin) && (LOP_local->mMax == mMax) && (mRecognizer->equal(LOP_local->mRecognizer)) && (LOP_local->getName() == mName)  && (mId == LOP_local->getId())	 ) ? true : false;
}

bool Loop::operator==(Recognizer &LOP){
	return this->equal(LOP.shared_from_this());
}

/**
function used to set the right pointed element for the recognizer pointers
*/
void Loop::linkPointer(std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd){
		mRecognizer->linkPointer(rcptrIterBegin, rcptrIterEnd);
}

CharRange::CharRange(int begin, int end) : mBegin(begin), mEnd(end){
}

size_t CharRange::_feed(const shared_ptr<ParserContextBase> &ctx, const string &input, size_t pos){
	int c = (unsigned char)input[pos];
	if (c >= mBegin && c <= mEnd) return 1;
	return string::npos;
}

void CharRange::_optimize(int recursionLevel){

}
/**
    Saves CharRange attributes in a file
		write format is : XCR mBegin mEnd
		TAG is "LOOP"
    @param std:: ofstream out stream of the file.
					long : save position inside the file
    @return void
*/
void CharRange::save(std::ofstream& outFile, long &savePos){

		outFile.seekp(savePos, ios::beg);

		if(outFile){

			outFile << "XCR" << " " << mName << " " << mId << " "<< mBegin  << " " <<	mEnd << " ";
			savePos = outFile.tellp();
		}
		else{
			cerr << "error writing to file : ecriture dans le fichier impossibe" << endl;
		}
}

/**
    Creates a CharRange from parsed data

    @param std::vector<string>::const_iterator : iterator over a string vector representing the parsed line from the file
		elements are parsed with space as separator
    @return shared_ptr<CharRange>
*/
shared_ptr<CharRange> CharRange::loadVect(std::vector<string>::const_iterator &inIter){
	if(*(inIter) == "XCR"){
		string mNameReloaded =  *(++inIter);
		int mIdReloaded = stoi(*(++inIter));

    int mBegin_loaded =  stoi(*(++inIter));
    int mEnd_loaded =  stoi(*(++inIter));
    ++inIter;
    auto charRangeRecon =  make_shared<CharRange>(mBegin_loaded, mEnd_loaded);
		charRangeRecon->mName = (mNameReloaded);
		charRangeRecon->mId = mIdReloaded;
		return charRangeRecon;
	}
	else{
		cerr << "ERROR : no CHARRANGE tag found, found <" << *inIter << "> instead" << endl;
	}
  return NULL;
}



bool CharRange::equal(const shared_ptr<Recognizer> &XCR){
	const shared_ptr<CharRange> XCR_local = dynamic_pointer_cast<CharRange>(XCR);
	return((XCR_local->mBegin == mBegin) && (XCR_local->mEnd == mEnd) && (mName == XCR_local->getName()) && (mId == XCR_local->getId())) ? true : false;
}

bool CharRange::operator==(Recognizer &XCR){
	return this->equal(XCR.shared_from_this());
	}

/**
function used to set the right pointed element for the recognizer pointers
*/
void CharRange::linkPointer(std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd){
	// nothing to do
}


shared_ptr<CharRecognizer> Foundation::charRecognizer(int character, bool caseSensitive){
	return make_shared<CharRecognizer>(character, caseSensitive);
}

shared_ptr<Selector> Foundation::selector(bool isExclusive){
	return isExclusive ? make_shared<ExclusiveSelector>() : make_shared<Selector>();
}

shared_ptr<Sequence> Foundation::sequence(){
	return make_shared<Sequence>();
}

shared_ptr<Loop> Foundation::loop(){
	return make_shared<Loop>();
}

Literal::Literal(const string& lit) : mLiteral(tolower(lit)), mLiteralSize(mLiteral.size()) {

}

size_t Literal::_feed(const shared_ptr< ParserContextBase >& ctx, const string& input, size_t pos){
	size_t i;
	for(i=0;i<mLiteralSize;++i){
		if (::tolower(input[pos+i])!=mLiteral[i]) return string::npos;
	}
	return mLiteralSize;
}

void Literal::_optimize(int recursionLevel){

}

bool Literal::_getTransitionMap(TransitionMap* mask){
	mask->mPossibleChars[::tolower(mLiteral[0])]=true;
	mask->mPossibleChars[::toupper(mLiteral[0])]=true;
	return true;
}

/**
    Saves Literal attributes in a file
		write format is : TAG mLiteral
		TAG is "LIT"
    @param std:: ofstream out stream of the file.
					long : save position inside the file
    @return void
*/
void Literal::save(std::ofstream& outFile, long &savePos){
	outFile.seekp(savePos, ios::beg);
	if(outFile){
		outFile << "LIT" << " " << mName << " " << mId << " "<< mLiteral  << " " ;
		savePos = outFile.tellp();
	}
	else{
		cerr << "error writing to file : ecriture dans le fichier impossibe" << endl;
	}
}

/**
    Creates a Literal from parsed data

    @param std::vector<string>::const_iterator : iterator over a string vector representing the parsed line from the file
		elements are parsed with space as separator
    @return shared_ptr<CharRange>
*/
shared_ptr<Literal> Literal::loadVect(std::vector<string>::const_iterator &inIter){
  if((*inIter) == "LIT"){
		string mNameReloaded =  *(++inIter);
		int mIdReloaded = stoi(*(++inIter));

    string mliteral_loaded =  *(++inIter);
    ++inIter;
    auto literalRecon = make_shared<Literal>(mliteral_loaded);
		literalRecon->mName = (mNameReloaded);
		literalRecon->mId = mIdReloaded;
		return literalRecon;
  }
	else{
  	cerr << "ERROR : no LITERAL tag found" << endl;
  }
  return NULL;
}



bool Literal::equal(const shared_ptr<Recognizer> &LIT){
	const shared_ptr<Literal> LIT_local = dynamic_pointer_cast<Literal>(LIT); //cast en type approprié pour pouvoir accéder aux attributs
	return((LIT_local->mLiteral == mLiteral) && ((LIT_local)->getName() == mName) && (LIT_local->getId() == mId)) ? true : false;
}

bool Literal::operator==(Recognizer &LIT){
	return this->equal(LIT.shared_from_this());
}

/**
function used to set the right pointed element for the recognizer pointers
*/
void Literal::linkPointer(std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd){
	// nothing to do
}

shared_ptr<Recognizer> Utils::literal(const string & lt){
	return make_shared<Literal>(lt);
}

shared_ptr<Recognizer> Utils::char_range(int begin, int end){
	return make_shared<CharRange>(begin, end);
}

shared_ptr<Recognizer> RecognizerPointer::getPointed(){
	return mRecognizer;
}

size_t RecognizerPointer::_feed(const shared_ptr<ParserContextBase> &ctx, const string &input, size_t pos){
	//cout << "DEBUG : name of the rule :: <" << mName << ">" << endl;
	if (mRecognizer){
		//cout << "been here done that <"<< mName<<">" << endl;
		return mRecognizer->feed(ctx, input, pos);
	}else{
		cerr<<"RecognizerPointer with name '"<<mName<<"' is undefined"<<endl;
		abort();
	}
	return string::npos;
}

void RecognizerPointer::setPointed(const shared_ptr<Recognizer> &r){
	mRecognizer=r;
}


void RecognizerPointer::_optimize(int recursionLevel){
	/*do not call optimize() on the pointed value to avoid a loop.
	 * The grammar will do it for all rules anyway*/
}


/**
    Saves Literal attributes in a file
		write format is : TAG mName mId mRecognizer->mName
		TAG is "RECP"
    @param std:: ofstream out stream of the file.
					long : save position inside the file
    @return void
*/
void RecognizerPointer::save(std::ofstream& outFile, long &savePos){ // ajouter un parametre la hashmap des regles
	outFile.seekp(savePos, ios::beg);
	if(outFile){
		outFile << "RECP" << " " << mName << " " << mId << " " << mRecognizer->getName() << " " ;
		savePos = outFile.tellp();
	}
	else{
		cerr << "ERROR : file stream unavailable" << endl;
	}
}

/**
    Creates a RecognizerPointer from parsed data

    @param std::vector<string>::const_iterator : iterator over a string vector representing the parsed line from the file
		elements are parsed with space as separator
					std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin : iterator set on the first element of the recognizers pointers list
					std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd : iterator set on the last element of the recognizers pointers list
these two parameters are used to go through the recognizers pointers list and look for a match incase a RECP tag is found and then set it correctly in the lodaded recognizer
    @return shared_ptr<RecognizerPointer>
*/
shared_ptr<RecognizerPointer> RecognizerPointer::loadVect(std::vector<string>::const_iterator &inIter, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd){
	shared_ptr<RecognizerPointer> recognpointer_reloaded = NULL;
	if((*inIter) == "RECP"){
		recognpointer_reloaded = make_shared<RecognizerPointer>();
		std::string mNameReloaded = *(++inIter);
		int mIdReloaded = stoi(*(++inIter));
		std::string mRecognizerNameReloaded = *(++inIter);
		recognpointer_reloaded->mId = mIdReloaded;
		recognpointer_reloaded->mName = (mNameReloaded);
//Check if the recognizer pointer is already parsed link it since we're at it
		for(std::list<std::shared_ptr<RecognizerPointer>>::iterator rcptrIter = (rcptrIterBegin);  rcptrIter != rcptrIterEnd ; ++rcptrIter){
		//	shared_ptr<RecognizerPointer>rcptLocal = dynamic_pointer_cast<RecognizerPointer>(*rcptrIter);
			if (mNameReloaded == (*rcptrIter)->getName()){
				recognpointer_reloaded->setPointed((*rcptrIter)->getPointed());
			}
		}
		if(recognpointer_reloaded->getPointed()) (recognpointer_reloaded->getPointed())->setOnlyName(mRecognizerNameReloaded);
		++inIter;
	}
	else{
		cerr << "ERROR : no RECONGIZER POINTER tag found" << endl;
	}
	return recognpointer_reloaded;
}

RecognizerPointer::RecognizerPointer() {

}

bool RecognizerPointer::equal(const std::shared_ptr<Recognizer> &LOP){
	bool condition = true;
	const shared_ptr<RecognizerPointer> LOP_local = dynamic_pointer_cast<RecognizerPointer>(LOP);
	if (LOP_local != NULL){
		//comparing adresses of recignizers because camparing the actual recignizers causes a segmentation in some cases
	condition = (&mRecognizer == &(LOP_local->mRecognizer)) ? true : false;
	condition = (this->getName() == LOP_local->getName()) ? true : false;
	condition = (mRecognizer->getName() == (LOP_local->mRecognizer)->getName()) ? true : false;
	}
	return condition;
}

bool RecognizerPointer::operator==(Recognizer &RCPTR){
	return this->equal(RCPTR.shared_from_this());
}

/**
	function used to set the right pointed element for the recognizer pointers
*/
void RecognizerPointer::linkPointer(std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterBegin, std::list<std::shared_ptr<RecognizerPointer>>::iterator &rcptrIterEnd){
	for(std::list<std::shared_ptr<RecognizerPointer>>::iterator rcptrIter = (rcptrIterBegin);  rcptrIter != rcptrIterEnd ; ++rcptrIter){
		if (mName == (*rcptrIter)->getName()){
			mRecognizer = ((*rcptrIter)->getPointed());
		}
	}
}


Grammar::Grammar(const string& name) : mName(name){

}

Grammar::~Grammar() {
	for(auto it = mRecognizerPointers.begin(); it != mRecognizerPointers.end(); ++it) {
		shared_ptr<RecognizerPointer> pointer = dynamic_pointer_cast<RecognizerPointer>(*it);
		pointer->setPointed(nullptr);
	}
}

void Grammar::assignRule(const string &argname, const shared_ptr<Recognizer> &rule){
	string name=tolower(argname);
	rule->setName(name);
	auto it=mRules.find(name);
	if (it!=mRules.end()){
		shared_ptr<RecognizerPointer> pointer=dynamic_pointer_cast<RecognizerPointer>((*it).second);
		if (pointer){
			pointer->setPointed(rule);
		}else{
			cerr<<"Error: rule '"<<name<<"' is being redefined !"<<endl;
			abort();
		}
	}
	/*in any case the map should contain real recognizers (not just pointers) */
	mRules[name]=rule;
}

void Grammar::_extendRule(const string &argname, const shared_ptr<Recognizer> &rule){
	string name=tolower(argname);
	rule->setName("");
	auto it=mRules.find(name);
	if (it!=mRules.end()){
		shared_ptr<Selector> sel=dynamic_pointer_cast<Selector>((*it).second);
		if (sel){
			sel->addRecognizer(rule);
		}else{
			cerr<<"Error: rule '"<<name<<"' cannot be extended because it was not defined with a Selector."<<endl;
			abort();
		}
	}else{
		cerr<<"Error: rule '"<<name<<"' cannot be extended because it is not defined."<<endl;
		abort();
	}
}

shared_ptr<Recognizer> Grammar::findRule(const string &argname){
	string name=tolower(argname);
	auto it=mRules.find(name);
	if (it!=mRules.end()){
		return (*it).second;
	}
	return nullptr;
}

shared_ptr<Recognizer> Grammar::getRule(const string &argname){
	shared_ptr<Recognizer> ret=findRule(argname);

	if (ret){
		shared_ptr<RecognizerPointer> pointer=dynamic_pointer_cast<RecognizerPointer>(ret);
		if (pointer){
			if (pointer->getPointed()){/*if pointer is defined return the pointed value directly*/
				return pointer->getPointed();
			}else{
				return pointer;
			}
		}
		return ret;
	}else{/*the rule doesn't exist yet: return a pointer*/
		shared_ptr<RecognizerPointer> recognizerPointer = make_shared<RecognizerPointer>();
		ret=recognizerPointer;
		string name=tolower(argname);
		ret->setName(string("@")+name);
		mRules[name]=ret;
		mRecognizerPointers.push_back(recognizerPointer);
	}
	return ret;
}

void Grammar::include(const shared_ptr<Grammar>& grammar){
	for(auto it=grammar->mRules.begin();it!=grammar->mRules.end();++it){
		if (mRules.find((*it).first)!=mRules.end()){
			cerr<<"Rule '"<<(*it).first<<"' is being redefined while including grammar '"<<grammar->mName<<"' into '"<<mName<<"'"<<endl;
		}
		mRules[(*it).first]=(*it).second;
	}
}

bool Grammar::isComplete()const{
	bool ret=true;
	for(auto it=mRules.begin(); it!=mRules.end(); ++it){
		shared_ptr<RecognizerPointer> pointer=dynamic_pointer_cast<RecognizerPointer>((*it).second);
		if (pointer && !pointer->getPointed()){
			cerr<<"Rule '"<<(*it).first<<"' is not defined."<<endl;
			ret=false;
		}
	}
	return ret;
}

void Grammar::optimize(){
	for(auto it=mRules.begin(); it!=mRules.end(); ++it){
		(*it).second->optimize();
	}
}


int Grammar::getNumRules() const{
	return (int)mRules.size();
}

/**
Saves the rules hash map into output stream
Format for each line : RuleName => Recognizer
@param: std::ofstream &outFile : outstream to which data is written
*/
void Grammar::saveRulesMap(ofstream &outFile){
	long grammarSavePosition = 0;
		if(outFile){
			for(map<string,shared_ptr<Recognizer>>::iterator it= mRules.begin(); it != mRules.end() ;it++){
				outFile.seekp(grammarSavePosition, ios::beg);
				outFile << it->first << " => ";
				grammarSavePosition = outFile.tellp();
				(it->second)->save(outFile, grammarSavePosition);
				outFile << "\r\n";
			}
		}
		else{
			cerr << "ERROR writing to file : output stream unavailable" << endl;
		}
	}

/**
	Saves the recognizer list into output stream
	Format for each line : RecognizermName => RecognizermId Recognizer
	@param: std::ofstream &outFile : outstream to which data is written
*/
void Grammar::savePointersList(ofstream &outFile){
	long grammarSavePosition = 0;
		if(outFile){
			for(list<shared_ptr<RecognizerPointer>>::iterator iter= mRecognizerPointers.begin(); iter != mRecognizerPointers.end() ;iter++){
				outFile.seekp(grammarSavePosition, ios::beg);
				outFile << (*iter)->getName() <<  " => " << (*iter)->getId() << " ";

				grammarSavePosition = outFile.tellp();
				((*iter)->getPointed())->save(outFile, grammarSavePosition);
				outFile << "\r\n";
			}
		}
		else{
			cerr << "ERROR writing to file : output stream unavailable" << endl;
		}

}

/**
Function that saves the entire grammar into a file which name is specialized as a parameter
Save format :
	GrammarName ==>
	RecognizerPointer1Name => RecognizerPointer1Id PointedElement
	RecognizerPointer2Name => RecognizerPointer2Id PointedElement
	RecognizerPointer3Name => RecognizerPointer3Id PointedElement
	.....
	END OF RECPTRS
	Rule1Name => Recognizer
	Rule2Name => Recognizer
	Rule3Name => Recognizer
	.....
*/
void Grammar::createGrammarDump(string dumpFileName){
	ofstream outFile (dumpFileName, ios::out | ios::app | ios::binary);
	outFile << mName << " ==> "<<endl;
	this->savePointersList(outFile);
	outFile << "END OF RECPTRS \r\n" ;
	this->saveRulesMap(outFile);
	outFile.close();
}

/**
template function to parse a string ina vector using a one character delimeter
*/

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

/**
Function that loads the grammar from the parameter specified fileName
	@param : std::string fileName :in File name , file in which the grammar was dumped
	@return : std::shared_ptr<Grammar> : Grammar loaded form the file
*/
shared_ptr<Grammar> Grammar::loadVectRulesMap(string fileName){

  string mNameLoaded = ""; // grammar name
  string separator = ""; // local variable to parse file separators
  vector <string> fileStringRules;
  string tempKey = ""; // local variable to parse rules names
  string tempTag = ""; // local varaible to parse rules tag
  shared_ptr<Recognizer> tempValue = NULL; // local variable to parse rules Recognizer
  std::map<std::string,std::shared_ptr<Recognizer>> mRulesLoaded; //map containing all grammar rules for return value
	std::list<std::shared_ptr<Recognizer>> mRecognizerPointersLoaded; //list containing recognizer pointers
	ifstream inFile (fileName, ios::in | ios::binary); //file stream to work on
	shared_ptr<Grammar> retGram = NULL;

  if ( !inFile )
    cerr << "ERROR : error reading file : no such name";
  else{
		//read the whole file in one shot
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    inFile.close();

    string stringBuffer = buffer.str(); //convert it into a regular string for parsing purposes
    std::vector<std::string> fileStringRules = split(stringBuffer, '\n'); // split the buffer into '\n' separated stringstream
		//fileStringRules each colun represents a line in the file

		//split the first line with ' ' as separator
    vector<string>::const_iterator fileIter = fileStringRules.begin();
    string line = (*fileIter);
    std::vector<std::string> result = split(line, ' '); //variable reused for each line parsing
    vector<string>::const_iterator it = result.begin();

		//read the first line and parse grammar name : support for space separated rule names
    mNameLoaded = *(it);
    separator = *(++it);
    while (separator != "==>") {
      mNameLoaded += ( " " + separator);
      separator = *(++it);
    }
		retGram = make_shared<Grammar>(mNameLoaded);
		std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItBegin = retGram->mRecognizerPointers.begin();
		std::list<shared_ptr<RecognizerPointer>>::iterator rcptrItEnd = retGram->mRecognizerPointers.end();

  	++fileIter;
		line = (*fileIter);

	//read the recognizer pointers first
		while( line.compare("END OF RECPTRS \r"))
		{

			rcptrItBegin = (retGram->mRecognizerPointers.begin());
			rcptrItEnd = (retGram->mRecognizerPointers.end());

			line =  (*fileIter);
			line.erase(line.size()-1,1); // since grammar are written with a CRLF format: we have at each end of line a "\r\n"
			//we parsed based on \n so now we have at each end of line a useless \r character
			result = split(line, ' ');
			it = result.begin();
			//parsing the rule name : support for space separated rule names
			tempKey = *it;
			separator = *(++it);
			while (separator != "=>") {
				tempKey +=separator;
				separator = *(++it);
			}
			//read a recognizer pointer and put it in da list
			//Parse the recognizer based on the tag found at the beginning
			int tempId = stoi(*(++it));
			tempTag = *(++it);

			if(tempTag == "CR"){tempValue = CharRecognizer::loadVect(it);}
			else if(tempTag == "SEL"){tempValue = Selector::loadVect(it, rcptrItBegin, rcptrItEnd);}
			else if(tempTag == "XSEL"){tempValue = ExclusiveSelector::loadVect(it, rcptrItBegin, rcptrItEnd);}
			else if(tempTag == "XCR"){tempValue = CharRange::loadVect(it);}
			else if(tempTag == "SEQ"){tempValue = Sequence::loadVect(it, rcptrItBegin, rcptrItEnd);}
			else if(tempTag == "LOOP"){tempValue = Loop::loadVect(it, rcptrItBegin, rcptrItEnd);}
			else if(tempTag == "LIT"){tempValue = Literal::loadVect(it);}
			else if(tempTag == "RECP") { tempValue = RecognizerPointer::loadVect(it, rcptrItBegin, rcptrItEnd);
			}
			//create the recognizer pointer set its name and value
			shared_ptr<RecognizerPointer> recognpointer_reloaded =  make_shared<RecognizerPointer>();
			recognpointer_reloaded->setOnlyName(tempKey.c_str());
			recognpointer_reloaded->setPointed(tempValue);
			recognpointer_reloaded->setId(tempId);
			//insert it into the dummy pointers list
			retGram->mRecognizerPointers.push_back(recognpointer_reloaded);

			++fileIter;
			line = *fileIter;
		}

		rcptrItBegin = (retGram->mRecognizerPointers.begin());
		rcptrItEnd = (retGram->mRecognizerPointers.end());

		//do the link for recogizer pointers inside other recognizer pointers :: VERY IMPORTANT
		for (std::list<shared_ptr<RecognizerPointer>>::iterator linkageIter = retGram->mRecognizerPointers.begin(); linkageIter != retGram->mRecognizerPointers.end() ; linkageIter++){
				((*linkageIter)->getPointed())->linkPointer(rcptrItBegin, rcptrItEnd);
		}

		++fileIter;
		while( fileIter !=  fileStringRules.end() )
		{

			line =  (*fileIter);
			line.erase(line.size()-1,1); // since grammar are written with a CRLF format: we have at each end of line a "\r\n"
			//we parsed based on \n so now we have at each end of line a useless \r character
			result = split(line, ' ');
			it = result.begin();
			//parsing the rule name : support for space separated rule names
			tempKey = *it;
			separator = *(++it);
			while (separator != "=>") {
				tempKey +=separator;
				separator = *(++it);
			}
			//read a recognizer pointer and put it in da list
			//Parse the recognizer based on the tag found at the beginning
			tempTag = *(++it);
			string mNameReloaded = tempKey;
			if(tempTag == "CR"){tempValue = CharRecognizer::loadVect(it);}
			else if(tempTag == "SEL"){tempValue = Selector::loadVect(it, rcptrItBegin, rcptrItEnd);}
			else if(tempTag == "XSEL"){tempValue = ExclusiveSelector::loadVect(it, rcptrItBegin, rcptrItEnd);}
			else if(tempTag == "XCR"){tempValue = CharRange::loadVect(it);}
			else if(tempTag == "SEQ"){tempValue = Sequence::loadVect(it, rcptrItBegin, rcptrItEnd);}
			else if(tempTag == "LOOP"){tempValue = Loop::loadVect(it, rcptrItBegin, rcptrItEnd);}
			else if(tempTag == "LIT"){tempValue = Literal::loadVect(it);}
			else if(tempTag == "RECP") { tempValue = RecognizerPointer::loadVect(it, rcptrItBegin, rcptrItEnd);}


			if(tempValue->getName() != tempKey){
				mNameReloaded = tempValue->getName();
			}

			tempValue->setOnlyName(mNameReloaded);
			retGram->mRules[tempKey] = tempValue;
			++fileIter;
		}

  }
	bctbx_message("[belr] Succesfully created grammar with %i rules.", retGram->getNumRules());
	if (retGram->isComplete()){
		bctbx_message("[belr] Grammar is complete.");
		retGram->optimize();
		bctbx_message("[belr] Grammar has been optimized.");
	}else{
		bctbx_warning("[belr] Grammar is not complete.");
	}
  return retGram;
}


bool Grammar::equal(shared_ptr<Grammar> &gramCompared){
	bool condition = true;
	list<shared_ptr<RecognizerPointer>>::iterator itRcp = mRecognizerPointers.begin();
	list<shared_ptr<RecognizerPointer>>::iterator	itRcpComp = gramCompared->mRecognizerPointers.begin();
	map<string,shared_ptr<Recognizer>>::iterator it= mRules.begin();
	map<string,shared_ptr<Recognizer>>::iterator itComp= gramCompared->mRules.begin();

	//compare names
	if(mName != gramCompared->mName){condition = false;}

	int i =0;
	while (itRcp!=mRecognizerPointers.end() && condition){
		if (itRcpComp == gramCompared->mRecognizerPointers.end()) condition =  false;
			itRcpComp++;
			itRcp++;
			i++;
	}

	//count the number of rules and check if equal between two grammars

	int j = 0;
	while (it!=mRules.end()){
		if (itComp == gramCompared->mRules.end() && condition ) condition = false;
			it++;
			itComp++;
			j++;
		}

	//compare rules
	itComp = gramCompared->mRules.begin();
	it = mRules.begin();
	while (it != mRules.end()/* && condition*/){
		if(it->first == itComp->first){
			if(!(*(it->second) == *(itComp->second))){
				condition = false;
				cout << "MERDE" << endl;
			}
		}
		else{
			condition = false;
		}
		it++;
		itComp++;
	}

	//compare recognizer pointers
	itRcp = mRecognizerPointers.begin();
	itRcpComp = gramCompared->mRecognizerPointers.begin();
	while (itRcp!=mRecognizerPointers.end() && condition){
		string firstRule = (*itRcp)->getName();
		string secondRule = (*itRcpComp)->getName();
		if (!firstRule.compare(secondRule)){
			if (!(*(*itRcp)->getPointed() == *(*itRcpComp)->getPointed())){
				condition =  false;
			}
		}
		else
			condition = false;
		itRcp++;
		itRcpComp++;
		}



	return condition;

}



string belr::tolower(const string &str){
	string ret(str);
	transform(ret.begin(),ret.end(), ret.begin(), ::tolower);
	return ret;
}
