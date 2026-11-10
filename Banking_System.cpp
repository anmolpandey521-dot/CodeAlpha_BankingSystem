#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <limits>
using namespace std;

class Customer
{
private:
    int customerId;
    string name;

public:
    Customer(int id, const string& customerName)
        : customerId(id), name(customerName) {}

    int getId() const { return customerId; }
    string getName() const { return name; }
};

class Transaction
{
private:
    int transactionId;
    string type;
    long long amount;
    int fromAccount;
    int toAccount;
    string dateTime;

public:
    Transaction(int id, const string& transactionType, long long value,
                int from, int to)
        : transactionId(id), type(transactionType), amount(value),
          fromAccount(from), toAccount(to)
    {
        time_t now = time(nullptr);
        char* timeText = ctime(&now);
        dateTime = timeText ? timeText : "Unknown date";
        if (!dateTime.empty() && dateTime.back() == '\n')
            dateTime.pop_back();
    }

    bool involvesAccount(int accountNumber) const
    {
        return fromAccount == accountNumber || toAccount == accountNumber;
    }

    void display() const
    {
        cout << left << setw(6) << transactionId
             << setw(14) << type
             << "Rs. " << setw(10) << amount
             << "From: " << setw(8) << (fromAccount == 0 ? "-" : to_string(fromAccount))
             << "To: " << setw(8) << (toAccount == 0 ? "-" : to_string(toAccount))
             << dateTime << '\n';
    }
};

class Account
{
private:
    int accountNumber;
    int customerId;
    long long balance;

public:
    Account(int number, int ownerId)
        : accountNumber(number), customerId(ownerId), balance(0) {}

    int getAccountNumber() const { return accountNumber; }
    int getCustomerId() const { return customerId; }
    long long getBalance() const { return balance; }

    void deposit(long long amount) { balance += amount; }
    bool withdraw(long long amount)
    {
        if (amount <= 0 || amount > balance)
            return false;

        balance -= amount;
        return true;
    }
};

class Bank
{
private:
    vector<Customer> customers;
    vector<Account> accounts;
    vector<Transaction> transactions;

    int nextCustomerId = 1001;
    int nextAccountNumber = 5001;
    int nextTransactionId = 1;

    Customer* findCustomer(int id)
    {
        for (auto& customer : customers)
            if (customer.getId() == id)
                return &customer;
        return nullptr;
    }

    Account* findAccount(int number)
    {
        for (auto& account : accounts)
            if (account.getAccountNumber() == number)
                return &account;
        return nullptr;
    }

public:
    void createCustomer()
    {
        string name;
        cout << "Enter customer name: ";
        getline(cin, name);

        if (name.empty())
        {
            cout << "Name cannot be empty.\n";
            return;
        }

        int id = nextCustomerId++;
        customers.emplace_back(id, name);

        cout << "Customer created successfully.\n";
        cout << "Customer ID: " << id << '\n';
    }

