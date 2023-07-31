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


#ifndef _INCL_ABSTRACTTABLEMANAGEMENT_CPP
#define _INCL_ABSTRACTTABLEMANAGEMENT_CPP

namespace AMD{
  class AbstractManager{
  protected:
    std::vector<long> internalVectorNumber;
    std::string textData;
    long existenceEstablishedBefore;
    std::string dbs_key0="fileId";
    std::string dbs_key1="userId";
    std::string dbsMFInd="fileMan";
    std::string cntrName="filesCntr";
    long primeIndex0=16;
    long primeIndex1=37;
    long permutInd=7;
    long shift=76442;
    BF::BijectiveFunction f0;
    BF::BijectiveFunction f1;
    std::vector<std::vector<long> > permuts01;
    std::vector<long> indCombs;
    long lenEach;
    long setPrimesCorrectly(BF::BijectiveFunction & , const long &, const long &);
    long setPermsCombsCorrectly(const long &, const long &);
  public:
    AbstractManager(const std::string & ="!*!", const std::string & ="!*!");
    void setTextData(const std::string &);
    virtual std::string getKey1() const =0;
    virtual void setKey1(const std::string &) =0;
    std::vector<long> getInternalNumberFromExternalCode(const std::string & = "!*!") const;// input: external code
    std::vector<long> getInternalNumberFromInternalId(const std::string & = "!*!") const;// input: internal id
    std::string getTextData() const;
    std::string getInternalIdFromInternalNumber(const std::vector<long> &) const;
    std::string getInternalIdFromInternalNumber() const;
    std::string getExternalCodeFromInternalNumber(const std::vector<long> &) const;
    std::string getExternalCodeFromInternalNumber() const;
    std::string getExternalCodeFromInternalId(const std::string &) const;// input: string with internal id
    std::string getExternalCodeFromInternalId( ) const;
    long checkExistenceInDatabase() const;
    int setFromInternalId(const std::string &);
    int setFromExternalCode(const std::string &);
    int deleteRecord(const long & = 1);
    // argument: 0 - put only into queue
    //           1 - delete from database
    int putInDB(const long & = 1);
    // argument: 0 - put only into queue
    //           1 - put in database
  };
  int AbstractManager::setFromInternalId(const std::string & _iId){
    std::vector<std::string> k,v;
    k.resize(1);v.resize(1);
    k[0]=dbs_key0;
    v[0]=_iId;
    std::vector< std::pair<std::vector<std::string>, std::string> > searchRes;
    searchRes=DD::GL_MAIN_DB.dbsM[dbsMFInd].search(v,k);
    long sz=searchRes.size();
    if(sz==0){
      return 0;
    }
    internalVectorNumber=getInternalNumberFromInternalId(_iId);
    setKey1(searchRes[0].first[1]);
    textData=searchRes[0].second;
    existenceEstablishedBefore=1;
    return 1;
  }
  int AbstractManager::setFromExternalCode(const std::string & _c){
    std::vector<long> _v=getInternalNumberFromExternalCode(_c);
    std::string _i=getInternalIdFromInternalNumber(_v);
    return setFromInternalId(_i);
  }
  long AbstractManager::setPrimesCorrectly(BF::BijectiveFunction & f, const long & x, const long &max){
    long ind=0;
    if((x>0)&&(max>0)){
      ind=x%max;
    }
    f.setParameters(BF::GLOBAL_PRIME_SEQUENCES.primes[ind],BF::GLOBAL_PRIME_SEQUENCES.mults[ind],BF::GLOBAL_PRIME_SEQUENCES.shifts[ind],BF::GLOBAL_PRIME_SEQUENCES.inverses[ind]);
    return ind;
  }
  long AbstractManager::setPermsCombsCorrectly(const long & x, const long &max){
    long ind=0;
    if((x>0)&&(max>0)){
      ind=x%max;
    }
    permuts01=BF::GLOBAL_PERMUTATION_SEQUENCES.pairsPermutations[ind];
    indCombs=BF::GLOBAL_PERMUTATION_SEQUENCES.indicatorCombinations[ind];
    lenEach=permuts01[0].size();
    return ind;
  }
  AbstractManager::AbstractManager(const std::string & _intUID, const std::string &  _intFId){}
  void AbstractManager::setTextData(const std::string & _o){ textData=_o; }
  std::vector<long> AbstractManager::getInternalNumberFromExternalCode(const std::string & _s) const{
    if(_s=="!*!"){
      return internalVectorNumber;
    }
    return BF::plainSeqFromString(_s,indCombs,permuts01,f0,f1,shift,0);
  }
  std::vector<long> AbstractManager::getInternalNumberFromInternalId(const std::string & _s) const{
    if(_s=="!*!"){
      return internalVectorNumber;
    }
    return BF::plainSeqFromString(_s,indCombs,permuts01,f0,f1,shift,1);
  }
  std::string AbstractManager::getTextData() const{return textData;}
  std::string AbstractManager::getExternalCodeFromInternalNumber(const std::vector<long> & _v) const{
    return BF::getCodeWord(_v, indCombs, permuts01,  f0,f1,  shift, 0);
  }
  std::string AbstractManager::getExternalCodeFromInternalNumber() const{
    return getExternalCodeFromInternalNumber(internalVectorNumber);
  }
  std::string AbstractManager::getExternalCodeFromInternalId(const std::string & _c) const{
    std::vector<long> _v=getInternalNumberFromInternalId(_c);
    return getExternalCodeFromInternalNumber(_v);
  }
  std::string AbstractManager::getExternalCodeFromInternalId() const{
    return getExternalCodeFromInternalNumber();
  }
  std::string AbstractManager::getInternalIdFromInternalNumber(const std::vector<long> & _v) const{
    return BF::getCodeWord(_v, indCombs, permuts01,  f0,f1,  shift, 1);
  }
  std::string AbstractManager::getInternalIdFromInternalNumber() const{
    return getInternalIdFromInternalNumber(internalVectorNumber);
  }
  long AbstractManager::checkExistenceInDatabase() const{
    // checks whether (internalUserId,internalVectorNumber) exists in a database
    // returns:
    // 0 - good combination. This is not in database and can be entered as a new entry
    // 1 - good combination. This is in the database and corresponds to a correct entry.
    //                       Can be updated
    if(existenceEstablishedBefore==1){
      return 1;
    }
    std::vector<std::string> k,v;
    k.resize(2);v.resize(2);
    k[0]=dbs_key0;
    k[1]=dbs_key1;
    v[0]=getInternalIdFromInternalNumber();
    v[1]=getKey1();
    std::vector< std::pair<std::vector<std::string>, std::string> > searchRes;
    searchRes=DD::GL_MAIN_DB.dbsM[dbsMFInd].search(v,k);
    long sz=searchRes.size();
    if(sz==0){
      return 0;
    }
    const long*  addressOfEEB;
    addressOfEEB=&existenceEstablishedBefore;
    size_t wbc=(size_t)(addressOfEEB);
    long* nonConstPointerAEEB;
    nonConstPointerAEEB = (long *)(wbc);
    *nonConstPointerAEEB=1;
    return 1;
  }
  int AbstractManager::deleteRecord(const long & _qOrDB){
    if(checkExistenceInDatabase()!=1){
      return 0;
    }
    std::vector<std::string> kV;
    kV.resize(2);
    kV[0]=getInternalIdFromInternalNumber();kV[1]=getKey1();
    if(_qOrDB==0){
      (DD::GL_MAIN_DB.dbsM[dbsMFInd]).delRowQ(kV);
    }
    else{
      (DD::GL_MAIN_DB.dbsM[dbsMFInd]).delRow(kV);
    }
    existenceEstablishedBefore=0;
    return 1;
  }
  int AbstractManager::putInDB(const long & _qOrDB){
    if(getKey1()=="!*!"){
      return 0;
    }
    if(checkExistenceInDatabase()>1){
      return 0;
    }
    std::vector<std::string> kV;
    kV.resize(2);
    kV[0]=getInternalIdFromInternalNumber();kV[1]=getKey1();
    if(_qOrDB==0){
      DD::GL_MAIN_DB.dbsM[dbsMFInd].insertQ(kV,textData);
    }
    else{
      DD::GL_MAIN_DB.dbsM[dbsMFInd].insert(kV,textData);
    }
    existenceEstablishedBefore=1;
    return 1;
  }
}
#endif
