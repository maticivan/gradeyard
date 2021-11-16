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


#ifndef _INCL_WI_GradeEvaluators_CPP
#define _INCL_WI_GradeEvaluators_CPP

namespace GEI{
  std::string GEvalData::debugPrinting() const{
    std::string fR="";
    fR+="name: "+name+"<BR>";
    fR+="type: "+type+"<BR>";
    fR+="Evaluation formulas:<BR>";
    std::map<std::string,std::string>::const_iterator it= evaluationFormulas.begin(),itE=evaluationFormulas.end();
    while(it!=itE){
      fR+=it->first+" -> "+it->second+"<BR>*************<BR>";
      ++it;

    }
    fR+="Grades to output:<BR>";
    std::set<std::string>::const_iterator itS= gradesToOutput.begin(),itSE=gradesToOutput.end();
    while(itS!=itSE){
      fR+=*itS+"<BR>*************<BR>";
      ++itS;
    }
    fR+="Explicit grades:<BR>";
    it= explicitGrades.begin();itE=explicitGrades.end();
    while(it!=itE){
      fR+=it->first+" : "+it->second+"<BR>*************<BR>";
      ++it;
    }
    fR+="End Debugging printing <BR>";
    return fR;
  }
}
#endif
