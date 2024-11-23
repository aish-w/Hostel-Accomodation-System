#include <iostream>
#include <mysql.h>
#include <windows.h>
using namespace std;

// MySQL Connection Details
const char* HOST = "localhost";
const char* USER = "root";
const char* PW = "Aish@7985"; // Leave empty for no password
const char* DB = "myhostel";

// Hostel Class
class Hostel {
private:
    string Name;
    int Bed, Fee;

public:
    Hostel(string name, int bed, int fee) { // Parameterized constructor
        Name = name;
        Bed = bed;
        Fee = fee;
    }

    // Getter functions
    string getName() {
        return Name;
    }
    int getBed() {
        return Bed;
    }
    int getFee() {
        return Fee;
    }
};

int main() {
    MYSQL* conn;
    conn = mysql_init(NULL);

    // Establish MySQL connection
    if (!mysql_real_connect(conn, HOST, USER, PW, DB, 3306, NULL, 0)) {
        cout << "Error: " << mysql_error(conn) << endl;
        return 1;
    } else {
        cout << "Connected to Database" << endl;
    }
    Sleep(2000);

    // Create Hostel Object
    Hostel h("3star", 5, 5000);

    // Insert initial hostel details into the database
    string insert = "INSERT INTO hostel (Name, Bed, Fee) VALUES ('" +
                    h.getName() + "', " + to_string(h.getBed()) + ", " + to_string(h.getFee()) + ")";
    if (mysql_query(conn, insert.c_str())) {
        cout << "Error: " << mysql_error(conn) << endl;
    } else {
        cout << "Data inserted successfully" << endl;
    }

    bool exit = false;
    while (!exit) {
        system("cls");
        cout << "Welcome to Hostel Management System" << endl;
        cout << "***********************************" << endl;
        cout << "1. Reserve Bed" << endl;
        cout << "2. Exit" << endl;
        cout << "Enter Your Choice: ";
        int val;
        cin >> val;

        if (val == 1) {
            string studentName;
            cout << "Enter Student Name: ";
            cin >> studentName;

            int totalBeds = 0;
            string check = "SELECT Bed FROM hostel WHERE Name='" + h.getName() + "'";
            if (mysql_query(conn, check.c_str())) {
                cout << "Error: " << mysql_error(conn) << endl;
            } else {
                MYSQL_RES* res = mysql_store_result(conn);
                if (res) {
                    MYSQL_ROW row = mysql_fetch_row(res);
                    if (row) {
                        totalBeds = atoi(row[0]); // Convert Bed count to integer
                    }
                    mysql_free_result(res);
                } else {
                    cout << "Error retrieving data: " << mysql_error(conn) << endl;
                }
            }

            if (totalBeds > 0) {
                totalBeds--;
                string update = "UPDATE hostel SET Bed=" + to_string(totalBeds) +
                                " WHERE Name='" + h.getName() + "'";
                if (mysql_query(conn, update.c_str())) {
                    cout << "Error: " << mysql_error(conn) << endl;
                } else {
                    cout << "Bed reserved successfully in " << h.getName() << " hostel for student " << studentName << endl;
                    cout << "Please pay " << h.getFee() << " rupees." << endl;
                }
            } else {
                cout << "SORRY! NO BEDS AVAILABLE." << endl;
            }
            Sleep(5000);
        } else if (val == 2) {
            exit = true;
            cout << "GOOD LUCK!" << endl;
            Sleep(2000);
        } else {
            cout << "INVALID INPUT!" << endl;
            Sleep(2000);
        }
    }

    // Close MySQL connection
    mysql_close(conn);
    return 0;
}
