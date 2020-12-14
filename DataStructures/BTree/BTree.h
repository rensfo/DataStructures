#ifndef BTREE_H
#define BTREE_H

#include <iterator>
#include <queue>
#include <iostream>
#include <cmath>

template<class T, int M = 4>
class BTree
{
public:
    BTree() = default;
    void add(const T &val);
    void remove(const T &val);

    void dump();

private:
    struct Node
    {
        static constexpr int MaxValues = M - 1;
        static constexpr int MaxChild = M;

        Node(Node *parent) : parent(parent)
        {
            clear();
        }

        bool isFull() const
        {
            return currentCount == MaxValues;
        }

        void clear()
        {
            for (int i = 0; i < MaxValues; ++i)
                values[i] = T();

            for (int i = 0; i < MaxChild; ++i)
                child[i] = nullptr;

            currentCount = 0;
        }

        void clearWith(int index)
        {
            for (int i = index; i < MaxValues; ++i)
                values[i] = T();

            for (int i = index + 1; i < MaxChild; ++i)
                child[i] = nullptr;

            currentCount = index;
        }

        Node *parent = nullptr;
        int currentCount = 0;
        bool isLeaf;
        T values[MaxValues];
        Node *child[MaxChild];
    };

    Node *createLeaf(Node *parent = nullptr) const;
    Node *createComposite(Node *parent = nullptr) const;
    Node *findNode(const T &val) const;
    void addToLeaf(Node *node, const T &val);
    void addToComposite(Node *node, const T &val, Node *rightNode);
    void splitLeaf(Node *node, const T &val);
    void splitComposite(Node *node, const T &val, Node *rightNode);
    void addValueToParent(Node *node, const T &val, Node *rightNode);
    void remove(Node *node, const T &val);

private:
    Node *root = nullptr;
};

template<class T, int C>
void BTree<T, C>::add(const T &val)
{
    if (root == nullptr)
    {
        root = createLeaf();
        root->values[0] = val;
        ++root->currentCount;
    }
    else
    {
        auto currentNode = findNode(val);
        addToLeaf(currentNode, val);
    }
}

template<class T, int M>
void BTree<T, M>::remove(const T &val)
{
    auto currentNode = findNode(val);
    remove(currentNode, val);
}

template<class T, int M>
void BTree<T, M>::dump()
{
    if (root)
    {
        std::queue<Node *> nodes;
        nodes.push(root);
        std::cout << "[ ";
        while (!nodes.empty())
        {
            auto node = nodes.front();
            nodes.pop();
            if (node)
            {
                std::cout << node->isLeaf << "{ ";
                for (const auto &val : node->values)
                    std::cout << val << ", ";
                std::cout << "}, ";

                for (auto n : node->child)
                    nodes.push(n);
            }
        }
        std::cout << " ]" << std::endl;
    }
}

template<class T, int M>
typename BTree<T, M>::Node *BTree<T, M>::createLeaf(Node *parent) const
{
    Node *newNode = new Node(parent);
    newNode->isLeaf = true;

    return newNode;
}

template<class T, int M>
typename BTree<T, M>::Node *BTree<T, M>::createComposite(Node *parent) const
{
    Node *newNode = new Node(parent);
    newNode->isLeaf = false;
    return newNode;
}

template<class T, int M>
typename BTree<T, M>::Node *BTree<T, M>::findNode(const T &val) const
{
    auto currentNode = root;
    while (!currentNode->isLeaf)
    {
        int currentValIndex = 0;
        int border = std::min(Node::MaxChild, currentNode->currentCount);
        while (currentValIndex < border  && currentNode->values[currentValIndex] < val)
            ++currentValIndex;

        currentNode = currentNode->child[currentValIndex];
    }
    return currentNode;
}

template<class T, int M>
void BTree<T, M>::addToLeaf(BTree::Node *node, const T &val)
{
    addToComposite(node, val, nullptr);
}

template<class T, int M>
void BTree<T, M>::addToComposite(BTree::Node *node, const T &val, BTree::Node *rightNode)
{
    if (node->isFull())
    {
        splitComposite(node, val, rightNode);
    }
    else
    {
        int currentValIndex = 0;
        while (currentValIndex < (node->currentCount) && node->values[currentValIndex] < val)
            ++currentValIndex;

        for (int i = Node::MaxValues - 1; i >= currentValIndex; i--)
            node->values[i] = node->values[i - 1];

        node->values[currentValIndex] = val;

        for (int i = Node::MaxChild - 1; i >= currentValIndex + 1; i--)
            node->child[i] = node->child[i - 1];

        if(rightNode)
        {
            rightNode->parent = node;
            node->child[currentValIndex + 1] = rightNode;
        }

        ++node->currentCount;
    }
}

template<class T, int M>
void BTree<T, M>::splitLeaf(BTree::Node *node, const T &val)
{
    splitComposite(node, val, nullptr);
}

template<class T, int M>
void BTree<T, M>::splitComposite(BTree::Node *node, const T &val, Node *rightNode)
{
    T values[Node::MaxValues + 1];
    for (int i = 0; i < Node::MaxValues; ++i)
        values[i] = node->values[i];

    Node *child[Node::MaxChild + 1];
    for (int i = 0; i < Node::MaxChild; ++i)
        child[i] = node->child[i];

    int currentValIndex = 0;
    while (currentValIndex < (Node::MaxValues) && values[currentValIndex] < val)
        ++currentValIndex;

    for (int i = Node::MaxValues; i >= currentValIndex; i--)
        values[i] = values[i - 1];

    values[currentValIndex] = val;

    for (int i = Node::MaxChild; i >= currentValIndex + 1; i--)
        child[i] = child[i - 1];

    child[currentValIndex + 1] = rightNode;

    int centerIndex = std::floor(Node::MaxValues / 2);
    node->clearWith(centerIndex);

    auto rightComposite = node->isLeaf ? createLeaf() : createComposite();
    int childI = 0;
    for (int i = centerIndex + 1; i < Node::MaxValues + 1; ++i, ++childI)
        rightComposite->values[childI] = values[i];

    rightComposite->currentCount = childI;

    if(!rightComposite->isLeaf)
    {
        childI = 0;
        for (int i = centerIndex + 1; i < Node::MaxChild + 1; ++i, ++childI)
        {
            child[i]->parent = rightComposite;
            rightComposite->child[childI] = child[i];
        }
    }

    addValueToParent(node, values[centerIndex], rightComposite);
}

template<class T, int M>
void BTree<T, M>::addValueToParent(BTree::Node *node, const T &val, Node *rightNode)
{
    if (node == root)
    {
        root = createComposite();
        root->values[0] = val;
        root->child[0] = node;
        root->child[1] = rightNode;
        root->currentCount = 1;

        node->parent = root;
        rightNode->parent = root;
    }
    else
    {
        addToComposite(node->parent, val, rightNode);
    }
}

template<class T, int M>
void BTree<T, M>::remove(BTree::Node *node, const T &val)
{
    int currentValIndex = 0;
    while (currentValIndex < (Node::MaxValues) && node->values[currentValIndex] != val)
        ++currentValIndex;

    for (int i = currentValIndex; i < Node::MaxValues - 1; ++i)
        node->values[i] = node->values[i + 1];
}

#endif // BTREE_H
