#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;

struct person
{
    int social;
    int birth_day;
    string first_name;
    string last_name;
    int zip_code;

    person(int ssn, int bd, string fn, string ln, int zip)
    {
        social = ssn;
        birth_day = bd;
        first_name = fn;
        last_name = ln;
        zip_code = zip;
    }
};

struct link
{
    person *info;
    link *next;

    link(person *i = NULL, link *n = NULL)
    {
        info = i;
        next = n;
    }
};

void print_list(link *content)
{
    while (content != NULL)
    {
        cout << content->info->social << " " << content->info->birth_day << " " << content->info->first_name << " " << content->info->last_name << " " << content->info->zip_code << "\n";
        content = content->next;
    }
}
person *find(link *content, string f, string l)
{
    while (content != NULL)
    {
        if (content->info->first_name == f && content->info->last_name == l)
            return content->info;
        content = content->next;
    }
    return NULL;
}

void save_to_file(link *content) {
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    std::ostringstream filename;
    filename << "database_" << timestamp.count() << ".txt";

    std::ofstream file(filename.str());
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing!" << std::endl;
        return;
    }

    while (content != NULL)
    {
        file << content->info->social << " " << content->info->birth_day << " " << content->info->first_name << " " << content->info->last_name << " " << content->info->zip_code << std::endl;
        content = content->next;
    }

    // Close the file
    file.close();

    std::cout << "Saved database to " << filename.str() << std::endl;
}

void sort_name(link *content)
{
    // check if list is empty or has only one element
    if (content == NULL || content->next == NULL) {
        return;
    }

    link *current = content;
    link *index = NULL;
    person *temp = NULL;

    // iterate through the list
    while (current != NULL) {
        index = current->next;
        while (index != NULL) {
            // compare last names first
            if (current->info->last_name > index->info->last_name) {
                temp = current->info;
                current->info = index->info;
                index->info = temp;
            }
            // if last names are equal, compare first names
            else if (current->info->last_name == index->info->last_name &&
                     current->info->first_name > index->info->first_name) {
                temp = current->info;
                current->info = index->info;
                index->info = temp;
            }
            index = index->next;
        }
        current = current->next;
    }
    cout<< "Sorted list based on name.\n";
    // Save the sorted list to a file
    save_to_file(content);
}

