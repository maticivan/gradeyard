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


#ifndef _INCL_Setup_CPP
#define _INCL_Setup_CPP
namespace DSD{
  class Setup{
  private:
      std::string default_extIdDB="mainFolderDB/externalIDDB";
      std::string default_mainFolder="mainFolderDB";
      std::string default_userNameDB="mainFolderDB/userNameDB";
      std::string default_hierarchy="mainFolderDB/hierarchyDB";
      std::string default_hCache="mainFolderDB/hierarchyCache";
      std::string default_counters="mainFolderDB/countersDB";
      std::string default_fManage="mainFolderDB/fileManDB";
      std::string default_messManage="mainFolderDB/messManDB";
      std::string default_couasManage="mainFolderDB/couasManDB";
      std::string default_certManage="mainFolderDB/certManDB";
      std::string default_backupDB="mainFolderDB/backupDB";
      long default_backupMaxSZ=500;
      std::string default_mText="mainFolderDB/mainTextDB";
      std::string default_responseTable="mainFolderDB/responseDB";
      std::string default_statTable="mainFolderDB/statDB";
      std::string default_fileForms="mainFolderDB/fForms"; 
      std::string default_initExtension="_initFile";
      std::string default_publicStorage="q0";
      std::string default_publicSystemStorage="qs0";
      std::string default_publicPDFCertStorage="pdfcrt0";
      std::string default_publicPDFFormStorage="pdffrm0";
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
      std::string certManage=default_certManage;
      std::string backupDB=default_backupDB;
      long backupMaxSZ=default_backupMaxSZ;
      std::string mText=default_mText;
      std::string responseTable=default_responseTable;
      std::string statTable=default_statTable;
      std::string fileForms=default_fileForms; 
      std::string initExtension=default_initExtension;
      std::string hCache=default_hCache;
      std::string publicStorage=default_publicStorage;
      std::string publicSystemStorage=default_publicSystemStorage;
      std::string publicPDFCertStorage=default_publicPDFCertStorage;
      std::string publicPDFFormStorage=default_publicPDFFormStorage;
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
      std::string certManage_jN;
      std::string backupDB_jN;
      std::string mText_jN;
      std::string responseTable_jN;
      std::string statTable_jN;
      std::string fileForms_jN; 
      std::string hCache_jN;
      std::string chIm_jN;
      std::string chAnsw_jN;
      std::string dcMountFolder_jN;
  public:
      void getSetupFromMap(const std::map<std::string,std::string> &);
      std::string getMainFolder() const;
      std::string getExternalIDDB() const;
      std::string getUserNameDB() const;
      std::string getHierarchy() const;
      std::string getHCache() const;
      std::string getCounters() const;
      std::string getFileManage() const;
      std::string getMessManage() const;
      std::string getCouasManage() const;
      std::string getCertManage() const;
      std::string getBackupDB() const;
      long getBackupMaxSZ() const;
      std::string getMainText() const;
      std::string getResponseTable() const;
      std::string getStatTable() const;
      std::string getFileFormsFolder() const; 
      std::string getInitExtension() const;
      std::string printSetup() const;
      std::string getPublicStorage() const;
      std::string getPublicSystemStorage() const;
      std::string getPublicPDFCertStorage() const;
      std::string getPublicPDFFormStorage() const;
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
  std::string Setup::getExternalIDDB() const{return extIdDB; }
  std::string Setup::getUserNameDB() const{return userNameDB;}
  std::string Setup::getHierarchy() const{return hierarchy;}
  std::string Setup::getCounters() const{return counters;}
  std::string Setup::getFileManage() const{return fManage;}
  std::string Setup::getMessManage() const{return messManage;}
  std::string Setup::getCouasManage() const{return couasManage;}
  std::string Setup::getCertManage() const{return certManage;}
  std::string Setup::getBackupDB() const{return backupDB;}
  long Setup::getBackupMaxSZ() const{return backupMaxSZ;}
  std::string Setup::getMainText() const{return mText;}
  std::string Setup::getResponseTable() const{return responseTable;}
  std::string Setup::getStatTable() const{return statTable;}
  std::string Setup::getFileFormsFolder() const{return fileForms;} 
  std::string Setup::getInitExtension() const{return initExtension;}
  std::string Setup::getHCache() const{return hCache;}
  std::string Setup::getPublicStorage() const{return publicStorage;}
  std::string Setup::getPublicSystemStorage() const{return publicSystemStorage;}
  std::string Setup::getPublicPDFCertStorage() const{return publicPDFCertStorage;}
  std::string Setup::getPublicPDFFormStorage() const{return publicPDFFormStorage;}
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
  void Setup::getSetupFromMap(const std::map<std::string,std::string> & stMap){
    SF::assignValueFromMap(stMap,"mainFolderDB*!",mainFolder);
    SF::assignValueFromMap(stMap,"externalIDDB*!",extIdDB_jN);
    extIdDB=mainFolder+"/"+extIdDB_jN;
    SF::assignValueFromMap(stMap,"usernameDB*!",userNameDB_jN);
    userNameDB=mainFolder+"/"+userNameDB_jN;
    SF::assignValueFromMap(stMap,"hierarchy*!",hierarchy_jN);
    hierarchy=mainFolder+"/"+hierarchy_jN;
    SF::assignValueFromMap(stMap,"hierarchyCache*!",hCache_jN);
    hCache=mainFolder+"/"+hCache_jN;
    SF::assignValueFromMap(stMap,"countersDB*!",counters_jN);
    counters=mainFolder+"/"+counters_jN;
    SF::assignValueFromMap(stMap,"fileManagement*!",fManage_jN);
    fManage=mainFolder+"/"+fManage_jN;
    SF::assignValueFromMap(stMap,"messageManagement*!",messManage_jN);
    messManage=mainFolder+"/"+messManage_jN;
    SF::assignValueFromMap(stMap,"couasManagement*!",couasManage_jN);
    couasManage=mainFolder+"/"+couasManage_jN;
    SF::assignValueFromMap(stMap,"certManagement*!",certManage_jN);
    certManage=mainFolder+"/"+certManage_jN;
    SF::assignValueFromMap(stMap,"backupDB*!",backupDB_jN);
    backupDB=mainFolder+"/"+backupDB_jN;
    SF::assignValueFromMap(stMap,"backupMaxSZ*!",backupMaxSZ);
    SF::assignValueFromMap(stMap,"mainText*!",mText_jN);
    mText=mainFolder+"/"+mText_jN;
    SF::assignValueFromMap(stMap,"responseDB*!",responseTable_jN);
    responseTable=mainFolder+"/"+responseTable_jN;
    SF::assignValueFromMap(stMap,"statDB*!",statTable_jN);
    statTable=mainFolder+"/"+statTable_jN;
    SF::assignValueFromMap(stMap,"fileForms*!",fileForms_jN);
    fileForms=mainFolder+"/"+fileForms_jN; 
    SF::assignValueFromMap(stMap,"publicImages*!",chIm_jN);
    challengeImages=chIm_jN;
    SF::assignValueFromMap(stMap,"privateAnswers*!",chAnsw_jN);
    challengeAnswers=mainFolder+"/"+chAnsw_jN;
    SF::assignValueFromMap(stMap,"dcMountingFolder*!",dcMountFolder_jN);
    dcMountFolder=mainFolder+"/"+dcMountFolder_jN;
    SF::assignValueFromMap(stMap,"dImageName*!",dImageName);
    SF::assignValueFromMap(stMap,"codesToAutoGrade*!",maxCodesToRun);
    SF::assignValueFromMap(stMap,"initializerExtension*!",initExtension);
    SF::assignValueFromMap(stMap,"publicSubFolderToStoreFiles*!",publicStorage);
    SF::assignValueFromMap(stMap,"publicSubFolderForSystemFiles*!",publicSystemStorage);
    SF::assignValueFromMap(stMap,"publicSubFolderForPdfCertificates*!",publicPDFCertStorage);
    SF::assignValueFromMap(stMap,"publicSubFolderForPdfForms*!",publicPDFFormStorage);
    SF::assignValueFromMap(stMap,"subFolderForGuestClones*!",guestClonesRelLoc);
    folderMainDBGuestClone=mainFolder+"/"+guestClonesRelLoc;
    systemFolderClones=mainFolder+"/"+"sys_"+guestClonesRelLoc;
    if(guestClonesRelLoc.length()>0){
      if(guestClonesRelLoc[guestClonesRelLoc.length()-1]!='/'){
        guestClonesRelLoc+="/";
      }
    }
    SF::assignValueFromMap(stMap,"numStorageSubFoldersInFolder*!",numSubFolders);
    SF::assignValueFromMap(stMap,"numFilesInFolder*!",numFilesInFolder);
    (IOF::GL_folders.s).insert(mainFolder);
    (IOF::GL_folders.s).insert("/");
    IOF::GL_folders.safeNameOfFolderThatDoesNotExist= certManage+"/"+"w"+SAIOF::get_GL_MAIN_SETUP_FILE_NAME();
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
      fR+=certManage+"\n";
      fR+=backupDB+"\n";
      fR+=std::to_string(backupMaxSZ);
      fR+=mText+"\n";
      fR+=responseTable+"\n";
      fR+=statTable+"\n";
      fR+=fileForms+"\n"; 
      fR+=initExtension+"\n";
      fR+=publicStorage+"\n";
      fR+=publicSystemStorage+"\n";
      fR+=publicPDFCertStorage+"\n";
      fR+=publicPDFFormStorage+"\n";
      fR+=guestClonesRelLoc+"\n";
      fR+=challengeImages+"\n";
      fR+=challengeAnswers+"\n";
      fR+=std::to_string(numSubFolders);
      fR+=std::to_string(numFilesInFolder);
      return fR;
  }
}
#endif
