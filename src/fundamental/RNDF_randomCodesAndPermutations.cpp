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


#ifndef _INCL_RCPF_CPP
#define _INCL_RCPF_CPP

namespace RNDF{
  long  randNum(const long & uniformLimit=1000000){
  std::uniform_int_distribution<long> uInt(0,uniformLimit-1);
    return uInt(GF::GL_mt_randIF);
  }
  std::string genRandCode(const long & ln){
    std::string rc;
    for(long i=0;i<ln;++i){
      rc += std::to_string(randNum(10));
    }
    return rc;
  }

  std::vector<long> genRandPermutation(const long & l){
  // returns a random permutation of {0,1,...,l-1}
    std::vector<long> fR;
    if(l<1){
      return fR;
    }
    std::vector<long> v;
    fR.resize(l);
    v.resize(l);
    long rN;
    for(long i=0;i<l;++i){
      v[i]=i;
    }
    long i=l;long j=0;
    while(i>0){

      rN=randNum(i);
      --i;
      fR[j]=v[rN];
      if(rN!=i){
        v[rN]=v[i];
      }
      ++j;
    }

    return fR;
  }

  std::vector<long> genRandPermutation(const long & min, const long & max){
  // returns a random permutation of {min,min+1,...,max}

    if(max<min){
      return genRandPermutation(max);
    }
    std::vector<long> fR=genRandPermutation(max+1-min);
    long sz=fR.size();
    for(long i=0;i<sz;++i){
      fR[i]+=min;
    }

    return fR;
  }

  std::vector<long> genRandCombination(const long & numOnes,const long & numZeros){
    //returns a random ordering of numZeros zeros and numOnes ones.
    std::vector<long> fR,fRR;
    long l=numZeros+numOnes;
    fR.resize(l);fRR.resize(l);
    for(long i=0;i<l;++i){
      fR[i]=0;
      if(i<numOnes){
        fR[i]=1;
      }
    }
    std::vector<long> pm=genRandPermutation(l);
    for(long i=0;i<l;++i){
      fRR[i]=fR[pm[i]];
    }

    return fRR;
  }
}




#endif
