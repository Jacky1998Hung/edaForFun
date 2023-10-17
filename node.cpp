#pragma once
#include <memory>
#include <algorithm>
enum GateType{ AND, OR, NOT, UNKNOWN};
class Node : public std::enable_shared_from_this<Node> {
public:
    bool is_scheduled = false;
    Node(const std::string& name, const std::string& nodeType):name_(name), nodeType_(nodeType){}
    virtual ~Node() = default;  // Virtual destructor is crucial for polymorphism.
    virtual bool evaluate() const = 0;  // Pure virtual function.
    virtual std::string returnName() const {}
    virtual std::string returnType()const{}
    virtual std::string returnNodeType()const{}
    virtual GateType returnGateType()const{
    }
    void addOutput(const std::shared_ptr<Node>& node)
    {
        outputs_.push_back(node);
    }
    void addInput(const std::shared_ptr<Node>& node) 
    {
        inputs_.push_back(node);
    }
    

    const std::vector<std::shared_ptr<Node>>& getOutputs() const 
    {
        return outputs_;
    }

    const std::vector<std::shared_ptr<Node>>& getInputs() const 
    {
        return inputs_;
    }
protected:
    std::string name_;
    std::string nodeType_;
    // Additional common functionality...
    std::vector<std::shared_ptr<Node>> outputs_;
    std::vector<std::shared_ptr<Node>> inputs_;

};

class InputNode : public Node {
public:
    InputNode(const std::string& name, const std::string& nodeType):Node(name, nodeType){is_scheduled = true;}
    bool evaluate() const override {
        // Logic to produce an input signal.
    }
    // Additional InputNode-specific functionality...
    std::string returnName()const override{
        return name_;
    }
    std::string returnNodeType()const override{
        return nodeType_;
    }
    
};


class OutputNode : public Node {
public:
    OutputNode(const std::string& name, const std::string& nodeType):Node(name, nodeType){}

    bool evaluate() const override {
        // Evaluate and potentially store/pass-through the input signal.
    }
    std::string returnName()const override{
        return name_;
    }
    std::string returnNodeType()const override{
        return nodeType_;
    }


    // Additional OutputNode-specific functionality...

};

class GateNode : public Node{
public:
    GateNode(const std::string& name, GateType type, const std::string& nodeType):Node(name, nodeType), type_(type){}

    std::string returnName() const override{
        return name_; 
    }
    std::string returnType()const override{
        return gateTypeToString(type_); 
    }
    std::string returnNodeType()const override{
        return nodeType_;
    }
    std::string gateTypeToString(GateType type) const {
    switch(type) {
        case AND: return "AND";
        case OR: return "OR";
        case NOT: return "NOT";
        default: return "UNKNOWN";
    }
}
    bool evaluate() const override {
        // Evaluate and potentially store/pass-through the input signal.
    }
    GateType returnGateType()const override{
        return type_;
    }


private:
    GateType type_;
    int latency = 1;
};

