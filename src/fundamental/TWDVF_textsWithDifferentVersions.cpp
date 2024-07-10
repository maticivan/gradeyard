//    GradeYard learning management system
//
//    Copyright (C) 2024 Ivan Matic, https://gradeyard.com
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

#ifndef _INCL_TEXTSWITHDIFFERENTVERSIONS_CPP
#define _INCL_TEXTSWITHDIFFERENTVERSIONS_CPP

namespace TWDVF{
  std::string eraseLeadingAndTrailingEmptyCharacters(const std::string &in){
    std::string output;
    long sz=in.size();
    while((sz>0)&&((in[sz-1]==' ')||(in[sz-1]=='\t')||(in[sz-1]=='\n')||(in[sz-1]=='\r'))){
      --sz;
    }
    long b=0;
    while((b<sz)&&((in[b]==' ')||(in[b]=='\t')||(in[b]=='\n')||(in[b]=='\r'))){
      ++b;
    }
    for(long i=b;i<sz;++i){
      output+=in[i];
    }
    return output;
  }
  std::vector<long> identifyPermutationExtension(const std::string& sReplace){
    std::vector<long> pExtensionL;
    std::string p_B="_permutationExtension_";
    std::string p_E="_/permutationExtension_";
    long pos=0;
    std::pair<std::string,int> allD=SF::extract(sReplace,pos,p_B,p_E);
    if(allD.second==0){return pExtensionL;}
    std::vector<std::string> pExtensionS=SF::stringToVector(allD.first,"_n*_","_/n*_");
    long eSz=pExtensionS.size();
    if(eSz<1){return pExtensionL;}
    pExtensionL.resize(eSz);
    for(long i=0;i<eSz;++i){
      pExtensionL[i]=BF::stringToInteger(pExtensionS[i]);
    }
    return pExtensionL;
  }
  long applyPermutation(const std::string& sReplace, const long& _w){
    std::vector<long> pExt=identifyPermutationExtension(sReplace);
    long eSz=pExt.size();
    if(eSz<1){return _w;}
    if(_w<0){return 0;}
    return pExt[_w%eSz];
  }
  std::vector<long> identifyVersions(const std::string & _input){
    std::vector<long> pExtension=identifyPermutationExtension(_input);
    long eSz=pExtension.size();
    if(eSz>0){
      for(long i=0;i<eSz;++i){
        pExtension[i]=i;
      }
      return pExtension;
    }
    std::set<long> allVs;
    long pos=0;
    std::pair<std::string,int> allD=SF::extract(_input,pos,"_v*","_");
    while(allD.second==1){
      if(BF::isNumeric(allD.first,0)){
        allVs.insert(BF::stringToInteger(allD.first));
      }
      allD=SF::extract(_input,pos,"_v*","_");
    }
    std::set<long>::const_iterator it=allVs.begin(),itE=allVs.end();
    std::vector<long> aV;
    aV.resize(allVs.size());
    long i=0;
    while(it!=itE){
      aV[i]=*it;++i;++it;
    }
    return aV;
  }
  std::pair<std::string,std::string> getSidesOfTheAssignment(const std::string & _a){
    std::pair<std::string, std::string> res;
    res.first=""; res.second="";
    std::string a=_a+"/= jkl";
    long pos=0;
    std::pair<std::string,int> allD=SF::getEverythingBefore(a,pos,"=");
    if(allD.second==0){
      return res;
    }
    res.first=allD.first;
    res.second=SF::getEverythingBefore(a,pos,"/=").first;
    return res;
  }
  std::pair<std::string,std::string> findVarThatEvaluates(const std::map<SF::LSPair,std::string> & _m){
    std::pair<std::string,std::string> next;
    next.first="";next.second="";
    std::map<SF::LSPair,std::string>::const_iterator it=_m.begin(),itE=_m.end();
    std::pair<double,int> tempEval;
    while((next.first=="")&&(it!=itE)){
      tempEval=FF::evFormula(it->second);
      if(tempEval.second==1){
        next.first=(it->first).s;
        next.second=BF::doubleToString(tempEval.first);
      }
      ++it;
    }
    return next;
  }
  int isIndependent(const std::map<SF::LSPair,std::string> & _m, const std::string & _x){
    int ind=1;
    std::map<SF::LSPair,std::string>::const_iterator it=_m.begin(),itE=_m.end();
    long pos;
    std::pair<std::string,int> allD;
    while((ind==1)&&(it!=itE)){
      pos=0;
      allD=SF::getEverythingBefore(_x,pos,(it->first).s);
      if(allD.second==1){
        ind=0;
      }
      ++it;
    }
    return ind;
  }
  std::pair<std::string,std::string> findIndependentVariable(const std::map<SF::LSPair,std::string> &_m){
    std::pair<std::string,std::string> next;
    next.first="";next.second="";
    std::map<SF::LSPair,std::string>::const_iterator it=_m.begin(),itE=_m.end();
    while((next.first=="")&&(it!=itE)){
      if(isIndependent(_m,it->second)){
        next.first=(it->first).s;
        next.second=it->second;
      }
      ++it;
    }
    return next;
  }
  void performSearchReplace(std::map<SF::LSPair,std::string> & _m,
                            const std::string & searchFor, const std::string &replaceWith){
    _m.erase(SF::toLSPair(searchFor));
    std::map<SF::LSPair,std::string>::iterator it=_m.begin(),itE=_m.end();
    while(it!=itE){
      it->second= SF::findAndReplace(it->second,searchFor, replaceWith);
      ++it;
    }
  }
  std::vector<std::string> getAssignmentVector(const std::string & manyLinesSeparatedBySeparators){
    std::vector<std::string> fR;
    std::string separator="@@";
    std::set<std::string> s;
    std::string input=manyLinesSeparatedBySeparators+separator+GF::GL_HIDING_STRING_TWDVF+separator+GF::GL_HIDING_STRING_TWDVF+separator;
    std::string oneLine="";
    long pos;
    std::pair<std::string,int> allD;
    pos=0;
    while(oneLine!=GF::GL_HIDING_STRING_TWDVF){
      oneLine=eraseLeadingAndTrailingEmptyCharacters( SF::getEverythingBefore(input,pos,separator).first);
      if((oneLine!="")&&(oneLine!=GF::GL_HIDING_STRING_TWDVF)){
        s.insert(oneLine);
      }
    }
    long sz=s.size();
    fR.resize(sz);
    std::set<std::string>::const_iterator it=s.begin(),itE=s.end();
    for(long i=0;i<sz;++i){
      fR[i]=*it;
      ++it;
    }
    return fR;
  }
  std::vector<std::vector<std::string> > getSteganographyMatrix(const std::string& _in){
    std::vector<std::vector<std::string> > m;
    std::vector<std::string> raw=SF::stringToVector(_in,"_steg*|_","_/steg*|_");
    long mSz=raw.size();
    if(mSz<1){return m;}
    m.resize(mSz);
    for(long i=0;i<mSz;++i){
      m[i]=SF::stringToVector(raw[i],"_n*_","_/n*_");
    }
    return m;
  }
  std::vector<long> convertToSeqBase(const std::vector<long>& b, long x){
      std::vector<long> res;
      long sz=b.size();
      if(sz<1){return res;}
      long i=0;
      res.resize(sz);
      while(i<sz){
          res[i]=0;++i;
      }
      i=0;
      while((i<sz)&&(x>0)){
          res[i]= x % b[i];
          x/=b[i];
          ++i;
      }
      return res;
  }
  std::string applySteganography(const std::string& _in, const long& _w){
    std::vector<std::vector<std::string> > stegM=getSteganographyMatrix(_in);
    long len=stegM.size();
    if(len<1){return _in;}
    std::vector<long> base;
    base.resize(len);
    for(long i=0;i<len;++i){
      base[i]=stegM[i].size();
    }
    std::vector<long> conv=convertToSeqBase(base,_w);
    std::string out=_in;
    long pos; std::pair<std::string,int> allD;
    for(long i=0;i<len;++i){
      pos=0; allD=SF::extractAndReplace(out,pos,"_steg*|_","_/steg*|_",0,stegM[i][conv[i]]);
      if(allD.second==1){
        out=allD.first;
      }
    }
    return out;
  }
  std::string singleVersion(const std::string & _input, const long & _w){
    std::string s_B="_searchReplace_";
    std::string s_E="_/searchReplace_";
    std::string s_Bi,s_Ei;
    long pos=0;
    std::pair<std::string,int> allD=SF::extract(_input,pos,s_B,s_E);
    if(allD.second==0){
      return _input;
    }
    std::string searchReplace=applySteganography(allD.first,_w);
    long v=applyPermutation(searchReplace,_w);
    std::vector<std::string> srAllVersions,srMyVersion;
    s_Bi="_v*a_";s_Ei="_/v*a_";
    srAllVersions=SF::stringToVector(searchReplace,s_Bi,s_Ei);
    s_Bi="v*"+std::to_string(v)+"_";s_Ei="_/"+s_Bi;s_Bi="_"+s_Bi;
    srMyVersion=SF::stringToVector(searchReplace,s_Bi,s_Ei);
    std::map<SF::LSPair,std::string> allSearchReplacements;
    long sz,szv;
    std::pair<std::string,std::string> assignment;
    sz=srAllVersions.size();
    std::vector<std::string> assignmentVect;
    for(long i=0;i<sz;++i){
      assignmentVect=getAssignmentVector(srAllVersions[i]);
      szv=assignmentVect.size();
      for(long j=0;j<szv;++j){
        assignment=getSidesOfTheAssignment(assignmentVect[j]);
        allSearchReplacements[SF::toLSPair(assignment.first)]=assignment.second;
      }
    }
    sz=srMyVersion.size();
    for(long i=0;i<sz;++i){
      assignmentVect=getAssignmentVector(srMyVersion[i]);
      szv=assignmentVect.size();
      for(long j=0;j<szv;++j){
        assignment=getSidesOfTheAssignment(assignmentVect[j]);
        allSearchReplacements[SF::toLSPair(assignment.first)]=assignment.second;
      }
    }
    allD=SF::eraseStuffBetween(_input,s_B,s_E);
    std::string output=allD.first;
    // STEP 1: Formulas that can be evaluated
    std::pair<std::string,std::string> nextVar;
    nextVar.first=".";
    std::map<std::string,std::string> finalKRMap;
    while(nextVar.first!="") {
      nextVar=findVarThatEvaluates(allSearchReplacements);
      if(nextVar.first!=""){
        performSearchReplace(allSearchReplacements,nextVar.first,nextVar.second);
        finalKRMap[nextVar.first]=nextVar.second;
      }
    }
    // STEP 2: Formulas that cannot be evaluated
    nextVar.first=".";
    while(nextVar.first!=""){
      nextVar=findIndependentVariable(allSearchReplacements);
      if(nextVar.first!=""){
        performSearchReplace(allSearchReplacements,nextVar.first,nextVar.second);
        finalKRMap[nextVar.first]=nextVar.second;
      }
    }
    output=MFRF::findAndReplace(output,finalKRMap);
    return output;
  }
  std::string prepareProblemForTest(const std::string & _input, const long & _v){
    std::string output=singleVersion(_input,_v);
    long pos=0;
    std::pair<std::string,int> allD=SF::extract(output,pos,"_rbAll*|_", "_/rbAll*|_" );
    if(allD.second==1){
      std::string newStr="_rbAll*|_\n";
      std::vector<std::string> vectCh=SF::stringToVector(allD.first,"_rb*_","_/rb*_");
      std::vector<std::string> smst;
      std::sort(vectCh.begin(),vectCh.end());
      long sz=vectCh.size();
      for(long i=0;i<sz;++i){
        smst=SF::stringToVector(vectCh[i],"_vl*_", "_/vl*_");
        if(smst.size()>1){
          newStr+="_rb*_ _vl*_";
          newStr+=smst[0]+"_/vl*_ _vl*_<B>("+smst[0]+")</B> "+smst[1]+"_/vl*_ ";
          newStr+="_/rb*_\n";
        }
      }
      newStr+="_/rbAll*|_";
      pos=0;
      allD=SF::eraseStuffBetween(output,"_rbAll*|_", "_/rbAll*|_" );
      if(allD.second==1){
        output=allD.first+newStr;
      }
    }
    return output;
  }
}
#endif
