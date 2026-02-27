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

#ifndef _INCL_QSCI_quickSpamCheck_CPP
#define _INCL_QSCI_quickSpamCheck_CPP

namespace QSCI{
long GL_spammerDataVSizeAcceptableForOldVersion=2;
long GL_spammerDataVSize=5;
long GL_deniedAccessDataVSize=2;
long GL_timeVectorSz=6;
long GL_numDaysToKeepTrackOfBans=30;
long GL_maxMapSizeForServer=1000000;
long GL_maxSizeForReport=1000;
std::string GL_rawPasswordOT="[^;*]";
std::string GL_rawPasswordCT="[/^;*]";
long warningScoreThatTranslatesIntoBan(){
    return HDPF::GLOBAL_PS.get_loginFailsSpammer();
}
std::vector<long> cutVector(const std::vector<long>& v, long sz){
    std::vector<long> res;
    res.resize(sz);
    for(long i=0;i<sz;++i){
        res[i]=0;
        if(i<v.size()){
            res[i]=v[i];
        }
    }
    return res;
}
struct AccessDeniedLogs{
public:
    long totalCounter;
    std::map<std::vector<long>,long> lastDays;
    std::string toString() const;
    std::string displayString(long =-1) const;
    void fromString(const std::string& );
    void increment(const std::vector<long>&);
    void increment();
    AccessDeniedLogs();
};
AccessDeniedLogs::AccessDeniedLogs(){
    totalCounter=0;
}
std::string AccessDeniedLogs::toString() const{
    std::string res;
    res+="_n_"+std::to_string(totalCounter)+"_/n_\n";
    res+="_n_\n";
    std::map<std::vector<long>,long>::const_iterator it=lastDays.begin();
    while(it!=lastDays.end()){
        res+="_k_"+SF::vectorToString(it->first,"_n_","_/n_")+"_/k_";
        res+="_v_"+std::to_string(it->second)+"_/v_\n";
        ++it;
    }
    res+="_/n_\n";
    return res;
}
std::string oneTable(const std::vector<std::pair<std::string,std::string> >& v){
    std::string res;
    res+="<table class=\"table table-striped\">\n";
    res+="<thead class=\"table-dark\">\n<tr>\n";
    for(long i=0;i<v.size();++i){
        res+="<th scope=\"col\">"+v[i].first+"</th>";
    }
    res+="</tr>\n</thead>\n<tbody>\n<tr>\n";
    for(long i=0;i<v.size();++i){
        res+="<td>"+v[i].second+"</td>";
    }
    res+="</tr></tbody>\n</table>\n";
    return res;
}
std::string displayDate(const std::vector<long>& _d){
    std::string res;
    for(long i=0;i<_d.size();++i){
        if(i>0){res+="/";}
        res+=std::to_string(_d[i]+(i==1));
    }
    return res;
}
std::string AccessDeniedLogs::displayString(long numPastDaysToShow) const{
    if((numPastDaysToShow < 0)||(numPastDaysToShow>lastDays.size())){
        numPastDaysToShow = lastDays.size();
    }
    std::string res;
    res+="<h1>Denied Access</h1><h2>Total: "+std::to_string(totalCounter)+"</h2>\n";
    res+="<h2>Last "+std::to_string(GL_numDaysToKeepTrackOfBans)+" days</h2>\n";
    if(lastDays.size()>0){
        std::map<std::vector<long>,long>::const_iterator it=lastDays.end();
        --it;
        std::vector<std::pair<std::string,std::string> > oneRow;
        oneRow.resize(7);
        long column=0;
        while(numPastDaysToShow>0){
            --numPastDaysToShow;
            oneRow[column].first=displayDate(it->first);
            oneRow[column].second=std::to_string(it->second);
            ++column;
            if(column==7){
                res+=oneTable(oneRow);
                for(long i=0;i<7;++i){
                    oneRow[i].first="";
                    oneRow[i].second="";
                }
                column=0;
            }
            if(it==lastDays.begin()){
                numPastDaysToShow=0;
            }
            else{
                --it;
            }
        }
        if(column>0){
            res+=oneTable(oneRow);
        }
    }
    return res;
}
std::map<std::vector<long>,long> stringToMapKeyVLValL(const std::string & everything,
                                                      const std::string & sepKeyB,
                                                      const std::string & sepKeyE,
                                                      const std::string & sepValB,
                                                      const std::string & sepValE){
    std::map<std::vector<long>,long> fR;
    int stillWorking=1;
    std::pair<std::string,int> allD; long pos=0;
    std::vector<long> nextKey;
    while(stillWorking){
        allD=SF::extract(everything,pos,sepKeyB,sepKeyE);
        if(allD.second==1){
            nextKey=SF::stringToVectorLong(allD.first,"_n_","_/n_");
            allD=SF::extract(everything,pos,sepValB,sepValE);
            if(allD.second==1){
                fR[nextKey]=BF::stringToInteger(allD.first);
            }
            else{
                stillWorking=0;
            }
        }
        else{
            stillWorking=0;
        }
    }
    return fR;
}
void AccessDeniedLogs::fromString(const std::string& _s){
    std::vector<std::string> vT=SF::stringToVector(_s,"_n_","_/n_");
    if(vT.size()!=GL_deniedAccessDataVSize){return ;}
    totalCounter=BF::stringToInteger(vT[0]);
    lastDays=stringToMapKeyVLValL(_s,"_k_","_/k_","_v_","_/v_");
}
template<typename K, typename V>
void trimTheMapFromBeginning(std::map<K,V>& m,long desiredSize){
    while(m.size()>desiredSize){m.erase(m.begin()->first);}
}
void AccessDeniedLogs::increment(const std::vector<long>& _today){
    ++totalCounter;
    std::vector<long> today;
    if(_today.size()<3){
        TMF::Timer tm;
        today=cutVector(tm.timeNowVector(),3);
    }
    else{
        if(_today.size()>3){
            today=cutVector(_today,3);
        }
        else{
            today=_today;
        }
    }
    std::map<std::vector<long>,long>::const_iterator it=lastDays.find(today);
    if(it==lastDays.end()){
        trimTheMapFromBeginning(lastDays,GL_numDaysToKeepTrackOfBans-1);
        lastDays[today]=1;
    }
    else{
        lastDays[today]=(it->second)+1;
    }
}
void AccessDeniedLogs::increment(){
    std::vector<long> empty;
    increment(empty);
}

