//    GradeYard learning management system
//
//    Copyright (C) 2021 Ivan Matic, https://gradeyard.com
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

#ifndef _INCL_WI_DEBUGGINGMESSAGES_CPP
#define _INCL_WI_DEBUGGINGMESSAGES_CPP

namespace DEBMF{
  class DebHandler{
  private:
    std::vector<std::string> allMs;
  public:
    DebHandler();
    void addMessage(const std::string &);
    std::string prepareAllMessages(const std::string & ) const;
    void clearMessages();
  };

  DebHandler::DebHandler(){
    allMs.clear();
  }
  void DebHandler::clearMessages(){
    allMs.clear();
  }
  void DebHandler::addMessage(const std::string & _m){
    allMs.push_back(_m);
  }
  std::string DebHandler::prepareAllMessages(const std::string & options) const{
    std::string fR;
    if(options=="no"){return "";}
    long sz=allMs.size();
    fR+="<TABLE BORDER = \"0\">";
    for(long i=0;i<sz;++i){
      if( (options!="timeOnly") || (i==sz-1)){
        fR+="<TR><TD>"+std::to_string(i+1)+"</TD><TD>";
        fR+=allMs[i];
        fR+="</TD></TR>\n";
      }

    }
    fR+="</TABLE>";
    return fR;
  }



  template<class T>
  void printVectDebug(const std::vector<T> & v){
    long sz=v.size();
    if(sz>0){
      for(long i=0;i<sz;++i){
        std::cout<<v[i]<<" ";
      }
      std::cout<<"\n";
    }
  }
}
#endif
