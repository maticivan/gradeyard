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


#ifndef _INCL_MYHDB_CPP
#define _INCL_MYHDB_CPP

namespace HBF{
  struct Separators{
  public:
    std::string sepB="_**!_";
    std::string sepE="_/**!_";
  } GL_HDB_SepStrings;
  template<typename TTT>
  class Node{
  public:
      std::vector<TTT> values;
      std::vector<long> counts;
      std::vector<HDPF::Pointer<Node<TTT> > > children;
      long num;
      long height;
      Node(){
          num=0;
          values.clear();
          counts.resize(1);
          children.resize(1);
          height=0;
      }
      Node(const TTT& k ) {
          num=1;
          values.resize(1);
          counts.resize(2);
          children.resize(2);
          values[0]=k;
          counts[0]=0;counts[1]=0;
          height = 1;
      }
      void setNum(const long &);
      std::string putIntoString() const;
      int putToFile(const std::string &) const;
      void loadFromString(const std::string &);
  };
  template<typename TTT>
  void Node<TTT>::setNum(const long & _n){
    if(_n>-1){
      num=_n;
      values.clear();
      if(num>0){
        values.resize(num);
      }
      long np1=num+1;
      counts.resize(np1);
      children.resize(np1);
      for(long i=0;i<np1;++i){
        counts[i]=0;
      }
    }
  }
  template<typename TTT>
  std::string Node<TTT>::putIntoString() const{
      std::string fR=GL_HDB_SepStrings.sepB+std::to_string(num)+GL_HDB_SepStrings.sepE;
      fR+=GL_HDB_SepStrings.sepB+std::to_string(height)+GL_HDB_SepStrings.sepE;
      if(num>0){
        for(long i=0;i<num;++i){
          fR+=GL_HDB_SepStrings.sepB+values[i].putIntoString()+GL_HDB_SepStrings.sepE;
        }
      }
      long np1=num+1;
      for(long i=0;i<np1;++i){
        fR+=GL_HDB_SepStrings.sepB+children[i].putIntoString()+GL_HDB_SepStrings.sepE;
      }
      for(long i=0;i<np1;++i){
        fR+=GL_HDB_SepStrings.sepB+std::to_string(counts[i])+GL_HDB_SepStrings.sepE;
      }
      return fR;
  }
  template<typename TTT>
  void Node<TTT>::loadFromString(const std::string &s){
      Node<TTT> reset0;
      std::string d=s;
      std::vector<std::string> vInfo=SF::stringToVector(s,GL_HDB_SepStrings.sepB,GL_HDB_SepStrings.sepE);
      long sz=vInfo.size();
      if(sz<1){
        *this=reset0;
        return;
      }
      num=BF::stringToInteger(vInfo[0]);
      if((num<1)||(sz!=3*num+4)){
        *this=reset0;
        return;
      }
      height=BF::stringToInteger(vInfo[1]);
      if(height<0){height=0;}
      long mainCounter=2;
      values.resize(num);
      for(long i=0;i<num;++i){
        values[i].loadFromString(vInfo[mainCounter]);
        ++mainCounter;
      }
      long np1=num+1;
      children.resize(np1);
      for(long i=0;i<np1;++i){
        children[i].loadFromString(vInfo[mainCounter]);
        ++mainCounter;
      }
      counts.resize(np1);
      for(long i=0;i<np1;++i){
        counts[i]=BF::stringToInteger(vInfo[mainCounter]);
        ++mainCounter;
      }
  }
  template<typename TTT>
  int Node<TTT>::putToFile(const std::string & fName) const{
    std::string s=putIntoString();
    return IOF::toFile(fName,s);
  }
}
#endif
