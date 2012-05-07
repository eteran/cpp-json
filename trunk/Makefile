
SHELL = /bin/sh

# suffix Definitions
.SUFFIXES:                # Delete the default suffixes
.SUFFIXES: .cc .o .h .tcc # Define our suffix list
	
.cc.o:
	@echo compiling $< ...
	@$(CXX) $(CXXFLAGS) $(DEFINES) -c $< -o $@ 1>/dev/null
	

CXX    := g++ -march=native -ansi
LD     := g++
RM     := rm -f
ED     := ed

APPNAME := cpp-json

DEFINES  := 
INCLUDES := 
CXXFLAGS := -g -O2 -pedantic -pipe $(INCLUDES) -W -Wall
LDFLAGS  := -g

H_FILES     := 
CXX_FILES   := main.cc
O_FILES     := $(CXX_FILES:.cc=.o)
SOURCEFILES := $(H_FILES) $(CXX_FILES)
.PRECIOUS: $(SOURCEFILES)
.PHONY:    clean distclean mrproper

# main targets
all: $(APPNAME)

$(APPNAME): $(O_FILES)
	@echo linking $@ ...
	@$(LD) $(LDFLAGS) $(O_FILES) -o $@ 1>/dev/null

mrproper: clean
	$(RM) makedep

clean:
	$(RM) $(O_FILES) $(APPNAME) core *~ 2> /dev/null

distclean: mrproper
	
depend:
	@echo "Building Dependency Information..."
	@$(RM) makedep
	@$(CXX) $(DEFINES) $(INCLUDES) -MM $(CXX_FILES) 2>/dev/null >> makedep || true
	@echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
	@echo '$$r makedep' >>eddep
	@echo 'w' >>eddep
	@$(ED) - Makefile < eddep
	@$(RM) eddep makedep 
	@echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
	@echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	@echo '# see make depend above' >> Makefile
	@echo "Done"
        
#-----------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend uses it
#-----------------------------------------------------------------
main.o: main.cc json.h json/token.h json/exception.h json/object.h \
 json/array.h json/value.h json/json.tcc json/object.tcc json/array.tcc \
 json/value.tcc
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
