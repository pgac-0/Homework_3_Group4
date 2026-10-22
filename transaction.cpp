#include "transaction.h"
#include <string>

std::string transaction::transaction_info()
{
    std::string info = "Transaction #: " + std::to_string(transaction_number);

    info += "\nCustomer:\n";
    info += customer.person_info();

    info += "Product: \n";
    info += purchased_item.product_info();

    info += "Quantity: " +
            std::to_string(quantity) + "\n";

    return info;
}
