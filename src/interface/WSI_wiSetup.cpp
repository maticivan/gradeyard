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

#ifndef _INCL_WI_Setup_CPP
#define _INCL_WI_Setup_CPP

namespace WSI{
  class Setup{
  private:
    std::string mfB="_websiteName*!_";
    std::string mfE="_/websiteName*!_";
    std::string muB="_websiteURL*!_";
    std::string muE="_/websiteURL*!_";
    std::string mdOB="_debugging*!_";
    std::string mdOE="_/debugging*!_";
    std::string vstOB="_versionStop*!_";
    std::string vstOE="_/versionStop*!_";
    std::string cNB="_cookieName*!_";
    std::string cNE="_/cookieName*!_";
    std::string s_parPageB="_pVar*!_";
    std::string s_parPageE="_/pVar*!_";
    std::string s_RRC_B="_rrc*!_";
    std::string s_RRC_E="_/rrc*!_";
    std::string s_formNameRT_B="_fromName*!_";
    std::string s_formNameRT_E="_/fromName*!_";
    std::string s_respSubmitReqRT_B="_rSubmitR*!_";
    std::string s_respSubmitReqRT_E="_/rSubmitR*!_";
    std::string s_rSubMDYes_B="_respSbmYES!*_";
    std::string s_rSubMDYes_E="_/respSbmYES!*_";
    std::string s_rConveniencePackage_B="_convenience!*_";
    std::string s_rConveniencePackage_E="_/convenience!*_";
    std::string s_numQuestionsOnPage_B="_questionsOnPage!*_";
    std::string s_numQuestionsOnPage_E="_/questionsOnPage!*_";
    std::string s_rWST_B="_defaultTexts!*_";
    std::string s_rWST_E="_/defaultTexts!*_";
    std::string s_rDefA_B="_defaultAlphabet!*_";
    std::string s_rDefA_E="_/defaultAlphabet!*_";
    std::string s_rDFR_B="_defaultFindReplace!*_";
    std::string s_rDFR_E="_/defaultFindReplace!*_";
    std::string s_dTOTB="_tableOpenTag*!_";
    std::string s_dTOTE="_/tableOpenTag*!_";
    std::string s_dTHOTB="_theadOpenTag*!_";
    std::string s_dTHOTE="_/theadOpenTag*!_";
    std::string default_challengeImages="c0";
    std::string default_challengeAnswers="mainFolderDB/a0";
    std::string default_fileUploadLoc="f001";
    std::string default_publicSystemFileLoc="fs001";
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
    std::string default_e_alphabet="english";
    std::string default_e_defaultFindReplace=" ";
    long default_q_on_page=10;
    std::string default_cookieName="s";
    std::string wsName=default_wsName;
    std::string wsURL=default_wsURL;
    std::string dbOptions=default_dbOptions;
    std::string vstOptions=default_vstOptions;
    std::string fileUploadLoc=default_fileUploadLoc;
    std::string publicSystemFileLoc=default_publicSystemFileLoc;
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
    std::string e_alphabet= default_e_alphabet;
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
    void getSetupFromString(const std::string &);
    void changeAlphabet(const std::string &);
    void updateDefaultWebTexts(const std::string &);
    std::string getWSName() const;
    std::string getUploadLoc() const;
    std::string getPublicSystemFileLoc() const;
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
    std::string wrapTextToPreventAlphabetChange(const std::string & )const;
  };
  std::string Setup::getWSName() const{return wsName;}
  std::string Setup::getWSURL() const{return wsURL;}
  std::string Setup::getDebuggingOptions() const{return dbOptions;}
  std::string Setup::getVersionStopOptions() const{return vstOptions;}
  std::string Setup::getUploadLoc() const{    return fileUploadLoc; }
  std::string Setup::getPublicSystemFileLoc() const{ return publicSystemFileLoc;}
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
  std::string Setup::getAlphabet() const{return e_alphabet;}
  std::string Setup::wrapTextToPreventAlphabetChange(const std::string & t) const{
    if(e_alphabet=="english"){return t;}
    return "_doNotChangeAlphabet*_"+t+"_/doNotChangeAlphabet*_";
  }
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
  void Setup::getSetupFromString(const std::string & fN){
    std::string sSt=fN;
    sSt += mfB+default_wsName+mfE;
    sSt += muB+default_wsURL+muE;
    sSt += mdOB+default_dbOptions+mdOE;
    sSt += vstOB+default_vstOptions+vstOE;
    sSt += cNB+default_cookieName+cNE;
    sSt += s_parPageB + default_e_parPage + s_parPageE;
    sSt += s_RRC_B + default_e_respRecReqRT + s_RRC_E;
    sSt += s_formNameRT_B + default_e_formNameRT + s_formNameRT_E;
    sSt += s_respSubmitReqRT_B + default_e_respSubmitReqRT + s_respSubmitReqRT_E;
    sSt += s_rSubMDYes_B + default_e_respSubmitSuccRT + s_rSubMDYes_E;
    sSt += s_rConveniencePackage_B+default_e_conveniencePack+s_rConveniencePackage_E;
    sSt += s_numQuestionsOnPage_B+std::to_string(default_q_on_page)+s_numQuestionsOnPage_E;
    sSt += s_rWST_B+default_e_wsTexts+s_rWST_E;
    sSt += s_rDefA_B+default_e_alphabet+s_rDefA_E;
    sSt += s_rDFR_B+default_e_defaultFindReplace+s_rDFR_E;
    sSt += s_dTOTB+default_tableOpenTag+s_dTOTE;
    sSt += s_dTHOTB+default_theadOpenTag+s_dTHOTE;
    long pos=0;
    wsName=(SF::extract(sSt,pos,mfB,mfE)).first;
    pos=0;
    wsURL=(SF::extract(sSt,pos,muB,muE)).first;
    cookiePath=determineCookiePathFromURL(wsURL);
    pos=0;
    dbOptions=(SF::extract(sSt,pos,mdOB,mdOE)).first;
    pos=0;
    vstOptions=(SF::extract(sSt,pos,vstOB,vstOE)).first;
    pos=0;
    cookieName=(SF::extract(sSt,pos,cNB,cNE)).first;
    pos=0;
    e_parPage=(SF::extract(sSt,pos,s_parPageB,s_parPageE)).first;
    pos=0;
    e_respRecReqRT=(SF::extract(sSt,pos,s_RRC_B,s_RRC_E)).first;
    pos=0;
    e_formNameRT=(SF::extract(sSt,pos,s_formNameRT_B,s_formNameRT_E)).first;
    pos=0;
    e_respSubmitReqRT=(SF::extract(sSt,pos,s_respSubmitReqRT_B,s_respSubmitReqRT_E)).first;
    pos=0;
    e_respSubmitSuccRT= (SF::extract(sSt,pos,s_rSubMDYes_B,s_rSubMDYes_E)).first;
    pos=0;
    e_conveniencePackage= (SF::extract(sSt,pos,s_rConveniencePackage_B,s_rConveniencePackage_E)).first;
    pos=0;
    e_wsTexts= (SF::extract(sSt,pos,s_rWST_B,s_rWST_E)).first;
    pos=0;
    e_alphabet= (SF::extract(sSt,pos,s_rDefA_B,s_rDefA_E)).first;
    pos=0;
    tableOpenTag= (SF::extract(sSt,pos,s_dTOTB,s_dTOTE)).first;
    pos=0;
    theadOpenTag= (SF::extract(sSt,pos,s_dTHOTB,s_dTHOTE)).first;
    pos=0;
    e_defaultFindReplace= (SF::extract(sSt,pos,s_rDFR_B,s_rDFR_E)).first;
    pos=0;
    std::string tempQPageSt= (SF::extract(sSt,pos,s_numQuestionsOnPage_B,s_numQuestionsOnPage_E)).first;
    q_on_page=BF::stringToInteger(tempQPageSt);
    if(q_on_page<2){q_on_page=2;}
    fileUploadLoc=DD::GL_DBS.getPublicStorage();
    std::string redirectInfo="?rF="+fileUploadLoc;
    IOF::sys_createFolderIfDoesNotExist(fileUploadLoc,"index.html",HSF::redirectIndexHTML_F(redirectInfo,wsURL));
    publicSystemFileLoc=DD::GL_DBS.getPublicSystemStorage();
    redirectInfo="?rF="+publicSystemFileLoc;
    IOF::sys_createFolderIfDoesNotExist(publicSystemFileLoc,"index.html",HSF::redirectIndexHTML_F(redirectInfo,wsURL));
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
    e_alphabet=a;
  }
  std::string Setup::printSetup() const{
      std::string fR=wsName+"<BR>\n";
      fR+=wsURL+"<BR> \n";
      fR+=fileUploadLoc+"<BR>\n";
      fR+=publicSystemFileLoc+"<BR>\n";
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
