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


#ifndef _INCL_MYTOFSETS_CPP
#define _INCL_MYTOFSETS_CPP

namespace TOSF{

  class TSets{
  private:
      std::string stFN;
      std::vector<std::string> keyNames;
      std::string mainDataName;
      std::string tableName;
      std::map<std::string,long> keyNameToVectInd;
      void reCreateSearchMapForKeys();
      std::string stFolderKeys;
      std::string stFolderData;
      ATF::Table tableKeys;
      void setFolderNames(const std::string &, const long & =1);
  public:
      TSets(const std::string &, const std::string & ="defaultTable");
      void initialize(const std::string &, const std::string & ="defaultTable");
      std::vector<std::string> getKeyNames() const;
      std::string getMainDataName() const;
      std::string getTableName() const;
      std::string getStartingFolderName() const;
      void setStartingFolderName(const std::string &);
      void setKeyNames(const std::vector<std::string> &);
      void setMainDataName(const std::string &);
      void setTableName(const std::string &);
      std::string updateDBInitStr(const std::string &) const;



      void copyFromTable(const TSets &, const std::string &);

      int initTableFromStr(const std::string &);


      int insert(const std::vector<std::string> &, const std::string &);
      int delRow(const std::vector<std::string> &);
      int delSetEntry(const std::vector<std::string> &, const std::string &);
      int delSetEntry(const long &, const std::string &);
      long findRowIndex(const std::vector<std::string> &) const;
      long lowerBoundRowIndex(const std::vector<std::string> &) const;
      std::vector<std::string> getKeysFromRow(const long &) const;
      long findIndexInSet(const std::vector<std::string> &, const std::string &) const;
      long findIndexInSet(const long &, const std::string &) const;
      std::string entireSetFromKey(SETF::Set<HDDBSF::String>& , const std::vector<std::string> &) const;
      std::string getValueFromSet(const std::vector<std::string> &, const long &) const;
      std::string getValueFromSet(const long &, const long &) const;
      long getSizeOfSet(const std::vector<std::string> &) const;
      long getSizeOfSet(const long &) const;

      std::vector<std::string> getRangeFromSet(const std::vector<std::string> & , const long & , const long &) const;

      long size() const;
      void clear();
      std::string statusReportForDebugging() const;
  };
  std::string TSets::statusReportForDebugging() const{
    std::string fR;
    fR="starting folder name: "+  stFN;
    fR+="\nKey names: ";
    for(long i=0;i<keyNames.size();++i){fR+=keyNames[i]+" ";}
    fR+="\nMain data name: "+ mainDataName;
    fR+="\nTable name: "+tableName;
    fR+="\nReport for tableKeys BEGIN\n";
    fR+=tableKeys.statusReportForDebugging();
    fR+="\nReport for tableKeys END\n";
    return fR;
  }

  void TSets::setFolderNames(const std::string &f,const long & skipTableKeys){
    stFN=f;

    IOF::sys_createFolderIfDoesNotExist(stFN,"readme.txt","Do not edit this folder.");
    stFolderData=f;
    stFolderKeys=f;
    stFolderData+="/d_";
    stFolderKeys+="/k_";
    if(skipTableKeys!=0){
      tableKeys.setStartingFolderName(stFolderKeys);
    }
  }
  TSets::TSets(const std::string & _sfn,const std::string &_tn){
      initialize(_sfn,_tn);

  }
  void TSets::initialize(const std::string & _sfn,const std::string &_tn){
      tableName=_tn;
      setFolderNames(_sfn);
      keyNames.clear();
      keyNameToVectInd.clear();

  }
  void TSets::setStartingFolderName(const std::string & _sfn){
    setFolderNames(_sfn);
  }
  std::string TSets::getStartingFolderName()const{
    return stFN;
  }
  std::vector<std::string> TSets::getKeyNames() const{
      return keyNames;
  }
  std::string TSets::getMainDataName() const{
      return mainDataName;
  }
  std::string TSets::getTableName() const{
      return tableName;
  }

  void TSets::reCreateSearchMapForKeys(){
    keyNameToVectInd.clear();
    long sz=keyNames.size();
    for(long i=0;i<sz;++i){
      keyNameToVectInd[keyNames[i]]=i;
    }
  }

