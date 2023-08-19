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


#ifndef _INCL_USERS_CPP
#define _INCL_USERS_CPP
namespace WUD{
  std::string GL_TheOnlyUserAllowedToGenerateMasterKey="mainAdmin";
  struct Separators{
  public:
    std::string keyB="_ek*_";
    std::string keyE="_/ek*_";
    std::string valB="_ev*_";
    std::string valE="_/ev*_";
  } SEP_ENC;
  long GL_MASTER_KEY_LENGTH=20;
  struct OthUsData{
  public:
    std::string password;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string isRoot;
    std::string allowedToExecuteCommands;
    std::string allowedToCloneTheWebsite;
    std::string rawEncSystemData;
    std::string masterKey;
    OthUsData();
  };
  OthUsData::OthUsData(){
    password="!*!";
    firstName="!*!";
    lastName="!*!";
    email="!*!";
    isRoot="!*!";
    allowedToExecuteCommands="!*!";
    allowedToCloneTheWebsite="!*!";
    rawEncSystemData="!*!";
    masterKey="!*!";
  }
  class User{
  private:
    std::string internalId;
    std::string externalId;
    std::string username;
    std::string otherData;
    std::string hCache;
    std::string dbsMUInd="usernames";
    std::string dbsMEIUInd="externalIds";
    std::string dbsMHInd="hierarchy";
    std::string cntrInternalId="userIntId";
    std::string cntrExternalId="userExtId";
    std::string level1Password;
    long level1PasswordSet=0;
    std::map<std::string,std::string> mPasswordsDisk;
    std::map<std::string,std::string> mPasswordsRam;
    long existenceEstablishedBefore;
    long cntrEIdI0=17;
    long cntrEIdI1=19;
    long cntrEIdIZ=2;
    std::string generateCookieData() const;
    std::string newCookieData();
    std::string getUserDataItem(const std::string &_bS, const std::string &_eS) const;
  public:
    User(const std::string & ="!*!", const std::string & ="!*!" , const std::string & ="!*!");
    void setVariables(const std::string & ="!*!", const std::string & ="!*!" , const std::string & ="!*!");
    void setOtherData(const std::string &);
    std::string getInternalId() const;
    std::string getExternalId() const;
    std::string getUsername() const;
    std::string getOtherData() const;
    int createUser(const std::string & , const std::string & = "", const std::string & = "!*!");
    // arguments: 1) username; 2) masterKey; 3) externalID
    // if externalId is !*!, it will be assigned
    // returns :
    // 1 - success
    // 0 - username exists
    // -3 - externalId exists
    int addUserData(const OthUsData &);
    int addUserData(const std::string &);
    std::string getEncPassword() const;
    int checkPasswordAndSetLevel1(const std::string & );
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getRawEncString() const;
    std::map<std::string,std::string> getRawEncMap() const;
    std::string getEmail() const;
    std::string isRoot() const;
    std::string isAllowedToExecuteCommands() const;
    std::string isAllowedToCloneWebsite() const;
    int checkCookieCorrectness(const std::string & ) const;
    std::string getEncCookieData() const;
    std::string getLastLogInTime() const;
    std::string getLastLogOutTime() const;
    int encryptOneParameter(const std::map<std::string,std::string> &, std::map<std::string,std::string> &,
                            const std::string &, const std::string &, const std::string &);
    int decryptOneParameter(const std::map<std::string,std::string> &, std::map<std::string,std::string> &,
                            const std::string &, const std::string &, const std::string &);
    int rootGenerateMasterKey(const std::string & );
    int updateEncryptionDataAfterCookieReading(const std::string & );
    std::string updateEncryptionDataAfterPassword(const std::string &);
    std::string updateEncryptionTextAfterPasswordChange();
    std::string updateEncryptionTextAfterReceivingMasterKey(const std::string &);
    std::string getMasterKeyFromRAM(const std::string &) const;
    std::string createKeyPair(const std::string &);
    std::string createKeyPairIfNoneExists(const std::string &);
    int masterKeyExists() const;
    int encryptedMasterKeyExists() const;
    int retreiveAndSaveMasterKeyIfSent(const std::string &);
    int addMasterKeyToLastCookieForAdditionalConvenience(const std::string &);
    std::string logIn(); //returns new cookieData
    void logOut();
    int changeExternalId(const std::string & = "!*!");
    // if supplied argument is "!*!" then the current externalId will be erased and
    // replaced by one generated by the program
    int setFromIndexFromExternalIdDB(const long &);
    int setFromIndexFromInternalIdDB(const long &);
    int setFromUsername(const std::string &);
    int setFromExternalId(const std::string &);
    int setFromInternalId(const std::string &);
    int addToHierarchy(const User &, const std::string & = "meLow");
    // u.addToHierarchy(v,"meHigh") adds (u.internalId,v.internalId) in the hierarchy
    // u.addToHierarchy(v,"meLow") adds (v.internalId,u.internalId) in the hierarchy
    int removeFromHierarchy(const User &, const std::string & = "meLow");
    // u.removeFromHierarchy(v,"meHigh") removes (u.internalId,v.internalId) in the hierarchy
    // u.removeFromHierarchy(v,"meLow") removes (v.internalId,u.internalId) in the hierarchy
    int addToHierarchy(const std::string &, const std::string & = "meLow");
    // u.addToHierarchy(vStringIntId,"meHigh") adds (u.internalId,vStringIntId) in the hierarchy
    // u.addToHierarchy(vStringIntId,"meLow") adds (vStringIntId,u.internalId) in the hierarchy
    int removeFromHierarchy(const std::string &, const std::string & = "meLow");
    // u.removeFromHierarchy(v.internalId,"meHigh") removes (u,v) in the hierarchy
    // u.removeFromHierarchy(v.internalId,"meLow") removes (v,u) in the hierarchy
    std::vector<std::string> hNeighborsInternalIds(const std::string & = "meHigh",
                                                   const long & = 0,
                                                   const long & = -1,
                                                   const std::string & = "noAlternativeStart") const;
    int removeHNeighbors(const std::string & = "meHigh",
                         const long & = 0,
                         const long & = -1);
    int existsPath(const std::string & , const std::string & = "meHigh", const std::string & = "uName", const long & = 0) const;
    // if the last argument is 1 - the algorithm will be faster
    // it won't check database very often
    int deleteUser(const long & = 1);
    // argument: 0 - put only into queue
    //           1 - put in database
    long checkExistenceInDatabase() const;
    // checks whether (username,externalId,internalId) is a good combination
    // returns:
    // 0 - good combination. This is not in database and can be entered as a new entry
    // 1 - good combination. This is in the database and corresponds to a correct entry.
    //                       Can be updated
    // everything else is a bad sign; combination cannot be entered in database
    std::pair<std::string,std::string>
    intId_username(const std:: string & , const std::string & ="username") const;
    // given username (internalId) returns internalId (username)
    // return values:
    // first is the internalId (username)
    // second is the otherUserData
    std::pair<std::string,std::string>
    extId_intId(const std::string & , const std::string & ="internalId") const;
    // given internalId (externalId) returns externalId (internalId)
    // first is the externalId (internalId)
    // second is the hierarchyCache
    int putInDB(const long & = 1);
    // argument: 0 - put only into queue
    //           1 - delete from database
  };
  User::User(const std::string & _u, const std::string & _e, const std::string & _i){
    setVariables(_u,_e,_i);
  }
  void User::setVariables(const std::string & _u, const std::string & _e, const std::string & _i){
    internalId=_i;
    externalId=_e;
    username=_u;
    existenceEstablishedBefore=0;
  }
  long User::checkExistenceInDatabase() const{
    if(existenceEstablishedBefore==1){
      return 1;
    }
    long fR=0;
    std::pair<std::string,std::string> test11,test12,test21,test22;
    // Step 1: get the pair (internalId, username) from the usernames database
    test11=intId_username(username,"username");
    test12=intId_username(internalId,"internalId");
    // Step 2: get the pair (externalId, internalId) from the externalIds database
    test21=extId_intId(externalId,"externalId");
    test22=extId_intId(internalId,"internalId");
    if(test11.first=="notFound"){
      fR+=1;
    }
    if(test12.first=="notFound"){
      fR+=10;
    }
    if(test21.first=="notFound"){
      fR+=100;
    }
    if(test22.first=="notFound"){
      fR+=1000;
    }
    // Step 3:
    if(fR==1111){
      return 0;//Good sign - none of internalId, externalId, or username are in the database
    }
    if(fR!=0){
      return 70000+fR;// bad combination; should not be entered in database
    }
    // fR is still 0.
    if(test11.first!=internalId){
      fR+=2;
    }
    if(test12.first!=username){
      fR+=20;
    }
    if(test21.first!=internalId){
      fR+=200;
    }
    if(test22.first!=externalId){
      fR+=2000;
    }
    if(fR!=0){
      return 80000+fR;// bad combination; should not be entered.
    }
    const long*  addressOfEEB;
    addressOfEEB=&existenceEstablishedBefore;
    size_t wbc=(size_t)(addressOfEEB);
    long* nonConstPointerAEEB;
    nonConstPointerAEEB = (long *)(wbc);
    *nonConstPointerAEEB=1;
    return 1;
  }
  int User::createUser(const std::string & _uName, const std::string & _masterKey, const std::string & _extId){
    if(SACF::acceptableUserName(_uName)==0){
      return 0; //unsafe username
    }
    if((_extId!="!*!")&&(SACF::veryStrictSafetyCheck(_extId)==0)) {
      return 0; //unsafe externalId
    }
    std::pair<std::string,std::string> intIdAndOthData=intId_username(_uName,"username");
    if(intIdAndOthData.first!="notFound"){
      return 0;//username Exists
    }
    std::string extId;
    if(_extId!="!*!"){
      std::pair<std::string,std::string> intIdAndCache=extId_intId(_extId,"externalId");
      if(intIdAndCache.first!="notFound"){
        return -3;// externalId exists
      }
      extId=_extId;
    }
    else{
      long extIdIsFree=0;
      while(extIdIsFree==0){
        CD::Counter cId(cntrExternalId,cntrEIdI0,cntrEIdI1,cntrEIdIZ);
        cId.increase();
        extId=cId.getCodeWord();
        std::pair<std::string,std::string> intIdAndCache=extId_intId(extId,"externalId");
        if(intIdAndCache.first=="notFound"){
          extIdIsFree=1;
        }
      }
    }
    long intIdIsFree=0;
    std::string intId;
    while(intIdIsFree==0){
      CD::Counter iId(cntrInternalId);
      iId.increase();
      intId=iId.getCodeWord(1);
      std::pair<std::string,std::string> intIdAndCache=extId_intId(intId,"internalId");
      if(intIdAndCache.first=="notFound"){
        intIdIsFree=1;
      }
    }
    internalId=intId;
    externalId=extId;
    username=_uName;
    if(_masterKey!=""){
      OthUsData tmpOUD;
      tmpOUD.rawEncSystemData=updateEncryptionTextAfterReceivingMasterKey(_masterKey);
      addUserData(tmpOUD);
    }
    putInDB();
    existenceEstablishedBefore=1;
    return 1;
  }
  int User::changeExternalId(const std::string & _extId){
    long dbEx=checkExistenceInDatabase();
    if(dbEx!=1){
      return 0;// the user does not exist in the database - cannot modify externalId;
    }
    std::string extId;
    if(_extId!="!*!"){
      std::pair<std::string,std::string> intIdAndCache=extId_intId(_extId,"externalId");
      if(intIdAndCache.first!="notFound"){
        return -3;// externalId exists
      }
      extId=_extId;
    }
    else{
      long extIdIsFree=0;
      while(extIdIsFree==0){
        CD::Counter cId(cntrExternalId,cntrEIdI0,cntrEIdI1,cntrEIdIZ);
        cId.increase();
        extId=cId.getCodeWord();
        std::pair<std::string,std::string> intIdAndCache=extId_intId(extId,"externalId");
        if(intIdAndCache.first=="notFound"){
          extIdIsFree=1;
        }
      }
    }
    std::pair<std::string,std::string> currentIdAndCache=extId_intId(externalId,"externalId");
    std::vector<std::string> kV;
    kV.resize(2);
    kV[0]=externalId;kV[1]=internalId;
    (DD::GL_MAIN_DB.dbsM[dbsMEIUInd]).delRow(kV);
    externalId=extId;
    kV[0]=externalId;kV[1]=internalId;
    (DD::GL_MAIN_DB.dbsM[dbsMEIUInd]).insert(kV,currentIdAndCache.second);
    return 1;
  }
  void User::setOtherData(const std::string &_o){
    otherData=_o;
  }
  std::string User::getInternalId() const{return internalId;}
  std::string User::getExternalId() const{return externalId;}
  std::string User::getUsername() const{return username;}
  std::string User::getOtherData() const{return otherData;}
  int User::putInDB(const long & _qOrDB){
    if((internalId=="!*!")||(externalId=="!*!")||(username=="!*!")){
      return 0;
    }
    if(checkExistenceInDatabase()>1){
      return 0;
    }
    std::vector<std::string> kV;
    kV.resize(2);
    kV[0]=externalId;kV[1]=internalId;
    std::string hC=" ";
    if(_qOrDB==0){
      DD::GL_MAIN_DB.dbsM[dbsMEIUInd].insertQ(kV,hC);
    }
    else{
      DD::GL_MAIN_DB.dbsM[dbsMEIUInd].insert(kV,hC);
    }
    kV[1]=username;kV[0]=internalId;
    if(_qOrDB==0){
      DD::GL_MAIN_DB.dbsM[dbsMUInd].insertQ(kV,otherData);
    }
    else{
      DD::GL_MAIN_DB.dbsM[dbsMUInd].insert(kV,otherData);
    }
    existenceEstablishedBefore=1;
    return 1;
  }
  std::pair<std::string,std::string> User::intId_username(const std::string & _u, const std::string & choice) const{
    std::vector<std::string> k,v;
    std::pair<std::string,std::string> fR;
    k.resize(1);v.resize(1);
    k[0]=choice;
    v[0]=_u;
    std::vector< std::pair<std::vector<std::string>, std::string> > searchRes;
    searchRes=DD::GL_MAIN_DB.dbsM[dbsMUInd].search(v,k);
    long sz=searchRes.size();
    if(sz==0){
      fR.first="notFound";
      fR.second="notFound";
      return fR;
    }
    std::vector<std::string> comp1=searchRes[0].first;
    std::string comp2=searchRes[0].second;
    //comp1[0] - internalId
    // comp1[1] - username
    fR.first=comp1[0];
    if(choice=="internalId"){
      fR.first=comp1[1];
    }
    fR.second=comp2;
    return fR;
  }
  std::pair<std::string,std::string> User::extId_intId(const std::string & _u, const std::string & choice) const{
    std::vector<std::string> k,v;
    std::pair<std::string,std::string> fR;
    k.resize(1);v.resize(1);
    k[0]=choice;v[0]=_u;
    std::vector< std::pair<std::vector<std::string>, std::string> > searchRes;
    searchRes=DD::GL_MAIN_DB.dbsM[dbsMEIUInd].search(v,k);
    long sz=searchRes.size();
    if(sz==0){
      fR.first="notFound";
      fR.second="notFound";
      return fR;
    }
    std::vector<std::string> comp1=searchRes[0].first;
    std::string comp2=searchRes[0].second;
    //comp1[0] - externalId
    // comp1[1] - internalId
    fR.first=comp1[0];
    if(choice=="externalId"){
      fR.first=comp1[1];
    }
    fR.second=comp2;
    return fR;
  }
  int User::deleteUser(const long & _qOrDB){
    if(checkExistenceInDatabase()!=1){
      return 0;
    }
    removeHNeighbors("meHigh");
    removeHNeighbors("meLow");
    std::vector<std::string> kV;
    kV.resize(2);
    kV[0]=externalId;kV[1]=internalId;
    if(_qOrDB==0){
      (DD::GL_MAIN_DB.dbsM[dbsMEIUInd]).delRowQ(kV);
    }
    else{
      (DD::GL_MAIN_DB.dbsM[dbsMEIUInd]).delRow(kV);
    }
    kV[0]=internalId;kV[1]=username;
    if(_qOrDB==0){
      (DD::GL_MAIN_DB.dbsM[dbsMUInd]).delRowQ(kV);
    }
    else{
      (DD::GL_MAIN_DB.dbsM[dbsMUInd]).delRow(kV);
    }
    existenceEstablishedBefore=0;
    return 1;
  }
  int User::setFromIndexFromExternalIdDB(const long &i){
    long uSize=DD::GL_MAIN_DB.numUsers();
    if((i<0)||(i>=uSize)){
      return 0;
    }
    std::pair<std::vector<std::string>, std::string>
    rs=(DD::GL_MAIN_DB.dbsM[dbsMEIUInd])[i];
    std::string _u,_i,_e,_o,_h;
    _e=(rs.first)[0];
    _i=(rs.first)[1];
    _h=rs.second;
    std::pair<std::string,std::string> r2=intId_username(_i,"internalId");
    if(r2.first=="notFound"){
      return 0;
    }
    _u=r2.first;
    _o=r2.second;
    username=_u;
    internalId=_i;
    externalId=_e;
    otherData=_o;
    hCache=_h;
    existenceEstablishedBefore=1;
    return 1;
  }
  int User::setFromIndexFromInternalIdDB(const long &i){
    long uSize=DD::GL_MAIN_DB.numUsers();
    if((i<0)||(i>=uSize)){
      return 0;
    }
    std::pair<std::vector<std::string>, std::string>
    rs=(DD::GL_MAIN_DB.dbsM[dbsMUInd])[i];
    std::string _u,_i,_e,_o,_h;
    _i=(rs.first)[0];
    _u=(rs.first)[1];
    _o=rs.second;
    std::pair<std::string,std::string> r2=extId_intId(_i,"internalId");
    if(r2.first=="notFound"){
      return 0;
    }
    _e=r2.first;
    _h=r2.second;
    username=_u;
    internalId=_i;
    externalId=_e;
    otherData=_o;
    hCache=_h;
    existenceEstablishedBefore=1;
    return 1;
  }
  int User::setFromUsername(const std::string & _u){
    std::pair<std::string,std::string> intIdAndOthData=intId_username(_u,"username");
    if(intIdAndOthData.first=="notFound"){
      return 0;
    }
    std::pair<std::string,std::string> extIdAndCache=
    extId_intId(intIdAndOthData.first,"internalId");
    if(extIdAndCache.first=="notFound"){return 0;}
    internalId=intIdAndOthData.first;
    externalId=extIdAndCache.first;
    username=_u;
    otherData=intIdAndOthData.second;
    hCache=extIdAndCache.second;
    existenceEstablishedBefore=1;
    return 1;
  }
  int User::setFromInternalId(const std::string & _i){
    std::pair<std::string,std::string> uNAndOthData=intId_username(_i,"internalId");
    if(uNAndOthData.first=="notFound"){
      return 0;
    }
    std::pair<std::string,std::string> extIdAndCache=
    extId_intId(_i,"internalId");
    if(extIdAndCache.first=="notFound"){return 0;}
    internalId=_i;
    externalId=extIdAndCache.first;
    username=uNAndOthData.first;
    otherData=uNAndOthData.second;
    hCache=extIdAndCache.second;
    existenceEstablishedBefore=1;
    return 1;
  }
  int User::setFromExternalId(const std::string & _e){
    std::pair<std::string,std::string> intIdAndCache=extId_intId(_e,"externalId");
    if(intIdAndCache.first=="notFound"){
      return 0;
    }
    std::pair<std::string,std::string> uNOthD=
    intId_username(intIdAndCache.first,"internalId");
    if(uNOthD.first=="notFound"){return 0;}
    internalId=intIdAndCache.first;
    externalId=_e;
    username=uNOthD.first;
    otherData=uNOthD.second;
    hCache=intIdAndCache.second;
    existenceEstablishedBefore=1;
    return 1;
  }
  int User::addToHierarchy(const std::string & _s, const std::string & position){
    long myEx=checkExistenceInDatabase();
    if(myEx!=1){
      return 0;
    }
    std::vector<std::string> kV;
    std::string datIns;
    TMF::Timer tm;
    long tN=tm.timeNow();
    datIns=std::to_string(tN);
    kV.resize(2);
    kV[0]=_s;
    kV[1]=internalId;
    if((position=="high")||(position=="meHigh")){
        kV[0]=internalId;
        kV[1]=_s;
    }
    (DD::GL_MAIN_DB.dbsM[dbsMHInd]).insert(kV,datIns);
    return 1;
  }
  int User::removeFromHierarchy(const std::string & _s, const std::string & position){
    long myEx=checkExistenceInDatabase();
    if(myEx!=1){
      return 0;
    }
    std::vector<std::string> kV;
    std::string datIns;
    kV.resize(2);
    kV[0]=_s;
    kV[1]=internalId;
    if((position=="high")||(position=="meHigh")){
        kV[0]=internalId;
        kV[1]=_s;
    }
    (DD::GL_MAIN_DB.dbsM[dbsMHInd]).delRow(kV);
    return 1;
  }
  int User::addToHierarchy(const User & it, const std::string & position){
    long itEx=it.checkExistenceInDatabase();
    if(itEx!=1){
      return 0;
    }
    return addToHierarchy(it.internalId,position);
  }
  int User::removeFromHierarchy(const User & it, const std::string & position){
    long itEx=it.checkExistenceInDatabase();
    if(itEx!=1){
      return 0;
    }
    return removeFromHierarchy(it.internalId,position);
  }
  std::vector<std::string> User::hNeighborsInternalIds(const std::string & position,
                                                              const long & _start,
                                                              const long & _end,
                                                              const std::string & alternativeStart) const{
    std::vector<std::string> fR;
    std::string intId=internalId;
    fR.resize(1);fR[0]="notFound";
    if(alternativeStart=="noAlternativeStart"){
      long myEx=checkExistenceInDatabase();
      if(myEx!=1){
        return fR;
      }
    }
    else{
      intId=alternativeStart;
    }
    std::vector<std::string> kV,iV;
    kV.resize(1);iV.resize(1);
    kV[0]=intId;
    iV[0]="high";
    if((position=="low")||(position=="meLow")){
      iV[0]="low";
    }
    std::vector< std::pair<std::vector<std::string>, std::string> >
    searchRes= (DD::GL_MAIN_DB.dbsM[dbsMHInd]).search(kV,iV,_start,_end);
    long srs=searchRes.size();
    if(srs==0){
      return fR;
    }
    fR.resize(srs);
    for(long i=0;i<srs;++i){
      fR[i]=(searchRes[i].first)[1];
      if((position=="low")||(position=="meLow")){
        fR[i]=(searchRes[i].first)[0];
      }
    }
    return fR;
  }
  int User::removeHNeighbors(const std::string & position,
                                    const long & _start,
                                    const long & _end){
    std::vector<std::string> nbs=hNeighborsInternalIds(position,_start,_end);
    if((nbs.size()==1)&&(nbs[0]=="notFound")){
      return 1;
    }
    long sz=nbs.size();
    for(long i=0;i<sz;++i){
      removeFromHierarchy(nbs[i],position);
    }
    return 1;
  }
  int User::existsPath(const std::string & targetInternalIdOrUserName,
                              const std::string & myPos,
                              const std::string & indIdOrUName,
                              const long & doItFaster) const{
    long myEx=checkExistenceInDatabase();
    if(myEx!=1){
      return 0;
    }
    std::string targetInternalId=targetInternalIdOrUserName;
    if(indIdOrUName=="uName"){
      User tmpU1;
      tmpU1.setFromUsername(targetInternalIdOrUserName);
      targetInternalId=tmpU1.getInternalId();
    }
    User tmpUsr;
    int indIntIdOK=1;
    if(doItFaster==0){
      indIntIdOK=tmpUsr.setFromInternalId(targetInternalId);
    }
    if(indIntIdOK==0){
      return 0;
    }
    std::set<std::string> analyzed,notYetAnalyzed;
    notYetAnalyzed.insert(internalId);
    std::set<std::string>::iterator it,itE;
    int found=0;
    std::vector<std::string> tmpNbr;
    std::string tmpUsrIntId;
    while((found==0)&&(!notYetAnalyzed.empty())) {
      it=notYetAnalyzed.begin();
      tmpUsrIntId=*it;
      indIntIdOK=1;
      if(doItFaster==0){
        indIntIdOK=tmpUsr.setFromInternalId(tmpUsrIntId);
      }
      if(indIntIdOK==0){
        analyzed.insert(tmpUsrIntId);
        notYetAnalyzed.erase(tmpUsrIntId);
      }
      else{
        if(tmpUsrIntId==targetInternalId){
          found=1;
        }
        else{
          if(doItFaster==0){
            tmpNbr=tmpUsr.hNeighborsInternalIds(myPos);
          }
          else{
            tmpNbr=hNeighborsInternalIds(myPos,0,-1,tmpUsrIntId);
          }
          if((tmpNbr.size()==1)&&(tmpNbr[0]=="notFound")){
            analyzed.insert(tmpUsrIntId);
            notYetAnalyzed.erase(tmpUsrIntId);
          }
          else{
            long nbrSz=tmpNbr.size();
            long i=0;
            while((i<nbrSz)&&(found==0)){
              if(tmpNbr[i]==targetInternalId){
                found =1;
              }
              else{
                itE=analyzed.end();
                it=analyzed.find(tmpNbr[i]);
                if(it==itE){
                  notYetAnalyzed.insert(tmpNbr[i]);
                }
              }
              ++i;
            }
            if(found==0){
              analyzed.insert(tmpUsrIntId);
              notYetAnalyzed.erase(tmpUsrIntId);
            }
          }
        }
      }
    }
    return found;

  }
  int User::encryptOneParameter(const std::map<std::string,std::string> & mPlain, std::map<std::string,std::string> & mEnc,
                          const std::string & nmPlain, const std::string & nmEnc, const std::string & password){
    std::map<std::string,std::string>::const_iterator
    it = mPlain.find(nmPlain);
    if(it==mPlain.end()){
      return 0;
    }
    mEnc[nmEnc] = ENCF::symmetricEncrypt(it->second,password);
    return 1;
  }
  int User::decryptOneParameter(const std::map<std::string,std::string> & mEnc, std::map<std::string,std::string> & mPlain,
                          const std::string & nmEnc, const std::string & nmPlain, const std::string & password){
    std::map<std::string,std::string>::const_iterator
    it = mEnc.find(nmEnc);
    if(it==mEnc.end()){
      return 0;
    }
    if(password=="noEncryptionJustCopy"){
      mPlain[nmPlain] = it->second;
    }
    else{
      mPlain[nmPlain] = ENCF::symmetricDecrypt(it->second,password);
    }
    return 1;
  }
  std::string User::updateEncryptionTextAfterPasswordChange(){
    encryptOneParameter(mPasswordsRam,mPasswordsDisk,"sigmaM","sigmaMP",level1Password);
    encryptOneParameter(mPasswordsRam,mPasswordsDisk,"privateKey","privateKeyP",level1Password);
    mPasswordsDisk.erase("encMasterKey");
    return SF::mapToString(mPasswordsDisk,SEP_ENC.keyB, SEP_ENC.keyE, SEP_ENC.valB, SEP_ENC.valE);
  }
  std::string User::updateEncryptionTextAfterReceivingMasterKey(const std::string & mKey){
    if(level1PasswordSet==1){
      mPasswordsRam["sigmaM"]=mKey;
      createKeyPair(RNDF::genRandCode(20));
      encryptOneParameter(mPasswordsRam,mPasswordsDisk,"sigmaM","sigmaMP",level1Password);
      encryptOneParameter(mPasswordsRam,mPasswordsDisk,"privateKey","privateKeyP",level1Password);
    }
    mPasswordsDisk.erase("encMasterKey");
    return SF::mapToString(mPasswordsDisk,SEP_ENC.keyB, SEP_ENC.keyE, SEP_ENC.valB, SEP_ENC.valE);
  }
  int User::addUserData(const OthUsData & _oud){
    const std::string & _p  = _oud.password;
    const std::string & _fN = _oud.firstName;
    const std::string & _lN = _oud.lastName;
    const std::string & _e  = _oud.email;
    const std::string & _iR = _oud.isRoot;
    const std::string & _iAEC=_oud.allowedToExecuteCommands;
    const std::string & _iAClone=_oud.allowedToCloneTheWebsite;
    std::string _rESD=_oud.rawEncSystemData;
    std::string _mKey=_oud.masterKey;
    if((_fN!="!*!") && (SACF::lessStrictSafetyCheck(_fN)==0) ) {
      return 0;// unsafe _fN
    }
    if((_lN!="!*!") && (SACF::lessStrictSafetyCheck(_lN)==0) ) {
      return 0;// unsafe _lN
    }
    if((_e!="!*!") && (SACF::strictSafetyCheck(_e)==0) ) {
      return 0;// unsafe _e
    }
    if((_iR!="!*!") && (SACF::veryStrictSafetyCheck(_iR)==0) ) {
      return 0;// unsafe _iR
    }
    if((_iAEC!="!*!") && (SACF::veryStrictSafetyCheck(_iAEC)==0) ) {
      return 0;// unsafe _iAEC
    }
    if((_iAClone!="!*!") && (SACF::veryStrictSafetyCheck(_iAClone)==0) ) {
      return 0;// unsafe _iAClone
    }
    std::string passChange="";long pos;
    std::pair<std::string,int> allD;
    if(_p.length()>4){
      level1PasswordSet=1;
      level1Password=HENCF::oneWayEncHENCF(_p);
      passChange=SF::removeInfoAndPrepareNew(otherData,"_password!*_","_/password!*_",HENCF::oneWayEncHENCF(level1Password,level1Password));
      if(_mKey=="!*!"){
        _rESD=updateEncryptionTextAfterPasswordChange();
      }
      else{
        _rESD=updateEncryptionTextAfterReceivingMasterKey(_mKey);
      }
    }
    std::string fNChange="";
    if(_fN!="!*!"){
      fNChange=SF::removeInfoAndPrepareNew(otherData,"_firstName!*_", "_/firstName!*_",_fN);
    }
    std::string lNChange="";
    if(_lN!="!*!"){
      lNChange=SF::removeInfoAndPrepareNew(otherData,"_lastName!*_", "_/lastName!*_",_lN);
    }
    std::string eChange="";
    if(_e!="!*!"){
      eChange=SF::removeInfoAndPrepareNew(otherData,"_email!*_", "_/email!*_",_e);
    }
    std::string rsChange="";
    if(_iR!="!*!"){
      rsChange=SF::removeInfoAndPrepareNew(otherData,"_isRoot!*_","_/isRoot!*_",_iR);
    }
    std::string aECChange="";
    if(_iAEC!="!*!"){
      aECChange=SF::removeInfoAndPrepareNew(otherData,"_aEC!*_","_/aEC!*_",_iAEC);
    }
    std::string aCloneChange="";
    if(_iAClone!="!*!"){
      aCloneChange=SF::removeInfoAndPrepareNew(otherData,"_aClone!*_","_/aClone!*_",_iAClone);
    }
    std::string crptChange="";
    if(_rESD!="!*!"){
      crptChange=SF::removeInfoAndPrepareNew(otherData,"_crpt!*_","_/crpt!*_",_rESD);
    }
    otherData+= passChange+fNChange+lNChange+eChange+rsChange+aECChange+aCloneChange+crptChange;
    putInDB();
    return 1;
  }
  int User::addUserData(const std::string & _p){
    WUD::OthUsData tmpOUD;
    tmpOUD.password=_p;
    return addUserData(tmpOUD);
  }
  std::string User::getUserDataItem(const std::string &_bS,const std::string &_eS) const{
    long pos=0;
    std::pair<std::string,int> allD=SF::extract(otherData,pos,_bS,_eS);
    std::string fR="notFound";
    if(allD.second==1){
      fR=allD.first;
    }
    return fR;
  }
  std::string User::getEncPassword() const{
    return getUserDataItem("_password!*_","_/password!*_");
  }
  int User::checkPasswordAndSetLevel1(const std::string & _v){
    std::string encPass=getUserDataItem("_password!*_","_/password!*_");
    level1PasswordSet=1;
    level1Password=HENCF::oneWayEncHENCF(_v,encPass);
    if(HENCF::oneWayEncHENCF(level1Password,encPass)==encPass){
      return 1;
    }
    return 0;
  }
  std::string User::getFirstName() const{
    return getUserDataItem("_firstName!*_","_/firstName!*_");
  }
  std::string User::getLastName() const{
    return getUserDataItem("_lastName!*_","_/lastName!*_");
  }
  std::string User::getRawEncString() const{
    return getUserDataItem("_crpt!*_","_/crpt!*_");
  }
  std::map<std::string,std::string> User::getRawEncMap() const{
    return SF::stringToMap(getRawEncString(),SEP_ENC.keyB, SEP_ENC.keyE, SEP_ENC.valB, SEP_ENC.valE);
  }
  std::string User::getMasterKeyFromRAM(const std::string &pName) const{
    std::map<std::string,std::string>::const_iterator
    it =  mPasswordsRam.find(pName);
    if(it==mPasswordsRam.end()){
      return "notFound";
    }
    return it->second;
  }
  std::string User::getEmail() const{
    return getUserDataItem("_email!*_","_/email!*_");
  }
  std::string User::isRoot() const{
     std::string iR=getUserDataItem("_isRoot!*_","_/isRoot!*_");
     if(iR=="notFound"){
       return "no";
     }
     return iR;
  }
  std::string User::isAllowedToExecuteCommands() const{
    std::string aE= getUserDataItem("_aEC!*_","_/aEC!*_");
    if(aE=="notFound"){
      return "no";
    }
    return aE;
  }
  std::string User::isAllowedToCloneWebsite() const{
    std::string aC= getUserDataItem("_aClone!*_","_/aClone!*_");
    if(aC=="notFound"){
      return "no";
    }
    return aC;
  }
  std::string User::getEncCookieData() const{
    return getUserDataItem("_cookieD!*_","_/cookieD!*_");
  }
  std::string User::generateCookieData() const{
    return SF::combineTwoWords(externalId,RNDF::genRandCode(7));
  }
  int User::checkCookieCorrectness(const std::string & cookie) const{
    std::string encCookie=getEncCookieData();
    if(encCookie==HENCF::oneWayEncHENCF(cookie,encCookie)){
      return 1;
    }
    return 0;
  }
  std::string User::newCookieData(){
    std::string cD=generateCookieData();
    otherData+=SF::removeInfoAndPrepareNew(otherData,"_cookieD!*_","_/cookieD!*_",HENCF::oneWayEncHENCF(cD));
    return cD;
  }
  std::string User::getLastLogInTime() const{
    return getUserDataItem("_lastLogIn!*_","_/lastLogIn!*_");
  }
  std::string User::getLastLogOutTime() const{
    return getUserDataItem("_lastLogOut!*_","_/lastLogOut!*_");
  }
  std::string User::createKeyPair(const std::string &cookie){
    ENCF::KeyPair kp=ENCF::createKeyPair(username);
    mPasswordsRam["privateKey"]=kp.privateKey;
    mPasswordsDisk["privateKeyP"] = ENCF::symmetricEncrypt(kp.privateKey,level1Password);
    mPasswordsDisk["privateKeyC"] = ENCF::symmetricEncrypt(kp.privateKey,cookie);
    mPasswordsDisk["publicKey"]=kp.publicKey;
    return SF::mapToString(mPasswordsDisk,SEP_ENC.keyB, SEP_ENC.keyE, SEP_ENC.valB, SEP_ENC.valE);
  }
  std::string User::createKeyPairIfNoneExists(const std::string &cookie){
    std::map<std::string,std::string>::const_iterator
    it=mPasswordsDisk.find("publicKey");
    if(it!=mPasswordsDisk.end()){
      it=mPasswordsDisk.find("privateKeyP");
      if(it!=mPasswordsDisk.end()){
        return "notPossible";
      }
    }
    return createKeyPair(cookie);
  }
  int User::retreiveAndSaveMasterKeyIfSent(const std::string & cookie){
    std::map<std::string,std::string>::const_iterator
    it=mPasswordsDisk.find("encMasterKey");
    if(it==mPasswordsDisk.end()){
      return 0;
    }
    std::map<std::string,std::string>::const_iterator
    itR=mPasswordsRam.find("privateKey");
    if(itR==mPasswordsRam.end()){
      mPasswordsDisk.erase("encMasterKey");
      return 0;
    }
    std::string plainMasterKey=ENCF::decryptWithPrivateKey(it->second, itR->second, username);
    mPasswordsDisk.erase("encMasterKey");
    mPasswordsRam["sigmaM"]=plainMasterKey;
    encryptOneParameter(mPasswordsRam,mPasswordsDisk,"sigmaM","sigmaMC",cookie);
    encryptOneParameter(mPasswordsRam,mPasswordsDisk,"sigmaM","sigmaMP",level1Password);
    return 1;
  }
  int User::addMasterKeyToLastCookieForAdditionalConvenience(const std::string & _mKey){
    std::string cookie=getEncCookieData();
    return 1;
  }
  int User::rootGenerateMasterKey(const std::string & cookie){
    mPasswordsDisk.erase("encMasterKey");
    if(level1PasswordSet==0){
      return 0;
    }
    std::map<std::string,std::string>::const_iterator
    itR=mPasswordsRam.find("privateKey");
    if(itR==mPasswordsRam.end()){
      return 0;
    }
    std::string plainMasterKey=RNDF::genRandCode(GL_MASTER_KEY_LENGTH);
    mPasswordsRam["sigmaM"]=plainMasterKey;
    encryptOneParameter(mPasswordsRam,mPasswordsDisk,"sigmaM","sigmaMC",cookie);
    encryptOneParameter(mPasswordsRam,mPasswordsDisk,"sigmaM","sigmaMP",level1Password);
    return 1;
  }
  int User::masterKeyExists() const{
    std::map<std::string,std::string>::const_iterator
    it=mPasswordsRam.find("sigmaM");
    if(it==mPasswordsRam.end()){
      return 0;
    }
    return 1;
  }
  int User::encryptedMasterKeyExists() const{
    std::map<std::string,std::string> mEncK=getRawEncMap();
    std::map<std::string,std::string>::const_iterator it=mEncK.find("sigmaMP");
    if(it==mEncK.end()){
      return 0;
    }
    return 1;
  }
  std::string User::updateEncryptionDataAfterPassword(const std::string & cookie){
    mPasswordsDisk=getRawEncMap();
    createKeyPairIfNoneExists(cookie);
    decryptOneParameter(mPasswordsDisk,mPasswordsRam,"sigmaMP","sigmaM",level1Password);
    decryptOneParameter(mPasswordsDisk,mPasswordsRam,"privateKeyP","privateKey",level1Password);
    decryptOneParameter(mPasswordsDisk,mPasswordsRam,"publicKey","publicKey","noEncryptionJustCopy");
    encryptOneParameter(mPasswordsRam,mPasswordsDisk,"sigmaM","sigmaMC",cookie);
    encryptOneParameter(mPasswordsRam,mPasswordsDisk,"privateKey","privateKeyC",cookie);
    int mkexists=masterKeyExists();
    int mkretreived=0;
    if(masterKeyExists()==0){
      mkretreived=retreiveAndSaveMasterKeyIfSent(cookie);
    }
    if((mkexists==0)&&(mkretreived==0)){
      if((isRoot()=="yes")&&(username==GL_TheOnlyUserAllowedToGenerateMasterKey)){
        rootGenerateMasterKey(cookie);
      }
    }
    return SF::mapToString(mPasswordsDisk,SEP_ENC.keyB, SEP_ENC.keyE, SEP_ENC.valB, SEP_ENC.valE);
  }
  int User::updateEncryptionDataAfterCookieReading(const std::string &cookie){
    mPasswordsDisk=getRawEncMap();
    decryptOneParameter(mPasswordsDisk,mPasswordsRam,"sigmaMC","sigmaM",cookie);
    decryptOneParameter(mPasswordsDisk,mPasswordsRam,"privateKeyC","privateKey",cookie);
    decryptOneParameter(mPasswordsDisk,mPasswordsRam,"publicKey","publicKey","noEncryptionJustCopy");
    if(encryptedMasterKeyExists()==0){
      retreiveAndSaveMasterKeyIfSent(cookie);
    }
    return 1;
  }
  std::string User::logIn(){
    std::string newCookie=newCookieData();
    TMF::Timer tm;
    otherData+= SF::removeInfoAndPrepareNew(otherData,"_lastLogIn!*_","_/lastLogIn!*_",tm.timeString());
    otherData+= SF::removeInfoAndPrepareNew(otherData,"_crpt!*_","_/crpt!*_",updateEncryptionDataAfterPassword(newCookie));
    putInDB();
    return newCookie;
  }
  void User::logOut(){
    TMF::Timer tm;
    otherData+=SF::removeInfoAndPrepareNew(otherData,"_lastLogOut!*_","_/lastLogOut!*_",tm.timeString());
    otherData+=SF::removeInfoAndPrepareNew(otherData,"_cookieD!*_","_/cookieD!*_","loggedOut");
    putInDB();
  }
  int sendMessageUsingPublicKey(const std::string & userName, const std::string & plain, const std::string & subject){
    User uToGetMessage;
    if(uToGetMessage.setFromUsername(userName)==0){
      return 0;
    }
    std::map<std::string,std::string> mapEncRaw=uToGetMessage.getRawEncMap();
    std::map<std::string,std::string>::const_iterator
    it=mapEncRaw.find("publicKey");
    if(it==mapEncRaw.end()){
      return 0;
    }
    mapEncRaw[subject]=ENCF::encryptWithPublicKey(plain,it->second,userName);
    OthUsData tmpOUD;
    tmpOUD.rawEncSystemData=SF::mapToString(mapEncRaw,SEP_ENC.keyB, SEP_ENC.keyE, SEP_ENC.valB, SEP_ENC.valE);
    uToGetMessage.addUserData(tmpOUD);
    return 1;
  }
}
#endif
