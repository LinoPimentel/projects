#include "stdafx.h"
#include "StructureFunctions.h"
#include "Abordagens.h"
#include <string>

namespace Abordagem1
{
	int StructureFunctionsAno::comparator(struct Ano **c1, struct Ano **c2)
	{
		if (c1 == NULL || c2 == NULL)
			return 0;
		if ((*c1)->ano < (*c2)->ano)
			return 1;
		else if ((*c1)->ano == (*c2)->ano)
			return 0;
		else
			return -1;
	}

	bool StructureFunctionsAno::edit(struct Ano **c1, struct Ano **c2)
	{
		if (c1 == NULL || c2 == NULL)
			return false;
		(*c1)->percentage = (*c2)->percentage;
		return true;
	}

	bool StructureFunctionsAno::deleteT(struct Ano **c1)
	{
		free(*c1);
		return true;
	}

	int StructureFunctionsCountry::comparator(struct Country **c1, struct Country **c2)
	{
		if (c1 == NULL || c2 == NULL)
			return 0;
		int resAbr = (*c1)->abr.compare((*c2)->abr);
		int resName = (*c1)->name.compare((*c2)->name);
		if (resAbr == 0 || resName == 0)
			return 0;
		else
			return resAbr * -1;
	}

	bool StructureFunctionsCountry::edit(struct Country **c1, struct Country **c2)
	{
		if (c1 == NULL || c2 == NULL)
			return false;
		if((*c2)->abr != "")
			(*c1)->abr = (*c2)->abr;
		if((*c2)->name != "")
			(*c1)->name = (*c2)->name;
		return true;
	}

	bool StructureFunctionsCountry::deleteT(struct Country **c1)
	{
		free(*c1);
		return true;
	}
}

namespace Abordagem2
{
	int StructureFunctionsAno::comparator(struct Ano **c1, struct Ano **c2)
	{
		if (c1 == NULL || c2 == NULL)
			return 0;
		if ((*c1)->ano < (*c2)->ano)
			return 1;
		else if ((*c1)->ano == (*c2)->ano)
			return 0;
		else
			return -1;
	}

	bool StructureFunctionsAno::edit(struct Ano **c1, struct Ano **c2)
	{
		if (c1 == NULL || c2 == NULL)
			return false;
		(*c1)->percentage = (*c2)->percentage;
		return true;
	}

	bool StructureFunctionsAno::deleteT(struct Ano **c1)
	{
		free(*c1);
		return true;
	}

	int StructureFunctionsCountry::comparator(struct Country **c1, struct Country **c2)
	{
		if (c1 == NULL || c2 == NULL)
			return 0;
		int resAbr = (*c1)->abr.compare((*c2)->abr);
		int resName = (*c1)->name.compare((*c2)->name);
		if (resAbr == 0 || resName == 0)
			return 0;
		else
			return resAbr * -1;
	}

	bool StructureFunctionsCountry::edit(struct Country **c1, struct Country **c2)
	{
		if (c1 == NULL || c2 == NULL)
			return false;
		if ((*c2)->abr != "")
			(*c1)->abr = (*c2)->abr;
		if ((*c2)->name != "")
			(*c1)->name = (*c2)->name;
		return true;
	}

	bool StructureFunctionsCountry::deleteT(struct Country **c1)
	{
		free(*c1);
		return true;
	}
}

namespace Abordagem3
{
	int StructureFunctionsAno::comparator(struct Ano **c1, struct Ano **c2)
	{
		if (c1 == NULL || c2 == NULL)
			return 0;
		if ((*c1)->ano < (*c2)->ano)
			return 1;
		else if ((*c1)->ano == (*c2)->ano)
			return 0;
		else
			return -1;
	}

	bool StructureFunctionsAno::edit(struct Ano **c1, struct Ano **c2)
	{
		if (c1 == NULL || c2 == NULL)
			return false;
		(*c1)->percentage = (*c2)->percentage;
		return true;
	}

	bool StructureFunctionsAno::deleteT(struct Ano **c1)
	{
		free(*c1);
		return true;
	}

	int StructureFunctionsCountry::comparator(struct Country **c1, struct Country **c2)
	{
		if (c1 == NULL || c2 == NULL)
			return 0;
		int resAbr = (*c1)->abr.compare((*c2)->abr);
		int resName = (*c1)->name.compare((*c2)->name);
		if (resAbr == 0 || resName == 0)
			return 0;
		else
			return resAbr * -1;
	}

	bool StructureFunctionsCountry::edit(struct Country **c1, struct Country **c2)
	{
		if (c1 == NULL || c2 == NULL)
			return false;
		if ((*c2)->abr != "")
			(*c1)->abr = (*c2)->abr;
		if ((*c2)->name != "")
			(*c1)->name = (*c2)->name;
		return true;
	}

	bool StructureFunctionsCountry::deleteT(struct Country **c1)
	{
		free(*c1);
		return true;
	}
}