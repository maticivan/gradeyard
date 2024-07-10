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

#ifndef _INCL_WI_Setup_CPP
#define _INCL_WI_Setup_CPP

namespace WSI{
  class Setup{
  private:
    std::string default_challengeImages="c0";
    std::string default_challengeAnswers="mainFolderDB/a0";
    std::string default_fileUploadLoc="f001";
    std::string default_publicSystemFileLoc="fs001";
    std::string default_publicPDFCertLoc="pc001";
    std::string default_publicPDFFormLoc="pf001";
    std::string default_guestClonesRelLoc="si001";
    std::string default_wsName="websiteName";
    std::string default_wsURL="websiteURL";
    std::string default_dbOptions="no";
    std::string default_vstOptions="noStop";
    std::string default_e_parPage="page1";
    std::string default_e_respRecReqRT="rrc1";
    std::string default_e_formNameRT="respRec1";
    std::string default_e_respSubmitReqRT="respSubmit1";
    std::string default_e_respSubmitSuccRT="yes1";
    std::string default_tableOpenTag="<table class=\"table table-striped table-responsive \">";
    std::string default_theadOpenTag="<thead class=\"table-info\">";
    std::string default_e_conveniencePack="_vVPair__vr_Hw_/vr__vl_HW_/vl__/vVPair_ _vVPair__vr_Mt_/vr__vl_M_/vl__/vVPair_ _vVPair__vr_Fin_/vr__vl_F_/vl__/vVPair_";
    std::string default_e_wsTexts=" ";
    std::string default_e_defaultFindReplace=" ";
    long default_q_on_page=10;
    std::string default_cookieName="s";
    std::string wsName=default_wsName;
    std::string wsURL=default_wsURL;
    std::string dbOptions=default_dbOptions;
    std::string vstOptions=default_vstOptions;
    std::string fileUploadLoc=default_fileUploadLoc;
    std::string publicSystemFileLoc=default_publicSystemFileLoc;
    std::string publicPDFCertLoc=default_publicPDFCertLoc;
    std::string publicPDFFormLoc=default_publicPDFFormLoc;
    std::string guestClonesRelLoc=default_guestClonesRelLoc;
    std::string cookiePath;
    std::string cookieName=default_cookieName;
    std::string challengeImages=default_challengeImages;
    std::string challengeAnswers=default_challengeAnswers;
    std::string clonesDBFolder;
    std::string clonesSysFolder;
    std::string e_parPage=default_e_parPage;
    std::string e_respRecReqRT=default_e_respRecReqRT;
    std::string e_formNameRT=default_e_formNameRT;
    std::string e_respSubmitReqRT=default_e_respSubmitReqRT;
    std::string e_respSubmitSuccRT=default_e_respSubmitSuccRT;
    std::string e_conveniencePackage=default_e_conveniencePack;
    std::string e_wsTexts=default_e_wsTexts;
    std::string e_defaultFindReplace=default_e_defaultFindReplace;
    std::string tableOpenTag=default_tableOpenTag;
    std::string theadOpenTag=default_theadOpenTag;
    std::string redirectOverwrite;
    std::string redirectForward;
    std::string mainPageName;
    std::string mainPageExternalCode;
    std::string failedLogIn;
    std::string showLogInLink;
    std::string fastUpdatingStat;
    std::string stList;
    std::string sortCr;
    long q_on_page;
    std::map<std::string,std::string> defaultWebTexts;
    std::map<std::string,std::string> default_FindReplacePairs;
  public:
    void getSetupFromMap(const std::map<std::string,std::string> &);
    void changeAlphabet(const std::string &);
    void updateDefaultWebTexts(const std::string &);
    std::string getWSName() const;
    std::string getUploadLoc() const;
    std::string getPublicSystemFileLoc() const;
    std::string getPublicPDFCertLoc() const;
    std::string getPublicPDFFormLoc() const;
    std::string getWSURL() const;
    std::string getDebuggingOptions() const;
    std::string getVersionStopOptions() const;
    std::string getCookiePath() const;
    std::string getCookieName() const;
    std::string get_e_parPage() const;
    std::string get_e_respRecReqRT() const;
    std::string get_e_formNameRT() const;
    std::string get_e_respSubmitReqRT() const;
    std::string get_e_respSubmitSuccRT() const;
    long get_q_on_page() const;
    std::string getChallengeImStorage() const;
    std::string getChallengeAnswStorage() const;
    std::string getClonesMainDBFolder() const;
    std::string getClonesSystemFolder() const;
    std::string printSetup() const;
    std::string getStartOfList() const;
    std::string getSortCriterion() const;
    std::string getRedirectOverwrite() const;
    void setRedirectOverwrite(const std::string & );
    std::string getRedirectForward() const;
    void setRedirectForward(const std::string & );
    void setMainPageName(const std::string & );
    std::string getMainPageName() const;
    std::string getMainPageExternalCode() const;
    std::string showLogInOnFrontPage() const;
    std::string fastUpdatingStatIndicator() const;
    std::string getFailedLogIn() const;
    void setFailedLogIn(const std::string & = "yes");
    void setLoginLink(const std::string &);
    void setFastUpdatingStat(const std::string &);
    void setStartOfList(const std::string &);
    void setSortCriterion(const std::string &);
    std::string getConveniencePackage() const;
    std::string getWSTexts() const;
    std::string getDefaultFindReplaceTexts() const;
    std::map<std::string,std::string> getDefaultFindReplaceMap() const;
    std::string getDefaultWebText(const std::string &) const;
    std::string getAlphabet() const;
    std::string getTableOpenTag() const;
    std::string getTheadOpenTag() const;
  };
  std::string Setup::getWSName() const{return wsName;}
  std::string Setup::getWSURL() const{return wsURL;}
  std::string Setup::getDebuggingOptions() const{return dbOptions;}
  std::string Setup::getVersionStopOptions() const{return vstOptions;}
  std::string Setup::getUploadLoc() const{    return fileUploadLoc; }
  std::string Setup::getPublicSystemFileLoc() const{ return publicSystemFileLoc;}
  std::string Setup::getPublicPDFCertLoc() const{ return publicPDFCertLoc;}
  std::string Setup::getPublicPDFFormLoc() const{ return publicPDFFormLoc;}
  std::string Setup::getCookiePath() const{return cookiePath;}
  std::string Setup::getCookieName() const{return cookieName;}
  std::string Setup::get_e_parPage() const{return e_parPage;}
  std::string Setup::get_e_respRecReqRT() const{return e_respRecReqRT;}
  std::string Setup::get_e_formNameRT() const{return e_formNameRT;}
  std::string Setup::get_e_respSubmitReqRT() const{return e_respSubmitReqRT;}
  std::string Setup::get_e_respSubmitSuccRT() const{return e_respSubmitSuccRT;}
  std::string Setup::getChallengeImStorage() const{return challengeImages;}
  std::string Setup::getChallengeAnswStorage() const{return challengeAnswers;}
  std::string Setup::getClonesMainDBFolder() const{return clonesDBFolder;}
  std::string Setup::getClonesSystemFolder() const{return clonesSysFolder;}
  std::string Setup::showLogInOnFrontPage() const{return showLogInLink;}
  std::string Setup::fastUpdatingStatIndicator() const{return fastUpdatingStat;}
  std::string Setup::getConveniencePackage() const{return e_conveniencePackage;}
  std::string Setup::getWSTexts() const {return e_wsTexts;}
  std::string Setup::getDefaultFindReplaceTexts() const{return e_defaultFindReplace;}
  std::map<std::string,std::string> Setup::getDefaultFindReplaceMap() const{return default_FindReplacePairs;}
  std::string Setup::getTableOpenTag() const{return tableOpenTag;}
  std::string Setup::getTheadOpenTag() const{return theadOpenTag;}
  long Setup::get_q_on_page() const{return q_on_page;}
  std::string Setup::getAlphabet() const{return GF::GL_Alphabet;}
  std::string Setup::getDefaultWebText(const std::string & tName) const {
    std::map<std::string,std::string>::const_iterator it,itE;
    itE=defaultWebTexts.end();
    it=defaultWebTexts.find(tName);
    if(it==itE){
      return tName;
    }
    return it->second;
  }
  std::string removeHTTPS(const std::string &_url){
    long pos; std::pair<std::string,int> allD;
    std::string url=_url+"$$$end";
    pos=0;allD=SF::extract(url,pos,"//","$$$");
    if(allD.second==0){
      return url;
    }
    return allD.first;
  }
  std::string determineCookiePathFromURL(const std::string &_url){
    std::string url=removeHTTPS(_url);
    std::string urlTest1=SF::findAndReplace(url,"/","");
    if(urlTest1==url){
      return "/";
    }
    long pos;std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(url+"_urlEnd*_",pos,"/","_urlEnd*_");
    if(allD.second==0){
      return "/";
    }
    return "/"+allD.first;
  }
  void Setup::getSetupFromMap(const std::map<std::string,std::string> & stMap){
    SF::assignValueFromMap(stMap,"websiteName*!",wsName);
    SF::assignValueFromMap(stMap,"websiteURL*!",wsURL);
    cookiePath=determineCookiePathFromURL(wsURL);
    SF::assignValueFromMap(stMap,"debugging*!",dbOptions);
    SF::assignValueFromMap(stMap,"versionStop*!",vstOptions);
    SF::assignValueFromMap(stMap,"cookieName*!",cookieName);
    SF::assignValueFromMap(stMap,"pVar*!",e_parPage);
    SF::assignValueFromMap(stMap,"rrc*!",e_respRecReqRT);
    SF::assignValueFromMap(stMap,"formName*!",e_formNameRT);
    SF::assignValueFromMap(stMap,"rSubmitR*!",e_respSubmitReqRT);
    SF::assignValueFromMap(stMap,"respSbmYES!*",e_respSubmitSuccRT);
    SF::assignValueFromMap(stMap,"convenience!*",e_conveniencePackage);
    SF::assignValueFromMap(stMap,"defaultTexts!*",e_wsTexts);
    GF::GL_Alphabet="english";
    SF::assignValueFromMap(stMap,"defaultAlphabet!*",GF::GL_Alphabet);
    SF::assignValueFromMap(stMap,"tableOpenTag*!",tableOpenTag);
    SF::assignValueFromMap(stMap,"theadOpenTag*!",theadOpenTag);
    SF::assignValueFromMap(stMap,"defaultFindReplace!*",e_defaultFindReplace);
    SF::assignValueFromMap(stMap,"questionsOnPage!*",q_on_page);
    if(q_on_page<2){q_on_page=2;}
    fileUploadLoc=DD::GL_DBS.getPublicStorage();
    std::string redirectInfo="?rF="+fileUploadLoc;
    IOF::sys_createFolderIfDoesNotExist(fileUploadLoc,"index.html",HSF::redirectIndexHTML_F(redirectInfo,wsURL));
    publicSystemFileLoc=DD::GL_DBS.getPublicSystemStorage();
    redirectInfo="?rF="+publicSystemFileLoc;
    IOF::sys_createFolderIfDoesNotExist(publicSystemFileLoc,"index.html",HSF::redirectIndexHTML_F(redirectInfo,wsURL));
    publicPDFCertLoc=DD::GL_DBS.getPublicPDFCertStorage();
    redirectInfo="?rF="+publicPDFCertLoc;
    IOF::sys_createFolderIfDoesNotExist(publicPDFCertLoc,"index.html",HSF::redirectIndexHTML_F(redirectInfo,wsURL));
    publicPDFFormLoc=DD::GL_DBS.getPublicPDFFormStorage();
    redirectInfo="?rF="+publicPDFFormLoc;
    IOF::sys_createFolderIfDoesNotExist(publicPDFFormLoc,"index.html",HSF::redirectIndexHTML_F(redirectInfo,wsURL));
    guestClonesRelLoc=DD::GL_DBS.getGuestClonesRelLoc();
    redirectInfo="?rF="+guestClonesRelLoc;
    if(guestClonesRelLoc!="/"){
      IOF::sys_createFolderIfDoesNotExist(guestClonesRelLoc,"index.html",HSF::redirectIndexHTML_F(redirectInfo,wsURL));
    }
    challengeImages=DD::GL_DBS.getChallengeImStorage();
    challengeAnswers=DD::GL_DBS.getChallengeAnswStorage();
    redirectInfo="?rF="+challengeImages;
    IOF::sys_createFolderIfDoesNotExist(challengeImages,"index.html",HSF::redirectIndexHTML_F(redirectInfo,wsURL));
    IOF::sys_createFolderIfDoesNotExist(challengeAnswers,"readme.txt","Do not edit this folder.");
    clonesDBFolder=DD::GL_DBS.getClonesMainDBFolder();
    IOF::sys_createFolderIfDoesNotExist(clonesDBFolder,"readme.txt","Do not edit this folder.");
    clonesSysFolder=DD::GL_DBS.getClonesSystemFolder();
    IOF::sys_createFolderIfDoesNotExist(clonesSysFolder,"readme.txt","Do not edit this folder.");
    IOF::sys_createFolderIfDoesNotExist(DD::GL_DBS.getMountFolder(),"readme.txt","Do not edit this folder.");
    redirectOverwrite="notFound";
    redirectForward="notFound";
    mainPageName="notFound";
    mainPageExternalCode="notFound";
    failedLogIn="no";
    showLogInLink="yes";
    fastUpdatingStat="yes";
    updateDefaultWebTexts(e_wsTexts);
    SF::varValPairs(e_defaultFindReplace,"_vVPair_","_/vVPair_","_vr_","_/vr_","_vl_","_/vl_",default_FindReplacePairs);
  }
  void Setup::updateDefaultWebTexts(const std::string & st){
    SF::varValPairs(st,"_vVPair_","_/vVPair_","_vr_","_/vr_","_vl_","_/vl_",defaultWebTexts);
  }
  void Setup::changeAlphabet(const std::string &a){
    GF::GL_Alphabet=a;
  }
  std::string Setup::printSetup() const{
      std::string fR=wsName+"<BR>\n";
      fR+=wsURL+"<BR> \n";
      fR+=fileUploadLoc+"<BR>\n";
      fR+=publicSystemFileLoc+"<BR>\n";
      fR+=publicPDFCertLoc+"<BR>\n";
      fR+=cookiePath+"<BR>\n";
      fR+=cookieName+"<BR>\n";
      fR+=challengeImages+"<BR>\n";
      fR+=challengeAnswers+"<BR>\n";
      return fR;
  }
  std::string Setup::getRedirectOverwrite() const{
      return redirectOverwrite;
  }
  void Setup::setRedirectOverwrite(const std::string &_r){
    redirectOverwrite=_r;
  }
  std::string Setup::getRedirectForward() const{
      return redirectForward;
  }
  void Setup::setRedirectForward(const std::string &_r){
    redirectForward=_r;
  }
  void Setup::setMainPageName(const std::string & _n){
    TMD::MText sf;
    int succ=sf.setFromTextName(_n);
    if(succ==1){
      mainPageName=_n;
      mainPageExternalCode=sf.getExternalCodeFromInternalId();
    }
  }
  std::string Setup::getMainPageName() const{
    return mainPageName;
  }
  std::string Setup::getMainPageExternalCode() const{
    return mainPageExternalCode;
  }
  std::string Setup::getFailedLogIn() const{
    return failedLogIn;
  }
  void Setup::setFailedLogIn(const std::string & _f){
    failedLogIn=_f;
  }
  void Setup::setLoginLink(const std::string & _f){
    showLogInLink=_f;
  }
  void Setup::setFastUpdatingStat(const std::string & _f){
    fastUpdatingStat=_f;
  }
  void Setup::setStartOfList(const std::string &_s){stList=_s;}
  void Setup::setSortCriterion(const std::string &_s){sortCr=_s;}
  std::string Setup::getStartOfList() const{return stList;}
  std::string Setup::getSortCriterion() const{return sortCr;}
}

#endif
