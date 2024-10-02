#include "neuronet.h"
#include <cmath>
#include <iostream>
#include <random>

/* Node class implementation */
// void Node::setValue(double toSet) {
//   inputSum = static_cast<double>(toSet);
//   return;
// }
Node::Node() : inputSum(0.) {}
Node::Node(int val) : inputSum(static_cast<double>(val)) {}
double Node::getValue() { return value; }
double Node::getInputSum() { return inputSum; }
void Node::addInputSum(double addr) { inputSum += static_cast<double>(addr); }
void Node::dropValue() {
  inputSum = 0;
  value = 0;
}

void Node::setValueAfterSum() {
  value = (1.0 / (1.0 + exp(-1.0 * inputSum))) - 0.1;
}
void Node::setValue(double val) { value = val; }
// void Node::operator()(Node &other) { inputSum = other.inputSum; } // not
// implemented because virtual method of base class

/* Connector class implementation */
// Connector::Connector(Node *leftNodePrt, Node *rightNodePrt) {
//   this->leftNodePrt = leftNodePrt;
//   this->rightNodePrt = rightNodePrt;
//   weight = 0.5;
// }
// void Connector::transmitForward() {
//   rightNodePrt->addInputSum(weight * leftNodePrt->getValue());
// }

// NodeLeftSide::NodeLeftSide() : Node() {}

/* NodeRightSide implementation */
// NodeRightSide::NodeRightSide(std::vector<NodeLeftSide *> &prevLayer) : Node()
// {
//   for (NodeLeftSide *prevNode : prevLayer) {
//     Connector *tmp = new Connector(prevNode, this);
//     vInputConnectorsPtr.push_back(tmp);
//     prevNode->vOutputConnectorsPtr.push_back(tmp);
//   }
// }

InputNode::InputNode() {}
void InputNode::push_back_out_connector(Connector *con) {
  vOutputConnectorsPtr.push_back(con);
}
/**
 * @brief отправляет сигнал на каждую правую связь
 *
 */
void InputNode::sendSignalsForward() {
  for (auto it : vOutputConnectorsPtr) {
    it->transmitSignalForward();
  }
}

// OutputNode::OutputNode(std::vector<HiddenNode *> &prevLayer) {
//   for (InputNode *prevNode : prevLayer) {
//     Connector *tmp = new Connector(prevNode, this);
//     vInputConnectorsPtr.push_back(tmp);
//     prevNode->push_back_out_connector(tmp);
//   }
// }
OutputNode::OutputNode(std::vector<InputNode *> &prevLayer) {
  // std::cout << "0.No segmentation fault yet\n";
  for (InputNode *prevNode : prevLayer) {
    // std::cout << "0.No segmentation fault yet\n";
    Connector *tmp = new Connector(prevNode, this);
    vInputConnectorsPtr.push_back(tmp);
    prevNode->push_back_out_connector(tmp);
    // std::cout << "0.No segmentation fault yet\n";
  }
}
OutputNode::OutputNode() : vInputConnectorsPtr{} {}

void OutputNode::deltaModifyWeights(double alpha, double rightAnswer) {
  double inputSum = getValue();
  double delta = inputSum * (1 - inputSum) * (rightAnswer - inputSum);
  for (auto connectorPtr : vInputConnectorsPtr) {
    connectorPtr->addWeight(delta * connectorPtr->getLeftNodeValue() * alpha);
    connectorPtr->setLastDelta(delta);
  }
}

void OutputNode::setXavierWeights(int prevLayerWidth) {
  std::random_device rd{};
  std::mt19937 gen{rd()};
  std::normal_distribution<double> rand(0, 1 / sqrt(prevLayerWidth));
  for (auto connectorPtr : vInputConnectorsPtr) {
    connectorPtr->setWeight(rand(gen));
  }
}

HiddenNode::HiddenNode(std::vector<InputNode *> &prevLayer)
    : OutputNode(prevLayer) {}

HiddenNode::HiddenNode(std::vector<HiddenNode *> &prevLayer) {
  for (InputNode *prevNode : prevLayer) {
    // std::cout << "1.No segmentation fault yet\n";
    Connector *tmp = new Connector(prevNode, this);
    vInputConnectorsPtr.push_back(tmp);
    prevNode->push_back_out_connector(tmp);
    // std::cout << "No segmentation fault yet";
  }
}

void HiddenNode::sendSignalsForward() {
  this->setValueAfterSum(); // изменяет значение нейронов, заменяя сумму входов
                            // на сигмоиду
  for (auto it : vOutputConnectorsPtr) {
    it->transmitSignalForward();
  }
}

void HiddenNode::deltaModifyWeights(double alpha) {
  double delta = calculateDelta();
  // std::cout << "delta: " << delta << std::endl;
  for (auto connectorPtr : vInputConnectorsPtr) {
    connectorPtr->addWeight(delta * connectorPtr->getLeftNodeValue() * alpha);
    connectorPtr->setLastDelta(delta);
  }
}

double HiddenNode::calculateDelta() {
  double retval = 0;
  for (auto connectorPtr : vOutputConnectorsPtr) {
    retval += connectorPtr->getLastDelta() * connectorPtr->getWeight();
  }
  return retval * getValue() * (1 - getValue());
}

Connector::Connector(InputNode *leftNodePrt, OutputNode *rightNodePrt) {
  this->leftNodePrt = leftNodePrt;
  this->rightNodePrt = rightNodePrt;
  weight = 0.5;
}

void Connector::transmitSignalForward() {
  double tmp = leftNodePrt->getValue() * weight;
  rightNodePrt->addInputSum(tmp);
}

void Connector::addWeight(double inputSum) { weight += inputSum; }

