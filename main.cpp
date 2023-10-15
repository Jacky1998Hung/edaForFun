#include<fstream>
#include<sstream>
#include<vector>
#include<iostream>
#include "circuit.cpp"
#include "node.cpp"
int main(){
    std::string str = "a";
    Circuit sample02;
    sample02.parseBlif("aoi_benchmark/aoi_sample02.blif");
    
    //sample02.showWhetherItIsTheFirstGate();
    sample02.checkFirstGate();
    sample02.computeCriticalPath();
    sample02.showPath();
    //list_scheduling(resourceConstraints);

}
