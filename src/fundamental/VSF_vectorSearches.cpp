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


#ifndef _INCL_VECTORSEARCHES_CPP
#define _INCL_VECTORSEARCHES_CPP

namespace VSF{


  template<typename TTT>
  int valuesEqual(const TTT& a, const TTT&b){
    if ( ( a<b)||(b<a)){
      return 0;
    }
    return 1;
  }
  template<typename TTT>
  long lowerBoundSortedVector(const std::vector<TTT> & v, const TTT & s ){
    // If there is an index i such that v[i]==s, then the function returns one such i;
    // If s is not an element of the vector v; then the function returns the
    //             largest k such that v[k]<s
    // If no term of the vector is smaller than or equal to s, then the function returns -1.

    long sz=v.size();
    if(sz<1){
      return -1;
    }
    if(s < v[0]){
      return -1;
    }
    if(sz==1){
      return 0;
    }

    long r=sz-1;
    if(!(s < v[r])){
      return r;
    }
    long l=0;
    long m=(l+r)/2;
    while( (l+1 <r) && (valuesEqual(v[m],s) == 0) ) {
      if( v[m]<s){
        l=m;
      }
      else{
        r=m;
      }
      m=(l+r)/2;
    }
    if( valuesEqual(v[m],s)==1){
      return m;
    }
    return l;
  }

  template<typename TTT>
  long upperBoundSortedVector(const std::vector<TTT> & v, const TTT & s ){
    // If there is an index i such that v[i]==s, then the function returns one such i;
    // If s is not an element of the vector v; then the function returns the
    //             smallest k such that s<v[k]
    // If no term of the vector is larger than or equal to s, then the function returns -1.
    long sz=v.size();
    if(sz<1){
      return -1;
    }
    long r=sz-1;
    if(v[r] < s){
      return -1;
    }
    if(sz==1){
      return 0;
    }
    if(s < v[0]){
      return 0;
    }
    long l=0;
    long m=(r+l)/2;
    while( (l+1 <r) && (valuesEqual(v[m],s) == 0) ) {
      if( v[m]<s){
        l=m;
      }
      else{
        r=m;
      }
      m=(l+r)/2;
    }
    if( valuesEqual(v[m],s)==1){
      return m;
    }
    return r;
  }
}
#endif
