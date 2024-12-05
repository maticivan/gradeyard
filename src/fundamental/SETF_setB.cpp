//    GradeYard learning management system
//
//    Copyright (C) 2024 Ivan Matic, https://gradeyard.com
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


#ifndef _INCL_SetB_CPP
#define _INCL_SetB_CPP

#include "SETBF_setB.cpp"
#include "HDDBRF_HdDBRecord.cpp" 

namespace SETF{
  template<typename TTT> struct MapSavedSearch{
  public:
    std::map<std::string,std::map<TTT,IRFNF::IndRecFName<TTT> > > m;
    std::map<std::string,std::map<std::string,HBF::Node<TTT> > > deRefFast;
    void clearM(const std::string &);
    void clearDeRefFast(const std::string &);
    void clear(const std::string &);
    void modifyKey(const std::string &, const TTT& , const TTT &, const IRFNF::IndRecFName<TTT> &, const HBF::Node<TTT> &);
  };
  template<typename TTT> void MapSavedSearch<TTT>::clearM(const std::string & s){
    std::map<TTT,IRFNF::IndRecFName<TTT> > emptyMapForM;
    m[s]=emptyMapForM;
  }
  template<typename TTT> void MapSavedSearch<TTT>::clearDeRefFast(const std::string & s){
    std::map<std::string,HBF::Node<TTT> > emptyMapForDeRefFast;
    deRefFast[s]=emptyMapForDeRefFast;
  }
  template<typename TTT> void MapSavedSearch<TTT>::clear(const std::string & s){
    clearM(s);clearDeRefFast(s);
  }
  template<typename TTT> void MapSavedSearch<TTT>::modifyKey(const std::string & s, const TTT & _oK, const TTT & _nK, const IRFNF::IndRecFName<TTT> & irfn, const HBF::Node<TTT> & _cNode){
    typename std::map<std::string,std::map<TTT,IRFNF::IndRecFName<TTT> > >::iterator it;
    it=m.find(s);
    if(it!=m.end()){
      (it->second).erase(_oK);
      (it->second)[_nK]=irfn;
    }
    typename std::map<std::string,std::map<std::string,HBF::Node<TTT> > >::iterator itD;
    itD=deRefFast.find(s);
    if(itD!=deRefFast.end()){
      (itD->second)[irfn.aRecord.putIntoString()]=_cNode;
    }
  }
  template<typename TTT>
  void updateSavedSearches(MapSavedSearch<TTT>& sS,
                           const std::string & tableId,
                           const std::vector<IRFNF::IndRecFName<TTT> > & v){
    long sz=v.size();
    typename std::map<TTT,IRFNF::IndRecFName<TTT> > emptyMap;
    typename std::map<std::string,std::map<TTT,IRFNF::IndRecFName<TTT> > >::iterator it,itE;
    itE=sS.m.end();
    it=sS.m.find(tableId);
    if(it==itE){
      sS.m[tableId]=emptyMap;
      itE=sS.m.end();
      it=sS.m.find(tableId);
    }
    for(long i=0;i<sz;++i){
      (it->second)[v[i].record]=v[i];
    }
  }
  template<typename TTT> class Set : public SETBF::BasicSet<TTT>{
  private:
    void operator=(const Set &);
  public:
    Set():SETBF::BasicSet<TTT>(){}
    Set(const Set & _s):SETBF::BasicSet<TTT>(_s){}
    Set(Set && _s):SETBF::BasicSet<TTT>(std::move(_s)){}
    void operator=(Set &&);
    IRFNF::IndRecFName<TTT> findIndexRecordFileNameQuick(MapSavedSearch<TTT>& ,
                                                        const std::string & ,
                                                        const TTT & ) const;
    long inPlaceModificationQuick(MapSavedSearch<TTT>&, const std::string & ,const TTT&, const TTT&, const long & = 1);
  };
  template<typename TTT> void Set<TTT>::operator=(Set<TTT>&& _moveFrom){
    SETBF::BasicSet<TTT>::operator=(std::move(_moveFrom));
  }
  template<typename TTT>
  IRFNF::IndRecFName<TTT> Set<TTT>::findIndexRecordFileNameQuick(MapSavedSearch<TTT>& sS,
                                                                const std::string & tableId,
                                                                const TTT & _v) const{
    HDPF::Pointer<HBF::Node<TTT> > researcher= SETBF::BasicSet<TTT>::_dRoot;
    HBF::Node<TTT> starRes;
    IRFNF::IndRecFName<TTT> found,tempJ;
    found.fileName="notFound";
    found.index=-1;
    long indexLowerBound;
    long ilbp1;
    long discardedLeft=0,discLeftTemp;
    typename std::map<std::string,std::map<TTT,IRFNF::IndRecFName<TTT> > >::iterator iM,iME;
    typename std::map<std::string,std::map<std::string,HBF::Node<TTT> > >::iterator iD,iDE;
    typename std::map<std::string,HBF::Node<TTT> >::iterator iDR,iDRE;
    iME=sS.m.end();
    iM=sS.m.find(tableId);
    if(iM==iME){
      sS.clearM(tableId);
      iME=sS.m.end();
      iM=sS.m.find(tableId);
    }
    iDE=sS.deRefFast.end();
    iD=sS.deRefFast.find(tableId);
    if(iD==iDE){
      sS.clearDeRefFast(tableId);
      iDE=sS.deRefFast.end();
      iD=sS.deRefFast.find(tableId);
    }
    iDRE=(iD->second).end();
    std::string resStr;
    while( (!researcher.isNullptr())&&(found.index==-1)){
      resStr=researcher.putIntoString();
      iDR=(iD->second).find(resStr);
      if(iDR==iDRE){
        starRes=researcher.deRefRead();
        (iD->second)[resStr]=starRes;
        iDRE=(iD->second).end();
        discLeftTemp=discardedLeft;
        tempJ.fileName=researcher.getNameOfFile();
        tempJ.aRecord=researcher;
        for(long j=0;j<starRes.num;++j){
          discLeftTemp += (starRes.counts[j]+1);
          tempJ.index=discLeftTemp-1;
          tempJ.record=starRes.values[j];
          tempJ.positionInNode=j;
          (iM->second)[tempJ.record]=tempJ;
        }
      }
      else{
        starRes=iDR->second;
      }
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
  long Set<TTT>::inPlaceModificationQuick(MapSavedSearch<TTT>& sS,
                                                                const std::string & tableId,
                                                                const TTT & _oldVal,
                                                                const TTT & _newVal,
                                                                const long & _safeMode){
    IRFNF::IndRecFName<TTT> irfnOld = findIndexRecordFileNameQuick(sS,tableId,_oldVal);
    if(irfnOld.index<0){
      return 0;
    }
    long prevIndex=irfnOld.index-1;
    long nextIndex=irfnOld.index+1;
    if(_safeMode==1){
      if(prevIndex > -1){
        if(!(SETBF::BasicSet<TTT>::b_randomAccess(SETBF::BasicSet<TTT>::_dRoot,prevIndex).first < _newVal)){
          return 0;
        }
      }
      if(nextIndex < SETBF::BasicSet<TTT>::_size){
        if(!(_newVal < SETBF::BasicSet<TTT>::b_randomAccess(SETBF::BasicSet<TTT>::_dRoot,nextIndex).first)){
          return 0;
        }
      }
    } 
    HBF::Node<TTT> currentNode = irfnOld.aRecord.deRefRead();
    currentNode.values[irfnOld.positionInNode]=_newVal;
    irfnOld.aRecord.deRefWrite(currentNode);
    sS.modifyKey(tableId,_oldVal,_newVal,irfnOld,currentNode);
    return 1;
  }

}
#endif