    void createAccount()
    {
        int customerId;
        cout << "Enter customer ID: ";
        if (!(cin >> customerId))
        {
            invalidInput();
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (findCustomer(customerId) == nullptr)
        {
            cout << "Customer not found. Create the customer first.\n";
            return;
        }

        int number = nextAccountNumber++;
        accounts.emplace_back(number, customerId);

        cout << "Account created successfully.\n";
        cout << "Account number: " << number << '\n';
    }

    void depositMoney()
    {
        int number;
        long long amount;

        cout << "Enter account number: ";
        if (!(cin >> number)) { invalidInput(); return; }

        Account* account = findAccount(number);
        if (account == nullptr)
        {
            cout << "Account not found.\n";
            return;
        }

        cout << "Enter deposit amount (whole rupees): ";
        if (!(cin >> amount)) { invalidInput(); return; }

        if (amount <= 0)
        {
            cout << "Amount must be greater than zero.\n";
            return;
        }

        account->deposit(amount);
        transactions.emplace_back(nextTransactionId++, "Deposit",
                                  amount, 0, number);
        cout << "Deposit successful. New balance: Rs. "
             << account->getBalance() << '\n';
    }

    void withdrawMoney()
    {
        int number;
        long long amount;

        cout << "Enter account number: ";
        if (!(cin >> number)) { invalidInput(); return; }

        Account* account = findAccount(number);
        if (account == nullptr)
        {
            cout << "Account not found.\n";
            return;
        }

        cout << "Enter withdrawal amount (whole rupees): ";
        if (!(cin >> amount)) { invalidInput(); return; }

        if (!account->withdraw(amount))
        {
            cout << "Withdrawal failed. Check the amount and available balance.\n";
            return;
        }

        transactions.emplace_back(nextTransactionId++, "Withdrawal",
                                  amount, number, 0);
        cout << "Withdrawal successful. New balance: Rs. "
             << account->getBalance() << '\n';
    }

    void transferMoney()
    {
        int fromNumber, toNumber;
        long long amount;

        cout << "Enter sender account number: ";
        if (!(cin >> fromNumber)) { invalidInput(); return; }

        cout << "Enter receiver account number: ";
        if (!(cin >> toNumber)) { invalidInput(); return; }

        if (fromNumber == toNumber)
        {
            cout << "Sender and receiver accounts must be different.\n";
            return;
        }

        Account* sender = findAccount(fromNumber);
        Account* receiver = findAccount(toNumber);

        if (sender == nullptr || receiver == nullptr)
        {
            cout << "One or both account numbers were not found.\n";
            return;
        }

        cout << "Enter transfer amount (whole rupees): ";
        if (!(cin >> amount)) { invalidInput(); return; }

        if (amount <= 0 || amount > sender->getBalance())
        {
            cout << "Transfer failed. Check the amount and sender balance.\n";
            return;
        }

        // Validate both accounts before changing either balance.
        sender->withdraw(amount);
        receiver->deposit(amount);

        transactions.emplace_back(nextTransactionId++, "Transfer",
                                  amount, fromNumber, toNumber);

        cout << "Transfer successful.\n";
        cout << "Sender balance: Rs. " << sender->getBalance() << '\n';
        cout << "Receiver balance: Rs. " << receiver->getBalance() << '\n';
    }

    void displayAccountInfo()
    {
        int number;
        cout << "Enter account number: ";
        if (!(cin >> number)) { invalidInput(); return; }

        Account* account = findAccount(number);
        if (account == nullptr)
        {
            cout << "Account not found.\n";
            return;
        }

        Customer* customer = findCustomer(account->getCustomerId());

        cout << "\n========== ACCOUNT DETAILS ==========\n";
        cout << "Customer name: "
             << (customer ? customer->getName() : "Unknown") << '\n';
        cout << "Customer ID: " << account->getCustomerId() << '\n';
        cout << "Account number: " << account->getAccountNumber() << '\n';
        cout << "Available balance: Rs. " << account->getBalance() << '\n';
    }

    void viewRecentTransactions()
    {
        int number, count;
        cout << "Enter account number: ";
        if (!(cin >> number)) { invalidInput(); return; }

        if (findAccount(number) == nullptr)
        {
            cout << "Account not found.\n";
            return;
        }

        cout << "How many recent transactions would you like to view? ";
        if (!(cin >> count)) { invalidInput(); return; }

        if (count <= 0)
        {
            cout << "Enter a positive number.\n";
            return;
        }

        cout << "\n========== RECENT TRANSACTIONS ==========\n";
        cout << left << setw(6) << "ID" << setw(14) << "Type"
             << setw(15) << "Amount" << setw(14) << "From"
             << setw(14) << "To" << "Date and time\n";

        int shown = 0;
        for (auto it = transactions.rbegin();
             it != transactions.rend() && shown < count; ++it)
        {
            // Show transactions involving this account.
            // Transaction display includes both account numbers.
            // Filter using a helper representation below.
            // The transaction is displayed when it belongs to this account.
            // Accessors are used to keep Transaction data encapsulated.
            if (it->involvesAccount(number))
            {
                it->display();
                shown++;
            }
        }

        if (shown == 0)
            cout << "No transactions found for this account.\n";
    }

    void invalidInput()
    {
        cout << "Invalid input. Please enter the correct value.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void run()
    {
        int choice;
        do
        {
            cout << "\n========== BANKING SYSTEM ==========\n"
                 << "1. Create Customer\n"
                 << "2. Create Account\n"
                 << "3. Deposit Money\n"
                 << "4. Withdraw Money\n"
                 << "5. Transfer Funds\n"
                 << "6. Display Account Information\n"
                 << "7. View Recent Transactions\n"
                 << "8. Exit\n"
                 << "Enter your choice: ";

            if (!(cin >> choice))
            {
                invalidInput();
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice)
            {
                case 1: createCustomer(); break;
                case 2: createAccount(); break;
                case 3: depositMoney(); break;
                case 4: withdrawMoney(); break;
                case 5: transferMoney(); break;
                case 6: displayAccountInfo(); break;
                case 7: viewRecentTransactions(); break;
                case 8: cout << "Thank you for using the Banking System.\n"; break;
                default: cout << "Invalid choice. Select 1 to 8.\n";
            }
        } while (choice != 8);
    }
};

int main()
{
    Bank bank;
    bank.run();
    return 0;
}
