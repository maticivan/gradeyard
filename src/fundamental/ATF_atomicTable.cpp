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

#ifndef _INCL_MYHDATOMICTABLE_CPP
#define _INCL_MYHDATOMICTABLE_CPP



// Table (atomic table) may have multiple keys but the only search allowed
// is when the entire key is submitted. This structure cannot do any searches with partial keys


namespace ATF{
  SETF::MapSavedSearch<HDDBRF::Record> savedSearches;
  class Table{
  private:
      std::string startingFolderName;
      std::vector<std::string> keyNames;
      std::string mainDataName;
      std::string tableName;
      SETF::Set<HDDBRF::Record> combinationKeySearch;
      std::map<std::string,long> keyNameToVectInd;
      void reCreateSearchMapForKeys();
      Table(Table &);
      void operator=(const Table &);
      std::string getTableIdForSavedSearches() const;
      void clearSavedSearches() const;
      IRFNF::IndRecFName<HDDBRF::Record> findInSavedSearches(const HDDBRF::Record&) const;
  public:
      Table(const std::string & = "dbDefault", const std::string & ="defaultSimpleTable");
      Table(Table &&);
      void operator=(Table &&);
      void initialize(const std::string & = "dbDefault", const std::string & ="defaultSimpleTable");
      std::vector<std::string> getKeyNames() const;
      std::string getMainDataName() const;
      std::string getTableName() const;
      std::string getStartingFolderName() const;
      void setStartingFolderName(const std::string &);
      void setKeyNames(const std::vector<std::string> &);
      void setMainDataName(const std::string &);
      void setTableName(const std::string &);
      std::string updateDBInitStr(const std::string &) const;

      void copyFromTable(const Table &, const std::string &);

      int initTableFromStr(const std::string &);


      std::string select(const std::vector<std::string> &) const;

      IRFNF::IndRecFName<HDDBRF::Record> findIndexRecordFileName(const HDDBRF::Record & ) const;
      IRFNF::IndRecFName<HDDBRF::Record> lowerBoundIndexRecordFileName(const HDDBRF::Record &, const long & = 0 ) const;
      long find(const std::vector<std::string> &) const;
      long findLowerBound(const std::vector<std::string> &) const;
      int insertOrUpdateAndGiveReport(const std::vector<std::string> &, const std::string &);
      int inPlaceKeyDataModification(const std::vector<std::string> &, const std::vector<std::string> &, const std::string &, const long &, const long & = 1);
      int delRow(const std::vector<std::string> &);
      long size() const;
      void clear();
      std::pair<HDDBRF::Record,std::string> operator[](const long & ) const;
      std::string statusReportForDebugging() const;
  };
  std::string Table::statusReportForDebugging() const{
    std::string fR;
    fR="starting folder name: "+  startingFolderName;
    fR+="\nKey names: ";
    for(long i=0;i<keyNames.size();++i){fR+=keyNames[i]+" ";}
    fR+="\nMain data name: "+ mainDataName;
    fR+="\nTable name: "+tableName;
    fR+="\nStatus report for combination key search begin:\n";
    fR+=combinationKeySearch.statusReportForDebugging();
    fR+="\nStatus report for combination key search end.";
    return fR;
  }
  std::string Table::getTableIdForSavedSearches() const{
    return startingFolderName+"|"+tableName;
  }
  void Table::clearSavedSearches() const{

    savedSearches.clear(getTableIdForSavedSearches());
  }
  IRFNF::IndRecFName<HDDBRF::Record> Table::findInSavedSearches(const HDDBRF::Record & _v) const{
    IRFNF::IndRecFName<HDDBRF::Record> fR;
    std::map<std::string,std::map<HDDBRF::Record,IRFNF::IndRecFName<HDDBRF::Record> > >::const_iterator it,itE;
    itE=savedSearches.m.end();
    it=savedSearches.m.find(getTableIdForSavedSearches());
    if(it==itE){
      return fR;
    }
    std::map<HDDBRF::Record,IRFNF::IndRecFName<HDDBRF::Record> >::const_iterator it2,it2E;
    it2E=(it->second).end();
    it2=(it->second).find(_v);
    if(it2!=it2E){
      return it2->second;
    }
    return fR;
  }

