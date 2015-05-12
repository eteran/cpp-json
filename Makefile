
SHELL=/bin/sh

LD      := $(CXX)
MKDIR   := mkdir -p
OBJ_DIR := .obj

.SUFFIXES:

$(OBJ_DIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

TARGET = cpp-json

CXXFLAGS  := -std=c++11 -pedantic -Wextra -Wall -Iinclude -O2 -g3
LDFLAGS   := 
H_FILES   :=
CXX_FILES := main.cc

O_FILES := $(patsubst %.cc, $(OBJ_DIR)/%.o, $(CXX_FILES))
D_FILES := $(O_FILES:.o=.d)

SOURCEFILES :=	$(H_FILES) $(CXX_FILES)
.PRECIOUS:	$(SOURCEFILES)
.PHONY:     all clean

all: $(TARGET)

$(O_FILES): | $(OBJ_DIR) 

$(D_FILES): | $(OBJ_DIR)

$(OBJ_DIR) :
	@$(MKDIR) $@

$(TARGET): $(O_FILES) 
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	$(RM) $(O_FILES) $(D_FILES) $(TARGET)

-include $(D_FILES)

