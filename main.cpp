#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<algorithm>
#include<iomanip>
#include<limits>

using namespace std;

const string BOOKS_FILE = "books.txt";
const string MEMBERS_FILE = "members.txt";
const string TRANSACTIONS_FILE = "transactions.txt";

vector<string> splitCSV(const string &line) {
	vector<string> tokens;
	stringstream ss(line);
	string item;
	while (getline(ss, item, ','))tokens.push_back(item);
	return tokens;
}

int readInt(const string &prompt) {
	int value;
	while (true) {
	cout << prompt;
	if(cin >> value) {
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	return value;
	}
	if (cin.eof()) {
	cout << "\nNo more input detected. Exiting.\n";
	exit(0);
	}
	cout << "Invalid input. Please enter a number.\n";
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(),'\n');
	}
}

class Book {
	public:
		int id;
		string title;
		string author;
		int totalCopies;
		int availableCopies;

		Book(): id(0), totalCopies(0), availableCopies(0) {}

		Book(int id_, string title_, string author_, int total_, int available_):id(id_), title(move(title_)), author(move(author_)),totalCopies(total_), availableCopies(available_) {}

		string toCSV() const {
			stringstream ss;
			ss << id << "," << title << "," << author << "," << totalCopies << "," << availableCopies;
			return ss.str();
		}

		static Book fromCSV(const vector<string> &t) {
			return Book(stoi(t[0]), t[1], t[2], stoi([3]), stoi(t[4]));
		}

		void display() const {
			cout << left << setw(5) << id << setw(30) << title << setw(20) << author << setw(10) << totalCopies << setw(10) << availableCopies << "\n";
		}
};

class Member {
public:
	int id;
	string name;
	string email;

	Member(): id(0) {}
	Member(int id_, string name_, string email_)
		:id(id_), name(move(name_)), email(move(email_)) {}
	string toCSV() const {
		stringstream ss;
		ss << id << "," << name << "," << email;
		return ss.str();
	}

	static Member fromCSV(const vector<string> &t) {
		return Member(stoi(t[0]),t[1],t[2]);
	} 

	void display() const {
	cout << left << setw(5) << id << setw(20) << name << setw(30) << email << "\n";
	}
};

class Transaction {
public:
	int bookId;
	int memberId;
	string status;

	Transaction(): bookId(0), memberId(0) {}
	Transaction(int bId, int mId, string st)
		:bookId(bId), memberId(mId), status(move(st)) {}

	string toCSV() const {
		stringstream ss;
		ss << bookId << "," << memberId << "," << status;
		return ss.str();
	}

	static Transaction fromCSV(const vector<string> &t) {
	return Transaction(stoi(t[0]), stoi(t[1]), t[2]);
	}
};

class Library {
private:
	vector<Book> books;
	vector<Member> members;
	vector<Transaction> transaction;

	int nextBookId = 1;
	int nextMemberId = 1;

public:
	Library() {
	loadBooks();
	loadMembers();
	loadTransactions();
	}

	void loadBooks() {
	ifstream fin(BOOKS_FILE);
	string line;
	while (getline(fin, line)) {
		if(line.empty()) continue;
		Book b = Book::fromCSV(splitCSV(line));
		books.push_back(b);
		nextBookId = max(nextBookId, b.id + 1);
	}
	}

	void loadMembers() {
		if stream fin(MEMBERS_FILE);
		string line;
		while (getline(fin, line)) {
			if (line.empty()) continue;
			Member m = Member::fromCSV(splitCSV(line));
			members.push_back(m);
			nextMemberId = max(nextMemberId, m.Id + 1);
		}
	}

	void loadTransactions() {
		ifstream fin(TRANSACTIONS_FILE);
		string line;
		while(getline(fin, line)) {
			if (line.empty()) continue;
			transactions.push_back(Transaction::fromCSV(splitCSV(line)));
		}
	}

	void saveBooks() {
		ofstream fout(BOOKS_FILE);
		for (auto &b:books) fout << b.toCSV() << "\n";
	}

	void saveMembers() {
		ofstream fout(MEMBERS_FILE);
		for (auto &m:members) fout << m.toCSV() << "\n";
	}

	void saveTransactions() {
		ofstream fout(TRANSACTIONS_FILE);
		for(auto &t:transactions) fout << t.toCSV() << "\n";
	}
	
	void addBook() {
		string title, author;
		cout << "Enter Book Title:";
		getline(cin, title);
		cout << "Enter Author Name:";
		getline(cin, author);
		int copies = readInt("Enter Number of Copies:");

		Book b(nextBookId++, title, author, copies, copies);
		books.push_back(b);
		saveBooks();
		cout << "Book added successfully! (Book ID: " <<b.id <<")\n"; 
	}

	void displayAllBooks() const {
		if (books.empty()) { cout << "No books in the library. \n"; return;}
		cout << "\n" << left << setw(5) << "ID" << setw(30) << "Title" << setw(20) << "Author" << setw(10) << "Total" << setw(10) << "Available" << "\n";
		cout << string(75, '-') << "\n";
		for (auto &b:books) b.display();
	}
	
	Book*findBookById(int id) {
		for(auto &b:books) if(b.id == id) return &b;
			return nullptr;
	}

	void addMember() {
		string name, email;
		cout<< "Enter Member Name:";
		getline(cin, name);
		cout << "Enter Member Email:";
		getline(cin, email);

		Member m(nextMemberID++, name , email);
		members.push_back(m);
		saveMembers();
		cout << "Member added successfully! (Member ID:" << m.id << ")\n";
	}

