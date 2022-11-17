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

#ifndef _INCL_CAGI_CodeAutoGrader_H
#define _INCL_CAGI_CodeAutoGrader_H
namespace CAGI{
  struct Obfuscator{
  public:
    std::string uniqueRandomCode;
    std::string secretOpenTag;
    std::string secretCloseTag;
    std::string sequencesNotAllowedExplanation;
    std::string wordNotAllowed;
  } GL_Obf;
  struct GROneTestCase{
  public:
    std::string result, input, output, correct;
  };
  struct GradingResult{
  public:
    double numericScore;
    std::string score;
    std::string comment,shorterComment,errorMessage,language;
    std::vector<GROneTestCase> testCasesRes;
  };
  struct DockerCodeCounter{
  public:
    long remainingCodes;
  } GL_Code_Counter;//will be initialized in mainFunction.cpp
}
#endif
