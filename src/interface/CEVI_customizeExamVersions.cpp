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


  std::map<std::string,std::string> createFromString(const std::string & _raw){
     long pos;
     std::map<std::string,std::string>emptyMap;
     std::pair<std::string,long> allDV,allDS;
     pos=0;allDV=SF::extract(_raw,pos,lVB,lVE);
     pos=0;allDS=SF::extract(_raw,pos,stCB,stCE);
     if((allDV.second==0)||(allDS.second==0)){
       return emptyMap;
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
     return SF::stringToMap(rawStData,unB,unE,vtB,vtE);
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
}

#endif
