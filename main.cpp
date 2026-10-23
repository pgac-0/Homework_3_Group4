
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

// Discards the remaining characters on the current line
// after reading data from a file.
void discardLine(ifstream &file)
{
	file.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main()
{
	// Arrays used to store products, customers, and credit cards.
	product products[100];
	Person people[100];
	credit_card cards[100];
	string cardOwners[100];

	// Keep track of how many records are loaded from each file.
	int productCount = 0, peopleCount = 0, cardCount = 0;

	// Open the product file and validate the number of products.
	ifstream productFile("products.txt");
	if (!productFile || !(productFile >> productCount) ||
		productCount < 0 || productCount > 100)
	{
		cerr << "Error opening products.txt or invalid product count.\n";
		return 1;
	}

	// Remove the newline after reading the product count.
	discardLine(productFile);

	// Read each product's name, description, and price.
	for (int i = 0; i < productCount; ++i)
	{
		if (!getline(productFile, products[i].name) ||
			!getline(productFile, products[i].description) ||
			!(productFile >> products[i].price))
		{
			cerr << "Invalid product record " << i + 1 << "\n";
			return 1;
		}

		// Clear the remaining characters before the next product.
		discardLine(productFile);
	}
	productFile.close();

	// Open the credit card file and validate the card count.
	ifstream cardFile("credit_card.txt");
	if (!cardFile || !(cardFile >> cardCount) ||
		cardCount < 0 || cardCount > 100)
	{
		cerr << "Error opening credit_card.txt or invalid card count.\n";
		return 1;
	}

	discardLine(cardFile);

	// Read each card owner's name, card number, credit limit,
	// and current balance.
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

	// Open the people file and validate the number of customers.
	ifstream peopleFile("people.txt");
	if (!peopleFile || !(peopleFile >> peopleCount) ||
		peopleCount < 0 || peopleCount > 100)
	{
		cerr << "Error opening people.txt or invalid person count.\n";
		return 1;
	}

	discardLine(peopleFile);

	// Read each person's personal information and address.
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

		// Search for the credit card that belongs to this person.
		int cardIndex = -1;
		for (int j = 0; j < cardCount; ++j)
		{
			if (cardOwners[j] == name)
			{
				cardIndex = j;
				break;
			}
		}

		// Stop the program if the person has no matching credit card.
		if (cardIndex == -1)
		{
			cerr << "No credit card found for: " << name << "\n";
			return 1;
		}

		// Create a Person object using the person's information
		// and their matching credit card.
		people[i] = Person(name, phone, home, cards[cardIndex]);
	}
	peopleFile.close();

	// Create the store object.
	Store store1;

	// Add all customers to the store.
	for (int i = 0; i < peopleCount; ++i)
	{
		if (!store1.add_customer(people[i]))
			return 1;
	}

	// Add all products to the store.
	for (int i = 0; i < productCount; ++i)
	{
		if (!store1.add_product(products[i]))
			return 1;
	}

	// Display the store information before any purchases.
	cout << "Store info:\n\n"
		 << store1.store_info() << "\n";

	// Make sure the store has customers and products
	// before attempting any transactions.
	if (store1.num_customers() == 0 || store1.num_products() == 0)
	{
		cerr << "Cannot simulate purchases in an empty store.\n";
		return 1;
	}

	// Seed the random number generator using the current time.
	srand(static_cast<unsigned int>(time(nullptr)));


	/* Controls for how many purchase attempts are made */
	const int maxPurchases = 13; // the maximum number of purchases that should be made (inclusive)
	const int minPurchases = 11; // the minimum number of purchases that should be made (inclusive)

	// (Logic explanation)
	// 'rand()' generates an arbitrary number between 0 and RAND_MAX (not sure exactly what number
	//	that is, but it's big.)
	// To get a random number between 0 and x (inclusive), use 'rand() % (x+1)'


	// Generate a random number of purchase attempts from 11 to 99.
	//const int attempts = 11 + rand() % 89;
	const int attempts = minPurchases + (rand() % ((maxPurchases - minPurchases) + 1));

	// Counters for successful and unsuccessful purchases.
	int approvedCount = 0;
	int declinedCount = 0;

	cout << "\nPurchase attempts: " << attempts << "\n";

	// Display monetary amounts with two decimal places.
	cout << fixed << setprecision(2);

	// Simulate the randomly generated number of purchase attempts.
	for (int attempt = 0; attempt < attempts; ++attempt)
	{
		// Randomly select a customer and product from the store.
		int customerIndex = rand() % store1.num_customers();
		int productIndex = rand() % store1.num_products();

		// Randomly select a quantity between 1 and 10.
		int quantity = 1 + rand() % 10;

		// Retrieve the selected customer and product.
		Person customer = store1.get_customer(customerIndex);
		product item = store1.get_product(productIndex);

		/* The totalCost variable was only included in the 'Attempt information' text
			(see below), and the code doesn't like that totalCost is declared but
			not used.*/
		/*
		// Calculate the total purchase cost.
		double totalCost = item.price * quantity;
		*/

		/* The 'Attempt information' directly below this is somewhat redundant,
			it's not included in the example formatting, and it makes the output
			harder to read.
			It may have been useful for testing, but for the final version of
			the code, don't include it in the output. */
			
			// The 'transaction number' of successful transactions is the first thing
			// included in transaction details; nothing else needs to be done there

		/*
		// Display the information for the current purchase attempt.
		cout << "\nAttempt #" << attempt + 1 << "\n";
		cout << "Customer: " << customer.name() << "\n";
		cout << "Product: " << item.name << "\n";
		cout << "Quantity: " << quantity << "\n";
		cout << "Attempted total: $" << totalCost << "\n";
		*/

		// Attempt the purchase and check whether it was approved.
		if (store1.make_purchase(customer, item, quantity))
		{
			// Increase the successful transaction counter.
			++approvedCount;

			cout << "Successful transaction!\nTransaction Details:\n";

			// Retrieve the most recent transaction from the store.
			transaction receipt = store1.get_transaction(store1.num_transactions() - 1);

			// Display the details of the approved transaction.
			cout << receipt.transaction_info() << "\n";
		}
		else
		{
			// Increase the declined transaction counter.
			++declinedCount;

			cout << "Unsuccessful transaction!\nTransaction Details:\n";

			// Display the customer's information.
			cout << "Person:\n"
				 << customer.person_info();

			// Display the product information.
			cout << "Product:\n"
				 << item.product_info();

			// Display the attempted quantity and decline message.
			cout << "Purchase Quantity: " << quantity << "\n";
			cout << "Purchase declined (insufficient credit or invalid attempt).\n";
		}
	}

	// Display the total number of approved and declined purchases.
	cout << "\nAttempt summary: " << approvedCount << " approved, "
		 << declinedCount << " declined, " << attempts << " total.\n\n";

	// Display the updated store information after all transactions.
	cout << "Store info:\n\n"
		 << store1.store_info() << "\n";

	// End the program successfully.
	return 0;
}
