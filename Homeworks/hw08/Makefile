#
# Created by gmakemake (Ubuntu Jul 25 2014) on Tue Nov 28 15:43:42 2017
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


CFLAGS =	-ggdb -Wall -Wextra -pedantic -std=c99 -pthread
CLIBFLAGS =	-lpthread -lrt


########## End of flags from header.mak


CPP_FILES =	
C_FILES =	display.c pt-cruisers.c racer.c
PS_FILES =	
S_FILES =	
H_FILES =	display.h racer.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	display.o racer.o 

#
# Main targets
#

all:	pt-cruisers 

pt-cruisers:	pt-cruisers.o $(OBJFILES)
	$(CC) $(CFLAGS) -o pt-cruisers pt-cruisers.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

display.o:	display.h
pt-cruisers.o:	
racer.o:	

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) pt-cruisers.o core

realclean:        clean
	-/bin/rm -f pt-cruisers 
