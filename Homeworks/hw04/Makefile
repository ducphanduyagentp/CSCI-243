#
# Created by gmakemake (Ubuntu Jul 25 2014) on Tue Oct  3 15:47:50 2017
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

CC =     gcc
CFLAGS = -ggdb -std=c99 -Wall -Wextra -pedantic

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	movie.c movietest.c mtime.c
PS_FILES =	
S_FILES =	
H_FILES =	movie.h mtime.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	movie.o mtime.o 

#
# Main targets
#

all:	movietest 

movietest:	movietest.o $(OBJFILES)
	$(CC) $(CFLAGS) -o movietest movietest.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

movie.o:	movie.h mtime.h
movietest.o:	movie.h mtime.h
mtime.o:	mtime.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) movietest.o core

realclean:        clean
	-/bin/rm -f movietest 
