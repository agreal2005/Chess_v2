CC = g++
CFLAGS = -O3 -std=c++17
TARG = Test
TEST_TARG = func_me_up

# My testing file is test.cpp
DEPS = fen.o functions.o moves.o sq_tables_eval.o eval.o search.o test.o
DEPS2 = fen.o functions.o function_testing.o

# Default target for the main executable
$(TARG): $(DEPS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule for compiling test.o
test.o: test.cpp 
	$(CC) $(CFLAGS) -c -g -o $@ $<

# Rule for compiling function_testing.o
function_testing.o: function_test.cpp
	$(CC) $(CFLAGS) -c -g -o $@ $<

# Generic rule for compiling .o files from .cpp and .h files
%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c -g -o $@ $<

# Clean target to remove object files and executables
clean:
	del *.o $(TARG).exe $(TEST_TARG)

# Target for creating func_me_up executable
$(TEST_TARG): $(DEPS2)
	$(CC) $(CFLAGS) -o $@ $^
