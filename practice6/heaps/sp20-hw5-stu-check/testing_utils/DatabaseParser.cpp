#include "DatabaseParser.h"
#include <sstream>
#include <cassert>

#include <misc_utils.h>

#include <gtest/gtest.h>

DatabaseParser::DatabaseProduct::DatabaseProduct(const std::string& type, 
			const std::string& name, 
			const std::string& info1, 
			const std::string& info2, 
			double price, 
			int stock)
	: mType(type)
	, mName(name)
	, mInfo1(info1)
	, mInfo2(info2)
	, mPrice(price)
	, mStock(stock)
{
}

bool DatabaseParser::DatabaseProduct::operator<(const DatabaseParser::DatabaseProduct& rhs) const
{
	if (mStock == rhs.mStock)
	{
		if (mType == rhs.mType)
		{
			if (mName == rhs.mName)
			{
				if (mInfo1 == rhs.mInfo1)
				{
					if (mInfo2 == rhs.mInfo2)
					{
						return mPrice < rhs.mPrice && !Approximately(mPrice, rhs.mPrice);
					}
					else
					{
						return mInfo2 < rhs.mInfo2;
					}
				}
				else
				{
					return mInfo1 < rhs.mInfo1;
				}
			}
			else
			{
				return mName < rhs.mName;
			}
		}
		else
		{
			return mType < rhs.mType;
		}
	}
	else
	{
		return mStock < rhs.mStock;
	}
}

bool DatabaseParser::DatabaseProduct::operator==(const DatabaseParser::DatabaseProduct& rhs) const
{
	return mType == rhs.mType
		&& mName == rhs.mName
		&& mInfo1 == rhs.mInfo1
		&& mInfo2 == rhs.mInfo2
		&& mStock == rhs.mStock
		&& Approximately(mPrice, rhs.mPrice);
}

DatabaseParser::DatabaseUser::DatabaseUser(const std::string& name, double credit, const int type)
	: mName(name)
	, mCredit(credit)
	, mType(type)
{
}

bool DatabaseParser::DatabaseUser::operator<(const DatabaseParser::DatabaseUser& rhs) const
{
	if (mType == rhs.mType)
	{
		if (mName == rhs.mName)
		{
			return mCredit < rhs.mCredit && !Approximately(mCredit, rhs.mCredit);
		}
		else
		{
			return mName < rhs.mName;
		}
	}
	else
	{
		return mType < rhs.mType;
	}
}

bool DatabaseParser::DatabaseUser::operator==(const DatabaseParser::DatabaseUser& rhs) const
{
	return mType == rhs.mType
		&& mName == rhs.mName
		&& Approximately(mCredit, rhs.mCredit);
}

testing::AssertionResult DatabaseParser::CompareProduct(const DatabaseParser &actual) const
{
	if(mProduct != actual.mProduct)
	{
		return testing::AssertionFailure() << "Product list does not match!  Expected: " << setToString(mProduct) << ", actual: " << setToString(actual.mProduct);
	}

	return testing::AssertionSuccess();
}

testing::AssertionResult DatabaseParser::CompareUser(const DatabaseParser &actual) const
{
	if(mUsers != actual.mUsers)
	{
		return testing::AssertionFailure() << "User list does not match!  Expected: " << setToString(mUsers) << ", actual: " << setToString(actual.mUsers);
	}

	return testing::AssertionSuccess();
}

testing::AssertionResult DatabaseParser::ParseProduct(std::istream &stream)
{
	bool start = false, end = false;
	std::string content;
	
	while (!end && getline(stream, content))
	{
		RemoveWhiteSpace(trim(content));
		if(content.empty())
		{
			return testing::AssertionFailure() << "Empty database file!";
		}

		if (content == "<products>")
		{
			start = true;
		}
		else if (content == "</products>") 
		{
			end = true;
		}
		else if (!start)
		{
			// keep looking
		}
		else
		{
			// parse 5 more lines of product info
			std::string type, name, info1, info2;
			double price;
			int stock;

			type = content;
			getline(stream, name);
			stream >> price >> stock;
			getline(stream, content); // consume empty line
			getline(stream, info1);
			getline(stream, info2);

			if (stream.fail())
			{
				return testing::AssertionFailure() << "Incomplete product in products section!";
			}
			else
			{
				// product parsed successfully
				RemoveWhiteSpace(trim(name));
				RemoveWhiteSpace(trim(info1));
				RemoveWhiteSpace(trim(info2));
				assert(!name.empty() && !info1.empty() && !info2.empty());
				mProduct.emplace(DatabaseProduct(type, name, info1, info2, price, stock));
			}
		}
	}

	if(!start)
	{
		return testing::AssertionFailure() << "No products section found in database";
	}

	if(!end)
	{
		return testing::AssertionFailure() << "Closing tag of products section was not found!";
	}

	return testing::AssertionSuccess();
}

testing::AssertionResult DatabaseParser::ParseUser(std::istream &stream)
{
	bool start = false, end = false;
	std::string content;

	while (!end && getline(stream, content))
	{
		RemoveWhiteSpace(trim(content));
		if(content.empty())
		{
			return testing::AssertionFailure() << "Empty database file!";
		}

		if (content == "<users>")
		{
			start = true;
		}
		else if (content == "</users>") 
		{
			end = true;
		}
		else if (!start)
		{
			// keep looking
		}
		else
		{
			// extract user info
			std::stringstream ss(content);
			std::string name;
			double credit;
			int type;
			ss >> name >> credit >> type;

			if (stream.fail())
			{
				return testing::AssertionFailure() << "Incomplete user in users section!";
			}
			else
			{
				// product parsed successfully
				mUsers.emplace(DatabaseUser(name, credit, type));
			}
		}
	}

	if(!start)
	{
		return testing::AssertionFailure() << "No users section found in database";
	}

	if(!end)
	{
		return testing::AssertionFailure() << "Closing tag of users section was not found!";
	}

	return testing::AssertionSuccess();
}


std::ostream & operator<<(std::ostream & stream, const DatabaseParser::DatabaseProduct databaseProduct)
{
	stream << "{category: " << databaseProduct.mType << ", name: " << databaseProduct.mName << ", stock: " << databaseProduct.mStock
	<< ", extra 1: " << databaseProduct.mInfo1 << ", extra 2: " << databaseProduct.mInfo2 << ", price: " << databaseProduct.mPrice << "}";
	return stream;
}

std::ostream &operator<<(std::ostream &stream, const DatabaseParser::DatabaseUser databaseUser)
{
	stream << "{name: " << databaseUser.mName << ", credit: " << databaseUser.mCredit << ", type: " << databaseUser.mType << "}";
	return stream;
}
