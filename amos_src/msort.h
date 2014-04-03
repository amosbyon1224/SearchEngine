#ifndef MSORT_H
#define MSORT_H

#include <cstdlib>
#include <deque>

template <class T, class Comparator>
void mergeSort (std::deque<T>& myArray, Comparator comp){
	if(1 < myArray.size()){
		std::deque<T> arr1(myArray.begin(), myArray.begin()+myArray.size()/2);
		mergeSort(arr1, comp);
		std::deque<T> arr2(myArray.begin()+myArray.size()/2, myArray.end());
		mergeSort(arr2, comp);

		myArray = merge(arr1, arr2, comp);
	}
}

template <class T, class Comparator>
std::deque<T> merge(std::deque<T> arr1, std::deque<T> arr2, Comparator comp){
	std::deque<T> result;
	while(!arr1.empty() && !arr2.empty()){
		if(comp(arr1.front(), arr2.front())){
			result.push_back(arr1.front());
			arr1.pop_front();
		}else{
			result.push_back(arr2.front());
			arr2.pop_front();
		}
	}
	if(arr1.empty() && !arr2.empty()){
		while(!arr2.empty()){
			result.push_back(arr2.front());
			arr2.pop_front();
		}
	}else if(!arr1.empty() && arr2.empty()){
		while(!arr1.empty()){
			result.push_back(arr1.front());
			arr1.pop_front();
		}
	}
	return result;
}

#endif