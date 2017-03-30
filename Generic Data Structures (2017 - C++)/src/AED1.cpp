// AED1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <functional>
#include <stdlib.h>

#include "List.h"
#include "List.cpp"
#include "Abordagens.h"

#include <algorithm>
#include <random>

using namespace Abordagem1;

List<std::function<bool(const AnoPtr *data)>> yearRestrictions;
List<std::function<bool(const CountryPtr *data)>> nameRestrictions;
Operations a1;

void addNumberRestriction(int type)
{
	int option, value;
	float value1;

	std::cout << "1 - Greater than\n"
		<< "2 - Lesser than\n"
		<< "3 - Equal to\n"
		<< "0 - back\n";
	std::cin >> option;
	if (option == 0)
		return;
	std::cout << "Value: ";
	if (type == 1)
		std::cin >> value;
	else
		std::cin >> value1;
	if(type == 1)
		yearRestrictions.insert(
			[=](const AnoPtr *data)->bool
		{
			switch (option)
			{
				case 1: if ((*data)->ano > value) return true; else return false;
				case 2: if ((*data)->ano < value) return true; else return false;
				case 3: if ((*data)->ano == value) return true; else return false;
				default: return false;
			}
		}
	);
	else
		yearRestrictions.insert(
			[=](const AnoPtr *data)->bool
	{
		switch (option)
		{
			case 1: if ((*data)->percentage > value1) return true; else return false;
			case 2: if ((*data)->percentage < value1) return true; else return false;
			case 3: if ((*data)->percentage == value1) return true; else return false;
			default: return false;
		}
	}
	);
}

void addNameRestrictions(int type)
{
	std::string value;
	std::cout << "String to filter by: ";
	std::cin >> value;
	nameRestrictions.insert(
		[=](const CountryPtr *data)->bool
	{
		if(type == 1)
		{
			if ((*data)->name.find(value.c_str(), 0, value.size()) != std::string::npos)
				return true;
		}
		else
			if ((*data)->abr.find(value.c_str(), 0, value.size()) != std::string::npos)
				return true;
		return false;
	}
	);

}

void applyConditions()
{
	while (1)
	{
		system("cls");
		int option;
		std::cout << "1 - Add year restriction\n"
			<< "2 - Add percentage restriction\n"
			<< "3 - Add name restriction\n"
			<< "4 - Add abr restriction\n"
			<< "0 - Done\n";
		std::cin >> option;
		switch (option)
		{
			case 1: {
				addNumberRestriction(1);
			}break;
			case 2: {
				addNumberRestriction(2);
			}break;
			case 3: {
				addNameRestrictions(1);
			}break;
			case 4: {
				addNameRestrictions(2);
			}break;
			case 0: return;
		}
	}
}

void insertMenu()
{
	bool last;
	while(1)
	{
		int option;
		std::string name, abr = "";
		system("cls");
		last = a1.printLastInserted();
		std::cout << "1 - Insert new country\n";
		std::cout << "2 - Insert new year at <ABR> country\n";
		if (last)
			std::cout << "3 - Insert new year at selected country\n";
		std::cout << "0 - Back\n";
		std::cin >> option;
		switch (option)
		{
			case 1: {
				std::cout << "Name: ";
				std::cin >> name;
				std::cout << "Abr: ";
				std::cin >> abr;
				a1.insertCountry(name, abr);
			}break;
			case 2: {
				std::cout << "Country Abr: ";
				std::cin >> abr;
			}
			case 3: {
				int year;
				float percent;
				std::cout << "Year: ";
				std::cin >> year;
				std::cout << "Percent: ";
				std::cin >> percent;
				a1.insertYear(abr, year, percent);
			}break;
			case 0: return;
		}
	}
}

void searchMenu()
{
	while (1)
	{
		int option;
		std::string abr = "";
		system("cls");
		a1.printLastInserted();
		std::cout << "1 - Search 1\n";
		std::cout << "2 - Print all with conditions\n";
		std::cout << "0 - Back\n";
		std::cin >> option;
		switch (option)
		{
			case 1: {
				std::cout << "Abr: ";
				std::cin >> abr;
				a1.search(abr);
			}break;
			case 2: {
				applyConditions();
				a1.printAll(&yearRestrictions, &nameRestrictions);
				yearRestrictions.destroyList();
				nameRestrictions.destroyList();
				system("pause");
			}break;
			case 0: return;
		}
	}
}

