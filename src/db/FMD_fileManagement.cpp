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



#ifndef _INCL_FILEMANAGEMENT_CPP
#define _INCL_FILEMANAGEMENT_CPP
namespace FMD{


  class FileManager:public AMD::AbstractManager{
  private:

    std::string internalUserId;

    std::string ow_dbs_key0="fileId";
    std::string ow_dbs_key1="userId";
    std::string ow_dbsMFInd="fileMan";
    std::string ow_cntrName="filesCntr";

    long ow_primeIndex0=16;
    long ow_primeIndex1=37;
    long ow_permutInd=7;
    long ow_shift=76442;


  public:
    FileManager(const std::string & ="!*!", const std::string & ="!*!");

    void setVariables(const std::string & ="!*!", const std::string & ="!*!");

    std::string getInternalUserId() const;
    std::string getKey1() const;
    void setKey1(const std::string &);
    long checkExistenceInDatabase() const;
    // checks whether (internalUserId,internalFileNumber) exists in a database
    // returns:
    // 0 - good combination. This is not in database and can be entered as a new entry
    // 1 - good combination. This is in the database and corresponds to a correct entry.
    //                       Can be updated


    int createFile(const std::string &);
    // arguments: 1) internalUserId

    // returns :
    // 1 - success
    // 0 - internalUserId does not exist


    std::pair<long,long> filePosition(const long &, const long & = 100, const long & =100) const;

    //       We assume that the files 0,                1, ..., l-1 are stored in the folder <rootFiles>/<subFolder>/f0/
    //                          files l,              l+1, ..., 2l-1 are in the folder <rootFiles>/<subFolder>/f1/
    //                                   ...
    //                          files (k-1)l,    (k-1)l+1, ..., kl-1 are in the folder <rootFiles>/<subFolder>/f[k-1]/
    //                          files kl,            kl+1, ..., (k+1)l-1 are in the folder <rootFiles>/<subFolder>/<subFolder>/f0/
    //                          files (k+1)l,    (k+1)l+1, ..., (k+2)l-1 are in <rootFiles>/<subFolder>/<subFolder>/f1/
    //                                   ...
    //                          files (2k-1)l,    (2k-1)l+1, ..., (2k-1)l-1 are in <rootFiles>/<subFolder>/<subFolder>/f[k-1]/
    //                          files  2kl,           2kl+1, ..., (2k+1)l-1 are in <rootFiles>/<subFolder>/<subFolder>/<subFolder>/f0/
    //
    // Arguments: 1 - n (fileNumber); 2 - k (foldersAtEachLevel); 3 - l (filesInEachFolder)
    // Returns the pair: (numLevels, numFolder)
    // nth file should be stored at <rootFiles>/<subFolder>/<subFolder>/.../<subFolder>/f[numFolder]/
    // where <subFolder> appears numLevels times

    std::string clearThePublicPathAndDetermineTheFolder(const std::string &, const std::string & , const std::string & , const std::string & , const long &, const long & = 100, const long & =100);
    // Arguments:
    // 1 - website URL, 2 - public root, 3 - name for each subfolder except for the last, 4 - name for last subfolder
    // 5 - n (fileNumber); 6 - k (foldersAtEachLevel); 7 - l (filesInEachFolder)
    // This function makes sure that the necessary folders exist for n-th file to be placed in public view

  };

  std::pair<long,long> FileManager::filePosition(const long & n, const long & k, const long & l) const{
    long numLevels=-1,numFolder=-1;
    if((k>0)&&(l>0)&&(n>-1)){
      numLevels=n/(k*l);
      numFolder=(n%(k*l))/l;
    }
    return std::pair<long,long>(numLevels,numFolder);
  }

