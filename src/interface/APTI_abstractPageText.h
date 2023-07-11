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


#ifndef _INCL_WI_AbstractText_H
#define _INCL_WI_AbstractText_H

namespace APTI{
  std::string GL_studentsAllowedToExecuteCodeOnPublicTestCases="yes";
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
    std::string v_regularText="regularText";
    std::map<std::string, FHI::InputForm> allForms;
    std::string s_root="root";
    std::string s_everyone="everyone";
    std::string s_notFound="notFound";
    std::string s_commands="commands";
    std::string e_redirectToForward="rdf";
    std::string val_sysDataReq_YES="yes";
    std::string val_sysDataReq_RAW="raw";
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
    std::string createSolvingInvitation(const PSDI::SessionData &,const std::string &,const std::string &,const std::string & ) const;
    std::string createCouasDisplay(const PSDI::SessionData &,const std::string &);
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
    std::string createLinkToText(const std::string &, const std::string & = "page", const std::string & ="") const;
    std::string createLinkToMessage(const std::string &, const std::string & , const std::string &) const;
    std::string createLinkToCouas(const std::string &, const std::string & ,const std::string &) const;
    std::string createLinkToExecuteBackup(const std::string &, const std::string &) const;
    std::string createLinkToExpandStat(const std::string &, const long & = 1) const;
    std::string createLinkToFile(const std::string &, const std::string &, const std::string &, const std::string &) const;
    std::string createLinkToResponseReceiver(const std::string &, const std::string &) const;
    std::string createLinkToSwitchSortCriterion(const std::string & , const std::string & , const std::string & , const std::string &) const;
    std::string createListOfUsers(const PSDI::SessionData & ,const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    std::string createListOfTexts(const PSDI::SessionData & ,const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    std::string createListOfFiles(const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    std::string createListOfMessages(const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    std::string createListOfCoursesAssignments(const PSDI::SessionData & , const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    std::string createListOfBackups(const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    std::string createListOfStatItems(const PSDI::SessionData &, const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    std::string createListOfFStatItems(const std::string & , const std::string & ) const;//WARNING: not fully implemented - works only for root
    std::string createListOfResponses(const PSDI::SessionData &, const std::string &, const std::string &) const;//WARNING: not fully implemented - works only for root
    std::string forumScroller(const PSDI::SessionData & , long & , long & , const long & , const long & , const std::string &) const;
    std::string pageAttributesForForumScroller(const PSDI::SessionData &) const;
    std::string createPowerMessageEdit(const PSDI::SessionData & ,const std::string &);
    std::string createPowerCouasEdit(const PSDI::SessionData & ,const std::string &);
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
    void updateLogInBar(const std::string &);
    std::stack<std::pair<std::string,std::string> > secondMenuStack(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &) const;
    void updateSecondMenuBar(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &);
    std::stack<std::pair<std::string,std::string> > thirdMenuStack(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &) const;
    void updateThirdMenuBar(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &);
    void updateMainMenuBar(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &);
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
    int allowedToModifyText(const PSDI::SessionData &, const std::string &, const std::string &, const std::string &) const;
    int allowedToDisplayText(const PSDI::SessionData &, const std::string &, const std::string & ="mainTextPosition") const;
  };
}
#endif
