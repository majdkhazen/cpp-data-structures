#ifndef AVLTREE_H_
#define AVLTREE_H_
#include <stdio.h>
#include <iostream>
#include <math.h>



template <class T>
class Node
{
public:
    T value;
    int height;
    Node* left;
    Node* right;
    Node* parent;
    Node(T value,Node*parent) : value(value), height(0) ,left(NULL), right(NULL), parent(parent){}
    ~Node()=default;
};

template <class T,class compare>
class AVLTree
{
public:
    Node<T>* head;
    compare comp;
    int size;
    //constructor
    AVLTree(compare comp): head(NULL),comp(comp),size(0){}
    AVLTree(int height,int toDelete,compare comp);
    //add and its help function
    void add(T addValue);
    // remove function
    void remove (T removeValue);
    //merge trees of pointers
    AVLTree<T,compare>* mergedAVL(AVLTree<T,compare>* dis,AVLTree<T,compare>* src);
    //isExist
    bool isExist(T value);
    //destructor
    ~AVLTree();
    void clearTree();
    /*
    //for debugging
    void printDetails();
    void printDetailsAux(Node<T>* head);
    */
    //for debug
    void printPlayers(Node<T>* head);
    //for debugging
    void printGroup(Node<T>* head, int &size);
    Node<T>* binarySearch(Node<T>* head, T addValue);
    void fillArray(T array[],Node<T>* head,int& index,int size);
    void inOrder(Node<T>* head,int& size,int& index,T arr[]);
    void inOrder_aux(Node<T>* head,Node<T>* min,Node<T>* max,int& size,int& index,T arr[]);
    T findPrev(T value);
    T findNext(T value);
    T rightson(Node<T>* head);
    Node<T>* ancestor(Node<T>* head,Node<T>* tmp1,Node<T>* tmp2);
    Node<T>* getLeastValue();


private:
    //add help function

    void AbalanceTree(Node<T> *current);
    int calculateBF(Node<T>* node);
    int getHeight(Node<T>* node);
    void balanceTree(Node<T> *current, int curBF);
    void rotateRight(Node<T>* damagedHead);
    int max(int a,int b)
    {
        return (a>b) ? a:b;
    }
    void rotateLeft(Node<T>* damagedHead);
    void updateHead();
    //remove function and its help functions
    int getSonsNum(Node<T> *head);
    void removeIfNoSons(Node<T>* removeNode);
    void RbalanceTree(Node<T> *current);
    void removeIfOneSon(Node<T> *removeNode);
    //merge trees help function
    void mergeSort(T mergedArray[],T array1[],int size1,T array2[],int size2);
    void fillSemiFull(Node<T>* semiFullTree ,T array[],int& index, int size);
    // consrtuctor that builds a semifull tree filled with NULL
    Node<T>* fullTree(int height,Node<T>* tree, Node<T>* parent=NULL);
    void semiFull(Node<T>* head,int& toDelete);
    //destructor help function
    void clearTreeAux(Node<T>* head);

};

//constructor for semifull tree
template <class T,class compare>
AVLTree<T,compare>::AVLTree(int height,int toDelete,compare comp):comp(comp)
{
    this->head=fullTree(height,NULL);
    //std::cout<<"to delete is "<<toDelete<<std::endl;
    int TD = toDelete;
    semiFull(this->head,TD);
    this->size=pow(2,height+1)-1-toDelete;
}

//from full to semifull
template <class T,class compare>
void  AVLTree<T,compare>::semiFull(Node<T>* head,int& toDelete)
{
    if(toDelete<=0||head==NULL)
    {
        return;
    }
    semiFull(head->right,toDelete);
    if (toDelete <=0)
    {
        head->height=max(getHeight(head->left),getHeight(head->right))+1;
        return;
    }
    if(head->right == NULL && head->left == NULL)
    {
        if(head->parent!=NULL)
        {
            if(head->parent->right==head)
            {
                head->parent->right=NULL;
            }
            else
            {
                head->parent->left=NULL;
            }
        }
        toDelete--;
        delete head;
        return;
    }
    semiFull(head->left,toDelete);
    head->height=max(getHeight(head->left),getHeight(head->right))+1;
}

