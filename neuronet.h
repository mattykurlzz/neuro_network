#ifndef _NEURONET_H
#define _NEURONET_H

#include <memory>
#include <vector>

class Connector;

/**
 * @brief base class for node types. Contains value and interface for setting
 * it.
 *
 */
class Node {
  friend class Connector;

private:
  float value;

protected:
  void setValue(double); // value should be set only through this method
  void addValue(double);
  void operator()(Node &);

public:
  Node();
  Node(float);
  Node(int val);
  float getValue();
};

/**
 * @brief Fully virtual class for node types inheritants. Provides input
 * connectors array.
 *
 */
class HasInputConnectors {
public:
  std::vector<std::shared_ptr<Connector>> inputConnectors;
};

/**
 * @brief Fully virtual class for node types inheritants. Provides output
 * connectors array decl.
 *
 */
class HasOutputConnectors {
public:
  std::vector<std::shared_ptr<Connector>> outputConnectors;
};

/**
 * @brief virtual class for inheritants with left side properties
 *
 */
class NodeLeftSide : virtual public Node, virtual public HasOutputConnectors {
protected:
  NodeLeftSide();
};

/**
 * @brief virtual class for inheritants with right side properties
 *
 */
class NodeRightSide : virtual public Node, public HasInputConnectors {
protected:
  NodeRightSide(std::vector<std::shared_ptr<NodeLeftSide>> prevLayer);
};

/**
 * @brief Connector class providing interface between two Neuron class objects.
 * Allows to modify input neuron's value and modify link strength
 *
 */
class Connector {
private:
  float weight;
  std::shared_ptr<Node> leftNode;
  std::shared_ptr<Node> rightNode;

public:
  Connector(std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode);
  void transmitForward();
};

class HiddenNode : public NodeLeftSide, public NodeRightSide {
public:
  HiddenNode(std::vector<std::shared_ptr<NodeLeftSide>> prevLayer)
      : NodeRightSide(prevLayer) {}
};

class InputNode : public NodeLeftSide {
public:
  InputNode() : NodeLeftSide() {}
};

class OutputNode : public NodeRightSide {
public:
  OutputNode(std::vector<std::shared_ptr<NodeLeftSide>> prevLayer)
      : NodeRightSide(prevLayer) {}
};

class Neuromodel {
private:
  std::vector<std::vector<std::shared_ptr<Node>>> nodeMembers;
  std::vector<std::vector<std::shared_ptr<Connector>>> nodeConnectors;

public:
  /*
   * Creates neuromodel class by constructing neuro network of a passed
   * structure. every std::vector<unsigned> depicts one neuron layer. given
   * vector should include input and output layer
   */
  Neuromodel(std::vector<unsigned> vStructure);
  void print();
};

#endif // _NEURONET_H