  void TSets::setKeyNames(const std::vector<std::string> & _v){
      long l=_v.size();
      long oldL=keyNames.size();
      tableKeys.setKeyNames(_v);
      if(l!=oldL){
          if(l>0){
              keyNames=_v;
          }
          else{
              keyNames.resize(1);
              l=1;
              keyNames[0]="defaultKey";
          }



      }
      else{
          keyNames=_v;
      }
      reCreateSearchMapForKeys();
  }
  void TSets::setMainDataName(const std::string & _s){
      mainDataName=_s;
      tableKeys.setMainDataName(_s);
  }
  void TSets::setTableName(const std::string & _s){
      tableName=_s;
      tableKeys.setTableName(_s);
  }
  void TSets::copyFromTable(const TSets & _cF, const std::string & _sfn){
    clear();
    setFolderNames(_sfn);
    keyNames=_cF.keyNames;
    mainDataName=_cF.mainDataName;
    tableName=_cF.tableName;
    tableKeys.setMainDataName(mainDataName);
    tableKeys.setTableName(tableName);
    tableKeys.setKeyNames(keyNames);
    long tl=_cF.size();
    long sl=0;
    std::vector<std::string> keyV;
    for(long i=0;i<tl;++i){
      keyV=_cF.getKeysFromRow(i);
      sl=_cF.getSizeOfSet(keyV);
      for(long j=0;j<sl;++j){
        insert(keyV,_cF.getValueFromSet(keyV,j));
      }
    }

    keyNameToVectInd=_cF.keyNameToVectInd;
  }
  std::string TSets::updateDBInitStr(const std::string & tableDBInitStr) const{

    return tableKeys.updateDBInitStr(tableDBInitStr);
  }

  int TSets::initTableFromStr(const std::string &stToInitFrom){

    int fR=tableKeys.initTableFromStr(stToInitFrom);
    if(fR==1){
      std::string folderNameKeys=tableKeys.getStartingFolderName();
      std::string foldN="";
      long len=folderNameKeys.size();
      for(long i=0;i<len-3;++i){
        foldN += folderNameKeys[i];
      }
      setFolderNames(foldN,0);
      keyNames=tableKeys.getKeyNames();
      mainDataName=tableKeys.getMainDataName();
      tableName=tableKeys.getTableName();
      reCreateSearchMapForKeys();
    }
    return fR;
  }


  int TSets::insert(const std::vector<std::string> & keyV, const std::string & da){
    if(keyV.size()!=keyNames.size()){
      return 0;
    }

    std::string setName="_*sN!*_";
    std::string initSet="";
    SETF::Set<HDDBSF::String> dataSet;
    //Step 1. Check whether the keyV is already in the table
    long ind=tableKeys.find(keyV);
    if(ind==-1){
      dataSet.setStartingFolderName(stFolderData);
    }
    else{
      initSet=tableKeys.select(keyV);
      dataSet.loadInitializationFromString(initSet,setName);
    }
    dataSet += da;
    initSet=dataSet.saveInitializationToString(initSet,setName);
    tableKeys.insertOrUpdateAndGiveReport(keyV,initSet);
      return 1;
  }

  int TSets::delRow(const std::vector<std::string> & keyV){
    long ind=tableKeys.find(keyV);
    if(ind==-1){
      return 0;
    }
    std::string setName="_*sN!*_";
    std::string initSet=tableKeys.select(keyV);
    SETF::Set<HDDBSF::String> dataSet;
    dataSet.loadInitializationFromString(initSet,setName);
    dataSet.clear();
    tableKeys.delRow(keyV);
    return 1;
  }


