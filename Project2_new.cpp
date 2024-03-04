//============================================================================
// Name        : Project2_new.cpp
// Author      : Ethan Mayberry
// Version     : 1.0
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <vector>

using namespace std;

//default time for sleep
const int GLOBAL_SLEEP_TIME = 5000;

struct Course {

    string cId;
    string cName;
    vector<string> preReq;
};

class BSTree {

private:
    // Define structures to hold courses
    struct Node {
        Course course;
        Node* right;
        Node* left;

        
        Node() {
            left = nullptr;
            right = nullptr;
        }

        
        Node(Course Acourse) {
            course = Acourse;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* Root;
    void inOrder(Node* node);
    int size = 0;

public:
    BSTree();
    void InOrder();
    void Insert(Course Acourse);
    void Remove(string cId);
    Course Search(string cId);
    int Size();
};

BSTree::BSTree() {
    this->Root = nullptr;
}

void BSTree::InOrder() {
    inOrder(Root);
}


void BSTree::Insert(Course Acourse) {

    Node* currNode = Root;

    if (Root == NULL) {

        Root = new Node(Acourse);

    }
    else {

        while (currNode != NULL) {

            if (Acourse.cId < currNode->course.cId) {

            if (currNode->left == nullptr) {

                    currNode->left = new Node(Acourse);
                    currNode = NULL;
                }
                else {

                    currNode = currNode->left;
                }
            }
            else {

            if (currNode->right == nullptr) {

                    currNode->right = new Node(Acourse);
                    currNode = NULL;
                }
             else {

                 currNode = currNode->right;
                }
            }
        }
    }
    size++;
}

//Remove a Course
 
void BSTree::Remove(string cId) {
    Node* par = NULL;
    Node* current = Root;

    while (current != NULL) {

        if (current->course.cId == cId) {
            if (current->left == NULL && current->right == NULL) {
                if (par == NULL) {
                    Root = nullptr;
                }
                else if (par->left == current) {
                    par->left = NULL;
                }
                else {
                    par->right = NULL;
                }
            }
            else if (current->right == NULL) {
                if (par == NULL) {
                    Root = current->left;
                }
                else if (par->left == current) {
                    par->left = current->left;
                }
                else {
                    par->right = current->left;
                }
            }
            else if (current->left == NULL) {
                if (par == NULL) {
                    Root = current->right;
                }
                else if (par->left == current) {
                    par->left = current->right;
                }
                else {
                    par->right = current->right;
                }
            }
            else {
                Node* succ = current->right;

                while (succ->left != NULL) {
                    succ = succ->left;

                }

                Node successData = Node(succ->course);
                Remove(succ->course.cId);
                current->course = successData.course;
            }
            cout << "\nNode found and removed" << endl;
            return;
        }
        else if (current->course.cId < cId) {
            par = current;
            current = current->right;
        }
        else {
            par = current;
            current = current->left;
        }
    }
    cout << "\n No value found" << endl;
    return;
}

//Search for course
 
Course BSTree::Search(string cId) {

    Course Acourse;

    Node* currNode = Root;

    while (currNode != NULL) {

        if (currNode->course.cId == cId) {

            return currNode->course;
        }
        else if (cId < currNode->course.cId) {

            currNode = currNode->left;
        }
        else {

            currNode = currNode->right;
        }
    }

    //not found
    cout << "No value found." << endl;
    return Acourse;
}

void BSTree::inOrder(Node* node) {

    if (node == NULL) {

        return;
    }
    inOrder(node->left);

    //print node
    cout << node->course.cId << ", " << node->course.cName << endl;

    inOrder(node->right);
}

int BSTree::Size() {

    return size;
}


vector<string> Split(string lineFeed) {

    char delim = ',';

    lineFeed += delim;
    vector<string> lineTokens;
    string temp = "";
    for (int i = 0; i < lineFeed.length(); i++)
    {
        if (lineFeed[i] == delim)
        {
            lineTokens.push_back(temp);
            temp = "";
            i++;
        }
        temp += lineFeed[i];
    }
    return lineTokens;
}


void loadCourses(string csvPath, BSTree* courseList) {

    ifstream inFS; //insteam to read file
    string line;
    vector<string> stringTokens;

    inFS.open(csvPath); //open the read file

    if (!inFS.is_open()) {
        cout << "Could not open. Check inputs. " << endl;
        return;
    }

    while (!inFS.eof()) {

        Course aCourse;//create a new struct for each "line"

        getline(inFS, line);
        stringTokens = Split(line); 

        if (stringTokens.size() < 2) { 

            cout << "\nError. line skipped." << endl;
        }

        else {

            aCourse.cId = stringTokens.at(0);
            aCourse.cName = stringTokens.at(1);

            for (unsigned int i = 2; i < stringTokens.size(); i++) {

                aCourse.preReq.push_back(stringTokens.at(i));
            }

            // push course to the end
            courseList->Insert(aCourse);
        }
    }

    inFS.close(); //close file
}

//Course passed in
//Prints:
//cId, cName
//Prereq list (if none print "None found.")
void displayCourse(Course Acourse) {

    cout << Acourse.cId << ", " << Acourse.cName << endl;
    cout << "Prerequisites: ";

    if (Acourse.preReq.empty()) {//if the lsit is empty then there are no prerequisites

        cout << "None found." << endl;
    }
    else {

        for (unsigned int i = 0; i < Acourse.preReq.size(); i++) {

            cout << Acourse.preReq.at(i);

            if (Acourse.preReq.size() > 1 && i < Acourse.preReq.size() - 1) {
                //put a comma for any elements greater than 1

                cout << ", ";
            }
        }
    }

    cout << endl;
}

//Convert to Uppercase

void convertCase(string& toConvert) {

    for (unsigned int i = 0; i < toConvert.length(); i++) {

        if (isalpha(toConvert[i])) {

            toConvert[i] = toupper(toConvert[i]);
        }
    }
}

int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, AcourseKey;

    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        AcourseKey = argv[2];
        break;
    default:
        csvPath = "Advising Program Input.csv";
    }

