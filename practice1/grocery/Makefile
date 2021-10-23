BINDIR := bin

CXX := g++
CXXFLAGS := -g -Wall --std=c++11

VALGRIND := valgrind --tool=memcheck --leak-check=yes

GTEST_CXXFLAGS := -I /usr/include/gtest/
GTEST_LDFLAGS := -l gtest_main -l gtest -pthread

all: test

# Create binary dir
$(BINDIR)/.dirstamp:
	mkdir -p $(BINDIR)
	touch $(BINDIR)/.dirstamp

# provided test executable
grocery-tests: $(BINDIR)/grocery-tests.o $(BINDIR)/grocery.o
	$(CXX) $(CXXFLAGS) $^ $(GTEST_LDFLAGS) -o $@

# grading test executable
grocery-grading: $(BINDIR)/grocery-grading-tests.o $(BINDIR)/grocery.o
	$(CXX) $(CXXFLAGS) $^ $(GTEST_LDFLAGS) -o $@

# test on solution executable
grocery-tests-sol: $(BINDIR)/grocery-tests.o $(BINDIR)/grocery-sol.o
	$(CXX) $(CXXFLAGS) $^ $(GTEST_LDFLAGS) -o $@

# grading test on solution executable
grocery-grading-sol: $(BINDIR)/grocery-grading-tests.o $(BINDIR)/grocery-sol.o
	$(CXX) $(CXXFLAGS) $^ $(GTEST_LDFLAGS) -o $@

# generic rule for .cpp files
$(BINDIR)/%.o : %.cpp $(BINDIR)/.dirstamp
	$(CXX) -c $(CXXFLAGS) $(GTEST_CXXFLAGS) $< -o $@

# student test target
test: grocery-tests
	$(VALGRIND) ./grocery-tests

# grading test target
grade: grocery-grading
	$(VALGRIND) ./grocery-grading

# header dependencies
$(BINDIR)/grocery-tests.o: testing-utils.h grocery.h
$(BINDIR)/grocery-grading-tests.o: testing-utils.h grocery.h
$(BINDIR)/grocery.o: grocery.h
$(BINDIR)/grocery-sol.o: grocery.h

# clean
clean:
	rm -rf $(BINDIR) grocery-tests grocery-grading grocery-grading-sol grocery-tests-sol

.PHONY: test grade clean
