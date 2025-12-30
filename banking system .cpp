#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstring>
#include <cctype>

using namespace std;

//-------------------- CONSTANTS --------------------//
const int NAME_LEN = 50;
const char USERNAME[] = "admin";
const char PASSWORD[] = "nitish@123";

//-------------------- CLASS ------------------------//
class BankAccount {
private:
    unsigned long long accNo;
    char holderName[NAME_LEN];
    char accType;
    int balance;
    char createdOn[30];

public:
    void createAccount();
    void showAccount() const;
    void showRow() const;

    unsigned long long getAccNo() const { return accNo; }
    int getBalance() const { return balance; }
    char getType() const { return accType; }
};

//-------------------- UTILITIES --------------------//
void clearScreen() {
    system("cls");
}

bool loginSystem() {
    char user[20], pass[20];
    int attempts = 3;

    while (attempts--) {
        cout << "\n===== LOGIN =====\n";
        cout << "Username: ";
        cin >> user;
        cout << "Password: ";
        cin >> pass;

        if (strcmp(user, USERNAME) == 0 && strcmp(pass, PASSWORD) == 0) {
            cout << "\nLogin Successful!\n";
            return true;
        }
        cout << "Invalid credentials. Attempts left: " << attempts << endl;
    }
    return false;
}

bool accountExists(unsigned long long acc) {
    BankAccount b;
    ifstream file("accounts.dat", ios::binary);

    while (file.read((char*)&b, sizeof(b))) {
        if (b.getAccNo() == acc) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

//---------------- ACCOUNT FUNCTIONS ----------------//
void BankAccount::createAccount() {
    cout << "\n===== CREATE ACCOUNT =====\n";
    cout << "Enter 12-digit Account Number: ";
    cin >> accNo;

    while (to_string(accNo).length() != 12 || accountExists(accNo)) {
        cout << "Invalid or Existing Account. Re-enter: ";
        cin >> accNo;
    }

    cin.ignore();
    cout << "Enter Holder Name: ";
    cin.getline(holderName, NAME_LEN);

    cout << "Account Type (S/C): ";
    cin >> accType;
    accType = toupper(accType);

    while (accType != 'S' && accType != 'C') {
        cout << "Enter only S or C: ";
        cin >> accType;
        accType = toupper(accType);
    }

    cout << "Initial Balance: ";
    cin >> balance;

    int minBal = (accType == 'S') ? 500 : 1000;
    while (balance < minBal) {
        cout << "Minimum balance is " << minBal << ". Re-enter: ";
        cin >> balance;
    }

    time_t now = time(0);
    strcpy(createdOn, ctime(&now));

    ofstream file("accounts.dat", ios::binary | ios::app);
    file.write((char*)this, sizeof(*this));
    file.close();

    cout << "\nAccount Created Successfully!\n";
}

void BankAccount::showAccount() const {
    cout << "\nAccount No : " << accNo;
    cout << "\nName       : " << holderName;
    cout << "\nType       : " << (accType == 'S' ? "Saving" : "Current");
    cout << "\nBalance    : " << balance;
    cout << "\nCreated On : " << createdOn << endl;
}

void BankAccount::showRow() const {
    cout << left << setw(15) << accNo
         << setw(20) << holderName
         << setw(10) << accType
         << setw(10) << balance << endl;
}

//---------------- VIEW FUNCTIONS -------------------//
void viewAllAccounts() {
    BankAccount b;
    ifstream file("accounts.dat", ios::binary);

    cout << "\n===== ALL ACCOUNTS =====\n";
    cout << left << setw(15) << "ACC NO"
         << setw(20) << "NAME"
         << setw(10) << "TYPE"
         << setw(10) << "BALANCE" << endl;
    cout << string(55, '-') << endl;

    while (file.read((char*)&b, sizeof(b))) {
        b.showRow();
    }
    file.close();
}

void searchAccount() {
    unsigned long long acc;
    BankAccount b;
    bool found = false;

    cout << "\nEnter Account Number: ";
    cin >> acc;

    ifstream file("accounts.dat", ios::binary);
    while (file.read((char*)&b, sizeof(b))) {
        if (b.getAccNo() == acc) {
            b.showAccount();
            found = true;
            break;
        }
    }
    file.close();

    if (!found)
        cout << "\nAccount Not Found!\n";
}

//---------------- STATISTICS -----------------------//
void accountStatistics() {
    BankAccount b;
    ifstream file("accounts.dat", ios::binary);

    int totalAcc = 0, saving = 0, current = 0;
    int totalBalance = 0, maxBalance = 0;
    unsigned long long richAcc = 0;

    while (file.read((char*)&b, sizeof(b))) {
        totalAcc++;
        totalBalance += b.getBalance();

        if (b.getType() == 'S') saving++;
        else current++;

        if (b.getBalance() > maxBalance) {
            maxBalance = b.getBalance();
            richAcc = b.getAccNo();
        }
    }
    file.close();

    cout << "\n===== ACCOUNT STATISTICS =====\n";
    cout << "Total Accounts        : " << totalAcc << endl;
    cout << "Total Bank Balance    : " << totalBalance << endl;
    cout << "Saving Accounts       : " << saving << endl;
    cout << "Current Accounts      : " << current << endl;
    cout << "Highest Balance A/C   : " << richAcc
         << " (₹" << maxBalance << ")" << endl;
}

//---------------- MAIN -----------------------------//
int main() {
    if (!loginSystem()) {
        cout << "\nAccess Denied!\n";
        return 0;
    }

    char choice;
    BankAccount acc;

    do {
        clearScreen();
        cout << "\n===== BANKING SYSTEM =====\n";
        cout << "1. Create Account\n";
        cout << "2. View All Accounts\n";
        cout << "3. Search Account\n";
        cout << "4. Account Statistics\n";
        cout << "5. Exit\n";
        cout << "Choose option: ";
        cin >> choice;

        clearScreen();
        switch (choice) {
        case '1': acc.createAccount(); break;
        case '2': viewAllAccounts(); break;
        case '3': searchAccount(); break;
        case '4': accountStatistics(); break;
        case '5': cout << "\nProject Developed by Nitish Kumar\n"; break;
        default: cout << "Invalid Choice!\n";
        }
        cin.ignore();
        cin.get();
    } while (choice != '5');

    return 0;
}