    // Define table to hold all courses
    BSTree* courseList = new BSTree();

    Course course;
    bool inputGood;
    int option = 0;

    while (option != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  4. Remove Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";

        AcourseKey = ""; //clear the string        
        string aKey = " "; //clear the string
        option = 0; //clear the option

        try {
            cin >> option;

            if ((option > 0 && option < 5) || (option == 9)) {
                // limit the user menu inputs to good values
                inputGood = true;
            }
            else {
                //throw error for catch
                inputGood = false;
                throw 1;
            }

            switch (option) {
            case 1:

                // call to load the courses
                loadCourses(csvPath, courseList);
                cout << courseList->Size() << " courses read" << endl;

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 2:
                courseList->InOrder();

                cout << "\nEnter \'y\' to continue..." << endl;

                cin >> aKey;

                break;

            case 3:

                cout << "\nWhich course would you like to learn more about? " << endl;
                cin >> AcourseKey;

                convertCase(AcourseKey); //convert to Uppercase

                course = courseList->Search(AcourseKey);

                if (!course.cId.empty()) {
                    displayCourse(course);
                }
                else {
                    cout << "\nCourse ID " << AcourseKey << " was not found." << endl;
                }

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 4:

                cout << "\nWhich course should be deleted? " << endl;
                cin >> AcourseKey;

                convertCase(AcourseKey); //convert to Uppercase

                courseList->Remove(AcourseKey);

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 9:
                exit;
                break;

            default:

                throw 2;
            }
        }

        catch (int error) {

            std::cout << "\nCheck your input." << endl;
            Sleep(GLOBAL_SLEEP_TIME);
        }

        //need to clear the operator of extra input
        cin.clear();
        cin.ignore();

        //clear the consolse to produce menu
        system("cls");
    }

    cout << "Goodbye." << endl;

    Sleep(GLOBAL_SLEEP_TIME);

    return 0;
}