// build a full tree of NULL
template <class T,class compare>
Node<T>* AVLTree<T,compare>::fullTree(int height,Node<T>* tree, Node<T>* parent)
{
    if(height==-1)
    {
        return NULL;
    }
    tree = new Node<T>(NULL,parent);
    tree->height=height;
    tree->left=fullTree(height-1,tree->left,tree);
    tree->right=fullTree(height-1,tree->right,tree);
    return tree;
}

//add function
template <class T,class compare>
void AVLTree<T,compare>::add(T addValue)
{
    if(this->head==NULL)
    {
        Node<T>* nodeValue = new Node<T>(addValue,NULL);
        this->head=nodeValue;
        size++;
        return;
    }
    Node<T>* parent= binarySearch(this->head,addValue);
    if(parent->value == addValue)
    {
        //exist
        return;
    }
    Node<T>* nodeValue = new Node<T>(addValue,parent);
    if(this->comp(addValue,parent->value)==1)
    {
        parent->left=nodeValue;
    }
    else
    {
        parent->right=nodeValue;
    }
    this->size++;
    AbalanceTree(nodeValue);
}

template <class T,class compare>
Node<T>* AVLTree<T,compare>::binarySearch(Node<T>* head, T addValue)
{
    if(head==NULL || comp(head->value,addValue)==0)
    {
        return head;
    }
    if(comp(head->value ,addValue)==1)
    {
        if (head->right==NULL)
            return head;
        return binarySearch(head->right,addValue);
    }
    if(head->left==NULL)
    {
        return head;
    }
    return binarySearch(head->left,addValue);
}

template <class T,class compare>
void  AVLTree<T,compare>::AbalanceTree(Node<T> *current)
{
    if(current==NULL)
    {
        return;
    }
    while(current->parent != NULL)
    {
        Node<T>* curParent=current->parent;
        if(curParent->height >= current->height+1)
        {
            return;
        }
        curParent->height=current->height + 1;
        int curParentBF=calculateBF(curParent);
        balanceTree(curParent,curParentBF);
        if(curParentBF==2||curParentBF==-2)
        {
            updateHead();
            return;
        }
        current=curParent;
    }
}

template<class T,class compare>
int AVLTree<T,compare>::calculateBF(Node<T>* node) {
    if(node==NULL){
        return 0;
    }
    return getHeight(node->left)- getHeight(node->right);
}

template<class T,class compare>
int AVLTree<T,compare>::getHeight(Node<T>* node)
{
    if(node==NULL)
    {
        return -1;
    }
    return node->height;
}


template <class T,class compare>
void  AVLTree<T,compare>::balanceTree(Node<T> *current, int curBF)
{
    if(curBF==2)
    {
        int BFleft=calculateBF(current->left);
        if(BFleft>=0)
        {
            //LL
            rotateRight(current);
        }
        if(BFleft==-1)
        {
            //LR
            rotateLeft(current->left);
            rotateRight(current);
        }
    }
    else if(curBF==-2)
    {
        int BFright=calculateBF(current->right);
        if(BFright<=0)
        {
            //RR
            rotateLeft(current);
        }
        if(BFright==1)
        {
            //RL
            rotateRight(current->right);
            rotateLeft(current);
        }
    }
}


template <class T,class compare>
void AVLTree<T,compare>::rotateRight(Node<T>* damagedHead)
{
    if(!damagedHead)
    {
        return;
    }
    Node<T>* headLeft = damagedHead->left;
    damagedHead->left = headLeft->right;
    headLeft->right = damagedHead;
    headLeft->parent = damagedHead->parent;
    if(damagedHead->left!=NULL){
        damagedHead->left->parent = damagedHead;
    }
    if(damagedHead->parent!=NULL)
    {
        if(damagedHead->parent->left==damagedHead)
        {
            damagedHead->parent->left=headLeft;
        }
        else
        {
            damagedHead->parent->right=headLeft;
        }

    }
    damagedHead->parent = headLeft;
    damagedHead->height = max(getHeight(damagedHead->left),getHeight(damagedHead->right))+1;
    headLeft->height = max(getHeight(headLeft->left),getHeight(headLeft->right))+1;
}


