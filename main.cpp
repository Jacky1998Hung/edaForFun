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
    std::map<GateType, int> resourceConstraints = {
        {AND, 2},
        {OR, 1},
        {NOT, 1},

        // ... add other gate type constraints as needed
    };
    sample02.show();
    //list_scheduling(resourceConstraints);

}
