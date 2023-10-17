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
#include<cstring>
struct ScheduledNode
{
    std::shared_ptr<Node> node;
    int scheduled_cycle;
};


class Circuit {
public:
    Circuit(int and_c, int or_c, int not_c)
    : and_constraint(and_c), or_constraint(or_c), not_constraint(not_c){}

    void scheduling2(){
        and_temp = and_constraint;
        or_temp = or_constraint;
        not_temp = not_constraint;

        int current_cycle = 1;

        int count_how_many_added_in_priority_queue_this_cycle = 0;

        //priority queue
        auto comparator = [this](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
            return longestPathTo[a->returnName()] < longestPathTo[b->returnName()];
        };
        std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, decltype(comparator)> ready_to_schedule(comparator);

        std::queue<std::shared_ptr<Node>> delay_queue;

        //now we have g h i j in priority queue and it is set h i j g
        for(const auto& node : firstGates) {
            ready_to_schedule.push(node);
            count_how_many_added_in_priority_queue_this_cycle++;
        }

        //now we can enter the scheduling 
        while(!ready_to_schedule.empty())
        {   
            auto current_node = ready_to_schedule.top();
            ready_to_schedule.pop();
            count_how_many_added_in_priority_queue_this_cycle--;

            bool inputsIsAllScheduled = all_inputs_scheduled(current_node);
            bool thisNodeCanBeScheduled = canBeScheduled(current_node);
            std::cout << "Name : " << current_node->returnName() <<" Type : " << current_node->returnType() <<" length : " << longestPathTo[current_node->returnName()] << std::endl;
            std::cout <<"Are the inputs of current node all scheduled : " << inputsIsAllScheduled << std::endl <<"Is the current node satisfy the contraint : " << thisNodeCanBeScheduled << " is_scheduled : " <<current_node->is_scheduled<< std::endl<<std::endl;
            if(inputsIsAllScheduled && thisNodeCanBeScheduled)
            {
                schedule.push_back({current_node, current_cycle});
                current_node->is_scheduled = true;
                for(const auto& output_node : current_node->getOutputs())
                {   
                    std::queue<std::shared_ptr<Node>> delay_queue_copy = delay_queue;
                    if(all_inputs_scheduled(output_node))
                    {   
                        

                         int ifAlready = 0;
                         while(!delay_queue_copy.empty())
                         {
                            if(output_node->returnName() == delay_queue_copy.front()->returnName())
                            {
                                ifAlready = 1;
                            }
                            delay_queue_copy.pop();
                            
                         }
                         std::cout << "dfd";
                        std::cout << ifAlready << std::endl;
                        if(ifAlready == 0)
                        {
                            delay_queue.push(output_node);
                        }  
                        
                    }
                }
            }
            else{delay_queue.push(current_node);}
            if(count_how_many_added_in_priority_queue_this_cycle ==0)
            {
                and_temp = and_constraint;
                or_temp = or_constraint;
                not_temp = not_constraint;
                current_cycle++;
                while(!delay_queue.empty()){
                    count_how_many_added_in_priority_queue_this_cycle++;
                    std::shared_ptr<Node> node = delay_queue.front();
                    ready_to_schedule.push(node);
                    delay_queue.pop();

                }
            }
        }




    }