template <class T,class compare>
void  AVLTree<T,compare>::rotateLeft(Node<T>* damagedHead)
{
    if(!damagedHead)
    {
        return;
    }
    Node<T>* headRight = damagedHead->right;
    damagedHead->right = headRight->left;
    headRight->left = damagedHead;
    headRight->parent = damagedHead->parent;
    if(damagedHead->right!=NULL){
        damagedHead->right->parent = damagedHead;
    }
    if(damagedHead->parent!=NULL)
    {
        if(damagedHead->parent->right==damagedHead)
        {
            damagedHead->parent->right=headRight;
        }
        else
        {
            damagedHead->parent->left=headRight;
        }

    }
    damagedHead->parent = headRight;
    damagedHead->height = max(getHeight(damagedHead->left),getHeight(damagedHead->right))+1;
    headRight->height = max(getHeight(headRight->left),getHeight(headRight->right))+1;
}


template <class T,class compare>
void AVLTree<T,compare>::updateHead()
{
    if(this->head==NULL)
    {
        return;
    }
    while(this->head->parent!=NULL)
    {
        this->head=this->head->parent;
    }
}

//remove
template <class T,class compare>
void AVLTree<T,compare>::remove (T removeValue)
{
    // if not exist then do nothing
    if(this->head==NULL)
    {
        return;
    }
    Node<T>* removeNode = binarySearch(this->head, removeValue);
    if(removeNode== nullptr) return;
    /*if(comp(removeNode->value,removeValue)!=0)
    {
        //not exist
        return;
    }*/
    int sons = getSonsNum(removeNode);
    if (sons==0)
    {
        removeIfNoSons(removeNode);
    }
    if(sons==1)
    {
        removeIfOneSon(removeNode);
    }
    if(sons==2)
    {
        Node<T>* toSwap=removeNode->right;
        while (toSwap->left)
        {
            toSwap=toSwap->left;
        }
        T temp=removeNode->value;
        removeNode->value=toSwap->value;
        toSwap->value=temp;
        if(getSonsNum(toSwap)==0)
        {
            removeIfNoSons(toSwap);
        }
        else
        {
            removeIfOneSon(toSwap);
        }
    }
    this->size--;
}

template <class T,class compare>
int AVLTree<T,compare>::getSonsNum(Node<T> *head)
{
    if(!head){
        return 0;
    }
    if(head->left==NULL && head->right==NULL){
        return 0;
    }
    if(head->left && head->right){
        return 2;
    }
    return 1;
}

template <class T,class compare>
void AVLTree<T,compare>::removeIfNoSons(Node<T>* removeNode)
{
    if(removeNode==NULL)
    {
        return;
    }
    if(removeNode == this->head)
    {
        this->head = NULL;
        delete removeNode;
        return;
    }
    if(removeNode->parent!=NULL){
        if(removeNode->parent->left==removeNode){
            removeNode->parent->left=NULL;
        }
        if(removeNode->parent->right==removeNode)
        {
            removeNode->parent->right=NULL;
        }
        Node<T>* parent= removeNode->parent;
        delete removeNode;
        RbalanceTree(parent);
    }
}


template <class T,class compare>
void  AVLTree<T,compare>::RbalanceTree(Node<T> *current)
{
    if(current==NULL)
    {
        return;
    }
    while(current!=NULL)
    {
        current->height=max(getHeight(current->left),getHeight(current->right))+1;
        int curBF=calculateBF(current);
        balanceTree(current,curBF);
        current=current->parent;
    }
    updateHead();
}


