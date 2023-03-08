//============================================================================
// Name        : ASKme.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

ifstream users_input("users.txt");
ofstream users_output("users.txt", ios::app);
ofstream questions_output("questions.txt"); // for asking questions
ofstream thread_questions_output("threadquestions.txt"); // for asking questions in thread


struct Question {
	static int question_id;
	string question;
	string answer;
	string parent_question_id;
	string From_user_id;
	string To_user_id;

	Question() {
		question_id ++;
	}
};

int Question::question_id = 0;
struct User {
	int user_id;
	string user_name;
	string password;
	string name;
	string email;
	bool allow_anonymous_questions;

	User() { };
	User(int _user_id, string _user_name, string _password, string _name, // for signup
			 string _email, bool _allow_anonymous_questions) {
		user_id = _user_id;
		user_name = _user_name;
		password = _password;
		name = _name;
		email = _email;
		allow_anonymous_questions = _allow_anonymous_questions;
	}


	void Ask_question() {
		string To_user_id_f, To_user_id, user_info;
		// user id
		cout << "Enter user id or -1 to cancel: ";
		cin >> To_user_id;
		if(To_user_id == "-1")
			return; // بتسلم الراية للmain تتصرف تشوف هتعمل ايه اوكيه

		// validation for user id check if allow anonymous questions
		// Check users.txt file   // problem
		while(getline(users_input, user_info)) {
			istringstream iss(user_info);
			iss >> To_user_id_f;

			if(To_user_id_f == To_user_id) {
				auto it = (user_info.end()) - 1; // access last position in the string
				if(!*it)
					cout << "Note: Anonymous questions are not allowed for this user";
			}
		}

		// In this part we assumed that we will go for -1 for now!
		int question_id_e;
		Question question; // generate id for each question

		cout << "For thread question, Enter Question id or -1 for new question: ";
		cin >> question_id_e;

		cout << "Enter question text: "; // problem
		getline(cin, question.question);

		// check if thread question or not
		if (question_id_e == -1) {
			// edit questions.txt file for non-thread questions
			questions_output << question.question_id << " " << user_id << " " << To_user_id
							 << " " << "0"  // by default not answered
							 << quoted(question.question); // to simplify operations on file
		}
		else { // هنفترض ان اليوزر هيدخل رقم سؤال موجود حاليا فمش هدور وراه فى ملف الاسئلة

			// edit threadquestions.txt file for non-thread questions
			thread_questions_output <<question_id_e << " " <<  question.question_id << " " << user_id << " " << To_user_id
							 << " " << "0"  // by default
							 << quoted(question.question);
		}
	}
};

bool SignUp() {
	// update users.txt with new users information
	static int user_id = 0;
	string user_name, password, name, email;
	bool allow_anonymous_questions;
	cout << "Enter user name. (no spaces) : ";
	cin  >> user_name;
	cout << "Enter password: ";
	cin  >> password;
	cout << "Enter name: ";
	cin  >> name;
	cout << "Enter email: ";
	cin  >> email;
	cout << "Allow anonymous questions? : (0 or 1)";
	cin >> allow_anonymous_questions;

	// Updating users.txt file // why appending a new line at the first time
	users_output << ++user_id << " " << user_name << " " << password << " "
				 << name << " " << email << " " << allow_anonymous_questions << "\n";

	return true; // مبدايا
}

bool Login() {
	// take user_name and password and check if they already exist in users.txt file
	// return true if the log-in was successful
	// return false otherwise
	bool is_valid_log = false;
	string user_name, password;

	cout << "Enter username & password: ";
	cin >> user_name >> password;

	string line;
	string user_name_f, password_f;
    // in each line extract username & password this way
	while(getline(users_input, line)) {
		istringstream iss(line);
		iss >> password_f >> user_name_f;
		iss >> password_f;

		// check
		if(user_name_f == user_name && password_f == password) {
			is_valid_log = true;
			return is_valid_log;
		}
	}

	 //to communicate the problem
	cout << "Invalid username or password!";
	return false;
}


int Menu() {  // called after signup or login operation // do internal checks
	int choice;
	cout << "Menu:\n";
	cout << "1: Print Questions To Me\n";
	cout << "2: Print Questions From Me\n";
	cout << "3: Answer Question\n";
	cout << "4: Delete Question\n";
	cout << "5: Ask Question\n";
	cout << "6: List System Users\n";
	cout << "7: Feed\n";
	cout << "8: Logout\n";
	do {
		cout << "Enter number in range 1-8: ";
		cin >> choice;
	}

	while (choice < 1 || choice > 8);
	return choice;

}

int AskMe() {
	// return an integer that represents what type of log operation was required
	 //does internal checks
	cout << "1: Login\n";
	cout << "2: Sign Up\n";
	int choice;

	do {
	cout << "Enter number in range 1-2: ";
	cin >> choice;
	}
	while (choice < 1 || choice >2);

	return choice;
}


int main() {
	if(users_input.fail())  { // for  login purpose
		cout << "can't open users.txt!";
	}

	if(questions_output.fail()) {
		cout << "can't open questions.txt!";
	}

	if(thread_questions_output.fail()) {
		cout << "can't open threadquestions.txt\"!";
	}

	int log_choice = AskMe();


	if (log_choice == 1) {
		if (Login()) { // case from 4 cases
			int choice = Menu();
			if (choice == 5) {
				User user;
				user.Ask_question();
			}


		}
	}

	else {
		if(SignUp()) {
			int choice = Menu();
			if (choice == 5) {
				User user;
				user.Ask_question();
			}
		}
	}


//			else if (choice == 2)
//			else if (choice == 3)
//			else if (choice == 4)
//			else if (choice == 5)
//			else if (choice == 6)
//			else if (choice == 7)
//			else {
//
//			}
//		}
//		Login();
//	}
	// script to check fin & cin
//	ifstream fin ("users.txt");
//	if (fin.fail()) {
//		cout << "can't open file";
//		return 0;
//	}
//	string To_user_id_f;
//	fin >> To_user_id_f;
//	cout << To_user_id_f;
//	string user_info;
//	getline(fin, user_info);
//	cout << user_info;

  return 0;
}
