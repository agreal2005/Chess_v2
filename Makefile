CC = g++
CFLAGS = -O3 -std=c++17 -g  # Added -g for debugging
TARG = Test
STD_TARG = func_me_up
TRAIN_TARG = train_me_please

# Dependencies for the main executable (Test)
DEPS = fen.o functions.o moves.o sq_tables_eval.o eval.o search.o test.o
# Dependencies for func_me_up executable
DEPS2 = fen.o functions.o function_testing.o
# Dependencies for training executable
DEPS3 = fen.o functions.o moves.o sq_tables_eval.o eval.o search.o train.o

# Default target: Test executable
$(TARG): $(DEPS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile test.o from test.cpp
test.o: test.cpp 
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile function_testing.o from function_test.cpp
function_testing.o: function_test.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile train.o from train.cpp (training dataset generator)
train.o: train.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# Generic rule for compiling .o files from .cpp/.h
%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Create func_me_up executable
$(STD_TARG): $(DEPS2)
	$(CC) $(CFLAGS) -o $@ $^

# Create train_me_please executable
$(TRAIN_TARG): $(DEPS3)
	$(CC) $(CFLAGS) -o $@ $^