void editMenu()
{
	std::string abr;
	std::string name = "", newAbr = "";
	int year = 0;
	float percent = -1;

	system("cls");
	std::cout << "Country to edit (abr): ";
	std::cin >> abr;

	if (a1.search(abr).abr.compare("") == 0)
		return;

	while (1)
	{
		int option;

		system("cls");

		a1.printLastInserted();

		std::cout << "1 - Edit name - Pending: " << name << "\n";
		std::cout << "2 - Edit abr - Pending: " << newAbr << "\n";
		std::cout << "3 - Edit year precentage - Pending: " << year << ";" << percent << "\n";
		std::cout << "4 - Apply\n";
		std::cout << "0 - Back\n";
		std::cin >> option;
		switch (option)
		{
			case 1: {
				std::cout << "Name: ";
				std::cin >> name;		
			}break;
			case 2: {
				std::cout << "Abr: ";
				std::cin >> newAbr;
			}break;
			case 3: {
				std::cout << "Year: ";
				std::cin >> year;
				std::cout << "New percentage: ";
				std::cin >> percent;
			}break;
			case 4: {
				a1.edit(abr, newAbr, name, year, percent);
				return;
			}break;
			case 0: return;
		}
		
	}
}

void removeMenu()
{
	while (1)
	{
		int option;
		std::string abr = "";
		system("cls");
		a1.printLastInserted();
		std::cout << "1 - Remove 1\n";
		std::cout << "2 - Remove year from 1\n";
		std::cout << "3 - Remove all with conditions\n";
		std::cout << "0 - Back\n";
		std::cin >> option;
		switch (option)
		{
			case 1: {
				std::cout << "Abr: ";
				std::cin >> abr;
				a1.deleteNode(abr);
			}break;
			case 2:
			{
				std::cout << "Abr: ";
				std::cin >> abr;
				std::cout << "Year to remove: ";
				int year;
				std::cin >> year;
				a1.deleteNode(abr, year);
			}break;
			case 3: {
				applyConditions();
				a1.removeAll(&yearRestrictions, &nameRestrictions);
				yearRestrictions.destroyList();
				nameRestrictions.destroyList();
				system("pause");
			}break;
			case 0: return;
		}
	}
}

void userInterface()
{
	while (1)
	{
		int option;
		bool lastInsert;
		system("cls");
		lastInsert = a1.printLastInserted();
		std::cout << "1 - Insert\n"
				  << "2 - Search\n"
				  << "3 - Edit\n"
				  << "4 - Remove\n"
				  << "0 - Exit\n";
		std::cin >> option;
		switch (option)
		{
			case 1: {
				insertMenu();
			}break;
			case 2: {
				searchMenu();
			}break;
			case 3: {
				editMenu();
			}break;
			case 4: {
				removeMenu();
			}break;
			case 0:{
				a1.finish();
				exit(0);
			}
		}
	}

}

void read_file(int max, std::string file);

int main()
{
	read_file(264,"dados.csv");
	userInterface();

	return 0;
}

void read_file(int max, std::string files)
{
	std::ifstream file(files);
	std::string line;

	if (!file.is_open())
	{
		std::cout << "Unable to open file\n";
		exit(-1);
	}

	//we can ignore the first line
	getline(file, line);
	int countries = 0;
	while (getline(file, line) && countries < max) //for each country...
	{
		int year = 0; 
		int separator_pos = -1;
		int first = 1;
		float percent;
		std::string name, abr;

		while ((separator_pos = line.find(';', separator_pos + 1)) != -1) //for each year...
		{
			int next_separator_pos = line.find(';', separator_pos + 1);

			if (first)			
				name = line.substr(1, separator_pos - 2);			

			if (next_separator_pos != -1 && next_separator_pos != separator_pos + 1)
			{
				std::string part = line.substr(separator_pos + 2, next_separator_pos - separator_pos - 3);
				if (first)
				{
					first = 0;
					abr = part; //second is abbr
					a1.insertCountry(name,abr);
					continue;
				}	
				else
					percent = strtof(part.c_str(), 0); // next ones are values
				a1.insertYear("", 1960 + year, percent);
			}
			year++;
		}
		countries++;
	}
	file.close();
}