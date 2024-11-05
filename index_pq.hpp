#ifndef INDEX_PRIORITY_QUEUE_HPP_
#define INDEX_PRIORITY_QUEUE_HPP_

#include <vector>
#include <algorithm>

template <typename T>
class IndexPriorityQueue {
private:
    // vector to hold priorities.  
    // priorities.at(i) is the priority associated to index i
    // the heap will only contain indices, we look up priorities
    // in this vector as needed
    std::vector<T> priorities{};
    // priorityQueue stores indices: priorityQueue.at(i) is an index
    // priorityQueue functions as the heap and is heap ordered: 
    // priorities.at(priorityQueue.at(i)) <= priorities.at(priorityQueue.at(2 * i)) 
    // priorities.at(priorityQueue.at(i)) <= priorities.at(priorityQueue.at(2 * i) + 1) 
    std::vector<int> priorityQueue{};
    // indexToPosition.at(i) is the position in priorityQueue of index i
    // priorityQueue.at(indexToPosition.at(i)) = i
    // indexToPosition.at(priorityQueue.at(j)) = j
    std::vector<int> indexToPosition{};
    int size_ = 0;
    int const EMPTY_VAR = -1;
    T const EMPTY_P = T();

public:
    explicit IndexPriorityQueue(int);
    void mySwap(int, int);
    bool isSmaller(int, int);
    bool isValidIndex(int);
    void push(const T&, int);
    void pop();
    void erase(int);
    bool contains(int) const;
    void changeKey(const T&, int);
    std::pair<T, int> top() const;
    bool empty() const;
    int size() const;

private:
    // TODO: you may want to add your own member functions. swim? sink?
    // Useful helper functions
    int leftChild(int i) {
        return 2 * i;
    }

    int rightChild(int i) {
        return 2 * i + 1;
    }

    int parent(int i) {
        return i / 2;
    }
    // From week 7 slides
    void swim(int index) {
        while ((index > 1) && isSmaller(index, parent(index))) {
            mySwap(index, parent(index));
            index = parent(index);
        }
    }
    /// Source = https://dotkay.github.io/2018/03/15/min-heap-sink-swim/
    // Need to check on final testcase - why is it swapping the wrong child?
    void sink(int index) {
        while (leftChild(index) <= size_) {
            int j = leftChild(index);
            if ((index < size_) && isSmaller(rightChild(index), leftChild(index))) {
                j++;
            }
            if (!isSmaller(j, index)) {
                break;
            }
            mySwap(j, index);
            index = j;
        }
    }
};


// IndexPriorityQueue member functions
template <typename T>
IndexPriorityQueue<T>::IndexPriorityQueue(int N) {
    for (int i = 0; i < N + 1; ++i) {
        priorities.push_back(EMPTY_P);
        priorityQueue.push_back(EMPTY_VAR);
        indexToPosition.push_back(EMPTY_VAR);
    }
}
// mySwap function is used to swap the indices in both the prioirtyQueue and indexToPosition vector.
// It is used whenever a node needs to be switched with a parent or a child.
template <typename T>
void IndexPriorityQueue<T>::mySwap(int index1, int index2){
    std::swap(indexToPosition.at(priorityQueue.at(index1)), indexToPosition.at(priorityQueue.at(index2)));
    std::swap(priorityQueue.at(index1), priorityQueue.at(index2));
}

// This function determins if an index is valid in the prioirty queue. The list queue always
// starts at index 1, leaving the zero index empty at all times, meaning the index can be between 1 and the size of the 
// priority queue, inclusive.
template <typename T>
bool IndexPriorityQueue<T>::isValidIndex(int index) {
    return (index > 0 && index <= size_);
}

// This is used to check if the priority of one index is smaller than the other
// by checking its prioirty. It is essentially index1 < index2, in terms of their respective
// priorities.
template <typename T>
bool IndexPriorityQueue<T>::isSmaller(int index1, int index2) {
    if (isValidIndex(index1) && isValidIndex(index2)) {
        return priorities.at(priorityQueue.at(index1)) <= priorities.at(priorityQueue.at(index2));
    }
    return false;
}

template <typename T>
bool IndexPriorityQueue<T>::empty() const {
    return size_ == 0;
}

template <typename T>
int IndexPriorityQueue<T>::size() const {
    return size_;
}

template <typename T>
void IndexPriorityQueue<T>::push(const T& priority, int index) {
    if (!contains(index)) {
        size_++;
        priorities[index] = priority;
        priorityQueue[size_] = index;
        indexToPosition[index] = size_;
        swim(size_);
    }
}

template <typename T>
void IndexPriorityQueue<T>::pop() {
    if (empty()) {
        return;
    }
    else if (size_ == 1) {
        int index = priorityQueue[1];
        indexToPosition[index] = EMPTY_VAR;
        priorityQueue[1] = EMPTY_VAR;
        priorities[index] = EMPTY_P;
        size_--;
    }
    else {
        mySwap(1, size_);
        indexToPosition.at(priorityQueue.at(size_)) = EMPTY_VAR;
        priorities[priorityQueue.at(size_)] = EMPTY_P;
        priorityQueue[size_] = EMPTY_VAR;
        size_--;
        sink(1);
    }
}

template <typename T>
void IndexPriorityQueue<T>::erase(int index) {
    if(static_cast<std::size_t>(index) > priorities.size()){
        return;
    }
    int eraseIndex = indexToPosition.at(index); 
    if(eraseIndex==EMPTY_VAR || empty() || eraseIndex == 0){
        return;
    }
    if(eraseIndex == 1){
        pop();
        return;
    }
    int lastIndex = size_;
    mySwap(eraseIndex, lastIndex);
    indexToPosition.at(priorityQueue.at(lastIndex)) = EMPTY_VAR;
    priorities.at(priorityQueue.at(lastIndex)) = EMPTY_P;
    priorityQueue.at(lastIndex) = EMPTY_VAR;
    size_--;
    // As you don't know if the relative position of the index,
    // you have to swim and sink it to make sure that it lands in the correct location
    // within the tree.
    sink(eraseIndex);
    swim(eraseIndex);
}

template <typename T>
std::pair<T, int> IndexPriorityQueue<T>::top() const {
    return { priorities.at(priorityQueue.at(1)), priorityQueue.at(1) };
}

// if vertex i is not present, insert it with key
// otherwise change the associated key value of i to key
template <typename T>
void IndexPriorityQueue<T>::changeKey(const T& key, int index) {
    if (indexToPosition[index] == EMPTY_VAR || empty()) {
        push(key, index);
    }
    else {
        T oldPriority = priorities.at(index);
        priorities.at(index) = key;
        if (priorities.at(index) > oldPriority) {
            sink(indexToPosition.at(index));
        }
        else {
            swim(indexToPosition.at(index));
        }
    }
}

template <typename T>
bool IndexPriorityQueue<T>::contains(int index) const {
    if (index > size_ || index < 0 || empty()) {
        return false;
    }
    else {
        return priorities.at(index) != EMPTY_P;
    }
}


#endif      // INDEX_PRIORITY_QUEUE_HPP_
