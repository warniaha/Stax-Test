#include <iostream>
#include <string>
#include <vector>
#include "Application.h"
#include "LinkedNode.h"

template <typename T>
void writeLinkedList(LinkedNodePtr<T> node)
{
	for (size_t counter = 1; node; node = node->getNext(), ++counter)
		std::cout << counter << ": " << node->value() << std::endl;
}

void Application::run()
{
	std::vector<std::string> data1{ "three", "five", "seven" };
	std::vector<std::string> data2{ "two", "four", "six" };
	LinkedNodePtr<std::string> node1 = create<std::string>(data1);
	LinkedNodePtr<std::string> node2 = create<std::string>(data2);
	std::cout << "List #1:" << std::endl;
	writeLinkedList<>(node1);
	std::cout << "List #2:" << std::endl;
	writeLinkedList<>(node2);

	auto node = node1->getNext()->remove();
	std::cout << "List #1 five should be missing:" << std::endl;
	writeLinkedList<>(node1);
	std::cout << "five: " << node->value() << std::endl;
}
