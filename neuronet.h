#ifndef _NEURONET_H
#define _NEURONET_H

#include <memory>
#include <vector>

class Connector;

/**
 * @brief base class for node types. Contains inputSum and interface for setting
 * it.
 *
 */
class Node {

private:
  double inputSum;
  double value;

public:
  Node();
  Node(int val);
  void addInputSum(double);
  double getValue();
  double getInputSum();
  void dropValue();
  void setValue(double);
  void setValueAfterSum();
};

/**
 * @brief Fully virtual class for node types inheritants. Provides input
 * connectors array.
 *
 */
// class HasInputConnectors {
// public:
//   std::vector<Connector *> vInputConnectorsPtr;
// };

/**
 * @brief Fully virtual class for node types inheritants. Provides output
 * connectors array decl.
 *
 */
// class HasOutputConnectors {
// public:
//   std::vector<Connector *> vOutputConnectorsPtr;
// };

/**
 * @brief virtual class for inheritants with left side properties
 *
 */
// class NodeLeftSide : virtual public Node, public HasOutputConnectors {
// protected:
//   NodeLeftSide();
// };

/**
 * @brief virtual class for inheritants with right side properties
 *
 */
// class NodeRightSide : virtual public Node, public HasInputConnectors {
// protected:
//   NodeRightSide(std::vector<Node *> &prevLayer);
// };

class InputNode : virtual public Node {
protected:
  std::vector<Connector *> vOutputConnectorsPtr;

public:
  InputNode();
  void sendSignalsForward();
  void push_back_out_connector(Connector *con);
};

class OutputNode : virtual public Node {
protected:
  std::vector<Connector *> vInputConnectorsPtr;

public:
  OutputNode(std::vector<InputNode *> &prevLayer);
  OutputNode();
  void deltaModifyWeights(double alpha, double rightAnswer);
  void setXavierWeights(int);
};

class HiddenNode : public InputNode, public OutputNode {
public:
  HiddenNode(std::vector<InputNode *> &prevLayer);
  HiddenNode(std::vector<HiddenNode *> &prevLayer);
  void sendSignalsForward();
  void deltaModifyWeights(double alpha);
  double calculateDelta();
};

/**
 * @brief Connector class providing interface between two Neuron class objects.
 * Allows to modify input neuron's inputSum and modify link strength
 *
 */
class Connector {
private:
  double weight;
  double lastDelta;
  InputNode *leftNodePrt;
  OutputNode *rightNodePrt;

public:
  Connector(InputNode *leftNodePrt, OutputNode *rightNodePrt);
  void transmitSignalForward();
  void addWeight(double inputSum);
  double getLeftNodeValue();
  void setLastDelta(double);
  double getLastDelta();
  double getWeight();
  void setWeight(double);
};

class Neuromodel {
private:
  std::vector<InputNode *> vInputNodeMembersPtr;
  std::vector<std::vector<HiddenNode *>> vvHiddenNodeMembersPtr;
  std::vector<OutputNode *> vOutputNodeMembersPtr;
  double error;
  double teachAlpha = 0.2;
  std::vector<double> rightAnswers;
  // std::vector<std::vector<Connector *>> vNodeConnectorsPtr;

public:
  /*
   * Creates neuromodel class by constructing neuro network of a passed
   * structure. every std::vector<unsigned> depicts one neuron layer. given
   * vector should include input and output layer
   */
  Neuromodel(std::vector<unsigned> vStructure);
  void print();
  void dropNet();
  void setInput(std::vector<double> &input);
  void calculate();
  std::vector<double> getOutputVector();
  double getErr();
  void teach();
  void setRightAnswer(std::vector<double> &answer);
};
#endif //