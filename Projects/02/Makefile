#
# Created by gmakemake (Ubuntu Jul 25 2014) on Sun Nov 12 19:18:46 2017
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

# any warning will be considered a compiler error.
CFLAGS =	-ggdb -std=c99 -Wall -Wextra -pedantic -Werror -O2
CLIBFLAGS =	-lm -z muldefs

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	hash.c table.c test_table.c
PS_FILES =	
S_FILES =	
H_FILES =	hash.h jukebox.h table.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	hash.o table.o 
LOCAL_LIBS =	libtable.a

#
# Main targets
#

all:	test_table 

test_table:	test_table.o $(OBJFILES)
	$(CC) $(CFLAGS) -o test_table test_table.o $(OBJFILES) $(LOCAL_LIBS) $(CLIBFLAGS)

#
# Dependencies
#

hash.o:	hash.h
table.o:	table.h
test_table.o:	hash.h table.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) test_table.o core

realclean:        clean
	-/bin/rm -f test_table 
