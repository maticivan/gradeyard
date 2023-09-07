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

#ifndef _INCL_GF_GLOBALINITS_CPP
#define _INCL_GF_GLOBALINITS_CPP
namespace GF{
  DEBMF::DebHandler GL_DEB_MESSAGES;
  std::map<std::string,std::string> GL_OPENED_FILES;
  long GL_MAXFILESINRAM=1000;
  long GLOBAL_FILETOSTRCOUNTER=0;
  long GLOBAL_MAX_DAYS_ST=60;
  long GLOBAL_MAX_MONTHS_ST=15;
  long GLOBAL_MAX_YEARS_ST=100;
  std::string GLOBAL_FILEOPENED;
  std::string GL_HIDING_STRING_FORMULACENTER="azmnvdtvnvdtv29122943";
  std::string GL_HIDING_STRING_DISPPF01="*feS8!!kwXW**az!n!vD*t|v|S";
  std::string GL_HIDING_STRING_DISPPF02="df*1l)k|f|";
  std::string GL_HIDING_STRING_HDDBRF="anv9dt1v5n77v2dtv_";
  std::string GL_HIDING_STRING_PTKF01="*3AA3";
  std::string GL_HIDING_STRING_PTKF02="2QVW1";
  std::string GL_HIDING_STRING_PTKF03="9*2k";
  std::string GL_HIDING_STRING_PTKF04="WNRf34";
  std::string GL_HIDING_STRING_SF="e!n!S!azm8*81!!7!2203!a";
  std::string GL_HIDING_STRING_TWDVF="!e*n&d=*7e&n&d";
  std::string GL_sepB="_*!.";
  std::string GL_sepE="_/*!.";
  std::string GL_errorDidNotCompile="Error:DidNotCompile";
  std::string GL_errorOutputTooBig="Error:OutputTooBig";
  std::string GL_officialNA="N/A";
  long GL_MAIN_SECRET_NUMBER=17;//This will be set to the secret code in mainFunction.cpp
  std::string GL_Alphabet;//This will be set to "english" in WSI_wiSetup.cpp
  std::map<std::string,std::map<std::string,double> > initializeLetterGradesConversionMap(){
    std::map<std::string,std::map<std::string,double> > fR;
    std::map<std::string,double> aConv;
    aConv["A+"]=4.0;
    aConv["A"]=4.0;
    aConv["A-"]=3.7;
    aConv["B+"]=3.3;
    aConv["B"]=3.0;
    aConv["B-"]=2.7;
    aConv["C+"]=2.3;
    aConv["C"]=2.0;
    aConv["C-"]=1.7;
    aConv["D+"]=1.3;
    aConv["D"]=1.0;
    aConv["F"]=0.0;
    aConv["WU"]=0.0;
    fR["Baruch"]=aConv;
    return fR;
  }
  struct Danger{
  public:
    std::set<std::string> strings;
    std::map<std::string,std::string> veryBadStringsToBeErased;
    std::set<char> unacceptableCharactersInUsernames;
    std::set<char> unacceptableCharactersInNames;
    Danger();
  } GL_DANGERS;
  Danger::Danger(){
    strings.insert("<?");
    strings.insert("<php");
    strings.insert("*|_");
    strings.insert("*!_");
    strings.insert("ata!!_");
    strings.insert("ing!!_");
    strings.insert("_n*_");
    strings.insert("_n_");
    strings.insert("_/n_");
    strings.insert("ame_");
    strings.insert("ser_");
    strings.insert("ed_");
    strings.insert("!code_");
    strings.insert("itle_");
    strings.insert("tion_");
    strings.insert("xams_");
    strings.insert("ctions_");
    strings.insert("ouas_");
    strings.insert("itle*");
    strings.insert("rmits*");
    strings.insert("ables*");
    strings.insert("opicCode_");
    strings.insert("_/n*_");
    veryBadStringsToBeErased["<?"]="";
    veryBadStringsToBeErased["<php"]="";
    veryBadStringsToBeErased["ata!!_"]="";
    veryBadStringsToBeErased["ing!!_"]="";
    std::string unBCharacters="/.\"\'\n\t\\#*=@!~$%^+,;:?<> |";
    for(long i=0;i<unBCharacters.length();++i){
      unacceptableCharactersInUsernames.insert(unBCharacters[i]);
    }
    std::string nBCharacters="/.\"\'\n\t\\#*=@!~$%^+,;:?<>_|";
    for(long i=0;i<nBCharacters.length();++i){
      unacceptableCharactersInNames.insert(nBCharacters[i]);
    }
  }
  std::map<std::string,std::map<std::string,double> > GL_GRADE_CONVERSION=initializeLetterGradesConversionMap();
  long GL_randSeed= std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 GL_mt_randIF(GL_randSeed);
}
#endif
