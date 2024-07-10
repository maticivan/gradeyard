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


#ifndef _INCL_WI_Response_H
#define _INCL_WI_Response_H
namespace RTI{
  class PossibleStatusOfTheForm{
  private:
    std::set<std::string> psForm;
    std::set<std::string>::const_iterator it,itE;
  public:
    PossibleStatusOfTheForm();
    std::string checkForLegalStatus(const std::string &);
  } GL_FORM_POSSIBLE_STATUS;
  PossibleStatusOfTheForm::PossibleStatusOfTheForm(){
    psForm.insert("nyo");//not yet open - do not show the form nor the answers
    //show form and accept answers
    psForm.insert("awd");//accept answers before deadline
    psForm.insert("a");//accept answers
    //show only questions
    psForm.insert("q");
    //show submissions and solutions
    psForm.insert("s");//show submissions of the user and official solutions
    psForm.insert("p");//show submissions of the user and points of the grader (but not comments)
    psForm.insert("c");//show submissions of the user and comments of the grader (incl. points)
    psForm.insert("sc");//both s and c
    psForm.insert("cs");//both s and c
    psForm.insert("sp");//both s and p
    psForm.insert("ps");//both s and p
    itE=psForm.end();
  }
  std::string PossibleStatusOfTheForm::checkForLegalStatus(const std::string &ch){
    it=psForm.find(ch);
    if(it==itE){
      return "no";
    }
    return "yes";
  }
  struct CodeAutoGraderInfo{
  public:
    std::string language, officialSource,userSource,codeToEmbed,messageDidNotCompile,asmRules,compilerFlags;
    std::vector<std::string> inputTestCases,revealTestCasesAfterGrading,publicTestCases,
    publicRevealTestCasesAfterGrading, includes,dbIncludes,  forbiddenStrs;
    std::vector<double> pointsTestCases,publicPointsTestCases;
  };
  struct SingleQuestionInfo{
  public:
    long num;
    std::string QNum, formulation, officialSolution, officialAnswer, maxPoints,
                displType, allChoicesSt, fileAllowed, userSolution, userAnswer,
                userPointsEarned, graderComment, grCommentDisplay,rawComment,rawScore;
    std::string scoreForDisplay;
    double scoreForCalculations;
    CodeAutoGraderInfo autoGraderCodeData;
    std::map<std::string,PSDI::GradingRule> gRulesMap;
    long acceptResp, displaySol, displayPoints, displayComments, displayQs;
  };
  struct ProblemCommentsAndScores{
  public:
    std::map<long,std::string> completionProgress;
    std::map<long,std::string> scoringProgress;
  };
  struct ResponderInfo{
  public:
    std::string docName;
    std::vector<std::string> fileInfoV;
    std::string statusOfTheForm;
    std::string formulationDocument;
    std::string idInfoData;
    std::string solverUserName;
    std::string exitStatus;// the function infoFromResponseText will set this to either "badForm" or "ok"
    std::string prefixForExamPDF;
    std::string addNameIndicator;
    std::string pdfTopOfThePage;
    std::set<std::string> pdfNewPageSeparators;
    long acceptResp;
    long acceptGrade;
    long displaySol;
    long displayPoints;
    long displayComments;
    long displayQs;
    std::string documentType;
    std::string gradingStatus;
    std::map<std::string,std::string> gradersComments;
    std::set<std::string> questionLabels;
    std::map<std::string,std::string> questionVersions;
    std::map<std::string,std::string> submittedAnswers;
    std::map<std::string,std::string> maxPoints;
    std::map<std::string,std::string> completeProblemDocs;
    std::map<std::string,std::map<std::string,PSDI::GradingRule> > gradingRules;
    int indicatorOfPOST;
    std::string commAllowedTime;
    std::string commAbsoluteEnd;
    long commTimerVersion;
    std::string indEndingTime;
    long indTimerVersion;
    long userCanTakeActionAndActivateExam;
    std::string potentialEndingTimeIfExamStartsNow;
    long actRequest;
    long remainingSeconds;
    long roundingTimeSec;
    std::string certificateDescriptionCode;
    std::string certificateIdCode;
    std::string debuggingPrint() const;
  };
  struct LocationOfDocuments{
  public:
    std::string userName;
    std::string testLocation;
    std::string gradeLocation;
    std::string studentEmail;
    std::string studentName;
    std::string graderUName;
    void getFromString(const std::string &);
    std::string putIntoString() const;
  };
  struct OfficialProblemData{
  public:
    std::string formulation="notFound";
    std::string solution="notFound";
    std::string answer="notFound";
    std::string points="notFound";
    std::string displayType="notFound";
    std::string choices="notFound";
    std::string filesAllowed="notFound";
    std::string autoGraderInfo="notFound";
    std::string latexPrintingInstructions="notFound";
    std::map<std::string,PSDI::GradingRule> gRules;
  };
  class Response:public APTI::AbstractText{
  protected:
    int ind_initSuccess;
    RMD::Response meRM;
    std::string myDocType;//can be responseToTest or gradeOfResponse
    std::string st_responseToTest="responseToTest";
    std::string st_gradeOfResponse="gradeOfResponse";
    std::string st_gradingColor="purple";
    RMD::Response responseDocumentForGrader;
    // when the object is a grading document
    //     then this attribute contains the responseDocument that has to be graded.
    // (when the object is a response document this attribute is irrelevant and not set to anything)
    int indRespDocForGraderCorrectlySet=0;
    RMD::Response gradingDocumentForReponse;
    // when the object is a response
    //     then this attribute contains the gradingDocument.
    // (when the object is a grading document this attribute is irrelevant and not set to anything)
    int indGradingDocForRespCorrectlySet=0;
    std::string rawTextOfResponseVisibleToGrader;
    // when the object is a grading document, this attribute is the rawText of the response document
    std::string e_formNameRT=MWII::GL_WI.get_e_formNameRT();
    std::string e_actNameRT=MWII::GL_WI.get_e_respSubmitReqRT()+"="+MWII::GL_WI.get_e_respSubmitSuccRT();//="respSubmit=yes";
    std::string labelOnTheSubmitButton;
    std::string instructionsOnTopOfTheForm;
    std::string timerCode;
    std::string messageUnavailableYet;
    std::string messageNoGradingYet;
    std::string messageStartExam;
    std::vector<std::string> mainFileInfoVector;
    long numFilesAllowed;
    std::map<std::string,std::vector<std::string> > userRTAnswMap;
    //key: question number; value = < solution,answer, pointsEarned, graders Comment>
    // std::map<std::string,std::vector<std::string> > formRTQsMap;
    //key: question number; value = <question, formulation, solution, answer, points, displayType, fileAllowed, autoGraderExecutionData,latexPrintingInstructions>
    std::map<std::string,OfficialProblemData > formRTQsMap;
    long numberOfDifferentDevicesThatAccessedTheForm;
    std::set<std::string> allDevicesThatAccessedTheForm;
    std::string modifyMe() const;
    std::string closestLegalRequest(const PSDI::SessionData &, const std::string &) const;
    std::vector<long> getStatusVector(const PSDI::SessionData & , const std::string & , const std::string & ) const;
    int analyzeAccessAndReportSuspiciousBehavior(const PSDI::SessionData &);
    std::string createStatusAndProgressLineTwoRowTable(const ProblemCommentsAndScores &, const double &, const ResponderInfo &) const;
    std::string createStatusAndProgressLine(const ProblemCommentsAndScores &, const double &, const ResponderInfo &, const long &) const;
    std::string createLinkToCertificate(const PSDI::SessionData &, const ResponderInfo &) const;
    std::string userAnswerDisplayInnerCard(const SingleQuestionInfo &, long &) const;
    std::string userAnswerDisplay(const SingleQuestionInfo &, long &, const int&) const;
    std::string singleProblemDisplay(const SingleQuestionInfo&, ProblemCommentsAndScores &, long &, double &) const;
    std::pair<std::string,int> singleProblemDisplayForGrader(const SingleQuestionInfo &, ProblemCommentsAndScores &, long &, double &) const;
  public:
    Response(const std::string & = "mainTextFirstPage", const std::string & = "no1117", const std::string & = "everyone");
    int initialize(const std::string & = "mainTextFirstPage", const std::string & = "no211", const std::string & = "everyone");
    std::string automaticGradingOfAllTests(const PSDI::SessionData  & ,const std::string & );
    std::string automaticGrading(const PSDI::SessionData &, const std::string &);
    std::string displayRespRec(const PSDI::SessionData &);
    std::string prepareDefaultRequest(const PSDI::SessionData &, const std::string &);
    std::pair<std::string,OfficialProblemData > getFormQData(const std::string &) const;
    std::pair<std::string,std::vector<std::string> > getUsrAVect(const std::string &) const;
    ResponderInfo infoFromResponseText(const PSDI::SessionData &, const std::string &, const int & = 0, const int & = 1);
    std::vector<LocationOfDocuments> getLocations(const PSDI::SessionData &) const;
    std::string indFileSepB(const long &) const;
    std::string indFileSepE(const long &) const;
    std::string fileLabelForHTML(const long &) const;
    std::string fileFieldForProblemInputForm(const std::string &, const long &) const;
    std::string fileForProblemInfo(const long &) const;
    std::string assignGraders(const PSDI::SessionData &, const std::string &);
    std::string editTimer(const PSDI::SessionData &, const std::string &);
    std::pair<std::string,std::string> getRespRecStatus(const PSDI::SessionData &) const;
    std::string changeResponseReceiverStatus(const PSDI::SessionData & , const std::string & , const std::string & );
    int isInitialized() const;
  };
}
#endif
