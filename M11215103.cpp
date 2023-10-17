#include<fstream>
#include<sstream>
#include<vector>
#include<iostream>
#include "circuit.cpp"
#include "node.cpp"

int main(int argc, char* argv[]){
    if(argc != 6 || std::string(argv[1]) != "-h") {
        std::cerr << "Usage: " << argv[0] << " -h <file_path> <int1> <int2> <int3>" << std::endl;
        return 1;
    }

    std::string filePath = argv[2];
    int arg1 = std::stoi(argv[3]);
    int arg2 = std::stoi(argv[4]);
    int arg3 = std::stoi(argv[5]);

    Circuit sample02(arg1, arg2, arg3);
    sample02.parseBlif(filePath);
    sample02.checkFirstGate();
    sample02.computeCriticalPath();
    sample02.scheduling2();
    sample02.showNodeCycle();

    return 0;
}

