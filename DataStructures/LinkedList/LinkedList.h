#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <ostream>

template<class T>
class LinkedList
{
public:

    LinkedList() = default;
    void addToBack(const T &value);
    void remove(const T &value);
    long long count() const;

private:
    template<class U>
    friend std::ostream &operator<<(std::ostream &os, const LinkedList<U> &ll);
    struct Node
    {
        Node *prev;
        Node *next;
        T value;
    };

private:
    Node *m_first = nullptr;
    Node *m_last = nullptr;
    long long m_count = 0;
};

template<class T>
void LinkedList<T>::addToBack(const T &value)
{
    if (m_first == nullptr)
    {
        m_first = new Node{nullptr, nullptr, value};
        m_last = m_first;
    }
    else if (m_first == m_last)
    {
        auto newNode = new Node{m_first, nullptr, value};
        newNode->prev->next = newNode;
        m_last = newNode;
    }
    else
    {
        auto newNode = new Node{m_last, nullptr, value};
        newNode->prev->next = newNode;
        m_last = newNode;
    }
    ++m_count;
}

template<class T>
void LinkedList<T>::remove(const T &value)
{
    auto currentNode = m_first;
    while (currentNode != nullptr && currentNode->value != value)
    {
        currentNode = currentNode->next;
    }

    if (currentNode)
    {
        if (currentNode->prev)
            currentNode->prev->next = currentNode->next;

        if (currentNode->next)
            currentNode->next->prev = currentNode->prev;

        delete currentNode;
        m_count--;
    }
}

template<class T>
long long LinkedList<T>::count() const
{
    return m_count;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const LinkedList<T> &ll)
{
    os << "LinkedList[ ";

    auto currentNode = ll.m_first;
    while (currentNode != nullptr)
    {
        os << currentNode->value << ", ";
        currentNode = currentNode->next;
    }
    os << "]";
    return os;
}


#endif // LINKEDLIST_H
