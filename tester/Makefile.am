EXTRA_DIST=grammars/basicgrammar.txt grammars/vcardgrammar.txt grammars/sipgrammar.txt

noinst_PROGRAMS=belr-tester
belr_tester_SOURCES=belr-tester.cpp \
	belr-charrecognizer-tester.cpp \
	belr-selector-tester.cpp \
	belr-exclusiveselector-tester.cpp \
	belr-sequence-tester.cpp \
	belr-loop-tester.cpp \
	belr-charrange-tester.cpp \
	belr-literal-tester.cpp \
	belr-recognizerpointer-tester.cpp \
	belr-basicgrammar-tester.cpp \
	belr-vcardgrammar-tester.cpp \
	belr-sipgrammar-tester.cpp

belr_tester_LDADD=$(top_builddir)/src/libbelr.la -lbcunit $(BCTOOLBOX_LIBS) $(BCTOOLBOXTESTER_LIBS)

belr_tester_CFLAGS=\
	-DBC_CONFIG_FILE=\"config.h\" \
	$(BCUNIT_CFLAGS)

belr_tester_CXXFLAGS=$(CXXFLAGS)

belr_tester_AM_CPPFLAGS=-I$(top_srcdir)/include \
			-I$(top_srcdir)/tester \
			-I$(top_srcdir)/tester/common

AM_CPPFLAGS=-I$(top_srcdir)/include
