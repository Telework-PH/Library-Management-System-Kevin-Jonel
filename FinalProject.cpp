#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <unordered_map>

using namespace std;

unordered_map<string, string> loginData;

// Function to get password without showing characters on the console
string getHiddenPassword() {
    const char BACKSPACE = 8;
    const char RETURN = 13;

    string password;
    char ch = 0;

    while ((ch = _getwch()) != RETURN) {
        if (ch == BACKSPACE) {
            if (password.length() > 0) {
                cout << "\b \b";
                password.pop_back();
            }
        } else {
            cout << '*';
            password += ch;
        }
    }
    cout << endl;
    return password;
}
void loadUser() {
    ifstream file("SignUp.txt");
    if (!file) {
        cout << "\t\t\t\t\tNo user data file found. Creating a new one." << std::endl;
        return;
    }
    string username, password;
    while (file >> username >> password) {
        loginData[username] = password;
    }
    file.close();
}
void saveUser() {
    ofstream file("SignUp.txt", ios::out | ios::app);
    for (const auto& user : loginData) {
        file << user.first << " " << user.second << endl; // first = username, second = password
    }
    file.close();
}
void signUp() {
    string username, password;
    error:
    cout << "\t\t\t\t\t----------------------------------" << endl;
    cout << "\t\t\t\t\tSign up here:" << endl;
    cout << "\t\t\t\t\t----------------------------------" << endl;
    cout << "\t\t\t\t\tEnter Username: ";
    cin >> username;

    if (loginData.find(username) != loginData.end()) {
        cout << "\t\t\t\t\tUsername already exists. Please try again." << endl;
        goto error;
    }
    cout << "\t\t\t\t\tEnter Password: ";
    password = getHiddenPassword();

    loginData[username] = password;
    saveUser();

    cout << "\t\t\t\t\tSign-up successful. Please continue to log in." << endl;
}
void logIn() {
    string username, password;
    error:
    cout << "\t\t\t\t\t----------------------------------" << endl;
    cout << "\t\t\t\t\tUsername: ";
    cin >> username;

    auto it = loginData.find(username);
    if (it == loginData.end()) {
        cout << "\t\t\t\t\tUsername not found. Please sign up first." << endl;
        goto error;
    }
    cout << "\t\t\t\t\tEnter password: ";
    password = getHiddenPassword();

    if (it->second == password) {
        cout << "\t\t\t\t\tLog-in successful. Welcome, " << username << "!" << endl;
    } else {
        cout << "\t\t\t\t\tIncorrect password. Please try again." << endl;
        goto error;
    }
}


struct Book {
    int id;
    string title, author, publisher;
    bool isAvailable = true;
};

vector<Book> books;

static bool compareId(const Book &number1, const Book &number2) {
    return number1.id < number2.id;
}
string lowerCase(const std::string& str) {
    string lowerLetter = str;
    transform(lowerLetter.begin(), lowerLetter.end(), lowerLetter.begin(), ::tolower);
    return lowerLetter;
}

