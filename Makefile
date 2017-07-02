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

CC =		cc
CXX =		CC

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
CXXFLAGS =	-g -xildoff -xsb
CFLAGS =	-g
CLIBFLAGS =	-lm -lncurses
CCLIBFLAGS =	


CPP_FILES =	
C_FILES =	action.c condition.c display.c entity.c exit.c fileProcess.c level.c room.c setting.c textplore.c utils.c
PS_FILES =	
S_FILES =	
H_FILES =	action.h condition.h display.h entity.h exit.h fileProcess.h level.h room.h setting.h textplore.h utils.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	action.o condition.o display.o entity.o exit.o fileProcess.o level.o room.o setting.o utils.o 

all:	textplore 

textplore:	textplore.o $(OBJFILES)
	$(CC) $(CFLAGS) -o textplore textplore.o $(OBJFILES) $(CLIBFLAGS)

action.o:	action.h condition.h display.h entity.h exit.h fileProcess.h level.h room.h setting.h textplore.h utils.h
condition.o:	action.h condition.h display.h entity.h exit.h fileProcess.h level.h room.h setting.h textplore.h utils.h
display.o:	display.h
entity.o:	action.h condition.h display.h entity.h exit.h fileProcess.h level.h room.h setting.h textplore.h utils.h
exit.o:	action.h condition.h display.h entity.h exit.h fileProcess.h level.h room.h setting.h textplore.h utils.h
fileProcess.o:	action.h condition.h display.h entity.h exit.h fileProcess.h level.h room.h setting.h textplore.h utils.h
level.o:	action.h condition.h display.h entity.h exit.h fileProcess.h level.h room.h setting.h textplore.h utils.h
room.o:	action.h condition.h display.h entity.h exit.h fileProcess.h level.h room.h setting.h textplore.h utils.h
setting.o:	setting.h utils.h
textplore.o:	action.h condition.h display.h entity.h exit.h fileProcess.h level.h room.h setting.h textplore.h utils.h
utils.o:	utils.h

clean:
	-/bin/rm $(OBJFILES) textplore.o textplore 2> /dev/null

realclean:        clean
	-/bin/rm -rf textplore 
