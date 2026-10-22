
#include "credit_card.h"
#include <sstream>
#include <iomanip>
#include <cmath>

bool credit_card::make_purchase(double purchase_amount)
{
    if (!std::isfinite(purchase_amount) || purchase_amount <= 0 ||
        !std::isfinite(limit) || !std::isfinite(current_balance))
        return false;
    if (purchase_amount > limit - current_balance)
        return false;
    current_balance += purchase_amount;
    return true;
}

bool credit_card::make_payment(double payment_amount)
{
    if (!std::isfinite(payment_amount) || payment_amount <= 0 ||
        payment_amount > current_balance)
        return false;
    current_balance -= payment_amount;
    return true;
}

std::string credit_card::credit_card_info()
{
    std::ostringstream out;
    out << "Credit card number: " << cc_number << "\n";
    out << std::fixed << std::setprecision(2);
    out << "Credit Card Limit: $" << limit << "\n";
    out << "Current Balance: $" << current_balance << "\n";
    return out.str();
}