  Table::Table(Table&& _t){
    startingFolderName=_t.startingFolderName;
    keyNames=std::move(_t.keyNames);
    mainDataName=_t.mainDataName;
    tableName=_t.tableName;
    combinationKeySearch=std::move(_t.combinationKeySearch);
    keyNameToVectInd=std::move(_t.keyNameToVectInd);
  }
  void Table::operator=(Table&& _t){
    if(&_t!=this){
      startingFolderName=_t.startingFolderName;
      keyNames.clear();
      keyNames=std::move(_t.keyNames);
      mainDataName=_t.mainDataName;
      tableName=_t.tableName;
      combinationKeySearch=std::move(_t.combinationKeySearch);
      keyNameToVectInd.clear();
      keyNameToVectInd=std::move(_t.keyNameToVectInd);
    }

  }
  Table::Table(const std::string & _sfn,const std::string &_tn){
      initialize(_sfn,_tn);
  }
  void Table::initialize(const std::string & _sfn, const std::string & _tn){
    tableName=_tn;
    startingFolderName=_sfn;
    keyNames.clear();
    keyNameToVectInd.clear();
    combinationKeySearch.setStartingFolderName(startingFolderName);
  }
  void Table::setStartingFolderName(const std::string & _sfn){
    startingFolderName=_sfn;
    combinationKeySearch.setStartingFolderName(startingFolderName);
  }
  std::string Table::getStartingFolderName()const{
    return startingFolderName;
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
      long l=_v.size();
      long oldL=keyNames.size();
      if(l!=oldL){
          if(l>0){
              keyNames=_v;
          }
          else{
              keyNames.resize(1);
              l=1;
              keyNames[0]="defaultKey";
          }



          SETF::Set<HDDBRF::Record> emptySet;
          combinationKeySearch.copyFromSet(emptySet,startingFolderName);
      }
      else{
          keyNames=_v;
      }
      reCreateSearchMapForKeys();
  }
  void Table::setMainDataName(const std::string & _s){
      mainDataName=_s;
  }
  void Table::setTableName(const std::string & _s){
      tableName=_s;
  }
  void Table::copyFromTable(const Table & _cF, const std::string & _sfn){
    startingFolderName=_sfn;
    keyNames=_cF.keyNames;
    mainDataName=_cF.mainDataName;
    tableName=_cF.tableName;
    combinationKeySearch.copyFromSet(_cF.combinationKeySearch,_sfn);
    keyNameToVectInd=_cF.keyNameToVectInd;
  }

  std::string Table::updateDBInitStr(const std::string & tableDBInitStr) const{

      std::string lWithinFileB="_table*"+tableName+"!*_";
      std::string lWithinFileE="_/table*"+tableName+"!*_";
      std::string nmKeysB="_nmKeys!*_";
      std::string nmKeysE="_/nmKeys!*_";

      std::string allKeyNamesB="_keyNames!*_";
      std::string allKeyNamesE="_/keyNames!*_";

      std::string mDNameB="_mainDataName!*_";
      std::string mDNameE="_/mainDataName!*_";


      std::string combKSKW="_*combKeySearchKeyword!*_";

      std::string nextKeyNameB="_n!*_";
      std::string nextKeyNameE="_/n!*_";

      long pos=0;
      std::string allD=SF::extract(tableDBInitStr,pos,lWithinFileB,lWithinFileE).first;


      allD=combinationKeySearch.saveInitializationToString(allD,combKSKW);

      pos=0;

      long numKeys=keyNames.size();

      allD=SF::eraseStuffBetween(allD,nmKeysB,nmKeysE,pos).first;

      allD+=nmKeysB+std::to_string(numKeys)+nmKeysE;

      pos=0;
      allD=SF::eraseStuffBetween(allD,allKeyNamesB,allKeyNamesE,pos).first;
      allD+=allKeyNamesB;
      for(long i=0;i<numKeys;++i){
          allD+=nextKeyNameB;
          allD+=keyNames[i];
          allD+=nextKeyNameE;

      }
      allD+=allKeyNamesE;


      pos=0;

      allD=SF::eraseStuffBetween(allD,mDNameB,mDNameE,pos).first;

      allD+=mDNameB+mainDataName+mDNameE;

      pos=0;
      std::string newTableDBInitStr = SF::eraseStuffBetween(tableDBInitStr,lWithinFileB,lWithinFileE,pos).first;
      newTableDBInitStr+= lWithinFileB+allD+lWithinFileE;

      return newTableDBInitStr;
  }

