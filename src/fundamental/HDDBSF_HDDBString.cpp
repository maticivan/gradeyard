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


#ifndef _INCL_MYHDString_CPP
#define _INCL_MYHDString_CPP

namespace HDDBSF{
  std::string secureForStorage(const std::string & source, int performanceIndicator=1){
    if(performanceIndicator==1){
      return SF::findAndReplace(source,"*!.","");
    }
    return SF::findAndReplace(source,"*!.",GF::GL_HIDING_STRING_HDDBRF);
  }
  std::string unpackFromStorage(const std::string & source, int performanceIndicator=1){
    if(performanceIndicator==1){
      return source;
    }
    return SF::findAndReplace(source,GF::GL_HIDING_STRING_HDDBRF,"*!.");
  }
  class String{
  private:
      std::string mainData;
  public:
      String(const std::string & = "noData");
      void setMainData(const std::string &);
      std::string getMainData() const;
      long operator<(const String &) const;
      void loadFromString(const std::string&);
      std::string putIntoString() const;
  };
  String::String(const std::string & _mD){
      mainData=_mD;
  }
  void String::setMainData(const std::string &_mD){
      mainData=_mD;
  }
  std::string String::getMainData() const{
      return mainData;
  }
  long String::operator<(const String & _oth) const{
      if(mainData<_oth.mainData){
        return 1;
      }
      return 0;
  }
  void String::loadFromString(const std::string& s){ 
      mainData=unpackFromStorage(s);
  }
  std::string String::putIntoString() const{
    return secureForStorage(mainData);
  }
}
#endif
