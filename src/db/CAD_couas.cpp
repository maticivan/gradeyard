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


#ifndef _INCL_COUAS_CPP
#define _INCL_COUAS_CPP

namespace CAD{

  class CouAs:public AMD::AbstractManager{ // Course or Assignment
  private:
    std::string internalUserId;
    std::string ow_dbs_key0="couasId";
    std::string ow_dbs_key1="userId";
    std::string ow_dbsMFInd="couasMan";
    std::string ow_cntrName="couasCntr";
    long ow_primeIndex0=22;
    long ow_primeIndex1=25;
    long ow_permutInd=7;
    long ow_shift=76442;
  public:
    CouAs(const std::string & ="!*!", const std::string & ="!*!");
    void setVariables(const std::string & ="!*!", const std::string & ="!*!");
    std::string getInternalUserId() const;
    std::string getKey1() const;
    void setKey1(const std::string &);
    int createCouAs(const std::string &);
  };
  CouAs::CouAs(const std::string & _intUID, const std::string &  _intFId){
    setVariables(_intUID,_intFId);
  }
  void CouAs::setVariables(const std::string & _intUID, const std::string &  _intFId){
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
  std::string CouAs::getInternalUserId() const{
    return internalUserId;
  }
  std::string CouAs::getKey1() const{
    return internalUserId;
  }
  void CouAs::setKey1(const std::string & _k){
    internalUserId=_k;
  }
  int CouAs::createCouAs(const std::string & _iId){
    WUD::User w;
    int sc=w.setFromInternalId(_iId);
    if(sc==0){
      return 0;
    }
    long couasIdIsFree=0;
    std::string couasId;
    std::vector<std::string> k,v;
    k.resize(1);v.resize(1);
    k[0]=dbs_key0;
    while(couasIdIsFree==0){
      CD::Counter mId(cntrName);
      mId.increase();
      couasId=mId.getCodeWord(1);
      v[0]=couasId;
      std::vector< std::pair<std::vector<std::string>, std::string> > searchRes;
      searchRes=DD::GL_MAIN_DB.dbsM[dbsMFInd].search(v,k);
      long sz=searchRes.size();
      if(sz==0){
        couasIdIsFree=1;
      }
    }
    internalUserId=_iId;
    internalVectorNumber=getInternalNumberFromInternalId(couasId);
    putInDB();
    existenceEstablishedBefore=1;
    return 1;
  }
  std::vector<CouAs> getCoursesAssignmentsByUser(const std::string & _iId, const long & start =0, const long & end=-1){
    CouAs f;
    return FMD::itemsByUser(f,"couasMan",_iId,start,end);
  }
}
#endif
