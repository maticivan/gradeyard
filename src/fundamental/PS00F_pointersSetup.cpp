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
      std::string spammerPasswordReplacement;
      std::string htmlForSpammer;
      std::set<std::string> m_unTHPStat;
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
      std::string get_spammerPasswordReplacement() const;
      int check_privacyProtection(const std::string& ) const;
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
  std::string PSetup::get_spammerPasswordReplacement() const{
      return spammerPasswordReplacement;
  }
  int PSetup::check_privacyProtection(const std::string& _u) const{
    if(m_unTHPStat.find(_u)!=m_unTHPStat.end()){
        return 1;
    }
    return 0;
  }
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
      std::string htmlStart;
      htmlStart  = "Status: 404 Not Found\r\n";
      htmlStart += "Content-Type: text/html; charset=UTF-8\r\n";
      htmlStart += "X-Content-Type-Options: nosniff\r\n";
      htmlStart += "Cache-Control: no-store, no-cache, must-revalidate\r\n";
      htmlStart += "Pragma: no-cache\r\n";
      htmlStart += "Expires: 0\r\n";
      htmlStart += "\r\n";
      htmlStart += "<!DOCTYPE html>\n";
      htmlStart += "<html lang=\"en\">\n";
      std::string htmlEnd="\n</html>";
      htmlForSpammer  = "<head>\n";
      htmlForSpammer += "<title>Unavailable</title>\n";
      htmlForSpammer += "</head>\n";
      htmlForSpammer += "<body>\n";
      htmlForSpammer += "<h1>Unavailable</h1>\n";
      htmlForSpammer += "<p>This resource is unavailable.</p>\n";
      htmlForSpammer += "<!-- req: " + RNDF::genRandCode(15) + " -->\n";
      htmlForSpammer += "</body>\n";
      spammerTempString="";
      SF::assignValueFromMap(stMap,"htmlForSpammer*!",spammerTempString);
      if((spammerTempString.size()>0)&&(spammerTempString!="notFound")){
          htmlForSpammer=SF::findAndReplace(spammerTempString,
                                            "[randomCode]",
                                            RNDF::genRandCode(15));
      }
      htmlForSpammer=htmlStart+htmlForSpammer+htmlEnd;
      spammerPasswordReplacement="";
      spammerTempString="";
      SF::assignValueFromMap(stMap,
                             "spammerPasswordReplacement*!",
                             spammerTempString);
      if((spammerTempString.size()>0)&&(spammerTempString!="notFound")){
          spammerPasswordReplacement=spammerTempString;
      }
      if(
         (spammerPasswordReplacement=="no") ||
         (spammerPasswordReplacement=="NO") ||
         (spammerPasswordReplacement=="No")
         ){
        spammerPasswordReplacement="";
      }
      loginFailsSpammer=5;
      spammerTempString="";
      SF::assignValueFromMap(stMap,"loginFailsSpammer*!",spammerTempString);
      if((spammerTempString.size()>0)&&((spammerTempString[0]=='n')||(spammerTempString[0]=='N'))){
          ind_spamCheck=0;
      }
      tempNum=BF::stringToInteger(spammerTempString);
      if(tempNum>0){
          loginFailsSpammer=tempNum;
      }
      loginTimeExpirationSpammer=60;
      spammerTempString="";
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
      else{
          spammerTempString="";
          SF::assignValueFromMap(stMap,"usernamesToHidePasswordsInStat*!",spammerTempString);
          if((spammerTempString.size()>0)&&(spammerTempString!="notFound")){
              m_unTHPStat=SF::stringToSetSimpleSeparator(spammerTempString,";");
          }
      }
  }
}


#endif
