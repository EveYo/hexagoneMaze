/**
 * @file quack.cpp
 * Implementation of the Quack class.
 *
 */

template <class T> Quack<T>::Quack() {
  n1 = 0;  // index of the first element
  n2 = -1; // index of the last element
}

/**
 * Adds the parameter object to the right of the Quack.
 *
 * @param newItem object to be added to the Quack.
 */
template <class T> void Quack<T>::pushR(T newItem) {
  data.push_back(newItem);
  n2++;
}

/**
 * Removes the object at the left of the Quack, and returns it to the
 * caller.
 *
 * See .h file for implementation notes.
 *
 * @return The item that used to be at the left of the Quack.
 */
template <class T> T Quack<T>::popL() {
    
  T temp = peekL();
  n1++;

  if (isEmpty()) {
    n1 = 0;
    n2 = -1;
    data.resize(0);
    return temp;

  } else if (n1 >= (n2 - n1)+1) { // greater or equal than

    vector<T> newData;
    for (int i = n1; i <= n2; i++) {
      newData.push_back(data[i]);
    }

    data.resize(newData.size());
    for (unsigned long i = 0; i < newData.size(); i++) {
      data[i] = newData[i];
    }
    n1 = 0;
    n2 = newData.size()-1;
    
    return temp;
  } else {
    return temp;
  }

}

/**
 * Removes the object at the right of the Quack, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Quack.
 */
template <class T> T Quack<T>::popR() {
  T temp = data[n2];
  n2--;
  data.pop_back(); //Delete but having problems
  
  if (isEmpty()) {  //n2<n1
    n1 = 0;
    n2 = -1;
    data.resize(0);
    return temp;

  } else if (n1 >= (n2 - n1)+1 ) { 
    // the empty space at left greater or equal than amount of element

    vector<T> newData;
    for (int i = n1; i <= n2; i++) {
      newData.push_back(data[i]);
    }

    data.resize(newData.size());
    for (unsigned long i = 0; i < newData.size(); i++) {
      data[i] = newData[i];
    }
    n1 = 0;
    n2 = newData.size()-1;
  }
  return temp;
}

/**
 * Finds the object at the left of the Quack, and returns it to the
 * caller. Unlike popL(), this operation does not alter the quack.
 *
 * @return The item at the front of the quack.
 */
template <class T> T Quack<T>::peekL() { return data[n1]; }

/**
 * Finds the object at the right of the Quack, and returns it to the
 * caller. Unlike popR(), this operation does not alter the quack.
 *
 * @return the value of The item at the right of the quack.
 */
template <class T> T Quack<T>::peekR() { return data[n2]; }

/**
 * Determines if the Quack is empty.
 *
 * @return bool which is true if the Quack is empty, false otherwise.
 */
template <class T> bool Quack<T>::isEmpty() const { 
  return data.empty(); 
  //return n2<n1;
}
