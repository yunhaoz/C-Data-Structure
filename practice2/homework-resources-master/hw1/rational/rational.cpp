#include <stdexcept>
#include "rational.h"
#include <sstream>

Rational::Rational()
{
    n = 0;
    d = 1;
}
Rational::Rational(int num, int denom)
{
    if (denom == 0) throw std::invalid_argument("Can't have denom = 0");
    // Continue your implementation below.
    // You should likely call this->reduce() and this->normalize0()
	
	this->normalize0();
	this->reduce();
	this->changSign();
}
	
void Rational::normalize0()
{
    if (n == 0 && d != 1) d = 1;
}

/*it is a comment*/
std::ostream& operator<<(std::ostream& ostr, const Rational& r)
{
	ostr << r.n << "/" << r.d;
	return ostr;
}

std::istream& operator>>(std::istream& istr, Rational& r)
{
	int temp1;
	char tempc;
	int temp2;
	istr >> temp1 >> tempc >> temp2;
	r = Rational(temp1, temp2);
	return istr;
}

void Rational::reduce()
{
	int temp = gcd(n, d);
	n = n / temp;
	d = d / temp;
}

int Rational::gcd(int a, int b)
{
	if (b == 0)
		return a;
	return gcd(b, a % b);
}

int Rational::lcm(int a, int b)
{
	int temp = gcd(a, b);
	return a * b / temp;
}

void Rational::changSign()
{
	if (d < 0)
	{
		n *= -1;
		d *= -1;
	}
}

Rational Rational::operator+(const Rational& rhs) const
{
	int temp1 = n + rhs.n;
	int temp2 = d + rhs.d;
	Rational temp(temp1, temp2);
	return temp;
}

Rational Rational::operator+(const int& rhs) const
{
	int temp1 = n + rhs;
	int temp2 = d + rhs;
	Rational temp(temp1, temp2);
	return temp;
}

Rational operator+(int lhs, const Rational& rhs)
{
	int temp1 = lhs + rhs.n;
	int temp2 = lhs + rhs.d;
	Rational temp(temp1, temp2);
	return temp;
}

Rational Rational::operator*(const Rational& rhs) const
{
	int temp1 = n * rhs.n;
	int temp2 = d * rhs.d;
	Rational temp(temp1, temp2);
	return temp;
}

Rational Rational::operator*(const int& rhs) const
{
	int temp1 = n * rhs;
	int temp2 = d * rhs;
	Rational temp(temp1, temp2);
	return temp;
}

Rational operator*(int lhs, const Rational& rhs)
{
	int temp1 = lhs * rhs.n;
	int temp2 = lhs * rhs.d;
	Rational temp(temp1, temp2);
	return temp;
}

Rational Rational::operator^(const int& rhs) const
{
	int temp1 = pow(n, rhs);
	int temp2 = pow(d, rhs);
	Rational temp(temp1, temp2);
	return temp;
}

bool Rational::operator==(const Rational& rhs) const
{
	return(n == rhs.n && d == rhs.d);
}

bool Rational::operator!=(const Rational& rhs) const
{
	return(n != rhs.n || d != rhs.d);
}

bool Rational::operator<(const Rational& rhs) const
{
	return(n < rhs.n && d < rhs.d);
}

Rational& Rational::operator+=(const Rational& rhs)
{
	n += rhs.n;
	d += rhs.d;
	return *this;
}

Rational& Rational::operator*=(const Rational& rhs)
{
	n *= rhs.n;
	d *= rhs.d;
	return *this;
}
