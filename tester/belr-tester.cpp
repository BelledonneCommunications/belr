/*
 * Copyright (c) 2016-2019 Belledonne Communications SARL.
 *
 * This file is part of belr - a language recognition library for ABNF-defined grammars.
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "belr-tester.h"

#include <fstream>
#include <sstream>

#include <bctoolbox/logging.h>

#include "config.h"

using namespace ::std;

namespace belr {

std::string bcTesterFile(const std::string &name) {
	char *file = bc_tester_file(name.c_str());
	std::string ret(file);
	bc_free(file);
	return ret;
}

std::string bcTesterRes(const std::string &name) {
	char *file = bc_tester_res(name.c_str());
	std::string ret(file);
	bc_free(file);
	return ret;
}

std::string openFile(const std::string &name) {
	std::ifstream istr(name, std::ios::binary);
	if (!istr.is_open()) {
		bctbx_error("Fail to open %s", name.c_str());
		BC_FAIL("Fail to open file");
		return "";
	}

	std::stringstream tmpStream;
	tmpStream << istr.rdbuf();
	return tmpStream.str();
	;
}

} // namespace belr

int main(int argc, char *argv[]) {
	int i;
	int ret;

	belr_tester_init(NULL);

	char *dirname = bctbx_dirname(argv[0]);
	std::string resDir = string(dirname) + "/../share/belr-tester/res/";
	std::string testResource = resDir + "sipgrammar.txt";
	bctbx_free(dirname);
	if (bctbx_file_exist(testResource.c_str()) == 0) {
		bc_tester_set_resource_dir_prefix(resDir.c_str());
		printf("Resource dir set to %s\n", resDir.c_str());
	} else {
		bc_tester_set_resource_dir_prefix(BELR_LOCAL_RESOURCE_LOCATION);
		printf("Resource dir set to %s\n", BELR_LOCAL_RESOURCE_LOCATION);
	}
	bc_tester_set_writable_dir_prefix("./");

	for (i = 1; i < argc; ++i) {
		ret = bc_tester_parse_args(argc, argv, i);
		if (ret > 0) {
			i += ret - 1;
			continue;
		} else if (ret < 0) {
			bc_tester_helper(argv[0], "");
		}
		return ret;
	}

	ret = bc_tester_start(argv[0]);
	belr_tester_uninit();
	return ret;
}

static void log_handler(int lev, const char *fmt, va_list args) {
#ifdef _WIN32
	vfprintf(lev == BCTBX_LOG_ERROR ? stderr : stdout, fmt, args);
	fprintf(lev == BCTBX_LOG_ERROR ? stderr : stdout, "\n");
#else
	va_list cap;
	va_copy(cap, args);
	/* Otherwise, we must use stdio to avoid log formatting (for autocompletion etc.) */
	vfprintf(lev == BCTBX_LOG_ERROR ? stderr : stdout, fmt, cap);
	fprintf(lev == BCTBX_LOG_ERROR ? stderr : stdout, "\n");
	va_end(cap);
#endif
}

void belr_tester_init(void (*ftester_printf)(int level, const char *fmt, va_list args)) {
	if (ftester_printf == NULL) ftester_printf = log_handler;
	bc_tester_init(ftester_printf, BCTBX_LOG_MESSAGE, BCTBX_LOG_ERROR, ".");

	bc_tester_add_suite(&grammar_suite);
	bc_tester_add_suite(&parser_suite);
}

void belr_tester_uninit(void) {
	bc_tester_uninit();
}
