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

#ifndef _INCL_POINTERSSETUP_CPP
#define _INCL_POINTERSSETUP_CPP


namespace PS00F{
  class PSetup{
  private:

      std::string fNB="_folderName*!_";
      std::string fNE="_/folderName*!_";
      std::string pSDB="_prefixSubdicrectories*!_";
      std::string pSDE="_/prefixSubdicrectories*!_";
      std::string pFPtB="_prefixFilesPt*!_";
      std::string pFPtE="_/prefixFilesPt*!_";
      std::string pFDtB="_prefixFilesDt*!_";
      std::string pFDtE="_/prefixFilesDt*!_";
      std::string mSFB="_maxStringBeforeNextFolder*!_";
      std::string mSFE="_/maxStringBeforeNextFolder*!_";
      std::string sysBTTB="_sysBTT*!_";
      std::string sysBTTE="_/sysBTT*!_";
      std::string adFB="_availableFileNames*!_";
      std::string adFE="_/availableFileNames*!_";


      std::string dDFB="_fileWithDynamicData*!_";
      std::string dDFE="_/fileWithDynamicData*!_";

      std::string extB="_fileExtension*!_";
      std::string extE="_/fileExtension*!_";

      std::string nextNameBSepB="_nextNameBB*!_";
      std::string nextNameBSepE="_nextNameBE*!_";
      std::string nextNameESepB="_nextNameEB*!_";
      std::string nextNameESepE="_nextNameEE*!_";

      std::string nextEmptyBSepB="_nextEmptyBB*!_";
      std::string nextEmptyBSepE="_nextEmptyBE*!_";
      std::string nextEmptyESepB="_nextEmptyEB*!_";
      std::string nextEmptyESepE="_nextEmptyEE*!_";





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




      void getSetupFromString(const std::string &);

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


      void printSetup() const;



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


  void PSetup::getSetupFromString(const std::string & fN){
      std::string sSt=fN;
      sSt += fNB+default_folderName+fNE;
      sSt += pSDB+default_prefixSubDirectories+pSDE;
      sSt += pFPtB+default_prefixFilesPT+pFPtE;
      sSt += pFDtB+default_prefixFilesDT+pFDtE;
      sSt += mSFB+default_maxStringBeforeNextFolder+mSFE;
      sSt += sysBTTB+std::to_string(default_sysBT_DegreeT)+sysBTTE;
      sSt += dDFB+default_fileWithDynData+dDFE;
      sSt += adFB+default_fileWithAvailableNames+adFE;
      sSt += extB+default_extension+extE;
      sSt += nextNameBSepB+default_nextNameB+nextNameBSepE;
      sSt += nextNameESepB+default_nextNameE+nextNameESepE;
      sSt += nextEmptyBSepB+default_nextEmptyB+nextEmptyBSepE;
      sSt += nextEmptyESepB+default_nextEmptyE+nextEmptyESepE;

      long pos=0;
      folderName=(SF::extract(sSt,pos,fNB,fNE)).first;
      pos=0;
      prefixSubDirectories=(SF::extract(sSt,pos,pSDB,pSDE)).first;
      pos=0;
      prefixFilesPT=(SF::extract(sSt,pos,pFPtB,pFPtE)).first;
      pos=0;
      prefixFilesDT=(SF::extract(sSt,pos,pFDtB,pFDtE)).first;
      pos=0;
      maxStringBeforeNextFolder=(SF::extract(sSt,pos,mSFB,mSFE)).first;
      pos=0;
      sysBT_DegreeT= BF::stringToInteger((SF::extract(sSt,pos,sysBTTB,sysBTTE)).first);
      pos=0;
      fileWithDynData=(SF::extract(sSt,pos,dDFB,dDFE)).first;
      pos=0;
      fileWithAvailableNames=(SF::extract(sSt,pos,adFB,adFE)).first;
      pos=0;
      extension=(SF::extract(sSt,pos,extB,extE)).first;
      pos=0;
      nextNameB=(SF::extract(sSt,pos,nextNameBSepB,nextNameBSepE)).first;
      nextNameE=(SF::extract(sSt,pos,nextNameESepB,nextNameESepE)).first;
      nextEmptyB=(SF::extract(sSt,pos,nextEmptyBSepB,nextEmptyBSepE)).first;
      nextEmptyE=(SF::extract(sSt,pos,nextEmptyESepB,nextEmptyESepE)).first;


  }
  void PSetup::printSetup() const{
      std::cout<<folderName<<"\n";
      std::cout<<prefixSubDirectories<<"\n";
      std::cout<<prefixFilesPT<<"\n";
      std::cout<<prefixFilesDT<<"\n";
      std::cout<<maxStringBeforeNextFolder<<"\n";
      std::cout<<fileWithDynData<<"\n";
      std::cout<<fileWithAvailableNames<<"\n";
      std::cout<<extension<<"\n";

  }

}


#endif
