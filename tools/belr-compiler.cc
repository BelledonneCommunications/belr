/*
 * Copyright (C) 2017  Belledonne Communications SARL
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "belr/grammarbuilder.h"
#include "belr/abnf.h"

using namespace std;
using namespace belr;

int main(int argc, char *argv[]){
	string file, ofile;

	if (argc<3){
		cerr<<argv[0]<<" <grammar file to load> <output filename>"<<endl;
		return -1;
	}
	file = argv[1];
	ofile = argv[2];
	//Create a GrammarBuilder:
	ABNFGrammarBuilder builder;
	//construct the grammar from the grammar file, the core rules are included since required by most RFCs.
	shared_ptr<Grammar> grammar = builder.createFromAbnfFile(file, make_shared<CoreRules>());

	if (!grammar){
		cerr<<"Fail to create grammar."<<endl;
		return -1;
	}
	if (grammar->save(ofile) != 0){
		return -1;
	}
	return 0;
}

