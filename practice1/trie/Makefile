BINDIR := bin

CXX := g++
CXXFLAGS := -g -Wall --std=c++11

VALGRIND := valgrind --tool=memcheck

GTEST_CXXFLAGS := -I /usr/include/gtest/
GTEST_LDFLAGS := -l gtest_main -l gtest -pthread

all: test

# Create binary dir
$(BINDIR)/.dirstamp:
	mkdir -p $(BINDIR)
	touch $(BINDIR)/.dirstamp

# provided test executable
trie-tests: $(BINDIR)/trie-tests.o $(BINDIR)/trie.o $(BINDIR)/trie-test-functions.o
	$(CXX) $(CXXFLAGS) $^ $(GTEST_LDFLAGS) -o $@

# grading test executable
trie-grading: $(BINDIR)/trie-grading-tests.o $(BINDIR)/trie.o $(BINDIR)/trie-test-functions.o
	$(CXX) $(CXXFLAGS) $^ $(GTEST_LDFLAGS) -o $@

# provided test using solution executable
trie-tests-sol: $(BINDIR)/trie-tests.o $(BINDIR)/trie-sol.o $(BINDIR)/trie-test-functions.o
	$(CXX) $(CXXFLAGS) $^ $(GTEST_LDFLAGS) -o $@

# grading test using solution executable
trie-grading-sol: $(BINDIR)/trie-grading-tests.o $(BINDIR)/trie-sol.o $(BINDIR)/trie-test-functions.o
	$(CXX) $(CXXFLAGS) $^ $(GTEST_LDFLAGS) -o $@

# generic rule for .cpp files
$(BINDIR)/%.o : %.cpp $(BINDIR)/.dirstamp
	$(CXX) -c $(CXXFLAGS) $(GTEST_CXXFLAGS) $< -o $@

# student test target
test: trie-tests
	$(VALGRIND) ./trie-tests

# grading test target
grade: trie-grading
	$(VALGRIND) ./trie-grading

# header dependencies
$(BINDIR)/trie-tests.o: trie-test-functions.h trie.h
$(BINDIR)/trie-grading-tests.o: trie-test-functions.h trie.h
$(BINDIR)/trie.o: trie.h
$(BINDIR)/trie-sol.o: trie.h
$(BINDIR)/trie-test-functions.o: trie-test-functions.h trie.h

# clean
clean:
	rm -rf $(BINDIR) trie-tests trie-grading trie-tests-sol trie-grading-sol

.PHONY: test grade clean
