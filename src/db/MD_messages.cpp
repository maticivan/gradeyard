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


#ifndef _INCL_MESSAGES_CPP
#define _INCL_MESSAGES_CPP

namespace MD{
  class Message:public AMD::AbstractManager{
  private:
    std::string internalUserId;
    std::string ow_dbs_key0="messId";
    std::string ow_dbs_key1="userId";
    std::string ow_dbsMFInd="messMan";
    std::string ow_cntrName="messCntr";
    long ow_primeIndex0=21;
    long ow_primeIndex1=34;
    long ow_permutInd=6;
    long ow_shift=76442;
  public:
    Message(const std::string & ="!*!", const std::string & ="!*!");
    void setVariables(const std::string & ="!*!", const std::string & ="!*!");
    std::string getInternalUserId() const;
    std::string getKey1() const;
    void setKey1(const std::string &);
    int createMessage(const std::string &);
    int modifyCollection(const std::string &, const std::string &, std::string &,std::string &) const;
    int addToCollection(const std::string &, std::string &, std::string &) const;
    int removeFromCollection(const std::string &, std::string &, std::string &) const;
  };
  Message::Message(const std::string & _intUID, const std::string &  _intFId){
    setVariables(_intUID,_intFId);
  }
  void Message::setVariables(const std::string & _intUID, const std::string &  _intFId){
    dbs_key0= ow_dbs_key0;
    dbs_key1= ow_dbs_key1;
    dbsMFInd = ow_dbsMFInd;
    cntrName= ow_cntrName;
    primeIndex0= ow_primeIndex0;
    primeIndex1= ow_primeIndex1;
    permutInd= ow_permutInd;
    shift= ow_shift;
    primeIndex0= setPrimesCorrectly(f0,primeIndex0, BF::GLOBAL_NUM_PRIME_SEQUENCES);
    primeIndex1= setPrimesCorrectly(f1,primeIndex1, BF::GLOBAL_NUM_PRIME_SEQUENCES);
    permutInd= setPermsCombsCorrectly(permutInd,BF::GLOBAL_NUM_PERMUTATION_SEQUENCES);
    internalUserId=_intUID;
    internalVectorNumber=getInternalNumberFromInternalId(_intFId);
    existenceEstablishedBefore=0;
  }
  std::string Message::getInternalUserId() const{
    return internalUserId;
  }
  std::string Message::getKey1() const{
    return internalUserId;
  }
  void Message::setKey1(const std::string & _k){
    internalUserId=_k;
  }
  int Message::createMessage(const std::string & _iId){
    WUD::User w;
    int sc=w.setFromInternalId(_iId);
    if(sc==0){
      return 0;
    }
    long messIdIsFree=0;
    std::string messId;
    std::vector<std::string> k,v;
    k.resize(1);v.resize(1);
    k[0]=dbs_key0;
    while(messIdIsFree==0){
      CD::Counter mId(cntrName);
      mId.increase();
      messId=mId.getCodeWord(1);
      v[0]=messId;
      std::vector< std::pair<std::vector<std::string>, std::string> > searchRes;
      searchRes=DD::GL_MAIN_DB.dbsM[dbsMFInd].search(v,k);
      long sz=searchRes.size();
      if(sz==0){
        messIdIsFree=1;
      }
    }
    internalUserId=_iId;
    internalVectorNumber=getInternalNumberFromInternalId(messId);
    putInDB();
    existenceEstablishedBefore=1;
    return 1;
  }
  int Message::modifyCollection(const std::string & _collId, const std::string & action, std::string & navSt, std::string & tTitle) const{
    Message bm;
    int messageExists=bm.setFromExternalCode(_collId);
    if(messageExists==0){
      return 0;
    }
    std::string othText=bm.textData;
    std::string elementsSeqSt="";
    long pos;
    std::pair<std::string,int> allD,allD1;
    pos=0;allD=SF::extract(othText,pos,"_els!*_","_/els!*_");
    if(allD.second==1){
      elementsSeqSt=allD.first;
    }
    else{
      if(action=="remove"){
        return 0;
      }
      else{
        othText=SF::findAndReplace(othText,"_/messText!*_","_els!*_ _/els!*__/messText!*_");
      }
    }
    std::vector<std::string> seqOld= SF::stringToVector(elementsSeqSt,"_n_","_/n_");
    long sz=seqOld.size();
    std::vector<std::string> seqNew;
    std::string myCode=getExternalCodeFromInternalId();
    seqNew=SF::removeFromSeq(seqOld,myCode);
    if(seqNew.size()==seqOld.size()){
      if(action=="remove"){//The element was not in the sequence: nothing to remove
        return 0;
      }
      long sz=seqOld.size();
      seqNew.resize(sz+1);
      for(long i=0;i<sz;++i){
        seqNew[i]=seqOld[i];
      }
      seqNew[sz]=myCode;
    }
    else{
      if(action=="add"){// The element was in the sequence: nothing to add
        return 0;
      }
    }
    std::string nst="_els!*_";
    nst+=SF::vectorToString(seqNew,"_n_","_/n_");
    nst+="_/els!*_";
    pos=0;allD1=SF::extractAndReplace(othText,pos,"_els!*_","_/els!*_",0,nst);
    if(allD1.second==0){
      return 0;
    }
    bm.textData=allD1.first;
    navSt="";
    pos=0;allD1=SF::extract(bm.textData,pos,"_navigation_","_/navigation_");
    if(allD1.second==1){
      navSt=allD1.first;
    }
    tTitle="";
    pos=0;allD1=SF::extract(bm.textData,pos,"_title!*_","_/title!*_");
    if(allD1.second==1){
      tTitle=allD1.first;
    }
    return bm.putInDB();
  }
  int Message::addToCollection(const std::string & _collId, std::string & navSt, std::string & tTitle) const{
    return modifyCollection(_collId,"add",navSt,tTitle);
  }
  int Message::removeFromCollection(const std::string & _collId, std::string & navSt, std::string & tTitle) const{
    return modifyCollection(_collId,"remove",navSt,tTitle);
  }
  std::vector<Message> getMessagesByUser(const std::string & _iId, const long & start =0, const long & end=-1){
    Message f;
    return FMD::itemsByUser(f,"messMan",_iId,start,end);
  }
}
#endif
