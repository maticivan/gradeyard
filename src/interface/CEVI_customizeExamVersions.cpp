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


#ifndef _INCL_CEVI_CustomizeExamVersions_CPP
#define _INCL_CEVI_CustomizeExamVersions_CPP

namespace CEVI{
  std::string lVB="_listVersions*|_";
  std::string lVE="_/listVersions*|_";
  std::string stCB="_stCustomizations*|_";
  std::string stCE="_/stCustomizations*|_";
  std::string vnmB="_vnm*_";
  std::string vnmE="_/vnm*_";
  std::string vtB="_vt*_";
  std::string vtE="_/vt*_";
  std::string unB="_un*_";
  std::string unE="_/un*_";
  std::string ansB="_ans*_";
  std::string ansE="_/ans*_";
  struct StudentAnswRRUpdateData{
  public:
    std::map<std::string,std::string> uNamesToVersions;
    std::map<std::string,std::string> uNamesToAnswers;
  };
  StudentAnswRRUpdateData createFromString(const std::string & _raw){
      StudentAnswRRUpdateData res;
      long pos;
      std::pair<std::string,long> allDV,allDS;
      pos=0;allDV=SF::extract(_raw,pos,lVB,lVE);
      pos=0;allDS=SF::extract(_raw,pos,stCB,stCE);
      if((allDV.second==0)||(allDS.second==0)){
        return res;
      }
      std::map<std::string,std::string> vRawMap=SF::stringToMap(allDV.first,vnmB,vnmE,vtB,vtE);
      std::map<std::string,std::string>::const_iterator it,itE;
      itE=vRawMap.end();
      it=vRawMap.begin();
      std::string rawStData=allDS.first;
      while(it!=itE){
        rawStData=SF::findAndReplace(rawStData,vnmB+it->first+vnmE,vtB+it->second+vtE);
        ++it;
      }
      res.uNamesToVersions=SF::stringToMap(rawStData,unB,unE,vtB,vtE);
      res.uNamesToAnswers=SF::stringToMap(rawStData,unB,unE,ansB,ansE);
      return res;
  }
  std::string updateVersionsForStudent(const std::string &input, const std::string &nVersions, const std::vector<std::string> &lb){
    std::string output=input;
    long sz=lb.size();
    long posO,posN;
    std::pair<std::string,int> allDO,allDN;
    std::string sB,sE;
    for(long i=0;i<sz;++i){
      sB=lb[i]+"*|_";
      sE="_/v"+sB;
      sB="_v"+sB;
      posO=0;allDO=SF::extract(output,posO,sB,sE);
      posN=0;allDN=SF::extract(nVersions,posN,sB,sE);
      if((allDO.second==1)&&(allDN.second==1)){
        output=SF::findAndReplace(output,sB+allDO.first+sE,sB+allDN.first+sE);
      }
    }
    return output;
  }
  int isCertificate(const std::string& s){
    if((s.size()>1)&&(s[0]=='Q')&&(s[1]=='C')){
      return 1;
    }
    return 0;
  }
  std::vector<std::string> removeCertificateIfPresent(const std::vector<std::string> &v){
    std::stack<std::string> g;
    long sz=v.size();
    for(long i=0;i<sz;++i){
      if(!isCertificate(v[i])){
        g.push(v[i]);
      }
    }
    return SF::stackToVector(g);
  }
  std::string debAnalysis(const std::vector<std::string> & a, const std::vector<std::string> &b){
    std::string res;
    res+=std::to_string(a.size())+" "+std::to_string(b.size())+"\n";
    for(long i=0;i<a.size();++i){
      res+=a[i]+"\t";
    }
    res+="\n";
    for(long i=0;i<b.size();++i){
      res+=b[i]+"\t";
    }
    res+="\n";
    return res;
  }
  std::map<std::string,std::string> getAnswMapFromVector(const std::vector<std::string> & v){
    std::map<std::string,std::string> resM;
    long sz=v.size();
    std::pair<std::string,int>allD; long pos;
    for(long i=0;i<sz;++i){
      pos=0;allD=SF::extract(v[i],pos,"_lb*|_","_/lb*|_");
      if(allD.second==1){
        resM[allD.first]=v[i];
      }
    }
    return resM;
  }
  void updateOneAnswer(const std::map<std::string,std::string>& mLabelRawSt, std::string& rrText,const std::string& label, const std::string& newAnsw){
    std::string toAdd="_in*|_ _aw*|_"+newAnsw+"_/aw*|_ _lb*|_"+label+"_/lb*|_ _/in*|_";
    std::map<std::string,std::string>::const_iterator it,itE;
    itE=mLabelRawSt.end();
    it=mLabelRawSt.find(label);
    if(it==itE){
      rrText=SF::findAndReplace(rrText,"_/textData!!_",toAdd+"_/textData!!_");
    }
    else{
      rrText=SF::findAndReplace(rrText,it->second,toAdd);
    }
  }
  std::string updateAnswersForStudent(const std::string &input, const std::string &_nAnswers, const std::vector<std::string> &_lb){
    std::string output=input;
    std::vector<std::string> lb=removeCertificateIfPresent(_lb);
    long sz=lb.size();
    std::string saB="_n*_", saE="_/n*_";
    std::string nAnsw=SF::findAndReplace(_nAnswers,";",saE+saB);
    nAnsw=saB+nAnsw;
    nAnsw+=saE;
    std::vector<std::string> answVect=SF::stringToVector(nAnsw,saB,saE); 
    long posO,posN;
    std::pair<std::string,int> allDO,allDN;
    if(answVect.size()<sz){
      sz=answVect.size();
    }
    std::vector<std::string> vectOldAnswers=SF::stringToVector(output,"_in*|_","_/in*|_");
    std::map<std::string,std::string> mapOldAnswRaw=getAnswMapFromVector(vectOldAnswers);
    for(long i=0;i<sz;++i){
      if(answVect[i]!=""){
        updateOneAnswer(mapOldAnswRaw,output,lb[i],answVect[i]);
      }
    }
    return output;
  }
}

#endif
