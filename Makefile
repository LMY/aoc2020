DIR_SRC  = .
DIR_BIN  = .
SRCS    := $(wildcard $(DIR_SRC)/*.cpp)
TXT     := $(wildcard $(DIR_SRC)/*.txt)
OBJ     := $(SRCS:%.cpp=%.o)
EXE     := $(SRCS:%.cpp=%)

CPPFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror
LDLIBS = -lm

# main rule: create all executables
all: $(EXE)

# each exe depends upon its .o
$(EXE): $(DIR_BIN)/% : $(DIR_BIN)/%.o
	g++ -o $@ $^ $(LDLIBS)

# each .o depends upon its .cpp
$(OBJ): $(DIR_BIN)/%.o : $(DIR_SRC)/%.cpp
	g++ -c -o $@ $< $(CPPFLAGS)

run:
	@$(foreach exe, $(sort $(EXE)), echo "$(exe)"; $(exe); echo "";)

check-leaks:
	rm -f memleak.log
	@$(foreach exe, $(sort $(EXE)), valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(exe) >> memleak.log 2>&1;)

backup:
	tar -cf ../aoc-2020.tar Makefile $(SRCS) $(TXT) *.h

.PHONY: all clean run check-leaks backup

clean:
	rm -f $(OBJ) $(EXE) memleak.log
