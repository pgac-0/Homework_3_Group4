
#include "person.h"

Person::Person()
{
    name_ = "";
    phone_ = "";

    person_address_ = address{};
    person_credit_card_ = credit_card{};
}

Person::Person(string name, string phone,
               address home, credit_card card)
{
    name_ = name;
    phone_ = phone;
    person_address_ = home;
    person_credit_card_ = card;
}

void Person::name(string newName)
{
    name_ = newName;
}

string Person::name()
{
    return name_;
}

void Person::phone(string newPhone)
{
    phone_ = newPhone;
}

string Person::phone()
{
    return phone_;
}

void Person::person_address(address newAddress)
{
    person_address_ = newAddress;
}

address Person::person_address()
{
    return person_address_;
}

void Person::person_credit_card(credit_card newCard)
{
    person_credit_card_ = newCard;
}

credit_card Person::person_credit_card()
{
    return person_credit_card_;
}

//
string Person::person_info()
{

    string info = "";

    info += "Name: " + name_ + "\n";
    info += "Phone: " + phone_ + "\n";

    info += "Address:\n";
    info += person_address_.address_info() + "\n";

    info += "Credit Card:\n";
    info += person_credit_card_.credit_card_info();

    return info;
}
