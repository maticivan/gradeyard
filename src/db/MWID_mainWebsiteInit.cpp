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

#ifndef _INCL_MAINWEBSITEITIT_CPP
#define _INCL_MAINWEBSITEITIT_CPP
namespace MWID{

  class MainDB{
  public:
    std::set<std::string> tablesJustCreated;
    std::set<std::string> fut_tablesJustCreated;
    std::map<std::string,MTF::Table> dbsM;
    std::map<std::string,FUTF::FastUpdatingTable> fu_dbsM;
    void initialize();
    void executeQueues();
    long numUsers() const;
    long numFiles() const;
    long numMessages() const;
    long numCoursesAssignments() const;
    long numCertificates() const;
    long numTexts() const;
    long numResponses() const;
    long numStats() const;
    long numStatsAdv() const;
    long numBackups() const;
    long saveForRecovery(const std::string &, const long & , const long & , const std::string & ) const;
    void putInitialDataInTable(const std::string &);
    MTF::Table createTableWithOneKey(const std::string &, const std::string &, const std::string &, const std::string &, const std::string &);
    MTF::Table createTableWithTwoKeys(const std::string &, const std::string &, const std::string &, const std::string &, const std::string &, const std::string &);
    FUTF::FastUpdatingTable createFastUpdatingTable(const std::string &,const std::string &,const std::string &);
    std::string statusReportForDebugging() const;
  };
  std::string MainDB::statusReportForDebugging() const{
    std::string fR;
    std::map<std::string,MTF::Table>::const_iterator it;
    std::map<std::string,MTF::Table>::const_iterator itE;
    itE=dbsM.end();
    it=dbsM.begin();
    while(it!=itE){
      fR+="Size of "+it->first+" is "+std::to_string((it->second).size())+"\n";
      ++it;
    }
    it=dbsM.begin();
    while(it!=itE){
      fR+="\n\n"+it->first+" status report: \n";
      fR+=(it->second).statusReportForDebugging();
      ++it;
    }
    return fR;
  }
  long MainDB::numUsers() const{
    std::map<std::string,MTF::Table>::const_iterator it;
    it=dbsM.find("usernames");
    return (it->second).size();
  }
  long MainDB::numFiles() const{
    std::map<std::string,MTF::Table>::const_iterator it;
    it=dbsM.find("fileMan");
    return (it->second).size();
  }
  long MainDB::numMessages() const{
    std::map<std::string,MTF::Table>::const_iterator it;
    it=dbsM.find("messMan");
    return (it->second).size();
  }
  long MainDB::numCoursesAssignments() const{
    std::map<std::string,MTF::Table>::const_iterator it;
    it=dbsM.find("couasMan");
    return (it->second).size();
  }
  long MainDB::numCertificates() const{
    std::map<std::string,MTF::Table>::const_iterator it;
    it=dbsM.find("certMan");
    return (it->second).size();
  }
  long MainDB::numBackups() const{
    std::map<std::string,MTF::Table>::const_iterator it;
    it=dbsM.find("backupDatabase");
    return (it->second).size();
  }
  long MainDB::numTexts() const{
    std::map<std::string,MTF::Table>::const_iterator it;
    it=dbsM.find("mainText");
    return (it->second).size();
  }
  long MainDB::numResponses() const{
    std::map<std::string,MTF::Table>::const_iterator it;
    it=dbsM.find("response");
    return (it->second).size();
  }
  long MainDB::numStats() const{
    std::map<std::string,MTF::Table>::const_iterator it;
    it=dbsM.find("stat");
    return (it->second).size();
  }
  long MainDB::numStatsAdv() const{
    std::map<std::string,FUTF::FastUpdatingTable>::const_iterator it;
    it=fu_dbsM.find("fStat");
    return (it->second).size();
  }
  MTF::Table MainDB::createTableWithOneKey(const std::string & _internalTableNickName,
                                                 const std::string & _folderName,
                                                 const std::string & _tabName,
                                                 const std::string & _key0,
                                                 const std::string & _mDN){
    MTF::Table tb;
    int tbCreated=tb.setFolderNames(_folderName);
    tb.setTableName(_tabName);
    std::vector<std::string> vKeys;
    vKeys.resize(1);
    vKeys[0]=_key0;
    tb.setKeyNames(vKeys);
    tb.setMainDataName(_mDN);
    if(tbCreated==1){
      tablesJustCreated.insert(_internalTableNickName);
    }
    return tb;
  }

