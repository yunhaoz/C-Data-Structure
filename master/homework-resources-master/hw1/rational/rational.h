#ifndef RATIONAL_H
#define RATIONAL_H
#include <iostream>

/**
 * Models a rational number represented as a numerator and denominator
 */
class Rational
{
public:
    /**
     * Default constructor initializing the rational number to 0/1
     */
    Rational();

    /**
     * Initializes the rational number using 'num' as the numerator
     * and 'denom' as denominator.
     *
     * Stores the rational number in reduced form.
     * Ex. Rational(2,-4) should yield a rational number of -1/2
     */
    Rational(int num, int denom);

    /**
     * Appropriate ostream (insertion) operator '<<'
     *
     * When outputting a Rational simply print the numerator (with the sign),
     * a '/' character, and the denominator.  Thus, if the ostream operator is
     * applied to Rational(2,-4), "-1/2" should be output.  If the ostream
     * operator is applied to Rational(-6,-4), "3/2" should be output.
     */
    friend std::ostream& operator<<(std::ostream& ostr, const Rational& r);

    /**
     * Appropriate istream (extraction) operator '>>'
     *
     * When inputting a Rational, read a numerator, a '/', and a denominator
     * and set the Rational input reference with the appropriate value specified
     * by the values read.
     *
     * Any amount of whitespace may separate the three components
     * (numerator, '/', and denominator).  And the numerator and/or denominator
     * may be negative when read.
     */
    friend std::istream& operator>>(std::istream& istr, Rational& r);

    // Add additional function prototypes here

private:
    // Private helper functions that you must implement

    /**
     * Divides the numerator and denominator by the gcd
     * thus leaving it in reduced form.
     */
    void reduce();

    // Note:  Static member functions are essentially global
    //        functions (take no implicit 'this' parameter and thus
    //        are not applied to any particular instance of the class).
    //        However, unlike a global scope function, a static member
    //        function is scoped within the class (e.g. Rational::gcd(2,4))
    //        and can access private data members of any class objects
    //        passed as input arguments.
    //        They are often used for helper functions for a class which
    //        do NOT require any access to the data members or that are
    //        not specific to a paritcular object instance.
    //        In fact, there can also be static data members.  For more
    //        information refer to:
    //    https://www.tutorialspoint.com/cplusplus/cpp_static_members.htm

    /**
     * Returns the greatest common divisor of a and b
     */
    static int gcd(int a, int b);

    /**
     * Returns the least common multiple of a and b
     */
    static int lcm(int a, int b);

    /**
         * If the numerator is 0, sets the denominator to 1
     * to provide a standard representation for 0.
         */
    void normalize0();

    // Private data members
    int n, d;

};


#endif
