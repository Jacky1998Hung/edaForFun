/*
we should store our input output and real gate in gate class?
so the node will have three flag 
so the input has a one cycle count, a: intput b: AND c :output(NOP) which we count end to starrt
c -> b -> a (0 -> 1 -> 2), need two cycle, 
what are we going now: 
1.we can put input output gates in there class objects
2.input and output is simple : nodeMap[input/output_name]=std::make_shared<InputNode/OutputNodeß>(input/output_name) 
3.now as we scan through each name, we need to do few operations, how many? we will confirm later..
    1)create the connection of the inputnode and the gate
    2)determine the gate type
4.Once we scan through the list and touch .end we are done, nothing crazy
*/
#include <vector>
#include "node.cpp"
#include <memory>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include<algorithm>
struct Schedule {
    int latency;
    std::vector<std::vector<std::shared_ptr<Node>>> cycles;
};

class Circuit {
public:
    
    void parseBlif(const std::string& filename) {
        std::ifstream file(filename);
        if(!file.is_open())
        {
            std::cerr << "Unable to open file: " << filename << std::endl;
            return;
        }
        std::string line;
        bool keeplooping = true;
        while(std::getline(file, line)){
            std::istringstream iss(line);
            std::string word;

            iss >> word;
            if(word == ".model" || word == ".end")
            {
                //handle starrt and end of the Blif file
                continue;
            }
            else if (word == ".inputs")
            {
                while(iss >> word)
                {
                    if(word == "\\")  // Line continuation
                    {
                        std::string nextLine;
                        std::getline(file, nextLine);  // Get the continued line
                        
                        // Find where the "\\" was in the original string
                        std::size_t slashPosition = line.find_last_of("\\");
                        
                        // Concatenate the next line after removing the "\\"
                        line = line.substr(0, slashPosition) + " " + nextLine;
                        
                        // Set the current stringstream to the concatenated line
                        iss.str(line);  
                        // Move the get pointer to after the position of "\\"
                        iss.seekg(slashPosition, std::ios::beg); 
                        
                        continue;
                    }
                    nodeMap[word] = std::make_shared<InputNode>(word, "input");
                    //std::cout << nodeMap[word]->returnName() << std::endl;
                }
            }
            else if (word == ".outputs")
            {
                while(iss >> word)
                {
                    if(word == "\\")  // Line continuation
                    {
                        std::string nextLine;
                        std::getline(file, nextLine);  // Get the continued line
                        
                        // Find where the "\\" was in the original string
                        std::size_t slashPosition = line.find_last_of("\\");
                        
                        // Concatenate the next line after removing the "\\"
                        line = line.substr(0, slashPosition) + " " + nextLine;
                        
                        // Set the current stringstream to the concatenated line
                        iss.str(line);  
                        // Move the get pointer to after the position of "\\"
                        iss.seekg(slashPosition, std::ios::beg); 
                        
                        continue;
                    }

                    nodeMap[word] = std::make_shared<OutputNode>(word, "output");
                    //std::cout << nodeMap[word]->returnName() << std::endl;

                }
            }
            else if (word == ".names")
            {   
                std::vector<std::string>inputs;
                std::string outputs;
                std::vector<std::string>truthTableLines;
                
                while(iss >> word)
                { 

                     // Check for the next word without moving the stream position
                    std::string peekWord;
                    if (!(iss >> peekWord)) {
                        /*std::cout << "The last word is: " << word << std::endl;
                        std::string name = word;
                        //GateNode word(name, AND);
                        nodeMap[name] = std::make_shared<GateNode>(name, AND);
                        std::string str = nodeMap[name]->returnName(); 
                        std::cout << str << std::endl;*/
                        
                        outputs = word;
                    }else
                    {
                        inputs.push_back(word);
                        iss.seekg(-peekWord.length(), std::ios_base::cur);
                    }
                    //Read all lines until another keyword is found
                    while(std::getline(file, line))
                    {
                        std::istringstream nextLineStream(line);
                        std::string nextWord;
                        nextLineStream >> nextWord;

                        //Check if next line is keyword
                        if(nextWord[0] == '.'|| nextLineStream.eof())
                        {
                            //if yes then need to roll back this line 
                            file.seekg(-line.length() - 1, std::ios_base::cur);
                            break;
                        }else
                        {
                            truthTableLines.push_back(line);
                        }
                    }
                    
                    //we have std::vector<std::string>inputs : a d
                    //        std::string outputs g 
                    //        std::vector<std::string>truthTableLines;
                    //          : 1- 1
                    //            -1 1
                    //first store gate
                    //GateType type = determineGateType(inputs, outputs, truthTableLines);

                }
                /*for(const auto& input : inputs){
                    std::cout << input << ",";
                }
                std::cout << std::endl;*/
                if(nodeMap.find(outputs) == nodeMap.end()){
                GateType type = determineGateType(inputs, outputs, truthTableLines);
                nodeMap[outputs] = std::make_shared<GateNode>(outputs, type, "gate");
                }
                //std::cout << nodeMap[outputs]->returnName() << " : " << nodeMap[outputs]->returnType() << std::endl;
                // Now, connect the input nodes to this gate
                for(const auto& input_name : inputs){
                    std::shared_ptr<Node> input_node = nodeMap[input_name];
                    input_node->addOutput(nodeMap[outputs]);  // Add the gate node as an output to the input node
                    nodeMap[outputs]->addInput(input_node);    // Add the input node as an input to the gate node
                }
                // just printing out the input of current destination node
                /*
                std::cout << "The inputs of " << outputs << " : " ;
                for(const auto& input : nodeMap[outputs]->getInputs())
                    {
                        std::cout << input->returnName() << ", ";
                    }
                std::cout << std::endl;
                
               for(const auto& i : inputs){
                std::cout << "The successor of " << i << " : " ;
                    for(const auto& successor : nodeMap[i]->getOutputs())
                    {
                        std::cout << successor->returnName() << ", ";
                    }
                    std::cout << std::endl;
               }
                std::cout << std::endl;*/
            }
        }
    }
    GateType determineGateType(std::vector<std::string>inputs, std::string outputs, std::vector<std::string>truthTableLines){
        if(truthTableLines.size() == 1 && truthTableLines[0].size() == 3)
        {
            if(truthTableLines[0] == "0 1" || truthTableLines[0] == "1 0") return NOT;
        }
        else if(truthTableLines.size() == 1 && inputs.size() > 1)
        {
            for(const auto& truth: truthTableLines)
            {
                if(truth == "0") return UNKNOWN;
            }
            return AND;
        }
        else return OR;
    }
    
//ya
void show(){
    for(const auto& pair: nodeMap){
        std::cout << pair.second->returnName() << " : " << pair.second->returnNodeType() << std::endl;
    }
    
}
void showWhetherItIsTheFirstGate(){
    for(const auto& pair: nodeMap)
    {
        std::cout << "Name : " << pair.first << " has intput : ";
        for(const auto& input : pair.second->getInputs())
        {
            std::cout << input->returnName() << " ";
        }
        if(pair.second->getInputs().empty()){
                std::cout << pair.second->returnNodeType();
            }
        std::cout << std::endl;
    }
}
void checkFirstGate(){
    for(const auto& pair: nodeMap)
    {   
        int flagFirst = 0;
        for(const auto& input: pair.second->getInputs())
        {
            //std::cout << input->returnNodeType();
            if(input->returnNodeType() == "input")
            {
                flagFirst = 1;
            }
        }
         if(flagFirst == 1)
            {
                firstGates.push_back(pair.second);
            }
        std::cout << std::endl;
    }
    for(const auto& node : firstGates)
    {
        std::cout << node->returnName() << std::endl;
    }
}
int DFS(std::string nodeName){
    if(visited[nodeName])return longestPathTo[nodeName];
    visited[nodeName] = true;
    int maxPath = 0;
    for(const auto& neighbor : nodeMap[nodeName]->getOutputs()) { 
        maxPath = std::max(maxPath, DFS(neighbor->returnName()));
    }
    longestPathTo[nodeName] = maxPath + 1;
    maxPathlength = std::max(maxPathlength, longestPathTo[nodeName]);
    return longestPathTo[nodeName];
}
void computeCriticalPath() {
    for(auto node : firstGates) { // firstGates is a vector of nodes that are first gates after input
        if(!visited[node->returnName()]) {
            DFS(node->returnName());
        }
    }
}
void showPath(){
    for(const auto& node : firstGates)
    {
        std::cout << longestPathTo[node->returnName()];
    }
}

private:
    std::map<std::string, std::shared_ptr<Node>>nodeMap;
    std::map<std::string, int>longestPathTo;
    std::map<std::string, bool>visited;
    std::vector<std::shared_ptr<Node>>firstGates;
    int maxPathlength = 0;
};
