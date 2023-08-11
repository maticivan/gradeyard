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


#ifndef _INCL_COMMANDCENTER_H
#define _INCL_COMMANDCENTER_H


namespace CCI{

  class Command{
  private:
    std::string type;
    std::vector<std::string> arguments;
    long initialized;
    std::string executeDeleteFile(SII::SessionInformation &);
    std::string executeCreateText(SII::SessionInformation &);
    std::string executeModifyText(SII::SessionInformation &);
    std::string executeDeleteText(SII::SessionInformation &);
    std::string executeCreateMessage(SII::SessionInformation &);
    std::string executeModifyMessage(SII::SessionInformation &);
    std::string executeDeleteMessage(SII::SessionInformation &);
    std::string executeCreateForum(SII::SessionInformation &);
    std::string executeCreateStandardCourse(SII::SessionInformation &);


    std::string executeCreateCouas(SII::SessionInformation &);
    std::string executeModifyCouas(SII::SessionInformation &);
    std::string executeDeleteCouas(SII::SessionInformation &);
    std::string executeCreateGradingForCourse(SII::SessionInformation &);

    std::string executeCreateCert(SII::SessionInformation &);
    std::string executeModifyCert(SII::SessionInformation &);
    std::string executeDeleteCert(SII::SessionInformation &);

    std::string executeCreateRespRec(SII::SessionInformation &);
    std::string executeModifyRespRec(SII::SessionInformation &);
    std::string executeDeleteRespRec(SII::SessionInformation &);

    std::string executeAddStudentsToExam(SII::SessionInformation &);
    std::string executeGeneratePdfsForExam(SII::SessionInformation &);
    std::string executeUpdateVersionsForExam(SII::SessionInformation &);
    std::string executeExamBackupText(SII::SessionInformation &);
    std::string executeGenerateExam(SII::SessionInformation &);
    std::string executeDistributeExamToStudents(SII::SessionInformation &);

    std::string executeDeleteUser(SII::SessionInformation &);
    std::string executeCreateUser(SII::SessionInformation &);
    std::string executeModifyUser(SII::SessionInformation &);
    std::string executeAddToHierarchy(SII::SessionInformation &);
    std::string executeRemoveFromHierarchy(SII::SessionInformation &);

    std::string executeEditTimer(SII::SessionInformation &);
    std::string executeAssignGraders(SII::SessionInformation &);
    std::string executeEnrollStudents(SII::SessionInformation &);
    std::string executeCreateBackupText(SII::SessionInformation &);
    std::string executeDeleteWebsiteForUser(SII::SessionInformation &);
    std::string executeCreateWebsiteForUser(SII::SessionInformation &);
  public:
    Command(const std::string & = "");
    void initFromString(const std::string & = "");
    // We assume that initialization string for a single command looks like this:
    // _nc***_<commandType>_/nc***_ _nc***_arg0_/nc***_ _nc***_arg1_/nc***_ ... _nc***_arg[n-1]_/nc***_
    void deInit();
    int allowedToExecute(SII::SessionInformation &) const;
    int syntaxCorrect(const std::string &, const std::vector<std::string> & ) const;
    std::string commandTextForDebugging() const;

    std::string executeCommand(SII::SessionInformation &);
  };
}

#endif
