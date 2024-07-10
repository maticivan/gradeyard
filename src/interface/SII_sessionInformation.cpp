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


#ifndef _INCL_WI_SessionInformation_CPP
#define _INCL_WI_SessionInformation_CPP

namespace SII{

  std::string smallMapPrinting(const std::map<std::string,PSDI::GradingRule>&m ){
    std::string res;
    std::map<std::string,PSDI::GradingRule>::const_iterator it,itE;
    itE=m.end();
    it=m.begin();
    while(it!=itE){
      res+="\t"+it->first+" ";
      res+=std::to_string((it->second).points)+" "+ (it->second).display +"\n";
      ++it;
    }
    return res;
  }
  std::string rulesDebugPrinting(const std::map<std::string,std::map<std::string,PSDI::GradingRule> >& m){
    std::string debRes;
    std::map<std::string,std::map<std::string,PSDI::GradingRule> >::const_iterator it,itE;
    itE=m.end(); it=m.begin();
    while(it!=itE){
      debRes+="<pre>"+it->first+"\n";
      debRes+=smallMapPrinting(it->second);
      debRes+="</pre>\n";
      ++it;
    }
    return debRes;
  }
  MPTI::MainText SessionInformation::getHeader() const{return header;}
  MPTI::MainText SessionInformation::getFooter() const{return footer;}
  MPTI::MainText SessionInformation::getMainText() const{return mainText;}
  std::string SessionInformation::getResponse(const std::string & _q) const{
    return SF::getElFromMapOrNotFoundMessage(psd.respMap,_q,s_notFound);
  }
  std::vector<std::vector<std::string> > SessionInformation::getRespInOrder() const{return respInOrder;}
  SPREPF::StatData SessionInformation::prepareStatData() const{
    SPREPF::StatData forSt;
    forSt.userName=psd.my_un;
    TMF::Timer tm;
    forSt.timeString=tm.timeString();
    forSt.ipAddr=envVariables[11];
    forSt.att_page=psd.pageRequested;
    if(loginActionIndicator==1){
      forSt.att_page="!*LOGIN*ATTEMPT*!";
      forSt.userName=getResponse("username");
    }
    forSt.att_rr=respRecRequested;
    forSt.pass1=getResponse("pass1");
    STI::addDataToStat(forSt);
    return forSt;
  }
  void SessionInformation::addToMainText(const std::string &_a){mainText.addToText(_a);}
  std::string SessionInformation::preparePage(const std::string & queryAnswer){
    SPREPF::StatData st_D=prepareStatData();
    std::string preqN=STI::hidePageFromThoseWhoAreTryingToGuessPasswords(psd.pageRequested,st_D);
    STI::updateStatDB(st_D);
    if(fastUpdatingStat=="yes"){
      long timeSinceLastVisit=STI::secondsSinceLastVisit(st_D,psd.pageRequested);
      if(timeSinceLastVisit>3){
        STI::updateFastUpdatingStatDB(psd.pageRequested);
      }
    }
    if(preqN!=psd.pageRequested){
      psd.pageRequested=preqN;
      changeMainText(psd.pageRequested);
    }
    if((psd.my_un!="visitor")&&(psd.pageRequested=="login")){
      psd.pageRequested="loginSuccessful";
      changeMainText(psd.pageRequested);
    }
    if(psd.createStandardCourseSuccess=="yes"){
      changeMainText(psd.createStandardCourseMainDocName);
      psd.pageRequested=psd.createStandardCourseMainDocName;
    }
    header.updateLogInAndMenuBars(psd, psd.pageRequested,respRecRequested);
    footer.updateFooterBar(psd, psd.pageRequested,respRecRequested, psd.my_un);
    std::string savedPER=psd.pEditReq;
    psd.pEditReq="no";
    std::string fR=header.displayText(psd,"header");
    fR+="\n<div  class=\"container\">\n";
    std::string footerFR=footer.displayText(psd,"footer");
    footerFR= SF::findAndReplace(footerFR,"_THIS*WEBSITE*URL*_",MWII::GL_WI.getWSURL());
    psd.pEditReq=savedPER;
    if((respRecRequested!="")&&(indicatorRespRecInitialized==0)){
      mainRespRec.initialize(respRecRequested,sysDataRequested,psd.my_un);
    }
    if(respRecRequested==""){
      if((psd.pEditReq=="no")&&(psd.pageRequested=="listFiles")){
        mainText.selectVersionForListOfFiles(addModFileCodeReq,startOfList);
      }
      if(psd.createStandardCourseSuccess=="yes"){
        fR+=SF::findAndReplace(MWII::GL_WI.getDefaultWebText("standardCourseCreated"),"_*PAGE*NAME*_",psd.createStandardCourseMainDocName);
      }
      long pos; std::pair<std::string,int> allD;
      std::string openT,closeT;
      std::string mTextInMainPosition=mainText.displayText(psd,"mainTextPosition");
      std::string titleInterior=SF::getTagInteriorAndRemoveTag(mTextInMainPosition,"_title*_","_/title*_");
      std::string descInterior=SF::getTagInteriorAndRemoveTag(mTextInMainPosition,"_metaDesc*_","_/metaDesc*_");
      if((titleInterior=="")||(descInterior=="")){
        HTII::GL_title.pels=HTII::generatePageElements(mTextInMainPosition);
      }
      if(titleInterior==""){
        titleInterior=HTII::generateTitle(HTII::GL_title.pels);
      }
      else{
        if(HTII::GL_title.tSuggestion!=""){
          titleInterior=SF::findAndReplace(titleInterior,HTII::GL_title.codewordThatTitleGenerationIsNeeded,HTII::GL_title.tSuggestion);
        }
      }
      openT="<title>"; closeT="</title>";
      pos=0; allD=SF::extractAndReplace(fR,pos,openT,closeT,0,openT+titleInterior+closeT);
      if(allD.second==1){
        fR=allD.first;
      }
      if(descInterior==""){
        descInterior=HTII::generateDescription(HTII::GL_title.pels);
      }
      else{
        if(HTII::GL_title.dSuggestion!=""){
          descInterior=SF::findAndReplace(descInterior,HTII::GL_title.codewordThatDescGenerationIsNeeded,HTII::GL_title.dSuggestion);
        }
      }
      openT="<meta name=\"description\" content=\""; closeT="\">";
      pos=0; allD=SF::extractAndReplace(fR,pos,openT,closeT,0,openT+descInterior+closeT);
      if(allD.second==1){
        fR=allD.first;
      }
      fR+=mTextInMainPosition;
      if(str_backups_IfCalledFor!=""){
        fR+=BI::textAreaField("probText",str_backups_IfCalledFor,15,100);
      }
      fR+=DEBUGGING_ADDITIONS;
      fR+=HDDBRF::GL_DBREC_DEB;
    }
    else{
      fR+=mainRespRec.displayRespRec(psd);
      if(psd.respRecBackupText!=""){
        fR+=BI::textAreaField("probText",psd.respRecBackupText,15,100);
      }
      fR+=DEBUGGING_ADDITIONS;
    }
    if(str_pdfSummary_IfCalledFor!=""){
      fR+=BI::textAreaField("probText",str_pdfSummary_IfCalledFor,15,100);
      if(psd.pdfNameForInclassExam!=""){
        fR+="<p></p><p>\n";
        fR+=HSF::createButtonLink(psd.pdfNameForInclassExam,"Download PDF");
        fR+="</p>\n";
      }
    }
    if(debuggingEnvVarRequest==s_deb_correctDebuggingEnvVarReq){
      fR+= BI::envToHTML(envVariables);
    }
    std::string beforeFormRepl= DISPPF::finalizeForDisplay(MWII::GL_WI.getDefaultFindReplaceMap(), fR+psd.passwordChangeStatus+"</div>"+footerFR);
    std::string afterFormRepl=beforeFormRepl;
    std::map<std::string,std::string> replMap;
    if((psd.inFormSearch!="")&&(psd.inFormReplace!="")){
      replMap[psd.inFormSearch]=psd.inFormReplace;
    }
    if( (psd.isRoot=="no") && ((psd.pageRequested=="createWebsite")||(psd.pageRequested=="changePassword")) ){
      replMap["_THIS*WEBSITE*URL*_"]=MWII::GL_WI.getWSURL();
      replMap["_GUEST*SITES*_"]=DD::GL_DBS.getGuestClonesRelLoc();
      replMap["_MY*USERNAME*_"]=psd.my_un;
    }
    if(replMap.size()>0){
      afterFormRepl=MFRF::findAndReplace(afterFormRepl,replMap);
    }
    timeToGenerateWebsite.end();
    if(psd.queryAnswRequired){
      afterFormRepl=SF::findAndReplace(afterFormRepl,"!verbSave!","");
      afterFormRepl=SF::findAndReplace(afterFormRepl,"*fjkl3"+GL_MAIN_SETUP_FILE_NAME+"2!3211","!verbSave!");
      afterFormRepl=SF::findAndReplace(afterFormRepl, psd.queryAnswPlaceHolder,queryAnswer);
    }
    if(fastUpdatingStat=="yes"){
      std::map<std::string,std::string>::const_iterator it=psd.respMap.find("delFStat");
      if(it!=psd.respMap.end()){
        if(it->second=="yes"){
          STI::delFromFastUpdatingStatDB(psd.pageRequested);
        }
      }
    }
    afterFormRepl=LANGF::changeAlphabet(afterFormRepl,MWII::GL_WI.getAlphabet());
    afterFormRepl=INCII::treatInCommandInsert(afterFormRepl);
    GF::GL_DEB_MESSAGES.addMessage("Time to generate page is "+ BF::doubleToString(timeToGenerateWebsite.getTime())); 
    afterFormRepl+=GF::GL_DEB_MESSAGES.prepareAllMessages(MWII::GL_WI.getDebuggingOptions());
    return afterFormRepl;
  }
  void SessionInformation::addDebuggingMessagesIfInDebuggingMode(){
    if(debuggingModeRequest==s_deb_correctDebuggingModeReq){
      addToMainText(GF::GL_DEB_MESSAGES.prepareAllMessages(MWII::GL_WI.getDebuggingOptions()));
    }
    if(MWII::GL_WI.getVersionStopOptions()!="stop"){
      std::map<std::string,std::string>::const_iterator itM;
      itM=(psd.respMap).find("version");
      if(itM!=(psd.respMap).end()){
        addToMainText(GL_VERSION);
      }
    }
  }
  std::vector<std::string> SessionInformation::envVars() const{return envVariables;}
  SessionInformation::SessionInformation(){
    timeToGenerateWebsite.start();
    psd.my_un="visitor";
    psd.isRoot="no";
    psd.allowedToExecuteAll="no";
    psd.pEditReq="no";
    psd.respRecMode="df";
    psd.respRecFlag="notReceived";
    psd.versionToLatex="0";
    psd.messEditReq="no";
    psd.sortCriterion="0";
    psd.couasEditReq="no";
    psd.rrgrader="no";
    psd.indChangeRespRecToPrintVersionOfCommonInClassExam=0;
    psd.probVersionsOfChangedRespRec="";
    psd.pdfNameForInclassExam="";
    psd.masterKey="";
    psd.queryAnswRequired=0;
    psd.queryAnswPlaceHolder="_*"+RNDF::genRandCode(15)+"_*qAsnw_";
    psd.displayDaysInWeek.resize(0);
    psd.displayMonthsInYear.resize(0);
    indicatorInitialized=0;
    indicatorFormResponded=-1;
    loginActionIndicator=0;
    loginStatusIndicator=0;
    indicatorFileReceived=-1;
    indicatorFileCanBeAccepted=-1;
    loginFailedIndicator=0;
    str_backups_IfCalledFor="";
    psd.inFormSearch="";
    psd.inFormReplace="";
    psd.messEditCode="";
    idOfMessageWhoseEditWasSubmitted="";
    newTextOfMessage="";
  }
  std::string SessionInformation::cookieText(const std::string &cookieName , const std::string &cookieValue ) const{
    std::string fR="";
    std::string domainName=MWII::GL_WI.getWSURL();
    fR+="Set-Cookie:";
    fR+=cookieName;fR+="=";
    if(cookieValue!="deleted"){
      fR+=cookieValue;
    }
    else{
      fR+="deleted";
    }
    fR+=";";
    fR+="Expires=";
    TMF::Timer tmC;
    if(cookieValue!="deleted"){
      fR+=tmC.cookieExpiration(0,3,0,0);//3 hours cookie
    }
    else{
      std::vector<long> timeInPast;
      timeInPast.resize(7);timeInPast[0]=1900;timeInPast[1]=0;timeInPast[2]=1;timeInPast[3]=10;
      timeInPast[4]=10;timeInPast[5]=10;timeInPast[6]=1;
      fR+=tmC.timeString(timeInPast);
    }
    fR+="; Path="+MWII::GL_WI.getCookiePath();
    fR+="; Secure; HttpOnly";
    return fR;
  }
  long SessionInformation::countSubmittedFiles(const cgicc::Cgicc & ch) const{
    return ch.getFiles().size();
  }
  int SessionInformation::uploadToServer(const cgicc::Cgicc & ch, const std::string & labelName, const std::string &fileName ){
    cgicc::const_file_iterator file = ch.getFile(labelName);
     if(file != ch.getFiles().end()) {
        std::ofstream mfileos;
        mfileos.open(fileName);
        file->writeToStream(mfileos);
        mfileos.close();
     }
     return 1;
  }
  void SessionInformation::analyzeEnvVarsAndForms(const cgicc::Cgicc & ch){
    indicatorFormResponded=0;
    indicatorFileReceived=0;
    psd.pEditReq="no";
    startOfList="";
    addModFileReq="nothing";
    addModFileCodeReq="";
    psd.pageRequested="";
    psd.comfUserEdit="n";
    respRecRequested="";
    respSubmitted="no";
    sysDataRequested="no17";
    debuggingEnvVarRequest="no";
    debuggingModeRequest="no";
    addModFileModifyInfo="no";
    psd.usrAgent=envVariables[8];//HTTP_USER_AGENT
    psd.remAddr=envVariables[11];//REMOTE_ADDR
    psd.reqMethod=envVariables[13];//REQUEST_METHOD
    if((envVariables[13]=="GET")||(envVariables[13]=="POST")){
      if(envVariables[13]=="POST"){indicatorFormResponded=1;}
      long i=0;
      MWII::GL_WI.setRedirectOverwrite(s_notFound);
      MWII::GL_WI.setRedirectForward(s_notFound);
      cgicc::const_form_iterator it, itE;
      itE = ch.getElements().end();
      it=ch.getElements().begin();
      long sz=ch.getElements().size();
      respInOrder.resize(sz);
      long skipOtherIfs;
      while(it!=itE){
        respInOrder[i].resize(2);
        respInOrder[i][0]=it->getName();
        respInOrder[i][1]=it->getValue();
        (psd.respMap)[respInOrder[i][0]]=respInOrder[i][1];
        skipOtherIfs=0;
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_pageName)) {
          skipOtherIfs=1;psd.pageRequested=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_respRecName)){
          skipOtherIfs=1;respRecRequested=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_respSubmit)){
          skipOtherIfs=1;
          if(respInOrder[i][1]==m_respSubmit){
            respSubmitted="yes";
          }
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_searchFor)){
          skipOtherIfs=1;
          psd.inFormSearch=SACF::sanitizeInFormSearchFor(respInOrder[i][1]);
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_replaceWith)){
          skipOtherIfs=1;
          DISPPF::RequestsForSanitizer reqS;
          psd.inFormReplace=DISPPF::sanitizeForDisplay(respInOrder[i][1],reqS);
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_messToEdit)){
          skipOtherIfs=1;
          psd.messEditCode=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_messEditSubmission)){
          skipOtherIfs=1;
          idOfMessageWhoseEditWasSubmitted=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_newTextWithMessage)){
          skipOtherIfs=1;
          newTextOfMessage=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_responseReceiverDisplayRequest)){
          skipOtherIfs=1;
          psd.respRecMode=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_verReq)){
          skipOtherIfs=1;
          psd.versionToLatex=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_startOfList)){
          skipOtherIfs=1;
          startOfList=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_sortCriterion)){
          skipOtherIfs=1;
          psd.sortCriterion=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_addModFileReq)){
          skipOtherIfs=1;
          addModFileReq=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_addModFileCodeReq)){
          skipOtherIfs=1;
          addModFileCodeReq=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_addModFileModifyInfo)){
          skipOtherIfs=1;
          addModFileModifyInfo=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_systemDataDisplayRequest)){
          skipOtherIfs=1;
          sysDataRequested=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_debEnvVarRequest)){
          skipOtherIfs=1;
          debuggingEnvVarRequest=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_debModeRequest)){
          skipOtherIfs=1;
          debuggingModeRequest=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_redirectOverwrite)){
          skipOtherIfs=1;
          MWII::GL_WI.setRedirectOverwrite(respInOrder[i][1]);
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_redirectToForward)){
          skipOtherIfs=1;
          MWII::GL_WI.setRedirectForward(respInOrder[i][1]);
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_editReq)){
          skipOtherIfs=1;
          if((respInOrder[i][1]==m_editReqY)||(respInOrder[i][1]==m_editReqR)||(respInOrder[i][1]==m_editReqW)){
            psd.pEditReq=respInOrder[i][1];}
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_editMessReq)){
          skipOtherIfs=1;
          if((respInOrder[i][1]==m_editReqY)||(respInOrder[i][1]==m_editReqR)){
            psd.messEditReq=respInOrder[i][1];
          }
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_editCouasReq)){
          skipOtherIfs=1;
          psd.couasEditReq=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_confirmationOfComfortableUserEdit)){
          skipOtherIfs=1;
          psd.comfUserEdit=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_passwCh)){
          skipOtherIfs=1;
          psd.passwordChangeRequested=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_respRecThatWillGradeCouas)){
          skipOtherIfs=1;
          psd.rrgrader=respInOrder[i][1];
        }
        if((skipOtherIfs==0)&&(respInOrder[i][0]==e_userLogin)){
          skipOtherIfs=1;
          if(respInOrder[i][1]==m_logIn){
            loginActionIndicator=1;
          }
          else{
            loginActionIndicator=-1;
          }
        }
        MWII::GL_WI.setStartOfList(startOfList);
        MWII::GL_WI.setSortCriterion(psd.sortCriterion);
        ++it;++i;
      }
      GF::GL_DEB_MESSAGES.addMessage("The number of files received is "+std::to_string(countSubmittedFiles(ch)));
    }
  }
  std::string SessionInformation::placeNewFileToServer(const cgicc::Cgicc & ch, const std::string & username, const std::string & extension, const std::string & formFieldName,const std::string &additionalData){
    if(allowedToUploadNewFile(username)==0){
      return "failed";
    }
    PublicFile pf;
    int ind=pf.init("!*new!*",extension,username,additionalData);
    if(ind==0){
      return "failed";
    }
    uploadToServer(ch,formFieldName,pf.getFilePath()+"/"+pf.getFileName());
    return pf.getFilePath()+"/"+pf.getFileName();
  }
  std::string SessionInformation::updateExistingFile(const cgicc::Cgicc & ch, const std::string & username, const std::string & extCode, const std::string & formFieldName,const std::string &additionalData){
    if(allowedToModifyFile(username,extCode)==0){
      return "Not permitted";
    }
    FMD::FileManager sf;
    int succ=sf.setFromExternalCode(extCode);
    if(succ==0){
      return "File does not exist";
    }
    std::string fileInfoData=sf.getTextData();
    long pos=0;
    std::pair<std::string,int> allD=SF::extract(fileInfoData,pos,"_fInfo*|_","_/fInfo*|_");
    std::string toSearch, toReplace;
    if(allD.second==1){
      toSearch="_fInfo*|_"+allD.first+"_/fInfo*|_";
      pos=0;allD=SF::extract(additionalData,pos,"_fInfo*|_","_/fInfo*|_");
      if(allD.second==1){
        toReplace="_fInfo*|_"+allD.first+"_/fInfo*|_";
        fileInfoData=SF::findAndReplace(fileInfoData,toSearch,toReplace);
      }
    }
    pos=0;allD=SF::extract(additionalData,pos,"_fSize*|_","_/fSize*|_");
    // if allD.second==0, the new file is not submitted with the form
    if(allD.second==1){
      std::string newFSize="_fSize*|_"+allD.first+"_/fSize*|_";
      std::string fName=extCode;
      pos=0;allD=SF::extract(fileInfoData,pos,"_fileName_","_/fileName_");
      if(allD.second==1){
        fName=allD.first;
      }
      pos=0;allD=SF::extract(fileInfoData,pos,"_fileExtension_","_/fileExtension_");
      std::string extOld="na";
      if(allD.second==1){
        extOld=allD.first;
        pos=0;allD=SF::extract(additionalData,pos,"_fileDataType_","_/fileDataType_");
        if(allD.second==1){
          std::string correctExtension=extOld+"bad";
          if(allD.first=="application/pdf"){
            correctExtension="pdf";
          }
          if(allD.first=="image/jpeg"){
            correctExtension="jpg";
          }
          if(extOld==correctExtension){
            std::string filePath;
            pos=0;allD=SF::extract(fileInfoData,pos,"_filePath_","_/filePath_");
            if(allD.second==1){
                uploadToServer(ch, formFieldName, allD.first+"/"+MWII::FILE_PREFIX+ fName+"."+extOld );
            }
            pos=0;allD=SF::extractAndReplace(fileInfoData,pos,"_fSize*|_","_/fSize*|_",0,newFSize);
            if(allD.second==1){
              fileInfoData=allD.first;
            }
          }
        }
      }
      pos=0;allD=SF::extract(additionalData,pos,"_fInfo*|_","_/fInfo*|_");;
      if(allD.second==1){
        toReplace="_fInfo*|_"+allD.first+"_/fInfo*|_";
        fileInfoData=SF::findAndReplace(fileInfoData,toSearch,toReplace);
      }
    }
    sf.setTextData(fileInfoData);
    sf.putInDB();
    return "";
  }
  long SessionInformation::loggedIn(){
    return loginStatusIndicator;
  }
  int SessionInformation::passedAntiSpam() const{
    return psd.passedAntiSpam;
  }
  long checkAntiSpam(const std::string & code, const std::string & solution){
    std::string fileWithCode=DD::GL_DBS.getChallengeAnswStorage()+"/a"+solution+".dat";
    std::string sol=IOF::fileToString(fileWithCode);
    if(sol=="fileNotFound"){return 0;}
    IOF::sys_deleteFile(fileWithCode);
    if(sol==code){
      return 1;
    }
    return 0;
  }
  void SessionInformation::initSession(const cgicc::Cgicc & ch){
    if(indicatorInitialized==0){
      indicatorRespRecInitialized=0;
      psd.my_un="visitor";
      psd.myFirstName="";
      psd.myLastName="";
      showLogInLink="yes";
      fastUpdatingStat="yes";
      psd.passwordChangeRequested="no";
      psd.passwordChangeStatus="";
      psd.allowRespRecDisplayToOthers="no";
      CAGI::GL_Obf.uniqueRandomCode=RNDF::genRandCode(5);
      CAGI::GL_Obf.secretOpenTag="Bb"+CAGI::GL_Obf.uniqueRandomCode+"e";
      CAGI::GL_Obf.secretCloseTag="Eb"+CAGI::GL_Obf.uniqueRandomCode+"e";
      CAGI::GL_Obf.sequencesNotAllowedExplanation="Arrays, sequences, vectors, and maps were not allowed, but your code tried to use them.";
      CAGI::GL_Obf.wordNotAllowed="Your code tried to use an illegal word: ";
      HTII::GL_title.nTGenerations=0;
      HTII::GL_title.nDGenerations=0;
      HTII::GL_title.maxTitleLength=60;
      HTII::GL_title.maxDescLength=300;
      HTII::GL_title.codewordThatTitleGenerationIsNeeded="_*autoTitle*_";
      HTII::GL_title.codewordThatDescGenerationIsNeeded="_*autoDesc*_";
      envVariables=BI::getEnvVars();
      analyzeEnvVarsAndForms(ch);
      currentCookie=envVariables[24];
      if(currentCookie!=s_notFound){
        currentCookie+=";"+MWII::GL_WI.getCookieName()+"="+s_notFound;
        currentCookie+=";"+MWII::GL_WI.getCookieName()+"="+s_notFound;
        long pos=0;
        std::pair<std::string,int> allD =SF::extract(currentCookie,pos,MWII::GL_WI.getCookieName()+"=",";");
        if(allD.second==1){
          currentCookie=allD.first;
        }
        else{
          currentCookie=s_notFound;
        }
      }
      if(currentCookie!=s_notFound){
        std::pair<std::string,std::string> uNRN=SF::oneWordToTwoWords(currentCookie);
        int succ=(psd.myWU).setFromExternalId(uNRN.first);
        if(succ==1){
          if((psd.myWU).checkCookieCorrectness(currentCookie)){
            loginStatusIndicator=1;
            (psd.myWU).updateEncryptionDataAfterCookieReading(currentCookie);
          }
        }
      }
      if((loginActionIndicator==-1)&&(loginStatusIndicator==1)){
        (psd.myWU).logOut();
        std::cout<<cookieText(MWII::GL_WI.getCookieName(),"deleted")<<"\n";
        IOF::deleteOldFiles(DD::GL_DBS.getChallengeAnswStorage(),(psd.myWU).getUsername(),0);
        loginStatusIndicator=0;
      }
      if((loginActionIndicator==1)&&(loginStatusIndicator==0)){
        SPREPF::StatData st_DLogin=prepareStatData();
        if(STI::checkIfSpammerIsTryingToGuessPasswords(st_DLogin)=="spammer"){
          loginActionIndicator=0;
          psd.pageRequested="tooManyAttemptsInShortTime";
          changeMainText(psd.pageRequested);
        }
        else{
          int succ=(psd.myWU).setFromUsername(getResponse("username"));
          loginFailedIndicator=1;
          if(succ==1){
            succ=(psd.myWU).checkPasswordAndSetLevel1(getResponse("pass1"));
            if(succ==1){
              loginStatusIndicator=1;
              std::string newCookie=(psd.myWU).logIn();
              IOF::deleteOldFiles(DD::GL_DBS.getChallengeAnswStorage(),(psd.myWU).getUsername(),0);
              IOF::deleteOldFiles(DD::GL_DBS.getChallengeAnswStorage(),"txt",10800,1);
              std::cout<<cookieText(MWII::GL_WI.getCookieName(),newCookie);
              loginFailedIndicator=0;
            }
            else{
              //failed log in - we should propagate the desired redirect page
              MWII::GL_WI.setFailedLogIn();
            }
          }
          else{
            MWII::GL_WI.setFailedLogIn();
          }
        }
      }
      if(loginStatusIndicator==1){
        if(loginActionIndicator==0){//renew cookie
          std::cout<<cookieText(MWII::GL_WI.getCookieName(),currentCookie);
        }
        psd.my_un=(psd.myWU).getUsername();
        psd.myFirstName=(psd.myWU).getFirstName();
        psd.myLastName=(psd.myWU).getLastName();
        psd.isRoot=(psd.myWU).isRoot();
        psd.allowedToExecuteAll=(psd.myWU).isAllowedToExecuteCommands();
        psd.masterKey=(psd.myWU).getMasterKeyFromRAM("sigmaM");
      }
      GF::GL_DEB_MESSAGES.addMessage("The current cookie is |"+currentCookie+"|\n");
      TMD::MText m;
      int s=m.setFromTextName("mainTextInitializer");
      if(s==1){
        std::string initText=m.getTextData();
        std::map<std::string,std::string> mParameters;
        std::map<std::string,std::string>::const_iterator itMP;
        SF::varValPairs(initText,"_vVPair_","_/vVPair_","_vr_","_/vr_","_vl_","_/vl_",mParameters);
        itMP=mParameters.find("defaultHeaderName");
        if(itMP!=mParameters.end()){
          dHN=itMP->second;
          header.initialize(dHN);
        }
        itMP=mParameters.find("defaultFooterName");
        if(itMP!=mParameters.end()){
          dFN=itMP->second;
          footer.initialize(dFN);
        }
        SF::assignValueFromMap(mParameters,"defaultFirstPage",dfPage);
        itMP=mParameters.find("defaultAlphabet");
        if(itMP!=mParameters.end()){
          MWII::GL_WI.changeAlphabet(itMP->second);
          SF::assignValueFromMap(mParameters,"daysOfTheWeek",psd.displayDaysInWeek);
          SF::assignValueFromMap(mParameters,"namesOfTheMonths",psd.displayMonthsInYear);
        }
        AGRDI::GL_AGParameters.colSize=10;
        SF::assignValueFromMap(mParameters,"agColSize",AGRDI::GL_AGParameters.colSize);
        if((AGRDI::GL_AGParameters.colSize<1)||(AGRDI::GL_AGParameters.colSize>30)){
          AGRDI::GL_AGParameters.colSize=10;
        }
        AGRDI::GL_AGParameters.compilerErrors="yes";
        SF::assignValueFromMap(mParameters,"agCErrors",AGRDI::GL_AGParameters.compilerErrors);
        SF::assignValueFromMap(mParameters,"maxTitleLength",HTII::GL_title.maxTitleLength);
        if((HTII::GL_title.maxTitleLength<1)||(HTII::GL_title.maxTitleLength>1000)){
          HTII::GL_title.maxTitleLength=10;
        }
        SF::assignValueFromMap(mParameters,"maxDescLength",HTII::GL_title.maxDescLength);
        if((HTII::GL_title.maxDescLength<1)||(HTII::GL_title.maxDescLength>3000)){
          HTII::GL_title.maxDescLength=300;
        }
        SF::assignValueFromMap(mParameters,"codeWordForAutoGenTitle",HTII::GL_title.codewordThatTitleGenerationIsNeeded);
        SF::assignValueFromMap(mParameters,"codeWordForAutoGenDesc",HTII::GL_title.codewordThatDescGenerationIsNeeded);
        SF::assignValueFromMap(mParameters,"allowRespRecToOthers",psd.allowRespRecDisplayToOthers);
        SF::assignValueFromMap(mParameters,"sNotAllowed",CAGI::GL_Obf.sequencesNotAllowedExplanation);
        SF::assignValueFromMap(mParameters,"wNotAllowed",CAGI::GL_Obf.wordNotAllowed);
        SF::assignValueFromMap(mParameters,"executePublicTestCases",APTI::GL_studentsAllowedToExecuteCodeOnPublicTestCases);
        SF::assignValueFromMap(mParameters,"maxNumForSimpson",FF::MAXIMAL_NUMBER_OF_INTERVALS_FOR_SIMPSON);
        SF::assignValueFromMap(mParameters,"allowPDFForCertificates",CERD::GL_CertificatesOptions.pdfsAllowed);
        SF::assignValueFromMap(mParameters,"timeInSecondsToKeepCertificatePDF",CERD::GL_CertificatesOptions.maxTimeToKeepPDF);
        SF::assignValueFromMap(mParameters,"allowPDFForForms",FFI::GL_PDFFormOptions.pdfsAllowed);
        SF::assignValueFromMap(mParameters,"timeInSecondsToKeepFormPDF",FFI::GL_PDFFormOptions.maxTimeToKeepPDF);
        itMP=mParameters.find("defaultTexts");
        if(itMP!=mParameters.end()){
          MWII::GL_WI.updateDefaultWebTexts(itMP->second);
        }
        itMP=mParameters.find("showLogInLink");
        if(itMP!=mParameters.end()){
          showLogInLink=itMP->second;
          MWII::GL_WI.setLoginLink(showLogInLink);
        }
        itMP=mParameters.find("fastUpdatingStat");
        if(itMP!=mParameters.end()){
          fastUpdatingStat=itMP->second;
          MWII::GL_WI.setFastUpdatingStat(fastUpdatingStat);
        }

        if(MWII::GL_WI.getRedirectOverwrite()!="notFound"){
          std::pair<std::string,std::string> twoCodes=SF::oneWordToTwoWords(MWII::GL_WI.getRedirectOverwrite());
          TMD::MText sf;
          int sc=sf.setFromExternalCode(twoCodes.first);
          if(sc==1){
            psd.pageRequested=sf.getTextName();
          }
        }
        if(loginFailedIndicator==1){
          psd.pageRequested="wrongUserNameOrPassword";
        }
        if(psd.pageRequested!=""){
          if((psd.pageRequested=="createWebsite")||(psd.pageRequested=="deleteWebsite")){
            createOrDeleteWebsiteOrChangeRequestedPage();
          }
          int initSucc=mainText.initialize(psd.pageRequested,sysDataRequested,psd.my_un);
          if(initSucc==0){
            psd.pageRequested="pageDoesNotExist";
            mainText.initialize(psd.pageRequested,sysDataRequested,psd.my_un);
          }
          MWII::GL_WI.setMainPageName(psd.pageRequested);
        }
        else{
          mainText.initialize(dfPage,"no",psd.my_un);
          MWII::GL_WI.setMainPageName(dfPage);
          psd.pageRequested=dfPage;
        }
        if(addModFileReq==m_addModFileDelete){
          if(allowedToCreateText()==1){//WARNING - allowedToDeleteFile is giving "yes" too easy to accommodate the following code
            //Step 1: Determine if the file is submitted by a form.
            //        and if it is, then tell the form that the file is deleted
            FMD::FileManager sf;
            int scc=sf.setFromExternalCode(addModFileCodeReq);
            if(scc==1){
              std::string td=sf.getTextData();
              long pos; std::pair<std::string, int> allD;
              pos=0; allD=SF::extract(td,pos,"_nRespRec*|_","_/nRespRec*|_");
              if(allD.second==1){
                RMD::Response rm;
                scc=rm.setFromTextName(allD.first);
                if(scc==1){
                  td=rm.getTextData();
                  pos=0;allD=SF::extract(td,pos,"_nf*|_","_/nf*|_");
                  long numFAllowed=0;
                  if(allD.second==1){
                    numFAllowed=BF::stringToInteger(allD.first);
                  }
                  long found=0;long i=0;
                  std::string sep_B,sep_E,insideText;
                  while((found==0)&&(i<numFAllowed)){
                    sep_B="f"+BF::padded(i,100,"0")+"*|_";
                    sep_E="_/"+sep_B;
                    sep_B="_"+sep_B;
                    pos=0;allD=SF::extract(td,pos,sep_B,sep_E);
                    if(allD.second==1){
                      if(allD.first!=SF::findAndReplace(allD.first,MWII::FILE_PREFIX+addModFileCodeReq+".","")){
                        found=1;
                        pos=0;allD=SF::extractAndReplace(td,pos,sep_B,sep_E);
                        if(allD.second==1){
                          td=allD.first;
                          rm.setTextData(td);
                          rm.putInDB();
                        }

                      }
                    }
                    ++i;
                  }
                }

              }
            }
            deleteFile(addModFileCodeReq);
          }
        }
        if(envVariables[13]=="POST"){
          if(countSubmittedFiles(ch)>0){
            indicatorFileReceived=1;
            if(addModFileReq==m_addModFileNew){
              //add new file and update the info with addModFileModifyInfo
              cgicc::const_file_iterator fileIt, fileItE;
              fileItE=ch.getFiles().end();
              fileIt=ch.getFile(e_addModFileFilePar);
              if(fileIt!=fileItE){
                std::string additionalData="_fInfo*|_"+addModFileModifyInfo+"_/fInfo*|_";
                additionalData+="_fileDataType_"+fileIt->getDataType()+"_/fileDataType_";
                additionalData+="_fSize*|_"+std::to_string(fileIt->getDataLength())+"_/fSize*|_";
                placeNewFileToServer(ch,psd.my_un,"findOutFromDataType",e_addModFileFilePar,additionalData);
              }
            }
          }
          if(addModFileReq==m_addModFileEdit){
            // update file info with addModFileModifyInfo.
            // If a new file is received, then replace the old file.
            cgicc::const_file_iterator fileIt, fileItE;
            fileItE=ch.getFiles().end();
            fileIt=ch.getFile(e_addModFileFilePar);
            std::string additionalData="_fInfo*|_"+addModFileModifyInfo+"_/fInfo*|_";;
            if(fileIt!=fileItE){
              additionalData+="_fileDataType_"+fileIt->getDataType()+"_/fileDataType_";
              additionalData+="_fSize*|_"+std::to_string(fileIt->getDataLength())+"_/fSize*|_";
            }
            updateExistingFile(ch,psd.my_un,addModFileCodeReq,e_addModFileFilePar,additionalData);
          }
          if(psd.couasEditReq=="y"){
            updateGradesFromResponse();
          }
        }
        if(respRecRequested!=""){
          if(respSubmitted=="yes"){
            RTI::Response tmpRT(respRecRequested,"no",psd.my_un);
            if(tmpRT.isInitialized()==1){
              RTI::ResponderInfo res=tmpRT.infoFromResponseText(psd,"df");
              if(res.documentType=="gradeOfResponse"){
                updateRespMapToProperlyAccountForBothGraderCommentsAndPoints(res);
              }
              if(res.exitStatus=="badForm"){
                res.acceptResp=0;
              }
              if(
                ((res.acceptResp==1)&&(res.documentType=="responseToTest"))
                ||
                ((res.acceptGrade==1)&&(res.documentType=="gradeOfResponse"))
                ){
                long pos;
                std::pair<std::string,int> allD;
                std::string dataToModify=tmpRT.getRawText();
                if(dataToModify!=""){
                  std::string dataToAdd;
                  std::map<std::string,std::string> replMap;
                  std::set<std::string> qLabels;
                  qLabels=res.questionLabels;
                  std::vector<std::string> oldAnswers=SF::stringToVector(dataToModify,"_in*|_","_/in*|_");
                  std::vector<std::string> qLbs;
                  long sz=oldAnswers.size();
                  qLbs.resize(sz);
                  std::map<std::string,long> invFunc;
                  std::string s_inRespLabelB="_aw*|_";
                  std::string s_inRespLabelE="_/aw*|_";
                  if(res.documentType=="gradeOfResponse"){
                    s_inRespLabelB="_gc*|_";
                    s_inRespLabelE="_/gc*|_";
                  }
                  for(long i=0;i<sz;++i){
                    pos=0;
                    allD=SF::extract( oldAnswers[i],pos,"_lb*|_", "_/lb*|_");
                    qLbs[i]="";
                    if(allD.second==1){
                      qLbs[i]=allD.first;
                      invFunc[qLbs[i]]=i;
                    }
                  }
                  std::string answUpdated;int isCertificate; std::string certLabel;
                  std::set<std::string>::const_iterator it,itE;
                  std::map<std::string,std::string>::const_iterator itM,itME;
                  std::map<std::string,long>::const_iterator itInvF, itInvFE;
                  itE=qLabels.end();it=qLabels.begin();
                  itInvFE=invFunc.end();
                  itME=(psd.respMap).end();
                  int dangerousInputDetected=0;
                  while((it!=itE)&&(dangerousInputDetected==0)){
                    itM=(psd.respMap).find(*it);
                    if(itM!=itME){
                      DISPPF::RequestsForSanitizer reqS;
                      dangerousInputDetected=(DISPPF::sanitizeForDisplay(itM->second,reqS)=="unsafeInput");
                      if(dangerousInputDetected==0){
                        isCertificate=CERTCLI::doesLabelCorrespondToCertificate(itM->first);
                        answUpdated="_in*|_ "+s_inRespLabelB;
                        if(isCertificate==0){
                          answUpdated+=itM->second;
                        }
                        else{
                          certLabel=CERTCLI::sanitizeCertificateComment(itM->second);
                          answUpdated+=certLabel;
                        }
                        answUpdated+=s_inRespLabelE+" _lb*|_";
                        answUpdated+=itM->first+"_/lb*|_ _/in*|_";
                        itInvF=invFunc.find(itM->first);
                        if(itInvF==itInvFE){
                          dataToAdd+=answUpdated;
                        }
                        else{
                          replMap["_in*|_"+oldAnswers[itInvF->second]+"_/in*|_"]=answUpdated;
                        }
                        if(isCertificate){
                          std::string oldCertificate;
                          if(itInvF!=itInvFE){oldCertificate="_in*|_"+oldAnswers[itInvF->second]+"_/in*|_";}
                          if(oldCertificate!=answUpdated){
                            dataToModify=CERTCLI::updateCertificate(psd,dataToModify, certLabel,res);
                          }
                        }
                      }
                    }
                    ++it;
                  }
                  dataToModify=MFRF::findAndReplace(dataToModify,replMap);
                  dataToModify+=dataToAdd;
                  if(dangerousInputDetected==0){
                    psd.respRecFlag="accepted";
                    dataToModify=uploadFilesFromResponseReceiver(ch,respRecRequested,dataToModify,res.fileInfoV,tmpRT,res.idInfoData);
                    modifyRespRec(respRecRequested, dataToModify,"yes");
                  }
                  else{
                    psd.respRecFlag="rejected";
                  }
                }
              }
            }
          }
          indicatorRespRecInitialized=mainRespRec.initialize(respRecRequested,sysDataRequested,psd.my_un);
          if(indicatorRespRecInitialized==0){
            respRecRequested="";
          }
        }
      }
    }
    if(idOfMessageWhoseEditWasSubmitted!=""){
      if(idOfMessageWhoseEditWasSubmitted=="nm"){
        createMessage("_!myself!_",newTextOfMessage,psd.inFormReplace);
      }
      else{
        modifyMessage(idOfMessageWhoseEditWasSubmitted,newTextOfMessage);
      }
    }
    if((psd.passwordChangeRequested=="yes")&&(loginStatusIndicator==1)) {
      psd.passwordChangeStatus="<P>"+analyzeRequestToChangePassword()+"</P>";
    }
    psd.passedAntiSpam=0;
    std::string antiSpamFileName=getResponse("asc");
    if(antiSpamFileName!=s_notFound){
      psd.passedAntiSpam=checkAntiSpam(getResponse("antiSpamCode"),antiSpamFileName);
    }
    indicatorInitialized=1;
  }
  std::string SessionInformation::analyzeRequestToChangePassword(){
    std::string oldPass,p1,p2;
    std::map<std::string,std::string>::const_iterator it,itE;
    itE=(psd.respMap).end();
    it=(psd.respMap).find("oldP");
    if(it==itE){
      return MWII::GL_WI.getDefaultWebText("Password change failed. Old password not provided.");
    }
    oldPass=it->second;
    it=(psd.respMap).find("pass1");
    if(it==itE){
      return MWII::GL_WI.getDefaultWebText("Password change failed. New password not provided.");
    }
    p1=it->second;
    it=(psd.respMap).find("pass2");
    if(it==itE){
      return MWII::GL_WI.getDefaultWebText("Password change failed. New password not provided.");
    }
    p2=it->second;
    if(p1!=p2){
      return MWII::GL_WI.getDefaultWebText("Password change failed. Two new passwords do not match.");
    }
    if((psd.myWU).checkPasswordAndSetLevel1(oldPass)==0){
      return MWII::GL_WI.getDefaultWebText("Password change failed. Old password incorrect.");
    }
    if(p1.length()<5){
      return MWII::GL_WI.getDefaultWebText("Password change failed. New password is too weak.");
    }
    (psd.myWU).addUserData(p1);
    return MWII::GL_WI.getDefaultWebText("Password changed.");
  }
  long SessionInformation::isInitialized() const{return indicatorInitialized;}
  long SessionInformation::isFormResponded() const{return indicatorFormResponded;}
  long SessionInformation::isFileReceived() const{return indicatorFileReceived;}
  long SessionInformation::canFileBeAccepted() const{return indicatorFileCanBeAccepted;}
  std::string SessionInformation::enhanceTheCommandDueToComfUserEdit(const std::string & s) const{
    if(psd.comfUserEdit!="y"){
      return s;
    }
    return CCFI::enhanceTheCommandDueToComfUserEdit(psd,s);
  }
  void SessionInformation::changeMainText(const std::string &_t){
    if((_t=="")&&(psd.pageRequested!="")){
      mainText.initialize(psd.pageRequested,sysDataRequested,psd.my_un);
    }
    else{
      mainText.initialize(_t,sysDataRequested,psd.my_un);
    }
  }
  PublicFile::PublicFile(){
    initialized=0;
  }
  int PublicFile::initializeNewFile(const std::string & _extFileCode, const std::string & _fE, const std::string & iUID, const std::string &additionalData){
    fileExtension=_fE;
    long pos=0;
    std::pair<std::string,int> allD=SF::extract(additionalData,pos,fDatTB,fDatTE);
    fileDataType="unknown";
    if(allD.second==1){
      fileDataType=allD.first;
    }
    if(fileExtension=="findOutFromDataType"){
      if(fileDataType=="image/jpeg"){
        fileExtension="jpg";
      }
      else{
        if(fileDataType=="application/pdf"){
          fileExtension="pdf";
        }
        else{
          if(fileDataType=="image/gif"){
            fileExtension="gif";
          }
          else{
            if(fileDataType=="image/png"){
              fileExtension="png";
            }
            else{
              return 0;
            }
          }
        }
      }
    }
    m.createFile(iUID);
    ivn=m.getInternalNumberFromInternalId();
    long sz=ivn.size();
    if(sz==1){
      long num=ivn[0];
      WUD::User wu;
      std::string u_Name="root";
      if(wu.setFromInternalId(iUID)==1){
        u_Name=wu.getUsername();
      }
      externalFileCode=m.getExternalCodeFromInternalNumber();
      filePath=m.clearThePublicPathAndDetermineTheFolder(MWII::GL_WI.getWSURL(),"",DD::GL_DBS.getPublicStorage(),DD::GL_DBS.getPublicStorage(),num,DD::GL_DBS.getNumSubFolders(),DD::GL_DBS.getNumFilesInFolder());
      fileName=SF::combineTwoWords(externalFileCode,RNDF::genRandCode(6));
      fileInfoData=fcB+externalFileCode+fcE;
      fileInfoData+=fnmB+fileName+fnmE;
      fileInfoData+=fPB+filePath+fPE;
      fileInfoData+=fnB+std::to_string(num)+fnE;
      fileInfoData+=fEB+fileExtension+fEE;
      fileInfoData+=fUCB+u_Name+fUCE;
      TMF::Timer tm;
      std::string tNow=tm.timeString();
      fileInfoData+= fTB+tNow+fTE;
      fileInfoData+=additionalData;
      m.setTextData(fileInfoData);
      m.putInDB();
      initialized=1;
      return 1;
    }
    return 0;
  }
  int PublicFile::initializeExistingFile(const std::string & _extFileCode){
    int scc=m.setFromExternalCode(_extFileCode);
    if(scc==1){
      ivn=m.getInternalNumberFromInternalId();
      long sz=ivn.size();
      if(sz==1){
        long num=ivn[0];
        externalFileCode=_extFileCode;
        filePath=m.clearThePublicPathAndDetermineTheFolder(MWII::GL_WI.getWSURL(),"",DD::GL_DBS.getPublicStorage(),DD::GL_DBS.getPublicStorage(),num,DD::GL_DBS.getNumSubFolders(),DD::GL_DBS.getNumFilesInFolder());
        fileInfoData=m.getTextData();
        long pos=0;
        std::pair<std::string,int> allD=SF::extract(fileInfoData,pos,fEB,fEE);
        fileExtension="na";
        if(allD.second==1){
          fileExtension=allD.first;
        }
        pos=0;
        allD=SF::extract(fileInfoData,pos,fDatTB,fDatTE);
        fileDataType="unknown";
        if(allD.second==1){
          fileDataType=allD.first;
        }
        pos=0;
        allD=SF::extract(fileInfoData,pos,fnmB,fnmE);
        fileName=externalFileCode;
        if(allD.second==1){
          fileName=allD.first;
        }
        initialized=1;
        return 1;
      }
    }
    return 0;
  }
  int PublicFile::init(const std::string & _extFileCode, const std::string & _fE,  const std::string & userName, const std::string &additionalData){
    WUD::User u;
    if(userName!="!noUser!"){
      int uSet=u.setFromUsername(userName);
      if(uSet==1){
        std::string iUID=u.getInternalId();
        if(_extFileCode=="!*new!*"){
          return initializeNewFile(_extFileCode,_fE,iUID,additionalData);
        }
        else{
          return initializeExistingFile(_extFileCode);
        }
      }
      return 0;
    }
    // The user is "!noUser!" . We need to set existing file
    return initializeExistingFile(_extFileCode);
  }
  std::string PublicFile::getFileInfoData() const{   return fileInfoData; }
  std::string PublicFile::getFilePath() const{ return filePath; }
  std::string PublicFile::getFileName() const{
    return MWII::FILE_PREFIX+fileName+"."+fileExtension;
  }
  long PublicFile::isInitialized() const{ return initialized; }
   int SessionInformation::allowedToDeleteFile(const PublicFile & _p) const{
    //WARNING:
    // returns 1 only for root or if the method is called while processing the ResponseReceiver
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")||(psd.respRecFlag=="accepted")){
      return 1;
    }
    return 0;
  }
  std::string PublicFile::deleteFile(){
    if(initialized!=1){
      return "!failed!: File not initialized";
    }
    std::string completeFilePath=getFilePath()+"/"+getFileName();
    m.deleteRecord();
    IOF::sys_deleteFile(completeFilePath);
    initialized=0;
    return "!success!";
  }
  std::string SessionInformation::deleteFile(PublicFile & _p){
    if(allowedToDeleteFile(_p)!=1){
      return "!failed!: Not allowed to delete";
    }
    return _p.deleteFile();
  }
  std::string SessionInformation::deleteSingleFile(const std::string &_extFileCode){
    PublicFile pf;
    pf.init(_extFileCode);
    return deleteFile(pf);
  }
  std::string SessionInformation::deleteFile(const std::string &_extFileCode){
    // Step 1: Check whether _extFileCode is a scraped website data with a lot of files to delete
    std::vector<std::string> manyFiles=PASF::getItemsFromScrapedPage(_extFileCode,"\n","\t");
    long sz=manyFiles.size();
    if(sz>0){
      // Step 2.1: There are multiple requests
      //           We delete each file
      for(long i=0;i<sz;++i){
        deleteSingleFile(manyFiles[i]);
      }
      return "!success!";
    }
    // Step 2.2: There is a single request
    //           We delete the single file
    return deleteSingleFile(_extFileCode);
  } 
  int SessionInformation::allowedToModifyFile(const std::string &_userName, const std::string & _extCode) const{
    if(_userName=="root"){
      return 1;
    }
    return allowedToCreateText();
  }
  int SessionInformation::allowedToUploadNewFile(const std::string &_userName) const{
    if(_userName=="root"){
      return 1;
    }
    return allowedToCreateText();
  }
  int SessionInformation::allowedToDeleteCloneForGuest(const std::string &guestUname) const{
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes") || (guestUname==psd.my_un)){
      return 1;
    }
    return 0;
  }
  int SessionInformation::allowedToCreateCloneForGuest(const std::string &guestUname) const{
    if(DD::GL_IND_ALLOW_WEBSITE_CLONES!=1){
      return 0;
    }
    if(allowedToDeleteCloneForGuest(guestUname)==0){
      return 0;
    }
    WUD::User w;
    int userExists=w.setFromUsername(guestUname);
    if(userExists==0){
      return 0;
    }
    if(w.isAllowedToCloneWebsite()=="yes"){
      return 1;
    }
    return 0;
  }
  std::string SessionInformation::deleteCloneForGuest(const std::string & guestUName){
    if(allowedToDeleteCloneForGuest(guestUName)==0){
      return "notAllowed";
    }
    GCSI::deleteClonedWebsite(guestUName);
    return "deleted";
  }
  std::string SessionInformation::createCloneForGuest(const std::string & guestUName){
    if(allowedToCreateCloneForGuest(guestUName)==0){
      return "notAllowed";
    }
    WUD::User w;
    int userExists=w.setFromUsername(guestUName);
    if(userExists==0){
      return "notAllowed";
    }
    GCSI::createClonedWebsite(guestUName,w.getEncPassword());
    return "created";
  }
  int SessionInformation::createOrDeleteWebsiteOrChangeRequestedPage(){
    if(psd.isRoot=="no"){
      if(psd.my_un=="visitor"){
        psd.pageRequested="pageDoesNotExist";
        return 0;
      }
      if(psd.pageRequested=="deleteWebsite"){
        deleteCloneForGuest(psd.my_un);
        return 1;
      }
      if(psd.pageRequested=="createWebsite"){
        std::string res=createCloneForGuest(psd.my_un);
        if(res=="notAllowed"){
          psd.pageRequested="createWebsiteFailed";
        }
      }
    }
    return 0;
  }
  int SessionInformation::allowedToCreateText() const{
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    return 0;
  }
  int SessionInformation::allowedToCreateRespRec() const{
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    return 0;
  }
  int SessionInformation::allowedToDeleteText(const std::string & _tN) const{
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    return 0;
  }
  int SessionInformation::allowedToModifyText(const std::string & _tN) const{
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    MPTI::MainText tToCheck(psd.pageRequested,"no1117",psd.my_un);
    return tToCheck.allowedToModifyText(psd,"","",psd.my_un);
  }
  int SessionInformation::allowedToModifyRespRec(const std::string & _tN,const std::string & _fSubmission) const{
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    if(_fSubmission=="yes"){
      return 1;
    }
    return 0;
  }
  std::string SessionInformation::createText(const std::string & _tName, const std::string & _tData, const std::string & _readPermitOverwrite, const std::string & _typeOverwrite){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToCreateText()==0){
      return "!failed!: Not allowed";
    }
    TMD::MText sf;
    int textExists=sf.setFromTextName(_tName);
    if(textExists==1){
      return "!failed!: Text name already exists";
    }
    int succ=sf.createText(_tName);
    if(succ==0){
      return "!failed!: Something may be bad with the database. The text name did not exist when checked but could not create text.";
    }
    psd.pageRequested=sf.getTextName();
    std::string textData=TDI::prepareTextForTextTableRaw(psd,_tData,"!noOldData!");
    std::map<std::string,std::string> replMap;
    if(_readPermitOverwrite!="!*!"){
      replMap["_permission__name_read_/name__userOrGroup_everyone_/userOrGroup__/permission_"]="_permission__name_read_/name__userOrGroup_"+_readPermitOverwrite+"_/userOrGroup__/permission_";
    }
    if(_typeOverwrite!="!*!"){
      replMap["_documentType!!_regularText_/documentType!!_"]="_documentType!!_"+_typeOverwrite+"_/documentType!!_";
    }
    if(replMap.size()>0){
      textData=MFRF::findAndReplace(textData,replMap);
    }
    sf.setTextData(textData);
    sf.putInDB();
    return "!success!: "+sf.getTextName();
  }
  std::string SessionInformation::modifyText(const std::string & _tName, const std::string & _tData){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToModifyText(_tName)==0){
      return "!failed!: Not allowed";
    }
    TMD::MText sf;
    int textExists=sf.setFromTextName(_tName);
    if(textExists==0){
      return "!failed!: Text name does not exist";
    }
    std::string oldData=sf.getTextData();
    psd.recoveryOperationNames.push("mainText");
    psd.recoveryOperationCommands.push(BMD::recoveryTextWithCustomData(_tName,oldData,"modifyText"));
    sf.setTextData(TDI::prepareTextForTextTableRaw(psd,_tData,oldData));
    sf.putInDB();
    return "!success!";
  }
  std::string SessionInformation::deleteText(const std::string & _tName){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToDeleteText(_tName)==0){
      return "!failed!: Not allowed";
    }
    TMD::MText sf;
    int textExists=sf.setFromTextName(_tName);
    if(textExists==0){
      return "!failed!: Text name does not exist";
    }
    int succ=sf.deleteRecord();
    if(succ==0){
      return "!failed!: Something may be bad with the database. The text name did exist when checked but could not delete.";
    }
    return "!success!";
  }
  int SessionInformation::allowedToCreateMessage(const std::string & idOfCollector) const{
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    MEI::MElement mEl;
    if(MEI::initMElementFromCode(mEl,idOfCollector)=="success"){
      return mEl.allowedToCreateMessage(psd.my_un);
    }
    return 0;
  }
  int SessionInformation::allowedToModifyMessage(const std::string & mId, const std::string & idOfCollector) const{
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    MEI::MElement mEl;
    if(MEI::initMElementFromCode(mEl,mId)=="success"){
      return mEl.allowedToModifyMessage(psd.my_un);
    }
    return 0;
  }
  int SessionInformation::allowedToCreateCouas(const std::string & idOfCollector) const{
      //WARNING: not implemented properly yet: permission checking is too restrictive
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    return 0;
  }
  int SessionInformation::allowedToModifyCouas(const std::string & couasId, const std::string & idOfCollector) const{
      //WARNING: not implemented properly yet: permission checking is too restrictive
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    return 0;
  }
  int SessionInformation::allowedToCreateCert(const std::string & idOfCollector) const{
      //WARNING: not implemented properly yet: permission checking is too restrictive
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    return 0;
  }
  int SessionInformation::allowedToModifyCert(const std::string & couasId) const{
      //WARNING: not implemented properly yet: permission checking is too restrictive
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    return 0;
  }
  std::string SessionInformation::createMessage(const std::string & _uName, const std::string & _mData, const std::string & _collectorId){
    if(allowedToCreateMessage(_collectorId)==0){
      return "!failed!: Not allowed";
    }
    if(psd.my_un=="visitor"){
      return "!failed!: Visitors cannot create messages";
    }
    MD::Message bm;
    std::string uNM=psd.my_un;
    std::string uFN=psd.myFirstName;
    std::string uLN=psd.myLastName;
    std::string nSt="";
    std::string intUserId=(psd.myWU).getInternalId();;
    if((_uName!="")&&(_uName!="_!myself!_")){
      WUD::User w;
      int userExists=w.setFromUsername(_uName);
      if(userExists==0){
        return "!failed!: Username does not exist";
      }
      uNM=_uName;
      uFN=w.getFirstName();
      uLN=w.getLastName();
      intUserId=w.getInternalId();
    }
    int succ=bm.createMessage(intUserId);
    if(succ==0){
      return "!failed!: Something may be bad with the database. The userId did exist when checked but could not create message.";
    }
    std::string navSt,collT;
    bm.addToCollection(_collectorId,navSt,collT);
    std::string mDataProcessed="";
    mDataProcessed+="_created_";
    TMF::Timer tm;
    mDataProcessed+= tm.timeString();
    mDataProcessed+="_/created_";
    mDataProcessed+="_user_"+uNM+"_/user_";
    mDataProcessed+="_firstName_"+uFN+"_/firstName_";
    mDataProcessed+="_lastName_"+uLN+"_/lastName_";
    std::vector<std::string> navSeq=SF::stringToVector(navSt,"_n_","_/n_");
    std::vector<std::string> navSeqNew;
    long szns=navSeq.size();
    navSeqNew.resize(szns+1);
    for(long i=0;i<szns;++i){
      navSeqNew[i]=navSeq[i];
    }
    navSeqNew[szns]="_!code_"+_collectorId+"_/!code_ _!title_"+collT+"_/!title_";
    ++szns;
    std::string navStNew="";
    for(long i=0;i<szns;++i){
      navStNew+="_n_"+navSeqNew[i]+"_/n_";
    }
    mDataProcessed+="_navigation_"+navStNew+"_/navigation_";
    mDataProcessed+="_messText!*_";
    mDataProcessed+=_mData;
    mDataProcessed+="_/messText!*_";
    bm.setTextData(TDI::prepareTextForTextTableRaw(psd,mDataProcessed,"!noOldData!"));
    bm.putInDB();
    return bm.getExternalCodeFromInternalId();
  }
  std::string SessionInformation::createForum(const std::string & _uName, const std::string & _fOptionsData,const std::string & _collectorId, const std::string &backupRecovery){
    if(psd.isRoot!="yes"){
      return "!failed!: Not allowed";
    }
    std::vector<std::string> subForums=SF::stringToVector(_fOptionsData,LI::GL_LN.st_subForumB,LI::GL_LN.st_subForumE);
    long numSF=subForums.size();
    long pos;std::pair<std::string,int> allD;
    std::string m_permits="";
    std::string m_title="";
    pos=0;allD=SF::extract(_fOptionsData,pos,LI::GL_LN.st_createMPermitsB,LI::GL_LN.st_createMPermitsE);
    if(allD.second==1){
      m_permits=LI::GL_LN.st_createMPermitsB+allD.first+LI::GL_LN.st_createMPermitsE;
    }
    pos=0;allD=SF::extract(_fOptionsData,pos,LI::GL_LN.st_sepTitleB,LI::GL_LN.st_sepTitleE);
    if(allD.second==1){
      m_title=allD.first;
    }
    std::string mainTopicText="";
    mainTopicText+= LI::GL_LN.st_sepTitleB+m_title+LI::GL_LN.st_sepTitleE;
    mainTopicText+="\n_mType!*_tColl_/mType!*_\n";
    std::string mainCode=createMessage(_uName,mainTopicText,_collectorId);
    if((mainCode.size()>1) &&(mainCode[0]=='!')&&(mainCode[1]=='f')){
      return "!failed!";
    }
    if(backupRecovery!=""){
      psd.recoveryOperationNames.push("courseCreation");
      psd.recoveryOperationCommands.push(BMD::deleteCouasOrMessage("deleteMessage",mainCode,"notFound"));
    }
    std::string messCode;
    for(long i=0;i<numSF;++i){
      mainTopicText=LI::GL_LN.st_sepTitleB+subForums[i]+LI::GL_LN.st_sepTitleE;
      mainTopicText+="\n_mType!*_mColl_/mType!*_\n";
      mainTopicText+=m_permits;
      messCode=createMessage(_uName,mainTopicText,mainCode);
      if(backupRecovery!=""){
        psd.recoveryOperationNames.push("courseCreation");
        psd.recoveryOperationCommands.push(BMD::deleteCouasOrMessage("deleteMessage",messCode,mainCode));
      }
    }
    return "!success!: _mainTopicCode_"+mainCode+"_/mainTopicCode_";
  }
  std::string SessionInformation::createSupportingPages(const std::map<std::string,std::string> & templM, const std::map<std::string,std::string> & varM, const std::map<std::string,std::string> & specialInstructions){
    std::map<std::string,std::string>::const_iterator it, itE,itV,itVE,itS,itSE;
    itE=templM.end();
    itVE=varM.end();
    itSE=specialInstructions.end();
    std::string varCode="varNotFound";
    std::string docName="docNotFound";
    itV=varM.find("docName");
    if(itV!=itVE){
      docName=itV->second;
    }
    itV=varM.find("variablesExtension");
    if(itV!=itVE){
      varCode=itV->second;
    }
    std::string supportingPageLabel,supportingPageContent,supportingPageName,supportingPageExtension;
    int skipInd;int rootInd;
    it=templM.begin();
    while(it!=itE){
      supportingPageLabel=it->first;supportingPageContent=it->second;
      supportingPageContent=SF::findAndReplace(supportingPageContent,"*variables*",docName+varCode);
      itV=varM.find(supportingPageLabel+"Extension");
      if(itV!=itVE){
        supportingPageExtension=itV->second;
        supportingPageName=docName+supportingPageExtension;
        itS=specialInstructions.find(supportingPageLabel);
        skipInd=0;rootInd=0;
        if(itS!=itSE){
          if(itS->second=="skip"){
            skipInd=1;
          }
          if(itS->second=="root"){
            rootInd=1;
          }
        }
        if(skipInd==0){
          std::string crSucc;
          if(rootInd==0){
            crSucc=createText(supportingPageName,supportingPageContent);
          }
          else{
            crSucc=createText(supportingPageName,supportingPageContent,"root");
          }
          if((crSucc.length()>3) && (crSucc[0]=='!')&&(crSucc[1]=='s')&&(crSucc[2]=='u')){
            psd.recoveryOperationNames.push("courseCreation");
            psd.recoveryOperationCommands.push(BMD::deleteCommand("deleteText",supportingPageName));
          }
        }
      }
      ++it;
    }
    return "";
  }
  std::string SessionInformation::createInitialExams(const std::map<std::string,std::string> & eM, const std::string &docName){
    std::map<std::string,std::string>::const_iterator it, itE;
    itE=eM.end();
    it=eM.begin();
    std::string extension,exName,dataToGenerateExam;
    while(it!=itE){
      extension=it->first;
      exName=docName+extension;
      dataToGenerateExam=it->second;
      generateExam(exName,dataToGenerateExam);
      ++it;
    }
    return "";
  }
  std::string SessionInformation::createStandardCourse(const std::string & _fOptionsData, const std::string & _forFutureUse){
    if(psd.isRoot!="yes"){
      return "!failed!: Not allowed";
    }
    long pos; std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(_fOptionsData,pos,"_variablesMain_","_/variablesMain_");
    if(allD.second==0){
      return "!failed!: no variables";
    }
    std::string vMain=allD.first;
    pos=0;allD=SF::extract(_fOptionsData,pos,"_templatesMain_","_/templatesMain_");
    if(allD.second==0){
      return "!failed!: no templates";
    }
    std::string templMain=allD.first;
    std::map<std::string,std::string> specInstr;
    pos=0;allD=SF::extract(_fOptionsData,pos,"_specialInstructions_","_/specialInstructions_");
    if(allD.second==1){
      specInstr=SF::stringToMap(allD.first,LI::GL_LN.st_sep_kB,LI::GL_LN.st_sep_kE,LI::GL_LN.st_sep_vB,LI::GL_LN.st_sep_vE);
    }
    std::map<std::string,std::string> examsMap;
    pos=0;allD=SF::extract(_fOptionsData,pos,"_genExams_","_/genExams_");
    if(allD.second==1){
      examsMap=SF::stringToMap(allD.first,LI::GL_LN.st_sep_keB,LI::GL_LN.st_sep_keE,LI::GL_LN.st_sep_veB,LI::GL_LN.st_sep_veE);
    }
    std::map<std::string,std::string> templatesMap=SF::stringToMap(templMain,LI::GL_LN.st_sep_tnB,LI::GL_LN.st_sep_tnE,LI::GL_LN.st_sep_tcB,LI::GL_LN.st_sep_tcE);
    std::map<std::string,std::string>::const_iterator it,itE,itV,itVE;
    itE=templatesMap.end();
    it=templatesMap.find("gradingForCourse");
    if(it==itE){
      return "!failed!: no grading creation template";
    }
    std::string gCreationTemplate=it->second;
    std::map<std::string,std::string> varMap;
    varMap=SF::stringToMap(vMain,LI::GL_LN.st_sep_vrB,LI::GL_LN.st_sep_vrE,LI::GL_LN.st_sep_vlB,LI::GL_LN.st_sep_vlE);
    itVE=varMap.end();
    itV=varMap.find("courseNumber");
    std::string courseNumber="No Course Number:";
    if(itV!=itVE){
      courseNumber=itV->second;
    }
    itV=varMap.find("semester");
    std::string semester="";
    if(itV!=itVE){
      semester="("+itV->second+")";
    }
    gCreationTemplate=SF::replaceVariablesWithValues(gCreationTemplate,LI::GL_LN.st_sep_vrB,LI::GL_LN.st_sep_vrE,varMap);
    std::string crScc=createGradingForCourse(gCreationTemplate,"na");
    pos=0;allD=SF::extract(crScc,pos,"_mainCouas_","_/mainCouas_");
    if(allD.second==0){
      return "!failed!: gradging documents were not created properly";
    }
    std::string couasCode=allD.first;
    it=templatesMap.find("forumForCourse");
    std::string forumCode="noForum";
    if(it!=itE){
      std::string fText=it->second;
      fText=SF::findAndReplace(fText,"*messpermits*","p_"+couasCode);
      fText=SF::findAndReplace(fText,"*courseTitle*",courseNumber+" "+semester);
      pos=0;allD=SF::extract(createForum("_!myself!_",fText,"bossMessage","backupRecovery"),pos,"_mainTopicCode_","_/mainTopicCode_");
      if(allD.second==0){
        return "!failed!: a problem occured when creating the forum";
      }
      forumCode=allD.first;
    }
    it=templatesMap.find("documentMain");
    if(it==itE){
      return "!failed!: no main document";
    }
    std::string mtContent=it->second;
    itV=varMap.find("docName");
    if(itV==itVE){
      return "!failed!: no name for the main document";
    }
    std::string mainDocName=itV->second;
    std::string variablesExt="Var";
    itV=varMap.find("variablesExtension");
    if(itV!=itVE){
      variablesExt=itV->second;
    }
    mtContent=SF::findAndReplace(mtContent,"*variables*",mainDocName+variablesExt);
    std::string mainTextCrRes=createText(mainDocName, mtContent);
    if( (mainTextCrRes.length()>3) && (mainTextCrRes[0]=='!')&&(mainTextCrRes[1]=='s')&&(mainTextCrRes[2]=='u') ){
      psd.recoveryOperationNames.push("courseCreation");
      psd.recoveryOperationCommands.push(BMD::deleteCommand("deleteText",mainDocName));
    }
    specInstr["variables"]="skip";
    createSupportingPages(templatesMap,varMap,specInstr);
    createInitialExams(examsMap,mainDocName);
    vMain=SF::findAndReplace(vMain,"***REPLACE***WITH***MAIN_GRADING_DOC***",couasCode);
    vMain=SF::findAndReplace(vMain,"***REPLACE***WITH***MAIN_FORUM_PAGE***",forumCode);
    mainTextCrRes=createText(mainDocName+variablesExt,vMain,"!*!","systemText");
    if( (mainTextCrRes.length()>3) && (mainTextCrRes[0]=='!')&&(mainTextCrRes[1]=='s')&&(mainTextCrRes[2]=='u') ){
      psd.recoveryOperationNames.push("courseCreation");
      psd.recoveryOperationCommands.push(BMD::deleteCommand("deleteText",mainDocName+variablesExt));
    }
    psd.createStandardCourseMainDocName=mainDocName;
    psd.createStandardCourseSuccess="yes";
    return "!success!";
  }
  std::string SessionInformation::modifyMessage(const std::string & _messId, const std::string & _mData){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToModifyMessage(_messId,"!*!")==0){
      return "!failed!: Not allowed";
    }
    MD::Message bm;
    int messageExists=bm.setFromExternalCode(_messId);
    if(messageExists==0){
      return "!failed!: The message does not exist";
    }
    std::pair<std::string,int> allD;
    long pos;
    int indicatorAdvancedUser=0;
    pos=0;allD=SF::extract(_mData,pos,s_tDataB,s_tDataE);
    if(allD.second==1){
      pos=0;allD=SF::extract(_mData,pos,s_sysDataB,s_sysDataE);
      if(allD.second==1){
        indicatorAdvancedUser=1;
      }
    }
    std::string oldData=bm.getTextData();
    std::string mDataProcessed=_mData,uNM=psd.my_un,uFN=psd.myFirstName,uLN=psd.myLastName,nSt="";
    if(indicatorAdvancedUser==0){
      mDataProcessed="";
      std::string oldDT="";
      std::string createdTM;
      TMF::Timer tm;
      createdTM=tm.timeString();
      pos=0;allD=SF::extract(oldData,pos,s_tDataB,s_tDataE);
      if(allD.second==1){
        oldDT=allD.first;
        pos=0;allD=SF::extract(oldDT,pos,"_created_","_/created_");
        if(allD.second==1){
          createdTM=allD.first;
        }
        pos=0;allD=SF::extract(oldDT,pos,"_user_","_/user_");
        if(allD.second==1){
          uNM=allD.first;
        }
        pos=0;allD=SF::extract(oldDT,pos,"_firstName_","_/firstName_");
        if(allD.second==1){
          uFN=allD.first;
        }
        pos=0;allD=SF::extract(oldDT,pos,"_lastName_","_/lastName_");
        if(allD.second==1){
          uLN=allD.first;
        }
        pos=0;allD=SF::extract(oldDT,pos,"_navigation_","_/navigation_");
        if(allD.second==1){
          nSt=allD.first;
        }
      }
      mDataProcessed+="_created_";
      mDataProcessed+= createdTM;
      mDataProcessed+="_/created_";
      mDataProcessed+="_user_"+uNM+"_/user_";
      mDataProcessed+="_firstName_"+uFN+"_/firstName_";
      mDataProcessed+="_lastName_"+uLN+"_/lastName_";
      mDataProcessed+="_navigation_"+nSt+"_/navigation_";
      mDataProcessed+="_messText!*_";
      mDataProcessed+=_mData;
      mDataProcessed+="_/messText!*_";
    }
    else{
      mDataProcessed=_mData;
    }
    bm.setTextData(TDI::prepareTextForTextTableRaw(psd,mDataProcessed,oldData));
    bm.putInDB();
    return "!success!";
  }
  std::string SessionInformation::createCouas(const std::string & _uName, const std::string & _couasData, const std::string & _collectorId){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToCreateCouas(_collectorId)==0){
      return "!failed!: Not allowed";
    }
    if(psd.my_un=="visitor"){
      return "!failed!: Visitors cannot create courses or assignments";
    }
    CAD::CouAs myCouas;
    std::string uNM=psd.my_un;
    std::string uFN=psd.myFirstName;
    std::string uLN=psd.myLastName;
    std::string nSt="";
    std::string intUserId=(psd.myWU).getInternalId();;
    if((_uName!="")&&(_uName!="_!myself!_")){
      WUD::User w;
      int userExists=w.setFromUsername(_uName);
      if(userExists==0){
        return "!failed!: Username does not exist";
      }
      uNM=_uName;
      uFN=w.getFirstName();
      uLN=w.getLastName();
      intUserId=w.getInternalId();
    }
    int succ=myCouas.createCouAs(intUserId);
    if(succ==0){
      return "!failed!: Something may be bad with the database. The userId did exist when checked but could not create course / assignment.";
    }
    TMF::Timer tm;
    std::string mDataProcessed=TDI::couasTextString(tm.timeString(),uNM, uFN,uLN,_couasData);
    myCouas.setTextData(TDI::prepareTextForTextTableRaw(psd,mDataProcessed,"!noOldData!"));
    myCouas.putInDB();
    return "!success!: "+myCouas.getExternalCodeFromInternalId();
  }
  std::string SessionInformation::modifyCouas(const std::string & _couasId, const std::string & _couasData){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToModifyCouas(_couasId,"!*!")==0){
      return "!failed!: Not allowed";
    }
    return MCWCPI::modifyCouasWithoutCheckingPermissions(psd,_couasId,_couasData);
  }
  std::string SessionInformation::deleteMessage(const std::string & _messId,const std::string & _collectorId){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToModifyMessage(_messId,_collectorId)==0){
      return "!failed!: Not allowed";
    }
    MD::Message bm;
    int messageExists=bm.setFromExternalCode(_messId);
    if(messageExists==0){
      return "!failed!: Message with this ID does not exist";
    }
    std::string notImp1,notImp2;
    bm.removeFromCollection(_collectorId,notImp1,notImp2);
    int succ=bm.deleteRecord();
    if(succ==0){
      return "!failed!: Something may be bad with the database. The ID of the message was found but could not delete.";
    }
    return "!success!";
  }
  std::string SessionInformation::deleteCouas(const std::string & _couasId,const std::string & _collectorId){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToModifyCouas(_couasId,_collectorId)==0){
      return "!failed!: Not allowed";
    }
    CAD::CouAs myCouas;
    int couasExists=myCouas.setFromExternalCode(_couasId);
    if(couasExists==0){
      return "!failed!: Course or assignment with this ID does not exist";
    }
    int succ=myCouas.deleteRecord();
    if(succ==0){
      return "!failed!: Something may be bad with the database. The ID of the course or assignment was found but could not delete.";
    }
    return "!success!";
  }
  std::string SessionInformation::createCert(const std::string & _cName, const std::string & _certData){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToCreateCert(psd.my_un)==0){
      return "!failed!: Not allowed";
    }
    if(psd.my_un=="visitor"){
      return "!failed!: Visitors cannot create certificates";
    }
    return MCEWCPI::createCertNoPermissionCheck(psd,_cName,_certData);
  }
  std::string SessionInformation::modifyCert(const std::string & _certName, const std::string & _certData){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToModifyCert(_certName)==0){
      return "!failed!: Not allowed";
    }
    return MCEWCPI::modifyCertWithoutCheckingPermissionsAndCreateRecoveryCommand(psd,_certName,_certData);
  }
  std::string SessionInformation::deleteCert(const std::string & _cName){
      //WARNING: not implemented properly yet: permission checking is too restrictive
      if(allowedToModifyCert(_cName)==0){
        return "!failed!: Not allowed";
      }
      return MCEWCPI::deleteCertNoPermissionCheck(_cName);
  }
  std::string SessionInformation::createGradingForCourse(const std::string & _couasData, const std::string & _collectorId){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToCreateCouas(_collectorId)==0){
      return "!failed!: Not allowed";
    }
    if(psd.my_un=="visitor"){
      return "!failed!: Visitors cannot create courses or assignments";
    }
    std::pair<std::map<std::string,std::string>, std::map<std::string,std::string> > bijectionCouasRR;
    return CMTI::createGradingForCourse(psd,_couasData,psd.recoveryOperationNames,psd.recoveryOperationCommands,bijectionCouasRR);
  }
  std::string SessionInformation::createRespRec(const std::string & _tName, const std::string & _tData){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToCreateRespRec()==0){
      return "!failed!: Not allowed";
    }
    RMD::Response sf;
    int textExists=sf.setFromTextName(_tName);
    if(textExists==1){
      return "!failed!: Text name already exists";
    }
    int succ=sf.createText(_tName);
    if(succ==0){
      return "!failed!: Something may be bad with the database. The text name did not exist when checked but could not create text.";
    }
    respRecRequested=sf.getTextName();
    sf.setTextData(TDI::prepareTextForTextTableRaw(psd,_tData,"!noOldData!"));
    sf.putInDB();
    return "!success!: "+sf.getTextName();
  }
  std::string SessionInformation::modifyRespRec(const std::string & _tName, const std::string & _tData, const std::string & _fSubmission){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToModifyRespRec(_tName,_fSubmission)==0){
      return "!failed!: Not allowed";
    }
    RMD::Response sf;
    int textExists=sf.setFromTextName(_tName);
    if(textExists==0){
      return "!failed!: Text name does not exist";
    }
    std::string oldData=sf.getTextData();
    sf.setTextData(TDI::prepareTextForTextTableRaw(psd,_tData,oldData));
    sf.putInDB();
    respRecRequested=_tName;
    return "!success!";
  }
  ExamAttributes SessionInformation::attributesFromRespRec(const std::string &d,const std::string &docName) const{
    ExamAttributes fR;
    fR.textForRespReceiver=d;
    std::vector<std::string> vectProblems=SF::stringToVector(d,"_in*|_","_/in*|_");
    long numProblems=vectProblems.size();
    (fR.possibleVersions).resize(numProblems);
    (fR.problemLabels).resize(numProblems);
    long p;std::pair<std::string,int> allD; long pos;
    fR.numFilesAllowed=0;
    pos=0;allD=SF::extract(d,pos,"_nf*|_","_/nf*|_");
    if(allD.second==1){
      fR.numFilesAllowed=BF::stringToInteger(allD.first);
      if((fR.numFilesAllowed<0)||(fR.numFilesAllowed>100)){
        fR.numFilesAllowed=0;
      }
    }
    fR.deadlineText="notYet";
    pos=0;allD=SF::extract(d,pos,"_dl*|_","_/dl*|_");
    if(allD.second==1){
      fR.deadlineText=allD.first;
    }
    fR.deadlineText="_dl*|_"+fR.deadlineText+"_/dl*|_";
    fR.msDoc=docName;
    pos=0;allD=SF::extract(d,pos,"_ms*|_","_/ms*|_");
    if(allD.second==1){
      fR.msDoc=allD.first;
    }
    for(long i=0;i<numProblems;++i){
      (fR.problemLabels)[i]="notFound";
      (fR.possibleVersions)[i].resize(0);
      (fR.possibleVersions)[i]=TWDVF::identifyVersions(vectProblems[i]);
      pos=0;allD=SF::extract(vectProblems[i],pos,"_lb*|_","_/lb*|_");
      if(allD.second==1){
        (fR.problemLabels)[i]=allD.first;
      }
    }
    return fR;
  }
  int SessionInformation::checkWhetherSingleProblemExists(const std::string & pText, TMD::MText& tmp) const{
    long pos,pos2; std::pair<std::string, int> allD,allD2;
    pos=0;allD=SF::extract(pText,pos,"_p*|_","_/p*|_");
    if(allD.second==1){
      pos2=0;allD2=SF::extract(allD.first,pos2,"_tx*|_","_/tx*|_");
      if(allD2.second==0){
        if(tmp.setFromTextName(allD.first)==0){return 0;}
      }
    }
    return 1;
  }
  long SessionInformation::checkWhetherAllProblemsExist(const std::vector<std::string> &vectProblems) const{
    // returns 1 if all problems exist
    // returns 0 if problem 0 does not exist
    // returns -1 if problem 1 does not exist but problem 0 does exist
    // returns -i if problem i does not exist but problems 0, 1, 2, ..., (i-1), do exist
    long i=0; TMD::MText tmp;
    while(i<vectProblems.size()){
      if(checkWhetherSingleProblemExists(vectProblems[i],tmp)==0){
        return -i;
      }
      ++i;
    }
    return 1;
  }
  ExamAttributes SessionInformation::newExamFromTemplate(const std::string &d,const std::pair<std::vector<std::string>,std::string>& pairProblemsCert, const std::string &docName){
    ExamAttributes fR;
    fR.textForRespReceiver="";
    std::vector<std::string> vectProblems;
    std::string certificateDocument;
    vectProblems=pairProblemsCert.first;
    certificateDocument=pairProblemsCert.second;
    long numProblems=vectProblems.size();
    long certDocExists=0;
    if(certificateDocument!=""){
      certDocExists=1;
    }
    (fR.possibleVersions).resize(numProblems+certDocExists);
    (fR.problemLabels).resize(numProblems+certDocExists);
    long p;std::pair<std::string,int> allD; long pos;
    pos=0;allD=SF::extract(d,pos,"_examOptions*|_","_/examOptions*|_");
    if(allD.second==1){
      fR.textForRespReceiver+=allD.first;
    }
    else{
      fR.textForRespReceiver+=MWII::GL_WI.getDefaultWebText("defaultExamOptions");
    }
    fR.numFilesAllowed=0;
    pos=0;allD=SF::extract(d,pos,"_nf*|_","_/nf*|_");
    if(allD.second==1){
      fR.numFilesAllowed=BF::stringToInteger(allD.first);
      if((fR.numFilesAllowed<0)||(fR.numFilesAllowed>100)){
        fR.numFilesAllowed=0;
      }
    }
    fR.deadlineText="notYet";
    pos=0;allD=SF::extract(d,pos,"_dl*|_","_/dl*|_");
    if(allD.second==1){
      fR.deadlineText=allD.first;
    }
    fR.deadlineText="_dl*|_"+fR.deadlineText+"_/dl*|_";
    fR.msDoc=docName;
    pos=0;allD=SF::extract(d,pos,"_ms*|_","_/ms*|_");
    if(allD.second==1){
      fR.msDoc=allD.first;
    }
    else{
      TMD::MText tmDefaultPermission;
      int tm_e;
      tm_e=tmDefaultPermission.setFromTextName("defaultTemplateForTestPermissions");
      if(tm_e==1){
        std::string pText=tmDefaultPermission.getTextData();
        pos=0;allD=SF::extract(pText,pos,s_tDataB,s_tDataE);
        if(allD.second==1){
          std::string permitsText=allD.first;
          std::string msDAttempt=docName+"p";
          long modifierToAdd=0;
          RMD::Response rmPermits;
          int rmpNameExists=rmPermits.setFromTextName(msDAttempt);
          while(rmpNameExists==1){
            ++modifierToAdd;
            msDAttempt=docName+"p"+std::to_string(modifierToAdd);
            rmpNameExists= rmPermits.setFromTextName(msDAttempt);
          }
          if( rmPermits.createText(msDAttempt)==1){
            fR.msDoc=msDAttempt;
            rmPermits.setTextData(TDI::prepareTextForTextTableRaw(psd,permitsText,"!noOldData!"));
            rmPermits.putInDB();
            psd.recoveryOperationNames.push("assignmentCreation");
            psd.recoveryOperationCommands.push(BMD::deleteCommand("deleteResponseReceiver",msDAttempt));
          }
        }
      }
    }
    fR.textForRespReceiver+="_ms*|_"+fR.msDoc+"_/ms*|_";
    TMD::MText tmp;
    for(long i=0;i<numProblems;++i){
      CERTCLI::updateExamAttributesWithProblemOrCertInformation(fR,tmp,vectProblems[i],i,"");
    }
    if(certificateDocument!=""){
      CERTCLI::updateExamAttributesWithProblemOrCertInformation(fR,tmp,certificateDocument,numProblems,"C");
    }
    return fR;
  }
  std::string SessionInformation::enrollExistingStudentsToExam(const ExamAttributes & ea, const std::string & d,const std::string &docName){
    std::string cntrName="examCounter";
    long studentRandomizer=RNDF::randNum(10000);
    long pos;std::pair<std::string,int> allD;
    long filesToAllowFromStudentEnrollmentDoc=0;
    pos=0;allD=SF::extract(d,pos,"_nf*|_","_/nf*|_");
    if(allD.second==1){
      filesToAllowFromStudentEnrollmentDoc=BF::stringToInteger(allD.first);
    }
    long numFA=ea.numFilesAllowed;
    if(filesToAllowFromStudentEnrollmentDoc>numFA){
      numFA=filesToAllowFromStudentEnrollmentDoc;
    }
    if(numFA>100){
      numFA=10;
    }
    std::string numFASt=std::to_string(numFA);
    std::string fR;
    fR=ea.textForRespReceiver;
    std::vector<std::string> vectStudents;
    std::pair<std::string,int> allDStudents;
    long posStudents;
    posStudents=0;allDStudents=SF::extract(d,posStudents,"_stLocation*|_","_/stLocation*|_");
    if(allDStudents.second==0){
      vectStudents=SF::stringToVector(d,"_st*|_","_/st*|_");
    }
    else{
      TMD::MText tmWithStudents;
      int tm_e;
      tm_e=tmWithStudents.setFromTextName(allDStudents.first);
      if(tm_e==1){
        std::string stDText=tmWithStudents.getTextData();
        long posStData;std::pair<std::string,int> allDStData;
        posStData=0;allDStData=SF::extract(stDText,posStData,s_tDataB,s_tDataE);
        if(allDStData.second==1){
          vectStudents=SF::stringToVector(allDStData.first,"_st*|_","_/st*|_");
        }
      }
    }
    std::vector<std::string> vectStData;
    long numStudents=vectStudents.size();
    int rrNameIsFree;
    std::string attempt;
    std::string gradeText,respText; long szSD;
    std::vector<std::string> tempStV;
    std::string sep_B,sep_E;
    for(long i=0;i<numStudents;++i){
      ++studentRandomizer;
      vectStData=SF::stringToVector(vectStudents[i],"_n*|_","_/n*|_");
      szSD=vectStData.size();
      if(szSD<5){
        tempStV.resize(5);for(long i=0;i<tempStV.size();++i){tempStV[i]="";}
        for(long k=0;k<vectStData.size();++k){
          tempStV[k]=vectStData[k];
        }
        if(tempStV[2].length()<3){
          tempStV[2]=tempStV[0];
        }
        vectStData=tempStV;
      }
      RMD::Response exResp,exGrade;
      szSD=vectStData.size();
      rrNameIsFree=0;
      while(rrNameIsFree==0){
        CD::Counter cId(cntrName,cntrEIdI0,cntrEIdI1,cntrEIdIZ);
        cId.increase();
        attempt=SF::combineTwoWords(cId.getCodeWord(),RNDF::genRandCode(6));
        if( exResp.setFromTextName(attempt) == 0){
          rrNameIsFree=exResp.createText(attempt);
        }
      }
      rrNameIsFree=0;std::string numFilesText;
      while(rrNameIsFree==0){
        CD::Counter cId(cntrName,cntrEIdI0,cntrEIdI1,cntrEIdIZ);
        cId.increase();
        attempt=SF::combineTwoWords(cId.getCodeWord(),RNDF::genRandCode(6));
        if( exGrade.setFromTextName(attempt) == 0){
          //Step 1: Create the grading document
          rrNameIsFree=exGrade.createText(attempt);
          gradeText="This is a grading document. \n";
          gradeText+="The grader will edit the document after the response to test is received.\n";
          gradeText+="_rsp*|_"+exResp.getTextName()+"_/rsp*|_ \n";
          gradeText+="Formulations and solutions: _f*|_"+docName+ "_/f*|_\n";
          gradeText+="Master status: _ms*|_"+ea.msDoc+"_/ms*|_\n";
          exGrade.setTextData(TDI::prepareTextForTextTableRaw(psd,gradeText,"!noOldData!"));
          exGrade.putInDB();
          //Step 2: Create the testing document
          respText="This is a response to test.\nThe solver will solve the test and answers will be listed below.\n";
          respText+="_grd*|_"+exGrade.getTextName()+"_/grd*|_\n";
          respText+=ea.deadlineText+"\n";
          respText+="\nFormulations and solutions: _f*|_"+docName+"_/f*|_\n";
          respText+="Master status: _ms*|_"+ea.msDoc+"_/ms*|_\n";
          respText+="_iI*|_"+vectStData[0]+"_/iI*|_\n";
          respText+="_email*|_"+vectStData[1]+"_/email*|_\n";
          respText+="_userName*|_"+vectStData[2]+"_/userName*|_\n";
          respText+="_otherIdData*|_"+vectStData[3]+"_/otherIdData*|_\n";
          numFilesText="_nf*|_"+numFASt+"_/nf*|_";
          respText+=numFilesText+"\n";
          long numProblems=(ea.problemLabels).size();
          for(long j=0;j<numProblems;++j){
            sep_B="v"+(ea.problemLabels)[j]+"*|_";
            sep_E="_/"+sep_B;
            sep_B="_"+sep_B;
            respText+=sep_B+std::to_string((ea.possibleVersions)[j][studentRandomizer % (ea.possibleVersions)[j].size() ])+sep_E+ "\n";
          }
          exResp.setTextData(TDI::prepareTextForTextTableRaw(psd,respText,"!noOldData!"));
          exResp.putInDB();
          // Step 3: Create the reference to Student in the main document
          fR+="_st*|_\n_n*|_"+exResp.getTextName()+"_/n*|_\n_n*|_";
          fR+=exGrade.getTextName()+"_/n*|_\n";
          for(long k=0;k<szSD;++k){
            fR+="_n*|_"+vectStData[k]+"_/n*|_\n";
          }
          fR+="_/st*|_";
          // Step 4: Create deleting commands for the backup database. If the administrator realizes that the exam
          // is incorrectly created, then the administrator can simply reverse the creation by going to backup database
          psd.recoveryOperationNames.push("assignmentCreation");psd.recoveryOperationNames.push("assignmentCreation");
          psd.recoveryOperationCommands.push(BMD::deleteCommand("deleteResponseReceiver",exResp.getTextName()));
          psd.recoveryOperationCommands.push(BMD::deleteCommand("deleteResponseReceiver",exGrade.getTextName()));
        }
      }
    }
    return fR;
  }
  std::string SessionInformation::genExamTemplate(const std::string &d, const std::pair<std::vector<std::string>,std::string>& pairProblemsCert, const std::string &docName){
    ExamAttributes ea=newExamFromTemplate(d,pairProblemsCert,docName);
    return enrollExistingStudentsToExam(ea,d,docName);
  }
  std::string SessionInformation::updateExamDocument(const std::string & oldT, const std::string &d,const std::string &docName){
    ExamAttributes ea=attributesFromRespRec(oldT,docName);
    return enrollExistingStudentsToExam(ea,d,docName);
  }
  std::string SessionInformation::addStudentsToExam(const std::string & _eName, const std::string & _dToAddFrom){
    if(allowedToCreateRespRec()==0){
      return "!failed!: Not allowed";
    }
    RMD::Response sf;
    int textExists=sf.setFromTextName(_eName);
    if(textExists==0){
      return "!failed!: Text name does not exist";
    }
    respRecRequested=sf.getTextName();
    std::string oldTD=sf.getTextData();
    long pos;std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(oldTD,pos,s_tDataB,s_tDataE);
    if(allD.second==0){
      return "!failed!: Broken response receiver";
    }
    sf.setTextData(TDI::prepareTextForTextTableRaw(psd,updateExamDocument(allD.first,_dToAddFrom,_eName),"!noOldData!"));
    sf.putInDB();
    return "!success!: "+sf.getTextName();
  }
  std::string SessionInformation::generateExam(const std::string & _eName, const std::string & _dToGenerateFrom){
    if(allowedToCreateRespRec()==0){
      return "!failed!: Not allowed";
    }
    std::string dToGenerateFrom=CEVI::createPlaceHoldersForProblemsWithNoDisplays(_dToGenerateFrom);
    std::vector<std::string> vectProblemsRaw=SF::stringToVector(dToGenerateFrom,"_in*|_","_/in*|_");
    std::pair<std::vector<std::string>,std::string> pairProblemsCert=CERTCLI::problemsAndCertificate(vectProblemsRaw);
    long problemsOK=checkWhetherAllProblemsExist(pairProblemsCert.first);
    if(problemsOK!=1){
      return "!failed!: Problem "+std::to_string(-problemsOK)+" does not exist";
    }
    if(pairProblemsCert.second!=""){
      TMD::MText tmpCheckCertExists;
      if(checkWhetherSingleProblemExists(pairProblemsCert.second,tmpCheckCertExists)==0){
        pairProblemsCert.second="";
      }
    }
    RMD::Response sf;
    int textExists=sf.setFromTextName(_eName);
    if(textExists==1){
      return "!failed!: Text name already exists";
    }
    int succ=sf.createText(_eName);
    if(succ==0){
      return "!failed!: Something may be bad with the database. The text name did not exist when checked but could not create text.";
    }
    respRecRequested=sf.getTextName();
    sf.setTextData(TDI::prepareTextForTextTableRaw(psd,genExamTemplate(dToGenerateFrom,pairProblemsCert,_eName),"!noOldData!"));
    sf.putInDB();
    psd.recoveryOperationNames.push("assignmentCreation");
    psd.recoveryOperationCommands.push(BMD::deleteCommand("deleteResponseReceiver",respRecRequested));
    return "!success!: "+sf.getTextName();
  }
  std::string SessionInformation::generatePdfsForExam(const std::string & _eName, const std::string & _dForReporting){
    if(allowedToCreateRespRec()==0){
      return "!failed!: Not allowed";
    }
    RMD::Response sf;
    int textExists=sf.setFromTextName(_eName);
    long pos; std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(_dForReporting,pos,"_clearFolder*|_","_/clearFolder*|_");
    if((allD.second==1)&& ( (allD.first=="yes")||(allD.first=="y") ) ){
      IOF::sys_clearFolder(MWII::GL_WI.getPublicSystemFileLoc(),"pdf");
    }
    std::vector<std::string> rawTextInClassExamVersions;
    pos=0;allD=SF::extract(_dForReporting,pos,"_inClassExams*|_","_/inClassExams*|_");
    psd.pdfBeforeProblems="";
    psd.pdfAfterProblems="";
    std::string preGeneratedCodes="";
    long numberOfExamsToGenerate=-1;
    long versionIndex=0;
    long versionShift=0;
    if((allD.second==1) && (allD.first!="no") && (allD.first!="n")) {
      psd.indChangeRespRecToPrintVersionOfCommonInClassExam=1;
      rawTextInClassExamVersions=SF::stringToVector(allD.first,"_n*_","_/n*_");
      pos=0;allD=SF::extract(_dForReporting,pos,"_before*|_","_/before*|_");
      if(allD.second==1){psd.pdfBeforeProblems=allD.first;}
      pos=0;allD=SF::extract(_dForReporting,pos,"_after*|_","_/after*|_");
      if(allD.second==1){psd.pdfAfterProblems=allD.first;}
      pos=0;allD=SF::extract(_dForReporting,pos,"_preGeneratedCodes*|_","_/preGeneratedCodes*|_");
      if(allD.second==1){preGeneratedCodes=allD.first;}
      pos=0;allD=SF::extract(_dForReporting,pos,"_numExams*|_","_/numExams*|_");
      if(allD.second==1){
        numberOfExamsToGenerate=BF::stringToInteger(allD.first);
      }
      pos=0;allD=SF::extract(_dForReporting,pos,"_versionIndex*|_","_/versionIndex*|_");
      if(allD.second==1){
        versionIndex=BF::stringToInteger(allD.first);
      }
      pos=0;allD=SF::extract(_dForReporting,pos,"_versionShift*|_","_/versionShift*|_");
      if(allD.second==1){
        versionShift=BF::stringToInteger(allD.first);
      }
    }
    if(textExists==0){
      return "!failed!: Exam does not exist";
    }
    respRecRequested=sf.getTextName();
    std::string rawTextRespRec=sf.getTextData();
    pos=0;allD=SF::extract(rawTextRespRec,pos,s_tDataB,s_tDataE);
    if(allD.second==1){
      std::vector<std::string> allRawStudentsData=SF::stringToVector(allD.first,"_st*|_","_/st*|_");
      long sz=allRawStudentsData.size();
      str_pdfSummary_IfCalledFor="";
      if(psd.indChangeRespRecToPrintVersionOfCommonInClassExam==1){
        if(sz>0){
          std::vector<std::string> stAllData0=SF::stringToVector(allRawStudentsData[0],"_n*|_","_/n*|_");
          if(stAllData0.size()>5){
            RTI::Response rtTemp(stAllData0[0],"no",psd.my_un);
            long numVersions=rawTextInClassExamVersions.size();
            std::vector<std::string> texSourceFiles;
            texSourceFiles.resize(numVersions);
            for(long j=0;j<numVersions;++j){
              psd.probVersionsOfChangedRespRec=rawTextInClassExamVersions[j];
              texSourceFiles[j]=rtTemp.prepareDefaultRequest(psd,"pm");
            }
            std::vector<std::string> allCodes=SF::stringToVector(preGeneratedCodes,"_n*_","_/n*_");
            long numExams=numVersions;
            if(allCodes.size()>1){
              numExams=allCodes.size();
            }
            else{
              if((numberOfExamsToGenerate>1)&&(numVersions>0)){
                numExams=numberOfExamsToGenerate;
                allCodes=LMF::genCodes(numExams,7,numVersions,versionShift,versionIndex);
              }
              else{
                allCodes.resize(numVersions);
              }
            }
            if(numVersions>0){
              for(long j=0;j<numExams;++j){
                str_pdfSummary_IfCalledFor+=LMF::addCodeToSource(texSourceFiles[j%numVersions],allCodes[j]);
              }
            }
            psd.pdfNameForInclassExam=MWII::GL_WI.getPublicSystemFileLoc()+"/exIC"+RNDF::genRandCode(10);
            str_pdfSummary_IfCalledFor=LMF::finalizeLatexForInClassExam(str_pdfSummary_IfCalledFor,MWII::GL_WI.getDefaultWebText("examPreambleLatexTemplate"),psd.pdfNameForInclassExam, MWII::GL_WI.getPublicSystemFileLoc());
            if(allCodes.size()>1){
              std::string codesCommentedInLatexSource;
              for(long i=0;i<allCodes.size();++i){
                codesCommentedInLatexSource+="%%% _n*_"+allCodes[i]+"_/n*_\n";
              }
              str_pdfSummary_IfCalledFor=codesCommentedInLatexSource+str_pdfSummary_IfCalledFor;
            }
            psd.pdfNameForInclassExam+=".pdf";
          }
        }
      }
      else{
        std::vector<std::vector<std::string> > stAllData;
        stAllData.resize(sz);
        for(long i=0;i<sz;++i){
          stAllData[i]=SF::stringToVector(allRawStudentsData[i],"_n*|_","_/n*|_");
          if(stAllData[i].size()>5){
            RTI::Response rtTemp(stAllData[i][0],"no",psd.my_un);
            stAllData[i][1]=rtTemp.prepareDefaultRequest(psd,"pm");
            str_pdfSummary_IfCalledFor+="\n";
            str_pdfSummary_IfCalledFor+=stAllData[i][0]+",";
            str_pdfSummary_IfCalledFor+= MWII::GL_WI.getWSURL()+"/"+stAllData[i][1]+",";
            str_pdfSummary_IfCalledFor+=BF::removeASCII10AND13(SF::findAndReplace(stAllData[i][2],"\n"," ")," ")+",";
            str_pdfSummary_IfCalledFor+=stAllData[i][3]+",";
            str_pdfSummary_IfCalledFor+=stAllData[i][4]+",";
            str_pdfSummary_IfCalledFor+=stAllData[i][5]+",endLine";
          }
        }
      }
    }
    psd.indChangeRespRecToPrintVersionOfCommonInClassExam=0;
    psd.probVersionsOfChangedRespRec="";
    return "!success!: "+respRecRequested;
  }
  std::string SessionInformation::updateIndividualVersionsOfExam(const std::string & _eName, const std::string & _textWithUpdate){
    if(allowedToCreateRespRec()==0){
      return "!failed!: Not allowed";
    }
    RMD::Response sf;
    int textExists=sf.setFromTextName(_eName);
    if(textExists==0){
      return "!failed!: Exam does not exist";
    }
    respRecRequested=sf.getTextName();
    std::string rawTextRespRec=sf.getTextData();
    long pos; std::pair<std::string, int> allD;
    pos=0;allD=SF::extract(rawTextRespRec,pos,s_tDataB,s_tDataE);
    if(allD.second==1){
      std::vector<std::string> problemLabels=SF::stringToVector(allD.first,"_lb*|_","_/lb*|_");
      std::vector<std::string> allRawStudentsData=SF::stringToVector(allD.first,"_st*|_","_/st*|_");
      CEVI::StudentAnswRRUpdateData stDataVA=CEVI::createFromString(problemLabels.size(),allRawStudentsData.size(),_textWithUpdate);
      std::map<std::string,std::string>::const_iterator it,itE,itA,itAE;
      itE=(stDataVA.uNamesToVersions).end();
      itAE=(stDataVA.uNamesToAnswers).end();
      std::vector<std::vector<std::string> > stAllData;
      long sz=allRawStudentsData.size();
      stAllData.resize(sz);
      str_pdfSummary_IfCalledFor="";
      for(long i=0;i<sz;++i){
        stAllData[i]=SF::stringToVector(allRawStudentsData[i],"_n*|_","_/n*|_");
        if(stAllData[i].size()>5){
          RMD::Response ssf;
          if(ssf.setFromTextName(stAllData[i][0])){
            std::string td=ssf.getTextData();
            it=(stDataVA.uNamesToVersions).find(stAllData[i][4]);
            if(it!=itE){td=CEVI::updateVersionsForStudent(td,it->second,problemLabels);}
            itA=(stDataVA.uNamesToAnswers).find(stAllData[i][4]);
            if(itA!=itAE){
              td=CEVI::updateAnswersForStudent(td,itA->second,problemLabels);
            }
            ssf.setTextData(td);
            ssf.putInDB();
          }
        }
      }
    }
    return "!success!: "+respRecRequested;
  }
  std::string SessionInformation::createRRBackup(const std::string& rrName, const std::string & action) const{
    RMD::Response ssf;
    if(ssf.setFromTextName(rrName)){
      std::string td=ssf.getTextData();
      long pos;std::pair<std::string,int> allD;
      pos=0;allD=SF::extract(td,pos,s_tDataB,s_tDataE);
      if(allD.second==1){
        td=allD.first;
      }
      return CCFI::createRRBackupActivationText(rrName,action,td);
    }
    return "notFound";
  }
  std::map<std::string,std::string> SessionInformation::mapNamesRawTexts(const std::set<std::string>& s){
    std::map<std::string,std::string> m;
    std::set<std::string>::const_iterator it,itE;
    itE=s.end();
    it=s.begin();
    while(it!=itE){
      TMD::MText dbText;
      int exists=dbText.setFromTextName(*it);
      if(exists){
        long pos; std::pair<std::string,int> allD;
        pos=0;allD=SF::extract(dbText.getTextData(),pos,s_tDataB,s_tDataE);
        if(allD.second){
          m[*it]=allD.first;
        }
      }
      ++it;
    }
    return m;
  }
  std::string SessionInformation::getOfflineAutograderData(const std::string& raw){
    std::string rawAGData=SF::vectorToString(SF::stringToVector(raw,"_agr*|_","_/agr*|_") ,"","");
    std::string rawDBIncl=SF::vectorToString(SF::stringToVector(rawAGData,"_dbIncludes_","_/dbIncludes_"),"","");
    std::set<std::string> dbIncludes=SF::vectorToSet(SF::stringToVector(rawDBIncl,"_n*_","_/n*_"));
    std::set<std::string> asmRules=SF::vectorToSet(SF::stringToVector(rawAGData,"_asmRules_","_/asmRules_"));
    std::map<std::string,std::string> m_dbIncl=mapNamesRawTexts(dbIncludes);
    std::map<std::string,std::string> m_asmRl=mapNamesRawTexts(asmRules);
    std::string textResult="_offlineData*|_\n_offDBIncludes*|_\n";
    textResult+=SF::mapToString(m_dbIncl,"_vVPair_","_/vVPair_","_vr_","_/vr_","_vl_","_/vl_");
    textResult+="\n_/offDBIncludes*|_\n\n_offASMRules*|_";
    textResult+=SF::mapToString(m_asmRl,"_vVPair_","_/vVPair_","_vr_","_/vr_","_vl_","_/vl_");
    textResult+="\n_/offASMRules*|_\n\n_/offlineData*|_\n\n";
    return textResult;
  }
  std::string SessionInformation::createExamBackupText(const std::string & _eName, const std::string & _commandsAndOptions){
    if(allowedToCreateRespRec()==0){
      return "!failed!: Not allowed";
    }
    RMD::Response sf;
    int textExists=sf.setFromTextName(_eName);
    if(textExists==0){
      return "!failed!: Exam does not exist";
    }
    respRecRequested=sf.getTextName();
    std::string rawTextRespRec=sf.getTextData();
    std::string examRRAction="modifyResponseReceiver";
    std::string gradingRRAction="modifyResponseReceiver";
    std::string masterRRAction="modifyResponseReceiver";
    long pos; std::pair<std::string, int> allD;
    pos=0;allD=SF::extract(_commandsAndOptions,pos,"_examRRAction*_","_/examRRAction*_");
    if(allD.second==1){
      examRRAction=allD.first;
    }
    pos=0;allD=SF::extract(_commandsAndOptions,pos,"_gradingRRAction*_","_/gradingRRAction*_");
    if(allD.second==1){
      gradingRRAction=allD.first;
    }
    pos=0;allD=SF::extract(_commandsAndOptions,pos,"_masterRRAction*_","_/masterRRAction*_");
    if(allD.second==1){
      masterRRAction=allD.first;
    }
    std::string offlineAGData=getOfflineAutograderData(rawTextRespRec);
    pos=0;allD=SF::extract(rawTextRespRec,pos,s_tDataB,s_tDataE);
    if(allD.second==1){
      std::vector<std::vector<std::string> > allStudentFileNames=SF::stringToMatrix(allD.first,"_st*|_","_/st*|_","_n*|_","_/n*|_");
      long sz=allStudentFileNames.size();
      for(long i=0;i<sz;++i){
        if(allStudentFileNames[i].size()>5){
          psd.respRecBackupText +=createRRBackup( allStudentFileNames[i][0],examRRAction);
          psd.respRecBackupText +=createRRBackup( allStudentFileNames[i][1],gradingRRAction);
        }
      }
    }
    psd.respRecBackupText =offlineAGData+ createRRBackup(_eName,masterRRAction)+psd.respRecBackupText;
    return "!success!: "+respRecRequested;
  }
  std::string SessionInformation::distributeExamToStudents(const std::string & _examTemplateName, const std::string & _distributionText){
    return "";
  }
  std::string SessionInformation::deleteSingleRespRec(const std::string & _tName){
    if(allowedToModifyRespRec(_tName,"no")==0){
      return "!failed!: Not allowed";
    }
    RMD::Response sf;
    int textExists=sf.setFromTextName(_tName);
    if(textExists==0){
      return "!failed!: Text name does not exist";
    }
    // We need to delete all files that this response receiver has uploaded.
    std::string rawText=sf.getTextData();
    long pos;std::pair<std::string,int> allD;
    std::string sep_B,sep_E, externalFileCode,fileTextToSearch;
    pos=0;allD=SF::extract(rawText,pos,"_nf*|_","_/nf*|_");
    if(allD.second==1){
      long numF=BF::stringToInteger(allD.first);
      for(long i=0;i<numF;++i){
        sep_B="f"+BF::padded(i,100,"0")+"*|_";
        sep_E="_/"+sep_B;
        sep_B="_"+sep_B;
        pos=0;allD=SF::extract(rawText,pos,sep_B,sep_E);
        if(allD.second==1){
          fileTextToSearch=allD.first;
          pos=0;allD=SF::extract(fileTextToSearch,pos,"_n*|_","_/n*|_");
          if(allD.second==1){
            externalFileCode=IOF::justFileNameNoExtensionNoFolder(allD.first,MWII::FILE_PREFIX);
            deleteFile(externalFileCode);
          }
        }
      }
    }
    // We need to delete its grader also
    pos=0;allD=SF::extract(rawText,pos,"_grd*|_","_/grd*|_ ");
    if(allD.second==1){
      deleteRespRec(allD.first);
    }
    int succ=sf.deleteRecord();
    if(succ==0){
      return "!failed!: Something may be bad with the database. The text name did exist when checked but could not delete.";
    }
    return "!success!";
  }
  std::string SessionInformation::deleteRespRec(const std::string & _tName){
    // Step 1: Check whether _tName is a scraped website data with a lot of response receivers
    std::vector<std::string> manyRespRec=PASF::getItemsFromScrapedPage(_tName,"\n","\t");
    long sz=manyRespRec.size();
    if(sz>0){
      // Step 2.1: There are multiple requests
      //           We delete each response receiver
      for(long i=0;i<sz;++i){
        deleteSingleRespRec(manyRespRec[i]);
      }
      return "!success!";
    }
    // Step 2.2: There is a single request
    //           We delete the single response receiver
    return deleteSingleRespRec(_tName);
  }
  int SessionInformation::allowedToDeleteUser(const std::string & ) const{
    if(psd.isRoot=="yes"){return 1;}//WARNING: permission checking is too restrictive
    return 0;
  }
  std::string SessionInformation::deleteSingleUser(const std::string & _uName){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToDeleteUser(_uName)==0){
      return "!failed!: Not allowed to delete user";
    }
    deleteCloneForGuest(_uName);
    WUD::User w;
    int userExists=w.setFromUsername(_uName);
    if(userExists==0){
      return "!failed!: Username does not exist";
    }
    int succ=w.deleteUser();
    if(succ==0){
      return "!failed!: Something may be bad with the database. The username did exist when checked but could not delete.";
    }
    return "!success!";
  }
  std::string SessionInformation::deleteUser(const std::string & _uName){
    // Step 1: Check whether _uName is a scraped website data with a lot of users
    std::vector<std::string> manyUsers=PASF::getItemsFromScrapedPage(_uName,"\n","\t");
    long sz=manyUsers.size();
    if(sz>0){
      // Step 2.1: There are multiple requests
      //           We delete each user
      for(long i=0;i<sz;++i){
        deleteSingleUser(manyUsers[i]);
      }
      return "!success!";
    }
    // Step 2.2: There is a single request
    //           We delete the single user
    return deleteSingleUser(_uName);
  }
  int SessionInformation::allowedToAddToHierarchy(const std::string & h, const std::string & l) const{
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    return 0;
  }
  int SessionInformation::allowedToRemoveFromHierarchy(const std::string & h, const std::string & l) const{
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    return 0;
  }
  int SessionInformation::allowedToAssignGraders(const std::string & p, const std::string & q) const{
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    return 0;
  }
  int SessionInformation::allowedToEnrollStudents(const std::string & p, const std::string & q) const{
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    return 0;
  }
  int SessionInformation::allowedToEditTimer(const std::string & p, const std::string & q) const{
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    return 0;
  }
  int SessionInformation::allowedToBackup(const std::string & db, const std::string & txt) const{
    if((psd.isRoot=="yes") ){
      return 1;
    }
    return 0;
  }
  std::string SessionInformation::addToHierarchy(const std::string & _uH, const std::string & _uL){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToAddToHierarchy(_uH,_uL)==0){
      return "!failed!: Operation not allowed";
    }
    WUD::User uH,uL;
    int userHExists=uH.setFromUsername(_uH);
    int userLExists=uL.setFromUsername(_uL);
    if((userHExists!=1)||(userLExists!=1)){
      return "!failed!: One of the users does not exist";
    }
    uH.addToHierarchy(uL,"meHigh");
    return "!success!";
  }
  std::string SessionInformation::removeFromHierarchy(const std::string & _uH, const std::string & _uL){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToRemoveFromHierarchy(_uH,_uL)==0){
      return "!failed!: Operation not allowed";
    }
    WUD::User uH,uL;
    int userHExists=uH.setFromUsername(_uH);
    int userLExists=uL.setFromUsername(_uL);
    if((userHExists!=1)||(userLExists!=1)){
      return "!failed!: One of the users does not exist";
    }
    uH.removeFromHierarchy(uL,"meHigh");
    return "!success!";
  }
  std::string SessionInformation::assignGraders(const std::string & _rR, const std::string & _options){
    //WARNING: not implemented properly yet: too resrtictive
    if(allowedToAssignGraders(_rR,_options)==0){
      return "!failed!: Operation not allowed";
    }
    RTI::Response tmpRT(_rR,"no",psd.my_un);
    if(tmpRT.isInitialized()==1){
      return tmpRT.assignGraders(psd,_options);
    }
    return "!failed! Could not find the document.";
  }
  std::string SessionInformation::editTimer(const std::string & _rR, const std::string & _options){
    //WARNING: not implemented properly yet: too resrtictive
    if(allowedToEditTimer(_rR,_options)==0){
      return "!failed!: Operation not allowed";
    }
    RTI::Response tmpRT(_rR,"no",psd.my_un);
    if(tmpRT.isInitialized()==1){
      return tmpRT.editTimer(psd,_options);
    }
    return "!failed! Could not find the document.";
  }
  std::string SessionInformation::submitEnrollmentReport(const std::string &_rName,const std::string &_couas, const std::vector<PASF::StudentData> & students){
    TMD::MText m;
    int exists=m.setFromTextName(_rName);
    if(exists!=1){
      createText(_rName,TDI::prepareEmptyTextWithAdvancedPermissions("administrator","administrator"));
    }
    exists=m.setFromTextName(_rName);
    if(exists==1){
      std::string oldTD=m.getTextData();
      long pos;std::pair<std::string,int> allD;
      pos=0;allD=SF::extract(oldTD,pos,s_tDataB,s_tDataE);
      if(allD.second==1){
        std::string mT=allD.first;
        mT+="<h2>Couas: "+_couas+"</h2>";
        long numSt=students.size();
        mT+="<h3>For emails to students</h3>";
        for(long i=0;i<numSt;++i){
          mT+="<BR>"+students[i].putIntoStringForEmail();
        }
        mT+="<h3>For mass enrollment in exams</h3>";
        for(long i=0;i<numSt;++i){
          mT+="<BR>"+students[i].putIntoString(0);
        }
        modifyText(_rName,mT);
        return "!success!";
      }
    }
    return "!failed!";
  }
  std::string SessionInformation::createAccountsForStudents(std::vector<PASF::StudentData> & students,const std::string & _couas){
    long intToAdd;
    long numSt=students.size();
    WUD::User wC;
    int couasUserExists=wC.setFromUsername("p_"+_couas);
    if(couasUserExists==0){
      wC.createUser("p_"+_couas);
      wC.addUserData(PASF::generatePassword(15));
      psd.recoveryOperationNames.push("enrollment");
      psd.recoveryOperationCommands.push(BMD::deleteCommand("deleteUser","p_"+_couas));
    }
    for(long i=0;i<numSt;++i){
      WUD::User wu;
      int userExists=wu.setFromExternalId(students[i].externalId);
      int creationSucc=-7;
      if(userExists==1){
        students[i].userName=wu.getUsername();
        students[i].password="na";
        WUD::OthUsData tmpOUD;
        tmpOUD.firstName=students[i].firstName;
        tmpOUD.lastName=students[i].lastName;
        tmpOUD.email=students[i].email;
        wu.addUserData(tmpOUD);
        wu.addToHierarchy(wC,"meHigh");
      }
      else{
        intToAdd=0;
        WUD::User wuN;
        std::string unToAttempt=students[i].userName;
        creationSucc=wuN.createUser(unToAttempt,"",students[i].externalId);
        while((intToAdd<10000)&&(creationSucc==0)){
          ++intToAdd;
          unToAttempt=students[i].userName+std::to_string(intToAdd);
          creationSucc=wuN.createUser(unToAttempt,"",students[i].externalId);
        }
        if(creationSucc==1){
          students[i].password=PASF::generatePassword(8);
          students[i].userName=unToAttempt;
          WUD::OthUsData tmpOUD;
          tmpOUD.password=students[i].password;
          tmpOUD.firstName=students[i].firstName;
          tmpOUD.lastName=students[i].lastName;
          tmpOUD.email=students[i].email;
          tmpOUD.masterKey=psd.masterKey;
          wuN.addUserData(tmpOUD);
          wuN.addToHierarchy(wC,"meHigh");
          psd.recoveryOperationNames.push("enrollment");
          psd.recoveryOperationCommands.push(BMD::deleteCommand("deleteUser",unToAttempt));
        }
      }
    }
    return "!success!";
  }
  std::string SessionInformation::enrollStudentsInCouas(const std::string & _couas, const std::vector<PASF::StudentData> & students, const long & recursionDepth){
    if(recursionDepth<1){
      return "";
    }
    CEI::CouasElement cEl(psd);
    if(ICEI::initFromCode(psd,cEl,_couas)=="failed"){
      return "";
    }
    std::string modifiedText=cEl.addStudents(psd,students);
    if(modifiedText!=""){
      MCWCPI::modifyCouasWithoutCheckingPermissionsAndCreateRecoveryCommand(psd,_couas,modifiedText);
    }
    long pos;std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(cEl.getRawText(),pos, LI::GL_LN.st_sepGradeFormulaB,LI::GL_LN.st_sepGradeFormulaE);
    std::string fR="success in "+_couas+"<BR>\n";
    if(allD.second==1){
      std::vector<std::string> vForRec=SF::stringToVector(allD.first,LI::GL_LN.st_sepGetFromB, LI::GL_LN.st_sepGetFromE);
      long sz=vForRec.size();
      for(long i=0;i<sz;++i){
        fR+=enrollStudentsInCouas(vForRec[i],students,recursionDepth-1);
      }
    }
    return fR;
  }
  std::string SessionInformation::enrollStudents(const std::string & _couas, const std::string & _reportName, const std::string & _prefixForExtId, const std::string & _options){
    //WARNING: not implemented properly yet: too resrtictive
    if(allowedToEnrollStudents(_couas,_options)==0){
      return "!failed!: Operation not allowed";
    }
    std::vector<PASF::StudentData> students=PASF::identifyStudents(_options,_prefixForExtId);
    std::string accCrRes=createAccountsForStudents(students,_couas);
    std::string couasModSuccess=enrollStudentsInCouas(_couas,students);
    std::string er= submitEnrollmentReport(_reportName,_couas,students);
    return er;
    return "!failed! Could not find the document.";
  }
  std::string SessionInformation::backupDBs(const std::string & _db, const std::string & _txts){
    //WARNING: not implemented properly yet: permission checking is too restrictive
    if(allowedToRemoveFromHierarchy(_db,_txts)==0){
      return "!failed!: Operation not allowed";
    }
    std::vector<std::string> backupTexts=SF::stringToVector(_txts,"_n*!_","_/n*!_");
    long sz=backupTexts.size();
    str_backups_IfCalledFor="";
    if((sz==2)&&(backupTexts[0]=="toLatex")){
      if(AICD::GL_ReplStrings.htmlTs.size()<1){
        AICD::GL_ReplStrings = APTI::createLatexReplacementStrings();
      }
      std::vector<std::string> docNames=SF::stringToVector(backupTexts[1],"_n*!_","_/n*!_");
      std::string lStart;
      lStart=MWII::GL_WI.getDefaultWebText("notesPreambleLatexTemplate");
      if(lStart=="notesPreambleLatexTemplate"){
        lStart=MWII::GL_WI.getDefaultWebText("examPreambleLatexTemplate");
      }
      long pos; std::pair<std::string,int> allD;
      pos=0; allD=SF::extract(backupTexts[1],pos,"_lStart*!_","_/lStart*!_");
      if(allD.second==1){
        lStart=allD.first;
      }
      str_backups_IfCalledFor+=lStart+"\n\n \\begin{document}\n";
      long szd=docNames.size();
      for(long i=0;i<szd;++i){
        str_backups_IfCalledFor+=BMD::recovery(_db,docNames[i],"latex");
      }
      str_backups_IfCalledFor+="\n\n\\end{document} \n";
      return "!success!";
    }
    if((sz==3) && (backupTexts[0]=="range") && (BF::isNumeric(backupTexts[1],0)) && (BF::isNumeric(backupTexts[2],0)) ){
      long start=BF::stringToInteger(backupTexts[1]);
      long end=BF::stringToInteger(backupTexts[2]);
      sz=end-start;
      if(sz>0){
        backupTexts.resize(sz);
        for(long j=start;j<end;++j){
          backupTexts[j-start]=BMD::convertNumberToDBRowName(_db,j);
        }
      }
    }
    for(long i=0;i<sz;++i){
      str_backups_IfCalledFor+=SF::findAndReplace(BMD::recovery(_db,backupTexts[i],"basic"),"!verbSave!","*fjkl3"+GL_MAIN_SETUP_FILE_NAME+"2!3211");
    }
    return "!success!";
  }
  int SessionInformation::allowedToAssignPermit(const std::string &_iR, const std::string &_iAEC) const{
    if((psd.isRoot=="no")&&(_iR=="yes")){
      return 0;
    }
    if((psd.allowedToExecuteAll=="no")&&(_iAEC=="yes")){
      return 0;
    }
    return 1;
  }
  int SessionInformation::allowedToModifyUser(const std::string & _uName) const{
    if(_uName==psd.my_un){
      return 1;
    }
    if(psd.isRoot=="yes"){return 1;}//WARNING: permission checking is too restrictive
    return 0;
  }
  std::string SessionInformation::createUser(const std::string & _uName, const std::string &_extId, const std::string &_fN, const std::string &_lN, const std::string &_p, const std::string &_e, const std::string &_iR, const std::string &_iAEC, const std::string & _iAClone){
    //WARNING: not implemented properly yet: too restrictive when checking permissions
    if(allowedToAssignPermit(_iR,_iAEC)==0){
      return "!failed!: illegal permits.";
    }
    if(allowedToModifyUser(_uName)==0){
      return "!failed!: cannot modify user.";
    }
    WUD::User w;
    WUD::OthUsData tmpOUD;
    tmpOUD.password=_p;
    tmpOUD.firstName=_fN;
    tmpOUD.lastName=_lN;
    tmpOUD.email=_e;
    tmpOUD.isRoot=_iR;
    tmpOUD.allowedToExecuteCommands=_iAEC;
    tmpOUD.allowedToCloneTheWebsite=_iAClone;
    w.addUserData(tmpOUD);
    int userCreationSucc=w.createUser(_uName,psd.masterKey,_extId);
    if(userCreationSucc!=1){
      return "!failed!: createUser returned the error code "+std::to_string(userCreationSucc);
    }
    return "!success!";
  }
  std::string SessionInformation::modifyUser(const std::string & _uName, const std::string &_fN, const std::string &_lN, const std::string &_p, const std::string &_e, const std::string &_iR, const std::string &_iAEC, const std::string & _iAClone){
    //WARNING: not implemented properly yet: too restrictive in checking permissions
    if(allowedToAssignPermit(_iR,_iAEC)==0){
      return "!failed!: illegal permits.";
    }
    if(allowedToModifyUser(_uName)==0){
      return "!failed!: cannot modify user.";
    }
    WUD::User w;
    int userIdentificationSucc=w.setFromUsername(_uName);
    if(userIdentificationSucc!=1){
      return "!failed!: user not found";
    }
    WUD::OthUsData tmpOUD;
    tmpOUD.password=_p;
    tmpOUD.firstName=_fN;
    tmpOUD.lastName=_lN;
    tmpOUD.email=_e;
    tmpOUD.isRoot=_iR;
    tmpOUD.allowedToExecuteCommands=_iAEC;
    tmpOUD.allowedToCloneTheWebsite=_iAClone;
    if( (_uName!=psd.my_un) && (_p!="!*!") ){
        tmpOUD.masterKey=psd.masterKey;
    }
    w.addUserData(tmpOUD);
    return "!success!";
  }
  std::string SessionInformation::uploadFilesFromResponseReceiver(
    const cgicc::Cgicc & ch,const std::string & respRecRequested,
    const std::string & dataToModify,
    const std::vector<std::string> &fInfoV,
    const RTI::Response & _t, const std::string & _nameOfPersonWhoIsFillingTheForm){
    std::string nDataText=dataToModify;
    long numFilesAllowed=fInfoV.size();
    cgicc::const_file_iterator fileIt, fileItE;
    fileItE=ch.getFiles().end();
    std::string sepSmB="_n*|_",sepSmE="_/n*|_";
    std::string sepB,sepE,labelHTML;
    long pos;
    std::pair<std::string,int> allD;
    std::vector<std::string> fiV;std::string fname_i;
    std::string itemToReplace;long fileNotFound,fileErased;
    if(numFilesAllowed>0){
      for(long i=0;i<numFilesAllowed;++i){
        sepB=_t.indFileSepB(i);
        sepE=_t.indFileSepE(i);
        labelHTML=_t.fileLabelForHTML(i);
        fname_i="notFound";
        fileNotFound=0;
        fileErased=0;
        if(fInfoV[i]!="notFound"){
          // we have the information about file - a version is already there
          // now we will check whether it is properly stored and
          // see if the user is re-submitting the file
          fiV=SF::stringToVector(fInfoV[i],sepSmB,sepSmE);
          if(fiV.size()>0){
            fname_i=fiV[0];
            if(fname_i!="notFound"){
              fileIt=ch.getFile(labelHTML);
              if(fileIt!=fileItE){
                //file exists
                if(fileIt->getDataLength() < MWII::GL_MAX_FILE_RESP_REC){
                  // delete the old file
                  // Step 1: find out the file code
                  std::string fSaltedCode=IOF::justFileNameNoExtensionNoFolder(fname_i,MWII::FILE_PREFIX);
                  std::pair<std::string,std::string> codeAndSalt=SF::oneWordToTwoWords(fSaltedCode);
                  // Step 2: delete file
                  deleteFile(codeAndSalt.first);
                  // Step 3: Tell that we need to upload new file
                  fileErased=1;
                }
                else{
                  psd.respRecFlag="fileTooLarge";
                }
              }
            }
          }
        }
        else{
          fileNotFound=1;
        }
        if((fileNotFound==1)||(fileErased==1)){
          // we don't have a previous records of this file
          // we will now check whether a user is submitting the file
          fileIt=ch.getFile(labelHTML);
          if(fileIt!=fileItE){
            if(fileIt->getDataLength() < MWII::GL_MAX_FILE_RESP_REC){
              std::string additionalData="_fInfo*|_"+_nameOfPersonWhoIsFillingTheForm+"("+labelHTML+")"+"_/fInfo*|_";
              additionalData+="_nRespRec*|_"+_t.getTextName()+"_/nRespRec*|_";
              additionalData+="_fileDataType_"+fileIt->getDataType()+"_/fileDataType_";
              additionalData+="_fSize*|_"+std::to_string(fileIt->getDataLength())+"_/fSize*|_";
              fname_i=placeNewFileToServer( ch, "root", "findOutFromDataType", labelHTML,additionalData);
              if(fname_i=="failed"){
                psd.respRecFlag="badExtension";
              }
              else{
                itemToReplace=sepB+sepSmB+fname_i+sepSmE;
                itemToReplace+=sepSmB+fileIt->getDataType();
                itemToReplace+=sepSmE;
                itemToReplace+=sepSmB+std::to_string(fileIt->getDataLength());
                itemToReplace+=sepSmE;
                itemToReplace+=sepE;
                allD=SF::eraseStuffBetween(nDataText,sepB,sepE);
                if(allD.second==1){
                  nDataText=allD.first;
                }
                nDataText+=itemToReplace;
              }
            }
            else{
              psd.respRecFlag="fileTooLarge";
            }
          }
        }
      }
    }
    return nDataText;
  }
  std::map<std::string,PSDI::GradingRule> initialProblemRuleMap(const std::string& key, const PSDI:: GradingRule& gr){
    std::map<std::string,PSDI::GradingRule> initialM;
    initialM[key]=gr;
    return initialM;
  }
  std::map<std::string,PSDI::GradingRule> initialProblemRuleMap(const std::string& key, const std::string& _display, const double& _points){
    PSDI::GradingRule gr;
    gr.display=_display;
    gr.points=_points;
    return initialProblemRuleMap(key,gr);
  }
  std::string SessionInformation::treatRuleCommands(const std::string& qLabel,const std::string &in){
    std::string out=in;
    std::pair<std::vector<std::string>,std::string> rulesToDelete=SF::stringToVectorAndRemoveItems(out,MWII::GL_WI.getDefaultWebText("[deleteRule]"),MWII::GL_WI.getDefaultWebText("[/deleteRule]"),1);
    if((rulesToDelete.first).size()>0){
      out=rulesToDelete.second;
      long sz=(rulesToDelete.first).size();
      for(long i=0;i<sz;++i){
        out+=MWII::GL_WI.getDefaultWebText("[addRule]")+(rulesToDelete.first)[i]+";";
        out+="0;*del*"+MWII::GL_WI.getDefaultWebText("[/addRule]");
      }
    }
    std::pair<std::vector<std::string>,std::string> rules=SF::stringToVectorAndRemoveItems(out,MWII::GL_WI.getDefaultWebText("[addRule]"),MWII::GL_WI.getDefaultWebText("[/addRule]"),1);
    if((rules.first).size()>0){
      out=rules.second;
      long sz=(rules.first).size();
      std::string ruleName; double rulePoints;std::string ruleDisplay;
      long pos; std::pair<std::string,int> allD;
      std::map<std::string,std::map<std::string,PSDI::GradingRule> >::iterator it,itE;
      PSDI::GradingRule nextRule;
      if(psd.isRoot=="yes"){
        for(long i=0;i<sz;++i){
          pos=0;allD=SF::getEverythingBefore((rules.first)[i],pos,";");
          if(allD.second==1){
            ruleName=BF::cleanSpaces(allD.first,0,0);
            ruleDisplay=ruleName;
            long oldPosSave=pos;
            allD=SF::getEverythingBefore((rules.first)[i],pos,";");
            if(allD.second==1){
              rulePoints=BF::stringToDouble(allD.first);
              ruleDisplay="";
              SF::copyTheRestOfTheStringAndGiveUp((rules.first)[i],pos,ruleDisplay,1);
              ruleDisplay=BF::cleanSpaces(ruleDisplay,0,0);
            }
            else{
              std::string pointsString;
              SF::copyTheRestOfTheStringAndGiveUp((rules.first)[i],oldPosSave,pointsString,1);
              rulePoints=BF::stringToDouble(pointsString);
            }
            itE=(psd.addModifyRulesCommands).end();
            it=(psd.addModifyRulesCommands).find(qLabel);
            if(it==itE){
              (psd.addModifyRulesCommands)[qLabel]=initialProblemRuleMap(ruleName,ruleDisplay,rulePoints);
            }
            else{
              nextRule.points=rulePoints;
              nextRule.display=ruleDisplay;
              ((psd.addModifyRulesCommands)[qLabel])[ruleName]=nextRule;
            }
          }
        }
      }
    }
    return out;
  }
  std::map<std::string,std::string> separateQuestionsAndComments(const std::map<std::string,std::string> & in){
    std::map<std::string,std::string> res;
    std::map<std::string,std::string>::const_iterator it,itE;
    it=in.begin(); itE=in.end();
    std::pair<std::string,int> allD; long pos;
    std::string score; std::string comment;
    while(it!=itE){
      pos=0; allD=SF::extract(it->second,pos,"_score_","_/score_");
      if(allD.second){
        score=allD.first;
        pos=0;allD=SF::extract(it->second,pos,"_comment_","_/comment_");
        if(allD.second){
          comment=allD.first;
          res[it->first]=comment;
          res["P"+it->first]=score;
        }
        else{
          res[it->first]=it->second;
        }
      }
      else{
        res[it->first]=it->second;
      }
      ++it;
    }
    return res;
  }
  void correctRespMapForDataRace(std::map<std::string,std::string>& resp, const std::map<std::string,std::string>& snapshotBeforeEdit, const std::map<std::string,std::string>& mostCurrentVersion){
    std::map<std::string,std::string>::iterator it,itE;
    std::map<std::string,std::string>::const_iterator itS, itSE, itC, itCE;
    it=resp.begin(); itE=resp.end();
    itSE=snapshotBeforeEdit.end();
    itCE=mostCurrentVersion.end();
    while(it!=itE){
      if( ((it->first)[0]=='Q')
                     ||
          (
            ((it->first).length()>1)
              &&
            ((it->first)[0]=='P')
              &&
            ((it->first)[1]=='Q')
          )
        ){
        itS=snapshotBeforeEdit.find(it->first);
        if(itS!=itSE){
          if( it->second == itS->second ){
            itC=mostCurrentVersion.find(it->first);
            if(itC!=itCE){
              it->second=itC->second;
            }
          }
        }
        itC=mostCurrentVersion.find(it->first);
      }
      ++it;
    }
  }
  void SessionInformation::updateRespMapToProperlyAccountForBothGraderCommentsAndPoints(const RTI::ResponderInfo& res){
    std::map<std::string,std::string>
      oldMap=SF::stringToMap( IOF::fileToString(
                                DD::GL_DBS.getChallengeAnswStorage()+"/"+"gr_"+res.docName+"."+psd.my_un,
                                1
                              ),
                              "_ky*_",
                              "_/ky*_",
                              "_vl*_",
                              "_/vl*_"
                            );
    correctRespMapForDataRace(psd.respMap,separateQuestionsAndComments(oldMap),separateQuestionsAndComments(res.gradersComments));
    std::map<std::string,std::string> tempRespToKeepPointsOnly;
    std::map<std::string,std::string>::iterator it,it2,itE,it2E;
    it=(psd.respMap).begin();
    itE=(psd.respMap).end();
    std::string qAnswerKey;
    long sz;
    while(it!=itE){
      it2E=tempRespToKeepPointsOnly.end();
      if((it->first)[0]=='Q'){
        //we found a comment to question
        it2=tempRespToKeepPointsOnly.find(it->first);
        if(it2==it2E){
          tempRespToKeepPointsOnly[it->first]="_comment_"+ it->second+"_/comment_";
        }
        else{
          it2->second += "_comment_"+treatRuleCommands(it->first,it->second)+"_/comment_";
        }
      }
      sz=(it->first).size();
      if(
        (sz>1)&&
        ((it->first)[0]=='P')&&
        ((it->first)[1]=='Q')
        ){
        // We found points for a question
        qAnswerKey="";
        for(long i=1;i<sz;++i){
          qAnswerKey+=(it->first)[i];
        }
        it2=tempRespToKeepPointsOnly.find(qAnswerKey);
        if(it2==it2E){
          tempRespToKeepPointsOnly[qAnswerKey]="_score_"+BF::makeItZeroIfNotNumeric(it->second)+"_/score_";
        }
        else{
          it2->second += "_score_"+BF::makeItZeroIfNotNumeric(it->second)+"_/score_";
        }
      }
      ++it;
    }
    it=tempRespToKeepPointsOnly.begin();
    itE=tempRespToKeepPointsOnly.end();
    while(it!=itE){
      (psd.respMap)[it->first]=it->second;
      ++it;
    }
  }
  int SessionInformation::updateGradesFromResponse(){
    std::string couasCode=psd.inFormReplace;
    CEI::CouasElement cEl(psd);
    if(ICEI::initFromCode(psd,cEl,couasCode)=="failed"){
      return 0;
    }
    cEl.sortGradeData(psd.sortCriterion);
    if(cEl.allowedToGrade(psd)){
      std::string newGradeData=cEl.updateGrades(psd,psd.respMap);
      if(newGradeData=="failed"){
        return 0;
      }
      std::string scc=MCWCPI::modifyCouasWithoutCheckingPermissions(psd,couasCode,newGradeData);
      if(scc=="!success!"){
        return 1;
      }
    }
    return 0;
  }
  std::string changeTypeIfOfHigherPriority(const std::string &type, const std::string &newType){
    if(type=="mainText"){
      return newType;
    }
    if(type=="assignmentCreation"){
      if((newType=="courseCreation")||(newType=="enrollment")) {
        return newType;
      }
      return type;
    }
    if(type=="enrollment"){
      if(newType=="courseCreation") {
        return newType;
      }
    }
    return type;
  }
  std::string SessionInformation::createRecoveryCommands(){
    if(psd.recoveryOperationCommands.size()<1){return "nothingToDo";}
    std::string operationCommands="";
    std::string type="mainText";
    SF::flipTheStack(psd.recoveryOperationCommands);
    while(!psd.recoveryOperationCommands.empty()){
      operationCommands += psd.recoveryOperationCommands.top();
      psd.recoveryOperationCommands.pop();
      type=changeTypeIfOfHigherPriority(type,psd.recoveryOperationNames.top());
      if(!psd.recoveryOperationNames.empty()){psd.recoveryOperationNames.pop();}
    }
    std::string finalResult="_user_"+psd.my_un+"_/user_";
    finalResult+="\n_type_"+type+"_/type_\n";
    finalResult+="_rec!!***!_"+operationCommands+"_/rec!!***!_";
    TMF::Timer tm;
    std::vector<long> tnVector=tm.timeNowVector();
    std::vector<std::string> name;
    name.resize(1);name[0]="";
    name[0]+="d"+BF::padded(tnVector[0],1000,"0");
    for(long j=1;j<6;++j){
      name[0]+=BF::padded(tnVector[j],10,"0");
    }
    name[0]+="e";
    MTF::Table& backupT=DD::GL_MAIN_DB.dbsM["backupDatabase"];
    long maxBackupSize=DD::GL_DBS.getBackupMaxSZ();
    if(backupT.size() >= maxBackupSize){
      std::pair<std::vector<std::string>, std::string> firstEl=backupT[0];
      backupT.delRow(firstEl.first);
    }
    backupT.insertMTF(name,finalResult);
    return "done";
  }
}
#endif
