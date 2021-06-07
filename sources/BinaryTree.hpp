#ifndef BT_BINARYTREE_HPP
#define BT_BINARYTREE_HPP
//template<typename T>
#include <iostream>
#include <stack>
#include <set>
#include <stdexcept>
#include <vector>
#include <map>

#include <stdlib.h>

using namespace std;


namespace ariel {


    template<class T>
    class BinaryTree {

        struct BTnode {

            T data;
            BTnode *right;
            BTnode *left;

            BTnode(const T &data) : data(data), right(nullptr), left(nullptr) {}

            //deep copy recursion
            BTnode(BTnode const &other): data(other.data), right(nullptr), left(nullptr) {
                if(other.right){
                    right=new BTnode(*other.right);
                }
                if(other.left){
                    left=new BTnode(*other.left);

                }
            }
            BTnode(BTnode&& other) noexcept{
                right=other.right;
                left=other.left;
                data=other.data;

                other.left=other.right= nullptr;

            }

            BTnode& operator=(BTnode other){
                if(this==other){
                    return *this;
                }
                if(right){
                    delete(right);
                }
                if(left){
                    delete(left);
                }
                left= new BTnode(*other.left);
                right= new BTnode(*other.right);
                return  *this;
            }

            BTnode& operator=(BTnode&& other) noexcept{
                if(right){
                    delete(right);
                }
                if(left){
                    delete(left);
                }
                data=other.data;
                right=other.right;
                left=other.left;
                other.left=other.right= nullptr;
                return *this;
            }
            bool operator==(const BTnode& other){
                return (data==other.data);
            }
            ~BTnode(){
                delete(right);
                delete(left);
            }

        };

    protected:


        class iterator_preorder {
        private:
            stack<BTnode *> myStk;
            BTnode *myPtr;

        public:
            //**********************PRE ORDER ITERATOR*****************

            iterator_preorder(BTnode *ptr = nullptr) : myPtr(ptr) {
                if (myPtr != nullptr) {
                    if (myPtr->right != nullptr) {
                        myStk.push(myPtr->right);
                    }
                    if (myPtr->left != nullptr) {
                        myStk.push(myPtr->left);
                    }
                }
            }

            T &operator*() const {
                return myPtr->data;
            }

            T *operator->() const {
                return &(myPtr->data);
            }

            //++it
            iterator_preorder &operator++() {
                if (myStk.empty()) {
                    myPtr = nullptr;
                    return *this;
                }
                myPtr = myStk.top();
                myStk.pop();
                if (myPtr->right != nullptr) {
                    myStk.push(myPtr->right);
                }
                if (myPtr->left != nullptr) {
                    myStk.push(myPtr->left);
                }
                return *this;
            }


            // it++ return copy and increment the original
            iterator_preorder operator++(int flag) {
                iterator_preorder temp = *this;
                if (myStk.empty()) {
                    myPtr = nullptr;
                    return temp;
                }
                myPtr = myStk.top();
                myStk.pop();
                if (myPtr->right != nullptr) {
                    myStk.push(myPtr->right);
                }
                if (myPtr->left != nullptr) {
                    myStk.push(myPtr->left);
                }
                return temp;
            }

            bool operator==(const iterator_preorder &other) const {
                return myPtr == other.myPtr;
            }

            bool operator!=(const iterator_preorder &other) const {

                return myPtr != other.myPtr;
            }

            iterator_preorder &operator=(BTnode *itOther) {
                return *this;

            }
        };
        //**********************POST ORDER ITERATOR*****************

        class iterator_postorder {
        private:
            stack<BTnode *> myStk;
            BTnode *myPtr;
            set<BTnode *> visited;
        public:
            iterator_postorder(BTnode *ptr = nullptr) : myPtr(ptr) {
                if (myPtr != nullptr) {
                    while (myPtr->left != nullptr || myPtr->right != nullptr) {
                        if (myPtr->left != nullptr) {
                            myStk.push(myPtr);
                            visited.insert(myPtr);
                            if (myPtr->right != nullptr) {
                                myStk.push(myPtr->right);
                                visited.insert(myPtr->right);

                            }
                            myPtr = myPtr->left;

                        } else {// only right
                            myStk.push(myPtr);
                            visited.insert(myPtr);
                            myPtr = myPtr->right;
                        }
                    }
                    visited.insert(nullptr);
                    visited.insert(myPtr);
                }
            }

