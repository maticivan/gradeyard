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


#ifndef _INCL_SF_STRINGS_CPP
#define _INCL_SF_STRINGS_CPP


namespace SF{
  template<class T1, class T2>
  class TwoValues{
  public:
    T1 x;
    T2 y;
    long operator<(const TwoValues &) const;
  };
  template<class T1, class T2>
  long TwoValues<T1,T2>::operator<(const TwoValues<T1,T2> & oth) const{
    if(x<oth.x){
      return 1;
    }
    if(x>oth.x){
      return 0;
    }
    if(y<oth.y){
      return 1;
    }
    return 0;
  }
  template<typename T1, typename T2>
  void sortWithFollower(std::vector<T1> & v1, std::vector<T2> & v2){
    long sz=v1.size();
    if(v2.size()==sz){
      std::vector<TwoValues<T1,T2> > w;
      w.resize(sz);
      for(long i=0;i<sz;++i){
        (w[i]).x=v1[i];
        (w[i]).y=v2[i];
      }
      std::sort(w.begin(),w.end());
      for(long i=0;i<sz;++i){
        v1[i]=(w[i]).x;
        v2[i]=(w[i]).y;
      }
    }
  }
  char toLowerCase(const char & x){
      char fR=x;
      if((x>='A')&&(x<='Z')){
          fR=fR-'A'+'a';
      }
      return fR;
  }
  std::string toLowerCase(const std::string & input){
      std::string output=input;
      long len = input.length();
      for(long i=0;i<len;++i){
          output[i]=toLowerCase(input[i]);
      }
      return output;
  }
  int isLetterOrBS(const char & c){
    if( (c>='a') && (c<='z')){return 1;}
    if( (c>='A') && (c<='Z')){return 1;}
    if(c=='\\'){return 1;}
    return 0;
  }
  long getPositionOfFirstLetterOrBS(const std::string & st, const long & start=0){
    long len=st.length();
    long res=-1;
    long i=start;
    while((i<len)&&(!isLetterOrBS(st[i]))){
      ++i;
    }
    if((i<len)&&(isLetterOrBS(st[i]))){
      res=i;
    }
    return res;
  }
  std::pair<std::string, int> getEverythingBefore(const std::string & src,
                                                  long & pos,
                                                  const std::string &endSt,
                                                  const int & caseSensitive = 0){
      //
      // If you do the following
      // std::string src="012345678901234567890123456789";
      // long pos=7;
      // std::pair<std::string,int> res=SF::getEverythingBefore(src,pos,"5678");
      // then you will have res.first="78901234" and pos=19
      long size=src.length();
      std::string fR="";
      long endingLen=endSt.length();
      std::string stToAdd="";
      long posInEnd=0;
      char currentCh;
      char compareCh;
      while ((pos<size) &&(posInEnd<endingLen ) ){
          currentCh=src[pos];
          compareCh=endSt[posInEnd];
          if(caseSensitive==0){
              currentCh=toLowerCase(currentCh);
              compareCh=toLowerCase(compareCh);
          }
          stToAdd+= src[pos];
          if(currentCh==compareCh){
              ++posInEnd;
          }
          else{
              pos-=stToAdd.length();
              ++pos;
              fR+=stToAdd[0];
              posInEnd=0;
              stToAdd="";
          }
          ++pos;
      }
      int success=0;
      if(posInEnd==endingLen){
          success=1;
      }
      return std::make_pair(fR,success);
  }
  long firstContainsTheOther(const std::string &st1,const std::string &st2,const int & caseSensitive=0){
    long pos=0;
    std::pair<std::string,int> aD=getEverythingBefore(st1,pos,st2,caseSensitive);
    return aD.second;
  }
  std::pair<std::string,int> extract(const std::string &  ,
                                     long &  ,
                                     const std::string & ,
                                     const std::string & ,
                                     const int &  );
  std::pair<std::string,int> extrStrong(const std::string & src,
                                        long & pos,
                                        const std::string &startString,
                                        const std::string &endString,
                                        const int & caseSensitive = 0){
      if(startString==endString){
        return extract(src,pos,startString,endString,caseSensitive);
      }
      std::pair<std::string,int> fR;
      fR.first="";
      fR.second=0;
      std::pair<std::string,int> thrashSt=getEverythingBefore(src,pos,startString,caseSensitive);
      std::pair<std::string,int> beforeNextStart,beforeNextEnd;
      if(thrashSt.second==1){
          long depth=1;
          long posAfterStart;
          long posAfterEnd;
          while(depth>0){
            posAfterStart=pos;
            posAfterEnd=pos;
            beforeNextStart=getEverythingBefore(src,posAfterStart,startString,caseSensitive);
            beforeNextEnd=getEverythingBefore(src,posAfterEnd,endString,caseSensitive);
            if((beforeNextStart.second==1)&&(beforeNextEnd.second==1)){
              if(posAfterStart<posAfterEnd){
                fR.first+=beforeNextStart.first+startString;
                pos=posAfterStart;
                ++depth;
              }
              else{
                --depth;
                fR.first+=beforeNextEnd.first;
                if(depth>0){
                  fR.first+=endString;
                }
                pos=posAfterEnd;
              }
            }
            else{
              if(beforeNextEnd.second==1){
                --depth;
                fR.first+=beforeNextEnd.first;
                if(depth>0){
                  fR.first+=endString;
                }
                pos=posAfterEnd;
              }
              else{
                fR.second=0;
                fR.first="";
                return fR;
              }
            }
          }
          fR.second=1;
      }
      if(fR.second==0){
          fR.first="";
      }
      return fR;
  }
  std::pair<std::string,int> extract(const std::string & src,
                                     long & pos,
                                     const std::string &startString,
                                     const std::string &endString,
                                     const int & caseSensitive = 0){

      if(startString!=endString){
        return extrStrong(src,pos,startString,endString,caseSensitive);
      }

      std::pair<std::string,int> fR;
      fR.first="";
      fR.second=0;
      std::pair<std::string,int> thrashSt=getEverythingBefore(src,pos,startString,caseSensitive);
      if(thrashSt.second==1){
          fR=getEverythingBefore(src,pos,endString,caseSensitive);
      }
      if(fR.second==0){
          fR.first="";
      }
      return fR;
  }
  std::pair<std::string,int> extractAndReplace(const std::string &  ,
                                               long &  ,
                                               const std::string & ,
                                               const std::string & ,
                                               const int &  ,
                                               const std::string &  );

