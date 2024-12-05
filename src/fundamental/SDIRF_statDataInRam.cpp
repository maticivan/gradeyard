//    GradeYard learning management system
//
//    Copyright (C) 2024 Ivan Matic, https://gradeyard.com
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

#ifndef _INCL_StatDataInRam_CPP
#define _INCL_StatDataInRam_CPP

namespace SDIRF{ 
  struct ScoreDataConstants{
  public:
    long numTextsInFile=100;
    std::string stFolderName="statSDIRF";
  } GL_CONSTANTS;
  std::vector<long> GL_scoreCoefficients={50,30,25,20,20,15,15,1};
  class ScoreData{
  private:
      long total;
      std::vector<long> lastSevenDays;
  public:
      ScoreData();
      ScoreData(const std::string& );
      void setFromString(const std::string &);
      std::string toString() const;
      void resetDays(long,long );
      long calculateScore(long ) const;
      long totalVisits() const;
      std::vector<long> get_lastSevenDays() const;
      void increaseCounter(long ,long =1);
      void rearrangeDays(long );
  };
  ScoreData::ScoreData(){
    total=0;
    lastSevenDays.resize(7);
    for(long i=0;i<7;++i){lastSevenDays[i]=0;}
  }
  ScoreData::ScoreData(const std::string& _s){
    setFromString(_s);
  } 
  long ScoreData::totalVisits() const{return total;}
  std::vector<long> ScoreData::get_lastSevenDays() const{
    return lastSevenDays;
  }
  void ScoreData::setFromString(const std::string& s){
    std::vector<std::string> rawNums=SF::stringToVectorSimpleSeparator(s,";");
    if(rawNums.size()<8){
      std::vector<std::string> extended;
      extended.resize(8);
      long i=0;
      while(i<rawNums.size()){
        extended[i]=rawNums[i];
        ++i;
      }
      while(i<8){
        extended[i]="0";
        ++i;
      }
      rawNums=extended;
    }
    total=BF::stringToInteger(rawNums[0]);
    lastSevenDays.resize(7);
    for(long i=0;i<7;++i){
      lastSevenDays[i]=BF::stringToInteger(rawNums[i+1]);
    }
  }
  std::string ScoreData::toString() const{
    std::string res=std::to_string(total);
    for(long i=0;i<lastSevenDays.size();++i){
      res+=";"+std::to_string(lastSevenDays[i]);
    }
    return res;
  }
  void ScoreData::resetDays(long numDaysToReset,long today){
    if(numDaysToReset<1){return;}
    if(numDaysToReset>7){numDaysToReset=7;}
    if((today<0)||(today>6)){
      return;
    }
    for(long i=0;i<numDaysToReset;++i){
      lastSevenDays[(7+today-i)%7]=0;
    }
  }
  long ScoreData::calculateScore(long today) const{
    long sc=total*GL_scoreCoefficients[7];
    if((today<0)||(today>6)){today=0;}
    long daysPast;
    for(long i=0;i<7;++i){
      daysPast=(7+today-i)%7;
      sc += lastSevenDays[i] * GL_scoreCoefficients[daysPast];
    }
    return sc;
  }
  void ScoreData::increaseCounter(long today,long inc){
    total+=inc;
    if((today>-1)&&(today<7)){
      lastSevenDays[today]+=inc;
    }
  } 
  void ScoreData::rearrangeDays(long today){
    std::vector<long> oldDays=lastSevenDays;
    for(long i=0;i<7;++i){
        lastSevenDays[i]=oldDays[(today+i+1)%7];
    }
  }
  void resetDays(std::map<std::string,std::string>& mapToReset,long daysToReset, long today){
    std::map<std::string,std::string>::iterator it,itE;
    it=mapToReset.begin();itE=mapToReset.end();
    while(it!=itE){
      ScoreData tmpSD(it->second);
      tmpSD.resetDays(daysToReset,today);
      it->second=tmpSD.toString();
      ++it;
    }
  }
  std::string increaseCounter(const std::string& in, const std::string& tKey, long daysToReset, long today, long inc=1){
    std::map<std::string,std::string> rawGroupStatData=SF::stringToMap(in,"_n*_","_/n*_","_v*_","_/v*_");
    if(daysToReset>0){
      resetDays(rawGroupStatData,daysToReset,today);
    }
    ScoreData tmpSD;
    std::map<std::string,std::string>::iterator it=rawGroupStatData.find(tKey), itE=rawGroupStatData.end();
    if(it!=itE){
      tmpSD.setFromString(it->second);
    }
    tmpSD.increaseCounter(today,inc);
    rawGroupStatData[tKey]=tmpSD.toString(); 
    return SF::mapToString(rawGroupStatData,"_n*_","_/n*_","_v*_","_/v*_");
  }
  std::string removeTextFromDatabase(const std::string& in, const std::string& tKey){
    std::map<std::string,std::string> rawGroupStatData=SF::stringToMap(in,"_n*_","_/n*_","_v*_","_/v*_");
    rawGroupStatData.erase(tKey);
    return SF::mapToString(rawGroupStatData,"_n*_","_/n*_","_v*_","_/v*_");
  }
  void updateStatDataForRamInternal(const SPREPF::StatData& sd,const std::string& folder_path){ 
    long pageIdL=BF::stringToInteger(sd.att_pageID);
    long pageGroup=pageIdL/GL_CONSTANTS.numTextsInFile;
    std::string fileName=folder_path+"/sd"+BF::padded(pageGroup,100000,"0")+".txt";
    std::string previousText=IOF::fileToString(fileName);
    TMF::Timer tm;
    std::vector<long> tnVect=tm.timeNowVector();
    long today=tnVect[6];
    long tNow=tm.timeNow();
    long nDaysToReset=0;
    if(IOF::fileExists(fileName)){
      long tModification=IOF::timeOfCreation(fileName);
      nDaysToReset=(tNow-tModification)/(60*60*24);
    } 
    std::string newText=increaseCounter(previousText,sd.att_page,nDaysToReset,today,1);
    IOF::toFile(fileName,newText);
  }
  void updateStatDataForRam(const SPREPF::StatData& sd,const std::string& hiddenPathFolder){
    std::string folder_path=hiddenPathFolder+"/"+GL_CONSTANTS.stFolderName; 
    IOF::sys_createFolderIfDoesNotExist(folder_path,"readme.txt","Do not edit this folder.");
    SPREPF::StatData sd0=sd;
    sd0.att_page="!*AllPages*!";
    sd0.att_pageID="0";
    updateStatDataForRamInternal(sd,folder_path);
    updateStatDataForRamInternal(sd0,folder_path);
  }
  void deleteTextFromStatData(const SPREPF::StatData& sd,const std::string& hiddenPathFolder){
    std::string folder_path=hiddenPathFolder+"/"+GL_CONSTANTS.stFolderName; 
    IOF::sys_createFolderIfDoesNotExist(folder_path,"readme.txt","Do not edit this folder.");
    long pageIdL=BF::stringToInteger(sd.att_pageID);
    long pageGroup=pageIdL/GL_CONSTANTS.numTextsInFile;
    std::string fileName=folder_path+"/sd"+BF::padded(pageGroup,100000,"0")+".txt";
    std::string previousText=IOF::fileToString(fileName);
    std::string newText=removeTextFromDatabase(previousText,sd.att_page);
    IOF::toFile(fileName,newText);
  }
  struct ScoreTextPair{
  public:
    long score;
    std::string text;
      long totalVisits;
      ScoreTextPair();
    int operator<(const ScoreTextPair& )const;
  };
  int ScoreTextPair::operator<(const ScoreTextPair& b)const{
    if(score<b.score){return 1;}
    if(score>b.score){return 0;}
    if(text<b.text){return 1;}
    return 0;
  }
    ScoreTextPair::ScoreTextPair(){
        score=0;totalVisits=0;
    }
  struct TextScorePair{
  public:
    long score;
    std::string text;
    long totalVisits;
    TextScorePair();
    int operator<(const TextScorePair& )const;
  };
  TextScorePair::TextScorePair(){
    score=0;totalVisits=0;
  }
  int TextScorePair::operator<(const TextScorePair& b)const{
    if(text<b.text){return 1;}
    if(text>b.text){return 0;}
    if(score<b.score){return 1;}
    return 0;
  }
  struct CompleteStats{
  public:
    std::map<std::string,ScoreData> mainMap;
    SSF::StatSeq<ScoreTextPair> sortedAccordingToScore;
    SSF::StatSeq<TextScorePair> sortedAccordingToName;
  };
  void updateStats(CompleteStats& cs, const std::string& fName){
    if(fName=="readme.txt"){return;}
    std::string jfn=IOF::justFileNameNoExtensionNoFolder(fName);
    if(jfn.length()<3){return;}
    if(jfn[0]!='s'){return;}
    if(jfn[1]!='d'){return;}
    TMF::Timer tm;
    std::vector<long> tnVect=tm.timeNowVector();
    long today=tnVect[6];
    long tNow=tm.timeNow();
    long nDaysToReset=0;
    if(IOF::fileExists(fName)){
      long tModification=IOF::timeOfCreation(fName);
      nDaysToReset=(tNow-tModification)/(60*60*24);
    } 
    std::string rawText=IOF::fileToString(fName);
    std::map<std::string,std::string> rawGroupStatData=SF::stringToMap(rawText,"_n*_","_/n*_","_v*_","_/v*_");
    if(nDaysToReset>0){
      resetDays(rawGroupStatData,nDaysToReset,today);
      rawText=SF::mapToString(rawGroupStatData,"_n*_","_/n*_","_v*_","_/v*_");
      IOF::toFile(fName,rawText);
    }
    std::map<std::string,std::string>::const_iterator it,itE;
    it=rawGroupStatData.begin();itE=rawGroupStatData.end();
    while(it!=itE){
      ScoreData tmpSD(it->second);
      (cs.mainMap)[it->first]=tmpSD;
      ScoreTextPair tmpSTP;
      TextScorePair tmpTSP;
      tmpSTP.text=it->first;
      tmpSTP.score=tmpSD.calculateScore(today);
      tmpSTP.totalVisits=tmpSD.totalVisits();
      tmpTSP.text=it->first;
      tmpTSP.score=tmpSTP.score;
      tmpTSP.totalVisits=tmpSTP.totalVisits;
      (cs.sortedAccordingToScore).insert(tmpSTP);
      (cs.sortedAccordingToName).insert(tmpTSP);
      ++it;
    }
  }
  CompleteStats getCompleteStats(const std::string& hiddenPathFolder){
    CompleteStats res;
    std::string folder_path=hiddenPathFolder+"/"+GL_CONSTANTS.stFolderName; 
    std::vector<std::string> allFiles=IOF::listFiles(folder_path);
    for(long i=0;i<allFiles.size();++i){
      updateStats(res,allFiles[i]);
    }
    return res;
  }
  std::string statAnalysis(const SPREPF::StatData& sd,const std::string& hiddenPathFolder,
                           const std::string& tableOpen, const std::string& tableClose){
    std::string folder_path=hiddenPathFolder+"/"+GL_CONSTANTS.stFolderName;
    IOF::sys_createFolderIfDoesNotExist(folder_path,"readme.txt","Do not edit this folder.");
    long pageIdL=BF::stringToInteger(sd.att_pageID);
    long pageGroup=pageIdL/GL_CONSTANTS.numTextsInFile;
    std::string fileName=folder_path+"/sd"+BF::padded(pageGroup,100000,"0")+".txt";
    TMF::Timer tm;
    std::vector<long> tnVect=tm.timeNowVector();
    long today=tnVect[6];
    std::string sA; 
    if(IOF::fileExists(fileName)){
      std::map<std::string,std::string> rawGroupStatData=SF::stringToMap(IOF::fileToString(fileName),"_n*_","_/n*_","_v*_","_/v*_");
      std::map<std::string,std::string>::iterator it=rawGroupStatData.find(sd.att_page), itE=rawGroupStatData.end();
      if(it!=itE){
        ScoreData scd=it->second;
        scd.rearrangeDays(today);
        std::vector<long> l7d=scd.get_lastSevenDays();
        long lsz=2;
        std::stack<std::vector<std::string> > allLines;
        std::vector<std::string> topLine,mLine;
        topLine.resize(lsz);mLine.resize(lsz);
        topLine[0]="Day";
        topLine[1]="V";
        for(long i=0;i<7;++i){
          if(i==6){
            mLine[0]="Today";
          }
          if(i==5){
            mLine[0]="Yesterday";
          }
          if(i<5){
            mLine[0]="Today - "+std::to_string(7-i);
          }
          mLine[1]=std::to_string(l7d[i]);
          allLines.push(mLine);
        }
        mLine[0]="...";
        mLine[1]="...";
        allLines.push(mLine);
        mLine[0]="Total";
        mLine[1]=std::to_string(scd.totalVisits());
        allLines.push(mLine);
        allLines.push(topLine);
        sA=HSF::tableFromStack(allLines,tableOpen,tableClose);
      }
    }
    return sA;
  }
}
#endif
