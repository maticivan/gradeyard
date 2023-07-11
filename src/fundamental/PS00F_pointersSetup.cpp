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

#ifndef _INCL_POINTERSSETUP_CPP
#define _INCL_POINTERSSETUP_CPP


namespace PS00F{
  class PSetup{
  private:
      std::string default_folderName=".";
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
      std::string folderName=default_folderName;
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
  public:
      void getSetupFromMap(const std::map<std::string,std::string> &);
      std::string getFolderName() const;
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
  };
  std::string PSetup::getFolderName() const{    return folderName; }
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
  void PSetup::getSetupFromMap(const std::map<std::string,std::string> & stMap){
      SF::assignValueFromMap(stMap,"folderName*!",folderName);
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
  }
}


#endif
