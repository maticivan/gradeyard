//    GradeYard learning management system
//
//    Copyright (C) 2025 Ivan Matic, https://gradeyard.com
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


#ifndef _INCL_WI_SessionInformation_H
#define _INCL_WI_SessionInformation_H


namespace SII{

  class PublicFile{
  private:
    FMD::FileManager m;
    std::string externalFileCode;
    std::vector<long> ivn;
    std::string filePath;
    std::string fileInfoData;
    std::string fileExtension;
    std::string fileDataType;
    std::string fileName;
    std::string fcB="_fileCode_";
    std::string fcE="_/fileCode_";
    std::string fnmB="_fileName_";
    std::string fnmE="_/fileName_";
    std::string fPB="_filePath_";
    std::string fPE="_/filePath_";
    std::string fnB="_fileNumber_";
    std::string fnE="_/fileNumber_";
    std::string fEB="_fileExtension_";
    std::string fEE="_/fileExtension_";
    std::string fDatTB="_fileDataType_";
    std::string fDatTE="_/fileDataType_";
    std::string fUCB="_user_";
    std::string fUCE="_/user_";
    std::string fTB="_created_";
    std::string fTE="_/created_";
    long initialized;
    int initializeNewFile(const std::string &, const std::string &, const std::string &, const std::string &);
    int initializeExistingFile(const std::string &);
  public:
    PublicFile();
    int init(const std::string & = "!*new!*", const std::string & = "pdf", const std::string & = "!noUser!", const std::string & ="");
    long isInitialized() const;
    std::string getFilePath() const;
    std::string getFileName() const;
    std::string getFileInfoData() const;
    std::string deleteFile();//WARNING: don't call this method directly. Let the SessionInformation make the call because it will check whether it is allowed to delete file
  };
  struct ExamAttributes{
  public:
    std::string textForRespReceiver;
    std::vector<std::vector<long> > possibleVersions;
    std::vector<std::string> problemLabels;
    long numFilesAllowed;
    std::string deadlineText;
    std::string msDoc;
  };
  class SessionInformation{
  private:
    std::string DEBUGGING_ADDITIONS="";
    std::string cookie;
    std::string option;
    MPTI::MainText header;
    MPTI::MainText footer;
    MPTI::MainText mainText;
    RTI::Response mainRespRec;
    std::string dHN;
    std::string dFN;
    std::string dfPage;
    std::string sName;
    std::string sURL;
    std::string showLogInLink; 
    std::string indicator_encryptIP;
    std::string approvedGuests;
    long cntrEIdI0=51;
    long cntrEIdI1=79;
    long cntrEIdIZ=3;
    std::string str_backups_IfCalledFor;
    std::string str_pdfSummary_IfCalledFor;
    std::string s_sysDataB="_systemData!!_";
    std::string s_sysDataE="_/systemData!!_";
    std::string s_tDataB="_textData!!_";
    std::string s_tDataE="_/textData!!_";
    std::string s_createdB="_created!!_";
    std::string s_createdE="_/created!!_";
    std::string s_modifiedB="_modified!!_";
    std::string s_modifiedE="_/modified!!_";
    std::string s_createdByB="_createdBy!!_";
    std::string s_createdByE="_/createdBy!!_";
    std::string s_modifiedByB="_modifiedBy!!_";
    std::string s_modifiedByE="_/modifiedBy!!_";
    std::string s_permissionStringB="_permissionString!!_";
    std::string s_permissionStringE="_/permissionString!!_";
    std::string s_docTypeStringB="_documentType!!_";
    std::string s_docTypeStringE="_/documentType!!_";
    std::string e_startOfList="st";
    std::string e_sortCriterion="sc";
    std::string e_pageName=MWII::GL_WI.get_e_parPage();
    std::string e_respRecName=MWII::GL_WI.get_e_respRecReqRT();//="rrc";
    std::string e_redirectOverwrite="rdo";
    std::string e_redirectToForward="rdf";
    std::string e_userLogin="userLogIn";
    std::string e_passwCh="passwCh";
    std::string e_respSubmit=MWII::GL_WI.get_e_respSubmitReqRT(); //="respSubmit";
    std::string e_editReq="er";
    std::string e_editMessReq="emr";
    std::string e_editCouasReq="ecr";
    std::string e_verReq="ver";
    std::string e_messToEdit="mei";
    std::string e_messEditSubmission="msd";
    std::string e_newTextWithMessage="nmt";
    std::string e_respRecThatWillGradeCouas="rrg";
    std::string e_confirmationOfComfortableUserEdit="ccu";
    std::string m_logIn="yes";
    std::string m_addModFileNew="new";
    std::string m_addModFileEdit="edit";
    std::string m_addModFileDelete="delete";
    std::string m_respSubmit=MWII::GL_WI.get_e_respSubmitSuccRT();//="yes";
    std::string m_logOut="no";
    std::string m_editReqY="y";
    std::string m_editReqR="r";
    std::string m_editReqW="w";
    std::string startOfList;
    std::string e_systemDataDisplayRequest="sysData";
    std::string e_debEnvVarRequest="debuggingEnvVar";
    std::string e_debModeRequest="debuggingMode";
    std::string e_responseReceiverDisplayRequest="rdd";
    std::string e_addModFileReq="amF";
    std::string e_addModFileCodeReq="fReq";
    std::string e_addModFileModifyInfo="fInfo";
    std::string e_addModFileFilePar="sbF";
    std::string e_searchFor="s1";
    std::string e_replaceWith="r1";
    std::string s_deb_correctDebuggingEnvVarReq="eFel223";
    std::string s_deb_correctDebuggingModeReq="dFel223";
    std::string s_notFound="notFound";
    std::string pageRequested;
    std::string respRecRequested;
    std::string respSubmitted;
    std::string sysDataRequested;
    std::string debuggingEnvVarRequest;
    std::string debuggingModeRequest;
    std::string addModFileReq;
    std::string addModFileCodeReq;
    std::string addModFileModifyInfo;
    std::string idOfMessageWhoseEditWasSubmitted;
    std::string newTextOfMessage;
    std::vector<std::string> envVariables;
    long loginActionIndicator;// 1-user wants to log in; 0- no change of status; -1 - user wants to log out
    long loginStatusIndicator;// 1 - user is logged in; 0 - user is logged out
    long loginFailedIndicator;// 1 - username/password supplied but wrong ; 0 - username/password correct, or no log in attempted
    std::string currentCookie;
    long indicatorInitialized;
    int indicatorRespRecInitialized;
    long indicatorFormResponded; 
    long indicatorFileReceived;
    long indicatorFileCanBeAccepted;
    PSDI::SessionData psd;
    void updateRespMapToProperlyAccountForBothGraderCommentsAndPoints(const RTI::ResponderInfo&);
    int updateGradesFromResponse();
    std::vector<std::vector<std::string> > respInOrder;
    int uploadToServer(const cgicc::Cgicc & , const std::string & , const std::string & );//WARNING: this one does not check for permissions
    ExamAttributes attributesFromRespRec(const std::string & ,const std::string & ) const;
    int checkWhetherSingleProblemExists(const std::string &, TMD::MText& ) const;
    long checkWhetherAllProblemsExist(const std::vector<std::string> & ) const;
    ExamAttributes newExamFromTemplate(const std::string &, const std::pair<std::vector<std::string>,std::string>&,  const std::string & );
    std::string enrollExistingStudentsToExam(const ExamAttributes & , const std::string & ,const std::string & );
    std::string genExamTemplate(const std::string &,const std::pair<std::vector<std::string>,std::string> &, const std::string &);
    std::string updateExamDocument(const std::string &,const std::string &,const std::string &);
    std::string createRRBackup(const std::string& , const std::string & ) const;
    std::string treatRuleCommands(const std::string& ,const std::string &);
    SPREPF::StatData prepareStatData() const;
    std::map<std::string,std::string> mapNamesRawTexts(const std::set<std::string>& );
    std::string getOfflineAutograderData(const std::string& );
    TMF::Timer timeToGenerateWebsite;
  public:
    SessionInformation();
    void initSession(const cgicc::Cgicc & );
    long isInitialized() const;
    void changeMainText(const std::string & ="");
    MPTI::MainText getHeader() const;
    MPTI::MainText getFooter() const;
    MPTI::MainText getMainText() const;
    std::vector<std::vector<std::string> > getRespInOrder() const;
    std::string getResponse(const std::string & ) const;
    long countSubmittedFiles(const cgicc::Cgicc & ) const;
    void analyzeEnvVarsAndForms(const cgicc::Cgicc &);
    long isFormResponded() const;
    long isFileReceived() const;
    long canFileBeAccepted() const;
    int allowedToAssignPermit(const std::string &, const std::string &) const;
    int allowedToModifyUser(const std::string & ) const;
    int allowedToDeleteUser(const std::string & )const;
    int passedAntiSpam() const;
    void addToMainText(const std::string &);
    void addDebuggingMessagesIfInDebuggingMode();
    std::string preparePage(const std::string &);
    std::vector<std::string> envVars() const;
    //WARNING: At the moment any logged in user can place a file to the server
    // In addition, anyone who is solving a test can place files
    std::string placeNewFileToServer(const cgicc::Cgicc & , const std::string & , const std::string & , const std::string & ="fIn1", const std::string & = "");
    std::string updateExistingFile(const cgicc::Cgicc & , const std::string & , const std::string & , const std::string & ="fIn1", const std::string & = "");
    int allowedToDeleteFile(const PublicFile &) const;//WARNING not yet implemented- returns 1 only for root
    int allowedToModifyFile(const std::string &, const std::string &) const;//WARNING not yet implemented - returns 1 only for root
    int allowedToUploadNewFile(const std::string &) const; //WARING not yet implemented - returns 1 only for root
    std::string deleteFile(PublicFile &);
    std::string deleteSingleFile(const std::string &);
    std::string deleteFile(const std::string &);
    int allowedToCreateText() const;//WARNING: returns 1 only for root
    int allowedToModifyText(const std::string &) const;//WARNING: returns 1 only for root
    int allowedToDeleteText(const std::string &) const;//WARNING: returns 1 only for root
    std::string createText(const std::string &, const std::string &, const std::string & ="!*!", const std::string & ="!*!");
    std::string modifyText(const std::string &, const std::string &);
    std::string deleteText(const std::string &);
    std::string createCloneForGuest(const std::string &);
    int allowedToCreateCloneForGuest(const std::string &) const;
    std::string deleteCloneForGuest(const std::string &);
    int allowedToDeleteCloneForGuest(const std::string & ) const;
    int createOrDeleteWebsiteOrChangeRequestedPage();
    int allowedToCreateMessage(const std::string &) const;
    int allowedToModifyMessage(const std::string &,const std::string &) const;
    int allowedToCreateCouas(const std::string &) const;
    int allowedToModifyCouas(const std::string &,const std::string &) const;
    int allowedToCreateCert(const std::string &) const;
    int allowedToModifyCert(const std::string &) const;
    std::string createMessage(const std::string &, const std::string &, const std::string &);
    std::string modifyMessage(const std::string &, const std::string &);
    std::string deleteMessage(const std::string &, const std::string &);
    std::string createForum(const std::string &, const std::string &, const std::string &, const std::string & ="");
    std::string createSupportingPages(const std::map<std::string,std::string> & , const std::map<std::string,std::string> & , const std::map<std::string,std::string> & );
    std::string createInitialExams(const std::map<std::string,std::string> & , const std::string & );
    std::string createStandardCourse(const std::string &, const std::string &);
    std::string createCouas(const std::string &, const std::string &, const std::string &);
    std::string modifyCouas(const std::string &, const std::string &);
    std::string deleteCouas(const std::string &, const std::string &);
    std::string createCert(const std::string &, const std::string &);
    std::string modifyCert(const std::string &, const std::string &);
    std::string deleteCert(const std::string &);
    std::string createGradingForCourse(const std::string &, const std::string &);
    int allowedToCreateRespRec() const;//WARNING: returns 1 only for root
    int allowedToModifyRespRec(const std::string &, const std::string &) const;
    //WARNING: returns 1 for root or if the second argument is "yes"
    // the first argument the code of the ResponseReceiver and the second is "yes" if the modification
    // is due to form being submitted
    std::string enhanceTheCommandDueToComfUserEdit(const std::string &) const;
    std::string uploadFilesFromResponseReceiver(const cgicc::Cgicc & ,const std::string & ,const std::string &, const std::vector<std::string> & , const RTI::Response & , const std::string &);
    std::string addStudentsToExam(const std::string &, const std::string &);
    std::string generateExam(const std::string &, const std::string &);
    std::string generatePdfsForExam(const std::string &, const std::string &);
    std::string updateIndividualVersionsOfExam(const std::string &, const std::string &);
    std::string createExamBackupText(const std::string &, const std::string &);
    std::string distributeExamToStudents(const std::string &, const std::string &);
    std::string createRespRec(const std::string &, const std::string &);
    std::string modifyRespRec(const std::string &, const std::string &, const std::string &);
    std::string deleteSingleRespRec(const std::string &);
    std::string deleteRespRec(const std::string &);
    std::string createUser(const std::string &,const std::string &,const std::string &,const std::string &,const std::string &,const std::string &,const std::string &,const std::string &,const std::string &);
    std::string modifyUser(const std::string &,const std::string &,const std::string &,const std::string &,const std::string &,const std::string &,const std::string &,const std::string &);
    std::string deleteSingleUser(const std::string &);
    std::string deleteUser(const std::string &);
    int allowedToAddToHierarchy(const std::string &, const std::string &) const;
    int allowedToRemoveFromHierarchy(const std::string &, const std::string &) const;
    int allowedToAssignGraders(const std::string &, const std::string & ) const;
    int allowedToEditTimer(const std::string &, const std::string & ) const;
    int allowedToEnrollStudents(const std::string &, const std::string & ) const;
    int allowedToBackup(const std::string & , const std::string &) const;
    std::string addToHierarchy(const std::string &, const std::string &);
    std::string removeFromHierarchy(const std::string &, const std::string &);
    std::string assignGraders(const std::string &, const std::string &);
    std::string editTimer(const std::string &, const std::string &);
    std::string createAccountsForStudents(std::vector<PASF::StudentData> & ,const std::string & );
    std::string submitEnrollmentReport(const std::string &, const std::string &, const std::vector<PASF::StudentData> & );
    std::string enrollStudents(const std::string &, const std::string &, const std::string &, const std::string &);
    std::string backupDBs(const std::string &, const std::string & ); 
    std::string analyzeRequestToChangePassword();
    std::string enrollStudentsInCouas(const std::string & , const std::vector<PASF::StudentData> & , const long & = 10 );
    std::string cookieText(const std::string & = "s", const std::string & ="deleted") const;
    std::string createRecoveryCommands();
    long loggedIn();
  };
}

#endif
