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

#ifndef _INCL_POINTERSSETUP_CPP
#define _INCL_POINTERSSETUP_CPP


namespace PS00F{
  class PSetup{
  private:
      std::string default_prefixSubDirectories="D";
      std::string default_prefixFilesPT="FP";
      std::string default_prefixFilesDT="FD";
      std::string default_maxStringBeforeNextFolder="zzz";
      long default_sysBT_DegreeT=3;
      std::string default_fileWithDynData="xdynData.txt";
      std::string default_fileWithAvailableNames="xavNames.txt";
      std::string default_extension="txt";
      std::string default_nextNameB="_lNF*!_";
      std::string default_nextNameE="_/lNF*!_";
      std::string default_nextEmptyB="?";
      std::string default_nextEmptyE="!";
      std::string rsFolderName="/";
      std::string prefixSubDirectories=default_prefixSubDirectories;
      std::string prefixFilesPT=default_prefixFilesPT;
      std::string prefixFilesDT=default_prefixFilesDT;
      std::string maxStringBeforeNextFolder=default_maxStringBeforeNextFolder;
      long sysBT_DegreeT=default_sysBT_DegreeT;
      std::string fileWithDynData=default_fileWithDynData;
      std::string fileWithAvailableNames=default_fileWithAvailableNames;
      std::string extension=default_extension;
      std::string nextNameB=default_nextNameB;
      std::string nextNameE=default_nextNameE;
      std::string nextEmptyB=default_nextEmptyB;
      std::string nextEmptyE=default_nextEmptyE;
      long loginFailsSpammer;
      long loginTimeExpirationSpammer;
      std::string htmlForSpammer;
  public:
      void getSetupFromMap(const std::map<std::string,std::string> &);
      std::string getRSFolderName() const;
      std::string getPrefixSubDirs() const;
      std::string getPrefixFilesPT() const;
      std::string getPrefixFilesDT() const;
      std::string getMaxString() const;
      long getTDegree() const;
      std::string getFileWithDynData() const;
      std::string getAvailableNamesF() const;
      std::string getExtension() const;
      std::string getNextNameB() const;
      std::string getNextNameE() const;
      std::string getNextEmptyB() const;
      std::string getNextEmptyE() const;
      long get_loginFailsSpammer() const;
      long get_loginTimeExpirationSpammer() const;
      std::string get_htmlForSpammer() const;
  };
  std::string PSetup::getRSFolderName() const{    return rsFolderName; }
  std::string PSetup::getPrefixSubDirs() const{ return prefixSubDirectories;}
  std::string PSetup::getPrefixFilesPT() const{return prefixFilesPT;}
  std::string PSetup::getPrefixFilesDT() const{return prefixFilesDT;}
  std::string PSetup::getMaxString() const{return maxStringBeforeNextFolder;}
  long PSetup::getTDegree() const{return sysBT_DegreeT;}
  std::string PSetup::getFileWithDynData() const{return fileWithDynData;}
  std::string PSetup::getAvailableNamesF() const{return fileWithAvailableNames;}
  std::string PSetup::getExtension() const{return extension;}
  std::string PSetup::getNextNameB() const{return nextNameB;}
  std::string PSetup::getNextNameE() const{return nextNameE;}
  std::string PSetup::getNextEmptyB() const{return nextEmptyB;}
  std::string PSetup::getNextEmptyE() const{return nextEmptyE;}
  long PSetup::get_loginFailsSpammer() const{return loginFailsSpammer;}
  long PSetup::get_loginTimeExpirationSpammer() const{return loginTimeExpirationSpammer;}
  std::string PSetup::get_htmlForSpammer() const{return htmlForSpammer;}
  void PSetup::getSetupFromMap(const std::map<std::string,std::string> & stMap){ 
      std::string tmpMainFolder,tmpStatDB;
      SF::assignValueFromMap(stMap,"mainFolderDB*!",tmpMainFolder);
      SF::assignValueFromMap(stMap,"statDB*!",tmpStatDB);
      if(tmpMainFolder.size()>0){
        if(tmpMainFolder[tmpMainFolder.size()-1]!='/'){
            tmpMainFolder+='/';
        }
      }
      else{tmpMainFolder="/";} 
      rsFolderName=tmpMainFolder+SF::removeStartingCharacters(tmpStatDB,'/')+"/rawSt";
      SF::assignValueFromMap(stMap,"prefixSubdicrectories*!",prefixSubDirectories);
      SF::assignValueFromMap(stMap,"prefixFilesPt*!",prefixFilesPT);
      SF::assignValueFromMap(stMap,"prefixFilesDt*!",prefixFilesDT);
      SF::assignValueFromMap(stMap,"maxStringBeforeNextFolder*!",maxStringBeforeNextFolder);
      SF::assignValueFromMap(stMap,"sysBTT*!",sysBT_DegreeT);
      SF::assignValueFromMap(stMap,"fileWithDynamicData*!",fileWithDynData);
      SF::assignValueFromMap(stMap,"availableFileNames*!",fileWithAvailableNames);
      SF::assignValueFromMap(stMap,"fileExtension*!",extension);
      SF::assignValueFromMap(stMap,"nextNameBB*!",nextNameB);
      SF::assignValueFromMap(stMap,"nextNameEB*!",nextNameE);
      SF::assignValueFromMap(stMap,"nextEmptyBB*!",nextEmptyB);
      SF::assignValueFromMap(stMap,"nextEmptyEB*!",nextEmptyE);
      std::string spammerTempString; long tempNum; long ind_spamCheck=1;
      std::string htmlStart="\n<!DOCTYPE html>\n<html lang=\"en\">";
      std::string htmlEnd="\n</html>";
      htmlForSpammer="<head>\n<title>Access Error</title>\n</head>\n<body>";
      htmlForSpammer+="<h1>Access Error</h1>";
      htmlForSpammer+="</body>";
      SF::assignValueFromMap(stMap,"htmlForSpammer*!",spammerTempString);
      if((spammerTempString.size()>0)&&(spammerTempString!="notFound")){
          htmlForSpammer=spammerTempString;
      }
      htmlForSpammer=htmlStart+htmlForSpammer+htmlEnd;
      loginFailsSpammer=5;
      SF::assignValueFromMap(stMap,"loginFailsSpammer*!",spammerTempString);
      if((spammerTempString.size()>0)&&((spammerTempString[0]=='n')||(spammerTempString[0]=='N'))){
          ind_spamCheck=0;
      }
      tempNum=BF::stringToInteger(spammerTempString);
      if(tempNum>0){
          loginFailsSpammer=tempNum;
      }
      loginTimeExpirationSpammer=60;
      SF::assignValueFromMap(stMap,"loginTimeExpirationSpammer*!",spammerTempString);
      if((spammerTempString.size()>0)&&((spammerTempString[0]=='n')||(spammerTempString[0]=='N'))){
          ind_spamCheck=0;
      }
      tempNum=BF::stringToInteger(spammerTempString);
      if(tempNum>0){
          loginTimeExpirationSpammer=tempNum;
      }
      if(ind_spamCheck==0){
          loginFailsSpammer=0;
          loginTimeExpirationSpammer=0;
      }
  }
}


#endif
