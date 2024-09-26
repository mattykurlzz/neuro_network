#include "neuronet.h"
#include <iostream>

/* Node class implementation */
void Node::setValue(double toSet) {
  value = static_cast<float>(toSet);
  return;
}
Node::Node() : value(0.) {}
Node::Node(float val) : value(val) {}
Node::Node(int val) : value(static_cast<float>(val)) {}
float Node::getValue() { return value; }
// void Node::operator()(Node &other) { value = other.value; } // not
// implemented because virtual method of base class

/* Connector class implementation */
Connector::Connector(std::shared_ptr<Node> leftNode,
                     std::shared_ptr<Node> rightNode) {
  this->leftNode = leftNode;
  this->rightNode = rightNode;
  weight = 0.5;
}
void Connector::transmitForward() {
  rightNode->addValue(weight * leftNode->value);
}

NodeLeftSide::NodeLeftSide() : Node() {}

/* NodeRightSide implementation */
NodeRightSide::NodeRightSide(
    std::vector<std::shared_ptr<NodeLeftSide>> prevLayer)
    : Node() {
  for (auto &prevNode : prevLayer) {
    std::shared_ptr<NodeRightSide> thisPtr =
        std::make_shared<NodeRightSide>(this);
    std::shared_ptr<Connector> tmp =
        std::make_shared<Connector>(prevNode, thisPtr);
    inputConnectors.push_back(tmp);
    prevNode->outputConnectors.push_back(tmp);
  }
}

// Neuromodel::Neuromodel(std::vector<unsigned> vStructure)
//     : nodeConnectors(), nodeMembers() {

//   std::cout << "creating neuro strcture:\n";

//   for (auto it = vStructure.begin(); it != vStructure.end(); ++it) {
//     std::vector<std::shared_ptr<Node>> tmpV;
//     // nodeMembers.push_back(std::vector<std::make_shared<Node>>);
//     if (it == vStructure.begin()) {
//       for (unsigned i = 0; i < *it; ++i) {
//         tmpV.push_back(std::make_shared<InputNode>());
//       }
//     } else {
//       int layer_pos = it - vStructure.begin();
//       for (unsigned i = 0; i < *it; ++i) {
//         std::shared_ptr<Node> tmp =
//             std::make_shared<HiddenNode>(nodeMembers[layer_pos - 1]);
//         tmpV.push_back(tmp);
//       }
//     }
//     nodeMembers.push_back(tmpV);
//   }
// }

// void Neuromodel::print() {
//   for (auto vNodeLayer = nodeMembers.begin(); vNodeLayer !=
//   nodeMembers.end();
//        ++vNodeLayer) {
//     for (auto itNode = (*vNodeLayer).begin(); itNode != (*vNodeLayer).end();
//          ++itNode) {
//       std::cout << (*itNode)->getValue();
//     }
//   }
// }