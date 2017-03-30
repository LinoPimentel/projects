#include "stdafx.h"
#include "DataOperations.h"
#include "InputStructures.h"
#include <iostream>

using namespace Operations;

namespace InputStructures
{
	Results DataByName::compare(std::string a, std::string b)
	{
		int result = a.compare(b);
		if (result < 0)
			return B_IS_GREATER;
		else if (result > 0)
			return A_IS_GREATER;
		else
			return BOTH_EQUAL;
	}
	Results DataByName::destroy()
	{
		//if we have any news or mallocs, destroy them here
		return SUCCESS;
	}
	Results DataByName::edit(std::string a)
	{
		return SUCCESS;
	}
	Results DataByName::print(std::string a)
	{
		std::cout << a << "\n";
		return SUCCESS;
	}

}