  std::pair<std::string,int> extrReplStrong(const std::string & src,
                                            long & pos,
                                            const std::string &startString,
                                            const std::string &endString,
                                            const int & caseSensitive = 0,
                                            const std::string & replaceWith=""){
      if(startString==endString){
        return extractAndReplace(src,pos,startString,endString,caseSensitive,replaceWith);
      }
      std::pair<std::string,int> fR;
      fR.first="";
      fR.second=0;
      std::pair<std::string,int> thrashSt=getEverythingBefore(src,pos,startString,caseSensitive);
      std::pair<std::string,int> beforeNextStart,beforeNextEnd;
      if(thrashSt.second==1){
          long depth=1;
          long posAfterStart;
          long posAfterEnd;
          while(depth>0){
            posAfterStart=pos;
            posAfterEnd=pos;
            beforeNextStart=getEverythingBefore(src,posAfterStart,startString,caseSensitive);
            beforeNextEnd=getEverythingBefore(src,posAfterEnd,endString,caseSensitive);
            if((beforeNextStart.second==1)&&(beforeNextEnd.second==1)){
              if(posAfterStart<posAfterEnd){
                fR.first+=beforeNextStart.first+startString;
                pos=posAfterStart;
                ++depth;
              }
              else{
                --depth;
                fR.first+=beforeNextEnd.first;
                if(depth>0){
                  fR.first+=endString;
                }
                pos=posAfterEnd;
              }
            }
            else{
              if(beforeNextEnd.second==1){
                --depth;
                fR.first+=beforeNextEnd.first;
                if(depth>0){
                  fR.first+=endString;
                }
                pos=posAfterEnd;
              }
              else{
                fR.second=0;
                fR.first="";
                return fR;
              }
            }
          }
          fR.second=1;
      }
      if(fR.second==0){
          fR.first=src;
          return fR;
      }
      fR.first=thrashSt.first;
      fR.first+=replaceWith;
      long posSave=(fR.first).length();
      long sz=src.length();
      while(pos<sz){
        fR.first += src[pos];
        ++pos;
      }
      pos=posSave;
      return fR;
  }
  std::pair<std::string,int> extractAndReplace(const std::string & src,
                                               long & pos,
                                               const std::string &startString,
                                               const std::string &endString,
                                               const int & caseSensitive = 0,
                                               const std::string & replaceWith=""){
    if(startString!=endString){
      return extrReplStrong(src,pos,startString,endString,caseSensitive,replaceWith);
    }
    std::pair<std::string,int> fR;
    fR.first="";
    fR.second=0;
    std::pair<std::string,int> thrashSt=getEverythingBefore(src,pos,startString,caseSensitive);
    if(thrashSt.second==1){
        fR=getEverythingBefore(src,pos,endString,caseSensitive);
    }
    if(fR.second==0){
        fR.first="";
        return fR;
    }
    fR.first=thrashSt.first;
    fR.first+=replaceWith;
    long posSave=(fR.first).length();
    long sz=src.length();
    while(pos<sz){
      fR.first += src[pos];
      ++pos;
    }
    pos=posSave;
    return fR;
  }
  std::pair<std::string,int> eraseStuffBetween(const std::string & src,
                                               const std::string &startString,
                                               const std::string &endString,
                                               const long & cpos = 0,
                                               const int & caseSensitive = 0){

      long pos=cpos;
      return extractAndReplace(src,pos,startString,endString,caseSensitive,"");
  }
  std::string genRandomValueIfNecessary(const std::string &input){
    long pos;
    std::pair<std::string,int> allD;
    pos=0;allD=extract(input,pos,"_randomCode_","_/randomCode_");
    if(allD.second==0){
      return input;
    }
    return RNDF::genRandCode(BF::stringToInteger(allD.first));
  }
  std::string replaceVariablesWithValues(const std::string & input,
                                         const std::string & sepVarB, const std::string & sepVarE,
                                         const std::map<std::string,std::string> & varsToVals){
    std::string output=input;
    std::string currentVar,currentVal;
    long sepLens=sepVarB.length()+sepVarE.length();
    long currentValLen;
    std::map<std::string,std::string>::const_iterator it,itE;
    itE=varsToVals.end();
    long pos;
    std::pair<std::string,int> allD;
    pos=0;allD=extract(output,pos,sepVarB,sepVarE);
    std::string newOutput;
    while(allD.second==1){
      currentVar=allD.first;
      it=varsToVals.find(currentVar);
      if(it!=itE){
        pos-=(sepLens+currentVar.length());
        if(pos<0){pos=0;}
        newOutput="";
        for(long i=0;i<pos;++i){
          newOutput+=output[i];
        }
        currentVal=it->second;
        allD=extractAndReplace(output,pos,sepVarB,sepVarE,1,currentVal);
        if(allD.second==1){
          output=newOutput+allD.first;
        }
      }
      allD=extract(output,pos,sepVarB,sepVarE);
    }
    return output;
  }
  template<typename T>
  void flipTheStack(std::stack<T>& s){
    std::stack<T> t;
    t=std::move(s);
    while(!t.empty()){
      s.push(t.top());
      t.pop();
    }
  }
  template<typename TTT>
  std::set<TTT> vectorToSet(const std::vector<TTT> &_v){
    std::set<TTT> fR;
    long sz=_v.size();
    for(long i=0;i<sz;++i){
      fR.insert(_v[i]);
    }
    return fR;
  }
  std::string findAndReplace(const std::string &input,
                          const std::string &_searchFor,
                          const std::vector<std::string> &_replaceWith,
                          const int &all=1,
                          const int &caseSensitive=0){
      // Searches input for _searchFor; The first one is replaced by _replaceWith[0],
      // the second occurrence with _replaceWith[1], ..., the _replaceWith.size()-th ocurrence
      // with _replaceWith[0] again and it cycles

      std::string searchFor=_searchFor;
      std::vector<std::string> replaceWith=_replaceWith;
      if(replaceWith.size()==0){
        replaceWith.resize(1);replaceWith[0]="";
      }
      long ctrReplaceWith=0;
      long lenReplW=replaceWith.size();
      std::string endS=GF::GL_HIDING_STRING_SF;
      long pos=0;
      std::pair<std::string,int> allD;
      std::string output="",nBefore;
      long len=input.length();
      long lsf=searchFor.length();
      long lp1=len+1;
      long oldPos;
      while(pos<len){
        oldPos=pos;
        allD=getEverythingBefore(input,pos,searchFor,caseSensitive);
        if(allD.second==1){
          output += allD.first;
          if(pos<lp1){
            //Found the real occurrence of searchFor
            // not the fake one that we added to the end
            output+=replaceWith[ctrReplaceWith];
            if(lenReplW>1){
              ++ctrReplaceWith;
              if(ctrReplaceWith==lenReplW){
                ctrReplaceWith=0;
              }
            }
            if(all==0){
              searchFor=searchFor+endS;
            }
          }
        }
        else{
          pos=len;
          for(long i=oldPos;i<pos;++i){
            output+=input[i];
          }
        }

      }
      return output;
  }
  std::string findAndReplace(const std::string &input,
                          const std::string &searchFor,
                          const std::string &_replaceWith,
                          const int &all=1,
                          const int &caseSensitive=0){

      if(searchFor==_replaceWith){
        return input;
      }
      std::vector<std::string> replw;
      replw.resize(1);
      replw[0]=_replaceWith;
      return findAndReplace(input,searchFor,replw,all,caseSensitive);
  }

