#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>
#include "index_pq.hpp"
#include "my_integer.hpp"

TEST(IndexPriorityQueueTest, pushOneInt) {
  IndexPriorityQueue<int> heap(1) ;
  heap.push(1, 0);
  ASSERT_EQ(heap.top().first, 1);
  ASSERT_EQ(heap.top().second, 0);
}

TEST(IndexPriorityQueueTest, pushOneDouble) {
  IndexPriorityQueue<double> heap(1) ;
  heap.push(0.53, 0);
  ASSERT_DOUBLE_EQ(heap.top().first, 0.53);
}

TEST(IndexPriorityQueueTest, pushAFewElements) {
  IndexPriorityQueue<int> heap(4);
  heap.push(1, 2);
  heap.push(2, 0);
  heap.push(3, 3);
  heap.push(4, 1);
  ASSERT_EQ(heap.top().first, 1);
  ASSERT_EQ(heap.top().second, 2);
  ASSERT_TRUE(heap.contains(3));
  ASSERT_TRUE(heap.contains(1));
}

TEST(IndexPriorityQueueTest, pushAndPopOne) {
  IndexPriorityQueue<int> heap(1) ;
  heap.push(1, 0);
  ASSERT_TRUE(heap.contains(0));
  heap.pop();
  ASSERT_TRUE(heap.empty());
  ASSERT_FALSE(heap.contains(0));
}

TEST(IndexPriorityQueueTest, eraseOne) {
  IndexPriorityQueue<std::string> heap(2);
  heap.push("hello", 0);
  heap.push("world", 1);
  ASSERT_TRUE(heap.contains(0));
  ASSERT_TRUE(heap.contains(1));
  heap.erase(1);
  ASSERT_FALSE(heap.empty());
  ASSERT_TRUE(heap.contains(0));
  ASSERT_FALSE(heap.contains(1));
}

TEST(IndexPriorityQueueTest, eraseTwiceDoNothing) {
  IndexPriorityQueue<std::string> heap(2);
  heap.push("hello", 0);
  heap.push("world", 1);
  heap.erase(1);
  heap.erase(0);
  ASSERT_TRUE(heap.empty());
  heap.erase(1);
  heap.push("yadayada", 1);
  heap.erase(0);
  ASSERT_EQ(heap.size(), 1);
}

TEST(IndexPriorityQueueTest, containsWithIndexOutOfRange) {
  IndexPriorityQueue<std::string> heap(3);
  heap.push("hello", 1);
  heap.push("world", 2);
  heap.push("!!", 0);
  ASSERT_TRUE(heap.contains(0));
  ASSERT_TRUE(heap.contains(1));
  ASSERT_TRUE(heap.contains(2));
  ASSERT_FALSE(heap.contains(3));
  ASSERT_FALSE(heap.contains(-1));
}

TEST(IndexPriorityQueueTest, dontPushSameIndexTwice) {
  IndexPriorityQueue<std::string> heap(3);
  heap.push("hello", 1);
  heap.push("world", 2);
  heap.push("abracadabra", 1);
  ASSERT_EQ(heap.top().first, "hello");
  ASSERT_EQ(heap.size(), 2);
}

TEST(IndexPriorityQueueTest, pushAndPop) {
  IndexPriorityQueue<int> heap(4);
  heap.push(4, 0);
  heap.push(2, 3);
  heap.push(1, 1);
  heap.push(3, 2);
  ASSERT_EQ(heap.top().first, 1);
  ASSERT_EQ(heap.top().second, 1);
  heap.pop();
  ASSERT_EQ(heap.top().first, 2);
  ASSERT_EQ(heap.top().second, 3);
  heap.pop();
  ASSERT_EQ(heap.top().first, 3);
  ASSERT_EQ(heap.top().second, 2);
  heap.pop();
  ASSERT_EQ(heap.top().first, 4);
  ASSERT_EQ(heap.top().second, 0);
  heap.pop();
}

TEST(IndexPriorityQueueTest, decreaseKey) {
  IndexPriorityQueue<int> heap(4);
  heap.push(1, 2);
  heap.push(2, 0);
  heap.push(3, 3);
  heap.push(4, 1);
  heap.changeKey(-1, 0);
  ASSERT_EQ(heap.top().first, -1);
  ASSERT_EQ(heap.top().second, 0);
}