    void scheduling(){
        and_temp = and_constraint;
        or_temp = or_constraint;
        not_temp = not_constraint;

        int current_cycle = 1;

        int nodeCount = and_constraint + or_constraint + not_constraint;

        auto comparator = [this](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
            return longestPathTo[a->returnName()] < longestPathTo[b->returnName()];
        };
        std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, decltype(comparator)> ready_to_schedule(comparator);
        std::queue<std::shared_ptr<Node>> delay_queue;
        for(const auto& node : firstGates) {
            node->is_scheduled = true;
            ready_to_schedule.push(node);
        }
        while(!ready_to_schedule.empty())
        {
            auto current_node = ready_to_schedule.top();
            ready_to_schedule.pop();
            std::cout << "Name : " << current_node->returnName() <<" Type : " << current_node->returnType() <<" length : " << longestPathTo[current_node->returnName()] << std::endl;
            bool inputsIsAllScheduled = all_inputs_scheduled(current_node);
            bool thisNodeCanBeScheduled = canBeScheduled(current_node);
            std::cout <<"Are the inputs of current node all scheduled : " << inputsIsAllScheduled << std::endl <<"Is the current node satisfy the contraint : " << thisNodeCanBeScheduled << " is_scheduled : " <<current_node->is_scheduled<< std::endl<<std::endl;
            //constraint need to be restore
            //is_scheduled should be set
            if(inputsIsAllScheduled && thisNodeCanBeScheduled)
            {
                schedule.push_back({current_node, current_cycle});

                current_node->is_scheduled = true;
                for(const auto& output_node : current_node->getOutputs()) {
                    // TODO: Check if all inputs of output_node are scheduled (or will be in this cycle).
                    // If so, add output_node to ready_to_schedule.
                    bool output_node_ready = all_inputs_scheduled(output_node);
                    if(output_node_ready) {
                    ready_to_schedule.push(output_node);
                }
                }
                
            }
            else{
                    delay_queue.push(current_node);
            }
            nodeCount--;
            std::cout << current_cycle <<std::endl;
            if(nodeCount <0){
                nodeCount = and_constraint + or_constraint + not_constraint;
                current_cycle++;
                and_temp = and_constraint;
                or_temp = or_constraint;
                not_temp = not_constraint;
                while(!delay_queue.empty()){
                    std::shared_ptr<Node> node = delay_queue.front();
                    ready_to_schedule.push(node);
                    delay_queue.pop();

                }
            }
        }

    }
    bool canBeScheduled(std::shared_ptr<Node> current_node)
    {
        switch (current_node->returnGateType())
        {
        case GateType::AND:
            if(and_temp>0)
            {
                and_temp--;
                return true;
            }
            break;
        case GateType::OR:
            if(or_temp>0)
            {
                or_temp--;
                return true;
            }
            break;
        case GateType::NOT:
            if(not_temp>0)
            {
                not_temp--;
                return true;
            }
            break;
        
        default:
        return false;
            break;
        }
    }

    void listScheduling(){
        int current_cycle = 1;
        int nodeCount = 0;
        auto comparator = [this](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
            return longestPathTo[a->returnName()] < longestPathTo[b->returnName()];
        };
        std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, decltype(comparator)> ready_to_schedule(comparator);
        // TODO: Add initial nodes to ready_to_schedule.
        for(const auto& node : firstGates) {
        // Assumption: If a node is in `firstGates`, it's ready to be scheduled initially.
        // Adapt as needed based on your actual logic/criteria.
        node->is_scheduled = true;
        ready_to_schedule.push(node);
       
    }

        while(!ready_to_schedule.empty()) {
            auto current_node = ready_to_schedule.top();  // Use top(), not front() for priority_queue.
            ready_to_schedule.pop();
            std::cout << "Name : " << current_node->returnName() <<" length : " << longestPathTo[current_node->returnName()] << std::endl;
        
    
        
            // TODO: Check if all inputs to current_node are scheduled.
            // If not, delay scheduling.
            bool can_schedule = all_inputs_scheduled(current_node);

            // Check if scheduling current_node adheres to gate type constraints.
            if(can_schedule && canSchedule(current_node, current_cycle)) {
                schedule.push_back({current_node, current_cycle});  // Schedule the node.
                
                // Update ready_to_schedule with nodes that are now ready to be scheduled.
                for(const auto& output_node : current_node->getOutputs()) {
                    // TODO: Check if all inputs of output_node are scheduled (or will be in this cycle).
                    // If so, add output_node to ready_to_schedule.
                    bool output_node_ready = all_inputs_scheduled(output_node);
                    if(output_node_ready) {
                    ready_to_schedule.push(output_node);
                }
                }
            } else {
                // Delay scheduling by re-adding it back into the priority queue, or handle as fits your use case.
                ready_to_schedule.push(current_node);
            }

            current_cycle++;
        }
    }
    bool all_inputs_scheduled(const std::shared_ptr<Node>& current_node){
    // Loop through all input nodes of current_node.
    for(const auto& input_node : current_node->getInputs()) {
        // If any input_node is not scheduled, return false.
        if(!input_node->is_scheduled) {
            return false;
        }
    }
    // If loop completes, all input nodes are scheduled. Return true.
    return true;
}


