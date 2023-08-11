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


#ifndef _INCL_WI_CouasElement_H
#define _INCL_WI_CouasElement_H

namespace CEI{
  class CouasListElAtt{
  private:
    std::string uName;
    std::string fName;
    std::string lName;
    std::string code;
    std::map<std::string,std::string> grades;
  public:
    int setFromString(const std::string &);
    std::string putIntoString() const;
    std::string putIntoString(const int &) const;
    std::string getUName() const;
    std::string getFName() const;
    std::string getLName() const;
    std::string getCode() const;
    std::string getGradeStr(const std::string & ) const;
    double getGradeNum(const std::string & ) const;
    void setUName(const std::string &);
    void setFName(const std::string &);
    void setLName(const std::string &);
    void setCode(const std::string &);
    void setGradeStr(const std::string &, const std::string &);
    int operator<(const CouasListElAtt &) const;
  };
  class CouasAttributes{
  public:
    std::string mUserN;
    std::string mFirstN;
    std::string mLastN;
    std::string navigationSt;
    std::string mCreated;
    std::string mText;
    std::string eType;
    std::string mElements;
    std::string mtTitle;
    std::string myCode;
    std::string bossCode;
    std::string bossName;
    std::string rawText;
    std::vector<CouasListElAtt> gradeData;
    std::map<std::string,std::string> grFormulas;
    std::map<std::string,std::string> grDisplays;
    std::string prepareGradeFormula(const PSDI::SessionData & ,const std::string &, const std::set<std::string> &, const long &) const;
    void sortGradeData(const std::string &);
    std::string setFromStringEasySteps(const std::string &);
    int setFromString(const std::string &,const std::map<std::string,CouasListElAtt> &);
    std::string putIntoString(const int &, const std::string &) const;
  };
  struct RawGrades{
  public:
    std::map<std::string,std::pair<std::vector<std::string>,std::string> > grStatusMap; 
    int certificateAddition;
  };
  class CouasElement{
  private:
    std::string cntrCouasCode="couasCode";
    long cntrCC0=19;
    long cntrCC1=14;
    long cntrCCZ=3;
    long existenceEstablishedBefore;
    std::string st_nil="nil";
    CouasAttributes coaa;
    std::map<std::string,std::string> displVarsThatNeedDBAccess(const std::map<std::string,std::string> & ,const std::map<std::string,std::string> & ) const;
  public:
    CouasElement(const PSDI::SessionData & , const std::string & = "", const std::string & = "");
    int initialize(const PSDI::SessionData & , const std::string & , const std::string & );
    std::string createMTEditLink(const std::string & , const std::string & , const std::string & , const std::string & ) const;
    std::string getRawText() const;
    std::string type() const;
    std::string title() const;
    std::string text() const;
    int gradeFromFormula() const;
    int allowedToRead(const std::string & ) const;
    int allowedToWrite(const std::string & ) const;
    int allowedToSeeAllGrades(const PSDI::SessionData & ) const;
    int allowedToExecuteAutomaticGrading(const PSDI::SessionData & ) const;
    int allowedToGrade(const PSDI::SessionData & ) const;
    std::string updateGrades(const PSDI::SessionData &, const std::map<std::string,std::string> &);
    void sortGradeData(const std::string &);
    std::string bossLink(const PSDI::SessionData &, const std::string &,const std::string &) const;
    std::string displayCouas(const PSDI::SessionData &, const std::string & , const std::string & ) const;
    std::vector<std::string> prepareTopLineWithSortingLinks(const PSDI::SessionData & , const std::vector<std::string> & ) const;
    std::string addStudents(const PSDI::SessionData &, const std::vector<PASF::StudentData> &);
    std::string gradeFromRespReceiver(const PSDI::SessionData &, const int &);
  };
  RawGrades rawGradesAndStatusFromRespReceiver(CouasAttributes & , const PSDI::SessionData & , const std::string &, const int & , const int &);
}
#endif
