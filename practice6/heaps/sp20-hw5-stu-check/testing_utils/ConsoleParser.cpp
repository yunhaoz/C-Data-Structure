#include "ConsoleParser.h"

#include <misc_utils.h>

kwsys::RegularExpression ConsoleParser::menuEqualsSep("===+");

kwsys::RegularExpression ConsoleParser::enterCommandLine("enter command:");
kwsys::RegularExpression ConsoleParser::invalidRequest("invalid request");
kwsys::RegularExpression ConsoleParser::invalidUsername("invalid username");


ConsoleParser::ConsoleProduct::ConsoleProduct(const std::string& name, const std::string& info, const std::string& stock, double price)
	: mName(name)
	, mInfo(info)
	, mStock(stock)
	, mPrice(price)
{
}

bool ConsoleParser::ConsoleProduct::operator<(const ConsoleParser::ConsoleProduct& rhs) const
{
	if (mStock == rhs.mStock)
	{
		if (mName == rhs.mName)
		{
			if (mInfo == rhs.mInfo)
			{
				return mPrice < rhs.mPrice && !Approximately(mPrice, rhs.mPrice);
			}
			else
			{
				return mInfo < rhs.mInfo;
			}
		}
		else
		{
			return mName < rhs.mName;
		}
	}
	else
	{
		return mStock < rhs.mStock;
	}
}

bool ConsoleParser::ConsoleProduct::operator==(const ConsoleParser::ConsoleProduct& rhs) const
{
	return mName == rhs.mName 
		&& mInfo == rhs.mInfo 
		&& mStock == rhs.mStock
		&& Approximately(mPrice, rhs.mPrice);
}

void ConsoleParser::Hit::AddProduct(ConsoleParser::ConsoleProduct&& p)
{
	emplace(p);
}

bool ConsoleParser::Hit::operator==(const Hit& rhs) const
{
	return UnOrderedComparison<ConsoleParser::ConsoleProduct>(*this, rhs);
}

void ConsoleParser::Cart::AddProduct(ConsoleParser::ConsoleProduct&& p)
{
	emplace_back(p);
}

bool ConsoleParser::Cart::operator==(const Cart& rhs) const
{
	return OrderedComparison<ConsoleParser::ConsoleProduct>(*this, rhs);
}

testing::AssertionResult ConsoleParser::CompareHit(const ConsoleParser &actual) const
{
	if(mHit.size() != actual.mHit.size())
	{
		return testing::AssertionFailure() << "Mismatch between numbers of search hit lists in actual and real outputs.";
	}

	for(size_t searchIndex = 0; searchIndex < mHit.size(); ++searchIndex)
	{
		if(mHit[searchIndex] != actual.mHit[searchIndex])
		{
			return testing::AssertionFailure() << "Mismatch between search result hits: Expected: " << setToString(mHit[searchIndex]) << ", actual: " << setToString(actual.mHit[searchIndex]);
		}
	}

	return testing::AssertionSuccess();
}

testing::AssertionResult ConsoleParser::CompareCart(const ConsoleParser &actual) const
{

	if(mCart.size() != actual.mCart.size())
	{
		return testing::AssertionFailure() << "Mismatch between numbers of search hit lists in actual and real outputs.";
	}

	for(size_t searchIndex = 0; searchIndex < mCart.size(); ++searchIndex)
	{
		if(mCart[searchIndex] != actual.mCart[searchIndex])
		{
			return testing::AssertionFailure() << "Mismatch between search result hits: Expected: " << vectorToString(mCart[searchIndex]) << ", actual: " << vectorToString(actual.mCart[searchIndex]);
		}
	}

	return testing::AssertionSuccess();
}

testing::AssertionResult ConsoleParser::ParseMenu(std::istream &stream)
{
	bool seenFirstEqualsSep;

	std::string content;
	while(true)
	{
		if(getline(stream, content))
		{
			if(menuEqualsSep.find(content))
			{
				if(!seenFirstEqualsSep)
				{
					seenFirstEqualsSep = true;
				}
				else
				{
					// second "================" divider found, done parsing menu
					return testing::AssertionSuccess();
				}

			}
		}
		else
		{
			return testing::AssertionFailure() << "End of file encountered while searching for end of menu";
		}
	}
}

testing::AssertionResult ConsoleParser::ParseInput(std::istream &stream)
{
	return ParseConsoleCommand(stream, "input");
}

testing::AssertionResult ConsoleParser::ParseInvalidRequest(std::istream &stream)
{
	std::string content;
	while(true)
	{
		if(getline(stream, content))
		{
			makeLowercase(content);
			if(invalidRequest.find(content))
			{
                return testing::AssertionSuccess();
			}
		}
		else
		{
			return testing::AssertionFailure() << "End of file encountered while searching for invalid request message";
		}
	}
}

testing::AssertionResult ConsoleParser::ParseInvalidUsername(std::istream &stream)
{
	std::string content;
	while(true)
	{
		if(getline(stream, content))
		{
			makeLowercase(content);
			if(invalidUsername.find(content))
			{
				return testing::AssertionSuccess();
			}
		}
		else
		{
            return testing::AssertionFailure() << "End of file encountered while searching for invalid username message";
		}
	}

}

testing::AssertionResult ConsoleParser::ParseHit(std::istream &stream)
{
	return ParseConsoleProduct<Hit>(stream, "Hit", mHit);
}

testing::AssertionResult ConsoleParser::ParseCart(std::istream &stream)
{
	return ParseConsoleProduct<Cart>(stream, "Item", mCart);
}

testing::AssertionResult ConsoleParser::ParseConsoleCommand(std::istream &stream, std::string const &sectionName)
{
	std::string content;
	while(true)
	{
		if(getline(stream, content))
		{
			makeLowercase(content);
			if(enterCommandLine.find(content))
			{
				// found prompt for next command
				return testing::AssertionSuccess();
			}
		}
		else
		{
			return testing::AssertionFailure() << "End of file encountered in " << sectionName << " section of output";
		}
	}
}

testing::AssertionResult ConsoleParser::parseSequence(std::istream& stream, std::vector<ConsoleParserCommand> sequence)
{
	for (unsigned int i = 0; i < sequence.size(); ++i)
	{
		switch(sequence[i])
		{
			case ConsoleParserCommand::Menu:
				SUB_ASSERT(ParseMenu(stream));
				break;
			case ConsoleParserCommand::Input:
				SUB_ASSERT(ParseInput(stream));
				break;
			case ConsoleParserCommand::Hit:
				SUB_ASSERT(ParseHit(stream));
				break;
			case ConsoleParserCommand::Cart:
				SUB_ASSERT(ParseCart(stream));
				break;
			case ConsoleParserCommand::InvalidRequest:
				SUB_ASSERT(ParseInvalidRequest(stream));
				break;
			case ConsoleParserCommand::InvalidUsername:
				SUB_ASSERT(ParseInvalidUsername(stream));
				break;
		}
	}
	return testing::AssertionSuccess();
}

std::ostream &operator<<(std::ostream &stream, const ConsoleParser::ConsoleProduct product)
{
	stream << "{name: " << product.mName << ", info: " << product.mInfo << ", stock: " << product.mStock << ", price: " << product.mPrice << "}";
	return stream;
}