  MTF::Table MainDB::createTableWithTwoKeys(const std::string & _internalTableNickName,
                                                  const std::string & _folderName,
                                                  const std::string & _tabName,
                                                  const std::string & _key0,
                                                  const std::string & _key1,
                                                  const std::string & _mDN){
    MTF::Table tb;
    int tbCreated=tb.setFolderNames(_folderName);
    tb.setTableName(_tabName);
    std::vector<std::string> vKeys;
    vKeys.resize(2);
    vKeys[0]=_key0;
    vKeys[1]=_key1;
    tb.setKeyNames(vKeys);
    tb.setMainDataName(_mDN);
    if(tbCreated==1){
      std::vector<std::string> index;
      index.resize(1);
      index[0]=_key0;
      tb.createIndex(index);
      index[0]=_key1;
      tb.createIndex(index);
      tablesJustCreated.insert(_internalTableNickName);
    }
    return tb;
  }

  FUTF::FastUpdatingTable MainDB::createFastUpdatingTable(const std::string & _internalTableNickName,
                                                    const std::string & _folderName,
                                                    const std::string & _tabName){
      FUTF::FastUpdatingTable tb;
      int tbCreated=tb.setFolderName(_folderName);
      tb.setTableName(_tabName);
      if(tbCreated==1){
        fut_tablesJustCreated.insert(_internalTableNickName);
      }
      return tb;
  }
  std::string newItemFormatted(const std::pair<std::vector<std::string>, std::string> & item, const std::string & sB, const std::string &sE, const std::string &ssB, const std::string &ssE){
    std::string formattedItem;
    formattedItem+=sB+"\n";
    formattedItem+=ssB+"insert"+ssE+"\n";
    if(item.first.size() > 0){formattedItem+=ssB+item.first[0]+ssE+"\n";}
    if(item.first.size()>1){formattedItem+=ssB+item.first[1]+ssE+"\n";}
    formattedItem+=ssB+HDDBSF::unpackFromStorage(item.second)+ssE+"\n";
    formattedItem+=sE+"\n";
    return formattedItem;
  }
  std::string newTextItemFormatted(const std::pair<std::vector<std::string>, std::string> & item){
    std::string formattedItem;
    formattedItem+="_command_\n_nc***_createText_/nc***_\nDocument name:_nc***_";
    if(item.first.size()>1){
      formattedItem+=item.first[1];
    }
    formattedItem+="_/nc***_\nSTART EDITING AFTER THIS LINE _nc***_";
    formattedItem+=HDDBSF::unpackFromStorage(item.second);
    formattedItem+="_/nc***_\nDO NOT EDIT THIS LINE  _/command_\n";
    return formattedItem;
  }