template <class T,class compare>
void AVLTree<T,compare>::removeIfOneSon(Node<T> *removeNode)
{
    if(removeNode==NULL)
    {
        return;
    }
    Node<T>* parent=removeNode->parent;
    Node<T>* son=removeNode->right;
    if(removeNode->right==NULL){
        son= removeNode->left;
    }
    if(removeNode==this->head){
        this->head=son;
        son->parent=NULL;
        delete removeNode;
        return;
    }
    if((parent->right!=NULL)&&(parent->right==removeNode)){
        parent->right=son;
    }
    if((parent->left!=NULL)&&(parent->left==removeNode)){
        parent->left=son;
    }
    son->parent=removeNode->parent;
    delete removeNode;
    RbalanceTree(parent);
}

template <class T,class compare>
AVLTree<T,compare>* AVLTree<T,compare>::mergedAVL(AVLTree<T,compare>* dis,AVLTree<T,compare>* src)
{
    if(dis==NULL||src==NULL)
    {
        return NULL;
    }
    if(src->size<=0)
    {
        T* disArray = new T [dis->size];
        int index=0;
        fillArray(disArray,dis->head,index,dis->size);
        //std::cout<<"at merge to delete = "<<pow(2,dis->head->height+1)-1-dis->size<<std::endl;
        int height = ceil(log2(dis->size+1))-1;
        AVLTree<T,compare>* semiFullTree=new AVLTree<T,compare>(height,pow(2,height+1)-1-dis->size,this->comp);
        index = 0;
        fillSemiFull(semiFullTree->head,disArray,index,dis->size);
        delete[] disArray;
        return semiFullTree;
    }
    if(dis->size<=0)
    {
        T* srcArray = new T [src->size];
        int index=0;
        fillArray(srcArray,src->head,index,src->size);
        //std::cout<<"src size is "<<src->size<<std::endl;
        //std::cout<<"src height is "<<src->head->height<<std::endl;
        //std::cout<<"at merge to delete = "<<pow(2,src->head->height+1)-1-src->size<<std::endl;
        int height = ceil(log2(src->size+1))-1;
        AVLTree<T,compare>* semiFullTree=new AVLTree<T,compare>(height,pow(2,height+1)-1-src->size,this->comp);
        index = 0;
        fillSemiFull(semiFullTree->head,srcArray,index,src->size);
        delete[] srcArray;
        return semiFullTree;
    }
    // for every T we should write an defualt constructor
    T* disArray = new T [dis->size];
    T* srcArray = new T [src->size];
    int newSize = dis->size+src->size;
    int index=0;
    fillArray(disArray,dis->head,index,dis->size);
    index=0;
    fillArray(srcArray,src->head,index, src->size);
    T* mergedArray = new T [newSize];
    mergeSort(mergedArray,disArray,dis->size,srcArray,src->size);
    int height = ceil(log2(newSize+1))-1;
    AVLTree<T,compare>* semiFullTree=new AVLTree<T,compare>(height,pow(2,height+1)-1-newSize,this->comp);
    index=0;
    fillSemiFull(semiFullTree->head,mergedArray,index,newSize);
    delete[] disArray;
    delete[] srcArray;
    delete[] mergedArray;
    return semiFullTree;
}

template <class T,class compare>
void AVLTree<T,compare>::fillArray(T array[],Node<T>* head,int& index, int size)
{
    if(index>=size || head == NULL)
    {
        return;
    }
    fillArray(array,head->left,index,size);
    if(index>=size)
    {
        return;
    }
    array[index++]=head->value;
    fillArray(array,head->right,index,size);
}

template <class T,class compare>
void AVLTree<T,compare>::mergeSort(T mergedArray[],T array1[],int size1,T array2[],int size2)
{
    int a1=0,a2=0,merged=0;
    while(a1 < size1 && a2 < size2)
    {
        if(comp(array1[a1] , array2[a2])==1)
        {
            mergedArray[merged++]=array1[a1++];
        }
        else
        {
            mergedArray[merged++]=array2[a2++];
        }
    }
    while(a1 < size1)
    {
        mergedArray[merged++]=array1[a1++];
    }
    while(a2 < size2)
    {
        mergedArray[merged++]=array2[a2++];
    }
}