  int TSets::delSetEntry(const std::vector<std::string> &keyV, const std::string &da){
    long ind=tableKeys.find(keyV);
    if(ind==-1){
      return 0;
    }
    std::string setName="_*sN!*_";
    std::string initSet=tableKeys.select(keyV);
    SETF::Set<HDDBSF::String> dataSet;
    dataSet.loadInitializationFromString(initSet,setName);
    dataSet -= da;
    long sz=dataSet.size();
    if(sz>0){
      initSet=dataSet.saveInitializationToString(initSet,setName);
      tableKeys.insertOrUpdateAndGiveReport(keyV,initSet);
    }
    else{
      tableKeys.delRow(keyV);
    }
    return 1;
  }
  int TSets::delSetEntry(const long & i, const std::string & da){
    if((i<0)||(i>tableKeys.size())){
      return 0;
    }
    return delSetEntry( ((tableKeys[i]).first).getKeys() , da);
  }

  long TSets::findRowIndex(const std::vector<std::string> & keyV) const{
    return tableKeys.find(keyV);
  }
  long TSets::lowerBoundRowIndex(const std::vector<std::string> &keyV) const{
    return tableKeys.findLowerBound(keyV);
  }
  std::vector<std::string> TSets::getKeysFromRow(const long & i) const{
    if((i<0)||(i>tableKeys.size())){
      std::vector<std::string> emptyV;
      return emptyV;
    }
    return  ((tableKeys[i]).first).getKeys();
  }
  long TSets::findIndexInSet(const std::vector<std::string> & keyV, const std::string & da) const{
    long ind=tableKeys.find(keyV);
    if(ind==-1){
      return -1;
    }
    std::string setName="_*sN!*_";
    std::string initSet=tableKeys.select(keyV);
    SETF::Set<HDDBSF::String> dataSet;
    dataSet.loadInitializationFromString(initSet,setName);
    return dataSet.find(da);
  }
  long TSets::findIndexInSet(const long & i, const std::string &da) const{
    if((i<0)||(i>tableKeys.size())){
      return -1;
    }
    return findIndexInSet(((tableKeys[i]).first).getKeys() , da);
  }

  std::string TSets::entireSetFromKey(SETF::Set<HDDBSF::String>& dataSet, const std::vector<std::string> & keyV) const{
    std::string initSet=tableKeys.select(keyV);
    if(initSet=="notFound"){
      return "notFound";
    }
    std::string setName="_*sN!*_";

    dataSet.loadInitializationFromString(initSet,setName);
    return "ok";
  }
  std::string TSets::getValueFromSet(const std::vector<std::string> & keyV, const long &i) const{
    SETF::Set<HDDBSF::String> dataSet;
    if(entireSetFromKey(dataSet,keyV)=="notFound"){
      return "notFound";
    }
    return ((dataSet[i]).first).getMainData();
  }

  std::vector<std::string> TSets::getRangeFromSet(const std::vector<std::string> & keyV, const long & _b, const long &_e) const{
    std::vector<std::string> fR;
    fR.resize(0);
    SETF::Set<HDDBSF::String> dataSet;
    if(entireSetFromKey(dataSet,keyV)=="notFound"){
      return fR;
    }
    long sz=dataSet.size();
    if(_b>sz){
      return fR;
    }
    long e=_e;
    if((e>sz)||(e<0)){
      e=sz;
    }
    if(e>_b){
      fR.resize(e-_b);
      for(long i=_b;i<e;++i){
        fR[i-_b]=((dataSet[i]).first).getMainData();
      }
    }
    return fR;
  }

  std::string TSets::getValueFromSet(const long &i, const long &j) const{
    if((i<0)||(i>tableKeys.size())){
      return "notFound";
    }
    return getValueFromSet(((tableKeys[i]).first).getKeys() , j);
  }
  long TSets::getSizeOfSet(const std::vector<std::string> & keyV) const{
    SETF::Set<HDDBSF::String> dataSet;
    if(entireSetFromKey(dataSet,keyV)=="notFound"){
      return -1;
    }
    return dataSet.size();
  }
  long TSets::getSizeOfSet(const long & i) const{
    if((i<0)||(i>tableKeys.size())){
      return -1;
    }
    return getSizeOfSet(((tableKeys[i]).first).getKeys());
  }
  long TSets::size() const{
    return tableKeys.size();
  }
  void TSets::clear(){
    long sz=tableKeys.size();
    while(sz>0){
       delRow(((tableKeys[0]).first).getKeys());
       sz=tableKeys.size();
    }
  }
}
#endif