    struct SpammerData{
    public:
        std::vector<long> timeVector;
        //(year,month,day,hour,minute,second)
        long warningScore;
        std::string userName;
        std::string password;
        std::string commentWithSubmittedData;
        std::string toString() const;
        void fromString(const std::string& );
        SpammerData();
    };
    SpammerData::SpammerData(){
        warningScore=0; 
    }
std::string hidePasswordIfNecessary(const std::string& _u,
                                    const std::string& _p){
    std::string spr=HDPF::GLOBAL_PS.get_spammerPasswordReplacement();
    if(spr!=""){
        return spr;
    }
    if(HDPF::GLOBAL_PS.check_privacyProtection(_u)){
        return "privacy";
    }
    return _p;
}
std::string hidePasswordsIfNecessary(const std::string& in){
    std::string spr=HDPF::GLOBAL_PS.get_spammerPasswordReplacement();
    if(spr==""){
        std::map<std::string,std::string> replMap;
        replMap[GL_rawPasswordOT]="";
        replMap[GL_rawPasswordCT]="";
        return MFRF::findAndReplace(in,replMap);
    }
    std::set<std::string> mS=SF::stringToSet(in,
                                             GL_rawPasswordOT,
                                             GL_rawPasswordCT);
    std::map<std::string,std::string> replMap;
    std::set<std::string>::const_iterator it=mS.begin();
    while(it!=mS.end()){
        replMap[GL_rawPasswordOT+(*it)+GL_rawPasswordCT]=spr;
        ++it;
    }
    return MFRF::findAndReplace(in,replMap);
}
std::string sanitize(const std::string& in){
    std::map<std::string,std::string> replMap;
    replMap["_n_"]="*oN*";
    replMap["_/n_"]="*cN*";
    replMap["<"]="*oT*";
    replMap[">"]="*cT*";
    replMap["|"]="*verticalBar*";
    return MFRF::findAndReplace(in,replMap);
}
    std::string SpammerData::toString() const{
        return "_n_"+std::to_string(warningScore)+"_/n__n_"+SF::vectorToString(timeVector,"_n_","_/n_")+"_/n__n_"+sanitize(userName)
        +"_/n__n_"+
        sanitize(hidePasswordIfNecessary(userName,password))
        +
        "_/n_\n_n_"+sanitize(commentWithSubmittedData)+"_/n_";
    }
    void SpammerData::fromString(const std::string& _s){
        warningScore=0;
        timeVector.resize(GL_timeVectorSz);
        for(long i=0;i<GL_timeVectorSz;++i){timeVector[i]=0;}
        std::vector<std::string> rV=SF::stringToVector(_s,"_n_","_/n_");
        if(rV.size()==GL_spammerDataVSizeAcceptableForOldVersion){
            rV.resize(GL_spammerDataVSize);
        }
        if(rV.size()==GL_spammerDataVSize){
            warningScore=BF::stringToInteger(rV[0]);
            std::vector<std::string> tvS=SF::stringToVector(rV[1],"_n_","_/n_");
            if(tvS.size()==GL_timeVectorSz){
                for(long i=0;i<GL_timeVectorSz;++i){
                    timeVector[i]=BF::stringToInteger(tvS[i]);
                }
            }
            userName=rV[2];
            password=rV[3];
            commentWithSubmittedData=rV[4];
        }
    }
std::string makeReport(const std::map<std::string,std::string>& postMap){
    std::string res;
    std::map<std::string,std::string>::const_iterator it=postMap.begin();
    while((res.size()<GL_maxSizeForReport) && (it!=postMap.end())){
        if((it->first!="username")&&(it->first!="pass1")){
            res+="["+it->first+"]="+it->second+"\n";
        }
        ++it;
    }
    if(res.size()>GL_maxSizeForReport){
        res.resize(GL_maxSizeForReport);
    }
    return res;
}
SpammerData getWarningData(long loginPageAccess,
                           long loginSubmission,
                           long registrationAccess,
                           long registrationSubmission,
                           const std::map<std::string,std::string>& postMap){
    SpammerData sD;
    sD.warningScore+=loginPageAccess+loginSubmission;
    sD.warningScore+=registrationAccess+registrationSubmission;
    if(sD.warningScore>0){
        TMF::Timer tm;
        sD.timeVector=cutVector(tm.timeNowVector(),6);
        sD.warningScore=1;
    }
    std::map<std::string,std::string>::const_iterator it;
    it=postMap.find("username");
    if(it!=postMap.end()){
        sD.userName=it->second;
    }
    it=postMap.find("pass1");
    if(it!=postMap.end()){
        sD.password=GL_rawPasswordOT+it->second+GL_rawPasswordCT;
    }
    sD.commentWithSubmittedData=makeReport(postMap);
    return sD;
}
std::map<std::string,SpammerData> getSpammerData(const std::string& _s,
                                                 long dataExpirationSec){
    std::map<std::string,std::string> sM=SF::stringToMap(_s,"_k_","_/k_","_v_","_/v_");
    std::map<std::string,SpammerData> res;
    std::map<std::string,std::string>::const_iterator it=sM.begin();
    TMF::Timer tm;
    long currentSeconds=TMF::getSecondsSinceYYYY(tm.timeNowVector());
    long counter=0;
    while(it!=sM.end()){
        SpammerData tmp;
        tmp.fromString(it->second);
        if(currentSeconds-TMF::getSecondsSinceYYYY(tmp.timeVector)<dataExpirationSec){
            if(counter<GL_maxMapSizeForServer){
                ++counter;
                res[it->first]=tmp;
            }
        }
        ++it;
    }
    return res;
}
std::string printRawWarnings(const std::string& _w){
    std::string res;
    res+="<div>_hideReveal__revealTitle_Show raw warnings_/revealTitle_";
    res+="_hideTitle_Hide raw warnings_/hideTitle_";
    std::string w=hidePasswordsIfNecessary(_w);
    res+="<pre>"+SF::findAndReplace(w,"_k_","\n_k_")+"</pre>";
    res+="_/hideReveal_\n</div>\n";
    return res;
}
std::string deniedAccessSummary(){
    std::string rawStatFolder=HDPF::GLOBAL_PS.getRSFolderName();
    if(IOF::sys_folderExists(rawStatFolder)==0){
        return "";
    }
    std::string fSpamBans=rawStatFolder+"/spammerDeniedLogs.txt";
    AccessDeniedLogs adLogs;
    adLogs.fromString(IOF::fileToString(fSpamBans));
    std::string fSpamWarnings=rawStatFolder+"/spammerWarnings.txt";
    return adLogs.displayString()+
            printRawWarnings(IOF::fileToString(fSpamWarnings));
}
std::map<std::string,std::string> toStringStringMap(const std::map<std::string,SpammerData>& _m){
    std::map<std::string,std::string> res;
    std::map<std::string,SpammerData>::const_iterator it=_m.begin();
    while(it!=_m.end()){
        res[it->first]=(it->second).toString();
        ++it;
    }
    return res;
}
  std::string messageForSpammer(const cgicc::Cgicc & ch,
                                const std::vector<std::string>& envV,
                                const std::map<std::string,std::string>& postMap){
      //returns "" if not spammer
      // otherwise returns the message for spammer
      if(HDPF::GLOBAL_PS.get_loginFailsSpammer()==0){
          // Spam checking is intentionally turned off
          return "";
      }
      std::string rawStatFolder=HDPF::GLOBAL_PS.getRSFolderName();
      if(IOF::sys_folderExists(rawStatFolder)==0){
          // The website doesn't exist yet. We don't want to interfere with
          // website initialization
          return "";
      }
      std::string currentCookie=envV[24];
      if(currentCookie.length()>10){
          // The user has a cookie. Should not be banned.
          return "";
      }
      std::string fSpamWarnings=rawStatFolder+"/spammerWarnings.txt";
      std::string fSpamBans=rawStatFolder+"/spammerDeniedLogs.txt";
      cgicc::const_form_iterator it, itE;
      itE=ch.getElements().end();
      long loginPageAccess=0;
      long loginSubmission=0;
      long registrationAccess=0;
      long registrationSubmission=0;
      std::string ipAddr=envV[11];
      it=ch.getElement(MWII::GL_WI.get_e_parPage());
      if(it!=itE){
          if(it->getValue()=="login"){loginPageAccess=1;}
          if(it->getValue()=="registration"){registrationAccess=1;}
      }
      it=ch.getElement("userLogIn");
      if(it!=itE){loginSubmission=1;}
      it=ch.getElement("userReg"); 
      if(it!=itE){registrationSubmission=1;}
      std::map<std::string,SpammerData>
      pastData=getSpammerData(IOF::fileToString(fSpamWarnings),
                              HDPF::GLOBAL_PS.get_loginTimeExpirationSpammer());
      SpammerData currentData=getWarningData(loginPageAccess,
                                             loginSubmission,
                                             registrationAccess,
                                             registrationSubmission,
                                             postMap);
      GF::GL_DEB_MESSAGES.addMessage("Spammer Data for file: "+
                                     currentData.toString());
      std::map<std::string,SpammerData>::const_iterator itSM;
      itSM=pastData.find(ipAddr);
      long shouldBeBanned=0;
      if(itSM!=pastData.end()){
          if(itSM->second.warningScore
               >=
             HDPF::GLOBAL_PS.get_loginFailsSpammer() ){
              currentData.warningScore =
              HDPF::GLOBAL_PS.get_loginFailsSpammer();
              shouldBeBanned=1;
          }
          else{
              if(currentData.warningScore>0){
                  currentData.warningScore+=itSM->second.warningScore;
                  if(currentData.warningScore>=HDPF::GLOBAL_PS.get_loginFailsSpammer()){
                      currentData.warningScore=HDPF::GLOBAL_PS.get_loginFailsSpammer();
                      shouldBeBanned=1;
                  }
              }
          }
      }
      if(currentData.warningScore>0){
          pastData[ipAddr]=currentData;
          IOF::toFile(fSpamWarnings,SF::mapToString(toStringStringMap(pastData),"","","_k_","_/k_","_v_","_/v_"));
      }
      if(shouldBeBanned==0){
          return "";
      }
      AccessDeniedLogs adLogs;
      adLogs.fromString(IOF::fileToString(fSpamBans));
      adLogs.increment();
      IOF::toFile(fSpamBans,adLogs.toString());
      return HDPF::GLOBAL_PS.get_htmlForSpammer();
  }
}
#endif
