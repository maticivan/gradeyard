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

#ifndef _INCL_PISI_pythonIndentationSupport_CPP
#define _INCL_PISI_pythonIndentationSupport_CPP
namespace PISI{
  std::string stringBefore(const std::string& input, const std::string& target){
    std::string revInput=SF::reverseString(input);
    std::string revTarget=SF::reverseString(target);
    std::pair<std::string,int> allD;
    long pos;
    pos=0;allD=SF::extract(revInput,pos,revTarget,"\n");
    if(allD.second==0){
      return "";
    }
    return SF::reverseString(allD.first);
  }
  std::pair<long,std::string> takeSpaceAwayFromPyLine(const std::string& pyLine){
    long tabEquivalent=4;
    std::pair<long,std::string> pLPair;
    pLPair.first=0;
    long i=0;
    long sz=pyLine.length();
    long currentCounter=0;
    while((i<sz)&&((pyLine[i]==' ')||(pyLine[i]=='\t'))){
      if(pyLine[i]=='\t'){
        currentCounter=tabEquivalent;
      }
      else{
        ++currentCounter;
      }
      if(currentCounter==tabEquivalent){
        pLPair.first+=tabEquivalent;
        currentCounter=0;
      }
      ++i;
    }
    pLPair.first+=currentCounter;
    while(i<sz){
      pLPair.second+=pyLine[i];
      ++i;
    }
    return pLPair;
  }
  std::vector<std::pair<long,std::string> > takeSpaceAwayFromPyLine(const std::vector<std::string>& pyLines){
    std::vector<std::pair<long,std::string> > res;
    long sz=pyLines.size();
    res.resize(sz);
    for(long i=0;i<sz;++i){
      res[i]=takeSpaceAwayFromPyLine(pyLines[i]);
    }
    return res;
  }
  std::string embedPythonCode(const std::string& bigCode, const std::string& smallPiece){
    std::string result;
    std::string stBeforeEmbed=stringBefore(bigCode,"_*embedHere*_");
    std::pair<long,std::string> referenceLine=takeSpaceAwayFromPyLine(stBeforeEmbed+"_*embedHere*_");
    std::vector<std::string> pyLines=SF::getLines(smallPiece,'\n');
    std::vector<std::pair<long,std::string> > pyLinesTWSp=takeSpaceAwayFromPyLine(pyLines);
    long sz=pyLinesTWSp.size();
    if(sz<1){
      return "";
    } 
    long courtesyAdjustment=referenceLine.first-pyLinesTWSp[0].first;
    std::string codeToEmbed;
    for(long i=0;i<sz;++i){
      for(long j=0;j<pyLinesTWSp[i].first+courtesyAdjustment;++j){
        codeToEmbed+=" ";
      }
      codeToEmbed+=pyLinesTWSp[i].second;
      codeToEmbed+="\n";
    }
    result=SF::findAndReplace(bigCode,"_*embedHere*_","\n"+codeToEmbed);
    return result;
  }
}
#endif
