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


#ifndef _INCL_WI_StatText_H
#define _INCL_WI_StatText_H

namespace STI{

  class StatText:public APTI::AbstractText{
  protected:

    int ind_initSuccess;
    SD::Stat meSM;
    std::string myDocType;//can be pageStat, userStat, or rawStat
    std::string st_pageStat="pageStat";
    std::string st_userStat="userStat";
    std::string st_rawStat="rawStat";
    std::string modifyMe() const;
  public:
    StatText(const std::string & = "mainTextFirstPage", const std::string & = "no1117", const std::string & = "everyone");
    int initialize(const std::string & = "mainTextFirstPage", const std::string & = "no211", const std::string & = "everyone");
    int isInitialized() const;
  };
  std::string statAnalysis(const std::string & );
  long getTotalVisits(const std::string & , const std::string & );
}
#endif
