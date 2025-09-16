CC = g++
CFLAGS = -O3 -std=c++23
TARG = Test
TEST_TARG = func_me_up

ifeq ($(OS),Windows_NT)
    RM = del /Q /F
else
    RM = rm -f
endif

DEPS = gamestate.o functions.o moves.o sq_tables_eval.o eval.o search.o test.o
DEPS2 = gamestate.o functions.o function_testing.o

$(TARG): $(DEPS)
	$(CC) $(CFLAGS) -o $@ $^

test.o: test.cpp
	$(CC) $(CFLAGS) -c -g -o $@ $<

function_testing.o: function_test.cpp
	$(CC) $(CFLAGS) -c -g -o $@ $<

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c -g -o $@ $<

clean:
	$(RM) *.o $(TARG).exe $(TARG) $(TEST_TARG)

$(TEST_TARG): $(DEPS2)
	$(CC) $(CFLAGS) -o $@ $^