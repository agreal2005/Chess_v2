CC = g++
CFLAGS = -O3 -std=c++17
TARG = Test
TARG2 = vt

# My testing file is test.cpp, you can change it accordingly
DEPS = fen.o functions.o moves.o sq_tables_eval.o eval.o search.o test.o
DEPS2 = fen.o functions.o moves.o sq_tables_eval.o eval.o search.o validmoves_testing.o

$(TARG2): $(DEPS2)
	$(CC) $(CFLAGS) -o $@ $(DEPS2)

$(TARG): $(DEPS)
	$(CC) $(CFLAGS) -o $@ $(DEPS)

test.o: test.cpp 
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	del *.o Test.exe vt.exe
