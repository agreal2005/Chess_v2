CC = g++
CFLAGS = -O3 -std=c++17
TARG = test

# My testing file is test.cpp, you can change it accordingly
DEPS = fen.o functions.o moves.o test.o

$(TARG): $(DEPS)
	$(CC) $(CFLAGS) -o $@ $(DEPS)

test.o: test.cpp 
	$(CC) $(CFLAGS) -c -o $@ $<


%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o test