void addBook() {
    Book book;
    fstream file;
    file.open("BookManagement.txt", ios::out | ios::app);
    cout << "\t\t\t\t\tADD BOOK" << endl;
    cout << "\t\t\t\t\tBook Title: ";
    getline(cin, book.title);
    cout << "\t\t\t\t\tBook Author: ";
    getline(cin, book.author);
    cout << "\t\t\t\t\tBook Publisher: ";
    getline(cin, book.publisher);
    cout << "\t\t\t\t\tAdd successful!" << endl;

    srand(time(0));
    int random = 100 + (rand() % 1000);
    book.id = random;
    file << book.id << "," << book.title << "," << book.author << "," << book.publisher << endl;
    file.close();
    books.push_back(book);
}
void searchBook() {
    ifstream file("BookManagement.txt");
    string search;
    cout << "\t\t\t\t\tSearch book here: " << endl;
    cin.ignore();
    getline(cin, search);
    bool bookFound = false;
    string line;
    string lowerCaseLetter = lowerCase(search);
    vector<string> linesFound;
    while (getline(file, line)) {
        string Letters = line;
        size_t exist = Letters.find(search);
        if (exist != string::npos) {
            bookFound = true;
            linesFound.push_back(line);
        }
    }
    file.close();
    if (!bookFound) {
        cout << "\t\t\t\t\tBook not found" << endl;
    } else {
        cout << "\t\t\t\t\tSearch Result: " << endl;
        for (const string& bookFound : linesFound) {
            cout << "\t\t\t\t\t" << bookFound << endl;
        }
    }
}
void deleteBook() {
    int deleteID;
    cout << "\t\t\t\t\tEnter Book ID to delete: ";
    cin >> deleteID;

    ifstream inputFile("BookManagement.txt");
    ofstream tempFile("TempFile.txt");

    if (!inputFile || !tempFile) {
        cout << "\t\t\t\t\tError opening files." << endl;
        return;
    }

    bool found = false;
    string line;
    while (getline(inputFile, line)) {
        string::size_type pos = line.find(',');
        int id = stoi(line.substr(0, pos));

        if (id == deleteID) {
            found = true;
        } else {
            tempFile << line << endl;
        }
    }
    inputFile.close();
    tempFile.close();

    if (found) {
        remove("BookManagement.txt");
        rename("TempFile.txt", "BookManagement.txt");
        cout << "\t\t\t\t\tBook with ID " << deleteID << " has been deleted." << endl;
    } else {
        remove("TempFile.txt");
        cout << "\t\t\t\t\tBook with ID " << deleteID << " not found." << endl;
    }
}
void updateBook() {
    int updateID;
    cout << "\t\t\t\t\tEnter Book ID to update: ";
    cin >> updateID;

    ifstream inputFile("BookManagement.txt");
    ofstream tempFile("TempFile.txt");

    if (!inputFile || !tempFile) {
        cout << "\t\t\t\t\tError opening files." << endl;
        return;
    }

    bool found = false;
    string line;
    while (getline(inputFile, line)) {
        string::size_type pos = line.find(',');
        int id = stoi(line.substr(0, pos));

        if (id == updateID) {
            found = true;

            Book book;
            book.id = updateID;
            cin.ignore();
            cout << "\t\t\t\t\tUpdate Book Title: ";
            getline(cin, book.title);
            cout << "\t\t\t\t\tUpdate Book Author: ";
            getline(cin, book.author);
            cout << "\t\t\t\t\tUpdate Book Publisher: ";
            getline(cin, book.publisher);

            tempFile << book.id << "," << book.title << "," << book.author << "," << book.publisher << endl;
        } else {
            tempFile << line << endl;
        }
    }

    inputFile.close();
    tempFile.close();

    if (found) {
        remove("BookManagement.txt");
        rename("TempFile.txt", "BookManagement.txt");
        cout << "\t\t\t\t\tBook with ID " << updateID << " has been updated." << endl;
    } else {
        remove("TempFile.txt");
        cout << "\t\t\t\t\tBook with ID " << updateID << " not found." << endl;
    }
}
void displayBooks() {
    ifstream file("BookManagement.txt");
    if (!file) {
        cout << "\t\t\t\t\tError opening file." << endl;
        return;
    }

    books.clear();
    int id;
    string title, author, publisher;
    while (file >> id) {
        file.ignore();
        getline(file, title, ',');
        getline(file, author, ',');
        getline(file, publisher, '\n');
        books.push_back({id, title, author, publisher});
    }


    if (books.empty()) {
        cout << "\t\t\t\t\tNo books found." << endl;
    } else {
        sort(books.begin(),books.end(), compareId);
        cout << "\t\t\t\t\tAll Books in the Library: " << endl;
        for (const Book& book : books) {
            cout << "\t\t\t\t\tBook ID: " << book.id << endl;
            cout << "\t\t\t\t\tTitle: " << book.title << endl;
            cout << "\t\t\t\t\tAuthor: " << book.author << endl;
            cout << "\t\t\t\t\tPublisher: " << book.publisher << endl;
            cout << "\t\t\t\t\tStatus: " << (book.isAvailable ? "Available" : "Borrowed") << endl;
            cout << "\t\t\t\t\t------------------------------" << endl;
        }
    }
    file.close();
}