TEST(IndexPriorityQueueTest, increaseKey) {
  IndexPriorityQueue<int> heap(4);
  heap.push(1, 2);
  heap.push(2, 0);
  heap.push(3, 3);
  heap.push(4, 1);
  heap.changeKey(5, 2);
  ASSERT_EQ(heap.top().first, 2);
  ASSERT_EQ(heap.top().second, 0);
  heap.pop();
  ASSERT_EQ(heap.top().first, 3);
  ASSERT_EQ(heap.top().second, 3);
  heap.pop();
  ASSERT_EQ(heap.top().first, 4);
  ASSERT_EQ(heap.top().second, 1);
  heap.pop();
  ASSERT_EQ(heap.top().first, 5);
  ASSERT_EQ(heap.top().second, 2);
  heap.pop();
  EXPECT_TRUE(heap.empty());
}


TEST(IndexPriorityQueueTest, changeKeyToInsert) {
  IndexPriorityQueue<int> heap(4);
  heap.changeKey(1, 2);
  ASSERT_EQ(heap.top().first, 1);
  ASSERT_EQ(heap.top().second, 2);
  heap.changeKey(0, 3);
  ASSERT_EQ(heap.top().first, 0);
  ASSERT_EQ(heap.top().second, 3);
  heap.changeKey(2, 3);
  ASSERT_EQ(heap.top().first, 1);
  ASSERT_EQ(heap.top().second, 2);
  heap.changeKey(0, 0);
  ASSERT_EQ(heap.top().first, 0);
  ASSERT_EQ(heap.top().second, 0);
}

TEST(IndexPriorityQueueTest, eraseSink) {
  IndexPriorityQueue<double> heap(7);
  heap.push(0.5, 0);
  heap.push(0.75, 1);
  heap.push(1.0, 2);
  heap.push(1.25, 3);
  heap.push(1.50, 4);
  heap.push(1.75, 5);
  heap.push(2.0, 6);
  heap.erase(1);
  EXPECT_DOUBLE_EQ(heap.top().first, 0.5);
  heap.pop(); 
  for (int i = 0; i < 5; ++i) {
    EXPECT_DOUBLE_EQ(heap.top().first, 1.0 + 0.25 * i);
    heap.pop(); 
  }
}

TEST(IndexPriorityQueueTest, eraseSwim) {
  IndexPriorityQueue<double> heap(7);
  heap.push(0.5, 0);
  heap.push(1.50, 1);
  heap.push(0.75, 2);
  heap.push(1.75, 3);
  heap.push(2.0, 4);
  heap.push(1.0, 5);
  heap.push(1.25, 6);
  heap.erase(4);
  for (int i = 0; i < 6; ++i) {
    EXPECT_DOUBLE_EQ(heap.top().first, 0.5 + 0.25 * i);
    heap.pop(); 
  }
}

TEST(IndexPriorityQueueTest, eraseAtEnd) {
  IndexPriorityQueue<double> heap(7);
  heap.push(0.5, 0);
  heap.push(1.50, 1);
  heap.push(0.75, 2);
  heap.push(1.75, 3);
  heap.push(2.0, 4);
  heap.push(1.0, 5);
  heap.push(1.25, 6);
  heap.erase(6);
  for (int i = 0; i < 3; ++i) {
    EXPECT_DOUBLE_EQ(heap.top().first, 0.5 + 0.25 * i);
    heap.pop(); 
  }
  for (int i = 0; i < 3; ++i) {
    EXPECT_DOUBLE_EQ(heap.top().first, 1.5 + 0.25 * i);
    heap.pop(); 
  }
}

void eraseHelper(int N, unsigned seed) {
  std::mt19937 mt {seed};
  std::vector<int> priorities(N);
  std::iota(priorities.begin(), priorities.end(), -5);
  std::shuffle(priorities.begin(), priorities.end(), mt);
  std::vector<int> operationOrder(N);
  std::iota(operationOrder.begin(), operationOrder.end(), 0);
  std::shuffle(operationOrder.begin(), operationOrder.end(), mt);
  IndexPriorityQueue<int> heap(N);
  // add elements according indices in operationOrder
  for (int i = 0; i < N; ++i) {
    int index = operationOrder.at(i);
    heap.push(priorities.at(index), index);
    ASSERT_EQ(heap.size(), i + 1);
  }
  std::shuffle(operationOrder.begin(), operationOrder.end(), mt);
  auto priority_copy {priorities};
  std::sort(priority_copy.begin(), priority_copy.end());
  // erase elements according indices in operationOrder
  for (int i = 0; i < N; ++i) {
    ASSERT_EQ(heap.top().first, priority_copy.front());  
    int index = operationOrder.at(i);
    heap.erase(index); 
    int priority = priorities.at(index);
    auto erase_it = 
      std::find(priority_copy.begin(), priority_copy.end(), priority);
    priority_copy.erase(erase_it);
  }
}