double Connector::getLeftNodeValue() { return leftNodePrt->getValue(); }

void Connector::setLastDelta(double lastDelta) { this->lastDelta = lastDelta; }

double Connector::getLastDelta() { return lastDelta; }

double Connector::getWeight() { return weight; }

void Connector::setWeight(double w) { weight = w; }

Neuromodel::Neuromodel(std::vector<unsigned> vStructure)
    : vvHiddenNodeMembersPtr(), vInputNodeMembersPtr(),
      vOutputNodeMembersPtr() {
  std::cout << "creating neuro strcture:\n";
  for (unsigned i = 0; i < *(vStructure.begin()); ++i) {
    vInputNodeMembersPtr.push_back(new InputNode());
  }

  std::cout << "created Input Nodes\n";

  for (auto it = (vStructure.begin() + 1); it != (vStructure.end() - 1); ++it) {
    vvHiddenNodeMembersPtr.push_back(std::vector<HiddenNode *>{});
    for (unsigned i = 0; i < *it; ++i) {
      if (it == (vStructure.begin() + 1)) {
        // std::cout << 2 << std::endl;
        vvHiddenNodeMembersPtr.back().push_back(
            new HiddenNode(vInputNodeMembersPtr));
      } else {
        // std::cout << 3 << std::endl;
        vvHiddenNodeMembersPtr.back().push_back(
            new HiddenNode(vvHiddenNodeMembersPtr[0]));
      }
      vvHiddenNodeMembersPtr.back().back()->setXavierWeights(*it);
    }
    std::cout << "created Hidden Nodes layer\n";
  }

  std::cout << "created Hidden Nodes\n";
  for (unsigned i = 0; i != *(vStructure.end() - 1); ++i) {
    std::vector<InputNode *> hiddenInput(vvHiddenNodeMembersPtr.back().begin(),
                                         vvHiddenNodeMembersPtr.back().end());
    vOutputNodeMembersPtr.push_back(new OutputNode(hiddenInput));
    vOutputNodeMembersPtr.back()->setXavierWeights(*(vStructure.begin()));
  }
}

void Neuromodel::print() {
  // std::cout << "\nInput Nodes:\n";
  // for (auto NodeP : vInputNodeMembersPtr) {
  //   std::cout << NodeP->getValue() << "\t";
  // }
  // for (auto vNodeP : vvHiddenNodeMembersPtr) {
  //   std::cout << "\nHidden Nodes:" << std::endl;
  //   for (auto NodeP : vNodeP) {
  //     std::cout << NodeP->getInputSum() << "->" << NodeP->getValue() << "\t";
  //   }
  // }
  std::cout << "\nOutput Nodes: " << std::endl;
  for (auto NodeP : vOutputNodeMembersPtr) {
    std::cout << NodeP->getInputSum() << "->" << NodeP->getValue() << "\t";
  }
  std::cout << std::endl;
}

void Neuromodel::dropNet() {
  for (auto it : vInputNodeMembersPtr) {
    it->dropValue();
  }
  for (auto itV : vvHiddenNodeMembersPtr) {
    for (auto it : itV) {
      it->dropValue();
    }
  }
  for (auto it : vOutputNodeMembersPtr) {
    it->dropValue();
  }
}

/**
 * @brief Устанавливает значение входных нейронов по заданному вектору
 *
 * Требует точно совпадающего по длине с количеством входных нейронов массива,
 * либо длиннее
 *
 * @param input
 */
void Neuromodel::setInput(std::vector<double> &input) {
  for (auto nodeIt = vInputNodeMembersPtr.begin();
       nodeIt < vInputNodeMembersPtr.end(); ++nodeIt) {
    (*(nodeIt))->dropValue();
    (*(nodeIt))->setValue(input[nodeIt - vInputNodeMembersPtr.begin()]);
  }
}

void Neuromodel::calculate() {
  for (auto nodePtr : vInputNodeMembersPtr) {
    nodePtr->sendSignalsForward();
  }
  for (auto vNodePtrs : vvHiddenNodeMembersPtr) {
    for (auto nodePtr : vNodePtrs) {
      nodePtr->sendSignalsForward();
    }
  }
  for (auto nodePtr : vOutputNodeMembersPtr) {
    nodePtr->setValueAfterSum();
  }
}

std::vector<double> Neuromodel::getOutputVector() {
  std::vector<double> retVal{};
  for (auto nodePtr : vOutputNodeMembersPtr) {
    double tmpValue = 1. / (1. + exp(-1 * nodePtr->getValue()));
    retVal.push_back(tmpValue);
  }
  return retVal;
}

void Neuromodel::setRightAnswer(std::vector<double> &answer) {
  rightAnswers = answer;
}

double Neuromodel::getErr() {
  error = 0;
  for (auto nodeIt = vOutputNodeMembersPtr.begin();
       nodeIt < vOutputNodeMembersPtr.end(); ++nodeIt) {
    error += pow((rightAnswers[nodeIt - vOutputNodeMembersPtr.begin()] -
                  (*nodeIt)->getValue()),
                 2) /
             2;
  }
  return fabs(error);
}

void Neuromodel::teach() {
  for (auto nodeIt = vOutputNodeMembersPtr.begin();
       nodeIt < vOutputNodeMembersPtr.end(); ++nodeIt) {
    (*nodeIt)->deltaModifyWeights(
        teachAlpha, rightAnswers[nodeIt - vOutputNodeMembersPtr.begin()]);
  }
  for (auto vNodePtrs : vvHiddenNodeMembersPtr) {
    for (auto NodePtrs : vNodePtrs) {
      NodePtrs->deltaModifyWeights(teachAlpha);
    }
  }
}