template <class T,class compare>
void AVLTree<T,compare>::fillSemiFull(Node<T>* semiFullTree ,T array[],int& index,int size)
{
    if(index>=size || semiFullTree==NULL)
    {
        return;
    }
    fillSemiFull(semiFullTree->left,array,index,size);
    if(index>=size)
    {
        return;
    }
    semiFullTree->value=array[index++];
    fillSemiFull(semiFullTree->right,array,index,size);
}

template <class T,class compare>
AVLTree<T,compare>::~AVLTree()
{
    ///if(this)return;
    this->size=0;
    clearTree();
    this->head = nullptr;
}

template <class T,class compare>
void AVLTree<T,compare>::clearTreeAux(Node<T>* head)
{
    if(head==NULL)
        return;
    clearTreeAux(head->left);
    clearTreeAux(head->right);
    delete head;
}

template <class T,class compare>
void AVLTree<T,compare>::clearTree()
{
    Node<T>* temp=this->head;
    this->head=NULL;
    clearTreeAux(temp);
    this->size=0;
}

template <class T,class compare>
bool AVLTree<T,compare>:: isExist(T value)
{
    Node<T>* node=binarySearch(this->head,value);
    if(node==NULL)
    {
        return false;
    }
    if(comp(node->value,value)==0)
    {
        return true;
    }
    return false;
}

template<class T,class compare>
void AVLTree<T,compare>::inOrder(Node<T>* head,int& size,int& index,T arr[])
{
    if(index>=size||head==NULL)
    {
        return;
    }
    inOrder(head->left,size,index,arr);
    if(index>=size)
    {
        return;
    }
    arr[index++]=head->value;
    inOrder(head->right,size,index,arr);
}

template<class T,class compare>
void AVLTree<T,compare>::inOrder_aux(Node<T> *head, Node<T> *min, Node<T> *max, int &size, int &index, T *arr)
{
    if(index>=size||head==NULL || max==NULL || min==NULL)
    {
        return;
    }
    if((comp(head->value,max->value)>=0) &&(comp(head->value,min->value))<=0)
    {
        inOrder_aux(head->left,min,max,size,index,arr);
        arr[index++]=head->value;
        inOrder_aux(head->right,min,max,size,index,arr);
    }
    else if(comp(head->value,min->value)==1)
    {
        inOrder_aux(head->right,min,max,size,index,arr);
    }
    else if(comp(head->value,max->value)==-1)
    {
        inOrder_aux(head->left,min,max,size,index,arr);
    }
}



/*
//for debug purposes
template<class T,class compare>
void AVLTree<T,compare>::printDetails()
{
    printDetailsAux(this->head);
    std::cout<<"**************************\n";
}

template<class T,class compare>
void AVLTree<T,compare>::printDetailsAux(Node<T>* head)
{
    if(head == NULL)
    {
        return;
    }
    printDetailsAux(head->left);
    std::cout<<"VALUE: "<<head->value<<"  ";
    std::cout<<"BF: "<<calculateBF(head)<<"  ";
    std::cout<<"HEIGHT: "<<head->height<<"\n";
    printDetailsAux(head->right);
}*/
/*template<class T, class compare>
Node<T>* helperParent(Node<T>* current){
    compare c;
    Node<T>* first_paren;
    if(current->parent == NULL)
    {
        return current->parent;
    }
    if(c(current->parent->value, current->value)==-1){
        return current->parent;
    }
    while(c(current->value,current->parent->value)==1 && current->parent != NULL)
        current = current->parent;
    return current->parent;
}

template<class T, class compare>
Node<T>* helperSon(Node<T>* current){
    if(current->left== nullptr)
        return nullptr;
    current=current->left;
    while (current->right!= nullptr)
        current=current->right;
    return current;
}

template<class T, class compare>
T AVLTree<T, compare>::findPrev(T value) {
    Node<T>* new_node = binarySearch(this->head,value);
    Node<T>* t1= helperParent<T,compare>(new_node);
    Node<T>* t2= helperSon<T,compare>(new_node);
    if(t1== nullptr){
        if(t2== nullptr)
            return nullptr;
        return t2->value;
    }
    if (t2== nullptr)
        return t1->value;

    if(comp(t1->value,t2->value)==1){
        return t2->value;
    }
    return t1->value;
}*/

