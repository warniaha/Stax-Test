#include "pch.h"
#include "CppUnitTest.h"
#include "LinkedNode.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using IntLinkedNode = LinkedNodePtr<int>;
using DoubleLinkedNode = LinkedNodePtr<double>;
using StringLinkedNode = LinkedNodePtr<std::string>;

struct Data
{
	int integerValue;
	double doubleValue;
	std::string stringValue;
};
using DataLinkedNode = LinkedNodePtr<Data>;

namespace StaxUnitTest
{
	TEST_CLASS(StaxUnitTest)
	{
	public:
		TEST_METHOD(TestLinkedNodeInsertInt)
		{
			std::vector<int> data{ 3, 5, 7 };
			IntLinkedNode node = create<int>(data);
			Assert::AreEqual(data.size(), node->size(), L"create failed to create a list with the proper number of elements");
			node->insertAfter(LinkedNode<int>::create(4));
			Assert::AreEqual(node->size(), data.size()+1, L"failed to add to the list");
			auto loop = node;
			size_t count = 0;
			for (; loop; ++count, loop = loop->getNext());
			Assert::AreEqual(node->size(), count, L"element count is wrong");
		}
		TEST_METHOD(TestLinkedNodeInsertDuplicate)
		{
			std::vector<int> data{ 3, 5, 7 };
			IntLinkedNode node = create<int>(data);
			Assert::AreEqual(data.size(), node->size(), L"create failed to create a list with the proper number of elements");
			node->insertAfter(LinkedNode<int>::create(4));
			Assert::ExpectException<std::exception>([&] { node->tail()->insertAfter(node); });
		}
		TEST_METHOD(TestLinkedNodeStructInsertHead)
		{
			std::vector<Data> data{ { 3, 3.3, "three" }, { 5, 5.5, "five"}, { 7, 7.7, "seven"} };
			DataLinkedNode node = create<Data>(data);
			Assert::AreEqual(data.size(), node->size(), L"create failed to create a list with the proper number of elements");
			node->head()->insertBefore(LinkedNode<Data>::create(Data{ 4, 4.4, "four" }));
			Assert::AreEqual(node->head()->value().integerValue, 4, L"element not inserted at head");
			Assert::AreEqual(node->head()->getNext()->value().integerValue, 3, L"original head not in second position");
		}
		TEST_METHOD(TestLinkedNodeStringInsertAfterTail)
		{
			std::vector<std::string> data{ "three", "five", "seven"};
			StringLinkedNode node = create<std::string>(data);
			Assert::AreEqual(data.size(), node->size(), L"create failed to create a list with the proper number of elements");
			node->tail()->insertAfter(LinkedNode<std::string>::create("four"));
			Assert::AreEqual(node->tail()->value(), std::string("four"), L"expected element not at tail");
		}
		TEST_METHOD(TestLinkedNodeStringFromAnotherList)
		{
			std::vector<std::string> data1{ "three", "five", "seven" };
			std::vector<std::string> data2{ "two", "four", "six" };
			StringLinkedNode node1 = create<std::string>(data1);
			StringLinkedNode node2 = create<std::string>(data2);
			Assert::AreEqual(data1.size(), node1->size(), L"create failed to create a list with the proper number of elements");
			Assert::AreEqual(data2.size(), node2->size(), L"create failed to create a list with the proper number of elements");
			Assert::ExpectException<std::exception>([&] { node1->insertAfter(node2); });
		}
		TEST_METHOD(TestLinkedNodeMoveStringFromAnotherList)
		{
			std::vector<std::string> data1{ "three", "five", "seven" };
			std::vector<std::string> data2{ "two", "four", "six" };
			StringLinkedNode node1 = create<std::string>(data1);
			StringLinkedNode node2 = create<std::string>(data2);
			Assert::AreEqual(data1.size(), node1->size(), L"create failed to create a list with the proper number of elements");
			Assert::AreEqual(data2.size(), node2->size(), L"create failed to create a list with the proper number of elements");
			auto node = node1->getNext()->remove();
			Assert::AreEqual(data1.size()-1, node1->size(), L"size after remove is wrong");
			node2->tail()->insertAfter(node);
			Assert::AreEqual(data2.size()+1, node2->size(), L"size after reinsert is wrong");
			std::vector<std::string> expected1{ "three", "seven" };
			std::vector<std::string> expected2{ "two", "four", "six", "five" };
			size_t index = 0;
			for (auto expNode = node1; expNode; expNode = expNode->getNext(), ++index)
			{
				Assert::AreEqual(expNode->value(), expected1[index], L"Values don't match after removal");
			}
			index = 0;
			for (auto expNode = node2; expNode; expNode = expNode->getNext(), ++index)
			{
				Assert::AreEqual(expNode->value(), expected2[index], L"Values don't match after reinsert");
			}
		}
		TEST_METHOD(TestLinkedNodeRemove)
		{
			std::vector<std::string> data{ "three", "five", "seven" };
			StringLinkedNode node = create<std::string>(data);
			auto savedNode = node->getNext();	// this will be the new head of the list
			auto headNode = node->head()->remove();
			node = savedNode;
			auto tailNode = node->tail()->remove();
			Assert::AreEqual(headNode->value(), data[0], L"headNode doesn't have the expected value");
			Assert::AreEqual(savedNode->value(), data[1], L"savedNode doesn't have the expected value");
			Assert::AreEqual(tailNode->value(), data[2], L"tailNode doesn't have the expected value");
			Assert::AreEqual(headNode->size(), (size_t)1, L"headNode doesn't have the correct number of nodes");
			Assert::AreEqual(tailNode->size(), (size_t)1, L"tailNode doesn't have the correct number of nodes");
			Assert::AreEqual(savedNode->size(), (size_t)1, L"savedNode doesn't have the correct number of nodes");
		}
	};
}
