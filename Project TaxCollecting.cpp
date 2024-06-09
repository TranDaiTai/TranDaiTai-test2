#include <iostream>
#include <string > 
#include <vector> 
#include<fstream> 
#include <sstream> 
#define COST_FOR_SQUARE_METER 560000 

using namespace std ; 
class Stall {
public: 
    virtual string to_string()const =0; 
    virtual double costArea() const = 0 ; 
};
class FoodStall:public Stall {
private:
    string _ID ; 
    double _Area;
    double _ColdStorage;
public:
    string to_string()const override{
        return  "Thuc pham ";
    }
    FoodStall(string id , double area) : _ID(id) , _Area(area) {} ; 
    double costArea() const override {
        return _Area*COST_FOR_SQUARE_METER+ _ColdStorage; 
    } ;
    void SetColdStorage(double ColdStorage) {
        _ColdStorage=ColdStorage;
    }
};
class ClothesStall:public Stall {
private:
    string _ID ; 
    double _Area; 
public:
    string to_string()const override{
        return  "Quan ao";
    }
    ClothesStall(string id , double area) : _ID(id) , _Area(area) {} ; 
    double costArea() const override {
        return _Area*COST_FOR_SQUARE_METER ; 
    }
};
class JewelryStall:public Stall {
private:
    string _ID ; 
    double _Area; 
    double _SecurityFee; 
public:
    string to_string()const override{
        return  " Da qui";
    }
    JewelryStall(string id , double area) : _ID(id) , _Area(area) {} ; 
    double costArea() const override {
        return _Area*COST_FOR_SQUARE_METER+_SecurityFee ; 
    }
    void setSecurityFee(double SecurityFee){
        _SecurityFee=SecurityFee;
    }
};

class StallFactory{
    public: 
    Stall* createStall(const string& typeStall, string id , double area) {
        if ( typeStall =="Food"){
            return new FoodStall(id,area) ; 
        }
        else if ( typeStall =="Clothes"){
            return new ClothesStall(id,area) ; 
        }
        else if ( typeStall =="Jewelry "){
            return new JewelryStall(id,area) ; 
        }
        else return nullptr ; 
    } 
};
class sysStall {
public: 
    vector<Stall*> list_stall ; 
    void push(Stall*& stall) {
        list_stall.push_back(stall) ;
    }
    double Totalcost() {
        double total = 0 ; 
        for (const auto &stall :list_stall ){
            total+=stall->costArea(); 
        }
    }
};

void ReadFileStall(const string & filepath, sysStall & sys  ){
    ifstream input(filepath) ; 
    if ( input.is_open()) {
        string line ; 
        StallFactory Stall_Factory ; 
        while ( getline(input,line)) {
            istringstream iss(line )  ;
            string token; 
            size_t first= 0;
            string ID={};  
            double anotherCost=0 ; 
            double Area =0; 
            if ( line.find("Food")!= string::npos){
                while(getline(iss,token,',') ){
                    first = token.find('=') +1 ; 
                    if ( token.find("Food")!=string::npos) {
                        ID=token.substr(first)  ; 
                    }
                    else if ( token.find("Area")!=string::npos) {
                        Area= stod(token.substr(first))  ; 
                    }
                    else if ( token.find("ColdStorage")!=string::npos) {
                        anotherCost= stod(token.substr(first))  ; 
                    }
                }
                Stall* stall = Stall_Factory.createStall("Food",ID,Area); 
                FoodStall* Food_Stall= dynamic_cast<FoodStall*>(stall); 
                Food_Stall->SetColdStorage(anotherCost);    
                sys.push(stall) ;  
            }
            else if ( line.find("Clothes")!= string::npos){
                while(getline(iss,token,',') ){
                    first = token.find('=') +1 ; 
                    if ( token.find("Clothes")!=string::npos) {
                        ID=token.substr(first)  ; 
                    }
                    else if ( token.find("Area")!=string::npos) {
                        Area= stod(token.substr(first))  ; 
                    }
                }
                Stall* stall = Stall_Factory.createStall("Clothes",ID,Area); 
                sys.push(stall) ;  
            }
            else if ( line.find("Jewelry ")!= string::npos){
                while(getline(iss,token,',') ){
                    first = token.find('=') +1 ; 
                    if ( token.find("Jewelry ")!=string::npos) {
                        ID=token.substr(first)  ; 
                    }
                    else if ( token.find("Area")!=string::npos) {
                        Area= stod(token.substr(first))  ; 
                    }
                    else if ( token.find("SecurityFee")!=string::npos) {
                        anotherCost= stod(token.substr(first))  ; 
                    }
                        
                }
                Stall* stall = Stall_Factory.createStall("Clothes",ID,Area); 
                JewelryStall* Jewelry_Stall= dynamic_cast<JewelryStall*>(stall); 
                Jewelry_Stall->setSecurityFee(anotherCost);     
                 sys.push(stall) ;  
            }
        }
    }
}
int main() {
    sysStall sys ; 
    string filepath("RentMay24.txt"); 
    ReadFileStall(filepath,sys) ; 
    cout <<sys.Totalcost() ; 
}
