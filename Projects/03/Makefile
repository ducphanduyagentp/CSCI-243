#
# Created by gmakemake (Ubuntu Jul 25 2014) on Sat Dec  9 11:54:50 2017
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

CFLAGS = -std=c99 -ggdb -Wall -Wextra -pedantic
CLIBFLAGS = -lm

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	fred.c stackADT.c symtab.c utils.c
PS_FILES =	
S_FILES =	
H_FILES =	stackADT.h symtab.h utils.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	stackADT.o symtab.o utils.o 

#
# Main targets
#

all:	fred 

fred:	fred.o $(OBJFILES)
	$(CC) $(CFLAGS) -o fred fred.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

fred.o:	symtab.h utils.h
stackADT.o:	stackADT.h
symtab.o:	symtab.h utils.h
utils.o:	stackADT.h symtab.h utils.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) fred.o core

realclean:        clean
	-/bin/rm -f fred 
