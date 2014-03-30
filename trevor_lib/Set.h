#ifndef SET_H
#define SET_H
#include <set>

template <class T>
class Set : public std::set<T> {
    public: 
        Set ();                     // constructor for an empty set
        Set (const Set<T> & other); // copy constructor, making a deep copy
        ~Set ();                    // destructor

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
        // other private variables you think you need.
};

template <class T>
// Constructor
Set<T>::Set() : std::set<T>() {}

template <class T>
// Copy constructor
Set<T>::Set(const Set<T> &other) : std::set<T>(other) {}

template <class T>
// Destructor, doesn't need to do anything
Set<T>::~Set()  {}

// Set intersection
template <class T>
Set<T> Set<T>::setIntersection(const Set<T> &other) const   {
    Set<T> result;
    for(typename std::set<T>::iterator it = std::set<T>::begin(); it != std::set<T>::end(); ++it)   {
        if(other.find(*it) != other.end()) result.insert(*it);
    }
    return result;
}

template <class T>
Set<T> Set<T>::setUnion(const Set<T> &other) const  {
    Set<T> result(other);
    for(typename std::set<T>::iterator it = std::set<T>::begin(); it!= std::set<T>::end(); ++it)   {
        result.insert(*it);
    }
    return result;
}
#endif
