#include <iostream>
#include <sstream>

#include "curricula.hpp"
#include "rational.h"

using namespace std;

// Student instructions
// To check your own homework, drop this file and curricula.hpp in your
// rational/ directory and compile it with rational.cpp. To run a case,
// run the following on the command line:
//
// $ ./harness test_{test_name}
//
// Where {test_name} is any of the below tests, i.e. constructor_default or
// addition_rational, like so:
//
// $ ./harness test_addition_rational
//
// Then make sure the output matches the commented correct response.

HARNESS_BEGIN

// 0/1
TEST(constructor_default) {
    Rational r;
    cout << r;
    return 0;
}

// -1/5
TEST(constructor_basic) {
    Rational r(4, -20);
    cout << r;
    return 0;
}

// -9/5 7/3 1/2
TEST(constructor_iostream) {
    stringstream in("18 /-10 -14 / -6 2/4");
    Rational r1, r2, r3;
    in >> r1 >> r2 >> r3;
    cout << r1 << " " << r2 << " " << r3;
    return 0;
}

// -2/1
TEST(addition_rational) {
    Rational r1(-4, 20), r2(-18, 10);
    cout << (r1 + r2);
    return 0;
}

// 4/5 4/5
TEST(addition_integral) {
    Rational r1(-4, 20);
    cout << (r1 + 1) << " " << (1 + r1);
    return 0;
}

// 9/25
TEST(multiplication_rational) {
    Rational r1(-4, 20), r2(-18, 10);
    cout << (r1 * r2);
    return 0;
}

// -2/5 -2/5
TEST(multiplication_integral) {
    Rational r1(-4, 20);
    cout << (r1 * 2) << " " << (2 * r1);
    return 0;
}

// -1/125
TEST(exponent_positive) {
    Rational r(-4, 20);
    cout << (r ^ 3);
    return 0;
}

// 1/1
TEST(exponent_zero) {
    Rational r(-4, 20);
    cout << (r ^ 0);
    return 0;
}

// -125/1
TEST(exponent_negative) {
    Rational r(-4, 20);
    cout << (r ^ -3);
    return 0;
}

// 1 0
TEST(equality) {
    Rational r1(-4, 20), r2(-4, 20), r3(-18, 10);
    cout << (r1 == r2) << " " << (r2 == r3);
    return 0;
}

// 0 1
TEST(inequality) {
    Rational r1(-4, 20), r2(-4, 20), r3(-18, 10);
    cout << (r1 != r2) << " " << (r2 != r3);
    return 0;
}

// 1 0
TEST(less) {
    Rational r1(-9, 5), r2(7, 3);
    cout << (r1 < r2) << " " << (r2 < r1);
    return 0;
}

// -2/1
TEST(assignment_addition_rational) {
    Rational r1(-4, 20), r2(-18, 10);
    r1 += r2;
    cout << r1;
    return 0;
}

// 4/5
TEST(assignment_addition_integral) {
    Rational r1(-4, 20);
    r1 += 1;
    cout << r1;
    return 0;
}

// 9/25
TEST(assignment_multiplication_rational) {
    Rational r1(-4, 20), r2(-18, 10);
    r1 *= r2;
    cout << r1;
    return 0;
}

// -2/5
TEST(assignment_multiplication_integral) {
    Rational r1(-4, 20);
    r1 *= 2;
    cout << r1;
    return 0;
}

// 0/1
TEST(normalized_zero) {
    Rational r1(-4, 20), r2(8, 40);
    cout << r1 + r2;
    return 0;
}

// 1/1
TEST(addition_with_reduction) {
    Rational r1(3, 13), r2(10, 13);
    cout << r1 + r2;
    return 0;
}

// 1/1
TEST(multiplication_with_reduction) {
    Rational r1(7, 9), r2(9, 7);
    cout << r1 * r2;
    return 0;
}

// 1/1
TEST(assignment_addition_with_reduction) {
    Rational r1(3, 13), r2(10, 13);
    r1 += r2;
    cout << r1;
    return 0;
}

// 1/1
TEST(assignment_multiplication_with_reduction) {
    Rational r1(7, 9), r2(9, 7);
    r1 *= r2;
    cout << r1;
    return 0;
}

// -2/5
TEST(assignment_addition_same) {
    Rational r1(-4, 20);
    r1 += r1;
    cout << r1;
    return 0;
}

// 0/1
TEST(constructor_zero) {
    Rational r(0, 24);
    cout << r;
    return 0;
}

HARNESS_END
