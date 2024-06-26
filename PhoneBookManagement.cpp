#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

class PhonebookEntry {
public:
    string name;
    string phone;

    PhonebookEntry() = default;

    PhonebookEntry(const string& name, const string& phone){
        this->name = name;
        this->phone = phone;
    } 
};

// Define the structure for a node in the linked list
class Node {
public:
    PhonebookEntry entry;
    unique_ptr<Node> next;

    Node(const PhonebookEntry& entry) {
        this->entry = entry;
        this->next = NULL;
    }
};

// Define the Phonebook class
class Phonebook {
private:
    unique_ptr<Node> head;

public:
    void addEntry(const PhonebookEntry& entry) {
        auto newNode = make_unique<Node>(entry);
        newNode->next = move(head);
        head = move(newNode);
    }

    void deleteEntry(const string& name) {
        Node* temp = head.get();
        Node* prev = nullptr;

        // If the head node itself holds the key to be deleted
        if (temp != nullptr && temp->entry.name == name) {
            head = move(temp->next);
            return;
        }

        // Search for the key to be deleted
        while (temp != nullptr && temp->entry.name != name) {
            prev = temp;
            temp = temp->next.get();
        }

        // If key was not present in linked list
        if (temp == nullptr) return;

        // Unlink the node from linked list
        prev->next = move(temp->next);
    }

    Node* searchEntry(const string& name) const {
        Node* current = head.get();
        while (current != nullptr) {
            if (current->entry.name == name)
                return current;
            current = current->next.get();
        }
        return nullptr;
    }

    void updateEntry(const string& name, const string& newPhone) {
        Node* entryNode = searchEntry(name);
        if (entryNode != nullptr) {
            entryNode->entry.phone = newPhone;
            cout << "Entry updated successfully.\n";
        } else {
            cout << "Entry not found.\n";
        }
    }

    void displayPhonebook() const {
        Node* current = head.get();
        while (current != nullptr) {
            cout << "Name: " << current->entry.name << ", Phone: " << current->entry.phone << endl;
            current = current->next.get();
        }
    }

    void sortPhonebook() {
        // Convert linked list to vector for sorting
        vector<PhonebookEntry> entries;
        Node* current = head.get();
        while (current != nullptr) {
            entries.push_back(current->entry);
            current = current->next.get();
        }

        // Sort the vector by name
        sort(entries.begin(), entries.end(), [](const PhonebookEntry& a, const PhonebookEntry& b) {
            return a.name < b.name;
        });

        // Rebuild the linked list from the sorted vector
        head.reset();
        for (auto it = entries.rbegin(); it != entries.rend(); ++it) {
            addEntry(*it);
        }
        cout << "Phonebook sorted successfully.\n";
    }

    void saveToFile(const string& filename) const {
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "Could not open file for writing.\n";
            return;
        }

        Node* current = head.get();
        while (current != nullptr) {
            outFile << current->entry.name << " " << current->entry.phone << "\n";
            current = current->next.get();
        }
        outFile.close();
        cout << "Phonebook saved to " << filename << " successfully.\n";
    }
};

int main() {
    Phonebook phonebook;
    PhonebookEntry entry;
    int choice;
    string name, phone, filename;

    while (true) {
        cout << "\nPhonebook Management System\n";
        cout << "1. Add Entry\n";
        cout << "2. Delete Entry\n";
        cout << "3. Search Entry\n";
        cout << "4. Update Entry\n";
        cout << "5. Display Phonebook\n";
        cout << "6. Sort Phonebook\n";
        cout << "7. Save Phonebook to File\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter name: ";
                cin >> entry.name;
                cout << "Enter phone number: ";
                cin >> entry.phone;
                phonebook.addEntry(entry);
                cout << "Entry added successfully.\n";
                break;

            case 2:
                cout << "Enter name to delete: ";
                cin >> name;
                phonebook.deleteEntry(name);
                cout << "Entry deleted successfully (if it existed).\n";
                break;

            case 3:
                cout << "Enter name to search: ";
                cin >> name;
                {
                    Node* searchResult = phonebook.searchEntry(name);
                    if (searchResult != nullptr) {
                        cout << "Found: Name: " << searchResult->entry.name << ", Phone: " << searchResult->entry.phone << endl;
                    } else {
                        cout << "Entry not found.\n";
                    }
                }
                break;

            case 4:
                cout << "Enter name to update: ";
                cin >> name;
                cout << "Enter new phone number: ";
                cin >> phone;
                phonebook.updateEntry(name, phone);
                break;

            case 5:
                cout << "Phonebook:\n";
                phonebook.displayPhonebook();
                break;

            case 6:
                phonebook.sortPhonebook();
                break;

            case 7:
                cout << "Enter filename to save: ";
                cin >> filename;
                phonebook.saveToFile(filename);
                break;

            case 8:
                cout << "Exiting...\n";
                return 0;

            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}
