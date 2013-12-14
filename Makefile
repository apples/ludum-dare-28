
.SUFFIXES: .d

SYS_DIR = E:/Gensokyo
IRRKLANG_DIR = ./irrKlang-1.4.0

# Preprocessor

CPPFLAGS += -isystem $(SYS_DIR)/include
CPPFLAGS += -DGLEW_STATIC -DPNG_STATIC -DZLIB_STATIC
CPPFLAGS += -I$(IRRKLANG_DIR)/include

# C++ Compiler

CXX       = g++ -m32
CXXFLAGS += -O3 -Wall -Weffc++ -std=c++11 -pedantic

# Linker

LD        = g++ -m32
LDFLAGS  += -mwindows -static
LDFLAGS  += -L$(SYS_DIR)/lib
LDFLAGS  += -L$(IRRKLANG_DIR)/lib/win32-gcc
LDLIBS   += -lglfw3 -lpng -lz -lglew32 -lopengl32 -lglu32 -lgdi32
LDLIBS   += -lirrKlang

# Files

SRC_FILES := $(shell find src/ -type f -name '*.cpp')
OBJ_FILES := $(patsubst src/%.cpp,obj/%.o,$(SRC_FILES))
DEP_FILES := $(patsubst src/%.cpp,dep/%.d,$(SRC_FILES))

MAIN_EXE = game.exe

# Targets

.PHONY: all
all: $(MAIN_EXE)

$(MAIN_EXE): $(OBJ_FILES)
	$(LD) $(LDFLAGS) -o $@ $(OBJ_FILES) $(LDLIBS)

# All Sources

obj/%.o: src/%.cpp dep/%.d
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Dependency Generation

dep/%.d: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM $< -MT $(patsubst dep/%.d,obj/%.o,$@) -MF $@

ifeq (0, $(words $(filter-out $(NODEPS),$(MAKECMDGOALS))))
-include $(DEP_FILES)
endif

# Clean

.PHONY: clean
clean:
	rm -rf $(MAIN_EXE) obj/ dep/
