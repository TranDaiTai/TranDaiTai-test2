#include <iostream> 
#include<iomanip> 
#include <sstream> 
#include <vector>
#include<fstream>
#include <string> 

using namespace std;
#define Percent_of_Fresher     0.35 ; 
#define Percent_of_Junior  0.45 ; 
#define Percent_of_Senior  0.9; 

#define FRESHER  "Fresher" ; 
#define JUNIOR  "Junior" ; 
#define SENIOR  "Senior"; 
class EmployeeType {
public:
    virtual double getSponSor() = 0;
    virtual string employeeType() = 0;
};

class Fresher :public EmployeeType {
private:
    string Type = FRESHER;
    double _Sponsor = Percent_of_Fresher;
public:
    double getSponSor() override {
        return _Sponsor;
    }
    string employeeType() override {
        return Type;
    }
};

class Junior :public EmployeeType {
private:
    double Sponsor = Percent_of_Junior;
    string Type = JUNIOR;
public:
    double getSponSor() override {
        return Sponsor;
    }
    string employeeType() override {
        return Type;
    }
};

class Senior :public EmployeeType {
private:
    double Sponsor = Percent_of_Senior;
    string Type = SENIOR;
public:
    double getSponSor() override {
        return Sponsor;
    }
    string employeeType() override {
        return Type;
    }
};

class Vehical {
public:
    virtual double getcost() const = 0;
    virtual string getName() const = 0;
};

class Motorbike :public Vehical {
private:
    double _cost;
public:

    double getcost() const override {
        return _cost;
    }
    Motorbike(double cost) : _cost(cost) {};
    string  getName() const {
        return "Motorbike"; 
    }
};

class Train :public Vehical {
private:
    double _cost;
public:

    double getcost() const override {
        return _cost;
    }
    Train(double cost) : _cost(cost) {};
    string  getName() const {
        return "TRAIN";
    }
};

class Bus :public Vehical {
private:
    double _cost;
public:

    double getcost() const override {
        return _cost;
    }
    Bus(double cost) : _cost(cost) {};
    string  getName() const {
        return "BUS";
    }
};

class VehicalFactory {
public:
    Vehical* createVehical(const string name, double cost) {
        if (name == "Motorbike") {
            return new Motorbike(cost);
        }
        else if (name == "Train") {
            return new Train(cost);
        }
        else if (name == "Bus") {
            return new Bus(cost);
        }
    }
};

class SponsorFactory {
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
        if (yearsOfWork < 1) {
            return new Fresher();
        }
        else if (yearsOfWork == 1) {
            return new Junior();
        }
        else {
            return new Senior();
        }
    }
};

class Employee {
private:
    string _name;
    string _StartDate;
    EmployeeType* _typeEmployee;
    Vehical* _vehical;
public:
    Employee(string name, string startdate, Vehical* vehical) : _name(name), _StartDate(startdate), _vehical(vehical) {
        SponsorFactory factory;
        _typeEmployee = factory.createEmployeeType(_StartDate);
    }
    string getName() const {
        return _name;
    }
    EmployeeType* getEmployeeType() const {
        return _typeEmployee;
    }
    Vehical* getvehical() const {
        return _vehical;
    }
};

class SystemEmployees {
public:
    vector<Employee> list_employee;
    void push(Employee& empl) {
        list_employee.push_back(empl);
    }
    double totalSponsor() const {
        double total = 0;
        for (const auto& empl : list_employee) {
            total += empl.getvehical()->getcost() * (1 - empl.getEmployeeType()->getSponSor());
        }
        return total;
    }
};

void ReadFileEmployees(const string& filepath, SystemEmployees& sys) {
    ifstream input(filepath);
    if (input.is_open()) {
        string line;
        string name = {};
        string startdate = {};
        double cost = 0;
        string vehicaltype = {};
        while (getline(input, line)) {
            istringstream iss(line);
            string token;
            if (line.find("Employee:") != string::npos) {
                while (getline(iss, token, ',')) {
                    size_t first = token.find('=') + 1;
                    if (token.find("Name=") != string::npos) {
                        name = token.substr(first);
                    }
                    else if (token.find("StartDate=") != string::npos) {
                        startdate = token.substr(first);
                    }
                }
            }
            if (line.find("=> Commute:") != string::npos) {
                while (getline(iss, token, ',')) {
                    size_t first = token.find_last_of('=') + 1;
                    if (token.find("Type=") != string::npos) {
                        vehicaltype = token.substr(first);
                    }
                    else if (token.find("Cost=") != string::npos) {
                        cost = stod(token.substr(first+1));
                    }
                }
                VehicalFactory factory;
                Vehical* vhc = factory.createVehical(vehicaltype, cost);
                Employee empl(name, startdate, vhc);
                sys.push(empl);
            }
        }
    }
    else {
        cout << "khong the mo file \n";
    }
}


void printSystemEmployees(const SystemEmployees& sys, const string& filepath) {
    cout << "Chuong trinh tai tro cho nhan vien cua LowRel 05/2024" << endl;
    cout << "Dang doc cac de nghi tu tap tin " << filepath << endl;
    cout << "Tim thay " << sys.list_employee.size() << " de nghi mua ban quyen phan mem" << endl;
    cout << endl;
    cout << "STT | Nhan vien       | Tham nien | Phan mem       | Gia     | Ho tro | Thanh tien" << endl;
    cout << "----------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < sys.list_employee.size(); i++) {
        const Employee& employee = sys.list_employee[i];
        const string& name = employee.getName();
        const Vehical* vehical = employee.getvehical();
        double price = vehical->getcost();
        double discount = employee.getEmployeeType()->getSponSor();
        double total = price * (1 - discount);

        cout << setw(3) << left << i + 1 << " | " << setw(15) << left << name << " | ";
        if (dynamic_cast<Fresher*>(employee.getEmployeeType()))
            cout << setw(9) << left << "Beginner" << " | ";
        else if (dynamic_cast<Junior*>(employee.getEmployeeType()))
            cout << setw(9) << left << "Core" << " | ";
        else if (dynamic_cast<Senior*>(employee.getEmployeeType()))
            cout << setw(9) << left << "Senior" << " | ";

        cout << setw(14) << left << vehical->getName() << " | $" << setw(8) << fixed << setprecision(2) << price << " | ";
        cout << setw(3) << left << fixed << setprecision(0) << discount * 100 << "% | $" << setw(6) << fixed << setprecision(2) << total << endl;
    }

    double totalSponsoring = sys.totalSponsor();
    cout << endl;
    cout << "Tong tien tai tro: $" << totalSponsoring << endl;
}


int main() {
    SystemEmployees sys;
    ReadFileEmployees("May2024Proposals.txt", sys);

    printSystemEmployees(sys, "May2024Proposals.txt");

    return 0;
}