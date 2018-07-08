
#include <string>
#include <iostream>
#include "Display.h"


int main(int argc, char** argv) {
	
    std::string ip;
    std::string user;
    std::string pass;

    SQLInjector sql;
    Display d;

    std::cout << "Enter the ip:sqlport (sql port is 3306 by default, Eg. 127.0.0.1:3306): ";
    std::cin >> ip;
    ip = "tcp://" + ip;

    std::cout << "Enter the username (Eg dev): ";
    std::cin >> user;
    
    std::cout << "Enter the password (Eg. 1234): ";
    std::cin >> pass;

    sql.connect(ip.c_str(), user.c_str(), pass.c_str());
    sql.setActiveDatabase("heatmap");
    sql.setActiveTable("clicks");

	d.ShowWindow(sql);
}