struct Patron {
    int id;
    string name, email, contactNumber;
};
vector<Patron> patrons;
static bool compareIdPatron(const Patron &number1, const Patron &number2) {
    return number1.id < number2.id;
}
void addPatron() {
    Patron patron;
    fstream file;
    file.open("PatronManagement.txt", ios::out | ios::app);
    cout << "\t\t\t\t\tADD PATRON" << endl;
    cout << "\t\t\t\t\tPatron Name: ";
    getline(cin, patron.name);
    cout << "\t\t\t\t\tPatron Contact Number: ";
    getline(cin, patron.contactNumber);
    cout << "\t\t\t\t\tPatron Email: ";
    getline(cin, patron.email);
    cout << "\t\t\t\t\tAdd successful!" << endl;

    srand(time(0));
    int random = 100 + (rand() % 1000);
    patron.id = random;
    file << patron.id << "," << patron.name << "," << patron.contactNumber << "," << patron.email << endl;
    file.close();
    patrons.push_back(patron);
}
void searchPatron() {
    ifstream file("PatronManagement.txt");
    string search;
    cout << "\t\t\t\t\tSearch patron here: " << endl;
    cin.ignore();
    getline(cin, search);
    bool patronFound = false;
    string line;
    string lowerCaseLetter = lowerCase(search);
    vector<string> linesFound;
    while (getline(file, line)) {
        string Letters = line;
        size_t exist = Letters.find(search);
        if (exist != string::npos) {
            patronFound = true;
            linesFound.push_back(line);
        }
    }
    file.close();
    if (!patronFound) {
        cout << "\t\t\t\t\tPatron not found" << endl;
    } else {
        cout << "\t\t\t\t\tPatron Result: " << endl;
        for (const string& patronFound : linesFound) {
            cout << "\t\t\t\t\t" << patronFound << endl;
        }
    }
}
void deletePatron() {
    int deleteID;
    cout << "\t\t\t\t\tEnter Patron ID to delete: ";
    cin >> deleteID;

    ifstream inputFile("PatronManagement.txt");
    ofstream tempFile("TempFilePatron.txt");

    if (!inputFile || !tempFile) {
        cout << "\t\t\t\t\tError opening files." << endl;
        return;
    }

    bool found = false;
    string line;
    while (getline(inputFile, line)) {
        string::size_type pos = line.find(',');
        int id = stoi(line.substr(0, pos));

        if (id == deleteID) {
            found = true;
        } else {
            tempFile << line << endl;
        }
    }
    inputFile.close();
    tempFile.close();

    if (found) {
        remove("PatronManagement.txt");
        rename("TempFilePatron.txt", "PatronManagement.txt");
        cout << "\t\t\t\t\tPatron with ID " << deleteID << " has been deleted." << endl;
    } else {
        remove("TempFilePatron.txt");
        cout << "\t\t\t\t\tPatron with ID " << deleteID << " not found." << endl;
    }
}
void updatePatron() {
    int updateID;
    cout << "\t\t\t\t\tEnter Patron ID to update: ";
    cin >> updateID;

    ifstream inputFile("PatronManagement.txt");
    ofstream tempFile("TempFilePatron.txt");

    if (!inputFile || !tempFile) {
        cout << "\t\t\t\t\tError opening files." << endl;
        return;
    }

    bool found = false;
    string line;
    while (getline(inputFile, line)) {
        string::size_type pos = line.find(',');
        int id = stoi(line.substr(0, pos));

        if (id == updateID) {
            found = true;

            Patron patron;
            patron.id = updateID;
            cin.ignore();
            cout << "\t\t\t\t\tUpdate Patron Name: ";
            getline(cin, patron.name);
            cout << "\t\t\t\t\tUpdate Patron Contact Number: ";
            cin >> patron.contactNumber;
            cout << "\t\t\t\t\tUpdate Patron Email: ";
            cin.ignore();
            getline(cin, patron.email);

            tempFile << patron.id << "," << patron.name << "," << patron.contactNumber << "," << patron.email << endl;
        } else {
            tempFile << line << endl;
        }
    }

    inputFile.close();
    tempFile.close();

    if (found) {
        remove("PatronManagement.txt");
        rename("TempFilePatron.txt", "PatronManagement.txt");
        cout << "\t\t\t\t\tPatron with ID " << updateID << " has been updated." << endl;
    } else {
        remove("TempFilePatron.txt");
        cout << "\t\t\t\t\tPatron with ID " << updateID << " not found." << endl;
    }
}
void displayPatrons() {
    ifstream file("PatronManagement.txt");
    if (!file) {
        cout << "\t\t\t\t\tError opening file." << endl;
        return;
    }

    patrons.clear();
    int id;
    string name, email, contactNumber;
    while (file >> id) {
        file.ignore();
        getline(file, name, ',');
        getline(file,contactNumber, ',');
        getline(file, email, '\n');
        patrons.push_back({id, name, email, contactNumber});
    }


    if (patrons.empty()) {
        cout << "\t\t\t\t\tNo patrons found." << endl;
    } else {
        sort(patrons.begin(),patrons.end(), compareIdPatron);
        cout << "\t\t\t\t\tAll Patrons in the Library: " << endl;
        for (const Patron& patron : patrons) {
            cout << "\t\t\t\t\tPatron ID: " << patron.id << endl;
            cout << "\t\t\t\t\tName: " << patron.name << endl;
            cout << "\t\t\t\t\tContact Number: " << patron.contactNumber << endl;
            cout << "\t\t\t\t\tEmail: " << patron.email << endl;
            cout << "\t\t\t\t\t------------------------------" << endl;
        }
    }
    file.close();
}

