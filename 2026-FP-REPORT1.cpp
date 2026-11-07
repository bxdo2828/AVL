#include <cstdio>
#include <iostream>
#include <algorithm>

template <class _Tp> class Node;
template <class _Tp> class BST;

template <class _Tp>
class Node {
	public: // Member types
		typedef _Tp					__key_type;
		typedef Node<__key_type>*	__node_pointer_type;
	
	public: // Member variables
		__key_type			__key_;
		__node_pointer_type	__left_;
		__node_pointer_type	__right_;
	
	public: // Constructor
		Node(): __key_(__key_type()), __left_(nullptr), __right_(nullptr) {}
		Node(const __key_type& key): __key_(key), __left_(nullptr), __right_(nullptr) {}

	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* 반드시 아래의 함수를 사용해야할 필요는 없습니다.
	*/
};



template <class _NodePtr, class _Tp>
_NodePtr getNodeBST() {
    _NodePtr newNode = new Node<_Tp>();
    return newNode;
}

template <class _NodePtr>
unsigned __height(_NodePtr __x) {
    if(__x == NULL) return 0;

    _NodePtr T = __x;

    if(T->__left_ == NULL && T->__right_ == NULL) {
        return 0;
    } else if(T->__left_ == NULL && T->__right_ != NULL) 
    {
        return __height(T->__right_) + 1;
    } else if(T->__left_ != NULL && T->__right_ == NULL) 
    {
        return __height(T->__left_) + 1;
    } else 
    {
        return std::max(__height(T->__left_), __height(T->__right_)) + 1;
    }

}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
    if(__x == NULL) return 0;
    
    _NodePtr T = __x;

    if(T->__left_ == NULL && T->__right_ == NULL) {
        return 1;
    } else if(T->__left_ == NULL && T->__right_ != NULL) 
    {
        return __size(T->__right_) + 1;
    } else if(T->__left_ != NULL && T->__right_ == NULL) 
    {
        return __size(T->__left_) + 1;
    } else 
    {
        return __size(T->__left_) + __size(T->__right_) + 1;
    }

}
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
    _NodePtr q = NULL;
    _NodePtr p = __x;

    while(p != NULL) {
        q = p;
        p = p->__left_;
    }

    return q;
}

template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
    _NodePtr q = NULL;
    _NodePtr p = __x;

    while(p != NULL) {
        q = p;
        p = p->__right_;
    }

    return q;
}


template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
    _NodePtr q = NULL;
    _NodePtr p = __root;

    while(p != NULL) {
        if(p->__key_ == key) {
            //std::printf("i %d: The key already exists", key);
            return std::pair<_NodePtr, bool>(p, false);
        } 

        q = p;
        if(key < p->__key_) p = p->__left_;
        else p = p->__right_;   
    }

    _NodePtr newNode = getNodeBST<_NodePtr, _Tp>();
    newNode->__key_ = key;
    newNode->__left_ = NULL;
    newNode->__right_ = NULL;
    
    if(__root == NULL) __root = newNode;
    else if(key < q->__key_) q->__left_ = newNode;
    else q->__right_ = newNode; 

    return std::pair<_NodePtr, bool>(newNode, true);
}

template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
    _NodePtr p = searchBST(__root, key);
    _NodePtr q = searchParent(__root, key);

    if(p == NULL){
        //std::printf("d %d: The key does not exist", key);
        return nullptr;
    }

    if(q == NULL) {
        if(p->__left_ == NULL || p->__right_ == NULL) {
            __root = (p->__left_ != NULL ? p->__left_ : p->__right_);
            return p;
        }
    }
    
    if(p->__left_ == NULL && p->__right_ == NULL && q == NULL) {
        //std::printf("d %d: The key does not exist", key);
        return nullptr;
    }

    

    if(p->__left_ == NULL && p->__right_ == NULL) {             //차수 = 0
        if(q->__left_ == p) q->__left_ = NULL;
        else q->__right_ = NULL;
        return p;
    } else if(p->__left_ == NULL || p->__right_ == NULL) {      //차수 = 1
        if(p->__left_ == NULL) {
            if(q->__left_ == p) q->__left_ = p->__right_;
            else q->__right_ = p->__right_;
        } else {
            if(q->__left_ == p) q->__left_ = p->__left_;
            else q->__right_ = p->__left_;
        }
        return p;
    } else {                                                    //차수 = 2
        _NodePtr r;
        bool isLeft;
        if(__height(p->__left_) > __height(p->__right_)) {
            r = __tree_max(p->__left_);
            isLeft = true;
        } else if(__height(p->__left_) < __height(p->__right_)) {
            r = __tree_min(p->__right_);
            isLeft = false;
        } else {
            if(__size(p->__left_) >= __size(p->__right_)) {
                r = __tree_max(p->__left_);
                isLeft = true;
            } else {
                r = __tree_min(p->__right_);
                isLeft = false;
            }
        }
        p->__key_ = r->__key_;
        if(isLeft) {
            return __eraseBST(p->__left_, r->__key_);
        } else {
            return __eraseBST(p->__right_, r->__key_);
        }
    }
}

