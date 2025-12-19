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

#ifndef _INCL_UMCI_CPP
#define _INCL_UMCI_CPP

namespace UMCI{
    int isContentAcceptable(const std::string& _c){
        std::set<std::string> dangers=GF::GL_DANGERS.strings;
        dangers.insert("!!_");
        if(MFRF::find(_c,dangers).second>-1){
          return 0;
        }
        return 1;
    }
    std::string prepareContentForEdit(const std::string& _mRaw,
                                      const std::string& _un){
        std::map<std::string,std::string> mM=SF::stringToMap(_mRaw,"_k_","_/k_","_v_","_/v_");
        std::map<std::string,std::string>::const_iterator it;
        it=mM.find(_un);
        if(it!=mM.end()){
            return it->second;
        }
        return "";
    }
    std::string createUMEditLink(const std::string& _pageRequested){
        std::string res;
        res+="<p>"+HSF::createButtonLink("index.cgi?page="+_pageRequested+"&eum=y","Edit")+"</p>";
        return res;
    }
    std::string createFinishedLink(const std::string& _pageRequested){
        std::string res;
        res+="<p>"+HSF::createButtonLink("index.cgi?page="+_pageRequested,"Click here")+" when you are finished editing.</p>";
        return res;
    }
    std::string userMapEdit(const PSDI::SessionData & _psd,
                            std::string userMapRaw,
                            const std::string& buttonName){
        if(_psd.my_un=="visitor"){return "";}
        std::map<std::string,std::string>::const_iterator it;
        it=_psd.respMap.find("eum");
        if((it!=_psd.respMap.end())&&(it->second=="y")){
            it=_psd.respMap.find("uMAnsw");
            if(it!=_psd.respMap.end()){
                if(isContentAcceptable(it->second)){
                    std::map<std::string,std::string> mUM;
                    mUM=SF::stringToMap(userMapRaw,"_k_","_/k_","_v_","_/v_");
                    mUM[_psd.my_un]=it->second;
                    if(it->second==""){
                        mUM.erase(_psd.my_un);
                    }
                    userMapRaw=SF::mapToString(mUM,"_k_","_/k_","_v_","_/v_");
                    if(mUM.size()<1){userMapRaw="emptyMap";}
                    TMD::MText sf;
                    if(sf.setFromTextName(_psd.pageRequested)!=0){
                        std::pair<std::string,int> allD;
                        long pos=0;
                        allD=SF::extractAndReplace(sf.getTextData(),pos,
                                                   LI::GL_LN.s_userMapB,
                                                   LI::GL_LN.s_userMapE,
                                                   0,
                                                   LI::GL_LN.s_userMapB
                                                   +userMapRaw
                                                   +LI::GL_LN.s_userMapE);
                        if(allD.second==1){
                            sf.setTextData(allD.first);
                            sf.putInDB();
                        }
                    }
                }
            }
            std::string formInit="",formEnd="", formMiddle="";
            formInit+="<div class=\"messageBox\"><FORM ACTION=\"";
            formInit+=MWII::GL_WI.getWSURL()+"/index.cgi?eum=y&"+MWII::GL_WI.get_e_parPage()+"="+_psd.pageRequested;
            formInit+="\" ENCTYPE=\"multipart/form-data";
            formInit+="\" METHOD=\"POST\">\n";
            formMiddle+="<div class=\"messageBox\">\n<P></P>\n<P>";
            formMiddle+="<textarea class=\"form-control\" name=\"uMAnsw\" rows=\"20\" ";
            formMiddle+="aria-label=\"text area for editing the content\">";
            formMiddle+=prepareContentForEdit(userMapRaw,_psd.my_un);
            formMiddle+="</textarea>\n</P>\n</div>\n";
            std::string bN=buttonName;
            if(bN==""){bN=MWII::GL_WI.getDefaultWebText("Submit");}
            formEnd+="<div>"+BI::submButton("mbs",bN);
            formEnd+="</div></FORM> </div>\n";
            return formInit+formMiddle+formEnd+createFinishedLink(_psd.pageRequested);
        }
        return createUMEditLink(_psd.pageRequested);
    }
    std::string prepareForDisplay(const std::string& uN, const std::string& uText){
        return "<h4>"+uN+"</h4>"+uText;
    }
    std::string userMapDisplay(const PSDI::SessionData & _psd,
                               const std::string& userMapRaw,
                               const std::string& request){
        std::map<std::string,std::string>::const_iterator it;
        it=_psd.respMap.find("eum");
        if(it!=_psd.respMap.end()){return "";}
        std::string res;
        std::string itemStart="\n\\begin{box}";
        std::string itemEnd="\\end{box}\n";
        std::map<std::string,std::string> mUM=SF::stringToMap(userMapRaw,"_k_","_/k_","_v_","_/v_");
        if(request=="all"){
            it=mUM.begin();
            while(it!=mUM.end()){
                res+=itemStart+prepareForDisplay(it->first,it->second)+itemEnd;
                ++it;
            }
            return res;
        }
        if(_psd.my_un=="visitor"){return "";}
        it=mUM.find(_psd.my_un);
        if(it==mUM.end()){
            return "";
        }
        if(request=="totp"){
            return TOTPF::prepareTOTPDisplay(it->second);
        }
        return itemStart+prepareForDisplay(it->first,it->second)+itemEnd;
    }
}
#endif
