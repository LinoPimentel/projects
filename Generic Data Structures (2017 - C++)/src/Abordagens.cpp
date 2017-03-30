#include "stdafx.h"
#include <string>
#include "AVL.h"
#include "AVL.cpp"
#include "List.h"
#include "List.cpp"
#include "Binary.h"
#include "Binary.cpp"
#include "Abordagens.h"
#include "StructureFunctions.h"


namespace Abordagem1
{
	StructureFunctionsAno structureFunctionsAno;
	StructureFunctionsCountry structureFunctionsCountry;
	Ano::Ano(int ano, float percentage)
	{
		Ano::ano = ano;
		Ano::percentage = percentage;
	}

	Country::Country(std::string name, std::string abr)
	{
		Country::name = name;
		Country::abr = abr;
		Country::percentages = List<struct Ano *>(&structureFunctionsAno);
	}
	bool Country::insertAno(int ano, float percentage)
	{
		Country::percentages.insert(new Ano(ano, percentage));
		return true;
	}
	
	Operations::Operations()
	{
		Operations::tree = AVL<struct Country *>(&structureFunctionsCountry);
	}
	struct Country Operations::insertCountry(std::string name, std::string abr)
	{
		lastInserted = new Country(name, abr);
		Operations::tree.insert(lastInserted);
		return *lastInserted;
	}
	bool Operations::insertYear(std::string abr, int year, float percentage)
	{
		if (lastInserted == NULL && abr == "")
			return false;
		if (abr != "" && lastInserted->abr.compare(abr) != 0)
			lastInserted = tree.search(&struct Country("", abr))->data;
		lastInserted->insertAno(year, percentage);
		return true;
	}
	
	void Operations::printAll(List<std::function<bool(const Abordagem1::AnoPtr *data)>> *yearRestrictions, List<std::function<bool(const Abordagem1::CountryPtr *data)>> *countryRestrictions)
	{
		tree.runActionOnEveryNode(
			[=](const CountryPtr *data)
			{
				for (int i = 0; i < countryRestrictions->getSize(); i++)
					if (!countryRestrictions->getNodeAt(i)->data(data))
						return;
				std::cout << "\nName: " << (*data)->name << "\tAbr: " << (*data)->abr << "\n";
				(*data)->percentages.runActionOnEveryNode(
					[=](const AnoPtr *data)->void
					{
						for (int i = 0; i < yearRestrictions->getSize(); i++)
							if (!yearRestrictions->getNodeAt(i)->data(data))
								return;
						std::cout << "\Year: " << (*data)->ano << "\t" << "Percentage:" << (*data)->percentage << "\n";
					}
				);
			}
		);
	}

	void Operations::removeAll(List<std::function<bool(const Abordagem1::AnoPtr *data)>> *yearRestrictions, List<std::function<bool(const Abordagem1::CountryPtr *data)>> *countryRestrictions)
	{
		List<CountryPtr> countriesToDelete;
		tree.runActionOnEveryNode(
			[=,&countriesToDelete](const CountryPtr *data)
			{
				if(countryRestrictions->getSize() == 0 && yearRestrictions->getSize() == 0)
					countriesToDelete.insert(*data);
				for (int i = 0; i < countryRestrictions->getSize(); i++)
					if (countryRestrictions->getNodeAt(i)->data(data))
					{
						countriesToDelete.insert(*data);
						return;
					}
				List<AnoPtr> yearsToDelete;
				(*data)->percentages.runActionOnEveryNode(
					[=,&yearsToDelete](const AnoPtr *dataAno)->void
					{
						if(yearRestrictions->getSize() == 0 && countryRestrictions->getSize() == 0)
							yearsToDelete.insert(*dataAno);
						for (int i = 0; i < yearRestrictions->getSize(); i++)
							if (yearRestrictions->getNodeAt(i)->data(dataAno))
							{
								yearsToDelete.insert(*dataAno);
								return;
							}
					}
				);
				for (int i = 0; i < yearsToDelete.getSize(); i++)
					(*data)->percentages.deleteNode(yearsToDelete.getNodeAt(i)->data);
			}
		);
		std::string abr = "";
		if (lastInserted != NULL)
			abr = lastInserted->abr;
		for (int i = 0; i < countriesToDelete.getSize(); i++)
		{
			struct Country* c = countriesToDelete.getNodeAt(i)->data;
			if (c->abr.compare(abr) == 0)
				lastInserted = NULL;
			tree.deleteNode(c);
		}
	}

	void Operations::edit(std::string orig, std::string newAbr, std::string newName, int year, float newPercent)
	{
		if (lastInserted->abr.compare(orig) != 0)
		{
			AVL_Node<CountryPtr> *temp = tree.search(&struct Country("", orig));
			if (temp == NULL)
				return;
			Operations::lastInserted = temp->data;
		}
		if (newAbr != "" || newName != "")
			tree.editNode(lastInserted, &struct Country(newName, newAbr));
		if (year != 0 && newPercent >= 0 && newPercent <= 100)
			lastInserted->percentages.edit(&struct Ano(year, -1), &struct Ano(year, newPercent));
	}

	struct Country Operations::search(std::string abr)
	{
		// implement by abr	
		struct Country temp = struct Country("", abr);
		AVL_Node<CountryPtr> *node = tree.search(&temp);
		if(node == NULL)
			return struct Country("", "");
		Operations::lastInserted = node->data;
		return *Operations::lastInserted;
	}

	void Operations::finish()
	{
		tree.runActionOnEveryNode(
			[](const CountryPtr *data)
			{
				(*data)->percentages.destroyList();
			}
		);
		tree.destroyTree();
	}

	bool Operations::printLastInserted()
	{
		if (Operations::lastInserted == NULL)
		{
			std::cout << "No current selected country\n\n";
			return false;
		}
		std::cout << "Name: " << lastInserted->name << "\tAbr: " << lastInserted->abr << "\n";
		lastInserted->percentages.runActionOnEveryNode(
			[=](const AnoPtr *data)->void
			{
				std::cout << "\tAno: " << (*data)->ano << "\t" << "Percentage:" << (*data)->percentage << "\n";
			}
		);
		return true;
	}

	bool Operations::deleteNode(std::string abr)
	{
		struct Country temp = struct Country("", abr);
		AVL_Node<CountryPtr> *node = tree.search(&temp);
		if (node == NULL)
			return false;
		if (lastInserted->abr.compare(abr) == 0)
			Operations::lastInserted = NULL;
		node->data->percentages.destroyList();
		tree.deleteNode(node->data);
		return true;
	}

	bool Operations::deleteNode(std::string abr, int year)
	{
		struct Country temp = struct Country("", abr);
		AVL_Node<CountryPtr> *node = tree.search(&temp);
		if (node == NULL)
			return false;
		node->data->percentages.deleteNode(&struct Ano(year, 0));
		lastInserted = node->data;
		return true;
	}
}