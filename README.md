# Banking System Using C++

A menu-driven console banking simulation built with C++ classes and object-oriented programming.

## Features
- Create customers and bank accounts.
- Deposit money into an account.
- Withdraw money with balance validation.
- Transfer funds between two accounts.
- Display customer and account details.
- View recent transactions for an account.
- Validate account numbers, amounts, and menu input.

## C++ Concepts
- Classes and objects (`Customer`, `Account`, `Transaction`, `Bank`)
- Encapsulation
- Vectors
- Functions
- Loops and conditional statements
- Input validation
- Reverse iteration for recent transaction history

## Requirements
- C++ compiler (GCC/MinGW).
- Visual Studio Code (optional).

## Compile

```bash
g++ Banking_System.cpp -o BankingSystem
```

## Run

Windows:
```bash
BankingSystem.exe
```

Linux/macOS:
```bash
./BankingSystem
```

## How to Use
1. Create a customer and note the generated customer ID.
2. Create an account using that customer ID.
3. Use the account number to deposit or withdraw money.
4. Create another customer/account to test fund transfers.
5. Display account details or view recent transactions.
6. Select Exit to close the program.

## Important Notes
- This is an educational simulation, not real banking software.
- Balances and transaction history are kept in memory and reset when the program exits.
- Amounts are entered as whole rupees (no paise/decimal amounts).
- No real money or bank services are involved.
