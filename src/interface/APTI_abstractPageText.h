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


#ifndef _INCL_WI_AbstractText_H
#define _INCL_WI_AbstractText_H

namespace APTI{
  std::string GL_studentsAllowedToExecuteCodeOnPublicTestCases="yes";
  struct SyntaxStrings{
  public:
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
    std::string s_individualPermissionB="_permission_";
    std::string s_individualPermissionE="_/permission_";
    std::string s_userOrGroupB="_userOrGroup_";
    std::string s_userOrGroupE="_/userOrGroup_";
    std::string s_insertB="_insert_";
    std::string s_insertE="_/insert_";
    std::string s_insSepInB="_n*_";
    std::string s_insSepInE="_/n*_";
    std::string s_listFromDB="listFromDB";
    std::string s_textAreaReqField="textAreaField";
    std::string s_textInputReqField="textInputField";
    std::string s_radioButtonsField="radioButtonsField";
    std::string s_checkBoxesField="checkBoxesField";
    std::string s_antiSpamChallengeField="antiSpamChallenge";
    std::string s_fileReqField="fileRequestField";
    std::string s_formInitialization="formInitialization";
    std::string s_formPlacement="formPlacement";
    std::string s_formInsertRedirect="redirectRequest";
    std::string s_logInLink="logInLink";
    std::string s_cloneInvitation="invitationToCloneWebsite";
    std::string s_statAnalysis="statAnalysis";
    std::string s_cRestore="commandRestoreFromBackup";
    std::string s_subText="text";
    std::string s_message="message";
    std::string s_couas="courseAssignment";
    std::string s_cert="certificate";
    std::string s_respRecStatus="responseReceiverStatus";
    std::string s_codeTest="codeTest";
    std::string s_codeTestInNotes="codeTestInNotes";
    std::string s_internalLink="internalLink";
    std::string s_buttonLink="buttonLink";
    std::string s_svgAdd="svgAdd";
    std::string s_invitationToSolve="invitationToSolve";
    std::string s_userPermits="userPermits";
    std::string s_itemTable="itemTable";
    std::string s_answerToQuery="answerToTheQuery";
    std::string s_fileForm="fileForm";
    std::string v_regularText="regularText";
    std::string s_root="root";
    std::string s_everyone="everyone";
    std::string s_notFound="notFound";
    std::string s_commands="commands";
    std::string e_redirectToForward="rdf";
    std::string val_sysDataReq_YES="yes";
    std::string val_sysDataReq_RAW="raw";
    std::string s_gradeLinkInResp_QRTB="_grd*|_";
    std::string s_gradeLinkInResp_QRTE="_/grd*|_";
    std::string s_respLinkInGrade_QRTB="_rsp*|_";
    std::string s_respLinkInGrade_QRTE="_/rsp*|_";
    std::string s_nextQ_QRTB="_in*|_";
    std::string s_nextQ_QRTE="_/in*|_";
    std::string s_tFormulation_QRTB="_tx*|_";
    std::string s_tFormulation_QRTE="_/tx*|_";
    std::string s_solution_QRTB="_sl*|_";
    std::string s_solution_QRTE="_/sl*|_";
    std::string s_answer_QRTB="_aw*|_";
    std::string s_answer_QRTE="_/aw*|_";
    std::string s_points_QRTB="_pt*|_";
    std::string s_points_QRTE="_/pt*|_";
    std::string s_certificateDescCodeB="_cdCode*|_";
    std::string s_certificateDescCodeE="_/cdCode*|_";
    std::string s_certificateIdCodeB="_cidCode*|_";
    std::string s_certificateIdCodeE="_/cidCode*|_";
    std::string s_requestType_QRTB="_rq*|_";
    std::string s_requestType_QRTE="_/rq*|_";
    std::string s_radioButtons_QRTB="_rbAll*|_";
    std::string s_radioButtons_QRTE="_/rbAll*|_";
    std::string s_fileAllowed_QRTB="_fa*|_";
    std::string s_fileAllowed_QRTE="_/fa*|_";
    std::string s_autograderInfo_QRTB="_agr*|_";
    std::string s_autograderInfo_QRTE="_/agr*|_";
    std::string s_latexPrintingInstructions_QRTB="_lpi*|_";
    std::string s_latexPrintingInstructions_QRTE="_/lpi*|_";
    std::string s_gradingRules_QRTB="_gru*_";
    std::string s_gradingRules_QRTE="_/gru*_";
    std::string s_cAllowedTime_QRTB="_cAllT*|_";
    std::string s_cAllowedTime_QRTE="_/cAllT*|_";
    std::string s_cAbsEnd_QRTB="_cAbsEnd*|_";
    std::string s_cAbsEnd_QRTE="_/cAbsEnd*|_";
    std::string s_cTVersion_QRTB="_cTVers*|_";
    std::string s_cTVersion_QRTE="_/cTVers*|_";
    std::string s_iEnd_QRTB="_iEnd*|_";
    std::string s_iEnd_QRTE="_/iEnd*|_";
    std::string s_iTVersion_QRTB="_iTVers*|_";
    std::string s_iTVersion_QRTE="_/iTVers*|_";
    std::string s_label_QRTB="_lb*|_";
    std::string s_label_QRTE="_/lb*|_";
    std::string s_buttonLabel_QRTB="_blb*|_";
    std::string s_buttonLabel_QRTE="_/blb*|_";
    std::string s_importantInfo_QRTB="_iinf*|_";
    std::string s_importantInfo_QRTE="_/iinf*|_";
    std::string s_notStartedYet_QRTB="_notOpen*|_";
    std::string s_notStartedYet_QRTE="_/notOpen*|_";
    std::string s_notReadyForGrading_QRTB="_noGrading*|_";
    std::string s_notReadyForGrading_QRTE="_/noGrading*|_";
    std::string s_needsActivation_QRTB="_startMessage*|_";
    std::string s_needsActivation_QRTE="_/startMessage*|_";
    std::string s_statusOfTheForm_ARTB="_sf*|_";
    std::string s_statusOfTheForm_ARTE="_/sf*|_";
    std::string s_statusOfTheGrading_ARTB="_sgr*|_";
    std::string s_statusOfTheGrading_ARTE="_/sgr*|_";
    std::string s_formulRespRec_ARTB="_f*|_";
    std::string s_formulRespRec_ARTE="_/f*|_";
    std::string s_masterStatus_ARTB="_ms*|_";
    std::string s_masterStatus_ARTE="_/ms*|_";
    std::string s_deadlineRespRec_ARTB="_dl*|_";
    std::string s_deadlineRespRec_ARTE="_/dl*|_";
    std::string s_idInfo_ARTB="_iI*|_";
    std::string s_idInfo_ARTE="_/iI*|_";
    std::string s_solverUName_ARTB="_userName*|_";
    std::string s_solverUName_ARTE="_/userName*|_";
    std::string s_comment_ARTB="_gc*|_";
    std::string s_comment_ARTE="_/gc*|_";
    std::string s_numFilesAllowed_ARTB="_nf*|_";
    std::string s_numFilesAllowed_ARTE="_/nf*|_";
    std::string s_verSepBB="_v";
    std::string s_verSepEB="_/v";
    std::string s_verSepE="*|_";
    std::string s_indFileSep_ARTBB="_f";
    std::string s_indFileSep_ARTEB="_/f";
    std::string s_infFileSep_ARTBEE="*|_";
    std::string s_accessLogs_ARTB="_alo*|_";
    std::string s_accessLogs_ARTE="_/alo*|_";
    std::string s_accessLogsInnerSep_ARTB="_alIn*|_";
    std::string s_accessLogsInnerSep_ARTE="_/alIn*|_";
    std::string s_accessLogsNumDevs_ARTB="_alND*|_";
    std::string s_accessLogsNumDevs_ARTE="_/alND*|_";
    std::string s_timerRepl="_|*timer*|_";
    std::string s_tmSecRepl="_|*tmSeconds*|_";
  } GL_syntax;
  class AbstractText{
  protected:
    std::set<std::string> permitRead;
    std::set<std::string> permitWrite;
    std::string initText;
    std::string rawText;
    std::string tName;
    std::string tExternalId;
    std::string tCreated;
    std::string createdBy;
    std::string tModified;
    std::string modifiedBy;
    std::string documentType;
    std::string sysDataRequested;
    std::string myUserName;
    std::string sysDataRaw;
    std::map<std::string, FHI::InputForm> allForms;
    std::map<std::string,std::string> translationVarToVal;
    long subTextRecursionDepth=0;
    long maxRecursionDepth=10;
    std::map<std::string,std::string> getTranslationMap(const std::string & ) const;
    std::string createSubText(const PSDI::SessionData &,const std::string &) const;
    std::string createCodeTest(const PSDI::SessionData &,const std::string &, const std::string &, const std::string &) const;
    std::string createCodeTestInNotes(const PSDI::SessionData &,const std::string &,const std::string &,const std::string&);
    std::string createUserPermitInfo(const PSDI::SessionData &,const std::string &);
    std::string createRespRecStatusDisplay(const PSDI::SessionData & , const std::string & ) const;
    std::string createMessageDisplay(const PSDI::SessionData &,const std::string &);
    std::string checkIfTheUserIsTA(const PSDI::SessionData &, const std::string & ) const;
    std::string createSolvingInvitation(const PSDI::SessionData &,const std::string &,const std::string &,const std::string & ) const;
    std::string createCouasDisplay(const PSDI::SessionData &,const std::string &);
    std::string createCertDisplay(const PSDI::SessionData &,const std::string &);
    std::string createItemTable(const PSDI::SessionData &,const std::string &) const;
    std::string createInternalLink(const PSDI::SessionData &, const std::string &, const std::string &) const;
    std::string createTextAreaField(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &, const std::string &, const std::string & ="notFound", const std::string & ="notFound");
    std::string createTextInputField(const std::string &, const std::string &, const std::string &, const std::string &, const std::string &);
    std::string createRadioButtonsField(const std::string &, const std::string &, const std::string &, const std::string &, const std::string &, const std::string &);
    std::string createAntiSpamField(const std::string & , const std::string & , const std::string & , const std::string & , const std::string &);
    std::string createFileRequestField(const std::string &, const std::string &, const std::string &);
    std::string initializeForm(const std::string &, const std::string &);
    std::string placeFormInText(const std::string &,const std::string & ="0",const std::string & ="notFound");
    std::string setRedirectInfo(const std::string &);
    std::pair<std::string,std::string> createLogInLinkPair(const std::string & ) const;
    std::pair<std::string,std::string> createLinkPair(const std::vector<std::string> &, const std::vector<std::string> &,const std::string & ) const;
    std::pair<std::string,std::string> createLinkPair(const std::string & , const std::string & ,const std::string & ) const;
    std::stack<std::pair<std::string,std::string> > logInBarRaw(const std::string & ) const;
    std::string createLogInLink() const;
    std::string createCloneInvitation(const PSDI::SessionData &);
    std::string createRestoreCommand(const PSDI::SessionData &, const std::string &);
    std::string createStatAnalysisPage(const PSDI::SessionData &, const std::string &) const;
    std::pair<std::string,std::string> createLogOutPair(const std::string &) const;
    std::pair<std::string,std::string> createModifyLinkPairUnsafe(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &, const std::string &, const std::string &) const;
    std::pair<std::string,std::string> createModifyLinkPair(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &, const std::string &, const std::string &) const;
    std::string createModifyLink(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &, const std::string &, const std::string &) const;
    std::pair<std::string,std::string> createCommandLinkPair(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &, const std::string &) const;
    std::string createCommandLink(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &, const std::string &) const;
    std::string createLinkForEditUser(const std::string &) const;
    std::string createLinkForEditUserAdvanced(const std::string &,const std::string &) const;
    std::string createLinkForEditUserExtraAdvanced(const std::string &,const std::string &) const;
    std::string createLinkToText(const std::string &, const std::string & = "page", const std::string & = "", const std::string & = "") const;
    std::string createLinkToMessage(const std::string &, const std::string & , const std::string &) const;
    std::string createLinkToCouas(const std::string &, const std::string & ,const std::string &) const;
    std::string createLinkToCertificate(const std::string &, const std::string & ,const std::string &, const std::string& = "") const;
    std::string createLinkToExecuteBackup(const std::string &, const std::string &) const;
    std::string createLinkToExpandStat(const std::string &, const long & = 0) const;
    std::string createLinkToFile(const std::string &, const std::string &, const std::string &, const std::string &) const;
    std::string createLinkToResponseReceiver(const std::string &, const std::string &) const;
    std::string createLinkToSwitchSortCriterion(const std::string & , const std::string & , const std::string & , const std::string &) const;
    std::string createListOfUsers(const PSDI::SessionData & ,const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    std::string createListOfTexts(const PSDI::SessionData & ,const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    std::string createListOfFiles(const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    std::string createListOfMessages(const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    std::string createListOfCoursesAssignments(const PSDI::SessionData & , const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    std::string createListOfCertificates(const PSDI::SessionData & , const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    std::string createListOfBackups(const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    std::string createListOfStatItems(const PSDI::SessionData &, const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    //std::string createListOfFStatItems(const std::string & , const std::string & ) const;//WARNING: not fully implemented - works only for root
    std::string createListOfResponses(const PSDI::SessionData &, const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    std::string forumScroller(const PSDI::SessionData & , long & , long & , const long & , const long & , const std::string &) const;
    std::string pageAttributesForForumScroller(const PSDI::SessionData &) const;
    std::string createPowerMessageEdit(const PSDI::SessionData & ,const std::string &);
    std::string createPowerCouasEdit(const PSDI::SessionData & ,const std::string &);
    std::string createPowerCertEdit(const PSDI::SessionData & ,const std::string &);
    virtual std::string modifyMe() const = 0;
    long allowedToInputCommands(const PSDI::SessionData &) const;//WARNING not implemented yet
  public:
    AbstractText(const std::string & = "mainTextFirstPage", const std::string & = "no1117", const std::string & = "everyone");
    std::set<std::string> readPermissions() const;
    std::set<std::string> writePermissions() const;
    std::string getRawText() const;
    std::string displayText(const PSDI::SessionData &, const std::string & = "mainTextPosition");
    std::string getTextName() const;
    std::string getTextExternalID() const;
    long getSubTextRecursionDepth() const;
    void setSubTextRecursionDepth(const long &);
    void addToText(const std::string &);
    std::stack<std::pair<std::string,std::string> > secondMenuStack(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &) const;
    std::stack<std::pair<std::string,std::string> > thirdMenuStack(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &) const;
    void updateLogInAndMenuBars(const PSDI::SessionData &, const std::string& , const std::string &);
    void updateFooterBar(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &);
    int rawSysDataAllowed(const PSDI::SessionData  &) const;//WARNING not implemented yet
    int sysDataAllowed(const PSDI::SessionData  &) const;//WARNING not implemented yet
    std::string groupsInPermission(const std::set<std::string> & ) const;
    std::string nicelyFormattedSystemData(const PSDI::SessionData  &) const;
    std::string sysDataIfNeededAndAllowed(const PSDI::SessionData  &) const;
    std::string evaluateInsert(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &);
    std::string treatInserts(const PSDI::SessionData &, const std::string &,const std::string &, const std::string &);
    std::string prepareProblem(const PSDI::SessionData  & , const std::string &  ) const;
    std::string convertVariablesToValues(const PSDI::SessionData  & ,const std::string &) const;
    std::string prepare(const PSDI::SessionData &, const std::string & , const std::string & = "mainTextPosition");
    std::string createListFromDB(const PSDI::SessionData &, const std::string &,  const std::string &, const std::string &) const;
    std::string displayFileForm(const PSDI::SessionData &, const std::string &,  const std::string &, const std::string & = "na", const std::string & = "na");
    int allowedToModifyText(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &) const;
    int allowedToDisplayText(const PSDI::SessionData &, const std::string &, const std::string & ="mainTextPosition") const;
    int allowedToMakePDFForCertificate(const PSDI::SessionData &, const std::string&) const;
    std::string pdfForCertificate(const PSDI::SessionData &,const std::string & ,const std::string&,const std::string&);
  };
}
#endif