            T &operator*() const {
                return myPtr->data;
            }

            T *operator->() const {
                return &(myPtr->data);
            }

            //it++
            iterator_postorder operator++(int flag) {
                iterator_postorder temp = *this;
                if (myStk.empty()) {
                    myPtr = nullptr;
                    return temp;
                }
                myPtr = myStk.top();
                myStk.pop();
                bool hasChildren = false;
//                if ((myPtr->left != nullptr && visited.count(myPtr->left) == 0) ||
//                    (myPtr->right != nullptr && visited.count(myPtr->right) == 0)) {//while
                    while((myPtr->left != nullptr && visited.count(myPtr->left) == 0) ||
                            (myPtr->right != nullptr && visited.count(myPtr->right) == 0)){

                        hasChildren=true;
                        myStk.push(myPtr);
                            if (myPtr->left != nullptr&& visited.count(myPtr->left) == 0) {
                        if (myPtr->right != nullptr && visited.count(myPtr->right) == 0) {
                            myStk.push(myPtr->right);
                            visited.insert(myPtr->right);
                        }
                        visited.insert(myPtr->left);
                        myPtr = myPtr->left;

                    } else {//only right
                        visited.insert(myPtr->right);
                        myPtr = myPtr->right;
                    }
                }

                return temp;
            }

            iterator_postorder &operator++() {
                if (myStk.empty()) {
                    myPtr = nullptr;
                    return *this;
                }
                myPtr = myStk.top();
                myStk.pop();
                while(visited.count(myPtr->left)==0 || visited.count(myPtr->right)==0){
                    if(visited.count(myPtr->left)==0 ){
                        if(visited.count(myPtr->right)==0){//left and right
                            myStk.push(myPtr);
                            myStk.push(myPtr->right);
                            visited.insert(myPtr);
                            visited.insert(myPtr->right);
                            myPtr=myPtr->left;

                        }
                        else{//only left
                            myStk.push(myPtr);
                            visited.insert(myPtr);
                            myPtr=myPtr->left;
                        }
                    }
                    else{//only right
                        myStk.push(myPtr);
                        visited.insert(myPtr);
                        myPtr=myPtr->right;
                    }

                }
                visited.insert(myPtr);

                return *this;
            }

            bool operator==(const iterator_postorder &other) const {
                return myPtr == other.myPtr;
            }

            bool operator!=(const iterator_postorder &other) const {
                return myPtr != other.myPtr;
            }

            iterator_postorder &operator=(BTnode *itOther) {
                return *this;
            }
        };


        //**********************IN ORDER ITERATOR*****************
        class iterator_inorder {
        private:
            stack<BTnode *> myStk;
            BTnode *myPtr;
        public:
            iterator_inorder(BTnode *ptr = nullptr) : myPtr(ptr) {
                if (myPtr != nullptr) {

                    while (myPtr->left != nullptr) {
                        //check if the value inside change
                        myStk.push(myPtr);
                        myPtr = myPtr->left;
                    }
                }
            }

            T &operator*() const {
                return myPtr->data;
            }

            T *operator->() const {
                return &(myPtr->data);
            }
            //i++
            iterator_inorder operator++(int flag) {
                iterator_inorder temp = *this;
                if (myStk.empty() && myPtr->right == nullptr) {
                    myPtr = nullptr;
                    return temp;
                }
                if (myPtr->right != nullptr) {
                    myPtr = myPtr->right;
                    while (myPtr->left != nullptr) {
                        myStk.push(myPtr);
                        myPtr = myPtr->left;
                    }
                } else {
                    myPtr = myStk.top();
                    myStk.pop();
                }
                return temp;
             }
            //++i
            iterator_inorder &operator++() {
                if (myStk.empty() && myPtr->right == nullptr) {
                    myPtr = nullptr;
                    return *this;
                }
                if (myPtr->right != nullptr) {
                    myPtr = myPtr->right;
                    while (myPtr->left != nullptr) {
                        myStk.push(myPtr);
                        myPtr = myPtr->left;
                    }
                } else {
                    myPtr = myStk.top();
                    myStk.pop();
                }
                return *this;
            }

            bool operator==(const iterator_inorder &other) const {
                return myPtr == other.myPtr;
            }

            bool operator!=(const iterator_inorder &other) const {
                return myPtr != other.myPtr;
            }

            iterator_inorder &operator=(BTnode *itOther) {
                return *this;
            }
        };