struct CheckOutRecord {
    int patronID;
    int bookID;
    time_t checkOutDate;
    time_t dueDate;
    bool returned;
};
vector<CheckOutRecord> checkOutRecords;
string checkoutFilename = "checkout.csv";
char separator = ',';
void loadCheckOutRecords() {
    ifstream file(checkoutFilename);
    if (!file.is_open()) {
        cout << "\t\t\t\t\tNo previous check-out records found." << endl;
        return;
    }
    checkOutRecords.clear();
    string line;
    while (getline(file, line)) {
        string patronID, bookID, checkOutDate, dueDate, returned;
        stringstream iss(line);
        getline(iss, patronID, separator);
        getline(iss, bookID, separator);
        getline(iss, checkOutDate, separator);
        getline(iss, dueDate, separator);
        getline(iss, returned);

        CheckOutRecord record;
        record.patronID = stoi(patronID);
        record.bookID = stoi(bookID);
        record.checkOutDate = stoi(checkOutDate);
        record.dueDate = stoi(dueDate);
        record.returned = (stoi(returned) == 1);

        checkOutRecords.push_back(record);
    }
    file.close();
}
void displayCheckOutRecords() {
    cout << "\t\t\t\t\tCHECK-OUT RECORDS" << endl;
    cout << "\t\t\t\t\t------------------------------------" << endl;
    for (const CheckOutRecord& record : checkOutRecords) {
        cout << "\t\t\t\t\tPatron ID: " << record.patronID << endl;
        cout << "\t\t\t\t\tBook ID: " << record.bookID << endl;
        cout << "\t\t\t\t\tCheck-out Date: " << asctime(localtime(&record.checkOutDate));
        cout << "\t\t\t\t\tDue Date: " << asctime(localtime(&record.dueDate));
        cout << "\t\t\t\t\tStatus: " << (record.returned ? "Returned" : "Not Returned") << endl;
        cout << "\t\t\t\t\t------------------------------------" << endl;
    }
}
void saveCheckOutRecords() {
    ofstream file(checkoutFilename);
    if (!file.is_open()) {
        cout << "\t\t\t\t\tError saving check-out records." << endl;
        return;
    }
    for (const CheckOutRecord &record : checkOutRecords) {
        file << record.patronID << separator << record.bookID << separator << record.checkOutDate << separator
             << record.dueDate << separator << (record.returned ? "1" : "0") << endl;
    }
    file.close();
}
void checkOutBook() {
    int patronID, bookID;
    time_t checkOutDate = time(nullptr);
    cout << "\t\t\t\t\tEnter Patron ID: ";
    cin >> patronID;

    // Check if the patron ID exists
    bool patronFound = false;
    for (const Patron& patron : patrons) {
        if (patron.id == patronID) {
            patronFound = true;
            break;
        }
    }
    if (!patronFound) {
        cout << "\t\t\t\t\tPatron with ID " << patronID << " not found." << endl;
        return;
    }

    cout << "\t\t\t\t\tEnter Book ID: ";
    cin >> bookID;

    // Check if the book ID exists
    bool bookFound = false;
    for (const Book& book : books) {
        if (book.id == bookID) {
            bookFound = true;
            break;
        }
    }
    if (!bookFound) {
        cout << "\t\t\t\t\tBook with ID " << bookID << " not found." << endl;
        return;
    }

    // Check if the book is available for check-out
    bool bookAvailable = false;
    for (Book& book : books) {
        if (book.id == bookID && book.isAvailable) {
            bookAvailable = true;
            book.isAvailable = false;
            break;
        }
    }
    if (!bookAvailable) {
        cout << "\t\t\t\t\tBook with ID " << bookID << " is not available for check-out." << endl;
        return;
    }

    // Calculate the due date (7 days from the check-out date)
    time_t dueDate = checkOutDate + (7 * 24 * 60 * 60); // 7 days in seconds

    // Add the check-out record to the checkOutRecords vector
    checkOutRecords.push_back({patronID, bookID, checkOutDate, dueDate, false});

    saveCheckOutRecords();

    cout << "\t\t\t\t\tBook with ID " << bookID << " has been checked out by Patron with ID " << patronID << "." << endl;
}
void returnBook() {
    int patronID, bookID;
    cout << "\t\t\t\t\tEnter Patron ID: ";
    cin >> patronID;
    cout << "\t\t\t\t\tEnter Book ID: ";
    cin >> bookID;

    // Find the check-out record with matching patronID and bookID
    auto it = std::find_if(checkOutRecords.begin(), checkOutRecords.end(), [patronID, bookID](const CheckOutRecord &record) {
        return record.patronID == patronID && record.bookID == bookID && !record.returned;
    });

    if (it != checkOutRecords.end()) {
        // Mark the book as returned and update the return date
        it->returned = true;
        it->dueDate = time(nullptr);

        // Update the book availability status
        for (Book& book : books) {
            if (book.id == bookID) {
                book.isAvailable = true;
                break;
            }
        }

        saveCheckOutRecords();

        cout << "\t\t\t\t\tBook with ID " << bookID << " has been returned by Patron with ID " << patronID << "." << endl;
    } else {
        cout << "\t\t\t\t\tNo matching check-out record found for Patron with ID " << patronID << " and Book with ID " << bookID << "." << endl;
    }
}

