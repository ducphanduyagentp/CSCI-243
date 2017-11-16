#
# Created by gmakemake (Ubuntu Jul 25 2014) on Sat Oct 21 15:22:08 2017
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

CFLAGS =	-ggdb -std=c99 -Wall -Wextra -pedantic -I/home/course/csci243/pub/include
CLIBFLAGS =	-lm -L/home/course/csci243/pub/lib/ -lobject_list

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	build_graph.c simple_graph.c
PS_FILES =	
S_FILES =	
H_FILES =	simple_graph.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	simple_graph.o 

#
# Main targets
#

all:	build_graph 

build_graph:	build_graph.o $(OBJFILES)
	$(CC) $(CFLAGS) -o build_graph build_graph.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

build_graph.o:	simple_graph.h
simple_graph.o:	simple_graph.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) build_graph.o core

realclean:        clean
	-/bin/rm -f build_graph 
