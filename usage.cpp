#include "SSPtr.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

template <typename T>
T wrap(std::istream &is) { T t; is >> t; return t;}

class BankAccount {
public:
    int balance;
    int accountId;
    bool state;
    BankAccount() {}
    enum state {ACTIVE=true, INACTIVE=false,};
    BankAccount(int balance, int accountId, int state) :
        balance(balance), accountId(accountId), state(state) {}

    BankAccount(const BankAccount &obj) {
        this->state = obj.state;
        this->accountId = obj.accountId;
        this->balance = obj.balance;
    }

    friend std::ostream& operator<<(std::ostream& os, const BankAccount& ba) {
        os << "Account Status :"<< (ba.state == true ? "Active" : "Inactive") << std::endl
            << "Account ID : " << ba.accountId << std::endl << "Balance : " <<  ba.balance << std::endl;

        return os;
    }

    BankAccount *operator=(BankAccount *ba) {
        this->balance = ba->balance;
        this->state = ba->state;
        this->accountId = ba->accountId;
        return ba;
    }
    friend std::istream& operator>>(std::istream& is, BankAccount& ba){
        std::cout << "Account Status (0, 1) : ";
        if(wrap<int>(std::cin) == 0)
            ba.state = BankAccount::INACTIVE;
        std::cout << "Account ID: ";
        is >> ba.accountId;
        std::cout << "Account Balance : ";
        is >> ba.balance;

        return is;
    }
};
int main() {

    SSPtr<int, 10> ar = new int[10];
    /*
     * Usage with object arrays
     */
    SSPtr<BankAccount, 5> baArr = new BankAccount[5];
    //Accessing like a normal array
    baArr[0] = BankAccount(10000, 99, BankAccount::ACTIVE);
    baArr[1] = BankAccount(43542, 12, BankAccount::INACTIVE);
    baArr[2] = BankAccount(baArr[1]);

    //using iterators
    SSPtr<BankAccount>::SSPIndexer index = baArr.first();
    std::srand(std::time(nullptr));

    for(unsigned i = 0; i < index.size(); i++)
        index[i] = BankAccount(std::rand(), std::rand(), std::rand()%2);

    for(index = baArr.first(); index < baArr.last(); index++)
        std::cout << *baArr << std::endl;

    index = baArr.first();

    while(index != baArr.last())
        std::cin >> *(index++);
    index = baArr.last() - 2;
    while(index != baArr.last())
        std::cout << *(index++);


    //Manual memory deallocation, yea its possible ;)
    SSPtr<int>::free();
    SSPtr<BankAccount>::free();

    SSPtr<int*, 10> multDArr = new int*[10];     //a pointer to pointer? why not?!.
    SSPtr<int*>::SSPIndexer multDArrIndexer = multDArr.first();
    SSPtr<int*>::free();
    return 0;
}
