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


#ifndef _INCL_SANITIZE_AND_CHECK_CPP
#define _INCL_SANITIZE_AND_CHECK_CPP

namespace SACF{
  std::string sanitizeInFormSearchFor(const std::string & s){return "**sF!!";}
  int strictSafetyCheck(const std::string & s){
    long dsz=GF::GL_DANGERS.strings.size();
    long i=0;
    long danger=0;
    while((danger==0)&&(i<dsz)){
      if( s!=SF::findAndReplace(s,GF::GL_DANGERS.strings[i],"")){
        danger=1;
      }
      ++i;
    }
    return 1-danger;
  }
  int badCharactersAvoided(const std::string& in, const std::set<char> &m){
    int stillGood=1;
    long i=0;
    long sz=in.length();
    while((stillGood==1)&&(i<sz)){
      if(m.find(in[i])!=m.end()){
        stillGood=0;
      }
      ++i;
    }
    return stillGood;
  }
  int veryStrictSafetyCheck(const std::string & s){
    if(strictSafetyCheck(s)==0){return 0;}
    return IOF::legalFileName(s);
  }
  int lessStrictSafetyCheck(const std::string & s){
    if(strictSafetyCheck(s)==0){return 0;}
    return badCharactersAvoided(s,GF::GL_DANGERS.unacceptableCharactersInNames);
  }
  int acceptableUserName(const std::string& s){
    return badCharactersAvoided(s,GF::GL_DANGERS.unacceptableCharactersInUsernames);
  }
}

#endif
