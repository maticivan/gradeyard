//    GradeYard learning management system
//
//    Copyright (C) 2023 Ivan Matic, https://gradeyard.com
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

#ifndef _INCL_SetAugmentedBTree_CPP
#define _INCL_SetAugmentedBTree_CPP

#include "HBF_HdB.cpp"
#include "RNISF_resNodeInsertionOrSplit.cpp"
#include "IRFNF_IndRecFName.cpp"
#include "DMF_dataFileManagement.cpp"
#include "NSPF_nodeSplitting.cpp"
#include "TRF_treeFunctionReturns.cpp"

// This is the implementation of the augmented B-tree

namespace SETBF{
  template<typename TTT> class BasicSet{
  protected:
      std::string startingFolderName;
      HDPF::Pointer<HBF::Node<TTT> > _dRoot;
      long _size;
      long _height;
      TRF::TreeInsertReturn<TTT> insTreeAdv(HDPF::Pointer<HBF::Node<TTT> > , const TTT &  );
      HBF::Node<TTT> shiftAndInsertChild(const HDPF::Pointer<HBF::Node<TTT> > & ,
                                       const HDPF::Pointer<HBF::Node<TTT> > & ,
                                       const HBF::Node<TTT> & ,
                                       const HBF::Node<TTT> & ,
                                       const HDPF::Pointer< HBF::Node<TTT> > & ,
                                       const long & ,
                                       const long & ,
                                       const long & );
      HBF::Node<TTT> removeEdgeValueAfterEdgeDFUpdate(const HDPF::Pointer<HBF::Node<TTT> > & ,
                                                                const HBF::Node<TTT> &  ,
                                                                const long &  );
      TRF::TreeEraseReturn<TTT> getValueFromASibling(HDPF::Pointer<HBF::Node<TTT> > & ,
                                                const HBF::Node<TTT>  & ,
                                                const long & childIndex,
                                                HDPF::Pointer<HBF::Node<TTT> > & ,
                                                const HBF::Node<TTT> & ,
                                                const long & ,
                                                const long & ,
                                                const long & ,
                                                const long & ,
                                                const long & ,
                                                const long & ,
                                                const long & ,
                                                const long & ,
                                                const long & );
      TRF::TreeEraseReturn<TTT> mergeChildWithSibling(HDPF::Pointer<HBF::Node<TTT> >&  ,
                                                 const HBF::Node<TTT> &  ,
                                                 const long &  ,
                                                 const HDPF::Pointer<HBF::Node<TTT> > &  ,
                                                 const HBF::Node<TTT> &  ,
                                                 const long &  );
      TRF::TreeEraseReturn<TTT> getValueFromASibling(HDPF::Pointer<HBF::Node<TTT> > & ,
                                                const HBF::Node<TTT> & ,
                                                const long & ,
                                                HDPF::Pointer<HBF::Node<TTT> >&,
                                                const HBF::Node<TTT> &);
      HBF::Node<TTT> removeOneValueFromTheNodeThatContainsIt(const HDPF::Pointer<HBF::Node<TTT> > & ,
                                                                       const HBF::Node<TTT> & ,
                                                                       const TTT& ,
                                                                       const HDPF::Pointer<HBF::Node<TTT> > &,
                                                                       const long & );
      TRF::TreeEraseReturn<TTT> removeFromIdentifiedInternalNode(HDPF::Pointer<HBF::Node<TTT> >, const HBF::Node<TTT> &, const TTT&, const long & );
      TRF::TreeEraseReturn<TTT> removeFromComfortableLeaf(HDPF::Pointer<HBF::Node<TTT> >, const HBF::Node<TTT> &, const TTT&);
      TRF::TreeEraseReturn<TTT> removeValueThatExists(HDPF::Pointer<HBF::Node<TTT> > , const TTT& );
      TRF::TreeEraseReturn<TTT> removeFromTree(HDPF::Pointer<HBF::Node<TTT> > , const TTT& );
      HDPF::Pointer<HBF::Node<TTT> > duplicateTree(HDPF::Pointer<HBF::Node<TTT> >, const std::string &);
      std::pair<TTT,std::string> b_randomAccess(HDPF::Pointer<HBF::Node<TTT> > , const long & ) const;
      virtual void operator=(const BasicSet &);
  public:
      BasicSet();
      virtual void setStartingFolderName(const std::string &);
      std::string getStartingFolderName()const;
      virtual void copyFromSet(const BasicSet &, const std::string &);
      BasicSet(const BasicSet &);
      BasicSet(BasicSet&&);
      virtual void operator=(BasicSet &&);
      long size() const;
      virtual long erase(const TTT & );
      virtual long erase(const BasicSet<TTT> & );
      virtual long insert(const TTT & );
      virtual long insert(const BasicSet &);
      virtual long operator+=(const TTT &);
      virtual long operator+=(const BasicSet &);
      virtual long operator-=(const TTT &);
      virtual long operator-=(const BasicSet &);
      virtual IRFNF::IndRecFName<TTT> findIndexRecordFileName(const TTT & ) const;
      virtual std::pair<long,TTT> findIndexRecordPair(const TTT & ) const;
      virtual long find(const TTT &) const;
      virtual long inPlaceModification(const TTT&, const TTT&, const long & = 1);
      IRFNF::IndRecFName<TTT> lowerBoundIndexRecordFileName(const TTT & , const long& =0 ) const;
      std::pair<long,TTT> lowerBoundIndexRecordPair(const TTT & ) const;
      long lowerBound(const TTT &) const;
      virtual void clear();
      void saveToFile(const std::string &, const std::string & = "noSpecialNameForSet") const;
      int loadFromFile(const std::string &, const std::string & = "noSpecialNameForSet");
      std::string saveInitializationToString(const std::string &, const std::string & = "noSpecialNameForSet") const;
      int loadInitializationFromString(const std::string &, const std::string & = "noSpecialNameForSet");
      std::pair<TTT,std::string> operator[](const long & ) const;
      virtual ~BasicSet();
      std::string statusReportForDebugging() const;
  };
  template<typename TTT>  BasicSet<TTT>::BasicSet(const BasicSet<TTT>& _copyFrom){
    startingFolderName=_copyFrom.startingFolderName;
    _dRoot= duplicateTree(_copyFrom._dRoot,startingFolderName);
    _size= _copyFrom._size;
    _height=_copyFrom._height;
  }
  template<typename TTT> BasicSet<TTT>::BasicSet(){
      // - no need to set to null pointer - the constructor for HDPointer will do that
      _size=0;
      _height=-117;
  }
  template<typename TTT> void BasicSet<TTT>::setStartingFolderName(const std::string &_stFN){
    startingFolderName=_stFN;
  }
  template<typename TTT> std::string BasicSet<TTT>::getStartingFolderName()const{
    return startingFolderName;
  }
  template<typename TTT> long BasicSet<TTT>::size() const{
      return _size;
  }
  template<typename TTT>  BasicSet<TTT>::~BasicSet(){
  }
  template<typename TTT> std::string BasicSet<TTT>::statusReportForDebugging() const{
    std::string fR="starting folder name: "+startingFolderName+"\n";
    fR+="size: "+std::to_string(_size)+"\n";
    fR+="Root of the tree report start:\n"+_dRoot.statusReportForDebugging();
    fR+="\nRoot of the tree report end.";
    return fR;
  }
  template<typename TTT>
  TRF::TreeInsertReturn<TTT> BasicSet<TTT>::insTreeAdv(HDPF::Pointer<HBF::Node<TTT> > root, const TTT & _value){
      if(root.isNullptr()){
          TRF::TreeInsertReturn<TTT> fR;
          (fR.aP).allocateMemory(startingFolderName);
          HBF::Node<TTT> newVal(_value);
          (fR.aP).deRefWrite(newVal);
          (fR.success)=1;
          return fR;
      }
      HBF::Node<TTT> starRoot=root.deRefRead();
      //Root is not null pointer!
      if(starRoot.height==0){
        // root is empty - has zero height but the memory is allocated to it.
        TRF::TreeInsertReturn<TTT> fR;
        fR.aP=root;
        HBF::Node<TTT> newVal(_value);
        root.deRefWrite(newVal);
        (fR.success)=1;
        return fR;
      }
      // Root is non-trivial
      if( starRoot.height==1){
        // This is a leaf
        RNISF::ResultInsSplit<TTT> resIns=NSPF::insertAndSplitIfNecessary(root,starRoot,_value);
        TRF::TreeInsertReturn<TTT> fR;
        fR.aP=root;
        if(resIns.indicatorInsertOrSplitOrNothing==0){
          return fR;
        }
        fR.success=1;
        if(resIns.indicatorInsertOrSplitOrNothing==1){
          root.deRefWrite(resIns.resultOfInsert);
          return fR;
        }
        fR.nodeSplitIndicator=1;
        fR.aLeft=root;
        fR.cLeft=(resIns.countsSplit).first;
        (fR.aRight).allocateMemory(startingFolderName);
        fR.cRight=(resIns.countsSplit).second;
        (fR.aLeft).deRefWrite((resIns.resultOfSplit).first);
        (fR.aRight).deRefWrite((resIns.resultOfSplit).second);
        fR.centralValue=resIns.centralValue;
        fR.pointerToOldLocationOfTheCentralValue=resIns.pointerToOldLocationOfTheCentralValue;
        fR.positionInNodeOfTheCentralValue=resIns.positionInNodeOfTheCentralValue;
        DMF::shiftLeftDFNames(fR.aLeft,fR.aRight,HDPF::GLOBAL_PS.getTDegree()+1,((resIns.resultOfSplit).second).num+HDPF::GLOBAL_PS.getTDegree()+1, HDPF::GLOBAL_PS.getTDegree()+1);
        return fR;
      }
      // Internal node. Its height is bigger than 1
      long indToFollow=VSF::lowerBoundSortedVector(starRoot.values,_value);
      if( (indToFollow>-1) && (VSF::valuesEqual(starRoot.values[indToFollow],_value))) {
        // no point in inserting - the value already exists
        TRF::TreeInsertReturn<TTT> fR;
        fR.aP=root;
        return fR;
      }
      indToFollow += 1;
      TRF::TreeInsertReturn<TTT> resultOfInsertion=insTreeAdv((starRoot.children)[indToFollow],_value);
      TRF::TreeInsertReturn<TTT> fR;
      fR.aP=root;
      if(resultOfInsertion.success==0){
        // The node was already in the tree, no insertion needed.
        return fR;
      }
      fR.success=1;
      if(resultOfInsertion.nodeSplitIndicator==0){
        // The child did not have to be split.
        // The count of the child has increased by 1.
        (starRoot.counts)[indToFollow] += 1;
        root.deRefWrite(starRoot);
        return fR;
      }
      // The child node was split and we are receiving the middle node
      RNISF::ResultInsSplit<TTT>
      resIns=NSPF::insertAndSplitIfNecessary(root,starRoot,resultOfInsertion.centralValue,
                                      resultOfInsertion.pointerToOldLocationOfTheCentralValue,
                                      resultOfInsertion.positionInNodeOfTheCentralValue,
                                      resultOfInsertion.aLeft, resultOfInsertion.aRight,resultOfInsertion.cLeft,resultOfInsertion.cRight);
      if(resIns.indicatorInsertOrSplitOrNothing==1){
          // insertion happened. The root was able to handle the new node without splitting
          // The root became bigger.
          // The new value of the root is resIns.resultOfInsert
          root.deRefWrite(resIns.resultOfInsert);
          return fR;
        }
        // after the insertion the root needs to be split
        fR.nodeSplitIndicator=1;
        fR.aLeft=root;
        fR.cLeft=(resIns.countsSplit).first;
        (fR.aRight).allocateMemory(startingFolderName);
        fR.cRight=(resIns.countsSplit).second;
        (fR.aLeft).deRefWrite((resIns.resultOfSplit).first);
        (fR.aRight).deRefWrite((resIns.resultOfSplit).second);
        fR.centralValue=resIns.centralValue;
        fR.pointerToOldLocationOfTheCentralValue=resIns.pointerToOldLocationOfTheCentralValue;
        fR.positionInNodeOfTheCentralValue=resIns.positionInNodeOfTheCentralValue;
        DMF::shiftLeftDFNames(fR.aLeft,fR.aRight,HDPF::GLOBAL_PS.getTDegree()+1,((resIns.resultOfSplit).second).num+HDPF::GLOBAL_PS.getTDegree()+1, HDPF::GLOBAL_PS.getTDegree()+1);
        return fR;
  }
  template<typename TTT> long  BasicSet<TTT>::insert(const TTT & _value){
      TRF::TreeInsertReturn<TTT> insRes=insTreeAdv(_dRoot,_value);
      if(insRes.success==1){
        ++_size;
        if(_height<1){
          _height=1;
        }
      }
      if(insRes.nodeSplitIndicator==1){
        //the root has grown
        HBF::Node<TTT> starNewRoot(insRes.centralValue);
        starNewRoot.children[0]=insRes.aLeft;
        starNewRoot.children[1]=insRes.aRight;
        starNewRoot.counts[0]=insRes.cLeft;
        starNewRoot.counts[1]=insRes.cRight;
        _height=_height+1;
        starNewRoot.height=_height;
        _dRoot.allocateMemory(startingFolderName);
        _dRoot.deRefWrite(starNewRoot);
        DMF::renameDataFile(insRes.pointerToOldLocationOfTheCentralValue,insRes.positionInNodeOfTheCentralValue,_dRoot,0);
      }
      else{
        _dRoot=insRes.aP;
      }
      return insRes.success;
  }
  template<typename TTT> long  BasicSet<TTT>::insert(const BasicSet<TTT> & _s2){
    long l2=_s2.size();
    long fR=0;
    for(int i=0;i<l2;++i){
      fR+= insert(_s2[i].first);
    }
    return fR;
  }
  template<typename TTT> long  BasicSet<TTT>::operator+=(const TTT & _value){
    return insert(_value);
  }
  template<typename TTT> long  BasicSet<TTT>::operator+=(const BasicSet<TTT> & _s2){
    return insert(_s2);
  }
  template<typename TTT>
  IRFNF::IndRecFName<TTT> BasicSet<TTT>::findIndexRecordFileName(const TTT & _v) const{
    HDPF::Pointer<HBF::Node<TTT> > researcher= _dRoot;
    HBF::Node<TTT> starRes;
    IRFNF::IndRecFName<TTT> found;
    found.fileName="notFound";
    found.index=-1;
    long indexLowerBound;
    long ilbp1;
    long discardedLeft=0;
    while( (!researcher.isNullptr())&&(found.index==-1)){
        starRes=researcher.deRefRead();
        indexLowerBound=VSF::lowerBoundSortedVector(starRes.values,_v);
        ilbp1= indexLowerBound+1;
        discardedLeft +=ilbp1;
        for(long i=0;i<ilbp1;++i){
          discardedLeft += starRes.counts[i];
        }
        if( (indexLowerBound>-1) && VSF::valuesEqual(_v,starRes.values[indexLowerBound])){
          found.index=discardedLeft-1;
          found.record=starRes.values[indexLowerBound];
          found.fileName=researcher.getNameOfFile();
          found.aRecord=researcher;
          found.positionInNode=indexLowerBound;
        }
        else{
          researcher=starRes.children[ilbp1];
        }
    }
    return found;
  }
  template<typename TTT>
  std::pair<long,TTT> BasicSet<TTT>::findIndexRecordPair(const TTT & _v) const{
      IRFNF::IndRecFName<TTT> irf=findIndexRecordFileName(_v);
      return std::pair<long,TTT>(irf.index,irf.record);
  }
  template<typename TTT> long BasicSet<TTT>::find(const TTT & _v) const{
    return (findIndexRecordFileName(_v)).index;
  }
  template<typename TTT> long BasicSet<TTT>::inPlaceModification(const TTT & _oldVal, const TTT & _newVal, const long & _safeMode){
    IRFNF::IndRecFName<TTT> irfnOld = findIndexRecordFileName(_oldVal);
    long prevIndex=irfnOld.index-1;
    long nextIndex=irfnOld.index+1;
    if(_safeMode==1){
      if(prevIndex > -1){
        if(!(b_randomAccess(_dRoot,prevIndex).first < _newVal)){
          return 0;
        }
      }
      if(nextIndex < _size){
        if(!(_newVal < b_randomAccess(_dRoot,nextIndex).first)){
          return 0;
        }
      }
    }
    // We are allowed to do in-place modification from _oldVal to _newVal
    HBF::Node<TTT> currentNode = irfnOld.aRecord.deRefRead();
    currentNode.values[irfnOld.positionInNode]=_newVal;
    irfnOld.aRecord.deRefWrite(currentNode);
    return 1;
  }
  template<typename TTT>
  IRFNF::IndRecFName<TTT> BasicSet<TTT>::lowerBoundIndexRecordFileName(const TTT & _v, const long & populateRecordFieldEvenIfMissmatch) const{
    HDPF::Pointer<HBF::Node<TTT> > researcher= _dRoot;
    HBF::Node<TTT> starRes;
    IRFNF::IndRecFName<TTT> found;
    found.fileName="notFound";
    found.index=-1;
    long indexLowerBound=-5;
    long ilbp1;
    long discardedLeft=0;
    TTT previousStepLowerBound;
    long previousStepLowerBoundSet=0;
    while( (!researcher.isNullptr())&&(found.index==-1)){
        starRes=researcher.deRefRead();
        indexLowerBound=VSF::lowerBoundSortedVector(starRes.values,_v);
        ilbp1= indexLowerBound+1;
        discardedLeft +=ilbp1;
        for(long i=0;i<ilbp1;++i){
          discardedLeft += starRes.counts[i];
        }
        if( (indexLowerBound>-1) && VSF::valuesEqual(_v,starRes.values[indexLowerBound])){
          found.index=discardedLeft-1;
          found.record=starRes.values[indexLowerBound];
          found.fileName=researcher.getNameOfFile();
          found.aRecord=researcher;
          found.positionInNode=indexLowerBound;
        }
        else{
          researcher=starRes.children[ilbp1];
          if(indexLowerBound>-1){
            previousStepLowerBoundSet=1;
            previousStepLowerBound=starRes.values[indexLowerBound];
          }
        }
    }
    found.index=discardedLeft-1;
    if( populateRecordFieldEvenIfMissmatch==1) {
      if(indexLowerBound>-1){
        found.record=starRes.values[indexLowerBound];
      }
      else{
        if(previousStepLowerBoundSet){
          found.record=previousStepLowerBound;
        }
      }
    }
    return found;
  }
  template<typename TTT>
  std::pair<long,TTT> BasicSet<TTT>::lowerBoundIndexRecordPair(const TTT & _v) const{
      IRFNF::IndRecFName<TTT> irf=lowerBoundIndexRecordFileName(_v);
      return std::pair<long,TTT>(irf.index,irf.record);
  }
  template<typename TTT> long BasicSet<TTT>::lowerBound(const TTT & _v) const{
    return (lowerBoundIndexRecordFileName(_v)).index;
  }
  template<typename TTT>
  std::pair<TTT,std::string> BasicSet<TTT>::b_randomAccess(HDPF::Pointer<HBF::Node<TTT> > root,
                                     const long &i) const{
      TTT tempWithRandValue;
      std::pair<TTT,std::string> failure=std::pair<TTT,std::string>(tempWithRandValue,"notFound");
      if(root.isNullptr()){
          return failure;
      }
      HBF::Node<TTT> starRoot=root.deRefRead();
      if(starRoot.height==1){
        if(i<starRoot.num){
          return std::pair<TTT,std::string>(starRoot.values[i],root.getNameOfFile());
        }
        return failure;
      }
      long remainingI=i;
      long cntr=0;
      long np1=starRoot.num+1;
      while((remainingI>=starRoot.counts[cntr])&&(cntr<np1)){
        remainingI -= starRoot.counts[cntr];
        if(remainingI==0){
          return std::pair<TTT,std::string>(starRoot.values[cntr],root.getNameOfFile());
        }
        --remainingI;
        ++cntr;
      }
      if(cntr==np1){
        return failure;
      }
      return b_randomAccess(starRoot.children[cntr], remainingI);
  }
  template<typename TTT>
  std::pair<TTT,std::string> BasicSet<TTT>::operator[](const long & i) const{
      if((i>-1)&&(i<_size)){
          return b_randomAccess(_dRoot,i);
      }
      TTT randomValue;
      return std::pair<TTT,std::string>(randomValue,"notFound");
  }
  template<typename TTT>
  std::string BasicSet<TTT>::saveInitializationToString(const std::string & priorStrToSave, const std::string &sN) const{
      std::string strToSave="";
      std::string setNameB="_setN!*"+sN+"!*_";
      std::string setNameE="_/setN!*"+sN+"!*_";
      std::string setPtB="_setPT!*_";
      std::string setPtE="_/setPT!*_";
      std::string setSzB="_setSZ!*_";
      std::string setSzE="_/setSZ!*_";
      std::string setHiB="_setHi!*_";
      std::string setHiE="_/setHi!*_";
      std::string setStFNB="_stfn!*_";
      std::string setStFNE="_/stfn!*_";
      long pos=0;
      strToSave=SF::eraseStuffBetween(priorStrToSave,setNameB,setNameE,pos).first;
      strToSave+= setNameB;
      strToSave+= setPtB+ _dRoot.putIntoString()+setPtE;
      strToSave+= setSzB+ std::to_string(_size)+setSzE;
      strToSave+= setHiB+std::to_string(_height)+setHiE;
      strToSave+= setStFNB + startingFolderName+setStFNE;
      strToSave+= setNameE;
      return strToSave;
  }
  template<typename TTT>
  void BasicSet<TTT>::saveToFile(const std::string &fN, const std::string &sN) const{
      std::string strToSave="";
      if(IOF::fileExists(fN)){
          strToSave=IOF::fileToString(fN);
      }
      strToSave=saveInitializationToString(strToSave,sN);
      IOF::toFile(fN,strToSave);
  }
  template<typename TTT>
  int BasicSet<TTT>::loadInitializationFromString(const std::string & _allData, const std::string &sN){
      std::string allData=_allData;
      std::string setNameB="_setN!*"+sN+"!*_";
      std::string setNameE="_/setN!*"+sN+"!*_";
      std::string setPtB="_setPT!*_";
      std::string setPtE="_/setPT!*_";
      std::string setSzB="_setSZ!*_";
      std::string setSzE="_/setSZ!*_";
      std::string setHiB="_setHi!*_";
      std::string setHiE="_/setHi!*_";
      std::string setStFNB="_stfn!*_";
      std::string setStFNE="_/stfn!*_";
      long pos=0;
      std::pair<std::string,int> setData=SF::extract(allData,pos,setNameB,setNameE);
      if(setData.second==0){
          return 0;
      }
      pos=0;allData=setData.first;
      std::pair<std::string,int> setPt=SF::extract(allData,pos,setPtB,setPtE);
      if(setPt.second==0){
          return 0;
      }
      //pos=0;
      std::pair<std::string,int> setSz=SF::extract(allData,pos,setSzB,setSzE);
      if(setSz.second==0){
          return 0;
      }
      _size=BF::stringToInteger(setSz.first);
      //pos=0;
      std::pair<std::string,int> setHi=SF::extract(allData,pos,setHiB,setHiE);
      if(setHi.second==0){
          return 0;
      }
      _height=BF::stringToInteger(setHi.first);
      //pos=0;
      std::pair<std::string,int> setStF=SF::extract(allData,pos,setStFNB,setStFNE);
      if(setStF.second==0){
          return 0;
      }
      startingFolderName=setStF.first;
      _dRoot.loadFromString(setPt.first);
      return 1;
  }
  template<typename TTT>
  int BasicSet<TTT>::loadFromFile(const std::string &fN, const std::string &sN){
      if(IOF::fileExists(fN)){
          std::string allData=IOF::fileToString(fN);
          return loadInitializationFromString(allData,sN);
      }
      return 0;
  }
  template<typename TTT>
  HDPF::Pointer<HBF::Node<TTT> > BasicSet<TTT>::duplicateTree(HDPF::Pointer<HBF::Node<TTT> >root,const std::string & stFolderToDuplicate){
    HDPF::Pointer<HBF::Node<TTT> >newN;
      if(!root.isNullptr()){
          newN.allocateMemory(stFolderToDuplicate);
          HBF::Node<TTT> starNewN;
          HBF::Node<TTT> starRoot=root.deRefRead();
          starNewN.height=starRoot.height;
          starNewN.setNum(starRoot.num);
          for(long i=0;i<starRoot.num;++i){
            (starNewN.values)[i]=(starRoot.values)[i];
          }
          long np1=starRoot.num+1;
          for(long i=0;i<np1;++i){
            (starNewN.counts)[i]=(starRoot.counts)[i];
            (starNewN.children)[i]=duplicateTree((starRoot.children)[i],stFolderToDuplicate);
          }
          newN.deRefWrite(starNewN);
      }
      return newN;
  }
  template<typename TTT> void BasicSet<TTT>::clear(){
      long delSucc;
      while(!_dRoot.isNullptr()){
          removeFromTree(_dRoot,(_dRoot.deRefRead()).values[0]);
          --_size;
      }
  }
  template<typename TTT> void BasicSet<TTT>::copyFromSet(const BasicSet<TTT>& _copyFrom,const std::string & _sFN){
    if(&_copyFrom!=this){
      clear();
      startingFolderName=_sFN;
      _dRoot= duplicateTree(_copyFrom._dRoot,startingFolderName);
      _size= _copyFrom._size;
      _height=_copyFrom._height;
    }
  }
  template<typename TTT> void BasicSet<TTT>::operator=(const BasicSet<TTT>& _copyFrom){
    if(&_copyFrom!=this){
      clear();
      copyFromSet(_copyFrom,_copyFrom.startingFolderName);
    }
  }
  template<typename TTT>
  HBF::Node<TTT> BasicSet<TTT>::removeOneValueFromTheNodeThatContainsIt(const HDPF::Pointer<HBF::Node<TTT> > & root,
                                                                   const HBF::Node<TTT> & starRoot,
                                                                   const TTT& _value,
                                                                   const HDPF::Pointer<HBF::Node<TTT> > &megaChild,
                                                                   const long & leafIndicator){
    HBF::Node<TTT> starNew;
    starNew.setNum(starRoot.num-1);
    starNew.height=starRoot.height;
    long iOld=0,iNew=0;
    long nodeRemoved=0;
    while(iOld<starRoot.num){
      if(!VSF::valuesEqual(starRoot.values[iOld],_value)){
        starNew.values[iNew]=starRoot.values[iOld];
        starNew.counts[iNew+nodeRemoved]=starRoot.counts[iOld+nodeRemoved];
        starNew.children[iNew+nodeRemoved]=starRoot.children[iOld+nodeRemoved];
        ++iNew;
      }
      else{
        starNew.children[iNew]=megaChild;
        starNew.counts[iNew]=starRoot.counts[iOld]+starRoot.counts[iOld+1];
        //the previous line should be
        //starNew.counts[iNew]=starRoot.counts[iOld]+starRoot.counts[iOld+1]+1;
        // (notice "+1" in the end)
        // however, whenever this function is called, the value is later erased, so we are saving one root update
        // future functions must be aware of this fact.
        nodeRemoved=1;
        if(leafIndicator){
          DMF::deleteDataFile(root,iOld);
        }
      }
      ++iOld;
      if((nodeRemoved)&&(iOld<starRoot.num)){
        DMF::renameDataFile(root,iOld,root,iOld-1);
      }
    }
    return starNew;
  }
  template<typename TTT>
  HBF::Node<TTT> BasicSet<TTT>::removeEdgeValueAfterEdgeDFUpdate(const HDPF::Pointer<HBF::Node<TTT> > & root,
                                                            const HBF::Node<TTT> & starRoot,
                                                            const long & left0_right1){
    HBF::Node<TTT> starNew;
    starNew.setNum(starRoot.num-1);
    starNew.height=starRoot.height;
    long iNew=0;
    if(left0_right1==0){
      DMF::shiftLeftDFNames(root,root,1,starRoot.num,1);
    }
    long shv=1-left0_right1;
    for(long i=0;i<starNew.num;++i){
      starNew.values[i]=starRoot.values[i+shv];
      starNew.children[i]=starRoot.children[i+shv];
      starNew.counts[i]=starRoot.counts[i+shv];
    }
    starNew.children[starNew.num]=starRoot.children[starNew.num+shv];
    starNew.counts[starNew.num]=starRoot.counts[starNew.num+shv];
    return starNew;
  }
  template<typename TTT>
  HBF::Node<TTT> BasicSet<TTT>::shiftAndInsertChild(const HDPF::Pointer<HBF::Node<TTT> > & root,
                                               const HDPF::Pointer<HBF::Node<TTT> > & aChild,
                                               const HBF::Node<TTT> & starRoot,
                                               const HBF::Node<TTT> & starChild,
                                               const HDPF::Pointer<HBF::Node<TTT> >& childPointerNew,
                                               const long & countsAttNew,
                                               const long & positionOfChangeInRoot,
                                               const long & childShiftRightInd){
    HBF::Node<TTT> starNewChild;
    starNewChild.setNum(starChild.num+1);
    starNewChild.height=starChild.height;
    long indexOfNewItem;
    long indexOfNewGrandChild;
    if(childShiftRightInd==1){
      DMF::shiftRightDFNames(aChild,aChild,0,starChild.num,1);
      indexOfNewItem=0;
      indexOfNewGrandChild=0;
    }
    else{
      indexOfNewItem=starChild.num;
      indexOfNewGrandChild=starChild.num+1;
    }
    DMF::renameDataFile(root,positionOfChangeInRoot,aChild,indexOfNewItem);
    for(long i=0;i<starChild.num;++i){
      starNewChild.values[i+childShiftRightInd]=starChild.values[i];
      starNewChild.children[i+childShiftRightInd]=starChild.children[i];
      starNewChild.counts[i+childShiftRightInd]=starChild.counts[i];
    }
    starNewChild.children[starChild.num+childShiftRightInd]=starChild.children[starChild.num];
    starNewChild.counts[starChild.num+childShiftRightInd]=starChild.counts[starChild.num];
    starNewChild.values[indexOfNewItem]=starRoot.values[positionOfChangeInRoot];
    starNewChild.counts[indexOfNewGrandChild]=countsAttNew;
    starNewChild.children[indexOfNewGrandChild]=childPointerNew;
    return starNewChild;
  }
  template<typename TTT>
  TRF::TreeEraseReturn<TTT> BasicSet<TTT>::removeFromComfortableLeaf(HDPF::Pointer<HBF::Node<TTT> > root, const HBF::Node<TTT>& starRoot, const TTT& _value){
    TRF::TreeEraseReturn<TTT> fR;
    fR.success=1;
    if(starRoot.num==1){
      DMF::deleteDataFile(root,0);
      _dRoot.deAllocateMemory();
      HDPF::Pointer<HBF::Node<TTT> > nullPointer;
      _dRoot=nullPointer;
      fR.aP=_dRoot;
      fR.lastNodeRemovedAndTreeBecameEmpty=1;
      return fR;
    }
    HDPF::Pointer<HBF::Node<TTT> > nullPointer;
    HBF::Node<TTT> starNew=removeOneValueFromTheNodeThatContainsIt(root,starRoot,_value,nullPointer,1);
    root.deRefWrite(starNew);
    fR.aP=root;
    return fR;
  }
  template<typename TTT>
  TRF::TreeEraseReturn<TTT> BasicSet<TTT>::removeFromIdentifiedInternalNode(HDPF::Pointer<HBF::Node<TTT> > root, const HBF::Node<TTT>& starRoot, const TTT& _value, const long & leftChildInd){
    TRF::TreeEraseReturn<TTT> fR;
    long valIndex=leftChildInd;
    long rightChildInd=leftChildInd+1;
    HDPF::Pointer<HBF::Node<TTT> > aLC=starRoot.children[leftChildInd];
    HDPF::Pointer<HBF::Node<TTT> > aRC=starRoot.children[rightChildInd];
    IRFNF::IndRecFName<TTT> myIRFN=findIndexRecordFileName(_value);
    HBF::Node<TTT> stLeftCh=aLC.deRefRead();
    HBF::Node<TTT> stRightCh=aRC.deRefRead();
    long indexOfNodeToBeRemovedFromComfortableChild=-1;
    long indComfortableChild;
    if(stRightCh.num >= HDPF::GLOBAL_PS.getTDegree()){
      indexOfNodeToBeRemovedFromComfortableChild=myIRFN.index+1;
      indComfortableChild=rightChildInd;
    }
    if(stLeftCh.num >= HDPF::GLOBAL_PS.getTDegree()){
      indexOfNodeToBeRemovedFromComfortableChild=myIRFN.index-1;
      indComfortableChild=leftChildInd;
    }
    if(indexOfNodeToBeRemovedFromComfortableChild > -1){
      IRFNF::IndRecFName<TTT> replIRFN = findIndexRecordFileName( (b_randomAccess(_dRoot,indexOfNodeToBeRemovedFromComfortableChild)).first );
      DMF::deleteDataFile(root,valIndex);
      DMF::renameDataFile(replIRFN.aRecord, replIRFN.positionInNode ,root, valIndex);
      removeValueThatExists(starRoot.children[indComfortableChild],replIRFN.record);
      HBF::Node<TTT> starNew=starRoot;
      starNew.values[valIndex]=replIRFN.record;
      starNew.counts[indComfortableChild] -= 1;
      root.deRefWrite(starNew);
      TRF::TreeEraseReturn<TTT> fR;
      fR.aP=root;
      fR.success=1;
      return fR;
    }
    // Merging children:
    DMF::renameDataFile(root,valIndex,aLC,stLeftCh.num);
    for(long i=0;i<stRightCh.num;++i){
      DMF::renameDataFile(aRC,i,aLC,stLeftCh.num+i+1);
    }
    HBF::Node<TTT> starNew=removeOneValueFromTheNodeThatContainsIt(root,starRoot,_value,aLC, 0);
    root.deRefWrite(starNew);
    HBF::Node<TTT> megaChild;
    megaChild.setNum(stLeftCh.num+stRightCh.num+1);
    megaChild.height=stLeftCh.height;
    long counter=0;
    while(counter<stLeftCh.num){
      megaChild.values[counter]=stLeftCh.values[counter];
      megaChild.counts[counter]=stLeftCh.counts[counter];
      megaChild.children[counter]=stLeftCh.children[counter];
      ++counter;
    }
    megaChild.values[counter]=_value;
    megaChild.children[counter]=stLeftCh.children[counter];
    megaChild.counts[counter]=stLeftCh.counts[counter];
    long shift=counter+1;
    counter=0;
    while(counter<stRightCh.num){
      megaChild.values[shift]=stRightCh.values[counter];
      megaChild.children[shift]=stRightCh.children[counter];
      megaChild.counts[shift]=stRightCh.counts[counter];
      ++counter;++shift;
    }
    megaChild.children[shift]=stRightCh.children[counter];
    megaChild.counts[shift]=stRightCh.counts[counter];
    aLC.deRefWrite(megaChild);
    aRC.deAllocateMemory();
    removeValueThatExists(aLC,_value);
    fR.aP=root;
    fR.success=1;
    if((starNew.height==_height) && (starNew.num==0)){
      HDPF::Pointer<HBF::Node<TTT> > _newRoot=starNew.children[0];
      _dRoot.deAllocateMemory();
      _dRoot=_newRoot;
      --_height;
      fR.aP=_dRoot;
      fR.heightDecreased=1;
    }
    return fR;
  }
  template<typename TTT>
  TRF::TreeEraseReturn<TTT> BasicSet<TTT>::getValueFromASibling(HDPF::Pointer<HBF::Node<TTT> > & root,
                                                        const HBF::Node<TTT>  & starRoot,
                                                        const long & childIndex,
                                                        HDPF::Pointer<HBF::Node<TTT> > & aChild,
                                                        const HBF::Node<TTT> & starChild,
                                                        const long & positionOfChangeInRoot,
                                                        const long & positionOfSiblingToInvestigate,
                                                        const long & positionOfChildPointerToChange,
                                                        const long & _indexOfValueToBeTakenFromSibling,
                                                        const long & _indexOfChildToBeTakenFromSibling,
                                                        const long & positionOfValueToBePutInChild,
                                                        const long & positionOfChildToBePutInChild,
                                                        const long & childShiftRightInd,
                                                        const long & siblingEdgeToRemoveLeft0Right1
                                                        ){
    HDPF::Pointer<HBF::Node<TTT> > aSibling=starRoot.children[positionOfSiblingToInvestigate];
    HBF::Node<TTT> starSibling=aSibling.deRefRead();
    HBF::Node<TTT> starNewSibling,starNewChild,starNewRoot;
    long indexOfValueToBeTakenFromSibling=_indexOfValueToBeTakenFromSibling;
    long indexOfChildToBeTakenFromSibling=_indexOfChildToBeTakenFromSibling;
    TRF::TreeEraseReturn<TTT> fR;
    if(indexOfValueToBeTakenFromSibling==-1){
      indexOfValueToBeTakenFromSibling=starSibling.num-1;
      indexOfChildToBeTakenFromSibling=starSibling.num;
    }
    if(starSibling.num > HDPF::GLOBAL_PS.getTDegree()-1){
      fR.indexOfComfortableSibling=positionOfSiblingToInvestigate;
      starNewChild=shiftAndInsertChild(root,aChild,starRoot, starChild,
                                       starSibling.children[indexOfChildToBeTakenFromSibling],
                                       starSibling.counts[indexOfChildToBeTakenFromSibling],
                                       positionOfChangeInRoot,childShiftRightInd);
      DMF::renameDataFile(aSibling,indexOfValueToBeTakenFromSibling,root,positionOfChangeInRoot);
      starNewSibling=removeEdgeValueAfterEdgeDFUpdate(aSibling,starSibling, siblingEdgeToRemoveLeft0Right1);
      starNewRoot=starRoot;
      starNewRoot.values[positionOfChangeInRoot]=starSibling.values[indexOfValueToBeTakenFromSibling];
      starNewRoot.counts[positionOfChildPointerToChange] -=( starSibling.counts[indexOfChildToBeTakenFromSibling]+1);
      starNewRoot.counts[childIndex] += ( starSibling.counts[indexOfChildToBeTakenFromSibling]+1);
      root.deRefWrite(starNewRoot);
      aChild.deRefWrite(starNewChild);
      aSibling.deRefWrite(starNewSibling);
      fR.starRootAfterTrading=starNewRoot;
    }
    else{
      fR.indexOfSiblingToMergeWith=positionOfSiblingToInvestigate;
    }
    return fR;
  }
  template<typename TTT>
  TRF::TreeEraseReturn<TTT> BasicSet<TTT>::getValueFromASibling(HDPF::Pointer<HBF::Node<TTT> > & root,
                                                        const HBF::Node<TTT>  & starRoot,
                                                        const long & childIndex,
                                                        HDPF::Pointer<HBF::Node<TTT> > & aChild,
                                                        const HBF::Node<TTT> & starChild){
    TRF::TreeEraseReturn<TTT> fR;
    long indexOfValueToBeTakenFromSibling=-1;
    long indexOfChildToBeTakenFromSibling=-1;
    long positionOfValueToBePutInChild=-1;
    long positionOfChildToBePutInChild=-1;
    long positionOfChangeInRoot=-1;
    long positionOfSiblingToInvestigate=-1;
    long positionOfChildPointerToChange=-1;
    long childShiftRightInd;
    long siblingEdgeToRemoveLeft0Right1;
    if(childIndex>0){
      positionOfChangeInRoot=childIndex-1;
      positionOfSiblingToInvestigate=childIndex-1;
      positionOfChildPointerToChange=childIndex-1;
      indexOfValueToBeTakenFromSibling=-1;
      indexOfChildToBeTakenFromSibling=-1;
      positionOfValueToBePutInChild=0;
      positionOfChildToBePutInChild=0;
      childShiftRightInd=1;
      siblingEdgeToRemoveLeft0Right1=1;
      fR=getValueFromASibling(root,starRoot, childIndex, aChild, starChild,
                              positionOfChangeInRoot, positionOfSiblingToInvestigate, positionOfChildPointerToChange,
                              indexOfValueToBeTakenFromSibling, indexOfChildToBeTakenFromSibling,
                              positionOfValueToBePutInChild, positionOfChildToBePutInChild,
                              childShiftRightInd,siblingEdgeToRemoveLeft0Right1);
    }
    if((fR.indexOfComfortableSibling == -1) && (childIndex < starRoot.num)) {
      positionOfChangeInRoot=childIndex;
      positionOfSiblingToInvestigate=childIndex+1;
      positionOfChildPointerToChange=childIndex+1;
      indexOfValueToBeTakenFromSibling=0;
      indexOfChildToBeTakenFromSibling=0;
      positionOfValueToBePutInChild=starChild.num;
      positionOfChildToBePutInChild=starChild.num+1;
      childShiftRightInd=0;
      siblingEdgeToRemoveLeft0Right1=0;
      fR=getValueFromASibling(root,starRoot, childIndex, aChild, starChild,
                              positionOfChangeInRoot, positionOfSiblingToInvestigate, positionOfChildPointerToChange,
                              indexOfValueToBeTakenFromSibling, indexOfChildToBeTakenFromSibling,
                              positionOfValueToBePutInChild, positionOfChildToBePutInChild,
                              childShiftRightInd,siblingEdgeToRemoveLeft0Right1);
    }
    return fR;
  }
  template<typename TTT>
  TRF::TreeEraseReturn<TTT> BasicSet<TTT>::mergeChildWithSibling(HDPF::Pointer<HBF::Node<TTT> >& root,
                                                         const HBF::Node<TTT> & starRoot,
                                                         const long & indexToChild,
                                                         const HDPF::Pointer<HBF::Node<TTT> > & aChild,
                                                         const HBF::Node<TTT> & starChild,
                                                         const long & indSibling){
    TRF::TreeEraseReturn<TTT> fR;
    HDPF::Pointer<HBF::Node<TTT> > aLeftC=aChild, aRightC=starRoot.children[indSibling];
    long indCentral=indexToChild;
    fR.indexToFollowAfterMerging=indexToChild;
    if(indSibling<indexToChild){
      fR.indexToFollowAfterMerging=indSibling;
      aLeftC=aRightC; aRightC=aChild;
      indCentral=indSibling;
    }
    long megaCount=starRoot.counts[indCentral]+starRoot.counts[indCentral+1]+1;
    HBF::Node<TTT> starLeftC=aLeftC.deRefRead();
    HBF::Node<TTT> starRightC=aRightC.deRefRead();
    DMF::renameDataFile(root,indCentral,aLeftC,starLeftC.num);
    DMF::shiftRightDFNames(aRightC,aLeftC,0,starRightC.num,starLeftC.num+1);
    HBF::Node<TTT> megaChild;
    megaChild.setNum(starLeftC.num+starRightC.num+1);
    megaChild.height=starLeftC.height;
    for(long i=0;i<starLeftC.num;++i){
      megaChild.values[i]=starLeftC.values[i];
      megaChild.counts[i]=starLeftC.counts[i];
      megaChild.children[i]=starLeftC.children[i];
    }
    megaChild.values[starLeftC.num]=starRoot.values[indCentral];
    megaChild.counts[starLeftC.num]=starLeftC.counts[starLeftC.num];
    megaChild.children[starLeftC.num]=starLeftC.children[starLeftC.num];
    long shv=starLeftC.num+1;
    for(long i=0;i<starRightC.num;++i){
      megaChild.values[i+shv]=starRightC.values[i];
      megaChild.counts[i+shv]=starRightC.counts[i];
      megaChild.children[i+shv]=starRightC.children[i];
    }
    megaChild.counts[megaChild.num]=starRightC.counts[starRightC.num];
    megaChild.children[megaChild.num]=starRightC.children[starRightC.num];
    aLeftC.deRefWrite(megaChild);
    aRightC.deAllocateMemory();
    fR.starRootAfterMerging= removeOneValueFromTheNodeThatContainsIt(root, starRoot, starRoot.values[indCentral], aLeftC, 0);
    root.deRefWrite(fR.starRootAfterMerging);
    return fR;
  }
  template<typename TTT>
  TRF::TreeEraseReturn<TTT> BasicSet<TTT>::removeValueThatExists(HDPF::Pointer<HBF::Node<TTT> > root, const TTT& _value){
    TRF::TreeEraseReturn<TTT> fR;
    HBF::Node<TTT> starRoot=root.deRefRead();
    if(starRoot.height==1){
      TRF::TreeEraseReturn<TTT> remRes=removeFromComfortableLeaf(root,starRoot,_value);
      fR=remRes;
      if(fR.lastNodeRemovedAndTreeBecameEmpty==0){ fR.aP=root;}
      return fR;
    }
    long indS=VSF::lowerBoundSortedVector(starRoot.values,_value);
    if((indS>-1)&&(VSF::valuesEqual(starRoot.values[indS],_value))){
      TRF::TreeEraseReturn<TTT> remRes=removeFromIdentifiedInternalNode(root,starRoot,_value,indS);
      fR=remRes;
      if(fR.heightDecreased==0){
        fR.aP=root;
      }
      return fR;
    }
    long indexToFollow=indS+1;
    HDPF::Pointer<HBF::Node<TTT> > runner=(starRoot.children)[indexToFollow];
    HBF::Node<TTT> starRunner=runner.deRefRead();
    if(starRunner.num > HDPF::GLOBAL_PS.getTDegree()-1){
      TRF::TreeEraseReturn<TTT> remRes=removeValueThatExists(runner,_value);
      fR=remRes;
      fR.aP=root;
      if(fR.success==1){
        starRoot.counts[indexToFollow] -= 1;
        root.deRefWrite(starRoot);
      }
      return fR;
    }
    fR = getValueFromASibling(root, starRoot,  indexToFollow,runner,starRunner);
    if(fR.indexOfComfortableSibling!=-1){
      starRoot=fR.starRootAfterTrading;
      fR=removeValueThatExists(runner,_value);
      fR.aP=root;
      if(fR.success==1){
        starRoot.counts[indexToFollow] -= 1;
        root.deRefWrite(starRoot);
      }
      return fR;
    }
    fR = mergeChildWithSibling(root,starRoot, indexToFollow, runner, starRunner, fR.indexOfSiblingToMergeWith);
    starRoot=fR.starRootAfterMerging;
    indexToFollow=fR.indexToFollowAfterMerging;
    runner=(starRoot.children)[indexToFollow];
    HBF::Node<TTT> stR=runner.deRefRead();
    fR=removeValueThatExists(runner, _value);
    fR.aP=root;
    if(fR.success==1){
      root.deRefWrite(starRoot);
      if((starRoot.height==_height) && (starRoot.num==0)){
        HDPF::Pointer<HBF::Node<TTT> > _newRoot=starRoot.children[0];
        _dRoot.deAllocateMemory();
        _dRoot=_newRoot;
        --_height;
        fR.aP=_dRoot;
        fR.heightDecreased=1;
      }
    }
    return fR;
  }
  template<typename TTT>
  TRF::TreeEraseReturn<TTT> BasicSet<TTT>::removeFromTree(HDPF::Pointer<HBF::Node<TTT> > root, const TTT& _value){
    TRF::TreeEraseReturn<TTT> fR;
    IRFNF::IndRecFName<TTT> irf=findIndexRecordFileName(_value);
    if(irf.index==-1){
      return fR;
    }
    return removeValueThatExists(root,_value);
  }
  template<typename TTT> long  BasicSet<TTT>::erase(const TTT & _value){
      TRF::TreeEraseReturn<TTT> delRes=removeFromTree(_dRoot,_value);
      if(delRes.success==1){
        --_size;
        _dRoot=delRes.aP;
      }
      return delRes.success;
  }
  template<typename TTT> long  BasicSet<TTT>::operator-=(const TTT & _value){
    return erase(_value);
  }
  template<typename TTT> long  BasicSet<TTT>::erase(const BasicSet<TTT> & _s2){
    long l2=_s2.size();
    long fR=0;
    for(int i=0;i<l2;++i){
      fR+= erase(_s2[i].first);
    }
    return fR;
  }
  template<typename TTT> long  BasicSet<TTT>::operator-=(const BasicSet<TTT> & _s2){
    return erase(_s2);
  }
  template<typename TTT>  BasicSet<TTT>::BasicSet( BasicSet<TTT>&& _moveFrom){
   _size= _moveFrom._size;
   _height=_moveFrom._height;
   _moveFrom._size=0;
   _moveFrom._height=-3777177;
   startingFolderName=_moveFrom.startingFolderName;
   _dRoot=_moveFrom._dRoot;
     HDPF::Pointer<HBF::Node<TTT> > myNullptr;
   _moveFrom._dRoot=myNullptr;
  }
  template<typename TTT> void BasicSet<TTT>::operator=( BasicSet<TTT>&& _moveFrom){
   if(&_moveFrom!=this){
     _size= _moveFrom._size;
     _height=_moveFrom._height;
     _dRoot= _moveFrom._dRoot;
     startingFolderName=_moveFrom.startingFolderName;
     _moveFrom._size=0;
     _moveFrom._height=-3777178;
       HDPF::Pointer<HBF::Node<TTT> > myNullptr;
       _moveFrom._dRoot=myNullptr;
   }
  }
}
#endif
