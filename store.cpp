
#include "store.h"
#include <cmath>

// Constructor initializes all store counters to zero.
Store::Store()
{
    num_customers_ = 0;
    num_products_ = 0;
    num_transactions_ = 0;
}

// Sets the number of customers if the value is between 0 and 100.
void Store::num_customers(int count)
{
    if (count >= 0 && count <= 100) num_customers_ = count;
}

// Returns the current number of customers.
int Store::num_customers() { return num_customers_; }

// Sets the number of products if the value is between 0 and 100.
void Store::num_products(int count)
{
    if (count >= 0 && count <= 100) num_products_ = count;
}

// Returns the current number of products.
int Store::num_products() { return num_products_; }

// Sets the number of transactions if the value is between 0 and 100.
void Store::num_transactions(int count)
{
    if (count >= 0 && count <= 100) num_transactions_ = count;
}

// Returns the current number of transactions.
int Store::num_transactions() { return num_transactions_; }

// Adds a new customer to the store if there is available space.
bool Store::add_customer(Person customer)
{
    // Do not add more than 100 customers.
    if (num_customers_ >= 100) return false;
    customers_[num_customers_] = customer;
    num_customers_++;
    return true;
}

// Updates an existing customer's information.
bool Store::update_customer(int index, Person customer)
{
    // Check that the customer index is valid.
    if (index < 0 || index >= num_customers_) return false;

    // Make sure the new customer has the same name.
    if (customers_[index].name() != customer.name()) return false;
    customers_[index] = customer;
    return true;
}

// Returns the customer at the given index.
Person Store::get_customer(int index)
{
    // Return a default Person if the index is invalid.
    if (index < 0 || index >= num_customers_) return Person();
    return customers_[index];
}

// Adds a new product to the store.
bool Store::add_product(product item)
{
    // Check that the product array is not full.
    if (num_products_ >= 100) return false;
    products_[num_products_] = item;
    num_products_++;
    return true;
}

// Returns the product at the given index.
product Store::get_product(int index)
{
    // Return a default product if the index is invalid.
    if (index < 0 || index >= num_products_) return product{};
    return products_[index];
}

// Adds a completed transaction to the store's records.
bool Store::add_transaction(transaction newTransaction)
{
    // Check that the transaction array is not full.
    if (num_transactions_ >= 100) return false;
    transactions_[num_transactions_] = newTransaction;
    num_transactions_++;
    return true;
}

// Returns the transaction at the given index.
transaction Store::get_transaction(int index)
{
    // Return a default transaction if the index is invalid.
    if (index < 0 || index >= num_transactions_) return transaction{};
    return transactions_[index];
}

// Searches for a customer in the store using their name.
int Store::find_customer(Person customer)
{
    // Loop through all customers until a matching name is found.
    for (int i = 0; i < num_customers_; i++)
    {
        if (customers_[i].name() == customer.name()) return i;
    }

    // Return -1 if the customer is not found.
    return -1;
}

// Attempts to purchase a product using the customer's credit card.
bool Store::make_purchase(Person customer, product item, int quantity)
{
    // Reject invalid quantities, prices, or a full transaction array.
    if (quantity <= 0 || !std::isfinite(item.price) || item.price <= 0 || num_transactions_ >= 100)
        return false;

    // Find the customer in the store.
    int index = find_customer(customer);

    // Reject the purchase if the customer does not exist.
    if (index == -1) return false;

    // Calculate the total purchase amount.
    double total = item.price * quantity;

    // Make sure the total is a valid finite number.
    if (!std::isfinite(total)) return false;

    // Get the customer's information and credit card.
    Person updated = customers_[index];
    credit_card card = updated.person_credit_card();

    // Attempt to charge the card and reject if unsuccessful.
    if (!card.make_purchase(total)) return false;

    // Update the customer's credit card with the new balance.
    updated.person_credit_card(card);

    // Create a receipt for the successful purchase.
    transaction receipt{};
    receipt.transaction_number = num_transactions_ + 1;
    receipt.customer = updated;
    receipt.purchased_item = item;
    receipt.quantity = quantity;

    // Save the transaction and update the customer in the store.
    if (!add_transaction(receipt)) return false;
    customers_[index] = updated;
    return true;
}

// Returns information about all customers in the store.
string Store::customer_info()
{
    string info;

    // Add each customer's information to one string.
    for (int i = 0; i < num_customers_; i++)
        info += customers_[i].person_info() + "\n";
    return info;
}

// Returns information about all products in the store.
string Store::product_info()
{
    string info;

    // Add each product's information to one string.
    for (int i = 0; i < num_products_; i++)
        info += products_[i].product_info() + "\n";
    return info;
}

// Returns information about all recorded transactions.
string Store::transaction_info()
{
    string info;

    // Add each transaction's information to one string.
    for (int i = 0; i < num_transactions_; i++)
        info += transactions_[i].transaction_info() + "\n";
    return info;
}

// Combines customer, product, and transaction information.
string Store::store_info()
{
    // Start with the list of customers.
    string info = "Customers:\n" + customer_info();

    // Add the list of products.
    info += "\nProducts:\n" + product_info();

    // Add the list of all transactions.
    info += "\nAll Transactions:\n" + transaction_info();
    return info;
}

// Returns the purchase history for a specific customer.
string Store::purchase_history(Person customer)
{
    string info;

    // Search all transactions for purchases made by this customer.
    for (int i = 0; i < num_transactions_; i++)
    {
        // Add the transaction if the customer's name matches.
        if (transactions_[i].customer.name() == customer.name())
            info += transactions_[i].transaction_info() + "\n";
    }

    // Return all matching transactions.
    return info;
}
