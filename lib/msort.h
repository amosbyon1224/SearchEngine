#ifndef MSORT_H
#define MSORT_H

#include <cstdlib>
#include <deque>

//recursive function to merge sort a deque
template <class T, class Comparator>
void mergeSort (std::deque<T>& myArray, Comparator comp){
	if(1 < myArray.size()){
		//splits the bottom half of array and sorts it
		std::deque<T> arr1(myArray.begin(), myArray.begin()+myArray.size()/2);
		mergeSort(arr1, comp);

		//splits the top half of an array and sorts it
		std::deque<T> arr2(myArray.begin()+myArray.size()/2, myArray.end());
		mergeSort(arr2, comp);

		//merges two arrays together in order
		myArray = merge(arr1, arr2, comp);
	}
}

//function to merge two arrays together in order
template <class T, class Comparator>
std::deque<T> merge(std::deque<T> arr1, std::deque<T> arr2, Comparator comp){
	std::deque<T> result;

	//initial loop through arrays to place them in order
	while(!arr1.empty() && !arr2.empty()){
		if(comp(arr1.front(), arr2.front())){
			result.push_back(arr1.front());
			arr1.pop_front();
		}else{
			result.push_back(arr2.front());
			arr2.pop_front();
		}
	}

	//extra check in case one array was larger than the other
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