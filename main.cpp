#include <iostream>
#include <string>

#include "AvlTree.cpp"

using namespace std;

/**
 * Enum-class for user option selections
 */
enum class UserChoice {
    Invalid = 0,
    Insert = 1,
    Delete = 2,
    Print = 3,
    Exit = 4,
};

/**
 * Prints the choice menu, takes user choice and returns the appropriate flag.
 * @return user choice from menu
 */
UserChoice printMenu() {
    const static vector<string>& options{
        "Insert a number into the AVL tree",
        "Delete a number from the AVL tree",
        "Print the AVL tree",
        "Exit"
    };

    unsigned short response;
    cout << string(40, '-') << endl;

    for (size_t i{0}; i < options.size(); i++) {
        cout << (i + 1) << ". " << options[i] << endl;
    }

    cout << "Choose an option: ";
    cin >> response;

    if (not (1 <= response and response <= options.size())) {
        response = -1;
    }

    return static_cast<UserChoice>(response);
}

int main(){
    AVL<int> t;
    int value;
    UserChoice choice;

    while (true) {
        choice = printMenu();

        switch (choice) {
            case UserChoice::Invalid:
                cout << "Invalid choice, try again!" << endl;
                break;
            case UserChoice::Insert:
                cout << "Enter an integer to insert: ";
                cin >> value;
                t.insert(value);
                break;
            case UserChoice::Delete:
                cout << "Enter an integer to delete: ";
                cin >> value;
                t.remove(value);
                break;
            case UserChoice::Print:
                cout << endl << t << endl;
                break;
            case UserChoice::Exit:
                exit(0);
        }
    }
}
