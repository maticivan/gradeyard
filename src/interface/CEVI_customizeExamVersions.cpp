//    GradeYard learning management system
//
//    Copyright (C) 2025 Ivan Matic, https://gradeyard.com
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
  std::string nDB="_noDisplayProblems|_";
  std::string nDE="_/noDisplayProblems|_";
  std::string lVB="_listVersions*|_";
  std::string lVE="_/listVersions*|_";
  std::string stCB="_stCustomizations*|_";
  std::string stCE="_/stCustomizations*|_";
  std::string eRAB="_eRFA*|_";
  std::string eRAE="_/eRFA*|_";
  std::string vnmB="_vnm*_";
  std::string vnmE="_/vnm*_";
  std::string vtB="_vt*_";
  std::string vtE="_/vt*_";
  std::string unB="_un*_";
  std::string unE="_/un*_";
  std::string ansB="_ans*_";
  std::string ansE="_/ans*_";
std::string eraseRules(const std::string& _in){
    if(_in.size()<1){ 
      return "";
    } 
    std::map<std::string,std::string> replMap;
    replMap["[rc]"]="[r]";
    replMap["[rw]"]="[r]";
    replMap["[/rc]"]="[/r]";
    replMap["[/rw]"]="[/r]";
    std::string out=MFRF::findAndReplace(_in,replMap);
    std::set<std::string> allRules=SF::stringToSet(out,"[r]","[/r]");
    replMap.clear();
    std::set<std::string>::const_iterator itS=allRules.begin();
    while(itS!=allRules.end()){
        replMap["[r]"+*itS+"[/r]"]="";
        ++itS;
    }
    out=MFRF::findAndReplace(out,replMap);
    if(out.size()<1){out="(no submission)";}
    std::vector<std::string> ins=SF::stringToVectorSimpleSeparator(out,";");
    out="";
    for(long i=0;i<ins.size();++i){
      if(i>0){
        out+=";";
      }
      out+=ins[i];
      if(ins[i].size()<1){out+=" ";}
    } 
    return out;
}
std::map<std::string,std::string> eraseRules(const std::map<std::string,std::string>& _in){
    std::map<std::string,std::string> out;
    std::map<std::string,std::string>::const_iterator it=_in.begin();
    while(it!=_in.end()){
        out[it->first]=eraseRules(it->second);
        ++it;
    }
    return out;
}
  struct StudentAnswRRUpdateData{
  public:
    std::map<std::string,std::string> uNamesToVersions;
    std::map<std::string,std::string> uNamesToAnswers;
  };
  std::string wrapperString(const std::string& start, const std::string & end, long j){
    std::string res=start;
    if(j<10){res+="0";}
    res+=std::to_string(j);
    res+=end;
    return res;
  }
  std::string trivialVersions(long numVersions, long _numProblems){
    std::string fR="";
    long numProblems=_numProblems+1;
    for(long i=0;i<numVersions;++i){
      fR+="_n*_\n_vnm*_"+std::to_string(i)+"_/vnm*_\n_vt*_\n";
      for(long j=1;j<numProblems%100;++j){
        fR+=wrapperString("_vQ","*|_",j)+std::to_string(i)+wrapperString("_/vQ","*|_",j)+"\n";
      }
      fR+="_/vt*_\n_/n*_\n";
    }
    return fR;
  }
  std::string placeHoldersForNDP(const std::string& rawNDP){
    std::vector<std::string> allVersions=SF::stringToVector(rawNDP,"_n*_","_/n*_");
    std::vector<std::string> allPoints=SF::stringToVector(rawNDP,"_pt*|_","_/pt*|_");
    std::vector<std::vector<std::string> > probMatrix;
    long numRows=allVersions.size();
    if(numRows<1){return "";}
    probMatrix.resize(numRows);
    long numProblems=0;
    for(long i=0;i<numRows;++i){
      probMatrix[i]=SF::stringToVectorSimpleSeparator(allVersions[i]);
      if(probMatrix[i].size()>numProblems){
        numProblems=probMatrix[i].size();
      }
    }
    std::vector<std::string> points;
    if(allPoints.size()==numProblems){
      points=std::move(allPoints);
    }
    else{
      if(allPoints.size()<1){
        points.resize(1);points[0]="1";
      }
      else{
        if(allPoints.size()==1){
          points=SF::stringToVectorSimpleSeparator(allPoints[0]);
        }
        else{
          points=std::move(allPoints);
        }
      }
      if(points.size()<numProblems){
        long oldSize=points.size();
        std::vector<std::string> ptsNew;
        ptsNew.resize(numProblems);
        for(long i=0;i<numProblems;++i){
          if(i<oldSize){
            ptsNew[i]=points[i];
          }
          else{
            ptsNew[i]=points[oldSize-1];
          }
        }
        points=std::move(ptsNew);
      }
    }
    std::string res;
    for(long i=0;i<numProblems;++i){
      res+="\n_in*|_\n_p*|_\n";
      res+="_tx*|_N/A for problem"+std::to_string(i+1)+"_/tx*|_\n";
      res+="_sl*|_N/A for problem"+std::to_string(i+1)+"_/sl*|_\n";
      res+="_aw*|_{answer}_/aw*|_\n_rq*|_textInputField_/rq*|_\n_searchReplace_\n";
      res+="_v*a_\n{alpha}=1@@\n_/v*a_\n";
      for(long row=0;row<numRows;++row){
        res+="_v*"+std::to_string(row)+"_\n";
        res+="{answer}=";
        if(i<probMatrix[row].size()){
          res+=probMatrix[row][i]+"@@\n";
        }
        else{
          res+="na@@\n";
        }
        res+="_/v*"+std::to_string(row)+"_\n";
      }
      res+="_/searchReplace_\n";
      res+="_/p*|_\n";
      res+="_pt*|_"+points[i]+"_/pt*|_\n";
      res+="_/in*|_\n";
    }
    return res;
  }
  std::string createPlaceHoldersForProblemsWithNoDisplays(const std::string& in){
    long pos; std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(in,pos,nDB,nDE);
    if(allD.second==0){
      return in;
    }
    pos=0;allD=SF::extractAndReplace(in,pos,nDB,nDE,0,placeHoldersForNDP(allD.first));
    if(allD.second==0){
      return in;
    }
    return allD.first;
  }
  StudentAnswRRUpdateData createFromString(const long& numProblems, const long& numStudents, const std::string & _raw){
      StudentAnswRRUpdateData res;
      long pos;
      std::pair<std::string,long> allDV,allDS, allDERA;
      int indEraseRules=0;
      pos=0;allDV=SF::extract(_raw,pos,lVB,lVE);
      pos=0;allDS=SF::extract(_raw,pos,stCB,stCE);
      pos=0;allDERA=SF::extract(_raw,pos,eRAB,eRAE);
      if((allDV.second==0)||((allDV.first).length()<15)){
        allDV.first=trivialVersions(numStudents,numProblems);
        allDV.second=1;
      }
      if((allDV.second==0)||(allDS.second==0)){
        return res;
      }
      if(allDERA.second==1){
          if((allDERA.first).length()>0){
              if( ((allDERA.first)[0]=='y') || ((allDERA.first)[0]=='Y') || ((allDERA.first)[0]=='1') ){
                  indEraseRules=1;
              }
          }
      }
      std::map<std::string,std::string> vRawMap=SF::stringToMap(allDV.first,vnmB,vnmE,vtB,vtE);
      std::map<std::string,std::string>::const_iterator it,itE;
      itE=vRawMap.end();
      it=vRawMap.begin();
      std::string rawStData=allDS.first;
      std::map<std::string,std::string> replMap;
      while(it!=itE){
        replMap[vnmB+it->first+vnmE]=vtB+it->second+vtE;
        ++it;
      }
      rawStData=MFRF::findAndReplace(rawStData,replMap);
      res.uNamesToVersions=SF::stringToMap(rawStData,unB,unE,vtB,vtE);
      res.uNamesToAnswers=SF::stringToMap(rawStData,unB,unE,ansB,ansE);
      if(indEraseRules){
          res.uNamesToAnswers=eraseRules(res.uNamesToAnswers);
      }
      return res;
  }
  std::string updateVersionsForStudent(const std::string &input, const std::string &nVersions, const std::vector<std::string> &lb){
    std::string output=input;
    long sz=lb.size();
    long posO,posN;
    std::pair<std::string,int> allDO,allDN;
    std::string sB,sE;
    std::map<std::string,std::string> replMap;
    for(long i=0;i<sz;++i){
      sB=lb[i]+"*|_";
      sE="_/v"+sB;
      sB="_v"+sB;
      posO=0;allDO=SF::extract(output,posO,sB,sE);
      posN=0;allDN=SF::extract(nVersions,posN,sB,sE);
      if((allDO.second==1)&&(allDN.second==1)){
        replMap[sB+allDO.first+sE]=sB+allDN.first+sE;
      }
    }
    output=MFRF::findAndReplace(output,replMap);
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
    std::vector<std::string> answVect=SF::stringToVectorSimpleSeparator(_nAnswers);
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
