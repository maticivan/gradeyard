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

#ifndef _INCL_DERANGEMENTS_CPP
#define _INCL_DERANGEMENTS_CPP

namespace DERF{
  std::vector<double> derangementsSequenceFractions(const std::vector<double> & D){
    long n=D.size();
    std::vector<double> fR;
    fR.resize(n);
    fR[0]=-1.0;fR[1]=-1.0;
    for(long i=2;i<n;++i){
      if(D[i-1]<0.000000001){
        fR[i]=-1.0;
      }
      else{
        fR[i]=D[i]/D[i-1];
      }
    }
    return fR;
  }

  template<typename T>
  std::vector<T> gl_derangementsSequence(const long &_n, const T & zero, const T & one){
    long n=_n;
    if(n<5){n=5;}
    std::vector<T> fR;
    fR.resize(n);
    fR[0]=zero;
    fR[1]=zero;
    fR[2]=one;
    for(long i=3;i<n;++i){
      fR[i]=(i-1)*(fR[i-1]+fR[i-2]);
    }
    return fR;
  }
  std::vector<double> GL_DER_SEQ_FRACS= derangementsSequenceFractions(gl_derangementsSequence(15,static_cast<double>(0),static_cast<double>(1)));

  double P_that_last_is_in_transposition(const double & n){
    if(n<2){return 0;}
    if(n==2){return 1;}
    if(n>13){return 1.0/(static_cast<double>(n-1));}

    return 1.0/GL_DER_SEQ_FRACS[n-1];
  }



  int bernoulliP(const double & _p){
    long scale=1;
    double p=_p;
    while(p<10000.0){
      p += p;
      scale += scale;
    }
    long sim=RNDF::randNum(scale);
    if(sim<static_cast<double>(p)){
      return 1;
    }
    return 0;
  }
  std::vector<long> genRandDerangement(const long &_n){
    std::vector<long> fR;
    if(_n<1){return fR;}
    if(_n==1){fR.resize(1);fR[0]=0; return fR;}
    if(_n==2){fR.resize(2);fR[0]=1;fR[1]=0; return fR;}
    if(_n==3){
      long l=RNDF::randNum(2);
      fR.resize(3);
      fR[0]=1;fR[1]=2;fR[2]=0;
      if(l==0){
        fR[0]=2;fR[1]=0;fR[2]=1;
      }
      return fR;
    }
    fR.resize(_n);
    long nm1=_n-1;
    long nm2=_n-2;
    long l=RNDF::randNum(nm1);
    fR[nm1]=l;
    if(bernoulliP(P_that_last_is_in_transposition(_n))==1){
      fR[l]=nm1;
      std::vector<long> derSm=genRandDerangement(nm2);
      long realI,realVal;
      for(long i=0;i<nm2;++i){
        realI=i;if(realI>=l){++realI;}
        realVal=derSm[i];if(realVal>=l){++realVal;}
        fR[realI]=realVal;
      }
    }
    else{
      std::vector<long> derSm=genRandDerangement(nm1);
      for(long i=0;i<nm1;++i){
        fR[i]=derSm[i];
        if(fR[i]==l){
          fR[i]=nm1;
        }
      }
    }
    return fR;
  }
  int checkIfDerangement(const std::vector<long> &v){
    int der=1;
    long sz=v.size();long i=0;
    while((der==1)&&(i<sz)){
      if(v[i]==i){
        der=0;
      }
      ++i;
    }
    return der;
  }
}


#endif
