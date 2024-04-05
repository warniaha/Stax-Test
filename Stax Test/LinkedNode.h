#pragma once
#include <memory>
#include <set>
#include <vector>

template <typename T>
class LinkedNode;

template <typename T>
using LinkedNodePtr = std::shared_ptr<LinkedNode<T>>;
template <typename T>
using LinkedNodeSet = std::shared_ptr<std::set<LinkedNodePtr<T>>>;

template <typename T>
class LinkedNode : public std::enable_shared_from_this<LinkedNode<T>>
{
    T data;
    LinkedNodeSet<T> nodes;
    LinkedNodePtr<T> prev;
    LinkedNodePtr<T> next;
    LinkedNode(T t) : data(t)
    {
        nodes = LinkedNodeSet<T>(new std::set<LinkedNodePtr<T>>()); // create a new list
    }
    bool contains(LinkedNodePtr<T> node)
    {
        return nodes->find(node) != nodes->end();
    }
public:
    static LinkedNodePtr<T> create(T data)
    {
        LinkedNodePtr<T> node = std::make_shared<LinkedNode<T>>(LinkedNode<T>(data));
        node->nodes->insert(node); // initial list should contain itself
        return node;
    }
    const T& value() const
    {
        return data;
    }
    T& value()
    {
        return data;
    }
    void insertBefore(LinkedNodePtr<T> node)
    {
        if (contains(node))
            throw std::exception("This list already contains this node, it can't be added a second time");
        if (node->nodes->size() > 1)
            throw std::exception("This node already belongs to another list, it can't be added a to this list");
        nodes->insert(node);
        node->nodes = nodes;
        node->prev = prev;
        prev = node;
        node->next = this->shared_from_this();
    }
    size_t size() const
    {
        return nodes->size();
    }
    void insertAfter(LinkedNodePtr<T> node)
    {
        if (contains(node))
            throw std::exception("This list already contains this node, it can't be added a second time");
        if (node->nodes->size() > 1)
            throw std::exception("This node already belongs to another list, it can't be added a to this list");
        nodes->insert(node);
        node->nodes = nodes;
        node->next = next;
        next = node;
        node->prev = this->shared_from_this();
    }
    LinkedNodePtr<T> remove()
    {
        LinkedNodePtr removeNode = this->shared_from_this();
        LinkedNodePtr prevNode = getPrev();
        LinkedNodePtr nextNode = getNext();
        if (prevNode)
            prevNode->next = nextNode;
        if (nextNode)
            nextNode->prev = prevNode;

        // clean up the removed node
        nodes->erase(removeNode); // remove from the shared list
        nodes = LinkedNodeSet<T>(new std::set<LinkedNodePtr<T>>()); // create a new list
        nodes->insert(removeNode);   // make node the only one in its list
        prev.reset();
        next.reset();
        return removeNode;
    }
    LinkedNodePtr<T> head()
    {
        auto node = this->shared_from_this();
        for (; node->prev; node = node->prev) {}
        return node;
    }
    LinkedNodePtr<T> tail()
    {
        auto node = this->shared_from_this();
        for (; node->next; node = node->next) {}
        return node;
    }
    LinkedNodePtr<T> getPrev()
    {
        return prev;
    }
    LinkedNodePtr<T> getNext()
    {
        return next;
    }
};

template <typename T>
LinkedNodePtr<T> create(std::vector<T> data)
{
    if (data.size() == 0)
        throw std::exception("Can't create the list from empty data");
    LinkedNodePtr<T> firstNode = LinkedNode<T>::create(data[0]);
    for (size_t index = 1; index < data.size(); ++index)
        firstNode->tail()->insertAfter(LinkedNode<T>::create(data[index]));
    return firstNode;
}
