# Banking System

This is a simple command-line banking system written in C++ that allows users to perform basic banking transactions such as checking balance, depositing money, withdrawing money, and transferring money. The system includes **user authentication** and **file-based storage** to manage accounts securely.

## Features

- **User Authentication**: Secure login system with username and password verification.
- **File-Based Storage**: Account details and transactions are stored persistently using files.
- **Balance Enquiry**: Displays the current balance of the account.
- **Deposit**: Allows users to deposit money into their accounts.
- **Withdraw**: Enables users to withdraw money with balance checks.
- **Transfer**: Supports money transfers between accounts.

## Code Structure

The project consists of the following files:

- **`main.cpp`**: Contains the main function and handles user interactions.
- **`Authentication.cpp`** and **`Authentication.h`**: Manage user authentication processes.
- **`BankAccount.cpp`** and **`BankAccount.h`**: Define the `BankAccount` class and its associated operations.
- **`Storage.cpp`** and **`Storage.h`**: Handle file-based data storage and retrieval.