    bool canSchedule(const std::shared_ptr<Node>& node, int cycle) {
        // TODO: Implement logic to check if node can be scheduled in the current cycle,
        // considering the constraints for gate types.
        // Example: Check the number of AND, OR, NOT gates scheduled in the current cycle
        // and compare with your constraints (2, 1, 1).
        int and_count = 0;
        int or_count = 0;
        int not_count = 0;

        for(const auto& scheduled_node : schedule){
            if(scheduled_node.scheduled_cycle == cycle){
                switch (scheduled_node.node->returnGateType())
                {
                    case GateType::AND: and_count++; break;
                    case GateType::OR: or_count++; break;
                    case GateType::NOT: not_count++; break;
                
                default:
                    break;
                }
            }
        }
        switch (node->returnGateType())
        {
        case GateType::AND:
            if(and_count < and_constraint){
                return true;
            }
            break;
        case GateType::OR:
            if(or_count < or_constraint){
                return true;
            }
            break;
        case GateType::NOT:
            if(not_count < not_constraint){
                return true;
            }
            break;
        
        default:
            break;
        }
        return false;
    }
    
    
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
                */
               for(const auto& i : inputs){
                std::cout << "The successor of " << i << " : " ;
                    for(const auto& successor : nodeMap[i]->getOutputs())
                    {
                        std::cout << successor->returnNodeType() << ", ";
                    }
                    std::cout << std::endl;
               }
                std::cout << std::endl;
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
        std::cout << pair.second->returnName() << " : " << pair.second->returnGateType() << std::endl;
    }
    
}
void showWhetherItIsTheFirstGate(){
    for(const auto& pair: nodeMap)
    {
        std::cout << "Name : " << pair.first << " has output : ";
        for(const auto& input : pair.second->getOutputs())
        {
            std::cout << input->returnName() << " ";
        }
        if(pair.second->getOutputs().empty()){
                std::cout << pair.second->returnNodeType();
            }
        std::cout << std::endl;
    }
}
void checkFirstGate(){
    for(const auto& pair: nodeMap)
    {   
        int flagFirst = 1;
        if(pair.second->getInputs().empty())
        {
            flagFirst = 0;
        }
        for(const auto& input: pair.second->getInputs())
        {
            //std::cout << input->returnNodeType();
            if(input->returnNodeType() != "input")
            {
                flagFirst = 0;
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
    for(const auto& node : nodeMap)
    {
        std::cout << longestPathTo[node.second->returnName()] << " ";
    }
}
void showList()
{
    for(const auto& scheduled_node: schedule){
        std::cout << scheduled_node.node->returnName() <<std::endl;
    }
}
void showNodeCycle(){
    for(const auto& sn : schedule)
    {
        std::cout << sn.node->returnName() << " : " << sn.scheduled_cycle << std::endl;
    }
}

private:
    std::map<std::string, std::shared_ptr<Node>>nodeMap;
    std::map<std::string, int>longestPathTo;
    std::map<std::string, bool>visited;
    std::vector<std::shared_ptr<Node>>firstGates;
    int maxPathlength = 0;
    std::vector<ScheduledNode> schedule;
    int and_constraint, or_constraint, not_constraint;
    int and_temp, or_temp, not_temp;
};
//if we 