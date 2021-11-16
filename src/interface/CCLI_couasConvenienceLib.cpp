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


#ifndef _INCL_WI_COUAS_CONVENIENCE_LIB_CPP
#define _INCL_WI_COUAS_CONVENIENCE_LIB_CPP

namespace CCLI{
  std::string subtractStr(const std::string & b, const std::string & s){
    std::string res="";
    long sL=s.length();
    long bL=b.length();
    if(sL>bL){
      return "notFound";
    }
    long i=0;
    while( (i>0) && (i<sL) ){
      if(b[i]!=s[i]){
        i=-100;
      }
      ++i;
    }
    if(i<0){
      return "notFound";
    }
    while(i<bL){
      res+=b[i];
      ++i;
    }
    return res;
  }
  std::string assignmentNameToCode(const std::string &aN, const std::map<std::string,std::string> & conveniencePackage){
    std::map<std::string,std::string>::const_iterator it,itE;
    itE=conveniencePackage.end();
    it=conveniencePackage.find(aN);
    if(it==itE){
      return "notFound";
    }
    return it->second;
  }

  std::pair<std::string,long> extractLettersAndNumberFromString(const std::string &input){
    std::string outputSt="";
    long outputNum=0;
    long sz=input.length();
    long i=0;
    while(i<sz){
      if((input[i]>='0')&&(input[i]<='9')){
        outputNum *= 10;
        outputNum += static_cast<long>( (input[i]-'0') );
      }
      else{
        if( ((input[i]>='a')&&(input[i]<='z')) || ((input[i]>='A')&&(input[i]<='Z')) ){
          outputSt+=input[i];
        }
      }
      ++i;
    }
    return std::pair<std::string,long>(outputSt,outputNum);
  }
  struct ConveniencePackageForCouas{
  public:
    std::string preFill;
    std::string linkForSummary;
  };
  ConveniencePackageForCouas convenience_get(const std::string & mainDocNameForCourse,const std::string & mainRespRecName, const std::string & cElTitle){
    ConveniencePackageForCouas fR;
    fR.preFill="";
    fR.linkForSummary="notFound";
    std::map<std::string,std::string> convenienceMap;
    SF::varValPairs(MWII::GL_WI.getConveniencePackage(),"_vVPair_","_/vVPair_","_vr_","_/vr_","_vl_","_/vl_",convenienceMap);


    std::pair<std::string,long> nameNumPair=extractLettersAndNumberFromString(cElTitle);
    std::string aC=assignmentNameToCode(nameNumPair.first,convenienceMap);
    if(aC=="notFound"){
      return fR;
    }
    fR.preFill=mainDocNameForCourse;
    fR.preFill+=aC;
    std::string paddedNum;
    long aN=nameNumPair.second;
    if(aN>0){
      paddedNum=BF::padded(aN,10,"0");
      fR.preFill+=paddedNum;
    }

    fR.linkForSummary="index.cgi?page=GrMain";
    fR.linkForSummary+="&ev0=docName&el0="+mainDocNameForCourse;
    fR.linkForSummary+="&ev1=aType&el1="+nameNumPair.first;
    fR.linkForSummary+="&ev2=aNum&el2="+std::to_string(aN);
    fR.linkForSummary+="&ev3=grExt&el3="+aC;
    fR.linkForSummary+="&ev4=aNP&el4="+paddedNum;
    fR.linkForSummary+="&ev5=mRR&el5=";
    if(mainRespRecName!=""){
      fR.linkForSummary+=mainRespRecName;
    }
    else{
      fR.linkForSummary+="n";
    }

    return fR;
  }

}
#endif
