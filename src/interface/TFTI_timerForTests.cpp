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


#ifndef _INCL_TFTI_TimerForTests_CPP
#define _INCL_TFTI_TimerForTests_CPP

namespace TFTI{
  long convertStringToSeconds(const std::string &st){
    std::string repl=st;
    std::map<std::string,std::string> replMap;
    replMap["years"]="y";
    replMap["yr"]="y";
    replMap["months"]="z";
    replMap["month"]="z";
    replMap["mnth"]="z";
    replMap["days"]="d";
    replMap["hours"]="h";
    replMap["minutes"]="m";
    replMap["seconds"]="s";
    replMap[","]="";
    replMap[" "]="";
    replMap["\t"]="";
    replMap["\n"]="";
    replMap["and"]="";
    repl=MFRF::findAndReplace(repl,replMap);
    replMap.clear();
    replMap["year"]="y";
    replMap["mon"]="z";
    replMap["mnt"]="z";
    replMap["day"]="d";
    replMap["hour"]="h";
    replMap["minute"]="m";
    replMap["second"]="s";
    repl=MFRF::findAndReplace(repl,replMap);
    replMap.clear();
    replMap["mn"]="z";
    replMap["min"]="m";
    replMap["sec"]="s";
    repl=MFRF::findAndReplace(repl,replMap); 
    std::pair<std::string,int> allD;long pos;long oldPos;
    long yr=0;long mn=0;long dy=0;long h=0;long m=0;long s=0;
    oldPos=0;pos=0;allD=SF::getEverythingBefore(repl,pos,"y");
    repl+="s";
    if(allD.second==1){
      yr=BF::stringToInteger(allD.first);
    }
    else{
      pos=oldPos;
    }
    oldPos=pos;allD=SF::getEverythingBefore(repl,pos,"z");
    if(allD.second==1){
      mn=BF::stringToInteger(allD.first);
    }
    else{
      pos=oldPos;
    }
    oldPos=pos;allD=SF::getEverythingBefore(repl,pos,"d");
    if(allD.second==1){
      dy=BF::stringToInteger(allD.first);
    }
    else{
      pos=oldPos;
    }
    oldPos=pos;allD=SF::getEverythingBefore(repl,pos,"h");
    if(allD.second==1){
      h=BF::stringToInteger(allD.first);
    }
    else{
      pos=oldPos;
    }
    oldPos=pos;allD=SF::getEverythingBefore(repl,pos,"m");
    if(allD.second==1){
      m=BF::stringToInteger(allD.first);
    }
    else{
      pos=oldPos;
    }
    oldPos=pos;allD=SF::getEverythingBefore(repl,pos,"s");
    if(allD.second==1){
      s=BF::stringToInteger(allD.first);
    }
    long result=0;
    result+=yr;
    result*=12;
    result+=mn;
    result*=31;
    result+=dy;
    result*=24;
    result+=h;
    result*=60;
    result+=m;
    result*=60;
    result+=s;
    return result;
  }
  RTI::ResponderInfo analyzeTimerAndUpdateInfo(const RTI::ResponderInfo & rInf){
    RTI::ResponderInfo res=rInf;
    if(res.acceptResp==1){
      std::vector<long> aEndVect=TMF::stGMT_to_timeVectorTMF(res.commAbsoluteEnd);
      if(res.indTimerVersion!=res.commTimerVersion){
        res.indEndingTime="notFound";
      }
      std::vector<long> iEndVect=TMF::stGMT_to_timeVectorTMF(res.indEndingTime);
      TMF::Timer tmpTm;
      std::vector<long> tNowVect=tmpTm.timeNowVector();
      long allowedTimeSec=convertStringToSeconds(res.commAllowedTime);
      //Checking if the exam is over
      long i_endTimeSec=-1;
      if(res.indEndingTime!="notFound"){
        i_endTimeSec=TMF::getSecondsSinceYYYY(iEndVect);
      }
      long c_endTimeSec=-1;
      if(res.commAbsoluteEnd!="notFound"){
        c_endTimeSec=TMF::getSecondsSinceYYYY(aEndVect);
      }
      long tmNowSec=TMF::getSecondsSinceYYYY(tNowVect);
      if(i_endTimeSec>-1){
        if(tmNowSec > i_endTimeSec){
          res.acceptResp=0;
        }
      }
      if(c_endTimeSec>-1){
        if(tmNowSec > c_endTimeSec){
          res.acceptResp=0;
        }
      }
      if(res.acceptResp==1){
        //Checking whether the exam is inactive and whether the user is allowed to activate it
        if(i_endTimeSec<0){
          if(c_endTimeSec > -1){
            if( tmNowSec+allowedTimeSec < c_endTimeSec){
              res.userCanTakeActionAndActivateExam=1;
              std::vector<long> allTSV;
              allTSV.resize(6);
              for(long i=0;i<allTSV.size();++i){allTSV[i]=0;}
              allTSV[5]=allowedTimeSec+res.roundingTimeSec;
              i_endTimeSec=tmNowSec+allowedTimeSec+res.roundingTimeSec;
              res.potentialEndingTimeIfExamStartsNow=tmpTm.timeString(tmpTm.addTime(tNowVect,allTSV));
            }
          }
        }
      }
      if(i_endTimeSec > -1){
        if( i_endTimeSec < tmNowSec){
          res.remainingSeconds=0;
        }
        else{
          res.remainingSeconds = i_endTimeSec - res.roundingTimeSec-tmNowSec;
          if(res.remainingSeconds<0){
            res.remainingSeconds=0;
          }
        }
      }
      else{
        if(c_endTimeSec < 0 ){
          res.remainingSeconds=-1;
        }
        else{
          if(c_endTimeSec < tmNowSec){
            res.remainingSeconds=0;
          }
          else{
            res.remainingSeconds = c_endTimeSec - res.roundingTimeSec - tmNowSec;
            if(res.remainingSeconds<0){
              res.remainingSeconds=0;
            }
          }
        }
      }
    }
    return res;
  }
}
#endif
