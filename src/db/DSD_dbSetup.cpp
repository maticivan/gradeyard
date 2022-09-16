//    GradeYard learning management system
//
//    Copyright (C) 2022 Ivan Matic, https://gradeyard.com
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


#ifndef _INCL_Setup_CPP
#define _INCL_Setup_CPP
namespace DSD{
  class Setup{
  private:
      std::string mfB="_mainFolderDB*!_";
      std::string mfE="_/mainFolderDB*!_";
      std::string eIdB="_externalIDDB*!_";
      std::string eIdE="_/externalIDDB*!_";
      std::string uNDBB="_usernameDB*!_";
      std::string uNDBE="_/usernameDB*!_";
      std::string hDBB="_hierarchy*!_";
      std::string hDBE="_/hierarchy*!_";
      std::string hCDBB="_hierarchyCache*!_";
      std::string hCDBE="_/hierarchyCache*!_";
      std::string cDBB="_countersDB*!_";
      std::string cDBE="_/countersDB*!_";
      std::string fDBB="_fileManagement*!_";
      std::string fDBE="_/fileManagement*!_";
      std::string messDBB="_messageManagement*!_";
      std::string messDBE="_/messageManagement*!_";
      std::string couasDBB="_couasManagement*!_";
      std::string couasDBE="_/couasManagement*!_";
      std::string backupDBB="_backupDB*!_";
      std::string backupDBE="_/backupDB*!_";
      std::string backupMaxSZB="_backupMaxSZ*!_";
      std::string backupMaxSZE="_/backupMaxSZ*!_";
      std::string mTBB="_mainText*!_";
      std::string mTBE="_/mainText*!_";
      std::string rPBB="_responseDB*!_";
      std::string rPBE="_/responseDB*!_";
      std::string staPBB="_statDB*!_";
      std::string staPBE="_/statDB*!_";
      std::string fustaPBB="_fstDB*!_";
      std::string fustaPBE="_/fstDB*!_";
      std::string initTB="_initializerExtension*!_";
      std::string initTE="_/initializerExtension*!_";
      std::string sFSFB="_publicSubFolderToStoreFiles*!_";
      std::string sFSFE="_/publicSubFolderToStoreFiles*!_";
      std::string sFSFSysB="_publicSubFolderForSystemFiles*!_";
      std::string sFSFSysE="_/publicSubFolderForSystemFiles*!_";
      std::string sFSFGClB="_subFolderForGuestClones*!_";
      std::string sFSFGClE="_/subFolderForGuestClones*!_";
      std::string st_chImB="_publicSubFolderToStoreChallengeImages*!_";
      std::string st_chImE="_/publicSubFolderToStoreChallengeImages*!_";
      std::string st_chAnB="_privateSubFolderToStoreChallengeAnswers*!_";
      std::string st_chAnE="_/privateSubFolderToStoreChallengeAnswers*!_";
      std::string nSSFIFB="_numStorageSubFoldersInFolder*!_";
      std::string nSSFIFE="_/numStorageSubFoldersInFolder*!_";
      std::string nFIFB="_numFilesInFolder*!_";
      std::string nFIFE="_/numFilesInFolder*!_";
      std::string dcMountFolderB="_dcMountingFolder*!_";
      std::string dcMountFolderE="_/dcMountingFolder*!_";
      std::string dImageNameB="_dImageName*!_";
      std::string dImageNameE="_/dImageName*!_";
      std::string dMaxCodesToRunB="_codesToAutoGrade*!_";
      std::string dMaxCodesToRunE="_/codesToAutoGrade*!_";
      std::string default_extIdDB="mainFolderDB/externalIDDB";
      std::string default_mainFolder="mainFolderDB";
      std::string default_userNameDB="mainFolderDB/userNameDB";
      std::string default_hierarchy="mainFolderDB/hierarchyDB";
      std::string default_hCache="mainFolderDB/hierarchyCache";
      std::string default_counters="mainFolderDB/countersDB";
      std::string default_fManage="mainFolderDB/fileManDB";
      std::string default_messManage="mainFolderDB/messManDB";
      std::string default_couasManage="mainFolderDB/couasManDB";
      std::string default_backupDB="mainFolderDB/backupDB";
      long default_backupMaxSZ=500;
      std::string default_mText="mainFolderDB/mainTextDB";
      std::string default_responseTable="mainFolderDB/responseDB";
      std::string default_statTable="mainFolderDB/statDB";
      std::string default_fastUpdatingStatTable="mainFolderDB/fuStatDB";
      std::string default_initExtension="_initFile";
      std::string default_publicStorage="q0";
      std::string default_publicSystemStorage="qs0";
      std::string default_guestClonesRelLoc="st";
      std::string default_challengeImages="c0";
      std::string default_challengeAnswers="mainFolderDB/a0";
      std::string default_dcMountFolder="dmountF11";
      std::string default_dImageName="ub_cpp_py11";
      long default_maxCodesToRun=10;
      long default_numSubFolders=100;
      long default_numFilesInFolder=100;
      std::string mainFolder=default_mainFolder;
      std::string extIdDB=default_extIdDB;
      std::string userNameDB=default_userNameDB;
      std::string hierarchy=default_hierarchy;
      std::string counters=default_counters;
      std::string fManage=default_fManage;
      std::string messManage=default_messManage;
      std::string couasManage=default_couasManage;
      std::string backupDB=default_backupDB;
      long backupMaxSZ=default_backupMaxSZ;
      std::string mText=default_mText;
      std::string responseTable=default_responseTable;
      std::string statTable=default_statTable;
      std::string fastUpdatingStatTable=default_fastUpdatingStatTable;
      std::string initExtension=default_initExtension;
      std::string hCache=default_hCache;
      std::string publicStorage=default_publicStorage;
      std::string publicSystemStorage=default_publicSystemStorage;
      std::string folderMainDBGuestClone;
      std::string systemFolderClones;
      std::string guestClonesRelLoc=default_guestClonesRelLoc;
      std::string challengeImages=default_challengeImages;
      std::string challengeAnswers=default_challengeAnswers;
      std::string dcMountFolder;
      std::string dImageName;
      long maxCodesToRun;
      long numSubFolders=default_numSubFolders;
      long numFilesInFolder=default_numFilesInFolder;
      std::string extIdDB_jN;
      std::string userNameDB_jN;
      std::string hierarchy_jN;
      std::string counters_jN;
      std::string fManage_jN;
      std::string messManage_jN;
      std::string couasManage_jN;
      std::string backupDB_jN;
      std::string mText_jN;
      std::string responseTable_jN;
      std::string statTable_jN;
      std::string fastUpdatingStatTable_jN;
      std::string hCache_jN;
      std::string chIm_jN;
      std::string chAnsw_jN;
      std::string dcMountFolder_jN;
  public:
      void getSetupFromString(const std::string &);
      std::string getMainFolder() const;
      std::string getExternalIDDB() const;
      std::string getUserNameDB() const;
      std::string getHierarchy() const;
      std::string getHCache() const;
      std::string getCounters() const;
      std::string getFileManage() const;
      std::string getMessManage() const;
      std::string getCouasManage() const;
      std::string getBackupDB() const;
      long getBackupMaxSZ() const;
      std::string getMainText() const;
      std::string getResponseTable() const;
      std::string getStatTable() const;
      std::string getFastUpdatingStatTable() const;
      std::string getInitExtension() const;
      std::string printSetup() const;
      std::string getPublicStorage() const;
      std::string getPublicSystemStorage() const;
      std::string getGuestClonesRelLoc() const;
      std::string getChallengeImStorage() const;
      std::string getChallengeAnswStorage() const;
      std::string getClonesMainDBFolder() const;
      std::string getClonesSystemFolder() const;
      std::string getMountFolder() const;
      std::string getImageName() const;
      long getMaxCodesToRun() const;
      long getNumSubFolders() const;
      long getNumFilesInFolder() const;
  };
  std::string Setup::getMainFolder() const{return mainFolder;}
  std::string Setup::getExternalIDDB() const{    return extIdDB; }
  std::string Setup::getUserNameDB() const{ return userNameDB;}
  std::string Setup::getHierarchy() const{return hierarchy;}
  std::string Setup::getCounters() const{return counters;}
  std::string Setup::getFileManage() const{return fManage;}
  std::string Setup::getMessManage() const{return messManage;}
  std::string Setup::getCouasManage() const{return couasManage;}
  std::string Setup::getBackupDB() const{return backupDB;}
  long Setup::getBackupMaxSZ() const{return backupMaxSZ;}
  std::string Setup::getMainText() const{return mText;}
  std::string Setup::getResponseTable() const{return responseTable;}
  std::string Setup::getStatTable() const{return statTable;}
  std::string Setup::getFastUpdatingStatTable() const{return fastUpdatingStatTable;}
  std::string Setup::getInitExtension() const{return initExtension;}
  std::string Setup::getHCache() const{return hCache;}
  std::string Setup::getPublicStorage() const{return publicStorage;}
  std::string Setup::getPublicSystemStorage() const{return publicSystemStorage;}
  std::string Setup::getGuestClonesRelLoc() const{return guestClonesRelLoc;}
  std::string Setup::getChallengeImStorage() const{return challengeImages;}
  std::string Setup::getChallengeAnswStorage() const{return challengeAnswers;}
  std::string Setup::getClonesMainDBFolder() const{return folderMainDBGuestClone;}
  std::string Setup::getClonesSystemFolder() const{return systemFolderClones;}
  std::string Setup::getMountFolder() const{return dcMountFolder;}
  std::string Setup::getImageName() const{return dImageName;}
  long Setup::getMaxCodesToRun() const{return maxCodesToRun;}
  long Setup::getNumSubFolders() const{return numSubFolders;}
  long Setup::getNumFilesInFolder() const{return numFilesInFolder;}
  void Setup::getSetupFromString(const std::string & fN){
      std::string sSt= fN ;
      sSt += mfB+default_mainFolder+mfE;
      sSt += eIdB+default_extIdDB+eIdE;
      sSt += uNDBB+default_userNameDB+uNDBE;
      sSt += hDBB+default_hierarchy+hDBE;
      sSt += hCDBB+default_hCache+hCDBE;
      sSt += cDBB+default_counters+cDBE;
      sSt += fDBB+default_fManage+fDBE;
      sSt += messDBB+default_messManage+messDBE;
      sSt += couasDBB+default_couasManage+couasDBE;
      sSt += backupDBB+default_backupDB+backupDBE;
      sSt += backupMaxSZB+std::to_string(default_backupMaxSZ)+backupMaxSZE;
      sSt += mTBB+default_mText+mTBE;
      sSt += rPBB+default_responseTable+rPBE;
      sSt += staPBB+default_statTable+staPBE;
      sSt += fustaPBB+default_fastUpdatingStatTable+fustaPBE;
      sSt += initTB+ default_initExtension+initTE;
      sSt += sFSFB +default_publicStorage+sFSFE;
      sSt += sFSFSysB + default_publicSystemStorage+sFSFSysE;
      sSt += sFSFGClB + default_guestClonesRelLoc+ sFSFGClE;
      sSt += nSSFIFB+std::to_string(default_numSubFolders)+nSSFIFE;
      sSt += nFIFB+std::to_string(default_numFilesInFolder)+ nFIFE;
      sSt += st_chImB+default_challengeImages+st_chImE;
      sSt += st_chAnB+default_challengeAnswers+st_chAnE;
      sSt +=  dcMountFolderB + default_dcMountFolder+ dcMountFolderE;
      sSt += dImageNameB +default_dImageName+ dImageNameE;
      sSt += dMaxCodesToRunB+std::to_string(default_maxCodesToRun)+dMaxCodesToRunE;
      long pos=0;
      mainFolder=(SF::extract(sSt,pos,mfB,mfE)).first;
      pos=0;
      extIdDB_jN=(SF::extract(sSt,pos,eIdB,eIdE)).first;
      extIdDB=mainFolder+"/"+extIdDB_jN;
      pos=0;
      userNameDB_jN=(SF::extract(sSt,pos,uNDBB,uNDBE)).first;
      userNameDB=mainFolder+"/"+userNameDB_jN;
      pos=0;
      hierarchy_jN=(SF::extract(sSt,pos,hDBB,hDBE)).first;
      hierarchy=mainFolder+"/"+hierarchy_jN;
      pos=0;
      hCache_jN=(SF::extract(sSt,pos,hCDBB,hCDBE)).first;
      hCache=mainFolder+"/"+hCache_jN;
      pos=0;
      counters_jN=(SF::extract(sSt,pos,cDBB,cDBE)).first;
      counters=mainFolder+"/"+counters_jN;
      pos=0;
      fManage_jN=(SF::extract(sSt,pos,fDBB,fDBE)).first;
      fManage=mainFolder+"/"+fManage_jN;
      pos=0;
      messManage_jN=(SF::extract(sSt,pos,messDBB,messDBE)).first;
      messManage=mainFolder+"/"+messManage_jN;
      pos=0;
      couasManage_jN=(SF::extract(sSt,pos,couasDBB,couasDBE)).first;
      couasManage=mainFolder+"/"+couasManage_jN;
      pos=0;
      backupDB_jN=(SF::extract(sSt,pos,backupDBB,backupDBE)).first;
      backupDB=mainFolder+"/"+backupDB_jN;
      pos=0;
      backupMaxSZ=BF::stringToInteger(SF::extract(sSt,pos,backupMaxSZB,backupMaxSZE).first);
      pos=0;
      mText_jN=(SF::extract(sSt,pos,mTBB,mTBE)).first;
      mText=mainFolder+"/"+mText_jN;
      pos=0;
      responseTable_jN=(SF::extract(sSt,pos,rPBB,rPBE)).first;
      responseTable=mainFolder+"/"+responseTable_jN;
      pos=0;
      statTable_jN=(SF::extract(sSt,pos,staPBB,staPBE)).first;
      statTable=mainFolder+"/"+statTable_jN;
      pos=0;
      fastUpdatingStatTable_jN=(SF::extract(sSt,pos,fustaPBB,fustaPBE)).first;
      fastUpdatingStatTable=mainFolder+"/"+fastUpdatingStatTable_jN;
      pos=0;
      chIm_jN=(SF::extract(sSt,pos,st_chImB,st_chImE)).first;
      challengeImages=chIm_jN;
      pos=0;
      chAnsw_jN=(SF::extract(sSt,pos,st_chAnB,st_chAnE)).first;
      challengeAnswers=mainFolder+"/"+chAnsw_jN;
      pos=0;
      dcMountFolder_jN=(SF::extract(sSt,pos,dcMountFolderB,dcMountFolderE)).first;
      dcMountFolder=mainFolder+"/"+dcMountFolder_jN;
      pos=0;
      dImageName=(SF::extract(sSt,pos,dImageNameB,dImageNameE)).first;
      pos=0;
      maxCodesToRun=BF::stringToInteger((SF::extract(sSt,pos,dMaxCodesToRunB,dMaxCodesToRunE)).first);
      pos=0;
      initExtension=SF::extract(sSt,pos,initTB,initTE).first;
      pos=0;
      publicStorage=SF::extract(sSt,pos,sFSFB,sFSFE).first;
      pos=0;
      publicSystemStorage=SF::extract(sSt,pos,sFSFSysB,sFSFSysE).first;
      pos=0;
      guestClonesRelLoc=SF::extract(sSt,pos,sFSFGClB,sFSFGClE).first;
      folderMainDBGuestClone=mainFolder+"/"+guestClonesRelLoc;
      systemFolderClones=mainFolder+"/"+"sys_"+guestClonesRelLoc;
      if(guestClonesRelLoc.length()>0){
        if(guestClonesRelLoc[guestClonesRelLoc.length()-1]!='/'){
          guestClonesRelLoc+="/";
        }
      }
      pos=0;
      numSubFolders=BF::stringToInteger(SF::extract(sSt,pos,nSSFIFB,nSSFIFE).first);
      pos=0;
      numFilesInFolder=BF::stringToInteger(SF::extract(sSt,pos,nFIFB,nFIFE).first);
  }
  std::string Setup::printSetup() const{
      std::string fR=mainFolder+"\n";
      fR+=extIdDB+"\n";
      fR+=userNameDB+"\n";
      fR+=hierarchy+"\n";
      fR+=hCache+"\n";
      fR+=counters+"\n";
      fR+=fManage+"\n";
      fR+=messManage+"\n";
      fR+=couasManage+"\n";
      fR+=backupDB+"\n";
      fR+=std::to_string(backupMaxSZ);
      fR+=mText+"\n";
      fR+=responseTable+"\n";
      fR+=statTable+"\n";
      fR+=fastUpdatingStatTable+"\n";
      fR+=initExtension+"\n";
      fR+=publicStorage+"\n";
      fR+=publicSystemStorage+"\n";
      fR+=guestClonesRelLoc+"\n";
      fR+=challengeImages+"\n";
      fR+=challengeAnswers+"\n";
      fR+=std::to_string(numSubFolders);
      fR+=std::to_string(numFilesInFolder);
      return fR;
  }
}
#endif