  std::string FileManager::clearThePublicPathAndDetermineTheFolder(const std::string & wsURL,
                                                                          const std::string & rootFiles,
                                                                          const std::string &subFolder,
                                                                          const std::string & folderStartName,
                                                                          const long & n,
                                                                          const long & k,
                                                                          const long & l){
    std::pair<long,long> sFFN=filePosition(n,k,l);
    long numSubFolders=sFFN.first;
    long lastFolderNum=sFFN.second;
    long i=0;
    std::string fileUploadLoc=rootFiles;
    if(rootFiles!=""){
      fileUploadLoc+="/";
    }
    fileUploadLoc+=subFolder;
    std::string redirectInfo="?rF="+fileUploadLoc;
    IOF::sys_createFolderIfDoesNotExist(fileUploadLoc,"index.html",HSF::redirectIndexHTML_F(redirectInfo,wsURL));
    while(i<numSubFolders){
      fileUploadLoc+="/"+subFolder;
      redirectInfo="?rF="+fileUploadLoc;
      IOF::sys_createFolderIfDoesNotExist(fileUploadLoc,"index.html",HSF::redirectIndexHTML_F(redirectInfo,wsURL));
      ++i;
    }
    fileUploadLoc+="/"+folderStartName+BF::padded(lastFolderNum,l,"0");
    redirectInfo="?rF="+fileUploadLoc;
    IOF::sys_createFolderIfDoesNotExist(fileUploadLoc,"index.html",HSF::redirectIndexHTML_F(redirectInfo,wsURL));
    return fileUploadLoc;
  }
  FileManager::FileManager(const std::string & _intUID, const std::string &  _intFId){
    setVariables(_intUID,_intFId);
  }
  void FileManager::setVariables(const std::string & _intUID, const std::string &  _intFId){

    dbs_key0= ow_dbs_key0;
    dbs_key1= ow_dbs_key1;
    dbsMFInd = ow_dbsMFInd;
    cntrName= ow_cntrName;

    primeIndex0= ow_primeIndex0;
    primeIndex1= ow_primeIndex1;
    permutInd= ow_permutInd;
    shift= ow_shift;


    primeIndex0= setPrimesCorrectly(f0,primeIndex0, BF::GLOBAL_NUM_PRIME_SEQUENCES);
    primeIndex1= setPrimesCorrectly(f1,primeIndex1, BF::GLOBAL_NUM_PRIME_SEQUENCES);
    permutInd= setPermsCombsCorrectly(permutInd,BF::GLOBAL_NUM_PERMUTATION_SEQUENCES);

    internalUserId=_intUID;

    internalVectorNumber=getInternalNumberFromInternalId(_intFId);
    existenceEstablishedBefore=0;
  }

  std::string FileManager::getInternalUserId() const{
    return internalUserId;
  }
  std::string FileManager::getKey1() const{
    return internalUserId;
  }
  void FileManager::setKey1(const std::string & _k){
    internalUserId=_k;
  }

  int FileManager::createFile(const std::string & _iId){

    WUD::User w;
    int sc=w.setFromInternalId(_iId);
    if(sc==0){
      return 0;
    }



    long fileIdIsFree=0;
    std::string fileId;
    std::vector<std::string> k,v;
    k.resize(1);v.resize(1);
    k[0]=dbs_key0;
    while(fileIdIsFree==0){
      CD::Counter fId(cntrName);
      fId.increase();
      fileId=fId.getCodeWord(1);
      v[0]=fileId;
      std::vector< std::pair<std::vector<std::string>, std::string> > searchRes;
      searchRes=DD::GL_MAIN_DB.dbsM[dbsMFInd].search(v,k);
      long sz=searchRes.size();
      if(sz==0){
        fileIdIsFree=1;
      }
    }
     internalUserId=_iId;
    internalVectorNumber=getInternalNumberFromInternalId(fileId);
    putInDB();
    existenceEstablishedBefore=1;
    return 1;
  }



  template<typename TTT>
  std::vector<TTT> itemsByUser(const TTT & fV,
                                  const std::string & dbName,
                                  const std::string & _iId,
                                  const long & start =0,
                                  const long & end=-1){
    std::vector<std::string> k,v;
    k.resize(1);v.resize(1);
    k[0]="userId";

    v[0]=_iId;
    std::vector< std::pair<std::vector<std::string>, std::string> > searchRes;
    searchRes=DD::GL_MAIN_DB.dbsM[dbName].search(v,k,start,end);
    long sz=searchRes.size();
    std::vector<TTT> fR;
    fR.resize(sz);
    for(long i=0;i<sz;++i){
      fR[i].setVariables((searchRes[i].first)[1],(searchRes[i].first)[0]);
      fR[i].setTextData(searchRes[i].second);
    }
    return fR;
  }
  std::vector<FileManager> filesByUser(const std::string & _iId, const long & start =0, const long & end=-1){
    FileManager f;
    return itemsByUser(f,"fileMan",_iId,start,end);
  }

}
#endif