TEST(IndexPriorityQueueTest, eraseRandomOrder) {
  eraseHelper(8, 1'234);
}

TEST(IndexPriorityQueueTest, emptyAndBackPop) {
  IndexPriorityQueue<std::string> heap(3);
  heap.push("hello", 1);
  heap.push("world", 2);
  heap.push("abracadabra", 0);
  ASSERT_EQ(heap.top().first, "abracadabra");
  heap.pop();
  ASSERT_EQ(heap.top().first, "hello");
  heap.pop();
  ASSERT_EQ(heap.top().first, "world");
  heap.pop();
  ASSERT_TRUE(heap.empty());
  heap.push("yada", 1);
  ASSERT_EQ(heap.top().first, "yada");
  ASSERT_EQ(heap.size(), 1);
}

TEST(IndexPriorityQueueTest, emptyAndBackErase) {
  IndexPriorityQueue<std::string> heap(3);
  heap.push("hello", 1);
  heap.push("world", 2);
  heap.push("abracadabra", 0);
  ASSERT_EQ(heap.top().first, "abracadabra");
  heap.erase(1);
  ASSERT_EQ(heap.top().first, "abracadabra");
  ASSERT_EQ(heap.size(), 2);
  heap.erase(0);
  ASSERT_EQ(heap.top().first, "world");
  heap.erase(2);
  ASSERT_TRUE(heap.empty());
  heap.push("yada", 1);
  ASSERT_EQ(heap.top().first, "yada");
  ASSERT_EQ(heap.size(), 1);
}

TEST(IndexPriorityQueueTest, operationsOnPrioritiesSmall) {
  const int N {4};
  std::vector<MyInteger> priorities 
  { MyInteger {3}, MyInteger {1}, MyInteger {-10}, MyInteger {50} };
  std::vector<int> indices {3, 0, 2, 1};
  IndexPriorityQueue<MyInteger> heap(N);
  MyInteger::clearCounts();
  for (int i = 0; i < N; ++i) {
    heap.push(priorities.at(i), indices.at(i));
  }
  ASSERT_LE(MyInteger::assignmentCount, N);
  ASSERT_LE(MyInteger::copyCount, N);
  ASSERT_LE(MyInteger::constructorCount, N);
  std::sort(priorities.begin(), priorities.end());
  MyInteger::clearCounts();
  for (const auto& x : priorities) {
    ASSERT_EQ(x, heap.top().first);
    heap.pop();
  }
  ASSERT_LE(MyInteger::assignmentCount, N);
  ASSERT_LE(MyInteger::copyCount, N);
  ASSERT_LE(MyInteger::constructorCount, N);
}

TEST(IndexPriorityQueueTest, operationsOnPrioritiesBig) {
  const int N {100};
  std::mt19937 mt {42};
  std::vector<int> intPriorities(N);
  std::iota(intPriorities.begin(), intPriorities.end(), 101);
  std::shuffle(intPriorities.begin(), intPriorities.end(), mt);
  std::vector<MyInteger> priorities(N);
  for (std::size_t i = 0; i < N; ++i) {
    priorities.at(i) = MyInteger {intPriorities.at(i)};
  } 
  std::vector<int> indices(N);
  std::iota(indices.begin(), indices.end(), 0);
  std::shuffle(indices.begin(), indices.end(), mt);
  IndexPriorityQueue<MyInteger> heap(N);
  MyInteger::clearCounts();
  for (int i = 0; i < N; ++i) {
    heap.push(priorities.at(i), indices.at(i));
  }
  ASSERT_LE(MyInteger::assignmentCount, N);
  ASSERT_LE(MyInteger::copyCount, N);
  ASSERT_LE(MyInteger::constructorCount, N);
  std::sort(priorities.begin(), priorities.end());
  MyInteger::clearCounts();
  for (const auto& x : priorities) {
    ASSERT_EQ(x, heap.top().first);
    heap.pop();
  }
  ASSERT_LE(MyInteger::assignmentCount, N);
  ASSERT_LE(MyInteger::copyCount, N);
  ASSERT_LE(MyInteger::constructorCount, N);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
