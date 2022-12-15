//    GradeYard learning management system
//
//    Copyright (C) 2022 Ivan Matic, https://gradeyard.com
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

#ifndef _INCL_MAINTEXTMANAGEMENT_CPP
#define _INCL_MAINTEXTMANAGEMENT_CPP

namespace TMD{
  class MText:public ATMD::Text{
  private:
    std::string ow2_dbs_key0="id";
    std::string ow2_dbs_key1="name";
    std::string ow2_dbsMFInd="mainText";
    std::string ow2_cntrName="textsCntr";
    long ow2_primeIndex0=9;
    long ow2_primeIndex1=20;
    long ow2_permutInd=8;
    long ow2_shift=76442;
  public:
    MText(const std::string & ="!*!", const std::string & ="!*!");
    void setVariables(const std::string & ="!*!", const std::string & ="!*!");
  };
  MText::MText(const std::string & _tn, const std::string &  _intTId){
    setVariables(_tn,_intTId);
  }
  void MText::setVariables(const std::string & _tn, const std::string &  _intTId){
    dbs_key0= ow2_dbs_key0;
    dbs_key1= ow2_dbs_key1;
    dbsMFInd = ow2_dbsMFInd;
    cntrName= ow2_cntrName;
    primeIndex0= ow2_primeIndex0;
    primeIndex1= ow2_primeIndex1;
    permutInd= ow2_permutInd;
    shift= ow2_shift;
    primeIndex0= setPrimesCorrectly(f0,primeIndex0, BF::GLOBAL_NUM_PRIME_SEQUENCES);
    primeIndex1= setPrimesCorrectly(f1,primeIndex1, BF::GLOBAL_NUM_PRIME_SEQUENCES);
    permutInd= setPermsCombsCorrectly(permutInd,BF::GLOBAL_NUM_PERMUTATION_SEQUENCES);
    textName=_tn;
    internalVectorNumber=getInternalNumberFromInternalId(_intTId);
    existenceEstablishedBefore=0;
  }
  std::string rawTextData(const std::string& textName){
    MText sf;
    int sc=sf.setFromTextName(textName);
    if(sc==1){
      return sf.getTextData();
    }
    return "";
  }
}

#endif