	void displayAllMembers() const {
		if (members.empty()) {cout << "No members registered.\n"; return;}
		cout << "\n" << left << setw(5) << "ID" << setw(20) << "Name" << setw(30) << "Email" << "\n";
		cout << string(55, '-') << "\n";
		for (auto &m:members) if (m.id == id) return &m;
			return nullptr;
	}

	void issueBook() {
		if (books.empty() || members.empty()) {
			cout << "Add atleast one book and one member first.\n";
			return;
		}

		int bookId = readInt("Enter Book ID to issue:");
		int memberId = readInt("Enter Member ID:");

		Book*book = findBookById(bookId);
		Member*member = findMemberById(memberId);

		if(!book) {cout << "Book ID not found.\n"; return;}
		if(!member) {cout << "Member ID not found.\n"; return;}

		if(book->availableCopies <= 0) {
			cout << "Sorry, no available copies of \"" << book->title << "\" right now.\n";
			return;
		}

		book->availableCopies--;
		transactions.push_back(Transaction(bookId, memberId, "ISSUED"));
		saveBooks();
		saveTransactions();

		cout << "Book\""<< book->title <<"\"issued to " << member->name << "successfully.\n";
	}

	void retunBook() {
		int bookId = readInt("Enter Book ID to return:");
		int memberId = readint("Enter Member ID:");

		Book*book = findBookById(bookId);
		Member*member = findMemberById(memberId);

		if(!book) {cout << "Book ID not found.\n"; return; }
		if(!member) {cout << "member ID not found.\n"; return;}

		for (int i=(int)transactions.size()-1;i>=0;--i) {
			Transaction &t = transactions[i];
			if(t.bookId == bookId && t.memberId == memberId && t.status == "ISSUED") {
				t.status = "RETURNED";
				book->availableCopies++;
				saveBooks();
				cout << "Book \"" << book->title << "\" returned by " << member->name << "successfully. \n";
				return;
			}
		}
		cout << "No matching active issue record found for this Book ID / member ID.\n";
	}

	void displayActiveIssues() const {
		bool any = false;
		cout << "\n" << left << setw(10) << "BookID" << setw(12) << "MemeberID" << setw(10) << "Status" << "\n";
		cout << string(32, '-') << "\n";
		for (auto &t:transactions) {
			if(t.status == "ISSUED") {
				cout << left << setw(10) << t.bookId << setw(12) << t.memberId << setw(10) << t.status << "\n";
				any = true;
			}
		}

		if(!any) cout << "No books currently issued. \n";
	}

	string toLower(const string &s) const {
		string r = s;
		transform(r.begin(),r.end(),r.begin(),::tolower);
		return r;
	}

	void searchByTitle() const {
		string query;
		cout << "Enter title (or part of it) to search:";
		getline(cin, query);
		string q = toLower(query);

		bool found = false;
		cout << "\n" << left <<setw(5) << "ID" << setw(30) << "Title" << setw(20) << "Author" << setw(10) << "Total" << setw(10) << "Available" << "\n";
		cout << string(75,'-') << "\n";
		for(auto &b:books) {
			if(toLower(b.title).find(q)!=string::npos) {
				b.display();
				found=true;
			}
		}

		if(!found) cout << "No books found matching title \"" << query << "\".\n";
	}

	void searchByAuthor() const {
		string query;
		cout << "Enter author name (or part of it) to search:";
		getline(cin, query);
		string q = toLower(query);

		bool found = false;
		cout << "\n" << left << setw(5) << "ID" << setw(30) << "Title" << setw(20) << "Author" << setw(10) << "Total" << setw(10) << "Available" << "\n";
		cout << string(75, '-') << "\n";
		for(auto &b: books) {
			if(toLower(b.author).find(q)!=string::npos) {
				b.display();
				found = true;
			}
		}
		if (!found) cout << "No books found matching author\"" << query << "\".\n";
	}
};

void printMenu() {
	cout << "\n======================LIBRARY MANAGEMENT SYSTEM=======================\n";
	cout << "1. Add Book\n";
	cout << "2. Display All Books\n";
	cout << "3. Add members\n";
	cout << "4. Display All Members\n";
	cout << "5. Issue Book\n";
	cout << "6. Return Book\n";
	cout << "7. Search Book by Title\n";
	cout << "8. Search Book by Author\n";
	cout << "9. Show Currently Issued Books\n";
	cout << "0. Exit\n";
	cout << "========================================================================\n";
}

int main() {
	Library library;
	int choice;

	cout << "Welcome to the Library Management System!\n";
	cout << "(Data is automatically saved to books.txt, members.txt, transactions.txt)\n";

	do {
		printMenu();
		choice = readInt("Enter your choice:");

		switch (choice) {
		case 1:library.addBook(); break;
		case 2:library.displayAllBook(); break;
		case 3:library.addMember(); break;
		case 4:library.displayAllMembers(); break;
		case 5:library.issueBook(); break;
		case 6:library.returnBook(); break;
		case 7:library.searchByTitle(); break;
		case 8:library.searchByAuthor(); break;
		case 9:library.displayActiveIssues(); break;
		case 0:cout << "Thank you for using the Library Management System. Goodbye!\n"; break;
		default: cout << "Invalid choice. Please try again.\n";
		}
	}
	while (choice != 0);

	return 0;
}
