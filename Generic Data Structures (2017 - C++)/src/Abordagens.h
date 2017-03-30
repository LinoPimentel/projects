#include "stdafx.h"
#include <string>
#include <functional>

#include "AVL.h"
#include "Binary.h"
#include "List.h"
#include "StructureFunctions.h"

namespace Abordagem1
{
	typedef struct Country* CountryPtr;
	typedef struct Ano* AnoPtr;
	struct Ano
	{
		int ano;
		float percentage;
		Ano(int ano, float percentage);
	};
	struct Country
	{
		std::string name;
		std::string abr;
		List<struct Ano *> percentages = 0;

		Country(std::string name, std::string abr);
		bool insertAno(int ano, float percent);
	};

	class Operations
	{
		private:
			AVL<struct Country *> tree = 0;
			struct Country *lastInserted;
		public:
			Operations();
			struct Country insertCountry(std::string name, std::string abr);
			bool insertYear(std::string abr, int year, float percentage);
			void printAll(List<std::function<bool(const Abordagem1::AnoPtr *data)>> *yearRestrictions, List<std::function<bool(const Abordagem1::CountryPtr *data)>> *countryRestrictions);
			void removeAll(List<std::function<bool(const Abordagem1::AnoPtr *data)>> *yearRestrictions, List<std::function<bool(const Abordagem1::CountryPtr *data)>> *countryRestrictions);
			void edit(std::string orig, std::string newAbr, std::string newName, int year, float newPercent);
			struct Country search(std::string abr);
			void finish();
			bool printLastInserted();
			bool deleteNode(std::string abr);
			bool deleteNode(std::string abr, int year);
	};
}

namespace Abordagem2
{
	typedef struct Country* CountryPtr;
	typedef struct Ano* AnoPtr;
	struct Ano
	{
		int ano;
		float percentage;
		Ano(int ano, float percentage);
	};
	struct Country
	{
		std::string name;
		std::string abr;
		Binary<struct Ano *> percentages = 0;

		Country(std::string name, std::string abr);
		bool insertAno(int ano, float percent);
	};

	class Operations
	{
	private:
		List<struct Country *> tree = 0;
		struct Country *lastInserted;
	public:
		Operations();
		struct Country insertCountry(std::string name, std::string abr);
		bool insertYear(std::string abr, int year, float percentage);
		void printAll(List<std::function<bool(const Abordagem2::AnoPtr *data)>> *yearRestrictions, List<std::function<bool(const Abordagem2::CountryPtr *data)>> *countryRestrictions);
		void removeAll(List<std::function<bool(const Abordagem2::AnoPtr *data)>> *yearRestrictions, List<std::function<bool(const Abordagem2::CountryPtr *data)>> *countryRestrictions);
		void edit(std::string orig, std::string newAbr, std::string newName, int year, float newPercent);
		struct Country search(std::string abr);
		void finish();
		bool printLastInserted();
		bool deleteNode(std::string abr);
		bool deleteNode(std::string abr, int year);
	};
}

namespace Abordagem3
{
	typedef struct Country* CountryPtr;
	typedef struct Ano* AnoPtr;
	struct Ano
	{
		int ano;
		float percentage;
		Ano(int ano, float percentage);
	};
	struct Country
	{
		std::string name;
		std::string abr;
		AVL<struct Ano *> percentages = 0;

		Country(std::string name, std::string abr);
		bool insertAno(int ano, float percent);
	};

	class Operations
	{
	private:
		Binary<struct Country *> tree = 0;
		struct Country *lastInserted;
	public:
		Operations();
		struct Country insertCountry(std::string name, std::string abr);
		bool insertYear(std::string abr, int year, float percentage);
		void printAll(List<std::function<bool(const Abordagem3::AnoPtr *data)>> *yearRestrictions, List<std::function<bool(const Abordagem3::CountryPtr *data)>> *countryRestrictions);
		void removeAll(List<std::function<bool(const Abordagem3::AnoPtr *data)>> *yearRestrictions, List<std::function<bool(const Abordagem3::CountryPtr *data)>> *countryRestrictions);
		void edit(std::string orig, std::string newAbr, std::string newName, int year, float newPercent);
		struct Country search(std::string abr);
		void finish();
		bool printLastInserted();
		bool deleteNode(std::string abr);
		bool deleteNode(std::string abr, int year);
	};
}
