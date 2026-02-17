//    GradeYard learning management system
//
//    Copyright (C) 2026 Ivan Matic, https://gradeyard.com
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

#ifndef _INCL_SAIOF_CPP
#define _INCL_SAIOF_CPP

#include "SSF_sortedSeq.cpp"
#include "MFRF_massiveFindReplace.cpp"
#include "BF_bijFunction.cpp"
#include "TMF_timer.cpp"
#include "DEBMF_debuggingMessages.cpp"
#include "HENCF_hashingEnc.cpp"
#include "VSF_vectorSearches.cpp"
#include "GF_globalInits.cpp"
#include "RNDF_randomCodesAndPermutations.cpp"
#include "SF_strings.cpp"
#include "TAF_tagAnalysis.cpp"
#include "HSF_html_strings.cpp"
#include "IOF_io.cpp"
#include "ENCF_encryption.cpp"
#include "LNF_labelsAndNumbers.cpp"
#include "LMF_latexManipulation.cpp"
#include "HCBF_htmlTagStorageForCodeBox.cpp"
#include "SPREPF_statPreparation.cpp"
#include "PASF_patternAnalysisStrings.cpp"
#include "DERF_derangements.cpp"
#include "SVGF_svgManipulation.cpp"
#include "ASMRIF_asmReadInstructions.cpp"
#include "ASMCF_asmCompiler.cpp"

namespace SAIOF{
  std::string GL_salt="notInitializedYet";
  int match(const std::string & st1, const std::string & st2,  long direction=1){
    long sz1=st1.length(); long sz2=st2.length();
    if(sz1 < sz2){
      return 0;
    }
    if(direction==0){direction=1;}
    long begin1=0;long begin2=0;
    long end2=sz2;
    if(direction<0){
      begin1=sz1-1;begin2=sz2-1;end2=-1;
    }
    int stillGood=1;
    while(stillGood&&(begin2!=end2)){
      if(st1[begin1]!=st2[begin2]){stillGood=0;}
      begin1+=direction;
      begin2+=direction;
    }
    return stillGood;
  }
  int startMatch(const std::string &st1, const std::string &st2){
    return match(st1,st2,1);
  }
  int endMatch(const std::string &st1, const std::string &st2){
    return match(st1,st2,-1);
  }
  int looksLikeSetupFile(const std::string &st,const std::string &start="st", const std::string &end="e.txt"){
    return startMatch(st,start)*endMatch(st,end);
  }
  std::string makeSalt(const std::string& _s){
      std::string res="m1234567890123456789012345678901234567890123456789012345678901234";
      long i=0;
      while((i<_s.length()) && (i<res.length()) && (_s[i]!='.')){
          res[i]=_s[i];
          ++i;
      }
      return res;
  }
  std::string get_GL_MAIN_SETUP_FILE_NAME(const std::string & st="st", const std::string & en="e.txt"){
    std::vector<std::string> allFiles=IOF::listFiles(".");
    long db_sz=allFiles.size();
    long i=0;
    std::string found="";
    while((i<db_sz)&&(found=="")){
      allFiles[i]=SF::findAndReplace(allFiles[i],"./","");
      if(looksLikeSetupFile(allFiles[i],st,en)){
        found=allFiles[i];
      }
      ++i;
    }
    if(found!=""){
        if(GL_salt=="notInitializedYet"){
            GL_salt=makeSalt(found);
        }
        return found;
    }
    i=0;
    while((i<db_sz)&&(found=="")){
      if(looksLikeSetupFile(allFiles[i],"setup",".txt")){
        found=allFiles[i];
      }
      ++i;
    }
    if(found!=""){return found;}
    return "notFoundSetup.txt";
  }
}
#endif