int main() {
    loadCheckOutRecords();
    int userInterface, choice, select;
    loadUser();
    cout << "\t\t\t\t\t===============================" << endl;
    cout << "\t\t\t\t\t|| LIBRARY MANAGEMENT SYSTEM ||" << endl;
    cout << "\t\t\t\t\t===============================" << endl;
    login:
    cout << "\t\t\t\t\tLOG IN or SIGN UP" << endl;
    cout << "\t\t\t\t\t[1] Sign in account." << endl;
    cout << "\t\t\t\t\t[2] Sign up account." << endl;
    cout << "\t\t\t\t\t[3] Exit." << endl;
    cout << "\t\t\t\t\t----------------------------------" << endl;
    cout << "\t\t\t\t\tSelect: ";
    cin >> userInterface;
    if (userInterface == 1)
    {
        loginuser:
        logIn();
        system( "pause");
        system("cls");
    }
    else if (userInterface == 2)
    {
        signUp();
        system("pause");
        system("cls");
        goto loginuser;
    }
    else if (userInterface == 3)
    {
        exit (0);
    }
    else goto login;
    selection:
    cout << "\t\t\t\t\t===============================" << endl;
    cout << "\t\t\t\t\t|| LIBRARY MANAGEMENT SYSTEM ||" << endl;
    cout << "\t\t\t\t\t===============================" << endl;
    cout << "\t\t\t\t\t[1] Book Management System." << endl;
    cout << "\t\t\t\t\t[2] Patron Management System." << endl;
    cout << "\t\t\t\t\t[3] Checked-out & Return Book." << endl;
    cout << "\t\t\t\t\t[0] Exit." << endl;
    cout << "\t\t\t\t\t-------------------------------------" << endl;
    cout << "\t\t\t\t\tSelect: ";
    cin >> choice;
    if (choice == 1)
    {
        while (true) {
            cout << "\t\t\t\t\t============================" << endl;
            cout << "\t\t\t\t\t|| BOOK MANAGEMENT SYSTEM ||" << endl;
            cout << "\t\t\t\t\t============================" << endl;
            cout << "\t\t\t\t\t[1] Add Book." << endl;
            cout << "\t\t\t\t\t[2] Search Book." << endl;
            cout << "\t\t\t\t\t[3] Update Book." << endl;
            cout << "\t\t\t\t\t[4] Delete Book." << endl;
            cout << "\t\t\t\t\t[5] Display Book." << endl;
            cout << "\t\t\t\t\t[0] Back to menu." << endl;
            cout << "\t\t\t\t\t----------------------------" << endl;
            cout << "\t\t\t\t\tSelect: ";
            cin >> select;
            switch (select) {
                case 1:
                    cin.ignore();
                    addBook();
                    system("pause");
                    system("cls");
                    break;
                case 2:
                    cin.ignore();
                    searchBook();
                    system("pause");
                    system("cls");
                    break;
                case 3:
                    cin.ignore();
                    updateBook();
                    system("pause");
                    system("cls");
                    break;
                case 4:
                    cin.ignore();
                    deleteBook();
                    system("pause");
                    system("cls");
                    break;
                case 5:
                    displayBooks();
                    system("pause");
                    system("cls");
                    break;
                case 0:
                    goto selection;
            }
        }
    }
    else if (choice == 2)
    {
        while (true) {
            cout << "\t\t\t\t\t==============================" << endl;
            cout << "\t\t\t\t\t|| PATRON MANAGEMENT SYSTEM ||" << endl;
            cout << "\t\t\t\t\t==============================" << endl;
            cout << "\t\t\t\t\t[1] Add Patron." << endl;
            cout << "\t\t\t\t\t[2] Search Patron." << endl;
            cout << "\t\t\t\t\t[3] Update Patron." << endl;
            cout << "\t\t\t\t\t[4] Delete Patron." << endl;
            cout << "\t\t\t\t\t[5] Display Patron." << endl;
            cout << "\t\t\t\t\t[0] Back to menu." << endl;
            cout << "\t\t\t\t\t------------------------------" << endl;
            cout << "\t\t\t\t\tSelect: ";
            cin >> select;
            switch (select) {
                case 1:
                    cin.ignore();
                    addPatron();
                    system("pause");
                    system("cls");
                    break;
                case 2:
                    cin.ignore();
                    searchPatron();
                    system("pause");
                    system("cls");
                    break;
                case 3:
                    updatePatron();
                    system("pause");
                    system("cls");
                    break;
                case 4:
                    deletePatron();
                    system("pause");
                    system("cls");
                    break;
                case 5:
                    cin.ignore();
                    displayPatrons();
                    system("pause");
                    system("cls");
                    break;
                case 0:
                    goto selection;
            }
        }
    }
    else if (choice == 3)
    {
        while (true) {
            cout << "\t\t\t\t\t=================================" << endl;
            cout << "\t\t\t\t\t|| CHECKED-OUT & RETURN SYSTEM ||" << endl;
            cout << "\t\t\t\t\t=================================" << endl;
            cout << "\t\t\t\t\t[1] Checked-Out." << endl;
            cout << "\t\t\t\t\t[2] Return Book." << endl;
            cout << "\t\t\t\t\t[3] Display Checked-Out Books." << endl;
            cout << "\t\t\t\t\t[0] Back to menu." << endl;
            cout << "\t\t\t\t\t------------------------------" << endl;
            cout << "\t\t\t\t\tSelect: ";
            cin >> select;
            switch (select) {
                case 1:
                    checkOutBook();
                    system("pause");
                    system("cls");
                    break;
                case 2:
                    returnBook();
                    system("pause");
                    system("cls");
                    break;
                case 3:
                    displayCheckOutRecords();
                    system("pause");
                    system("cls");
                    break;
                case 0:
                    goto selection;
            }
        }
    }
    else if (choice == 0)
    {
        cout << "\t\t\t\t\tThank you for using LIBRARY MANAGEMENT SYSTEM." << endl;
        exit (0);
    }
    else goto selection;

    return 0;
}
