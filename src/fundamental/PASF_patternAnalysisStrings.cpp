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

#ifndef _INCL_PATTERNSFORSTRINGS_CPP
#define _INCL_PATTERNSFORSTRINGS_CPP

namespace PASF{
  struct StudentData{
  public:
      std::string studentID;
      std::string firstName;
      std::string lastName;
      std::string userName;
      std::string externalId;
      std::string email;
      std::string password;
      std::string putIntoString(const long &) const;
      std::string putIntoStringForEmail(const std::string &  = ",") const;
      int prepareExternalAndInternalId(const std::string &);
      int setFromString(const std::string &);
  };
  int StudentData::setFromString(const std::string &st){
    long pos;std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(st,pos,"_id*_","_/id*_");
    if(allD.second==0){
      return 0;
    }
    studentID=allD.first;
    pos=0;allD=SF::extract(st,pos,"_first*_","_/first*_");
    if(allD.second==0){
      return 0;
    }
    firstName=allD.first;
    pos=0;allD=SF::extract(st,pos,"_last*_","_/last*_");
    if(allD.second==0){
      return 0;
    }
    lastName=allD.first;
    pos=0;allD=SF::extract(st,pos,"_email*_","_/email*_");
    if(allD.second==0){
      return 0;
    }
    email=allD.first;
    return 1;
  }
  std::string StudentData::putIntoStringForEmail(const std::string &sep) const{
    std::string fR="";
    fR+=firstName+sep+lastName+sep+email+sep+userName+sep+password+sep+"endLine";
    return fR;
  }
  std::string StudentData::putIntoString(const long & includePasswordAndExternalId) const{
    std::string fR="";
    fR+="_st*|_";
    std::string sB="_n*|_";
    std::string sE="_/n*|_";
    fR+=sB+firstName+" "+lastName+sE;
    fR+=sB+email+ sE;
    fR+=sB+userName+sE;
    if(includePasswordAndExternalId==1){
      fR+=sB+externalId+sE;
    }
    else{
      fR+=sB+"na"+sE;
    }
    if(includePasswordAndExternalId==1){
      fR+=sB+password+sE;
    }
    fR+="_/st*|_";
    return fR;
  }
  std::string removeNewLineCharacters(const std::string &in){
    std::string out=in;
    out=SF::findAndReplace(out,"\n","");
    out=BF::removeASCII10AND13(out);
    return out;
  }
  std::string makeStringSafe(const std::string &in){
    std::string o1=BF::cleanAllSpaces(in);
    std::string out="";
    long sz=o1.length();
    for(long i=0;i<sz;++i){
      if( (o1[i]>='a') && (o1[i]<='z') ){out+=o1[i];}
      if( (o1[i]>='A') && (o1[i]<='Z') ){out+=o1[i];}
      if( (o1[i]>='0') && (o1[i]<='9') ){out+=o1[i];}
      if( o1[i]=='_'){out+=o1[i];}
      if( o1[i]=='-'){out+=o1[i];}
    }
    return out;
  }
  int StudentData::prepareExternalAndInternalId(const std::string & ext){
    externalId=ext+studentID;
    firstName=BF::cleanSpaces(removeNewLineCharacters(firstName));
    lastName=BF::cleanSpaces(removeNewLineCharacters(lastName));
    userName=makeStringSafe(firstName+lastName);
    long oldLength=userName.length();
    if(oldLength>15){
      std::string fNPrep;
      std::string lNPrep;
      std::string attempt;
      long newLength;
      long pos;std::pair<std::string,int> allD;
      pos=0;allD=SF::getEverythingBefore(firstName,pos," ");
      if(allD.second==1){
        fNPrep=allD.first;
        attempt=makeStringSafe(fNPrep+lastName);
        newLength=attempt.length();
        if((newLength>5)&&(newLength<oldLength)){
          userName=attempt;
          return 1;
        }
      }
      fNPrep=makeStringSafe(firstName);
      lNPrep=makeStringSafe(lastName);
      if(lNPrep.length()>5){
        attempt="";
        if(fNPrep.length()>1){attempt+=fNPrep[0];}
        attempt+=lNPrep;
        userName=attempt;
        return 1;
      }
      if(fNPrep.length()>5){
        attempt=fNPrep;
        if(lNPrep.length()>0){attempt+=lNPrep[0];}
        userName=attempt;
        return 1;
      }
    }
    return 1;
  }
  std::string generatePassword(const long & len){
    std::string psswd="";
    std::vector<std::string> pc;
    long nCh=20;
    pc.resize(nCh);
    for(long i=0;i<10;++i){pc[i]=std::to_string(i);}
    pc[10]="!";pc[11]="@";pc[12]="&";pc[13]="?";pc[14]="a";pc[15]="b";pc[16]="A";pc[17]="B";pc[18]="w";pc[19]="W";

    for(long i=0;i<len;++i){
      psswd+=pc[RNDF::randNum(nCh)];
    }
    return psswd;
  }
  long mostCommonDifference(const std::map<long,long> & m){
    long fR=-1;
    long maxFreq=-1;
    std::map<long,long>::const_iterator it,itE;
    it=m.begin();itE=m.end();
    while(it!=itE){
      if(it->second>maxFreq){
        maxFreq=it->second;
        fR=it->first;
      }
      ++it;
    }
    return fR;
  }
  std::map<long,long> possibleDifferences(const std::vector<long> &v){
    std::map<long,long> fR;
    long szv=v.size();
    --szv;
    std::map<long,long>::const_iterator it,itE;
    for(long i=0;i<szv;++i){
      itE=fR.end();
      it=fR.find(v[i+1]-v[i]);
      if(it==itE){
        fR[v[i+1]-v[i]]=1;
      }
      else{
        fR[v[i+1]-v[i]]=fR[v[i+1]-v[i]]+1;
      }
    }
    return fR;
  }
  long mostCommonDifference(const std::vector<long> &v){
    return mostCommonDifference(possibleDifferences(v));
  }
  std::vector<std::string> getIndividualLines(const std::string & input,const std::string &separator="\n"){
    std::vector<std::string> lines;
    std::stack<std::string> lS;
    long pos;std::pair<std::string,int> allD;
    pos=0;allD=SF::getEverythingBefore(input,pos,separator);
    while(allD.second==1){
      lS.push(allD.first);
      allD=SF::getEverythingBefore(input,pos,separator);
    }
    return SF::stackToVector(lS);
  }
  int numbersOnly(const std::string & _s){
    std::string s=BF::cleanAllSpaces(_s);
    long sz=s.length();
    if(sz==0){
      return 0;
    }
    long i=0;
    while(i<sz){
      if((s[i]>='0')&&(s[i]<='9')){
        ++i;
      }
      else{
        return 0;
      }
    }
    return 1;
  }
  std::vector<long> linesWithnumbersOnly(const std::vector<std::string> & lns){
    std::stack<long> stNums;
    long sz=lns.size();
    for(long i=0;i<sz;++i){
      if(numbersOnly(lns[i])==1){
        stNums.push(i);
      }
    }
    return SF::stackToVector(stNums);
  }
  std::pair<std::string,std::string> commaSeparatedPair(const std::string & _input, const std::string & _comma=","){
    std::pair<std::string,std::string> fR;
    fR.second="notFound";
    long pos;
    std::pair<std::string,int> allD;
    std::string forAdd=_comma+"notFound";
    std::string input=_input;
    for(long i=0;i<5;++i){
      input+=forAdd;
    }
    pos=0;allD=SF::getEverythingBefore(input,pos,_comma);
    if(allD.second==1){
      fR.first=allD.first;
      allD=SF::getEverythingBefore(input,pos,_comma);
      if(allD.second==1){
        fR.second=allD.first;
      }
    }
    return fR;
  }
  std::vector<StudentData> identifyStudentsNonCleanInput(const std::string & _input,const std::string &addToExtId){
    std::pair<std::string,int> allD;
    long pos;
    std::string emB="_emails!*_";
    std::string emE="_/emails!*_";
    std::string inp=_input+emE+"end"+emE;
    pos=0;allD=SF::getEverythingBefore(inp,pos,emB);
    std::string input=_input;
    std::string emailsSt="";
    if(allD.second==1){
      input = allD.first;
      allD=SF::getEverythingBefore(inp,pos,emE);
      if(allD.second==1){
        emailsSt=allD.first;
      }
    }
    std::vector<std::string> indEmails=getIndividualLines(emailsSt+",end",",");
    std::vector<std::string> lines=getIndividualLines(input,"\n");
    std::vector<long> lNums=linesWithnumbersOnly(lines);
    long sz=lNums.size();
    long bsz=lines.size();
    long mcd=mostCommonDifference(lNums);
    std::stack<StudentData> sds;
    StudentData tmp;
    std::pair<std::string,std::string> lnfn,lnfnT;
    long nummostCommonDifferencesTest;
    for(long i=0;i<sz;++i){
      if(lNums[i]+1<bsz){
        nummostCommonDifferencesTest=0;
        if((i>0)&&(lNums[i]-lNums[i-1]==mcd)){
          ++nummostCommonDifferencesTest;
        }
        if((i<sz-1)&&(lNums[i+1]-lNums[i]==mcd)){
          ++nummostCommonDifferencesTest;
        }
        if(nummostCommonDifferencesTest>0){
          lnfnT=commaSeparatedPair(lines[lNums[i]+1]);
          if(lnfnT.second!="notFound"){
            lnfn=lnfnT;
          }
          else{
            lnfnT=commaSeparatedPair(lines[lNums[i]+1]," ");
            if(lnfnT.second!="notFound"){
              lnfn=lnfnT;
            }
            else{
              lnfnT=commaSeparatedPair(lines[lNums[i]+2]);
              if(lnfnT.second!="notFound"){
                lnfn=lnfnT;
              }
              else{
                lnfnT=commaSeparatedPair(lines[lNums[i]+2]," ");
                if(lnfnT.second!="notFound"){
                  lnfn=lnfnT;
                }
                else{
                  lnfn.second="notFound";
                }
              }
            }
          }
          if(lnfn.second!="notFound"){
            tmp.studentID=BF::cleanAllSpaces(lines[lNums[i]]);
            tmp.firstName=SF::findAndReplace(lnfn.second,"\n","");
            tmp.lastName=SF::findAndReplace(lnfn.first,"\n","");
            sds.push(tmp);
          }
        }
      }
    }
    std::vector<StudentData> fR= SF::stackToVector(sds);
    long numSt=fR.size();
    for(long i=0;i<numSt;++i){
      fR[i].prepareExternalAndInternalId(addToExtId);
      fR[i].email="na";
    }
    long numEmails=indEmails.size();
    if((numEmails==numSt)||(numEmails==numSt+1)){
      for(long i=0;i<numSt;++i){
        fR[i].email=BF::cleanAllSpaces( indEmails[i]);
      }
    }
    return fR;
  }
  std::vector<StudentData> identifyStudentsCleanInput(const std::string & _input,const std::string &addToExtId){
    std::pair<std::string,int> allD;
    long pos;
    std::vector<std::string> lines=SF::stringToVector(_input,"_n*_","_/n*_");
    long sz=lines.size();
    StudentData tmp;std::stack<StudentData> sds;
    for(long i=0;i<sz;++i){
      if(tmp.setFromString(lines[i])==1){
        tmp.prepareExternalAndInternalId(addToExtId);
        sds.push(tmp);
      }
    }
    std::vector<StudentData> fR= SF::stackToVector(sds);
    return fR;
  }
  std::vector<StudentData> identifyStudents(const std::string & _input,const std::string &addToExtId){
    long pos;std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(_input,pos,"_id*_","_/id*_");
    if(allD.second==0){
      return identifyStudentsNonCleanInput(_input,addToExtId);
    }
    return identifyStudentsCleanInput(_input,addToExtId);
  }
  std::string numsAndLettersOnly(const std::string &in){
    std::string out="";
    long sz=in.length();
    long i=0;
    while(i<sz){
      if( ((in[i]>='0')&&(in[i]<='9')) || ((in[i]>='a')&&(in[i]<='z'))||((in[i]>='A')&&(in[i]<='Z'))){
        out+=in[i];
      }
      ++i;
    }
    return out;
  }
  std::vector<std::string> getItemsFromScrapedPage(const std::string & bigText, const std::string & s_B,const std::string & s_E, const int & clearSpaces=1){
    std::vector<std::string> fR=SF::stringToVector(bigText,s_B,s_E);
    long i=0, sz=fR.size();
    while(i<sz){
      fR[i]=numsAndLettersOnly(fR[i]);
      ++i;
    }
    return fR;
  }
struct StudentSearchData{
public:
    StudentData mData;
    std::string nameForSearch;
    std::string nameForDisplay;
    std::string semester;
    std::string course;
    long year;
    int operator<(const StudentSearchData& ) const;
};
int StudentSearchData::operator<(const StudentSearchData& oth) const{
    if(nameForSearch<oth.nameForSearch){return 1;}
    if(nameForSearch>oth.nameForSearch){return 0;}
    if(year<oth.year){return 1;}
    if(year>oth.year){return 0;}
    if(semester<oth.semester){return 1;}
    if(semester>oth.semester){return 0;}
    if(course<oth.course){return 1;}
    if(course>oth.course){return 0;}
    return 0;
}
}
#endif
