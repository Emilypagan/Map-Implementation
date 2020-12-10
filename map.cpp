/** CS515 Assignment 7
 * File: map.cpp
 * Name: Mily Pagan
 * Section: 03
 * Date: 11/30/20
 * Collaboration Declaration: piazza */

// A non-generic Map ADT implemented with a BST data structure
// The BST is not-balanced and non-threaded
#include "map.h"

//map constructor
Map::Map(){
	// create a dummy root node and set up an empty tree
	_root = new Elem;
	_root->left = _root;  
	_root->right = 0;
	_size = 0;
}
// copy constructor
Map::Map(const Map &v){
	// if empty tree
	if (v._root == v._root->left){
		_root = new Elem;
		_root->left = _root;  // empty tree
		_root->right = 0;
		_size = 0;
	} else {
		_root = new Elem;
		_root->left = _root;
		_root->right = 0;
		copyCode(_root->left, v._root->left);
		_size = v._size;
	}
}
//destructor
Map::~Map() {
    if (_size == 0)
        delete _root;
    else {
        destructCode( _root->left );
        delete _root;
    }
}
//assignment operator
Map& Map::operator=(const Map &rhs) {
    if(this == &rhs) {
        return *this;
    }
    if(_size > 0){
        destructCode(_root);
    } else {
        delete _root;
    }

    _root = new Elem;
    _root->left = _root;
    _root->right = 0;
    copyCode( _root ->left, rhs._root->left );
    _size = rhs._size;
    return *this;
}
//insert
bool Map::insert(KEY_TYPE key, VALUE_TYPE data) {
    if (insert(_root->left, key, data)) {
        return true;
    }
    return false;
}
//remove
bool Map::erase(KEY_TYPE key) {
    Iterator i = find(key);
    //Iterator j(_root);
    if(i == end()) {
        return false;
    }
    Elem* prev = _root;
    Elem* nodeToDelete = _root->left;
    bool isRight = 0;
    //finding the desired key and getting its parent
    while(nodeToDelete->key != key){
        if(key < nodeToDelete->key){
            prev = nodeToDelete;
            nodeToDelete = nodeToDelete->left;
            isRight = 0;
        } else {
            prev = nodeToDelete;
            nodeToDelete = nodeToDelete->right;
            isRight = 1;
        }
    }

    //if node to delete is a leaf node
    if(nodeToDelete->left == 0 && nodeToDelete->right == 0) { //zero children
        if(isRight) {
            prev->right = NULL;
        } else {
            prev->left = NULL;
        }
        delete nodeToDelete;
        _size--;
        return true;
    } else if ((nodeToDelete->left == NULL) && (nodeToDelete->right != NULL)) { //one child on the right

        if (isRight) {
            prev->right = nodeToDelete->right;
        } else {
            prev->left = nodeToDelete->right;
        }
        delete nodeToDelete;
        _size--;
        return true;

    } else if ((nodeToDelete->left != NULL)  && (nodeToDelete->right == NULL)) { //one child on the left

        if (isRight) {
            prev->right = nodeToDelete->left;
        } else {
            prev->left = nodeToDelete->left;
        }
        delete nodeToDelete;
        _size--;
        return true;

    } else { //both children
        Elem* m = nodeToDelete->right;
        Elem* f = nodeToDelete;
        string tempKey;
        string tempVal;
        if(m->left == NULL) {
            tempKey = m->key;
            tempVal = m->data;
            m->key = nodeToDelete->key;
            m->data = nodeToDelete->data;
            nodeToDelete->key = tempKey;
            nodeToDelete->data = tempVal;
            //previous points to same location
            nodeToDelete->right = m->right;
            delete m;
            _size--;
            return true;
        }
        while( m->left != 0 ) {
            f = m ;
            m = m->left;
        }
        tempKey = m->key;
        tempVal = m->data;
        m->key = nodeToDelete->key;
        m->data = nodeToDelete->data;
        nodeToDelete->key = tempKey;
        nodeToDelete->data = tempVal;
        f->left = m->right;
        delete m;
        _size--;
        return true;
    }
}
//size
int Map::size() const {
    return _size;
}
//find
Map::Iterator Map::find(KEY_TYPE key) const {
    Elem *cur = _root->left;
    while (cur) {
        if (key < cur->key) {
            cur = cur->left;
        } else if (cur->key < key) {
            cur = cur->right;
        } else {
            Iterator i(cur);
            return i;
        }
    }
    Elem *cur2 = _root->right;
    Iterator i2(cur2);
    return i2;
}
//begin iterator
Map::Iterator Map::begin() const {
    Elem *b = _root->left;
    Iterator i(b);
    return i;
}
//end iterator
Map::Iterator Map::end() const {
    Elem *e = _root->right;
    Iterator i(e);
    return i;
}
//overload [] operator
VALUE_TYPE& Map::operator[](KEY_TYPE k) {
    string temp = "";
    Elem* e;
    Iterator it = find(k);
    if(it != end()){
        e = &*it;
        return e->data;
    } else {
        insert(k,temp);
        it = find(k);
        e = &*it;
        return e->data;
    }
}
//////////////////////////////////////////////////////////////////////////////////////
//overloading * operator
Map::Elem& Map::Iterator::operator*() {
    return *_cur;
}
//overloading -> operator
Map::Elem* Map::Iterator::operator->() {
    return _cur;
}
//equality == operator
bool Map::Iterator::operator==(Iterator it) const {
    return _cur == it._cur;
}
//inequality != operator
bool Map::Iterator::operator!=(Iterator it) const {
    return _cur != it._cur;
}
//insert
bool Map::insert(Elem *& root, const KEY_TYPE& key, const VALUE_TYPE& data) {
    Elem *n = new Elem;
    n->key = key;
    n->data = data;
    Iterator i = find(key);
    if (i == end()){ //did not find
       if (_size == 0) {
           n->left = NULL;
           n->right = NULL;
           _root->left = n;
           _size++;
           return true;
       } else {
           Elem *cur = _root->left;
           while (cur) {
               if (key < cur->key) {
                   //go left - if left is empty add node there
                   if (cur->left == NULL) {
                       cur->left = n;
                       n->left = NULL;
                       n->right = NULL;
                       _size++;
                       return true;
                   } else {
                       cur = cur->left;
                   }
               } else if (cur->key < key) {
                   //go right - if right is empty add node there
                   if (cur->right == NULL) {
                       cur->right = n;
                       n->left = NULL;
                       n->right = NULL;
                       _size++;
                       return true;
                   } else {
                       cur = cur->right;
                   }
               }
           }
       }

    } else {
        delete n;
        return false;
    }
}
// common code for deallocation
void Map::destructCode(Elem *& p) {
    if(p->left != NULL){
        destructCode( p->left );
    }
    if(p->right != NULL){
        destructCode( p->right );
    }
    delete p;
}

/////////////////////////////////////////////////////////////////////////////////////////
// common copy code for deep copy a tree
void Map::copyCode(Elem* &newRoot, Elem* origRoot){
	if (origRoot == 0)
		newRoot = 0;
	else{
		newRoot = new Elem;
		newRoot->key = origRoot->key;
		newRoot->data = origRoot->data;
		copyCode(newRoot->left, origRoot->left);
		copyCode(newRoot->right, origRoot->right);
	}
}

// 
//  Do Not Modify Below Here
//
//
// output the structure of tree. The tree is output as "lying down"
// in which _root is the LEFT most Elem.
void Map::printTree(ostream& out, int level, Elem *p) const{
	int i;
	if (p) {
		printTree(out, level+1, p->right);
		for(i=0;i<level;i++) {
			out << "\t";
		}
		out << p->key << " " << p->data << '\n';
		printTree(out, level+1, p->left);
	}
}

// outputs information in tree
ostream& Map::dump(ostream& out) const{
	if ( _root == _root->left) { // tree empty
		return out;
	}
	printTree(out, 0, _root->left);   // print tree structure
	return out;
}

// outputs using overloaded << operator
ostream& operator<< (ostream& out, const Map& v){
	v.dump(out);
	return out;
}