void delete_person(link *content, string f, string l) {
    link *prev = NULL;
    link *curr = content;
    
    while (curr != NULL)
    {
        if (curr->info->first_name == f && curr->info->last_name == l)
        {
            if (prev == NULL)
            {
                // removing the first node in the list
                content = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            delete curr->info;
            delete curr;
            cout << "Person deleted successfully.\n";
            save_to_file(content);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    cout << "Person not found with "<<f<<" and "<<l<<" in database.\n";
}

void find_all(link *content, string s)
{
    while (content != NULL)
    {
        if (content->info->last_name == s)
            cout << content->info->social << " " << content->info->birth_day << " " << content->info->first_name << " " << content->info->last_name << " " << content->info->zip_code << "\n";
        content = content->next;
    }
}

void find_zip(link *content, int a)
{
    link *allzip = NULL;
    while (content != NULL)
    {
        if (content->info->zip_code == a)
            allzip = new link(content->info, allzip);
        content = content->next;
    }
    print_list(allzip);
}

void kill_zip(link *content, int zipcode)
{
    if(content == NULL) {
        return;
    }

    while(content != NULL && content->info->zip_code == zipcode) {
        link *temp = content;
        content = content->next;
        delete temp;
    }

    // Handle remaining nodes
    link *curr = content;
    while (curr != NULL && curr->next != NULL) {
        if (curr->next->info->zip_code == zipcode) {
            link *temp = curr->next;
            curr->next = curr->next->next;
            delete temp;
        }
        else {
            curr = curr->next;
        }
    }
    cout << "Person found with "<< zipcode << "got deleted.\n";
    save_to_file(content);
}

int find_oldest(link *content)
{
    int oldest = content->info->birth_day;
    while (content != NULL)
    {
        if (content->info->birth_day < oldest)
            oldest = content->info->birth_day;
        content = content->next;
    }
    return oldest;
}

void sort_age(link *content) {
    bool swapped = true;
    while (swapped) {
        swapped = false;
        link* current = content;
        link* prev = nullptr;
        while (current != nullptr && current->next != nullptr) {
            if (current->info->birth_day < current->next->info->birth_day) {
                // swap the nodes
                link* next = current->next;
                current->next = next->next;
                next->next = current;
                if (prev == nullptr) {
                    content = next;
                } else {
                    prev->next = next;
                }
                prev = next;
                swapped = true;
            } else {
                prev = current;
                current = current->next;
            }
        }
    }
    cout<< "Sorted list based on age.\n";
    // Save the sorted list to a file
    save_to_file(content);
}

void read_file(string fileName, link *content)
{
    ifstream fin;
    fin.open(fileName);
    int ssn, bd, zip;
    string fn, ln;

    if (fin.fail())
    {
        cout << "could not open file"
             << "\n";
    }

    else
    {
        while (fin >> ssn >> bd >> fn >> ln >> zip)
        {
            person *p = new person(ssn, bd, fn, ln, zip);
            link *l = new link(p, content);
            content = l;
        }
    }
    fin.close();
    link *all = NULL;
    while (true)
    {
        cout << "\n Command list: "
             << "\n"
             << "Find - displays all information about the named person\n"
             << "\n"
             << "All - displays all information about all people who have the given last name\n"
             << "\n"
             << "Zip - make a new linked list of all people living in the given zip code, then displays the entire contents of that linked list.\n"
             << "\n"
             << "Oldest - print the name and zipcode of the oldest person in the database.\n"
             << "\n"
             << "Exit - exit from the program.\n"
             << "\n"
             << "Delete - remove the named person from the list.\n"
             << "\n"
             << "SAVE filename - save the entire database in the named file. Make sure the format is correct, you should be able to run your\n"
             "program again, this time giving it the new file name, and it being able to read it correctly. Also make sure that when\n"
             "you read the saved file, all the deleted people remain deleted.\n"
             << "\n"
             << "PRINT - print every detail of every person in the database.\n"
             << "\n"
             << "SORT NAME - sort the database linked list so that the people appear an alphabetical order based on their names.\n"
              "Sort primarily on last names, but when two people have the same last name, use their first names instead.\n"
             << "\n"
             << "SORT AGE - sort the database linked list so that the people appear an descending order of their ages (i.e. oldest first).\n"
             "If two people have the same birth date, it doesn't matter what order they appear in.\n"
             "Make sure that whenever you SAVE a sorted database, the people appear in their sorted order in the file you create.\n"
             << "\n"
             << "KILLZIP zipcode - remove everyone who has the given zip code from the database.\n"
             << "\n";
        cout << "\n";
        string command;
        cin >> command;

        if (command == "Find")
        {
            cout << "Enter first name."
                 << "\n";
            string first, last;
            cin >> first;
            cout << "Enter last name."
                 << "\n";
            cin >> last;
            person *p = find(content, first, last);
            if (p == NULL)
            {
                cout << "Person could not be found."
                     << "\n";
            }
            else
            {
                all = new link(p, all);
            }
            print_list(all);
        }

        if (command == "All")
        {
            cout << "Enter person's last name."
                 << "\n";
            string ln;
            cin >> ln;
            find_all(content, ln);
        }
        if (command == "Zip")
        {
            cout << "Enter desired zip code."
                 << "\n";
            int zc;
            cin >> zc;
            find_zip(content, zc);
        }

        if (command == "Oldest")
        {
            int oldest = find_oldest(content);
            while (content != NULL)
            {
                if (content->info->birth_day == oldest)
                    cout << "The oldest person is " << content->info->first_name << " " << content->info->last_name << " " << content->info->zip_code << "\n";
                content = content->next;
            }
        }

        if(command == "Delete")
        {
            cout << "Enter first name."
                 << "\n";
            string first, last;
            cin >> first;
            cout << "Enter last name."
                 << "\n";
            cin >> last;
            delete_person(content,first,last);
        }

        if(command == "Print") {
            print_list(content);
        }

        if(command == "Killzip") {
            cout << "Enter desired zip code."
                 << "\n";
            int zc;
            cin >> zc;
            kill_zip(content,zc);
        }

        if(command == "Sortage") {
            sort_age(content);
        }

        if(command == "Sortname") {
            sort_name(content);
        }

        if(command == "Save") {
            save_to_file(content);
        }

        if (command == "Exit")
        {
            break;
        }
        fin.close();
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " filename\n";
        exit(1);
    }

    string filename = argv[1];
    link *head = NULL;

    read_file(filename, head);
    save_to_file(head);
    return 0;
}