  int Table::initTableFromStr(const std::string &stToInitFrom){
    std::string lWithinFileB="_table*"+tableName+"!*_";
    std::string lWithinFileE="_/table*"+tableName+"!*_";
    std::string nmKeysB="_nmKeys!*_";
    std::string nmKeysE="_/nmKeys!*_";

    std::string allKeyNamesB="_keyNames!*_";
    std::string allKeyNamesE="_/keyNames!*_";

    std::string mDNameB="_mainDataName!*_";
    std::string mDNameE="_/mainDataName!*_";


    std::string combKSKW="_*combKeySearchKeyword!*_";

    std::string nextKeyNameB="_n!*_";
    std::string nextKeyNameE="_/n!*_";

      long pos=0;
      std::pair<std::string,int> allDataP=SF::extract(stToInitFrom,pos,lWithinFileB,lWithinFileE);
      if(allDataP.second==0){
          return 0;
      }

      pos=0;
      std::pair<std::string,int> numKeysStP=SF::extract(allDataP.first,pos,nmKeysB,nmKeysE);
      if(numKeysStP.second==0){
          return 0;
      }
      long numKeys=BF::stringToInteger(numKeysStP.first);

      if(numKeys<1){
          return 0;
      }

      std::vector< std::pair<std::string, int> > kNamesP;
      kNamesP.resize(numKeys);

      pos=0;
      long i=0;int good=1;
      while((i<numKeys)&&(good=1)){
          kNamesP[i]=SF::extract(allDataP.first,pos,nextKeyNameB,nextKeyNameE);
          good=(kNamesP[i]).second;
          ++i;
      }
      if(good==0){
        return 0;
      }
      keyNames.resize(numKeys);

      for(i=0;i<numKeys;++i){
        keyNames[i]=kNamesP[i].first;
      }

      good *= combinationKeySearch.loadInitializationFromString(allDataP.first,combKSKW);
      startingFolderName=combinationKeySearch.getStartingFolderName();
      pos=0;
      mainDataName=SF::extract(allDataP.first,pos,mDNameB,mDNameE).first;

      reCreateSearchMapForKeys();
      return good;

  }
  std::string Table::select(const std::vector<std::string> & keyV) const{
    if(keyV.size()!=keyNames.size()){
      return "notFound";
    }
    HDDBRF::Record tmp;
    tmp.setKeys(keyV);
    IRFNF::IndRecFName indRFN=findIndexRecordFileName(tmp);
    if(indRFN.fileName=="notFound"){
      return "notFound";
    }
    std::string dfn=DMF::dataFNameFromIRFN(indRFN);
    std::string da=IOF::fileToString(dfn);
    if(da=="fileNotFound"){
      return "";
    }
    long pos;std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(da,pos,"_dB*!_","_/dB*!_");
    if(allD.second==1){
      da=allD.first;
    }


    return da;
  }
  IRFNF::IndRecFName<HDDBRF::Record> Table::findIndexRecordFileName(const HDDBRF::Record & dbr) const{

    IRFNF::IndRecFName<HDDBRF::Record> res;
    res=findInSavedSearches(dbr);
    if(res.index>-1){
      return res;
    }
    res=combinationKeySearch.findIndexRecordFileNameQuick(savedSearches,getTableIdForSavedSearches(), dbr);
    if(res.index==-1){
      return res;
    }
    HBF::Node<HDDBRF::Record> entireNode=(res.aRecord).deRefRead();
    std::vector<IRFNF::IndRecFName<HDDBRF::Record> > allTerms;
    if(entireNode.num>0){
      allTerms.resize(entireNode.num);
      if((res.positionInNode>-1)&&(res.positionInNode<entireNode.num)){
        allTerms[res.positionInNode]=res;
        for(long i=res.positionInNode+1;i<entireNode.num;++i){
          allTerms[i]=res;
          allTerms[i].positionInNode=i;
          allTerms[i].record=entireNode.values[i];
          allTerms[i].index=allTerms[i-1].index+1+entireNode.counts[i];
        }
        for(long i=res.positionInNode-1;i>-1;--i){
          allTerms[i]=res;
          allTerms[i].positionInNode=i;
          allTerms[i].record=entireNode.values[i];
          allTerms[i].index=allTerms[i+1].index-1-entireNode.counts[i+1];
        }
      }

    }

    return res;
  }
  IRFNF::IndRecFName<HDDBRF::Record> Table::lowerBoundIndexRecordFileName(const HDDBRF::Record & dbr, const long & populateRecordFieldEvenIfMissmatch) const{
    return combinationKeySearch.lowerBoundIndexRecordFileName(dbr, populateRecordFieldEvenIfMissmatch);
  }
  long Table::find(const std::vector<std::string> & keyV) const{
    if(keyV.size()!=keyNames.size()){
      return -1;
    }
    HDDBRF::Record tmp;
    tmp.setKeys(keyV);
    return findIndexRecordFileName(tmp).index;
  }

