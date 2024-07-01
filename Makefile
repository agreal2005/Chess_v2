CC = g++
CFLAGS = -O3 -std=c++17
TARG = Test

# My testing file is test.cpp, you can change it accordingly
DEPS = fen.o functions.o moves.o sq_tables_eval.o eval.o search.o test.o

$(TARG): $(DEPS)
	$(CC) $(CFLAGS) -o $@ $(DEPS)

test.o: test.cpp 
	$(CC) $(CFLAGS) -c -g -o $@ $<

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c -g -o $@ $<

clean:
	del *.o Test.exe vt.exe
