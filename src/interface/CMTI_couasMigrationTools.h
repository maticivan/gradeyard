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


#ifndef _INCL_CMTI_couasMigrationTools_H
#define _INCL_CMTI_couasMigrationTools_H

namespace CMTI{   
  struct Student{
  public:
    std::string first;
    std::string last;
    std::string username;
    std::string email;
    int operator<(const Student& ) const;
  }; 
  struct RespRecNameComponents{
    int success;
    std::string examAbbreviation;
    std::string examNumber;
    std::string extension;
  };
  struct DocumentData{
    int success;
    std::string semester;
    std::string courseNameAndSection;
    std::vector<RespRecNameComponents> homeworks;
    std::vector<RespRecNameComponents> quizzes;
    std::vector<RespRecNameComponents> midterms;
    std::vector<RespRecNameComponents> finals;
    std::set<Student> students;
  };     
}
#endif
