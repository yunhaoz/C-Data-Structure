#include <iostream>
#include <sstream>

#include "curricula.hpp"
#include "reversi.h"

using namespace std;

// Student instructions
// To check your own homework, drop this file and curricula.hpp in your
// reversi/ directory and compile it with reversi.cpp. To run a case,
// run the following on the command line:
//
// $ ./harness test_{test_name}
//
// Where {test_name} is any of the below tests, i.e. <<<>>> or
// <<<>>>, like so:
//
// $ ./harness <<<>>>
//
// Then make sure the output matches the commented correct response.

HARNESS_BEGIN

TEST(square_assignment) {
    Square square;
    square = Square::SquareValue::WHITE;
    EXPECT_EQUAL(square.value_, Square::SquareValue::WHITE);
    return 0;
}

TEST(square_equal) {
    Square square(Square::SquareValue::WHITE);
    EXPECT_TRUE(square == Square::SquareValue::WHITE);
    return 0;
}

TEST(square_unequal) {
    Square square(Square::SquareValue::WHITE);
    EXPECT_TRUE(square != Square::SquareValue::BLACK);
    return 0;
}

TEST(square_output) {
    cout << Square(Square::SquareValue::WHITE) << endl << Square(Square::SquareValue::BLACK);
    return 0;
}

TEST(board_creation) {
    Board board(10);
    EXPECT_EQUAL(board('e', 5), Square::SquareValue::BLACK);
    EXPECT_EQUAL(board('e', 6), Square::SquareValue::WHITE);
    EXPECT_EQUAL(board('f', 5), Square::SquareValue::WHITE);
    EXPECT_EQUAL(board('f', 6), Square::SquareValue::BLACK);
    return 0;
}

TEST(board_copy) {
    Board board1(4);
    Board board2(board1);
    board1('a', 1) = Square::SquareValue::WHITE;
    EXPECT_EQUAL(board2('a', 1), Square::SquareValue::FREE);
    return 0;
}

TEST(board_output) {
    Board board(4);
    board('b', 2) = Square::SquareValue::BLACK;
    board('b', 3) = Square::SquareValue::WHITE;
    board('c', 2) = Square::SquareValue::WHITE;
    board('c', 3) = Square::SquareValue::BLACK;
    cout << board << board;
    return 0;
}

HARNESS_END
