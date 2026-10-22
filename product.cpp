#include "product.h"
#include <sstream>
#include <iomanip>

std::string product::product_info()
{
    std::ostringstream output;
    output << std::fixed << std::setprecision(2);

    output << "Name: " << name << "\n";
    output << "Description: " << description << "\n";
    output << "Price: $" << price << "\n";

    return output.str();
}
