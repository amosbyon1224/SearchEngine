// necessary includes can go here
#ifndef SET_H
#define SET_H

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <set>

template <class T>
class Set : public std::set<T>{
  public: 
    Set ();                     // constructor for an empty set
    Set (const Set<T> & other); // copy constructor, making a deep copy

    Set<T> setIntersection (const Set<T> & other) const;
      /* Returns the intersection of the current set with other.
         That is, returns the set of all items that are both in this
         and in other. */

    Set<T> setUnion (const Set<T> & other) const;
      /* Returns the union of the current set with other.
         That is, returns the set of all items that are in this set
         or in other (or both).
         The resulting set should not contain duplicates. */

  private:
};

//constructor
template<typename T>
Set<T>::Set() : std::set<T>(){
}

//alt constructor
template<typename T>
Set<T>::Set(const Set<T> & other) : std::set<T>(other){
}

//returns a set of the intersection between this and other
template<typename T>
Set<T> Set<T>::setIntersection(const Set<T> & other) const{
  Set<T> intersectionSet;

  typename std::set<T>::iterator it;

  for(it = this->begin(); it != this->end(); ++it){
    if(other.find(*it) != other.end()){
      intersectionSet.insert(*it);
    }
  }
  return intersectionSet;
}

//returns a set of the union between this and other
template<typename T>
Set<T> Set<T>::setUnion(const Set<T> & other) const{
  Set<T> unionSet(other);

  typename std::set<T>::iterator it;

  for(it = this->begin(); it != this->end(); ++it){
    unionSet.insert(*it);
  }

  return unionSet;
}

#endif