#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream> 
#include <iomanip>
#include <fstream>
#include <map> 
#define Percent_of_beginner 0.2
#define Percent_of_senior 0.4
#define Percent_of_core 0.85

using namespace std;
class EmployeeType {
public:
    virtual double discountPercent() = 0;
};

class Beginner :public EmployeeType {
private:
    double percent_discount = Percent_of_beginner;
public:
    double discountPercent() override {
        return percent_discount;
    }
    Beginner() {
    }
};

class Core :public EmployeeType {
private:
    double percent_discount = Percent_of_core;
public:
    double discountPercent() override {
        return percent_discount;
    }
    Core() {
    }
};

class Senior :public EmployeeType {
private:
    double percent_discount = Percent_of_senior;
public:
    double discountPercent() override {
        return percent_discount;
    }
    Senior() {
    }
};

class EmployeeTypeFactory {
private:
    int calculateYearsOfWork(const std::string& startDate) const {
        time_t now = std::time(nullptr);
        tm start = {};
        istringstream iss(startDate);
        iss >> get_time(&start, "%d/%m/%Y");
        time_t startTime = mktime(&start);
        double secondsPerYear = 365.25 * 24 * 60 * 60;
        int yearsOfWork = static_cast<int>((now - startTime) / secondsPerYear);
        return yearsOfWork;
    }
public:
    EmployeeType* createEmployeeType(const std::string& startDate) {
        int yearsOfWork = calculateYearsOfWork(startDate);
        if (yearsOfWork < 3) {
            return new Beginner();
        }
        else if (yearsOfWork < 5) {
            return new Core();
        }
        else {
            return new Senior();
        }
    }
};

class SoftWare {
private:
    string _name;
    double _price = 0;
    string _reference; 
public:
    double getPrice() const {
        return _price;
    }
    string getName() const {
        return _name;
    }
    SoftWare(string name, double price,string reference) : _name(name), _price(price) , _reference(reference){}
    SoftWare() {
    }
};
class Employee {
private:
    string name;
    string startDate;
    SoftWare sponsoredSoftware;
    EmployeeType* employeeType;

public:
    Employee(const string& name, const string& startDate, const SoftWare& sponsoredSoftware)
        : name(name), startDate(startDate), sponsoredSoftware(sponsoredSoftware) {
        EmployeeTypeFactory factory;
        employeeType = factory.createEmployeeType(startDate);
    }
    string getName() const {
        return name;
    }
    string getStartDate() const {
        return startDate;
    }
    SoftWare getSponsoredSoftware() const {
        return sponsoredSoftware;
    }
    EmployeeType* getEmployeeType() const {
        return employeeType;
    }
};

class SystemEmployees {
   
public:
    vector<Employee> list_employee;
    void push_employee(Employee& empl) {
        list_employee.push_back(empl);
    }
    double total_sponsoring() const {
        double total = 0;
        for (auto& empl : list_employee) {
            SoftWare software;
            double discount = empl.getEmployeeType()->discountPercent();
            software = empl.getSponsoredSoftware();
            total += software.getPrice() * (1 - discount);
        }
        return total;
    }
};

void ReadFileEmployees(const string& filepath, SystemEmployees& sys) {
    ifstream input(filepath);
    if (input.is_open()) {
        string line;
        string softwareName = {}, softwareLink = {};
        double softwarePrice = 0;
        string name = {}, startDate = {};
        while (getline(input, line)) {
          
            if (line.find("Employee: Name=") != string::npos) {
                // Tìm thông tin về nhân viên

                istringstream iss(line);
                string token;
                while (getline(iss, token, ',')) {
                    size_t first = token.find('=');

                    if (token.find("Name=") != string::npos) {

                        name = token.substr(first + 1, token.size() - first);

                    }
                    else if (token.find("StartDate=") != string::npos) {

                        startDate = token.substr(first + 1 ,token.size() - first);
                    }
                }
            }
         
            if (line.find("=> Software: Name=") != string::npos) {
                // Tìm thông tin về phần mềm
                istringstream iss(line);
                string token;
                while (getline(iss, token, ',')) {
                    size_t first = token.find_last_of('=');
                

                    if (token.find("Name=") != string::npos) {

                        softwareName = token.substr(first + 1, token.size() - first);;

                    }
                    else if (token.find("Price=$") != string::npos) {

                        softwarePrice = stod(token.substr(first + 2, token.size() - first));

                    }
                    else if (token.find("Link=") != string::npos) {

                        softwareLink = token.substr(first + 1, token.size() - first);

                    }
                }
                // Tạo đối tượng Software và Employee, sau đó thêm vào hệ thống
                SoftWare software(softwareName, softwarePrice, softwareLink);
                Employee employee(name, startDate, software);
                sys.push_employee(employee);
            }
      
        }
        input.close();
    }
    else {
        cout << "khong the mo file \n";
    }
}

void printSystemEmployees(const SystemEmployees& sys,const string& filepath) {
    cout << "Chuong trinh tai tro cho nhan vien cua LowRel 05/2024" << endl;
    cout << "Dang doc cac de nghi tu tap tin "<<filepath << endl;
    cout << "Tim thay " << sys.list_employee.size() << " de nghi mua ban quyen phan mem" << endl;
    cout << endl;
    cout << "STT | Nhan vien       | Tham nien | Phan mem       | Gia     | Ho tro | Thanh tien" << endl;
    cout << "----------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < sys.list_employee.size(); i++) {
        const Employee& employee = sys.list_employee[i];
        const string& name = employee.getName();
        const string& startDate = employee.getStartDate();
        const SoftWare& software = employee.getSponsoredSoftware();
        double price = software.getPrice();
        double discount = employee.getEmployeeType()->discountPercent();
        double total = price * (1 - discount);

        cout << setw(3)<< left << i + 1 << " | " << setw(15)<< left << name << " | ";
        if (employee.getEmployeeType()->discountPercent() == Percent_of_beginner)
            cout << setw(9)<< left << "Beginner" << " | ";
        else if (employee.getEmployeeType()->discountPercent() == Percent_of_core)
            cout << setw(9) << left << "Core" << " | ";
        else if (employee.getEmployeeType()->discountPercent() == Percent_of_senior)
            cout << setw(9)<< left << "Senior" << " | ";

        cout << setw(14)<<left << software.getName() << " | $" << setw(8) << fixed << setprecision(2) << price << " | ";
        cout << setw(3)<< left << fixed << setprecision(0) << discount * 100 << "% | $" << setw(6) << fixed << setprecision(2) << total << endl;
    }

    double totalSponsoring = sys.total_sponsoring();
    cout << endl;
    cout << "Tong tien tai tro: $" << totalSponsoring << endl;
}

int main() {
    SystemEmployees sys;
    ReadFileEmployees("May2024Proposals.txt", sys);

    printSystemEmployees(sys, "May2024Proposals.txt");

    return 0;
}