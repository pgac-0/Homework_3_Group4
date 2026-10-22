#include "store.h"
#include <cmath>

Store::Store()
{
    num_customers_ = 0;
    num_products_ = 0;
    num_transactions_ = 0;
}

void Store::num_customers(int count)
{
    if (count >= 0 && count <= 100) num_customers_ = count;
}
int Store::num_customers() { return num_customers_; }

void Store::num_products(int count)
{
    if (count >= 0 && count <= 100) num_products_ = count;
}
int Store::num_products() { return num_products_; }

void Store::num_transactions(int count)
{
    if (count >= 0 && count <= 100) num_transactions_ = count;
}
int Store::num_transactions() { return num_transactions_; }

bool Store::add_customer(Person customer)
{
    if (num_customers_ >= 100) return false;
    customers_[num_customers_] = customer;
    num_customers_++;
    return true;
}

bool Store::update_customer(int index, Person customer)
{
    if (index < 0 || index >= num_customers_) return false;
    if (customers_[index].name() != customer.name()) return false;
    customers_[index] = customer;
    return true;
}

Person Store::get_customer(int index)
{
    if (index < 0 || index >= num_customers_) return Person();
    return customers_[index];
}

bool Store::add_product(product item)
{
    if (num_products_ >= 100) return false;
    products_[num_products_] = item;
    num_products_++;
    return true;
}

product Store::get_product(int index)
{
    if (index < 0 || index >= num_products_) return product{};
    return products_[index];
}

bool Store::add_transaction(transaction newTransaction)
{
    if (num_transactions_ >= 100) return false;
    transactions_[num_transactions_] = newTransaction;
    num_transactions_++;
    return true;
}

transaction Store::get_transaction(int index)
{
    if (index < 0 || index >= num_transactions_) return transaction{};
    return transactions_[index];
}

int Store::find_customer(Person customer)
{
    for (int i = 0; i < num_customers_; i++)
    {
        if (customers_[i].name() == customer.name()) return i;
    }
    return -1;
}

bool Store::make_purchase(Person customer, product item, int quantity)
{
    if (quantity <= 0 || !std::isfinite(item.price) || item.price <= 0 || num_transactions_ >= 100)
        return false;

    int index = find_customer(customer);
    if (index == -1) return false;

    double total = item.price * quantity;
    if (!std::isfinite(total)) return false;

    Person updated = customers_[index];
    credit_card card = updated.person_credit_card();
    if (!card.make_purchase(total)) return false;

    updated.person_credit_card(card);

    transaction receipt{};
    receipt.transaction_number = num_transactions_ + 1;
    receipt.customer = updated;
    receipt.purchased_item = item;
    receipt.quantity = quantity;

    if (!add_transaction(receipt)) return false;
    customers_[index] = updated;
    return true;
}

string Store::customer_info()
{
    string info;
    for (int i = 0; i < num_customers_; i++)
        info += customers_[i].person_info() + "\n";
    return info;
}

string Store::product_info()
{
    string info;
    for (int i = 0; i < num_products_; i++)
        info += products_[i].product_info() + "\n";
    return info;
}

string Store::transaction_info()
{
    string info;
    for (int i = 0; i < num_transactions_; i++)
        info += transactions_[i].transaction_info() + "\n";
    return info;
}

string Store::store_info()
{
    string info = "Customers:\n" + customer_info();
    info += "\nProducts:\n" + product_info();
    info += "\nAll Transactions:\n" + transaction_info();
    return info;
}

string Store::purchase_history(Person customer)
{
    string info;
    for (int i = 0; i < num_transactions_; i++)
    {
        if (transactions_[i].customer.name() == customer.name())
            info += transactions_[i].transaction_info() + "\n";
    }
    return info;
}