    private:
        BTnode *root=nullptr;
        string tree="";
        int size=0;

    public:


        BinaryTree() : root(nullptr), size(0) {

        }
        BinaryTree(const BinaryTree &other){

            root=new BTnode(*other.root);
        }
        BinaryTree(BinaryTree&& other)noexcept{
            root=other.root;
            other.root= nullptr;
        }
        BinaryTree& operator=(BinaryTree&& other)noexcept{
            if(root){
                delete(root);
            }
            root=other.root;
            other.root= nullptr;
            return *this;
        }
        //shallow copy
        BinaryTree& operator=(BinaryTree other){
            if(&other == this){
                return *this;
            }
            delete(root);
            this->root= new BTnode(*other.root);
            return *this;
        }

        bool operator==(BinaryTree other){
           return     (root==other.root);
        }

        ~BinaryTree(){
            delete(root);
        }


        BinaryTree<T> &add_root(T data) {
            if (root == nullptr) {
                //BTnode* temp=new BTnode(data);
                root = new BTnode(data);
                size++;

            } else {
                root->data = data;
            }


            return *this;
        }

        BinaryTree<T> &add_left(T oldData, T newData) {
            if(size==0){
                throw runtime_error("wrong input");

            }
            bool flag = false;
            stack<BTnode *> tempStack;
            tempStack.push(root);
            while (!flag && !tempStack.empty()) {
                BTnode *temp = tempStack.top();
                tempStack.pop();
                if (temp->data == oldData) {
                    if (temp->left == nullptr) {
                        temp->left = new BTnode(newData);
                        size++;
                    } else {
                        temp->left->data = newData;
                    }
                    flag = true;

                } else {
                    if (temp->left != nullptr) {
                        tempStack.push(temp->left);
                    }
                    if (temp->right != nullptr) {
                        tempStack.push(temp->right);
                    }
                }

            }
            if(!flag){
                throw runtime_error("wrong input");
            }
            return *this;
        }

        BinaryTree<T> &add_right(T oldData, T newData) {
            if(size==0){
                throw runtime_error("wrong input");

            }

            bool flag = false;
            stack<BTnode *> tempStack;
            tempStack.push(root);
            while (!flag && !tempStack.empty()) {
                BTnode *temp = tempStack.top();
                tempStack.pop();
                if (temp->data == oldData) {
                    if (temp->right == nullptr) {
                        temp->right = new BTnode(newData);
                        size++;
                    } else {
                        temp->right->data = newData;
                    }
                    flag = true;

                } else {
                    if (temp->left != nullptr) {
                        tempStack.push(temp->left);
                    }
                    if (temp->right != nullptr) {
                        tempStack.push(temp->right);
                    }
                }
            }
            if(!flag){
                throw runtime_error("wrong input");
            }
            return *this;

        }

        iterator_preorder begin_preorder() {
            return iterator_preorder(root);
        }

        iterator_preorder end_preorder() {
            return iterator_preorder(nullptr);
        }

        iterator_inorder begin_inorder() {
            return iterator_inorder(root);
        }

        iterator_inorder end_inorder() {
            return iterator_inorder(nullptr);
        }

        iterator_postorder begin_postorder() {
            return iterator_postorder(root);
        }

        iterator_postorder end_postorder() {
            return iterator_postorder(nullptr);
        }

        iterator_inorder begin(){
            return iterator_inorder(root);
        }
        iterator_inorder end(){
            return iterator_inorder(nullptr);
        }

         void printBT( const std::string& prefix, const BTnode* node, bool isLeft)
        {
            if( node != nullptr )
            {
                 cout<< prefix;
                 if(isLeft){
                     cout<<"└──";
                 }
                 else{
                     cout<<"├──";
                 }


                // print the value of the node
                cout<<node->data<<endl;

                // enter the next tree level - left and right branch
                printBT( prefix + (isLeft ? "│   " : "    "), node->right, true);
                printBT( prefix + (isLeft ? "│   " : "    "), node->left, false);
            }
        }

          void showBT(const BTnode* node)
        {

             printBT("", node, false) ;
        }

// pass the root node of your binary tree

        friend ostream &operator<<(ostream &os, BinaryTree& bt) {

            bt.showBT(bt.root);

//            os.write(bt.tree);

            return os;
        }
    };
}
#endif //BT_BINARYTREE_HPP