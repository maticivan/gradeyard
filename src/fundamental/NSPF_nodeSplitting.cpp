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

#ifndef _INCL_NODESPLITTING_CPP
#define _INCL_NODESPLITTING_CPP

namespace NSPF{
  template<typename TTT>
  RNISF::ResultInsSplit<TTT> insertAndSplitIfNecessary(const HDPF::Pointer<HBF::Node<TTT> > & pointerToOldNode,
                                                               const HBF::Node<TTT> & oldNode,
                                                               const TTT& _value,
                                                               const HDPF::Pointer<HBF::Node<TTT> > & pointerNodeOfThe_value,
                                                               const long & _valuePosInNode,
                                                               const HDPF::Pointer<HBF::Node<TTT> > & aSplittingL,
                                                               const HDPF::Pointer<HBF::Node<TTT> > & aSplittingR,
                                                               const long & cL,
                                                               const long & cR){
    RNISF::ResultInsSplit<TTT> fR;
    long indexVal=VSF::lowerBoundSortedVector(oldNode.values,_value);

    if( (indexVal>-1) && (VSF::valuesEqual(oldNode.values[indexVal],_value))) {
      // no point in inserting - the value already exists
      fR.indicatorInsertOrSplitOrNothing=0;
      return fR;
    }



    HBF::Node<TTT> nodeNew;
    nodeNew.setNum(oldNode.num+1);
    nodeNew.height=oldNode.height;
    long np1=nodeNew.num+1;

    long copyLimit=indexVal+1;
    long i=0;
    while(i<copyLimit){
      (nodeNew.values)[i]=(oldNode.values)[i];
      (nodeNew.counts)[i]=(oldNode.counts)[i];
      (nodeNew.children)[i]=(oldNode.children)[i];
      ++i;
    }
    (nodeNew.values)[i]=_value;
    for(long j=oldNode.num-1;j>indexVal;--j){
      DMF::renameDataFile(pointerToOldNode,j,pointerToOldNode,j+1);
    }

    DMF::renameDataFile(pointerNodeOfThe_value,_valuePosInNode, pointerToOldNode,i);
    (nodeNew.counts)[i]=cL;
    (nodeNew.children)[i]=aSplittingL;
    (nodeNew.counts)[i+1]=cR;
    (nodeNew.children)[i+1]=aSplittingR;
    ++i;
    while(i<nodeNew.num){
      (nodeNew.values)[i]=(oldNode.values)[i-1];
      (nodeNew.counts)[i+1]=(oldNode.counts)[i];
      (nodeNew.children)[i+1]=(oldNode.children)[i];
      ++i;
    }

    if(nodeNew.num<HDPF::GLOBAL_PS.getTDegree()*2){
      // The node is not full
      fR.indicatorInsertOrSplitOrNothing=1;
      fR.resultOfInsert=std::move(nodeNew);
      return fR;
    }
    // The node is full. The splitting is necessary.

    fR.indicatorInsertOrSplitOrNothing=2;
    ((fR.resultOfSplit).first).setNum(HDPF::GLOBAL_PS.getTDegree());
    ((fR.resultOfSplit).second).setNum(HDPF::GLOBAL_PS.getTDegree() - 1);
    ((fR.resultOfSplit).first).height=nodeNew.height; ((fR.resultOfSplit).second).height=nodeNew.height;
    i=0;
    (fR.countsSplit).first=HDPF::GLOBAL_PS.getTDegree();
    (fR.countsSplit).second=HDPF::GLOBAL_PS.getTDegree() - 1;
    while(i<HDPF::GLOBAL_PS.getTDegree()){
      ((fR.resultOfSplit).first).values[i]=nodeNew.values[i];
      ((fR.resultOfSplit).first).counts[i]=nodeNew.counts[i];
      ((fR.resultOfSplit).first).children[i]=nodeNew.children[i];
      (fR.countsSplit).first += nodeNew.counts[i];
      ++i;
    }
    ((fR.resultOfSplit).first).counts[i]=nodeNew.counts[i];
    ((fR.resultOfSplit).first).children[i]=nodeNew.children[i];
    (fR.countsSplit).first += nodeNew.counts[i];

    fR.centralValue=nodeNew.values[i];
    fR.pointerToOldLocationOfTheCentralValue=pointerToOldNode;
    fR.positionInNodeOfTheCentralValue=i;
    ++i;
    long shiftValue=i;
    while(i<nodeNew.num){
      ((fR.resultOfSplit).second).values[i-shiftValue]=nodeNew.values[i];
      ((fR.resultOfSplit).second).children[i-shiftValue]=nodeNew.children[i];
      ((fR.resultOfSplit).second).counts[i-shiftValue]=nodeNew.counts[i];
      (fR.countsSplit).second += nodeNew.counts[i];
      ++i;
    }
    ((fR.resultOfSplit).second).counts[i-shiftValue]=nodeNew.counts[i];
    ((fR.resultOfSplit).second).children[i-shiftValue]=nodeNew.children[i];
    (fR.countsSplit).second += nodeNew.counts[i];
    return fR;

  }

  template<typename TTT>
  RNISF::ResultInsSplit<TTT> insertAndSplitIfNecessary(const HDPF::Pointer<HBF::Node<TTT> > & pointerToOldNode, const HBF::Node<TTT> & oldNode, const TTT& _value){

    HDPF::Pointer<HBF::Node<TTT> > nullPointer;
    return insertAndSplitIfNecessary(pointerToOldNode, oldNode,_value,nullPointer,-1,nullPointer,nullPointer,0,0);
  }

}

#endif
