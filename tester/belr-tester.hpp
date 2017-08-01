/*
	belr-tester.hpp
	Copyright (C) 2015  Belledonne Communications SARL

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef belr_tester_hpp
#define belr_tester_hpp

#include <bctoolbox/tester.h>
#include "belr/grammarbuilder.hh"
#include "belr/abnf.hh"
#include "belr/belr.hh"


#include <chrono>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>
#include <iostream>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif



extern test_suite_t recognizer_char_test_suite;
extern test_suite_t recognizer_selector_test_suite;
extern test_suite_t recognizer_exclusiveselector_test_suite;
extern test_suite_t recognizer_sequence_test_suite;
extern test_suite_t recognizer_loop_test_suite;
extern test_suite_t recognizer_charrange_test_suite;
extern test_suite_t recognizer_literal_test_suite;
extern test_suite_t recognizer_recognizerpointer_test_suite;
extern test_suite_t grammar_simple_test_suite;
extern test_suite_t grammar_vcardgrammar_suite;
extern test_suite_t grammar_sipgrammar_suite;



void belr_tester_init(void(*ftester_printf)(int level, const char *fmt, va_list args));
void belr_tester_uninit(void);

#ifdef __cplusplus
};
#endif



#endif
