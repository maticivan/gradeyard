//    GradeYard learning management system
//
//    Copyright (C) 2022 Ivan Matic, https://gradeyard.com
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

#ifndef _INCL_WI_PSDI_SessionData_H
#define _INCL_WI_PSDI_SessionData_H

namespace PSDI{
  struct SessionData{
  public:
    WUD::User myWU;
    std::string my_un;
    std::string isRoot;
    std::string allowedToExecuteAll;
    std::string pEditReq;
    std::string respRecMode;
    std::string respRecFlag;
    std::string versionToLatex;
    std::string usrAgent;
    std::string remAddr;
    std::string reqMethod;
    std::string inFormSearch;
    std::string inFormReplace;
    std::string messEditCode;
    std::string myFirstName;
    std::string myLastName;
    std::string messEditReq;
    std::string sortCriterion;
    std::string couasEditReq;
    std::string rrgrader;
    std::string pageRequested;
    std::string passwordChangeRequested;
    std::string passwordChangeStatus;
    std::string comfUserEdit;

    std::string createStandardCourseMainDocName;
    std::string createStandardCourseSuccess;

    std::string queryAnswerPlaceHolder;

    std::map<std::string,std::string> respMap;
    std::map<std::string,std::string> encryptionMap;

    std::stack<std::string> recoveryOperationCommands;
    std::stack<std::string> recoveryOperationNames;

    int indChangeRespRecToPrintVersionOfCommonInClassExam;
    std::string probVersionsOfChangedRespRec;

    std::string pdfNameForInclassExam;
    std::string pdfBeforeProblems;
    std::string pdfAfterProblems;

    std::string masterKey;
    std::string respRecBackupText;
    std::vector<std::string> displayDaysInWeek;
    std::vector<std::string> displayMonthsInYear;
  };
}
#endif
