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


#ifndef _INCL_EMAI_ErrorMessageAnalysis_CPP
#define _INCL_EMAI_ErrorMessageAnalysis_CPP
namespace EMAI{
  std::string removeFileNames(const std::string& in, const std::string &extension){
    std::string inRev=SF::reverseString(in);
    std::string extRev=SF::reverseString("."+extension);
    long pos;std::pair<std::string,int> allD;
    pos=0; allD=SF::extractAndReplace(inRev,pos,extRev," ",0,"YPPCeliFniam\n");
    while(allD.second==1){
      inRev=allD.first;
      pos=0; allD=SF::extractAndReplace(inRev,pos,extRev," ",0,"YPPCeliFniam\n");
    }
    inRev=SF::reverseString(inRev);
    inRev=SF::findAndReplace(inRev,"mainFileCPPY","mainFile."+extension);
    return inRev;
  }
  std::string adjustIfForbiddenWordWasUsed(const std::string& rawError){
    if(rawError==SF::findAndReplace(rawError,CAGI::GL_Obf.secretOpenTag,"")){
      return rawError;
    }
    std::string rAtt=rawError;
    rAtt=SF::findAndReplace(rAtt,"\'"+CAGI::GL_Obf.secretOpenTag,"");
    rAtt=SF::findAndReplace(rAtt,"\'"+CAGI::GL_Obf.secretCloseTag,"");
    std::vector<std::string> forbidden=SF::stringToVector(rAtt,CAGI::GL_Obf.secretOpenTag,CAGI::GL_Obf.secretCloseTag);
    long sz=forbidden.size();
    if(sz<1){
      return rawError;
    }
    std::set<std::string> forbSet;
    for(long i=0;i<sz;++i){
      forbSet.insert(forbidden[i]);
    }
    std::string finalError;
    std::string currentError;
    std::set<std::string>::const_iterator it=forbSet.begin();
    while(it!=forbSet.end()){
      currentError=*it;
      if(currentError=="seqNotAllowed"){
        currentError=CAGI::GL_Obf.sequencesNotAllowedExplanation;
      }
      else{
        currentError=CAGI::GL_Obf.wordNotAllowed+currentError;
      }
      finalError+=currentError;
      finalError+="\n";
      ++it;
    }
    return finalError;
  }
  std::string explainCPPErrorMessage(const std::string& rawError){
    return "";
  }
}
#endif
