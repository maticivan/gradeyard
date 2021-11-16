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

#ifndef _INCL_TreeFunctionReturns_CPP
#define _INCL_TreeFunctionReturns_CPP

namespace TRF{
  template<typename TTT> struct TreeEraseReturn{
  public:
    HDPF::Pointer<HBF::Node<TTT> > aP;
    int success;
    int lastNodeRemovedAndTreeBecameEmpty;

    long indexOfComfortableSibling;
    long indexOfSiblingToMergeWith;
    HBF::Node<TTT> starRootAfterTrading;
    HBF::Node<TTT> starRootAfterMerging;
    long indexToFollowAfterMerging;
    int heightDecreased;
    TreeEraseReturn();
  };
  template<typename TTT> TreeEraseReturn<TTT>::TreeEraseReturn(){
    success=0;
    lastNodeRemovedAndTreeBecameEmpty=0;
    indexOfComfortableSibling=-1;
    indexOfSiblingToMergeWith=-1;
    heightDecreased=0;
  }


  template<typename TTT> struct TreeInsertReturn{
  public:
    HDPF::Pointer<HBF::Node<TTT> > aP;
    int success;
    int nodeSplitIndicator;

    TTT centralValue;
    HDPF::Pointer<HBF::Node<TTT> > pointerToOldLocationOfTheCentralValue;
    long positionInNodeOfTheCentralValue;
    HDPF::Pointer<HBF::Node<TTT> > aLeft;
    HDPF::Pointer<HBF::Node<TTT> > aRight;
    long cLeft;
    long cRight;
    TreeInsertReturn();
  };
  template<typename TTT> TreeInsertReturn<TTT>::TreeInsertReturn(){
    success=0;
    nodeSplitIndicator=0;
    cLeft=0;
    cRight=0;
  }

}

#endif
