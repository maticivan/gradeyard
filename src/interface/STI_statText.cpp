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


#ifndef _INCL_WI_StatText_CPP
#define _INCL_WI_StatText_CPP
namespace STI{ 
  std::string rawStatFolder(){
    return DD::GL_DBS.getStatTable()+"/rawSt";
  }
  std::string countersFName(){
    std::string rawStatDataFolder=rawStatFolder();
    IOF::sys_createFolderIfDoesNotExist(rawStatDataFolder,"readme.txt","Do not edit this folder.","counters.txt","_n*_1_/n*_ _n*_0_/n*_ ");
    return rawStatDataFolder+"/counters.txt";
  }
  std::vector<std::string> statFileNames(const long &num){
    std::vector<std::string> fR;
    if(num<1){
      return fR;
    }
    fR.resize(num);
    std::string cntrfn=countersFName();
    std::pair<long,long> twoCntrs=SDIRF::getTwoCounters(cntrfn);
    long counterF=twoCntrs.first;
    std::string fnStart=rawStatFolder()+"/df";
    for(long i=0;i<num;++i){
      fR[i]=fnStart+BF::padded(counterF,SPREPF::STAT_CONSTS.paddingConst,"0")+".txt";
      --counterF;
      if(counterF<1){
        counterF=SPREPF::STAT_CONSTS.maxNumFiles;
      }
    }
    return fR;
  }
  std::string updateStatCountersAndGetDataFName(const std::string & cFName, long newCF,long newCI, long update){
    std::string rawStatDataFolder=rawStatFolder();
    std::string dataFileName=rawStatDataFolder+"/df"+BF::padded(newCF,SPREPF::STAT_CONSTS.paddingConst,"0")+".txt";
    if(update==0){
      return dataFileName;
    }
    ++newCI;
    if(newCI>SPREPF::STAT_CONSTS.maxItemsInFile){
      newCI=1;
      ++newCF;
      if(newCF>SPREPF::STAT_CONSTS.maxNumFiles){newCF=1;}
      dataFileName=rawStatDataFolder+"/df"+BF::padded(newCF,SPREPF::STAT_CONSTS.paddingConst,"0")+".txt";
      IOF::toFile(dataFileName,"");
    }
    IOF::toFile(cFName,"_n*_"+std::to_string(newCF)+"_/n*_ _n*_"+std::to_string(newCI)+"_/n*_ ");
    return dataFileName;
  }
  std::string getStatDataFileName(const long & updateCounters=0){
    std::string cFName =countersFName();
    long newCF,newCI;
    std::pair<long,long> cfi=SDIRF::getTwoCounters(cFName);
    newCF=cfi.first;newCI=cfi.second;
    if(updateCounters==1){
      return updateStatCountersAndGetDataFName(cFName,newCF,newCI,1);
    }
    return updateStatCountersAndGetDataFName(cFName,newCF,newCI,0);
  }
  std::string addRawDataToStat(const SPREPF::StatData &st){
    std::string dataFileName=getStatDataFileName(1);
    std::string st_Data=IOF::fileToString(dataFileName);
    if(st_Data=="fileNotFound"){
      st_Data="";
    }
    st_Data+="_nD*_"+st.putIntoString()+"_/nD*_\n";
    IOF::toFile(dataFileName,st_Data);
    return "success";
  }
  std::string addDataToStat(const SPREPF::StatData &st){
    SDIRF::updateStatDataForRam(st,DD::GL_DBS.getStatTable()); 
    std::string fR=addRawDataToStat(st);
    return fR;
  }
  std::vector<SPREPF::StatData> getStatData(const long & numF){
    std::vector<std::string> statFNames=statFileNames(numF);
    std::stack<SPREPF::StatData> fSt; SPREPF::StatData temp;
    std::vector<std::string> singleFileRawData;
    long szI;
    long i=numF;
    while(i>0){
      --i;
      singleFileRawData=SF::stringToVector(IOF::fileToString(statFNames[i]),SPREPF::STAT_CONSTS.sepStat_NRDB,SPREPF::STAT_CONSTS.sepStat_NRDE);
      szI=singleFileRawData.size();
      for(long j=0;j<szI;++j){
        temp.setFromString(singleFileRawData[j]);
        fSt.push(temp);
      }
    }
    SF::flipTheStack(fSt);
    return SF::stackToVector(fSt);
  }
  std::stack<SPREPF::StatData> statDataForSingleIPAndPage(const std::vector<SPREPF::StatData> & _input, const SPREPF::StatData & st_D, const std::string & _pg){
    std::stack<SPREPF::StatData> fRS;
    long sz=_input.size();
    long DEB_succ=0, DEB_fail=0;
    for(long i=0;i<sz;++i){
      if(_input[i].ipAddr==st_D.ipAddr){
        if(_input[i].att_page==_pg){
          fRS.push(_input[i]);
        }
      }
    }
    SF::flipTheStack(fRS);
    return fRS;
  }
  std::string vectLongToString(const std::vector<long> &v){
    std::string fR;
    long sz=v.size();
    for(long i=0;i<sz;++i){
      fR+=" "+std::to_string(v[i]);
    }
    return fR;
  }
  long secondsSinceLastVisit(const SPREPF::StatData & ist_D, const std::string & pageName){
    std::vector<SPREPF::StatData> v_D=getStatData(SPREPF::STAT_CONSTS.numFilesToLookForSpammerBehavior);
    std::stack<SPREPF::StatData> st_DR=statDataForSingleIPAndPage(v_D,ist_D,pageName);
    long secBetween=999999;
    if(st_DR.size()>1){
      SPREPF::StatData last=st_DR.top();
      st_DR.pop();
      SPREPF::StatData prev=st_DR.top();
      secBetween=TMF::getSecondsSinceYYYY(TMF::stGMT_to_timeVectorTMF(last.timeString))-TMF::getSecondsSinceYYYY(TMF::stGMT_to_timeVectorTMF(prev.timeString));
    }
    return secBetween;
  }
  std::string checkIfSpammerIsTryingToGuessPasswords(const SPREPF::StatData & ist_D){
    std::vector<SPREPF::StatData> v_D=getStatData(SPREPF::STAT_CONSTS.numFilesToLookForSpammerBehavior);
    std::stack<SPREPF::StatData> st_DR=statDataForSingleIPAndPage(v_D,ist_D,"!*LOGIN*ATTEMPT*!");
    std::vector<long> timeVect;
    long secBetween;
    if(st_DR.size()>SPREPF::STAT_CONSTS.numLoginAttemptsToStartCollectingData){
      SPREPF::StatData dataCollector=ist_D;
      if(dataCollector.pass1!=""){
        dataCollector.att_rr="psswdAtt";
      }
      addRawDataToStat(dataCollector);
    }
    long cutoff=SPREPF::STAT_CONSTS.numLoginAttemptsToGetSuspcious;
    if(cutoff<3){cutoff=3;}
    if(st_DR.size()>cutoff){
      SPREPF::StatData last=st_DR.top();
      st_DR.pop();
      SPREPF::StatData prev=st_DR.top();
      st_DR.pop();
      SPREPF::StatData p_prev=st_DR.top();
      st_DR.pop();
      SPREPF::StatData pp_prev=st_DR.top();
      st_DR.push(p_prev);
      st_DR.push(prev);
      st_DR.push(last);
      secBetween=TMF::getSecondsSinceYYYY(TMF::stGMT_to_timeVectorTMF(last.timeString))-TMF::getSecondsSinceYYYY(TMF::stGMT_to_timeVectorTMF(pp_prev.timeString));
      if(secBetween<SPREPF::STAT_CONSTS.cutoff_in_seconds_ForStalling){
        return "spammer";
      }
    }
    return "ok";
  }
  std::string hidePageFromThoseWhoAreTryingToGuessPasswords(const std::string & _input,const SPREPF::StatData & ist_D){
    if((_input!="login")&&(_input!="registration")){
      return _input;
    }
    if(ist_D.userName!="visitor"){
      return _input;
    }
    std::vector<SPREPF::StatData> v_D=getStatData(SPREPF::STAT_CONSTS.numFilesToLookForSpammerBehavior);
    std::stack<SPREPF::StatData> st_DR=statDataForSingleIPAndPage(v_D,ist_D,_input);
    std::vector<long> timeVect;
    long secBetween;
    if(st_DR.size()>SPREPF::STAT_CONSTS.numLoginAttemptsToGetSuspcious){
      SPREPF::StatData last=st_DR.top();
      st_DR.pop();
      SPREPF::StatData prev=st_DR.top();
      st_DR.push(last);
      secBetween=TMF::getSecondsSinceYYYY(TMF::stGMT_to_timeVectorTMF(last.timeString))-TMF::getSecondsSinceYYYY(TMF::stGMT_to_timeVectorTMF(prev.timeString));
      if(secBetween<SPREPF::STAT_CONSTS.cutoff_in_seconds_ForStalling){
        return "tooManyAttemptsInShortTime";
      }
    }
    return _input;
  }
  std::string updateRawSt(const std::string & rawSt,const std::string &cat, const std::string &name, const long &max){
    SPREPF::StatPeriod sp(cat,max);
    sp.setRawStringAndUpdate(rawSt,name);
    sp.increaseCounter();
    sp.updateRawString();
    return sp.getRawString();
  } 
  std::map<std::string,long> addMaps(const std::map<std::string, long> & m1, const std::map<std::string,long> &m2){
    std::map<std::string,long> m3=m1;
    std::map<std::string,long>::iterator itM,itME;
    std::map<std::string,long>::const_iterator it,itE;
    it=m2.cbegin(); itE=m2.cend();
    while(it!=itE){
      itM=m3.find(it->first);
      if(itM==m3.end()){
        m3[it->first]=it->second;
      }
      else{
        (itM->second) += it->second;
      }
      ++it;
    }
    return m3;
  }
  std::string threeMapsIntoTable(const std::map<std::string, long> & m1, const std::map<std::string,long> &m2, const std::map<std::string,long> &m3){
    std::map<std::string,long>::const_iterator it,itE,itM;
    long lsz=4;
    std::stack<std::vector<std::string> > allLines;
    std::vector<std::string> topLine,mLine;
    topLine.resize(lsz);mLine.resize(lsz);
    topLine[0]="Date";
    topLine[1]="M";
    topLine[2]="V";
    topLine[3]="T";
    long mL,vL;
    it=m3.cbegin();
    itE=m3.cend();
    while(it!=itE){
      mLine[0]=it->first;
      mL=0;vL=0;
      itM=m1.find(it->first);
      if(itM!=m1.cend()){
        mL=itM->second;
      }
      itM=m2.find(it->first);
      if(itM!=m2.cend()){
        vL=itM->second;
      }
      mLine[1]=std::to_string(mL);
      mLine[2]=std::to_string(vL);
      mLine[3]=std::to_string(mL+vL);
      allLines.push(mLine);
      ++it;
    }
    if(mLine[0].length()<7){topLine[0]="Month";}
    if(mLine[0].length()<5){topLine[0]="Year";}
    allLines.push(topLine);
    return HSF::tableFromStack(allLines,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
  }
  std::string mapsIntoTable(const std::map<std::string, long> & m1, const std::map<std::string,long> &m2){
    return threeMapsIntoTable(m1,m2,addMaps(m1,m2));
  }
  std::map<std::string,long> catToMap(const std::string & rawSt,const std::string &cat,const long &max){
    std::map<std::string,long> res;
    SPREPF::StatPeriod sp(cat,max);
    sp.setRawString(rawSt);
    std::vector<SPREPF::StatAtom> stData=sp.statSummary();
    for(long i=0;i<stData.size();++i){
      if(stData[i].name!="NA"){res[stData[i].name]=stData[i].visits;}
    }
    return res;
  }
  long getTotalVisits(const std::string &raw, const std::string & tp){
    long pos;std::vector<SPREPF::StatAtom> stData;
    SPREPF::StatPeriod spM(tp, GF::GLOBAL_MAX_YEARS_ST);
    spM.setRawString(raw);
    stData=spM.statSummary(1);
    long vML=0;
    if(stData.size()>0){
      vML=stData[0].visits;
    }
    return vML;
  }
}

#endif
