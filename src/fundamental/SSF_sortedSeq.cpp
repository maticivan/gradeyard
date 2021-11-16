//    GradeYard learning management system
//
//    Copyright (C) 2021 Ivan Matic, https://gradeyard.com
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE as published by
//    the Free Software Foundation, either version 3 of the License, or
//    any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU AFFERO GENERAL PUBLIC LICENSE for more details.
//
//    You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
//    along with this program.  If not, see https://www.gnu.org/licenses/.

#ifndef _INCL_StatSeq_CPP
#define _INCL_StatSeq_CPP

namespace SSF{
  template<typename TTT> class StatSeq{
  private:
    class AVLNode{
    public:
      TTT value;
      long leftCount;
      long rightCount;
      long height;
      AVLNode* leftChild;
      AVLNode* rightChild;
      AVLNode(){
        leftChild=nullptr;
        rightChild=nullptr;
        leftCount=-17;
        rightCount=-17;
      }
      AVLNode(const TTT& k ) {
        value = k; leftChild = nullptr;
        rightChild = nullptr;
        leftCount=-17;
        rightCount=-17;
        height = 1;
      }
    }* _dRoot;
    long _size;
    long getHeight(AVLNode * );
    long hDiff(AVLNode * );
    long redoHeight(AVLNode * );
    AVLNode* rRotation(AVLNode* );
    AVLNode* lRotation(AVLNode* );
    AVLNode* balance(AVLNode* );
    AVLNode* insertInTree(AVLNode* , const TTT & , long & );
    AVLNode* getMinNode(AVLNode* );
    AVLNode* rewireMin(AVLNode* );
    AVLNode* removeFromTree(AVLNode* , const TTT& , long & );
    AVLNode* duplicateTree(AVLNode*);
    TTT avl_access(AVLNode* , const long & ) const;
  public:
    StatSeq();
    StatSeq(const StatSeq &);
    void operator=(const StatSeq &);
    StatSeq(StatSeq&&);
    void operator=(StatSeq &&);
    long size() const;
    long erase(const TTT & );
    long erase(const StatSeq & );
    long insert(const TTT & );
    long insert(const StatSeq &);
    long operator+=(const TTT &);
    long operator+=(const StatSeq &);
    long operator-=(const TTT &);
    long operator-=(const StatSeq &);
    long find(const TTT &) const;
    void clear();
    TTT operator[](const long & ) const;
    virtual ~StatSeq();
  };
  template<typename TTT> long StatSeq<TTT>::getHeight(AVLNode * root){
    if(root==nullptr){
      return 0;
    }
    return root->height;
  }
  template<typename TTT> long StatSeq<TTT>::hDiff(AVLNode * root){
    return getHeight(root->rightChild)-getHeight(root->leftChild);
  }
  template<typename TTT> long StatSeq<TTT>::redoHeight(AVLNode * root){
    long lHeight = getHeight(root->leftChild);
    long rHeight = getHeight(root->rightChild);
    root->height = rHeight;
    if(lHeight>rHeight){root->height = lHeight;}
    root->height += 1;
    return root->height;
  }
  template<typename TTT>
  typename StatSeq<TTT>::AVLNode* StatSeq<TTT>::rRotation(AVLNode * root){
    AVLNode * other = root->leftChild;
    root->leftChild = other->rightChild;
    other->rightChild = root;
    long a,c;
    a= other->leftCount;
    c= root->rightCount;
    root->leftCount -= (a+1);
    other->rightCount += (c+1);
    redoHeight(root);
    redoHeight(other);
    return other;
  }
  template<typename TTT>
  typename StatSeq<TTT>::AVLNode * StatSeq<TTT>::lRotation(AVLNode * root){
    AVLNode * other = root->rightChild;
    root->rightChild = other->leftChild;
    other->leftChild = root;
    long a,c;
    a= other->rightCount;
    c=root->leftCount;
    root->rightCount -= (a+1);
    other->leftCount += (c+1);
    redoHeight(root);
    redoHeight(other);
    return other;
  }
  template<typename TTT>
  typename StatSeq<TTT>::AVLNode * StatSeq<TTT>::
  balance(AVLNode * root){
    redoHeight(root);
    if( hDiff(root)==2 ){
      if( hDiff(root->rightChild) < 0 ){
        root->rightChild = rRotation(root->rightChild);
      }
      return lRotation(root);
    }
    if( hDiff(root)==-2 ){
      if( hDiff(root->leftChild) > 0 ){
        root->leftChild = lRotation(root->leftChild);
      }
      return rRotation(root);
    }
    return root;
  }
  template<typename TTT>
  typename StatSeq<TTT>::AVLNode * StatSeq<TTT>::
  insertInTree(AVLNode * root, const TTT & _value, long & insSucc){
    if( root==nullptr ){
      insSucc=1;
      AVLNode *fR=new AVLNode (_value);
      fR->leftCount=0;
      fR->rightCount=0;
      return fR;
    }
    if( _value < root->value ){
      root->leftChild = insertInTree(root->leftChild,_value,insSucc);
      if(insSucc==1){
        root->leftCount+=1;
      }
    }
    else{
      if(root->value < _value ){
        root->rightChild = insertInTree(root->rightChild,_value,insSucc);
        if(insSucc==1){
          root->rightCount+=1;
        }
      }
      else{
        insSucc=0;
        return root;
      }
    }
    if(insSucc==1){
      return balance(root);
    }
    return root;
  }
  template<typename TTT>
  typename StatSeq<TTT>::AVLNode * StatSeq<TTT>::getMinNode(AVLNode * root){
    if(root->leftChild==nullptr){
      return root;
    }
    return getMinNode(root->leftChild);
  }
  template<typename TTT>
  typename StatSeq<TTT>::AVLNode * StatSeq<TTT>::rewireMin(AVLNode * root){
    if( root->leftChild==nullptr ){
      return root->rightChild;
    }
    root->leftChild = rewireMin(root->leftChild);
    root->leftCount -= 1;
    return balance(root);
  }
  template<typename TTT>
  typename StatSeq<TTT>::AVLNode * StatSeq<TTT>::removeFromTree(AVLNode * root, const TTT& _value, long & remSucc){
    if( root==nullptr ){
      remSucc=0;
      return nullptr;
    }
    if( _value < root->value ){
      root->leftChild = removeFromTree(root->leftChild,_value, remSucc);
      if(remSucc==1){
        root->leftCount -=1;
      }
    }
    else{
      if(root->value < _value ){
        root->rightChild = removeFromTree(root->rightChild,_value,remSucc);
        if(remSucc==1){
          root->rightCount -=1;
        }
      }
      else{
        AVLNode * lTemp = root->leftChild;
        AVLNode * rTemp = root->rightChild;
        long oldleftCount,oldrightCount;
        oldleftCount=root->leftCount;
        oldrightCount=root->rightCount;
        delete root;
        remSucc=1;
        if( rTemp == nullptr ){
          return lTemp;
        }
        AVLNode * rightMin = getMinNode(rTemp);
        rightMin->rightChild = rewireMin(rTemp);
        rightMin->leftChild = lTemp;
        rightMin->leftCount=oldleftCount;
        rightMin->rightCount=oldrightCount-1;
        remSucc=1;
        return balance(rightMin);
      }
    }
    return balance(root);
  }
  template<typename TTT> typename StatSeq<TTT>::AVLNode * StatSeq<TTT>::duplicateTree(AVLNode *root){
    AVLNode *newN;
    newN=nullptr;
    if(root!=nullptr){
      newN=new AVLNode;
      newN->value=root->value;
      newN->leftCount=root->leftCount;
      newN->rightCount=root->rightCount;
      newN->height=root->height;
      newN->leftChild = duplicateTree(root->leftChild);
      newN->rightChild= duplicateTree(root->rightChild);
    }
    return newN;
  }
  template<typename TTT> TTT StatSeq<TTT>::avl_access(AVLNode *root, const long &i) const{
    if(root==nullptr){
      TTT tempWithRandValue;
      return tempWithRandValue;
    }
    if(i==root->leftCount){
      return root->value;
    }
    if(i<root->leftCount){
      return avl_access(root->leftChild,i);
    }
    return avl_access(root->rightChild, i-root->leftCount-1);
  }
  template<typename TTT> StatSeq<TTT>::StatSeq(){
    _dRoot=nullptr;
    _size=0;
  }
  template<typename TTT> StatSeq<TTT>::StatSeq(const StatSeq<TTT>& _copyFrom){
    _dRoot=nullptr;
    _dRoot= duplicateTree(_copyFrom._dRoot);
    _size= _copyFrom._size;
  }
  template<typename TTT> StatSeq<TTT>::StatSeq( StatSeq<TTT>&& _moveFrom){
    _size= _moveFrom._size;
    _moveFrom._size=0;
    _dRoot=_moveFrom._dRoot;
    _moveFrom._dRoot=nullptr;
  }
  template<typename TTT> void StatSeq<TTT>::operator=(const StatSeq<TTT>& _copyFrom){
    if(&_copyFrom!=this){
      clear();
      _dRoot= duplicateTree(_copyFrom._dRoot);
      _size= _copyFrom._size;
    }
  }
  template<typename TTT> void StatSeq<TTT>::operator=( StatSeq<TTT>&& _moveFrom){
    if(&_moveFrom!=this){
      clear();
      _size= _moveFrom._size;
      _dRoot= _moveFrom._dRoot;
      _moveFrom._size=0;
      _moveFrom._dRoot=nullptr;
    }
  }
  template<typename TTT> long StatSeq<TTT>::size() const{
    return _size;
  }
  template<typename TTT> StatSeq<TTT>::~StatSeq(){
    clear();
  }
  template<typename TTT> void StatSeq<TTT>::clear(){
    long delSucc;
    while(_dRoot!=nullptr){
      delSucc=0;
      _dRoot=removeFromTree(_dRoot,_dRoot->value,delSucc);
      if(delSucc!=0){--_size;}
    }
  }
  template<typename TTT> long StatSeq<TTT>::erase(const TTT & _value){
    long delSucc=0;
    _dRoot=removeFromTree(_dRoot,_value,delSucc);
    if(delSucc==1){
      --_size;
    }
    return delSucc;
  }
  template<typename TTT> long StatSeq<TTT>::insert(const TTT & _value){
    long addSucc=0;
    _dRoot=insertInTree(_dRoot,_value,addSucc);
    if(addSucc==1){
      ++_size;
    }
    return addSucc;
  }
  template<typename TTT> long StatSeq<TTT>::insert(const StatSeq<TTT> & _s2){
    long l2=_s2.size();
    long fR=0;
    for(int i=0;i<l2;++i){
      fR+= insert(_s2[i]);
    }
    return fR;
  }
  template<typename TTT> long StatSeq<TTT>::erase(const StatSeq<TTT> & _s2){
    long l2=_s2.size();
    long fR=0;
    for(int i=0;i<l2;++i){
      fR+= erase(_s2[i]);
    }
    return fR;
  }
  template<typename TTT> long StatSeq<TTT>::operator+=(const TTT & _value){
    return insert(_value);
  }
  template<typename TTT> long StatSeq<TTT>::operator+=(const StatSeq<TTT> & _s2){
    return insert(_s2);
  }
  template<typename TTT> long StatSeq<TTT>::operator-=(const TTT & _value){
    return erase(_value);
  }
  template<typename TTT> long StatSeq<TTT>::operator-=(const StatSeq<TTT> & _s2){
    return erase(_s2);
  }
  template<typename TTT> TTT StatSeq<TTT>::operator[](const long & i) const{
    if((i>-1)&&(i<_size)){
      return avl_access(_dRoot,i);
    }
    TTT irrelevantValue;
    return irrelevantValue;
  }
  template<typename TTT> long StatSeq<TTT>::find(const TTT & _v) const{
    AVLNode* researcher= _dRoot;
    long found=-1;
    long discardedLeft=0;
    while( (researcher!=nullptr)&&(found==-1)){
      if(_v<researcher->value){
        researcher=researcher->leftChild;
      }
      else{
        if(researcher->value < _v){
          discardedLeft+=researcher->leftCount+1;
          researcher=researcher->rightChild;
        }
        else{
          found=discardedLeft+researcher->leftCount;
        }
      }
    }
    return found;
  }
}
#endif