template <class _NodePtr>
void __inorder(_NodePtr __x) {
    _NodePtr p = __x;
    if(p == NULL) return;
    std::printf("<");
    __inorder(p->__left_);
    std::printf(" %d ", p->__key_);
    __inorder(p->__right_);
    std::printf(">");

    return;
}

template <class _NodePtr, class _Tp>
_NodePtr searchBST(_NodePtr& __x, const _Tp& searchKey) 
{
    _NodePtr p = __x;
    if(p == NULL) return NULL;

    if(searchKey == p->__key_) return p;
    
    if(searchKey < p->__key_) 
    {
        return searchBST(p->__left_, searchKey);
    } 
    else 
    {
        return searchBST(p->__right_, searchKey);
    }
}

template <class _NodePtr, class _Tp>
_NodePtr searchParent(_NodePtr& __x, const _Tp& key) {
    _NodePtr p = __x;
    if(p == NULL) return NULL;

    if(p->__left_ != NULL && p->__left_->__key_ == key) return p;    
    if(p->__right_ != NULL && p->__right_->__key_ == key) return p;

    if(key < p->__key_) return searchParent(p->__left_, key);
    else return searchParent(p->__right_, key);
}

template <class _NodePtr>
void __clear(_NodePtr& __x) {
    _NodePtr p = __x;

    _NodePtr left_child;
    _NodePtr right_child;

    
    if(p != NULL) {
        left_child = p->__left_;
        right_child = p->__right_;
        delete p;
        __clear(left_child);
        __clear(right_child);
        __x = nullptr;
    }
}





template <class _Tp>
class BST {
	public: // Member types
		typedef _Tp						key_type;
		typedef std::size_t				size_type;
		typedef Node<key_type>*			pointer;
		typedef const Node<key_type>*	const_pointer;
	
	private: // Member variables
		pointer	__root_;
	
	public: // Constructor
		BST(): __root_(nullptr) {}
	
	/*
	* 아래는 반드시 구현해야하는 부분입니다.
	* 위의 추천드리는 설계 방식을 이용하여 구현하는 것을 추천합니다.
	* 추전드리는 설계 방식을 이용하지 않고 새로 구현하셔도 무방합니다.
	*/

	public: // Capacity
		size_type height() const {
			// use __height or write your own code here
			return __height(__root_);
		}

		size_type size() const {
			// use __size or write your own code here
			return __size(__root_);
		}
	
	public: // Lookup
		void inorder() const {
			// use __inorder or write your own code here
			__inorder(__root_);
			std::cout << std::endl;
		}
		
	public: // Modifier
		std::pair<const_pointer, bool> insert(const key_type& key) {
			// use __insertBST or write your own code here
			return __insertBST(__root_, key);
		}

		const_pointer erase(const key_type& key) {
			// use __eraseBST or write your own code here
			pointer __r = __eraseBST(__root_, key);

			// Client still needs to destruct/deallocate it
			// Or memory leak will occur
			delete __r;

			return __r; 
		}

		void clear() {
			// use __clear or write your own code here
			__clear(__root_);
		}
	
	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* 반드시 아래의 함수를 사용해야할 필요는 없습니다.
	*/
	friend std::ostream& operator<<(std::ostream& os, const BST& tree) {
		__inorder(tree.__root_);
		return os;
	}
};

     
int main(int argc, char **argv) {
	BST<int> tree;
	char command;
	int	key;

	while (std::cin >> command >> key) {
		switch ((int)command) {
			case (int)'i':
				if (tree.insert(key).second == false) {
					std::cerr << "i " << key << ": The key already exists" << std::endl;
					continue;
				}
				break;
			case (int)'d':
				if (tree.erase(key) == nullptr) {
					std::cerr << "d " << key << ": The key does not exist" << std::endl;
					continue;
				}
				break;
			default:
				std::cerr << "Invalid command: " << command << std::endl;
				return (1);
				break;
		}
		std::cout << tree << std::endl;
	}

	// 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 반드시 해제해야 합니다.
	tree.clear();

	return (0);
}