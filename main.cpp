#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include "address.h"
#include "credit_card.h"
#include "person.h"
#include "product.h"
#include "store.h"
#include "transaction.h"

using namespace std;

void discardLine(ifstream &file)
{
	file.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main()
{
	product products[100];
	Person people[100];
	credit_card cards[100];
	string cardOwners[100];
	int productCount = 0, peopleCount = 0, cardCount = 0;

	ifstream productFile("products.txt");
	if (!productFile || !(productFile >> productCount) ||
		productCount < 0 || productCount > 100)
	{
		cerr << "Error opening products.txt or invalid product count.\n";
		return 1;
	}
	discardLine(productFile);
	for (int i = 0; i < productCount; ++i)
	{
		if (!getline(productFile, products[i].name) ||
			!getline(productFile, products[i].description) ||
			!(productFile >> products[i].price))
		{
			cerr << "Invalid product record " << i + 1 << "\n";
			return 1;
		}
		discardLine(productFile);
	}
	productFile.close();

	ifstream cardFile("credit_card.txt");
	if (!cardFile || !(cardFile >> cardCount) ||
		cardCount < 0 || cardCount > 100)
	{
		cerr << "Error opening credit_card.txt or invalid card count.\n";
		return 1;
	}
	discardLine(cardFile);
	for (int i = 0; i < cardCount; ++i)
	{
		if (!getline(cardFile, cardOwners[i]) ||
			!(cardFile >> cards[i].cc_number >> cards[i].limit >> cards[i].current_balance))
		{
			cerr << "Invalid credit card record " << i + 1 << "\n";
			return 1;
		}
		discardLine(cardFile);
	}
	cardFile.close();

	ifstream peopleFile("people.txt");
	if (!peopleFile || !(peopleFile >> peopleCount) ||
		peopleCount < 0 || peopleCount > 100)
	{
		cerr << "Error opening people.txt or invalid person count.\n";
		return 1;
	}
	discardLine(peopleFile);
	for (int i = 0; i < peopleCount; ++i)
	{
		string name, phone;
		address home{};
		if (!getline(peopleFile, name) || !getline(peopleFile, phone) ||
			!getline(peopleFile, home.address_line1) ||
			!getline(peopleFile, home.address_line2) ||
			!getline(peopleFile, home.city) ||
			!getline(peopleFile, home.state) ||
			!getline(peopleFile, home.zip))
		{
			cerr << "Invalid person record " << i + 1 << "\n";
			return 1;
		}

		int cardIndex = -1;
		for (int j = 0; j < cardCount; ++j)
		{
			if (cardOwners[j] == name)
			{
				cardIndex = j;
				break;
			}
		}
		if (cardIndex == -1)
		{
			cerr << "No credit card found for: " << name << "\n";
			return 1;
		}
		people[i] = Person(name, phone, home, cards[cardIndex]);
	}
	peopleFile.close();

	Store store1;
	for (int i = 0; i < peopleCount; ++i)
	{
		if (!store1.add_customer(people[i]))
			return 1;
	}
	for (int i = 0; i < productCount; ++i)
	{
		if (!store1.add_product(products[i]))
			return 1;
	}

	cout << "Store info:\n\n"
		 << store1.store_info() << "\n";

	if (store1.num_customers() == 0 || store1.num_products() == 0)
	{
		cerr << "Cannot simulate purchases in an empty store.\n";
		return 1;
	}
	srand(static_cast<unsigned int>(time(nullptr)));
	const int attempts = 11 + rand() % 89;
	int approvedCount = 0;
	int declinedCount = 0;
	cout << "\nPurchase attempts: " << attempts << "\n";
	cout << fixed << setprecision(2);

	for (int attempt = 0; attempt < attempts; ++attempt)
	{
		int customerIndex = rand() % store1.num_customers();
		int productIndex = rand() % store1.num_products();
		int quantity = 1 + rand() % 10;
		Person customer = store1.get_customer(customerIndex);
		product item = store1.get_product(productIndex);
		double totalCost = item.price * quantity;

		cout << "\nAttempt #" << attempt + 1 << "\n";
		cout << "Customer: " << customer.name() << "\n";
		cout << "Product: " << item.name << "\n";
		cout << "Quantity: " << quantity << "\n";
		cout << "Attempted total: $" << totalCost << "\n";

		if (store1.make_purchase(customer, item, quantity))
		{
			++approvedCount;
			cout << "Successful transaction!\nTransaction Details:\n";
			transaction receipt = store1.get_transaction(store1.num_transactions() - 1);
			cout << receipt.transaction_info() << "\n";
		}
		else
		{
			++declinedCount;
			cout << "Unsuccessful transaction!\nTransaction Details:\n";
			cout << "Person:\n"
				 << customer.person_info();
			cout << "Product:\n"
				 << item.product_info();
			cout << "Purchase Quantity: " << quantity << "\n";
			cout << "Purchase declined (insufficient credit or invalid attempt).\n";
		}
	}

	cout << "\nAttempt summary: " << approvedCount << " approved, "
		 << declinedCount << " declined, " << attempts << " total.\n\n";

	cout << "Store info:\n\n"
		 << store1.store_info() << "\n";
	return 0;
}
