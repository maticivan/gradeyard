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


#ifndef _INCL_MYHDB_CPP
#define _INCL_MYHDB_CPP


namespace HBF{
  struct Separators{
  public:
    std::string sepPTB="_P**!_";
    std::string sepPTE="_/P**!_";
    std::string sepCTB="_C**!_";
    std::string sepCTE="_/C**!_";

    std::string sepHB="_H**!_";
    std::string sepHE="_/H**!_";

    std::string sepNumB="_NUM**!_";
    std::string sepNumE="_/NUM**!_";
    std::string sepNB="_N**!_";
    std::string sepNE="_/N**!_";

    std::string sepVB="_V**!_";
    std::string sepVE="_/V**!_";



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




      std::string fR=GL_HDB_SepStrings.sepNumB+std::to_string(num)+GL_HDB_SepStrings.sepNumE;
      fR+=GL_HDB_SepStrings.sepHB+std::to_string(height)+GL_HDB_SepStrings.sepHE;
      if(num>0){
        fR+=GL_HDB_SepStrings.sepVB;
        for(long i=0;i<num;++i){
          fR+=GL_HDB_SepStrings.sepNB+values[i].putIntoString()+GL_HDB_SepStrings.sepNE;
        }
        fR+=GL_HDB_SepStrings.sepVE;
      }
      long np1=num+1;
      fR+=GL_HDB_SepStrings.sepPTB;
      for(long i=0;i<np1;++i){
        fR+=GL_HDB_SepStrings.sepNB+children[i].putIntoString()+GL_HDB_SepStrings.sepNE;
      }
      fR+=GL_HDB_SepStrings.sepPTE;
      fR+=GL_HDB_SepStrings.sepCTB;
      for(long i=0;i<np1;++i){
        fR+=GL_HDB_SepStrings.sepNB+std::to_string(counts[i])+GL_HDB_SepStrings.sepNE;
      }
      fR+=GL_HDB_SepStrings.sepCTE;
      return fR;
  }

  template<typename TTT>
  void Node<TTT>::loadFromString(const std::string &s){
      std::string d=s;



      d+=GL_HDB_SepStrings.sepNumB+std::to_string(num)+GL_HDB_SepStrings.sepNumE;
      d+=GL_HDB_SepStrings.sepHB+std::to_string(height)+GL_HDB_SepStrings.sepHE;
      if(num>0){
        d+=GL_HDB_SepStrings.sepVB;
        for(long i=0;i<num;++i){
          d+=GL_HDB_SepStrings.sepNB+values[i].putIntoString()+GL_HDB_SepStrings.sepNE;
        }
        d+=GL_HDB_SepStrings.sepVE;
      }
      long np1=num+1;
      d+=GL_HDB_SepStrings.sepPTB;
      for(long i=0;i<np1;++i){
        d+=GL_HDB_SepStrings.sepNB+children[i].putIntoString()+GL_HDB_SepStrings.sepNE;
      }
      d+=GL_HDB_SepStrings.sepPTE;
      d+=GL_HDB_SepStrings.sepCTB;
      for(long i=0;i<np1;++i){
        d+=GL_HDB_SepStrings.sepNB+std::to_string(counts[i])+GL_HDB_SepStrings.sepNE;
      }
      d+=GL_HDB_SepStrings.sepCTE;

      long pos;
      pos=0;
      std::string forNum;
      forNum=(SF::extract(d,pos,GL_HDB_SepStrings.sepNumB,GL_HDB_SepStrings.sepNumE)).first;
      num=BF::stringToInteger(forNum);
      if(num<1){
        Node<TTT> reset0;
        *this=reset0;
      }
      else{

        pos=0;forNum=(SF::extract(d,pos,GL_HDB_SepStrings.sepHB,GL_HDB_SepStrings.sepHE)).first;
        height=BF::stringToInteger(forNum);
        if(height<0){height=0;}
        np1=num+1;
        std::string forVals;
        pos=0;forVals=(SF::extract(d,pos,GL_HDB_SepStrings.sepVB,GL_HDB_SepStrings.sepVE)).first;
        std::vector<std::string> vectSt=SF::stringToVector(forVals,GL_HDB_SepStrings.sepNB,GL_HDB_SepStrings.sepNE);
        if(vectSt.size()!=num){
          Node<TTT> reset0;
          *this=reset0;
        }
        else{
          values.resize(num);
          for(long i=0;i<num;++i){
            values[i].loadFromString(vectSt[i]);
          }
          pos=0;forVals=(SF::extract(d,pos,GL_HDB_SepStrings.sepCTB,GL_HDB_SepStrings.sepCTE)).first;
          vectSt=SF::stringToVector(forVals,GL_HDB_SepStrings.sepNB,GL_HDB_SepStrings.sepNE);
          if(vectSt.size()!=np1){
            Node<TTT> reset0; *this=reset0;
          }
          else{
            counts.resize(np1);
            for(long i=0;i<np1;++i){
              counts[i]=BF::stringToInteger(vectSt[i]);
            }
            pos=0;forVals=(SF::extract(d,pos,GL_HDB_SepStrings.sepPTB,GL_HDB_SepStrings.sepPTE)).first;
            vectSt=SF::stringToVector(forVals,GL_HDB_SepStrings.sepNB,GL_HDB_SepStrings.sepNE);
            if(vectSt.size()!=np1){
              Node<TTT> reset0; *this=reset0;
            }
            else{
              children.resize(np1);
              for(long i=0;i<np1;++i){
                children[i].loadFromString(vectSt[i]);
              }
            }
          }
        }

      }

  }

  template<typename TTT>
  int Node<TTT>::putToFile(const std::string & fName) const{
    std::string s=putIntoString();
    return IOF::toFile(fName,s);
  }

}




#endif
