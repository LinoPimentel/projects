#pragma once
#include "stdafx.h"
#include <string>

template <typename T>
class StructureFunctions
{
	public:
	virtual int comparator(T *c1, T *c2) = 0;
	virtual bool edit(T *c1, T *c2) = 0;
	virtual bool deleteT(T *c1) = 0;
};

namespace Abordagem1
{
	struct Ano;
	struct Country;
	class StructureFunctionsAno : public StructureFunctions<struct Ano*>
	{
		int comparator(struct Ano **c1, struct Ano **c2);
		bool edit(struct Ano **c1, struct Ano **c2);
		bool deleteT(struct Ano **c1);
	};
	class StructureFunctionsCountry : public StructureFunctions<struct Country*>
	{
		int comparator(struct Country **c1, struct Country **c2);
		bool edit(struct Country **c1, struct Country **c2);
		bool deleteT(struct Country **c1);
	};
}

namespace Abordagem2
{
	struct Ano;
	struct Country;
	class StructureFunctionsAno : public StructureFunctions<struct Ano*>
	{
		int comparator(struct Ano **c1, struct Ano **c2);
		bool edit(struct Ano **c1, struct Ano **c2);
		bool deleteT(struct Ano **c1);
	};
	class StructureFunctionsCountry : public StructureFunctions<struct Country*>
	{
		int comparator(struct Country **c1, struct Country **c2);
		bool edit(struct Country **c1, struct Country **c2);
		bool deleteT(struct Country **c1);
	};
}

namespace Abordagem3
{
	struct Ano;
	struct Country;
	class StructureFunctionsAno : public StructureFunctions<struct Ano*>
	{
		int comparator(struct Ano **c1, struct Ano **c2);
		bool edit(struct Ano **c1, struct Ano **c2);
		bool deleteT(struct Ano **c1);
	};
	class StructureFunctionsCountry : public StructureFunctions<struct Country*>
	{
		int comparator(struct Country **c1, struct Country **c2);
		bool edit(struct Country **c1, struct Country **c2);
		bool deleteT(struct Country **c1);
	};
}