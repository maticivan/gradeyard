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

#ifndef _INCL_MYMAINTABLE_CPP
#define _INCL_MYMAINTABLE_CPP


#include "PTKF_plainTextKeeper.cpp"
#include "DISPPF_displayPreparations.cpp"
#include "SACF_sanitize_and_check.cpp"
#include "AF_arithmeticCenter.cpp"
#include "SICF_simpleIntegralCenter.cpp"
#include "FF_formulaCenter.cpp"
#include "AEF_answerExtraction.cpp"
#include "TWDVF_textsWithDifferentVersions.cpp"
#include "HDPF_pointers.cpp"
#include "HDDBSF_HDDBString.cpp"
#include "SETF_setB.cpp"
#include "ATF_atomicTable.cpp"
#include "FUTF_fastUpdatingTable.cpp"
#include "TOSF_tOfSets.cpp"
#include "LANGF_languageFunctions.cpp"
#include "QAF_queryAnalysis.cpp"
namespace MTF{
  void nanoSleep(const long & tm){
    timespec timeOut,remains;
    timeOut.tv_sec = 0;
    timeOut.tv_nsec = tm;//500000000; /* 50 milliseconds */
    nanosleep(&timeOut, &remains);
  }
  class StringWithCounter{
  public:
    std::string s;
    long counter;
    long operator<(const StringWithCounter & ) const;
  };
  long StringWithCounter::operator<(const StringWithCounter & z) const{
    if(counter<z.counter){return 1;}
    if(counter>z.counter){return 0;}
    if(s<z.s){return 1;}
    return 0;
  }
  class Table{
  private:
      long timeToWaitBeforeDecidingThatPreviousProcessIsStuckAndNeedsToBeKilled=10;
      long timeToSleepBeforeAttemptingToClearTheQueueAgain=50000000;
      std::string stFN;
      std::vector<std::string> keyNames;
      std::string mainDataName;
      std::string tableName;
      std::map<std::string,long> keyNameToVectInd;
      void reCreateSearchMapForKeys();
      std::set<std::string> indexNames;
      std::string stFolderIndex="inds";
      std::string tInitFName="tableInit.txt";
      std::string tCKIFName="tCKIInit.txt";
      std::string indFName="inInd.txt";
      std::string folderInstQueue="instQueue";
      std::string extForCommandsInQueue="cmq";
      std::string extForFlag="flg";
      std::string stFolderData;
      std::string tableFolder;
      std::set<long> setKeyForIndexFromStr(const std::string &) const;
      std::vector<long> vectKeyForIndexFromStr(const std::string &) const;
      std::string indNameFromVectKeys(const std::vector<long> & ) const;
      std::string strFromVectKeys(const std::vector<long> &) const;
      std::vector<long> keyNamesToInds(const std::vector<std::string> &) const;
      ATF::Table getCombKey() const;
      int updateIndexWithEntry(const std::string &, const std::vector<std::string> &, const long &);
      std::string combKeyToString(const std::vector<std::string> &) const;
      std::set<StringWithCounter> stringToCombKeySet(const std::string &) const;
      std::vector<std::string> stringToCombKey(const std::string &) const;
      std::string createFileName(const std::string &,const std::string &) const;
      std::string prepareKeyString(const std::vector<std::string> &) const;
      std::vector<std::string> invertKeyString(const std::string &) const;
      std::string prepareDBRecString(const std::vector<std::string> &, const std::string &) const;
      std::pair<std::vector<std::string>,std::string> invertDBRecString(const std::string &) const;
    int insertUnsafe(const std::vector<std::string> &, const std::string &);
    // unsafe insert may end with memory race
    // This method is private for a reason. It should be called only by methods that can
    // guarantee the safety
    int delRowUnsafe(const std::vector<std::string> &);
    // unsafe delRow may end with memory race
    // This method is private for a reason. It should be called only by methods that can
    // guarantee the safety
  public:
      Table(const std::string & ="notSet!***!", const std::string & ="defaultTable");
      std::vector<std::string> getKeyNames() const;
      std::string getMainDataName() const;
      std::string getTableName() const;
      std::string getStartingFolderName() const;
      int createIndex(const std::vector<std::string> &, const int & =0);
      int createIndex(const std::vector<long> &, const int & =0);
      int deleteIndex(const std::vector<std::string> &);
      int deleteIndex(const std::vector<long> &);
      void setKeyNames(const std::vector<std::string> &);
      void setMainDataName(const std::string &);
      int setFolderNames(const std::string &, const std::string & = "no");
      // returns 1 if the table is created for the first time
      // If the second argument is "yes" then the function will trust that all folders are created
      //    and will save time by not performing the verifications
      int setTableName(const std::string &, const std::string & = "no");
      // returns 1 if the table is created for the first time
      // If the second argument is "yes" then the function will trust that all folders are created
      //    and will save time by not performing the verifications
      std::string updateDBInitStr(const std::string &) const;
      TOSF::TSets getIndTable(const std::string & ) const;
      int initTableFromStr(const std::string &);
      int insertQ(const::std::vector<std::string> &, const std::string &);
      //This will only insert in queue - requires executeFromQueue() afterwards
      int insertMTF(const::std::vector<std::string> &, const std::string &, const long & = 1000);
      // This will force insert in the database. Before and after the insert it will clear
      // the execution Queue - it will make multiple attempts (the last argument is the number of attempts)
      int executeFromQueue();
      long searchSize(const std::vector<std::string> &,
                      const std::vector<long> & ) const;
      long searchSize(const std::vector<std::string> &,
                      const std::vector<std::string> & ) const;
      std::pair<std::vector<std::string>, std::string> operator[](const long & ) const;
      std::vector< std::pair<std::vector<std::string>, std::string> >
      search(const std::vector<std::string> &,
             const std::vector<long> & = std::vector<long>{-1,-1},
             const long & = 0,
             const long & = -1) const;
      //Arguments of search:
      // 1 - combination key
      // 2 - the keys over which the search occurs (in the case that combination key is not the complete key sequence)
      // 3,4 - In the case that there is more than one result and we don't want all of them the third and fourth argument are range
      std::vector< std::pair<std::vector<std::string>, std::string> >
      search(const std::vector<std::string> &,
             const std::vector<std::string> &,
             const long & = 0,
             const long & = -1) const;
      //Arguments of search:
      // 1 - combination key
      // 2 - the keys over which the search occurs (in the case that combination key is not the complete key sequence)
      // 3,4 - In the case that there is more than one result and we don't
      // want all of them the third and fourth argument are range
      int delRowQ(const std::vector<std::string> &);
      //This will only submit delRow request to queue - requires executeFromQueue() afterwards
      int delRow(const std::vector<std::string> &, const long & = 1000);
      // This will force delete from the database. Before and after the delete it will clear
      // the execution Queue - it will make multiple attempts (the last argument is the number of attempts)
      long size() const;
      std::string statusReportForDebugging() const;
      int finalizeTableUpdate();
  };
  std::set<long> Table::setKeyForIndexFromStr(const std::string & st) const{
      std::set<long> setInd;
      std::string sepB="_i_";
      std::string sepE="_ei_";
      std::string inpS=st;
      for(long i=0;i<3;++i){
          inpS+=sepB+"endInd"+sepE;
      }
      long pos=0;
      std::pair<std::string,int> allDataP=SF::extract(inpS,pos,sepB,sepE);
      while( (allDataP.second==1) && (allDataP.first!="endInd")){
          setInd.insert(BF::stringToInteger(allDataP.first));
          allDataP=SF::extract(inpS,pos,sepB,sepE);
      }
      return setInd;
  }
  std::vector<long> Table::vectKeyForIndexFromStr(const std::string & st) const{
      std::set<long> setInd=setKeyForIndexFromStr(st);
      std::vector<long> v;
      v.resize(setInd.size());
      long i=0;
      std::set<long>::iterator it,itE;
      itE=setInd.end();
      it=setInd.begin();
      while(it!=itE){
          v[i]=*it;
          ++i;++it;
      }
      return v;
  }
  std::string Table::indNameFromVectKeys(const std::vector<long> & vI) const{
    std::string badVector="badVector";
    int fR=1;
    std::set<long> inds;
    long sz=vI.size();
    long i=0;
    long numKeys=keyNames.size();
    while((i<sz)&&(fR==1)){
        if(vI[i]<numKeys){
            inds.insert(vI[i]);
        }
        else{
            fR=0;
        }
        ++i;
    }
    if(fR==0){
        return badVector;
    }
    std::set<long>::iterator it,itE;
    itE=inds.end();
    it=inds.begin();
    std::string newInd;
    std::string sepB="_i_";
    std::string sepE="_ei_";
    while(it!=itE){
        newInd+=sepB+std::to_string(*it)+sepE;
        ++it;
    }
    return newInd;
  }
  std::string Table::strFromVectKeys(const std::vector<long> & _v) const{
      std::vector<long> v=_v;
      std::sort(v.begin(),v.end());
      std::string fR="";
      long sz=v.size(),i=0;
      std::string sepB="_i_";
      std::string sepE="_ei_";
      while(i<sz){
          fR+=sepB+std::to_string(v[i])+sepE;
          ++i;
      }
      return fR;
  }
  std::string Table::statusReportForDebugging() const{
    std::string fR;
    fR="starting folder name: "+  stFN;
    fR+="\nKey names: ";
    for(long i=0;i<keyNames.size();++i){fR+=keyNames[i]+" ";}
    fR+="\nMain data name: "+ mainDataName;
    fR+="\nTable name: "+tableName;
      fR+="\nINDEX START\n";
      std::set<std::string>::const_iterator it,itE;
      itE=indexNames.end();
      it=indexNames.begin();
      while(it!=itE){
          fR+="    "+(*it)+"\n";++it;
      }
      fR+="INDEX END\n";
      fR+="\n<BR>\nCOMBINATION KEY STATUS REPORT BEGIN<BR>\n";
      fR+=getCombKey().statusReportForDebugging();
      fR+="<BR>\nCOMBINATION KEY STATUS REPORT END<BR>\n";
    return fR;
  }
  int Table::setFolderNames(const std::string &f, const std::string & trust){
      stFN=f;
      if(trust=="yes"){
        return 0;
      }
      return IOF::sys_createFolderIfDoesNotExist(stFN,"readme.txt","Do not edit this folder.");
  }
  Table::Table(const std::string & _sfn,const std::string &_tn){
      if(_sfn!="notSet!***!"){
          setFolderNames(_sfn);
          if(tableName!="defaultTable"){
              setTableName(_tn);
          }
      }
  }
  std::string Table::getStartingFolderName()const{
    return stFN;
  }
  std::vector<std::string> Table::getKeyNames() const{
      return keyNames;
  }
  std::string Table::getMainDataName() const{
      return mainDataName;
  }
  std::string Table::getTableName() const{
      return tableName;
  }
  void Table::reCreateSearchMapForKeys(){
    keyNameToVectInd.clear();
    long sz=keyNames.size();
    for(long i=0;i<sz;++i){
      keyNameToVectInd[keyNames[i]]=i;
    }
  }
  void Table::setKeyNames(const std::vector<std::string> & _v){
      keyNames=_v;
      std::sort(keyNames.begin(),keyNames.end());
      reCreateSearchMapForKeys();
  }
  void Table::setMainDataName(const std::string & _s){
      mainDataName=_s;
  }
  int Table::setTableName(const std::string & _s,const std::string & _trust){
      tableName=_s;
      tableFolder=stFN+"/"+tableName;
    if((_trust=="yes")||(IOF::sys_createFolderIfDoesNotExist(tableFolder,tInitFName,"Do not edit this folder.")==0)){
      // table existed from before
      std::string initData=IOF::fileToString(tableFolder+"/"+tInitFName);
      initTableFromStr(initData);
    }
    if(_trust!="yes"){
      std::string t2Init=tableFolder;
      t2Init+="/"+tCKIFName;
      std::ifstream f2Test(t2Init);
      if(!f2Test.good()){
        IOF::toFile(t2Init," ");
      }
    }
    std::string fQ=tableFolder;
    fQ+="/"+folderInstQueue;
    if(_trust!="yes"){
      return IOF::sys_createFolderIfDoesNotExist(fQ,tInitFName,"Do not edit this folder.");
    }
    return 0;
  }
  std::string Table::updateDBInitStr(const std::string & tableDBInitStr) const{
      ATF::Table tempT(stFN,tableName);
      // The class ATF::Table has a very good method to update init string
      // that we want to use at this point.
      // Create a fake table tempT from the string.
      tempT.setMainDataName(mainDataName);
      tempT.setKeyNames(keyNames);
      std::string st1=tempT.updateDBInitStr(tableDBInitStr);
      std::string indexNamesB="_inds*"+tableName+"!*_";
      std::string indexNamesE="_/inds*"+tableName+"!*_";
      long pos=0;
      std::string nextIndNameB="_n!*_";
      std::string nextIndNameE="_/n!*_";
      std::string newTableDBInitStr = SF::eraseStuffBetween(st1,indexNamesB,indexNamesE,pos).first;
      newTableDBInitStr+= indexNamesB;
      std::set<std::string>::iterator it,itE;
      it=indexNames.begin();
      itE=indexNames.end();
      while(it!=itE){
          newTableDBInitStr+= nextIndNameB;
          newTableDBInitStr+= *it;
          newTableDBInitStr+= nextIndNameE;
          ++it;
      }
      newTableDBInitStr+= indexNamesE;
      return newTableDBInitStr;
  }
  int Table::initTableFromStr(const std::string &stToInitFrom){
    ATF::Table tempT;
    // The class ATF::Table has a very good method to update init string
    // that we want to use at this point.
    // Create a fake table tempT from the string.
      tempT.setTableName(tableName);
    int fR= tempT.initTableFromStr(stToInitFrom);
    if(fR==1){
      stFN=tempT.getStartingFolderName();
      keyNames=tempT.getKeyNames();
      mainDataName=tempT.getMainDataName();
      reCreateSearchMapForKeys();
      std::string indexNamesB="_inds*"+tableName+"!*_";
      std::string indexNamesE="_/inds*"+tableName+"!*_";
      long pos=0;
      std::string nextIndNameB="_n!*_";
      std::string nextIndNameE="_/n!*_";
      std::pair<std::string,int> allDataP=SF::extract(stToInitFrom,pos,indexNamesB,indexNamesE);
      if(allDataP.second==0){
        return 0;
      }
        pos=0;
        std::string stInds=allDataP.first;
        indexNames.clear();
        for(long i=0;i<3;++i){stInds+= nextIndNameB+"endIndices"+nextIndNameE;}
        allDataP=SF::extract(stInds,pos,nextIndNameB,nextIndNameE);
        while( (allDataP.second==1) && (allDataP.first!="endIndices")){
            indexNames.insert(allDataP.first);
            allDataP=SF::extract(stInds,pos,nextIndNameB,nextIndNameE);
        }
    }
    return 1;
  }
  std::vector<long> Table::keyNamesToInds(const std::vector<std::string> & vI) const{
      long i=0;
      long sz=vI.size();
      std::map<std::string,long>::const_iterator iM,iME;
      iME=keyNameToVectInd.end();
      std::vector<long> fR;
      fR.resize(sz);
      while(i<sz){
          iM=keyNameToVectInd.find(vI[i]);
          if(iM!=iME){
              fR[i]=iM->second;
          }
          ++i;
      }
      return fR;
  }
  int Table::finalizeTableUpdate(){
      std::string nSt=updateDBInitStr("");
      std::string tInit=tableFolder;
      tInit+="/"+tInitFName;
      IOF::toFile(tInit,nSt);
      return 1;
  }
  int Table::createIndex(const std::vector<long> & vI,
                            const int & repair){
      std::string newInd=indNameFromVectKeys(vI);
      if(newInd=="badVector"){return 0;}
      std::set<std::string>::iterator itS,itSE;
      itSE=indexNames.end();
      itS=indexNames.find(newInd);
      int createdForTheFirstTime=0;
      if(itS==itSE){
          indexNames.insert(newInd);
          std::string indFolders=stFN+"/"+ tableName+"/"+stFolderIndex;
          IOF::sys_createFolderIfDoesNotExist(indFolders,"readme.txt","Do not edit this folder.");
          indFolders += "/"+newInd;
          IOF::sys_createFolderIfDoesNotExist(indFolders,"readme.txt","Do not edit this folder.",indFName," ");
          finalizeTableUpdate();
          createdForTheFirstTime=1;
      }
      if((createdForTheFirstTime==1)||(repair==1)){
        long szz=this->size();
        long ii=0;
        std::vector<std::string> tmpV;
        while(ii<szz){
          tmpV=((*this)[ii]).first;
          updateIndexWithEntry(newInd, tmpV,1);
          ++ii;
        }
      }
      return createdForTheFirstTime;
  }
  int Table::createIndex(const std::vector<std::string> & vI, const int & repair){
      std::vector<long> vL=keyNamesToInds(vI);
      return createIndex(vL,repair);
  }
  int Table::deleteIndex(const std::vector<long> & vI){
    std::string newInd=indNameFromVectKeys(vI);
    if(newInd=="badVector"){return 0;}
      std::set<std::string>::iterator itS,itSE;
      itSE=indexNames.end();
      itS=indexNames.find(newInd);
      if(itS==itSE){
        return 0;
      }
      indexNames.erase(newInd);
      std::string indFolders=stFN+"/"+ tableName+"/"+stFolderIndex;
      std::string tF=indFolders+"/readme.txt";
      std::ifstream fTest(tF);
      if(fTest.good()){
        indFolders += "/"+newInd;
        std::string tF1=indFolders+"/readme.txt";
        std::ifstream fT1(tF1);
        if(fT1.good()){
            std::string systemCommand="rm -rf "+indFolders;
            system(systemCommand.c_str());
        }
      }
      finalizeTableUpdate();
      return 1;
  }
  int Table::deleteIndex(const std::vector<std::string> & vI){
      std::vector<long> vL=keyNamesToInds(vI);
      return deleteIndex(vL);
  }
  ATF::Table Table::getCombKey() const{
    std::string tbF=tableFolder;
    std::string tN=tableName+"*CKT*";
    ATF::Table combKInd(tableFolder,tN);
    std::string t2Init=tableFolder;
    t2Init+="/"+tCKIFName;
    std::string inSt=IOF::fileToString(t2Init);
    int succ=combKInd.initTableFromStr(inSt);
    if(succ==0){
      combKInd.setTableName(tN);
      combKInd.setStartingFolderName(tbF+"/"+"cKDB");
      combKInd.setKeyNames(keyNames);
      combKInd.setMainDataName(mainDataName);
    }
    return combKInd;
  }
  TOSF::TSets Table::getIndTable(const std::string &indName) const{
    std::string indFolders=stFN+"/"+ tableName+"/"+stFolderIndex+"/";
    indFolders+=indName;
    std::string fForInit=indFolders+"/"+indFName;
    std::string stIn=IOF::fileToString(fForInit);
    std::string tN=indName+"*IND*";
    TOSF::TSets indT(indFolders,tN);
    indT.setStartingFolderName(indFolders);
    indT.setTableName(tN);
    std::vector<long> knsInds=vectKeyForIndexFromStr(indName);
    std::vector<std::string> kns;
    long sz=knsInds.size();
    kns.resize(sz);
    for(long i=0;i<sz;++i){
      kns[i]=keyNames[knsInds[i]];
    }
    indT.setKeyNames(kns);
    indT.setMainDataName("allKeys");
    int resInit=indT.initTableFromStr(stIn);
    if(resInit==0){
      indT.setStartingFolderName(indFolders);
      indT.setTableName(tN);
      std::vector<long> knsInds=vectKeyForIndexFromStr(indName);
      std::vector<std::string> kns;
      long sz=knsInds.size();
      kns.resize(sz);
      for(long i=0;i<sz;++i){
        kns[i]=keyNames[knsInds[i]];
      }
      indT.setKeyNames(kns);
      indT.setMainDataName("allKeys");
    }
    return indT;
  }
  std::string Table::combKeyToString(const std::vector<std::string> &k) const{
    std::string fR="";
    std::string bSep="_!*X!*_";
    std::string eSep="_/!*X!*_";
    long sz=k.size();
    for(long i=0;i<sz;++i){
      fR+=bSep+k[i]+eSep;
    }
    return fR;
  }
  std::set<StringWithCounter> Table::stringToCombKeySet(const std::string &s) const{
    std::set<StringWithCounter> fR;
    long pos=0;
    std::pair<std::string,int> nextV;
    std::string bSep="_!*X!*_";
    std::string eSep="_/!*X!*_";
    std::string rS=s;
    std::string symbolEnd="endK!*_";
    for(long i=0;i<3;++i){
      rS+=bSep+symbolEnd+eSep;
    }
    long sz=rS.size();
    nextV=SF::extract(rS,pos,bSep,eSep);
    StringWithCounter tmp;long counter=0;
    while((pos<sz)&&(nextV.second==1)&&(nextV.first!=symbolEnd)){
      tmp.s=nextV.first;
      tmp.counter=counter;
      ++counter;
      fR.insert(tmp);
      nextV=SF::extract(rS,pos,bSep,eSep);
    }
    return fR;
  }
  std::vector<std::string> Table::stringToCombKey(const std::string& s) const{
    std::set<StringWithCounter> cS=stringToCombKeySet(s);
    std::set<StringWithCounter>::const_iterator it,itE;
    it=cS.begin();itE=cS.end();
    long sz=cS.size();
    std::vector<std::string> fR;
    fR.resize(sz);
    long i=0;
    while(it!=itE){
      fR[i]=(*it).s;
      ++it;++i;
    }
    return fR;
  }
  int Table::updateIndexWithEntry(const std::string & indName, const std::vector<std::string> & combKey, const long & action){
  //action == 1 : add
  //action == 2 : delete
    int fR=0;
    std::vector<long> vI=vectKeyForIndexFromStr(indName);
    TOSF::TSets inT=getIndTable(indName);
    std::string mData=combKeyToString(combKey);
    std::vector<std::string> keyForSearch;
    long kS=vI.size();
    keyForSearch.resize(kS);
    for(long i=0;i<kS;++i){
      keyForSearch[i]=combKey[vI[i]];
    }
    if(action==1){
      inT.insert(keyForSearch,mData);
    }
    if(action==2){
      inT.delSetEntry(keyForSearch,mData);
    }
    std::string indFolders=stFN+"/"+ tableName+"/"+stFolderIndex+"/";
    indFolders+=indName;
    std::string fForInit=indFolders+"/"+indFName;
    std::string stIn=IOF::fileToString(fForInit);
    stIn=inT.updateDBInitStr(stIn);
    IOF::toFile(fForInit,stIn);
    return fR;
  }
  int  Table::insertUnsafe(const std::vector<std::string> & keyV, const std::string & data){
    if(keyV.size()!=keyNames.size()){
      return 0;
    }
    ATF::Table combKInd=getCombKey();
    //Step 1: Inserting into the main table
    int insertionReport=combKInd.insertOrUpdateAndGiveReport(keyV,data);
    if(insertionReport==1){
      //Step 2: Update table initialization - the root may have changed
      std::string t2Init=tableFolder;
      t2Init+="/"+tCKIFName;
      std::string inSt=IOF::fileToString(t2Init);
      inSt=combKInd.updateDBInitStr(inSt);
      IOF::toFile(t2Init,inSt);
      //Step 3: Update indices
      std::set<std::string>::const_iterator it,itE;
      it=indexNames.begin();itE=indexNames.end();
      while(it!=itE){
        updateIndexWithEntry(*it,keyV,1);
        ++it;
      }
    }
    return 1;
  }
  std::pair<std::vector<std::string>, std::string>
  Table::operator[](const long & i) const{
    std::pair<std::vector<std::string>, std::string>  fR;
    ATF::Table combKInd=getCombKey();
    std::pair<HDDBRF::Record,std::string> pDBRS=combKInd[i];
    HDDBRF::Record dS=pDBRS.first;
    fR.first=dS.getKeys();
    IRFNF::IndRecFName indRFN = combKInd.findIndexRecordFileName(pDBRS.first);
    std::string dfn=DMF::dataFNameFromIRFN(indRFN);
    fR.second=IOF::fileToString(dfn);
    return fR;
  }
  long Table::size() const{
    ATF::Table combKInd=getCombKey();
    return combKInd.size();
  }
  std::vector< std::pair<std::vector<std::string>, std::string> >
  Table::search(const std::vector<std::string> & _keyV,
                   const std::vector<long> & _indV,
                   const long & _start,
                   const long & _end) const{
    std::vector< std::pair<std::vector<std::string>, std::string> > fR;
    std::vector<std::string> keyV=_keyV;
    std::vector<long> indV=_indV;
    if( indV.size()==keyV.size()){
      if( (indV.size()!=2) || (indV[0]!=-1)){
        SF::sortWithFollower(indV,keyV);
      }
    }
    if( ((indV.size()==2)&&(indV[0]==-1)) || (indV.size()==keyNames.size()) ) {
      fR.resize(1);
      ATF::Table combKInd=getCombKey();
      fR[0].first=keyV;
      fR[0].second=combKInd.select(keyV);
      return fR;
    }
    std::string indName=strFromVectKeys(indV);
    std::set<std::string>::const_iterator it,itE;
    itE=indexNames.end();
    it=indexNames.find(indName);
    if(it==itE){
      return fR;
    }
    // indName is an actual index;
    TOSF::TSets inT=getIndTable(indName);
    std::vector<std::string> completeKeys=inT.getRangeFromSet(keyV,_start,_end);
    long sz=completeKeys.size();
    fR.resize(sz);
    std::vector<std::string> cKey;
    ATF::Table combKInd=getCombKey();
    for(long i=0;i<sz;++i){
      cKey=stringToCombKey(completeKeys[i]);
      fR[i].first=cKey;
      fR[i].second=combKInd.select(cKey);
    }
    return fR;
  }
  //Arguments of search:
  // 1 - combination key
  // 2 - the keys over which the search occurs (in the case that combination key is not the complete key sequence)
  // 3,4 - In the case that there is more than one result and we don't want all of them the third and fourth argument are range
  std::vector< std::pair<std::vector<std::string>, std::string> >
  Table::search(const std::vector<std::string> & keyV,
                   const std::vector<std::string> & indV,
                   const long & start,
                   const long & end) const{
      if((indV.size()==2)&&(indV[0]=="*!_")){
        std::vector<long>l2;
        l2.resize(2);l2[0]=-1;l2[1]=-1;
        return search(keyV,l2,start,end);
      }
      return search(keyV,keyNamesToInds(indV),start,end);
  }
  long Table::searchSize(const std::vector<std::string> & keyV,
                            const std::vector<long> & _indV) const{
    std::string indName=strFromVectKeys(_indV);
    std::set<std::string>::const_iterator it,itE;
    itE=indexNames.end();
    it=indexNames.find(indName);
    if(it==itE){
      return 0;
    }
    // indName is an actual index;
    TOSF::TSets inT=getIndTable(indName);
    return inT.getSizeOfSet(keyV);
  }
  long Table::searchSize(const std::vector<std::string> & keyV,
                            const std::vector<std::string> & indV) const{
    return searchSize(keyV,keyNamesToInds(indV));
  }
  int Table::delRowUnsafe(const std::vector<std::string> & keyV){
    int fR;
    //Step 1: Deleting from the main table
    ATF::Table combKInd=getCombKey();
    fR=combKInd.delRow(keyV);
    std::string t2Init=tableFolder;
    t2Init+="/"+tCKIFName;
    std::string inSt=IOF::fileToString(t2Init);
    inSt=combKInd.updateDBInitStr(inSt);
    IOF::toFile(t2Init,inSt);
    //Step 2: Update index tables
    std::set<std::string>::const_iterator it,itE;
    it=indexNames.begin();itE=indexNames.end();
    while(it!=itE){
      updateIndexWithEntry(*it,keyV,2);
      ++it;
    }
    return fR;
  }
  std::string Table::prepareDBRecString(const std::vector<std::string> & vKey, const std::string & vD) const{
    HDDBRF::Record dbr;
    dbr.setKeys(vKey);
    dbr.setMainDataRTD(vD);
    return dbr.putIntoString();
  }
  std::pair<std::vector<std::string>,std::string> Table::invertDBRecString(const std::string & st) const{
    HDDBRF::Record dbr;
    dbr.loadFromString(st);
    std::pair<std::vector<std::string>,std::string> fR;
    fR.first=dbr.getKeys();
    fR.second=dbr.getMainDataRTD();
    return fR;
  }
  std::string Table::prepareKeyString(const std::vector<std::string> & vKey) const{
    return prepareDBRecString(vKey,"noMainDataJustKey");
  }
  std::vector<std::string> Table::invertKeyString(const std::string & stD) const{
    return (invertDBRecString(stD)).first;
  }
  std::string Table::createFileName(const std::string & _s, const std::string & _e) const{
    long processId=getpid();
    std::string fN=_s+std::to_string(processId)+"j";
    fN+=std::to_string(RNDF::randNum());
    fN+="."+_e;
    return fN;
  }
  int Table::insertQ(const std::vector<std::string> & keyV, const std::string & data){
    if(keyV.size()!=keyNames.size()){
      return 0;
    }
    std::string entryToInsert=prepareDBRecString(keyV,data);
    std::string enS="_enIns*!_";
    std::string enE="_/enIns*!_";
    std::string cS="_command*!_";
    std::string cE="_/command*!_";
    std::string ccommand="insert";
    std::string entireSt=" ";
    entireSt+=cS+ccommand+cE+" "+enS+entryToInsert+enE;
    std::string fileName=tableFolder+"/"+folderInstQueue;
    fileName+="/";
    std::string fN=createFileName("i",extForCommandsInQueue);
    fileName+=fN;
    IOF::toFile(fileName,entireSt);
    return 1;
  }
  int Table::insertMTF(const std::vector<std::string> & keyV, const std::string & data, const long & numAttempts){
    if(keyV.size()!=keyNames.size()){
      return 0;
    }
    int exSuccess=executeFromQueue();
    long i=0;
    while((i<numAttempts)&&(exSuccess==0)){
      nanoSleep(timeToSleepBeforeAttemptingToClearTheQueueAgain);
      ++i;
      exSuccess=executeFromQueue();
    }
    if(exSuccess==0){return 0;}
    insertQ(keyV,data);
    exSuccess=executeFromQueue();
    i=0;
    while((i<numAttempts)&&(exSuccess==0)){
      nanoSleep(timeToSleepBeforeAttemptingToClearTheQueueAgain);
      ++i;
      exSuccess=executeFromQueue();
    }
    return exSuccess;
  }
  int Table::delRowQ(const std::vector<std::string> & keyV){
    std::string entryToInsert=prepareKeyString(keyV);
    std::string enS="_enIns*!_";
    std::string enE="_/enIns*!_";
    std::string cS="_command*!_";
    std::string cE="_/command*!_";
    std::string ccommand="delete";
    std::string entireSt=" ";
    entireSt+=cS+ccommand+cE+" "+enS+entryToInsert+enE;
    std::string fileName=tableFolder+"/"+folderInstQueue;
    fileName+="/";
    std::string fN=createFileName("d",extForCommandsInQueue);
    fileName+=fN;
    IOF::toFile(fileName,entireSt);
    return 1;
  }
  int Table::delRow(const std::vector<std::string> & keyV, const long & numAttempts){
    int exSuccess=executeFromQueue();
    long i=0;
    while((i<numAttempts)&&(exSuccess==0)){
      nanoSleep(timeToSleepBeforeAttemptingToClearTheQueueAgain);
      ++i;
      exSuccess=executeFromQueue();
    }
    if(exSuccess==0){return 0;}
    delRowQ(keyV);
    exSuccess=executeFromQueue();
    i=0;
    while((i<numAttempts)&&(exSuccess==0)){
      nanoSleep(timeToSleepBeforeAttemptingToClearTheQueueAgain);
      ++i;
      exSuccess=executeFromQueue();
    }
    return exSuccess;
  }
  int Table::executeFromQueue(){
    //return values:
    // 1 - success
    // 0 - there is a healthy process writing to the database; or I am taking too long and will give up
    TMF::Timer tm;
    int remainingProcesses=0;
    long tN=tm.timeNow();
    std::string folderQ=tableFolder+"/"+folderInstQueue;
    std::vector<std::string> seqF;
    seqF=IOF::listFiles(folderQ);
    std::vector<std::string> sFl=IOF::selectFilesWithExtension(seqF,extForFlag);
    std::vector<std::string> sCQ=IOF::selectFilesWithExtension(seqF,extForCommandsInQueue);
    if(sFl.size()>0){
      std::vector<std::time_t> cTimes=IOF::timeOfCreation(sFl);
      SF::sortWithFollower(cTimes,sFl);
      long sz=sFl.size();
      for(long i=0;i<sz;++i){
        if (tN-cTimes[i]>2*timeToWaitBeforeDecidingThatPreviousProcessIsStuckAndNeedsToBeKilled){
          IOF::sys_deleteFile(sFl[i]);
        }
        else{
          ++remainingProcesses;
        }
      }
      if(remainingProcesses>0){
        return 0;
      }
     }
    long tN2=tm.timeNow();
    if(tN2-tN>timeToWaitBeforeDecidingThatPreviousProcessIsStuckAndNeedsToBeKilled){
      return 0;
    }
    std::vector<std::time_t> cTimes=IOF::timeOfCreation(sCQ);
    SF::sortWithFollower(cTimes,sCQ);
    long sz=sCQ.size();
    std::vector<std::string> commands;
    std::vector<std::vector<std::string> > keys;
    std::vector<std::string> datas;
    commands.resize(sz);
    keys.resize(sz);
    datas.resize(sz);
    std::vector<std::string> rawFiles;
    rawFiles.resize(sz);
    long pos=0;
    std::string enS="_enIns*!_";
    std::string enE="_/enIns*!_";
    std::string cS="_command*!_";
    std::string cE="_/command*!_";
    std::pair<std::string,int> allDataP;
    for(long i=0;i<sz;++i){
      rawFiles[i]=IOF::fileToString(sCQ[i]);
      pos=0;
      allDataP=SF::extract(rawFiles[i],pos,cS,cE);
      commands[i]="wrongCommand";
      datas[i]="notNeeded";
      if(allDataP.second==1){
        commands[i]=allDataP.first;
        pos=0;
        allDataP=SF::extract(rawFiles[i],pos,enS,enE);
        if(allDataP.second==0){
          commands[i]="wrongCommand";
        }
        else{
          if(commands[i]=="insert"){
            std::pair<std::vector<std::string>,std::string> tP=invertDBRecString(allDataP.first);
            keys[i]=tP.first;
            datas[i]=tP.second;
          }
          if(commands[i]=="delete"){
            keys[i]=invertKeyString(allDataP.first);
          }
        }
      }
    }
    tN2=tm.timeNow();
    if(tN2-tN>timeToWaitBeforeDecidingThatPreviousProcessIsStuckAndNeedsToBeKilled){
      return 0;
    }
    std::string myFlagName=createFileName("f",extForFlag);
    std::string flagFile=folderQ+"/"+myFlagName;
    IOF::toFile(flagFile,"This flag means that I am writing to the database.");
    for(long i=0;i<sz;++i){
      if(commands[i]=="insert"){
        insertUnsafe(keys[i],datas[i]);
      }
      if(commands[i]=="delete"){
        delRowUnsafe(keys[i]);
      }
      IOF::sys_deleteFile(sCQ[i]);
    }
    IOF::sys_deleteFile(flagFile);
    return 1;
  }
}
#endif