  long Table::findLowerBound(const std::vector<std::string> & keyV) const{
    if(keyV.size()!=keyNames.size()){
      return -1;
    }
    HDDBRF::Record tmp;
    tmp.setKeys(keyV);
    return lowerBoundIndexRecordFileName(tmp).index;
  }

  int Table::delRow(const std::vector<std::string> &keyV){

    HDDBRF::Record tmp;
    tmp.setKeys(keyV);
    std::string dFName;
    IRFNF::IndRecFName<HDDBRF::Record> indRFN=findIndexRecordFileName(tmp);
    std::string fName=indRFN.fileName;
    if(fName=="notFound"){
      return 0;
    }
    combinationKeySearch -= tmp;
    clearSavedSearches();
    // These two commands must be used if the database is AVL
    // They must be commented out if the database is B-tree
    // dFName=DMF::dataFNameFromIRFN(indRFN);
    // IO::sys_deleteFile(dFName);
    return 1;
  }
  int Table::insertOrUpdateAndGiveReport(const std::vector<std::string> & keyV, const std::string & da){
    //function returns the report;
    // 0 - key is of wrong size, or data is corrupted: did nothing;
    // 1 - inserted new entry
    // 2 - updated old entry
    int report=0;
    if(keyV.size()!=keyNames.size()){
      return report;
    }
    std::string oldText="",newText="";
    HDDBRF::Record tmp;
    tmp.setKeys(keyV);
    std::string dFName;
    IRFNF::IndRecFName<HDDBRF::Record> indRFN= findIndexRecordFileName(tmp);
    std::string fName=indRFN.fileName;
    if(fName=="notFound"){
      combinationKeySearch += tmp;
      clearSavedSearches();
      report=1;
      indRFN= findIndexRecordFileName(tmp);
      fName=(indRFN).fileName;

    }
    else{
      report=2;
    }
    dFName=DMF::dataFNameFromIRFN(indRFN);


    if(report==2){
      oldText=IOF::fileToString(dFName);
    }

    long pos=0;
    std::pair<std::string,int> allD;
    std::string sepB="_dB*!_";std::string sepE="_/dB*!_";
    newText=sepB+da+sepE;
    allD=SF::extractAndReplace(oldText,pos,sepB,sepE,0,newText);
    if(allD.second==1){
        IOF::toFile(dFName,allD.first);
        return report;
    }
    IOF::toFile(dFName,newText);



    return report;
  }

  int Table::inPlaceKeyDataModification(const std::vector<std::string> & keyOld, const std::vector<std::string> & keyNew, const std::string & da, const long & dataUpdateIndicator, const long & _safeMode){
    if(keyOld.size()!=keyNames.size()){
      return 0;
    }
    if(keyNew.size()!=keyNames.size()){
      return 0;
    }
    int reportDataUpdate = -1;
    if(dataUpdateIndicator==1){
      reportDataUpdate=insertOrUpdateAndGiveReport(keyOld,da);
    }
    HDDBRF::Record tmpOld, tmpNew;
    tmpOld.setKeys(keyOld); tmpNew.setKeys(keyNew);
    return combinationKeySearch.inPlaceModificationQuick(savedSearches,getTableIdForSavedSearches(), tmpOld,tmpNew,_safeMode);

  }
  long Table::size() const{
    return combinationKeySearch.size();
  }
  std::pair<HDDBRF::Record,std::string> Table::operator[](const long & i) const{
    return combinationKeySearch[i];

  }
  void Table::clear(){
    combinationKeySearch.clear();
  }
}
#endif
