#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

struct User {
    string username;
    string password;
    
};

struct Question {
    string questionText;
    vector<string> options;
    char correctOption;
    char userChoice;  // Added to store the user's choice
    
};

class CBTestSystem {
private:
    vector<Question> questions;
    vector<size_t> skippedQuestions;  // Track indices of skipped questions
    int score;
    vector<User> users;  // Vector to store user login details
    User currentUser;

public:
    CBTestSystem() : score(0) {
        // Load user login details from CSV file
        users = readUserLoginDetails("C:\\Users\\User\\Documents\\user_login.txt");
        // Load questions from CSV file
        questions = readQuestions("C:\\Users\\User\\Documents\\questions.txt");
    }

    vector<User> readUserLoginDetails(const string& filename) {
        vector<User> users;

        ifstream file(filename);

        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return users;
        }

        string line;
        getline(file, line);  // Read and discard the header line

        while (getline(file, line)) {
            stringstream ss(line);
            string username, password;

            getline(ss, username, ',');
            getline(ss, password, ',');

            while (!username.empty() && username.back() == ' ')
                username.pop_back();
            while (!password.empty() && password.back() == ' ')
                password.pop_back();

            users.push_back({username, password});
        }

        file.close();

        return users;
    }

    vector<Question> readQuestions(const string& filename) {
        vector<Question> loadedQuestions;

        ifstream file(filename);

        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return loadedQuestions;
        }

        string line;
        getline(file, line);  // Read and discard the header line

        while (getline(file, line)) {
            stringstream ss(line);
            Question q;

            getline(ss, q.questionText, ',');

            for (char option = 'A'; option <= 'D'; ++option) {
                string optionText;
                getline(ss, optionText, ',');
                string optionValue;
                optionValue += option;
                optionValue += ".";
                optionValue += optionText;
                q.options.push_back(optionValue);
            }

            string correctOption;
            getline(ss, correctOption, ',');

            if (correctOption.length() == 1) {
                q.correctOption = correctOption[0];  // Extract the first character
            } else {
                // Handle the error or continue with a default value
                q.correctOption = 'A';
            }

            loadedQuestions.push_back(q);
        }

        file.close();

        return loadedQuestions;
    }

 bool login() {
    int attempts = 0;
    const int maxAttempts = 20;  // Maximum number of login attempts

    do {
        cout << "Enter your username: ";
        cin >> currentUser.username;

        cout << "Enter your password: ";
        cin >> currentUser.password;

        // Check if the entered credentials are valid
        for (const User& user : users) {
            if (currentUser.username == user.username && currentUser.password == user.password) {
                cout << "Login successful!\n";
                return true;
            }
        }

        cout << "Invalid username or password. reinput login details\n";
        attempts++;

        

    } while (attempts < maxAttempts);

    return false;
}


    void displayQuestion(const Question& q) const {
        cout << q.questionText << "\n";
        for (const auto& option : q.options) {
            cout << option << "\n";
        }
        cout << "Your choice (or 'S' to skip): ";
    }

    char getUserChoice() const {
        char userChoice;
        cin >> userChoice;
        return toupper(userChoice);  // Convert to uppercase for case-insensitive comparison
    }
void viewTestScript() const {
    cout << "Test Script:\n\n";
    for (size_t i = 0; i < questions.size(); ++i) {
        const Question& question = questions[i];

        cout << "Question " << i + 1 << ":\n";
        cout << question.questionText << "\n";

        // Display options
        for (const auto& option : question.options) {
            cout << option << "\n";
        }

        cout << "Correct Option: " << question.correctOption << "\n";
        cout << "Your Choice: " << (question.userChoice == '\0' ? "Skipped" : string(1, question.userChoice)) << "\n";
        cout << "----------------\n";
    }
}

    void conductTest() {
        for (size_t i = 0; i < questions.size(); ++i) {
            const Question& question = questions[i];

            // Skip the question if 'S' is selected
            

            displayQuestion(question);

            char userChoice = getUserChoice();

            while (!isValidOption(userChoice)) {
                cout << "Invalid choice. Please enter a valid option or 'S' to skip.\n";
                userChoice = getUserChoice();
            }

            questions[i].userChoice = userChoice;

            if (userChoice == 'S') {
                cout << "Question skipped.\n";
                skippedQuestions.push_back(i);  // Track the index of the skipped question
            }
            if (userChoice == question.correctOption)
                score++;

            cout << "----------------\n";
        }

        cout << "Test complete. Your score: " << score << " out of " << questions.size() << "\n";

        // Offer the option to revisit skipped questions
        revisitSkippedQuestions(); 
        
        // Offer the option to view test scripts
        viewTestScript();
    }

    bool isValidOption(char option) const {
        return option == 'A' || option == 'B' || option == 'C' || option == 'D' || option == 'S';
    }

    

    void revisitSkippedQuestions() {
        if (!skippedQuestions.empty()) {
            cout << "You have skipped questions. Do you want to revisit them? (Y/N): ";
            char revisitChoice;
            cin >> revisitChoice;

            if (toupper(revisitChoice) == 'Y') {
                for (size_t skippedIndex : skippedQuestions) {
                    const Question& skippedQuestion = questions[skippedIndex];
                    displayQuestion(skippedQuestion);
                    char userChoice = getUserChoice();

                    while (!isValidOption(userChoice)) {
                        cout << "Invalid choice. Please enter a valid option or 'S' to skip.\n";
                        userChoice = getUserChoice();
                    }

                    questions[skippedIndex].userChoice = userChoice;

                    if (userChoice == 'S') {
                        cout << "Question skipped.\n";
                    } else if (userChoice == skippedQuestion.correctOption) {

                        score++;
                    }


                    cout << "----------------\n";
                }

                cout << "Revisited skipped questions. Your final score: " << score << " out of " << questions.size() << "\n";
            }
        }
    }
};

int main() {
    CBTestSystem cbtSystem;

    if (cbtSystem.login()) {
        cbtSystem.conductTest();
    }

    return 0;
}
