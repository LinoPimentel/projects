#pragma once
#include "stdafx.h"
#include <string>
#include "DataOperations.h"

using namespace Operations;
namespace InputStructures
{
	class DataByName : public DataOperations<std::string>
	{
		public:
			Results compare(std::string a, std::string b);
			Results destroy();
			Results edit(std::string a);
			Results print(std::string a);
	};
}