#include "sqlite3.h"
#include <iostream>
#include <fstream>

using namespace std;

string getInput(const string &prompt)
{
	ofstream f;
	string s;
	std::cout << prompt;
	getline(cin, s);
	return s;
}

string chooseName(const string &task)
{
	ofstream f;
	string name;
	std::cout << std::string("Enter ") + task + " name: ";
	getline(cin, name);
	if (task == "database")
	{
		name += ".db";
	}
	return name;
}

static int makeTable(const string &tablename, const string &filename)
{
	sqlite3 *db;
	char *messageError;
	std::string table =
		std::string("CREATE TABLE IF NOT EXISTS ") +
		tablename +
		std::string("("
					"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
					"NAME       TEXT NOT NULL, "
					"TITLE      TEXT NOT NULL, "
					"NOTE     NVARCHAR(500) );");

	try
	{
		int exit = 0;
		exit = sqlite3_open(filename.c_str(), &db);
		exit = sqlite3_exec(db, table.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK)
		{
			std::cout << messageError << "test" << std::endl;
			sqlite3_free(messageError);
			return 1;
		}
		else
		{
			std::cout << "Table created successfully" << std::endl;
			return 0;
		}
	}
	catch (const exception &e)
	{
		cerr << e.what();
	}

	return 0;
}

static int makeDB(const string &filename)
{
	sqlite3 *db;
	int exit = 0;
	exit = sqlite3_open(filename.c_str(), &db);
	return 0;
}

static int insertData(const string &tablename, const string &filename)
{
	std::cout << '\n'
			  << "Insert Data: " << std::endl;
	string name = getInput("Enter name: ");
	string title = getInput("Enter title: ");
	string note = getInput("Enter note: ");

	sqlite3 *db;
	char *messageError;
	std::string data =
		std::string("INSERT INTO ") + tablename +
		std::string("(NAME, TITLE, NOTE) VALUES('") +
		name + "'" + ", '" + title + "'" + ", '" + note + "');";

	int exit = sqlite3_open(filename.c_str(), &db);
	exit = sqlite3_exec(db, data.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK)
	{
		std::cout << messageError << std::endl;
		sqlite3_free(messageError);
		return 1;
	}
	else
	{
		std::cout << "Data inserted successfully" << std::endl;
		return 0;
	}
}

//make attributelist responsive
static int updateData()
{
	string filename = chooseName("database");
	string table = getInput("Which table would you like to update? ");
	string attr = getInput("Which attribute would you like to update? (ID/NAME/TITLE/NOTE): ");

	string oldVal = getInput("What is the old value of the attribute? ");

	string newVal = getInput("Enter new value for attribute: ");
	// string otherAttr = getInput("Which (other) attribute would you like to use to find the row? (ID/NAME/TITLE/NOTE): ");
	// string otherAttrVal = getInput("Enter value of other attribute: ");

	sqlite3 *db;
	char *messageError;
	std::string update = std::string("UPDATE ") + table + std::string(" SET ") + attr + std::string(" = '") + 
	newVal + std::string("' WHERE ") + attr + std::string(" = ") + 
	std::string("'") + oldVal + std::string("'") + std::string(";");
	int exit = sqlite3_open(filename.c_str(), &db);
	exit = sqlite3_exec(db, update.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK)
	{
		std::cout << messageError << std::endl;
		sqlite3_free(messageError);
		return 1;
	}
	else
	{
		std::cout << "Data updated successfully" << std::endl;
		return 0;
	}
	return 0;
}

static int deleteData()
{
	string filename = chooseName("database");
	string table = getInput("Enter table to wipe: ");

	sqlite3 *db;
	char *messageError;
	std::string del = std::string("DELETE FROM ") + table + std::string(" ;");

	int exit = sqlite3_open(filename.c_str(), &db);
	exit = sqlite3_exec(db, del.c_str(), NULL, 0, &messageError);

	if (exit != SQLITE_OK)
	{
		std::cout << messageError << std::endl;
		sqlite3_free(messageError);
		sqlite3_close(db);
		return 1;
	}
	else
	{
		std::cout << "Wiped successfully" << std::endl;
		sqlite3_close(db);
		return 0;
	}
	return 0;
}

// this is useless
static int readData(const string &tablename, const string &filename)
{
	sqlite3 *db;
	char *messageError;
	std::string q = std::string("SELECT * FROM ") + tablename + std::string(" ;");
	int exit =
		sqlite3_open(filename.c_str(), &db);
	exit = sqlite3_exec(db, q.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK)
	{
		std::cout << messageError << std::endl;
		sqlite3_free(messageError);
		sqlite3_close(db);
		return 1;
	}
	else
	{
		std::cout << "Opened successfully" << std::endl;
		sqlite3_close(db);
		return 0;
	}
	return 0;
}

void handleDataInsert(const string &tablename, const string &filename)
{
	insertData(tablename, filename);
	bool t = false;
	while (!t)
	{
		std::cout << '\n'; 
		string choice = getInput("Continue inserting data? (Y/N): ");
		switch (choice[0])
		{
		case 'Y':
		case 'y':
		{
			insertData(tablename, filename);
			break;
		}
		case 'N':
		case 'n':
		{
			t = true;
			break;
		}
		default:
		{
			std::cout << "Invalid input" << std::endl;
			break;
		}
		}
	}
}

void handleTableInsert(const string &filename)
{
	bool cont = false;
	while (!cont)
	{
		string choice = getInput("Add table? (Y/N): ");
		switch (choice[0])
		{
		case 'Y':
		case 'y':
		{
			string tablename = chooseName("table");
			makeTable(tablename, filename);
			handleDataInsert(tablename, filename);
			break;
		}
		case 'N':
		case 'n':
		{
			cont = true;
			break;
		}
		default:
		{
			std::cout << "Invalid input" << std::endl;
			break;
		}
		}
	}
}

void controlFlow()
{
	string filename, tablename;
	bool done = false;
	while (!done)
	{
		string choice = getInput("C/R/U/D/E: ");
		switch (choice[0])
		{
		case 'C':
		case 'c':
		{
			filename = chooseName("database");
			makeDB(filename);
			handleTableInsert(filename);
			break;
		}
		case 'R':
		case 'r':
		{
			tablename = getInput("Which table would you like to read? ");
			readData(tablename, filename);
			break;
		}
		case 'U':
		case 'u':
		{
			updateData();
			break;
		}
		case 'D':
		case 'd':
		{
			deleteData();
			break;
		}
		case 'E':
		case 'e':
		{
			if (getInput("Exiting program--Delete local database file? (y/n): ") == "y" ) {
				string f = chooseName("database"); 
				system(("rm " + f).c_str());
				done = true;
				break;
			} 
			else
            {
			done = true;
			break;
			}
		}
		default:
		{
			std::cout << "Invalid input" << std::endl;
			break;
		}
		}
	}
}

int main()
{
	controlFlow();
	return 0;
}