template<class T, class compare>
T AVLTree<T,compare>::findNext(T value)
{
    Node<T>* new_node = binarySearch(this->head,value);
    if(new_node->right!=NULL)
    {
        new_node = new_node->right;
        while(new_node->left != NULL)
        {
            new_node = new_node->left;
        }
        return new_node->value;
    }
    else if(new_node->parent != NULL)
    {
        if(new_node->parent->left == new_node)
        {
            return new_node->parent->value;
        }
        if(new_node->parent->right == new_node)
        {
            while(new_node->parent != NULL && new_node->parent->right == new_node)
            {
                new_node = new_node->parent;
            }
            if(new_node == head)
            {
                return NULL;
            }
            else
            {
                return new_node->parent->value;
            }
        }
    }
    return NULL;
}


template<class T, class compare>
T AVLTree<T, compare>::findPrev(T value) {
    Node<T>* new_node = binarySearch(this->head,value);
    if(new_node->left!=NULL)
    {
        new_node = new_node->left;
        while(new_node->right != NULL)
        {
            new_node = new_node->right;
        }
        return new_node->value;
    }
    else if(new_node->parent != NULL)
    {
        if(new_node->parent->right == new_node)
        {
            return new_node->parent->value;
        }
        if(new_node->parent->left == new_node)
        {
            while(new_node->parent != NULL && new_node->parent->left == new_node)
            {
                new_node = new_node->parent;
            }
            if(new_node == head)
            {
                return NULL;
            }
            else
            {
                return new_node->parent->value;
            }
        }
    }
    return NULL;
}



//for debug only
template<class T,class compare>
void AVLTree<T,compare>::printPlayers(Node<T>* head)
{
    if(head == NULL)
    {
        return;
    }
    printPlayers(head->left);
    std::cout<<"VALUE: "<<head->value->playerID<<"  ";
    std::cout<<"BF: "<<calculateBF(head)<<"  ";
    std::cout<<"HEIGHT: "<<head->height<<"\n";
    printPlayers(head->right);
}


template<class T,class compare>
void AVLTree<T,compare>::printGroup(Node<T>* head, int &size)
{
    if(head == NULL)
    {
        return;
    }
    printGroup(head->left,size);
    /*
    std::cout<<"VALUE: "<<head->value->groupID<<"  ";
    std::cout<<"BF: "<<calculateBF(head)<<"  ";
    std::cout<<"HEIGHT: "<<head->height<<"\n";
    */
    size ++;
    if (head->value == NULL)
    {
        //std::cout<<"there is NULL\n";
    }
    printGroup(head->right,size);
}

template<class T,class compare>
T AVLTree<T,compare>::rightson(Node<T> *head)
{
    if(!head) return nullptr;
    if(!head->right) return head->value;
    while (head->right!= nullptr)
    {
        head=head->right;
    }
    return head->value;
}

template<class T,class compare>
Node<T>* AVLTree<T,compare>::ancestor(Node<T> *head, Node<T> *min, Node<T> *max)
{
    Node<T>* curr=head;
    while (curr)
    {
        if((comp(curr->value,max->value)==-1) && (comp(curr->value,min->value)==-1))
        {
            curr=curr->left;
        }
        else if((comp(curr->value,min->value)==1) && (comp(curr->value,max->value)==1))
        {
            curr=curr->right;
        }
        else return curr;
    }
    return curr;
}

template<class T, class compare>
Node<T>* AVLTree<T, compare>::getLeastValue() {
    Node<T>* curr = this->head;
    while (curr->left!=NULL){
        curr = curr->left;
    }
    return curr;
}




#endif