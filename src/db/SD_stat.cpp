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


#ifndef _INCL_STATMANAGEMENT_CPP
#define _INCL_STATMANAGEMENT_CPP

namespace SD{

  class Stat:public ATMD::Text{
  private:


    std::string ow2_dbs_key0="id";
    std::string ow2_dbs_key1="name";
    std::string ow2_dbsMFInd="stat";
    std::string ow2_cntrName="statCntr";


    long ow2_primeIndex0=14;
    long ow2_primeIndex1=5;
    long ow2_permutInd=8;
    long ow2_shift=76442;


  public:
    Stat(const std::string & ="!*!", const std::string & ="!*!");

    void setVariables(const std::string & ="!*!", const std::string & ="!*!");


  };
  Stat::Stat(const std::string & _tn, const std::string &  _intTId){
    setVariables(_tn,_intTId);
  }
  void Stat::setVariables(const std::string & _tn, const std::string &  _intTId){

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

}


#endif