  long MainDB::saveForRecovery(const std::string &_internalTableNickName, const long & _st, const long & _en, const std::string & typeOfBackup) const{
    std::string rc;
    long start=_st;
    long end=_en;
    if(start<0){start=0;}
    std::map<std::string,MTF::Table>::const_iterator it;
    it=dbsM.find(_internalTableNickName);
    if(it!=dbsM.end()){
      long sz=(it->second).size();
      if(start > sz-1){
        start=0;end=sz;
      }
      if(start> end-1){
        start=0;end=sz;
      }
      if(end > sz){
        end=sz;
      }
      for(long i=start;i<end;++i){
        std::pair<std::vector<std::string>, std::string> itemI=(it->second)[i];
        if(typeOfBackup=="init"){
          rc+=newItemFormatted(itemI,"_nextCommand!*!!_", "_/nextCommand!*!!_", "_n*!!***!_","_/n*!!***!_");
        }
        if(typeOfBackup=="text"){
          rc+=newTextItemFormatted(itemI);
        }
      }
      std::string fToSave=(it->second).getTableName();
      fToSave+=DD::GL_DBS.getInitExtension()+"_"+std::to_string(start);
      fToSave+=".txt";
      IOF::toFile(fToSave,rc);
      return 1;
    }
    return 0;
  }
  void MainDB::putInitialDataInTable(const std::string &_internalTableNickName){
    std::string tableName=dbsM[_internalTableNickName].getTableName();
    std::string fToInit=tableName;
    fToInit+=DD::GL_DBS.getInitExtension();
    fToInit+=".txt";
    std::string initCommands=IOF::fileToString(fToInit);
    if(initCommands!="fileNotFound"){
      std::vector<std::vector<std::string> > allCommands=SF::stringToMatrix(initCommands, "_nextCommand!*!!_", "_/nextCommand!*!!_", "_n*!!***!_", "_/n*!!***!_");
      long sz=allCommands.size(),szIn;
      std::vector<std::string> kV;
      for(long i=0;i<sz;++i){
        szIn=allCommands[i].size();
        if(szIn>2){
          if(allCommands[i][0]=="insert"){
            kV.resize(szIn-2);
            for(long j=0;j<szIn-2;++j){
              kV[j]=allCommands[i][j+1];
            }
            dbsM[_internalTableNickName].insertMTF(kV,allCommands[i][szIn-1]);
          }
        }
      }
    }
  }
  void MainDB::initialize(){
    IOF::sys_createFolderIfDoesNotExist(DD::GL_DBS.getMainFolder(),"readme.txt","Do not edit this folder");
    tablesJustCreated.clear();
    fut_tablesJustCreated.clear();
    std::vector<std::string> vKeys;
    // externalIds initialization
    dbsM["externalIds"]=createTableWithTwoKeys("externalIds", DD::GL_DBS.getExternalIDDB(),"eIDT","externalId","internalId","hierarchyCache");
    //usernames initialization
    dbsM["usernames"]=createTableWithTwoKeys("usernames", DD::GL_DBS.getUserNameDB(),"uns","internalId","username","otherUserData");
    // hierarchy initialization
    dbsM["hierarchy"]=createTableWithTwoKeys("hierarchy", DD::GL_DBS.getHierarchy(),"bossDB","high","low","hData");
    // counters initialization
    dbsM["counters"]=createTableWithOneKey("counters", DD::GL_DBS.getCounters(),"sysCounters","counterName","counterD");
    // file manager initialization
    dbsM["fileMan"]=createTableWithTwoKeys("fileMan", DD::GL_DBS.getFileManage(),"fMan","fileId","userId","fData");
    // message manager initialization
    dbsM["messMan"]=createTableWithTwoKeys("messMan", DD::GL_DBS.getMessManage(),"mMan","messId","userId","messData");
    // courses and assignments manager initialization
    dbsM["couasMan"]=createTableWithTwoKeys("couasMan", DD::GL_DBS.getCouasManage(),"cMan","couasId","userId","couasData");
    // certificates manager initialization
    dbsM["certMan"]=createTableWithTwoKeys("certMan", DD::GL_DBS.getCertManage(),"crtMan","intId","mainId","certData");
    // database of backups for undoing the administrator actions
    dbsM["backupDatabase"]=createTableWithOneKey("backupDatabase", DD::GL_DBS.getBackupDB(),"bDb","name","bData");
    // main text initialization
    dbsM["mainText"]=createTableWithTwoKeys("mainText", DD::GL_DBS.getMainText(),"mainText","id","name","tData");
    // responses initialization
    dbsM["response"]=createTableWithTwoKeys("response", DD::GL_DBS.getResponseTable(),"response","id","name","tData");
    // stat initialization
    dbsM["stat"]=createTableWithOneKey("stat", DD::GL_DBS.getStatTable(),"stat","name","tData");
    // fast updating stat saveInitialization
    fu_dbsM["fStat"]=createFastUpdatingTable("fStat",DD::GL_DBS.getFastUpdatingStatTable(),"fStat");
    executeQueues();
    if(!tablesJustCreated.empty()){
      std::set<std::string>::iterator it,itE;
      it=tablesJustCreated.begin();itE=tablesJustCreated.end();
      while(it!=itE){
        putInitialDataInTable(*it);
        ++it;
      }
    }
  }
  void MainDB::executeQueues(){
    std::map<std::string,MTF::Table>::iterator it;
    std::map<std::string,MTF::Table>::iterator itE;
    itE=dbsM.end();
    it=dbsM.begin();
    while(it!=itE){
      if(it->first!="counters"){
        (it->second).executeFromQueue();
      }
      ++it;
    }
  }
}
#endif
