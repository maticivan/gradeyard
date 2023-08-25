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


#ifndef _INCL_INCII_inCommandInsert_CPP
#define _INCL_INCII_inCommandInsert_CPP
namespace INCII{
  long getNumberOfVersions(const std::string & q){
    long pos; std::pair<std::string,int> allD;
    pos=0; allD=SF::extract(q,pos,"_searchReplace_","_/searchReplace_");
    if(allD.second==0){
        return -1;
    }
    std::string versions=allD.first;
    std::vector<std::string> openingVersions=SF::stringToVector(versions,"_v*","_");
    std::vector<std::string> closingVersions=SF::stringToVector(versions,"_/v*","_");
    long numV=openingVersions.size();
    if(numV!=closingVersions.size()){
      return -1;
    }
    long i=0;
    long numNonNumeric=0;
    while(i<numV){
      if(openingVersions[i]!=closingVersions[i]){
        numV=-1;
      }
      else{
        if(!BF::isNumeric(openingVersions[i],0)){
          ++numNonNumeric;
        }
      }
      ++i;
    }
    return numV-numNonNumeric;
  }
  std::string singleExamParameters(const std::vector<std::vector<long> > &per, const std::vector<long> & szs, const long & j){
    std::string res="";
    long sz=per.size();
    long chI;
    std::string stI;
    for(long i=0;i<sz;++i){
      chI=0;
      if(szs[i]>0){
        chI = per[i][j % szs[i]];
      }
      stI=BF::padded(i+1,10,"0");
      res+="_vQ"+stI+"*|_"+std::to_string(chI)+"_/vQ"+stI+"*|_\n";
    }
    res="_n*_\n_vnm*_"+std::to_string(j)+"_/vnm*_\n_vt*_\n"+res;
    res+="_/vt*_\n_/n*_\n";
    return res;
  }
  std::string randomizeExamVersions(const std::string & _eName, const long &numExams,const std::string & howOftenToRandomize){
    std::string res="";
    RMD::Response sf;
    int examExists=sf.setFromTextName(_eName);
    if(examExists==0){
      return "error";
    }
    std::string rawText=sf.getTextData();
    long pos;std::pair<std::string,int> allD;
    if(howOftenToRandomize=="randomizeOnlyFirstTime"){
      pos=0;allD=SF::extract(rawText,pos,"_randomVersionForInClass*|_","_/randomVersionForInClass*|_");
      if(allD.second==1){
        return allD.first;
      }
    }
    std::vector<std::string> allQuestions=SF::stringToVector(rawText,"_in*|_","_/in*|_");
    long qsz=allQuestions.size();
    std::vector<long> numVersions;
    std::vector<std::vector<long> > permutations;
    numVersions.resize(qsz);
    permutations.resize(qsz);
    for(long i=0;i<qsz;++i){
      numVersions[i]=getNumberOfVersions(allQuestions[i]);
      permutations[i]=RNDF::genRandPermutation(numVersions[i]);
    }
    for(long j=0;j<numExams;++j){
      res+="\n";
      res+=singleExamParameters(permutations,numVersions,j);
    }
    if(howOftenToRandomize=="randomizeOnlyFirstTime"){
      std::string toRepl="\n\n\n_randomVersionForInClass*|_"+res+"_/randomVersionForInClass*|_\n\n\n";
      rawText=SF::findAndReplace(rawText,"_/textData!!_",toRepl+"_/textData!!_");
      sf.setTextData(rawText);
      sf.putInDB();
    }
    return res;
  }
  std::string formatSingleStudentData(const std::string &rawD){
    std::string formattedData="";
    std::vector<std::string> vectorD=SF::stringToVector(rawD,"_n*|_","_/n*|_");
    if(vectorD.size()>4){
      formattedData="_n*__fn*_"+vectorD[2]+"_/fn*__un*_"+vectorD[4]+"_/un*__vnm*_0_/vnm*__/n*_";
    }
    return formattedData;
  }
  std::string listStudentData(const std::string & _eName){
    std::string res="";
    RMD::Response sf;
    int examExists=sf.setFromTextName(_eName);
    if(examExists==0){
      return "error";
    }
    std::string rawText=sf.getTextData();
    std::vector<std::string> stVect=SF::stringToVector(rawText,"_st*|_","_/st*|_");
    long sNum=stVect.size();
    for(long i=0;i<sNum;++i){
      res+=formatSingleStudentData(stVect[i])+"\n";
    }
    return res;
  }
  std::string evaluateInCommandInsert(const std::vector<std::string> & args){
    std::string result="error";
    long sz=args.size();
    if((sz==4)&&(args[0]=="randomExamVersions")){
      result=randomizeExamVersions(args[1],BF::stringToInteger(args[2]),args[3]);
    }
    if((sz==2)&&(args[0]=="listEnrolledStudentData")){
      result=listStudentData(args[1]);
    }
    return result;
  }
  std::string treatInCommandInsert(const std::string &in){
    std::vector<std::string> cInserts=SF::stringToVector(in,"_inCommandInsert_","_/inCommandInsert_");
    long numInserts=cInserts.size();
    if(numInserts<1){
      return in;
    }
    std::vector<std::string> oldText;
    std::vector<std::vector<std::string> >args;
    std::vector<std::string> result;
    oldText.resize(numInserts);
    args.resize(numInserts);
    result.resize(numInserts);
    for(long i=0;i<numInserts;++i){
      oldText[i]="_inCommandInsert_"+cInserts[i]+"_/inCommandInsert_";
      args[i]=SF::stringToVector(cInserts[i],"_n*_","_/n*_");
      result[i]=evaluateInCommandInsert(args[i]);
    }
    std::map<std::string,std::string> replMap;
    for(long i=0;i<numInserts;++i){
      if((result[i]!="")&&(result[i]!="error")){
        replMap[oldText[i]]=result[i];
      }
    }
    return MFRF::findAndReplace(in,replMap);
  }
}
#endif
