#include<fstream>
#include<sstream>
#include<vector>
#include<iostream>
#include "circuit.cpp"
#include "node.cpp"
int main(){
    std::string str = "a";
    Circuit sample02(2,1,1);
    sample02.parseBlif("aoi_benchmark/aoi_z4ml.blif");
    //sample02.show();
    sample02.showWhetherItIsTheFirstGate();
    sample02.checkFirstGate();
    sample02.computeCriticalPath();
    //sample02.showPath();
    sample02.scheduling2();
    sample02.showNodeCycle();
    //sample02.listScheduling();
    //list_scheduling(resourceConstraints);
}
