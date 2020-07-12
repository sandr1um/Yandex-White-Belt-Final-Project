//============================================================================
// Name        : DataBase.cpp
// Author      : Alexander Ivanets
// Version     :
// Copyright   : Your copyright notice
// Description : C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <string>
using namespace std;

class Date {
public:
    Date(const int& y, const int& m, const int& d) {
    	if (m < 1 || m > 12) {
    		throw invalid_argument("Month value is invalid: " + to_string(m));
    	}
    	if (d < 1 || d > 31) {
    		throw invalid_argument("Day value is invalid: " + to_string(d));
    	}
    	year = y;
    	month = m;
    	day = d;
    }

    Date() {}
	int GetYear() const { return year; }
	int GetMonth() const { return month; }
	int GetDay() const { return day; }
private:
	int year;
	int month;
	int day;
};

bool operator < (const Date& left, const Date& right) {
	if (left.GetYear() != right.GetYear()) {
		return left.GetYear() < right.GetYear();
	} else {
		if (left.GetMonth() != right.GetMonth()) {
			return left.GetMonth() < right.GetMonth();
		} else {
			return left.GetDay() < right.GetDay();
		}
	}
}

ostream& operator << (ostream& stream, const Date& date) {
	stream << setfill('0') << setw(4) << date.GetYear() << "-"
		   << setfill('0') << setw(2) << date.GetMonth() << "-"
		   << setfill('0') << setw(2) << date.GetDay();
	return stream;
}

Date ParseDate(const std::string& date)
{
    std::istringstream date_stream(date);
    bool flag = true;

    int year;
    flag = flag && (date_stream >> year);
    flag = flag && (date_stream.peek() == '-');
    date_stream.ignore(1);

    int month;
    flag = flag && (date_stream >> month);
    flag = flag && (date_stream.peek() == '-');
    date_stream.ignore(1);

    int day;
    flag = flag && (date_stream >> day);
    flag = flag && date_stream.eof();

    if (!flag)
    {
        throw std::logic_error("Wrong date format: " + date);
    }
    return Date(year, month, day);
}

class Database {
public:
	void AddEvent(const Date& date, const string& event) {
		if (!event.empty()) {
			data[date].insert(event);
		}
	}

	bool DeleteEvent(const Date& date, const string& event) {
		if (data.count(date) > 0 && data[date].count(event) > 0) {
			data[date].erase(event);
			return true;
		} else {
			return false;
		}
	}

	int DeleteDate(const Date& date) {
		int N = 0;
		if (data.count(date) > 0) {
			N = data.at(date).size();
			data.erase(date);
		}
		return N;
	}

	set<string> Find(const Date& date) const {
		set<string> result;
		if (data.count(date) > 0) {
			result = data.at(date);
		}
			return result;
	}

	void Print() const {
		for (const auto& d : data) {
			for (const auto& s : d.second) {
				cout << d.first << ' ' << s << endl;
			}
		}
	}
private:
	map<Date, set<string>> data;
};

int main() {
  try {
	  Database db;
	  string command;
	  while (getline(cin, command)) {
		  if (command == "") continue;

		  stringstream input(command);

		  string operation;
		  input >> operation;

		  if (operation == "Add") {
			  string date_string, event;
			  input >> date_string >> event;
			  const Date date = ParseDate(date_string);
			  if (event != "" || event != " ") {
				  db.AddEvent(date, event);
			  }
		  } else if (operation == "Del") {
			  string date_string, event;
			  input >> date_string;
			  if (!input.eof()) {
				  input >> event;
			  }

			  const Date date = ParseDate(date_string);

			  if (event.empty()) {
				  const int EventsToDel = db.DeleteDate(date);
				  cout << "Deleted " << EventsToDel << " events" << endl;
			  } else {
				  if (db.DeleteEvent(date, event)) {
					  cout << "Deleted successfully" << endl;
				  } else {
					  cout << "Event not found" << endl;
				  }
			  }
		  } else if (operation == "Find") {
			  string date_string, event;
			  input >> date_string, event;
			  const Date date = ParseDate(date_string);
			  set<string> events = db.Find(date);

			  for (const string& s : events) {
				  cout << s << endl;
			  }
		  } else if (operation == "Print") {
			  db.Print();
		  } else {
			  cout << "Unknown command: " << operation << endl;
		  }
	  }
	} catch (exception &ex) {
		  cout << ex.what() << endl;
		  return 1;
	}

  return 0;
}