  long getFirstPosition(const std::string &input,
                        const long & _pos,
                        const std::string &searchFor,
                        const int & caseSensitive=0){
    long pos=_pos;
    std::pair<std::string,int> allD=getEverythingBefore(input,pos,searchFor,caseSensitive);
    if(allD.second==0){
      return input.length();
    }
    return pos-searchFor.length();
  }
  void updateFirstPositions(std::vector<long>& fPositions,
                            const std::string &input,
                            const long &_pos,
                            const std::vector<std::string> &searchFor,
                            const int& caseSensitive=0){

    long ln=searchFor.size();
    if((ln==fPositions.size())&&(ln>0)){
      for(long i=0;i<ln;++i){
        if( fPositions[i]<_pos ){
          fPositions[i]=getFirstPosition(input,_pos,searchFor[i],caseSensitive);
        }
      }
    }
  }
  std::pair<long,long> minAndMinimizer(const std::map<long,long> & m){
    std::pair<long,long> res;
    res.first=-1;res.second=-1;
    long sz=m.size();
    if(sz<1){return res;}
    return *(m.begin());
  }
  std::map<long,long> firstPosMap(const std::vector<long> & v){
    std::map<long,long> res;
    long i=v.size();
    while(i>0){
      --i;
      res[v[i]]=i;
    }
    return res;
  }
  std::string findAndReplaceSecureButNotVeryFast(const std::string &input,
                             const std::vector<std::string> &searchFor,
                             const std::vector<std::string> &replaceWith,
                             const int &caseSensitive=0){
      long lenSF=searchFor.size();
      if((lenSF<1)||(replaceWith.size()!=lenSF)){
        return input;
      }
      std::vector<long> fPositions;
      fPositions.resize(lenSF);
      for(long i=0;i<lenSF;++i){
        fPositions[i]=-1;
      }
      std::pair<long,long> minsV;
      long pos=0;
      updateFirstPositions(fPositions,input,pos,searchFor,caseSensitive);
      std::map<long,long> fPMap=firstPosMap(fPositions);
      long sz=input.length();
      std::string output;
      while(pos<sz){
        minsV=minAndMinimizer(fPMap);
        while(pos<minsV.first){
          output+=input[pos];
          ++pos;
        }
        if((minsV.second>-1)&&(minsV.first<sz)){
          output+=replaceWith[minsV.second];
          pos+=searchFor[minsV.second].length();
        }
        fPMap.erase(fPMap.begin());
        fPMap[getFirstPosition(input,pos, searchFor[minsV.second],caseSensitive)]=minsV.second;
      }
      return output;
  }
  std::string findAndReplace(const std::string &input,
                             const std::vector<std::string> &searchFor,
                             const std::vector<std::string> &replaceWith,
                             const int &caseSensitive=0){
      // This function trusts that
      // 1) searchFor[i] appears only once for every i.
      // 2) searchFor[i] appears before searchFor[j] if i<j
      // If 1) or 2) fails, then the function will findAndReplaceSecureButNotVeryFast.
      long lenSF=searchFor.size();
      if((lenSF<1)||(replaceWith.size()!=lenSF)){
        return input;
      }
      std::vector<long> fPositions;
      fPositions.resize(lenSF);
      for(long i=0;i<lenSF;++i){
        fPositions[i]=-1;
      }
      std::pair<long,long> minsV;
      long pos=0;
      updateFirstPositions(fPositions,input,pos,searchFor,caseSensitive);
      long trustworthy=1;
      long i=1;
      while((i<lenSF)&&(trustworthy==1)){
        if(fPositions[i-1]>=fPositions[i]){
          trustworthy=0;
        }
        ++i;
      }
      if(trustworthy==0){
        return findAndReplaceSecureButNotVeryFast(input,searchFor,replaceWith,caseSensitive);
      }
      long sz=input.length();
      std::string output;
      i=0;
      while((pos<sz)&&(i<lenSF)){
        minsV.first=fPositions[i];
        minsV.second=i;
        while(pos<fPositions[i]){
          output+=input[pos];
          ++pos;
        }
        if(fPositions[i]<sz){
          output+=replaceWith[i];
          pos+=searchFor[i].length();
        }

        ++i;
      }
      while(pos<sz){
        output+=input[pos];
        ++pos;
      }
      return output;
  }
  long countInString(const std::string & _allItems,
                                    const std::string & _nextB="_n_",
                                    const std::string & _nextE="_/n_",
                                    const std::string & _attMustHave="!*!"){
    long fR=0;
    long pos=0;
    std::pair<std::string,int> nextSt=extract(_allItems,pos,_nextB,_nextE);
    while(nextSt.second==1){
      if(_attMustHave!="!*!"){
        fR+=firstContainsTheOther(nextSt.first,_attMustHave);
      }
      else{
        ++fR;
      }
      nextSt=extract(_allItems,pos,_nextB,_nextE);
    }
    return fR;
  }
  std::string vectorToString(const std::vector<std::string> & v,
                            const std::string & _nextB="_n_",
                            const std::string & _nextE="_/n_"){
    std::string fR="";
    long sz=v.size();
    for(long i=0;i<sz;++i){
      fR+=_nextB+v[i]+_nextE;
    }
    return fR;
  }
  std::vector<std::string> stringToVector(const std::string & _allItems,
                                          const std::string & _nextB="_n_",
                                          const std::string & _nextE="_/n_",
                                          const std::string & _attMustHave="!*!"){
    std::vector<std::string> fR;
    long sz,szAll;
    sz=countInString(_allItems,_nextB,_nextE);
    szAll=sz;
    if(_attMustHave!="!*!"){
      sz=countInString(_allItems,_nextB,_nextE,_attMustHave);
    }
    if(sz==0){
      return fR;
    }
    fR.resize(sz);
    long pos=0;
    std::string nextSt;
    long j=0,i;
    for(i=0;i<szAll;++i){
        nextSt= extract(_allItems,pos,_nextB,_nextE).first;
        if(_attMustHave=="!*!"){
          fR[i]=nextSt;
        }
        else{
          if(firstContainsTheOther(nextSt,_attMustHave)==1){
            fR[j]=nextSt;++j;
          }
        }
    }
    return fR;
  }
  std::vector<std::vector<std::string> > stringToMatrix(const std::string & _qs,
                                                        const std::string & _nextB,
                                                        const std::string & _nextE,
                                                        const std::string & _sepInsideB,
                                                        const std::string & _sepInsideE){
    std::vector<std::vector<std::string> > fR;
    long sz=countInString(_qs,_nextB,_nextE);
    if(sz==0){
      return fR;
    }
    fR.resize(sz);
    long pos=0;
    std::string individualLine;
    for(long i=0;i<sz;++i){
      individualLine=extract(_qs,pos,_nextB,_nextE).first;
      fR[i]=stringToVector(individualLine,_sepInsideB,_sepInsideE);
    }
    return fR;
  }
  std::string removeInfoAndPrepareNew(std::string &oldSysData, const std::string & _bS, const std::string & _eS, const std::string & _nD){
    std::string nD=_bS+_nD+_eS;
    long pos=0;
    std::pair<std::string, int> allD=eraseStuffBetween(oldSysData,_bS,_eS,pos);
    if(allD.second==1){
      oldSysData=allD.first;
    }
    return nD;
  }
  long getLegalCharacters(std::map<char,long> &fR1, std::map<long,char> &fR2){
   char c;
   long toAdd=0;
   for(c='0';c<='9';++c){
     fR1[c]=toAdd;
     fR2[toAdd]=c;
     ++toAdd;
   }
   for(c='a';c<='z';++c){
     fR1[c]=toAdd;
     fR2[toAdd]=c;
     ++toAdd;
   }
   for(c='A';c<='Z';++c){
     fR1[c]=toAdd;
     fR2[toAdd]=c;
     ++toAdd;
   }
   return toAdd;
  }
  class TranslatorCharToLong{
  private:
    long num;
    std::map<char,long> cToL;
    std::map<long,char> lToC;
  public:
    TranslatorCharToLong();
    long getLong(const char &) const;
    char getChar(const long &) const;
    long getNum() const;
  };
  TranslatorCharToLong::TranslatorCharToLong(){
    num=getLegalCharacters(cToL,lToC);
  }
  long TranslatorCharToLong::getLong(const char & _c) const{
    std::map<char,long>::const_iterator it,itE;
    itE=cToL.end();
    it=cToL.find(_c);
    long fR=-1;
    if(it!=itE){
      fR=it->second;
    }
    return fR;
  }
  char TranslatorCharToLong::getChar(const long & _l) const{
    std::map<long,char>::const_iterator it,itE;
    itE=lToC.end();
    it=lToC.find(_l);
    char fR='0';
    if(it!=itE){
      fR=it->second;
    }
    return fR;
  }
  long TranslatorCharToLong::getNum() const{
    return num;
  }
  TranslatorCharToLong GLOBAL_TRANSLATOR_CHAR_LONG;
  long charToNum(const char & x){
    return GLOBAL_TRANSLATOR_CHAR_LONG.getLong(x);
  }
  std::string numToStr(const long & _l){
    long l=_l%256;
    std::string fR="a";
    char c=GLOBAL_TRANSLATOR_CHAR_LONG.getChar(l);
    fR[0]=c;
    return fR;
  }
  std::vector<long> strToVectInt(const std::string &_s){
    std::vector<long> fR;
    long sz=_s.size();
    fR.resize(sz);
    long i=0;
    while(i<sz){
      fR[i]= charToNum(_s[i]);
      ++i;
    }
    return fR;
  }
  std::string vectIntToStr(const std::vector<long> &_c){
    long i=0;
    long lC=_c.size();
    std::string c;
    while(i<lC){
      c+=numToStr(_c[i]);
      ++i;
    }
    return c;
  }
  std::vector<long> getAdditionVector6(){
    std::vector<long> aV6;
    aV6.resize(6);
    aV6[0]=7;aV6[1]=9;aV6[2]=3;aV6[3]=8;aV6[4]=7;aV6[5]=4;
    return aV6;
  }
  std::string combineTwoWords(const std::string &_a, const std::string &_b){
    // lengths of _a and _b must be smaller than or equal to 999
    long numChr=GLOBAL_TRANSLATOR_CHAR_LONG.getNum();
    std::vector<long> aV6=getAdditionVector6();
    std::string __c;
    std::string __b=_b;
    if(__b.length()<3){
      __b="000"+__b;
    }
    std::string __a=_a;
    if(__a.length()<3){
      __a="000"+__a;
    }
    std::vector<long> a,b;
    a=strToVectInt(__a);b=strToVectInt(__b);
    std::vector<long> c;
    long lA,lB,lC;
    lA=a.size();lB=b.size();
    std::string __slA=BF::padded(lA,100,"0");
    std::string __slB=BF::padded(lB,100,"0");
    std::vector<long> slA=strToVectInt(__slA);
    std::vector<long> slB=strToVectInt(__slB);
    lC=lA+lB+6;
    c.resize(lC);
    long i=1;
    c[0]=(a[0]+b[0])%numChr;
    while(i<3){
      c[i]=(a[i]+b[i]+c[i-1])%numChr;++i;
    }
    c[3]=(c[2]+c[1]+slA[0]+aV6[0])%numChr;
    c[4]=(c[3]+c[1]+slB[0]+aV6[1])%numChr;
    c[5]=(c[4]+c[1]+slA[1]+aV6[2])%numChr;
    c[6]=(c[5]+c[1]+slB[1]+aV6[3])%numChr;
    c[7]=(c[6]+c[1]+slA[2]+aV6[4])%numChr;
    c[8]=(c[7]+c[1]+slB[2]+aV6[5])%numChr;
    i=9;
    long posB=lB;
    while(posB>0){
      --posB;
      c[i]=(c[i-1]+b[posB])%numChr;
      ++i;
    }
    posB=3;
    long posA=3;
    while(posA<lA){
      c[i]=(c[i-1]+b[posB]+a[posA])%numChr;
      ++i;++posA;++posB;
      if(posB>=lB){
        posB=0;
      }
    }
    return vectIntToStr(c);
  }
  long makeItBiggerThanOrEqual0(long x){
    long numChr=GLOBAL_TRANSLATOR_CHAR_LONG.getNum();
    while(x<0){
      x+=numChr;
    }
    return x;
  }
  std::pair<std::string,std::string> oneWordToTwoWords(const std::string &_st){
    std::vector<long> aV6=getAdditionVector6();
    long lenC=_st.length();
    if(lenC<9){
      return std::pair<std::string,std::string>("notFound","notFound");
    }
    std::string __a,__b;
    std::vector<long> a,b,c,slA,slB;
    c=strToVectInt(_st);
    slA.resize(3);slB.resize(3);
    slA[0]=makeItBiggerThanOrEqual0(c[3]-c[2]-c[1]-aV6[0]);
    slB[0]=makeItBiggerThanOrEqual0(c[4]-c[3]-c[1]-aV6[1]);
    slA[1]=makeItBiggerThanOrEqual0(c[5]-c[4]-c[1]-aV6[2]);
    slB[1]=makeItBiggerThanOrEqual0(c[6]-c[5]-c[1]-aV6[3]);
    slA[2]=makeItBiggerThanOrEqual0(c[7]-c[6]-c[1]-aV6[4]);
    slB[2]=makeItBiggerThanOrEqual0(c[8]-c[7]-c[1]-aV6[5]);
    long lA,lB;
    lA=BF::stringToInteger(vectIntToStr(slA));
    lB=BF::stringToInteger(vectIntToStr(slB));
    long posC=9;
    long posB=lB;
    a.resize(lA);b.resize(lB);
    while(posB>0){
      --posB;
      b[posB]=makeItBiggerThanOrEqual0(c[posC]-c[posC-1]);
      ++posC;
    }
    long posA=3;
    posB=3;
    a[0]=makeItBiggerThanOrEqual0(c[0]-b[0]);
    a[1]=makeItBiggerThanOrEqual0(c[1]-c[0]-b[1]);
    a[2]=makeItBiggerThanOrEqual0(c[2]-c[1]-b[2]);
    while(posA<lA){
      a[posA]=makeItBiggerThanOrEqual0(c[posC]-c[posC-1]-b[posB]);
      ++posA;++posC;++posB;
      if(posB>=lB){
        posB=0;
      }
    }
    __a=vectIntToStr(a);
    __b=vectIntToStr(b);
    return std::pair<std::string,std::string>(__a,__b);
  }
  std::string getElFromMapOrNotFoundMessage(const std::map<std::string,std::string> & _mm, const std::string & _q, const std::string & _mnf){
    std::map<std::string,std::string>::const_iterator it,itE=_mm.end();
    it=_mm.find(_q);
    if(it!=itE){
      return it->second;
    }
    return _mnf;
  }
  std::vector<std::string> removeFromSeq(const std::vector<std::string> &in, const std::string & toRemove){
    std::vector<std::string> out;
    long sz=in.size();
    long i=0;
    while((i<sz)&&(in[i]!=toRemove)){
      ++i;
    }
    if(i==sz){
      return in;
    }
    out.resize(sz-1);
    if(sz==1){
      return out;
    }
    for(long j=0;j<i;++j){
      out[j]=in[j];
    }
    ++i;
    while(i<sz){
      out[i-1]=in[i];
    }
    return out;
  }
  void varValPairs(const std::string & input,
                                                const std::string & sepVarValB, const std::string & sepVarValE,
                                                const std::string & sepVarB,const std::string & sepVarE,
                                                const std::string & sepValB, const std::string &sepValE,
                                                std::map<std::string,std::string> & mM){
    std::vector<std::string> indItems=stringToVector(input,sepVarValB,sepVarValE);
    long sz=indItems.size();
    long pos;std::pair<std::string,int> allD;
    std::string val,var;
    for(long i=0;i<sz;++i){
      pos=0;allD=extract(indItems[i],pos,sepVarB,sepVarE);
      if(allD.second==1){
        var=allD.first;
        pos=0;allD=extract(indItems[i],pos,sepValB,sepValE);
        if(allD.second==1){
          val=allD.first;
          mM[var]= genRandomValueIfNecessary( val);
        }
      }
    }
  }
  std::string massiveFiRepl(const std::string &input, const std::vector<std::pair<std::string, std::string> > & v, const int & caseSensitive){
    std::string output=input;
    long sz=v.size();
    for(long i=0;i<sz;++i){
      output=findAndReplace(output,v[i].first,v[i].second,1,caseSensitive);
    }
    return output;
  }
  std::string massiveFiReplReverse(const std::string &input, const std::vector<std::pair<std::string, std::string> > & v, const int & caseSensitive){
    std::string output=input;
    long sz=v.size();
    for(long i=0;i<sz;++i){
      output=findAndReplace(output,v[i].second,v[i].first,1,caseSensitive);
    }
    return output;
  }
  std::pair<std::string,int> replaceAllOuterLayerSeparators(const std::string &input,
                                                            const std::string & _cOpen,
                                                            const std::string &_cClose,
                                                            const std::string & _rOpen,
                                                            const std::string &_rClose,
                                                            const std::vector<std::pair<std::string,std::string> > & permanentChangesForProtection ,
                                                            const int & caseSensitive =0){
    std::string output="";
    std::pair<std::string,int> fR;fR.second=1;
    long pos=0,oldPos,newPos;
    std::pair<std::string,int> allD,allDB;
    allD=extract(input,pos,_cOpen,_cClose,caseSensitive);
    if(allD.second==0){
      fR.first=input;
      return fR;
    }
    oldPos=0;
    int keepGoing=1;
    std::string replacingPart;
    long szV=permanentChangesForProtection.size();
    while(keepGoing==1){
      replacingPart= allD.first;
      if(szV>0){
        replacingPart=massiveFiRepl(replacingPart,permanentChangesForProtection,caseSensitive);
      }
      allDB=getEverythingBefore(input,oldPos,_cOpen,caseSensitive);
      if(allDB.second==1){
        output+=allDB.first;
        output+=_rOpen+replacingPart+_rClose;
        oldPos=pos;
        allD=extract(input,pos,_cOpen,_cClose,caseSensitive);
        if(allD.second==0){
          keepGoing=0;
        }
      }
      else{
        fR.second=0;keepGoing=0;
      }
    }
    long ln=input.length();
    while(oldPos<ln){
      output+=input[oldPos];
      ++oldPos;
    }
    fR.first=output;
    return fR;
  }
  std::string getTagInteriorAndRemoveTag(std::string& source,const std::string& openT, const std::string& closeT){
    long pos; std::pair<std::string,int> allD;
    pos=0;allD=extract(source,pos,openT,closeT);
    if(allD.second==0){
      return "";
    }
    std::string interior=allD.first;
    source=findAndReplace(source,openT+interior+closeT,"");
    return interior;
  }
  std::set<std::string> stringToSet(const std::string & everything, const std::string & sepKeyB,const std::string & sepKeyE){
    std::set<std::string> fR;
    std::vector<std::string> keys=stringToVector(everything,sepKeyB,sepKeyE);
    long sz=keys.size();
    for(long i=0;i<sz;++i){
      fR.insert(keys[i]);
    }
    return fR;
  }
  std::string setToString(const std::set<std::string> & s, const std::string & sepKeyB,const std::string & sepKeyE ,const std::string &formattingSeparator=""){
    std::string fR="";
    std::set<std::string>::const_iterator it=s.begin(),itE=s.end();
    while(it!=itE){
      fR+=sepKeyB+(*it)+sepKeyE+formattingSeparator;
      ++it;
    }
    return fR;
  }
  std::map<std::string,std::string> stringToMap(const std::string & everything, const std::string & sepKeyB,const std::string & sepKeyE,const std::string & sepValB,const std::string & sepValE){
    std::map<std::string,std::string> fR;
    std::vector<std::string> keys=stringToVector(everything,sepKeyB,sepKeyE);
    std::vector<std::string> vals=stringToVector(everything,sepValB,sepValE);
    long szK=keys.size();
    long szV=vals.size();
    long sz=szK;
    if(sz>szV){sz=szV;}
    for(long i=0;i<sz;++i){
      fR[keys[i]]=vals[i];
    }
    return fR;
  }
  std::string mapToString(const std::map<std::string,std::string> & m,const std::string &beginSeparator,const std::string &endSeparator, const std::string & sepKeyB,const std::string & sepKeyE,const std::string & sepValB,const std::string & sepValE){
    std::string fR="";
    std::map<std::string,std::string>::const_iterator it=m.begin(),itE=m.end();
    while(it!=itE){
      fR+=beginSeparator+sepKeyB+(it->first)+sepKeyE+sepValB+(it->second)+sepValE+endSeparator;
      ++it;
    }
    return fR;
  }
  std::string mapToString(const std::map<std::string,std::string> & m, const std::string & sepKeyB,const std::string & sepKeyE,const std::string & sepValB,const std::string & sepValE,const std::string &formattingSeparator = ""){
    return mapToString(m,"",formattingSeparator,sepKeyB,sepKeyE,sepValB,sepValE);
  }
  std::string updateVarsWithVals(const std::string & input, const std::map<std::string,std::string> & sr, const std::string & sB, const std::string &sE){
    std::string output=input;
    std::map<std::string,std::string>::const_iterator it,itE;
    it=sr.begin();itE=sr.end();
    while(it!=itE){
      output=findAndReplace(output,sB+(it->first)+sE,sB+(it->second)+sE);
      ++it;
    }
    return output;
  }
  template<typename TTT>
  std::vector<TTT> oneElementVector(const TTT& el){
    std::vector<TTT> fR;fR.resize(1);fR[0]=el;
    return fR;
  }
  template<typename TTT>
  std::stack<TTT> clearStack(std::stack<TTT> & v){
    if(v.empty()){
      return v;
    }
    v.pop();
    return clearStack(v);
  }
  template<typename TTT>
  std::vector<TTT> stackToVector(std::stack<TTT> & st){
    std::vector<TTT> fR;
    long sz=st.size();
    if(sz>0){
      fR.resize(sz);
      for(long i=0;i<sz;++i){
        fR[sz-i-1]=st.top();
        st.pop();
      }
    }
    return fR;
  }
  std::string incrementString(const std::string & x){
      std::string res=x;
      long cS=res.size();
      long success=0;--cS;
      char zeroM1='0'-1;
      char nineP1='9'+1;
      char aM1='a'-1;
      char zP1='z'+1;
      while((success==0)&&(cS>-1)){
          if((res[cS]>zeroM1)&&(res[cS]<'9')){
              res[cS]=res[cS]+1;success=1;
          }
          if((success==0)&&(res[cS]=='9')){
              res[cS]='a';success=1;
          }
          if((success==0)&&(res[cS]>aM1)&&(res[cS]<zP1)){
              if(res[cS]<'z'){
                  res[cS]=res[cS]+1;
                  success=1;
              }
              else{
                  res[cS]='0';
              }
          }
          cS--;
      }
      if(success==0){
          res='1'+res;
      }
      return res;
  }
  std::string findYForNonSubstring(const std::string & x, const std::string & start ="aaa", const std::string &beg="b", const std::string &end="e"){
    //The function will find a string y >= start for which (beg+y+end) is not a substring of x
    std::string y=start;
    while(findAndReplace(x,beg+y+end,"")!=x){
      y=incrementString(y);
    }
    return y;
  }
  std::string findNonSubstring(const std::string &x, const std::string & start ="aaa", const std::string &beg="b",const std::string &end="e"){
    //The function will find a string z that starts with beg, ends with end, is not substring of x,
    //and the middle part y of z (obtained after removing beg and end) is >=start
    return beg+findYForNonSubstring(x,start,beg,end)+end;
  }
  void extendMapWithSubMapOfAnotherMap(std::map<std::string,std::string>& dest, const std::map<std::string,std::string> &src, const std::string & keyPrefix, const std::string & valPrefix, const long & maxNum){
    long i=0;
    std::string key,val;
    std::map<std::string,std::string>::const_iterator itSrc,itSrcE;
    itSrcE=src.end();
    while(i<maxNum){
      itSrc=src.find(keyPrefix+std::to_string(i));
      if(itSrc!=itSrcE){
        key=itSrc->second;
        itSrc=src.find(valPrefix+std::to_string(i));
        if(itSrc!=itSrcE){
          val=itSrc->second;
          dest[key]=val;
        }
      }
      ++i;
    }
  }
  std::string forwardValVarPairs(const std::map<std::string,std::string> & src, const std::string & keyPrefix, const std::string & valPrefix, const long &maxNum){
    std::map<std::string,std::string> kvPairs;
    extendMapWithSubMapOfAnotherMap(kvPairs,src,keyPrefix,valPrefix,maxNum);
    std::map<std::string,std::string>::const_iterator it,itE;
    itE=kvPairs.end();
    it=kvPairs.begin();
    std::string res="";
    long i=0;
    while(it!=itE){
      res+="&"+keyPrefix+std::to_string(i)+"="+it->first;
      res+="&"+valPrefix+std::to_string(i)+"="+it->second;
      ++it;
      ++i;
    }
    return res;
  }
}
#endif
