//    GradeYard learning management system
//
//    Copyright (C) 2025 Ivan Matic, https://gradeyard.com
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


#ifndef _INCL_WSCI_websiteCommunications_CPP
#define _INCL_WSCI_websiteCommunications_CPP

namespace WSCI{
static size_t fCalculator(void *p, size_t s, size_t m, void *u){
 ((std::string*)u)->append((char*)p, s * m);
 return s * m;
}
std::string readFromURL(const std::string & mainLink){
 CURL *curl;
 std::string result;
 curl = curl_easy_init();
 if(curl) {
   const char* linkArrayChar=mainLink.c_str();
   curl_easy_setopt(curl, CURLOPT_URL, linkArrayChar);
   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fCalculator);
   curl_easy_setopt(curl, CURLOPT_WRITEDATA, & result);
   curl_easy_perform(curl);
   curl_easy_cleanup(curl);
 }
 return result;
}
std::vector<std::string> getWSCHeader(const std::string& in){
    std::vector<std::string> res=SF::stringToVector(in,"_ADVWSCHEADER_","_/ADVWSCHEADER_");
    if(res.size()<1){
        std::vector<std::string> empty;
        return empty;
    }
    return SF::stringToVector(res[0],"_n_","_/n_");
}
std::string prepareDenial(const std::vector<std::string>& v){
    std::string res;
    res="<h2>Denied access</h2>\n";
    res+="<p>External web resource has denied access.</p>";
    if(v.size()<1){
        return res;
    }
    res+="<p>IP address: "+v[0]+"</p>\n";
    res+="<p>Ask the webmaster of the external website to add the above ip address ";
    res+="to the list of approved guests.</p>";
    return res;
}
std::string processText(const std::string& in){
    std::vector<std::string> wscHeader=getWSCHeader(in);
    std::vector<std::string> v=SF::stringToVector(in,"<pre>ADVWSC</pre>","<pre>/ADVWSC</pre>");
    if(v.size()>0){
        if(v[0]!="Denied"){return v[0];}
        return prepareDenial(wscHeader);
    }
    return in;
}
std::string readExternalWebsite(const std::string& extURL){
    return processText(readFromURL(extURL));
}
std::string updateIMG(const std::string& wsURL, const std::string& in){
    return SF::findAndReplace(in,"src=\"img/","src=\""+wsURL+"/img/");
}
std::string prepareForGuests(const std::string& wsURL, const std::string& in){
    std::string out=in;
    out=updateIMG(wsURL,in);
    return out;
}
  std::string websiteRequest(const PSDI::SessionData & _psd,const std::string& approvedGuests, MPTI::MainText& mainText){
    std::map<std::string,std::string>::const_iterator itRM=_psd.respMap.find("tu");
    if(itRM==_psd.respMap.end()){return "";}
    if(approvedGuests==""){return "no";}
    TMD::MText pDocument;
    int scc=pDocument.setFromTextName(approvedGuests);
    if(scc==0){return "no";}
    std::set<std::string> tV=SF::stringToSet(pDocument.getTextData(),"_websiteGuest_","_/websiteGuest_");
    if(tV.find(_psd.remAddr)==tV.end()){
        return "no";
    }
    return "yes";
  }
}
#endif
