
SHELL = /bin/sh

# suffix Definitions
.SUFFIXES:                # Delete the default suffixes
.SUFFIXES: .cc .o .h .tcc # Define our suffix list
	
.cc.o:
	@echo compiling $< ...
	@$(CXX) $(CXXFLAGS) $(DEFINES) -c $< -o $@ 1>/dev/null
	

CXX    := g++ -march=native -ansi
RM     := rm -f
ED     := ed

APPNAME := cpp-json

DEFINES  := 
INCLUDES := 
CXXFLAGS := -g -O2 -pedantic -pipe $(INCLUDES) -W -Wall
LDFLAGS  := -g

H_FILES     := json.h json_array.h json_object.h json_token.h json_value.h
TCC_FILES   := json.tcc
CXX_FILES   := json.cc json_array.cc json_object.cc json_value.cc main.cc
O_FILES     := $(CXX_FILES:.cc=.o)
SOURCEFILES := $(H_FILES) $(CXX_FILES) $(TCC_FILES)
.PRECIOUS: $(SOURCEFILES)
.PHONY:    clean distclean mrproper

# main targets
all:	$(APPNAME)

$(APPNAME):	$(O_FILES)
	@echo linking $@ ...
	@$(CXX) $(LDFLAGS) $(O_FILES) -o $@ 1>/dev/null

mrproper: clean
	$(RM) makedep

clean:
	$(RM) $(O_FILES) $(APPNAME) core *~  2> /dev/null

distclean:	mrproper
	
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
json.o: json.cc json.h json_token.h json_object.h json_value.h \
 json_error.h json_array.h json.tcc
json_array.o: json_array.cc json_array.h json_value.h json_token.h \
 json_error.h
json_object.o: json_object.cc json_object.h json_value.h json_token.h \
 json_error.h
json_value.o: json_value.cc json_value.h json_token.h json_error.h json.h \
 json_object.h json_array.h json.tcc
main.o: main.cc json.h json_token.h json_object.h json_value.h \
 json_error.h json_array.h json.tcc
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
