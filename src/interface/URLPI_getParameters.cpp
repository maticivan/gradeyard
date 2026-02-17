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


#ifndef _INCL_URLPI_GetParameters_CPP
#define _INCL_URLPI_GetParameters_CPP

namespace URLPI{
void initializeAttributes(PSDI::SessionData& psd){
    psd.my_un="visitor";
    psd.myFirstName="";
    psd.myLastName="";
    psd.isRoot="no";
    psd.isGradingAdmin="no";
    psd.allowedToExecuteAll="no";
    psd.pEditReq="no";
    psd.respRecMode="df";
    psd.respRecFlag="notReceived";
    psd.versionToLatex="0";
    psd.messEditReq="no";
    psd.sortCriterion="0";
    psd.couasEditReq="no";
    psd.rrgrader="no";
    psd.indChangeRespRecToPrintVersionOfCommonInClassExam=0;
    psd.probVersionsOfChangedRespRec="";
    psd.pdfNameForInclassExam="";
    psd.masterKey="";
    psd.queryAnswRequired=0;
    psd.queryAnswPlaceHolder="_*"+RNDF::genRandCode(15)+"_*qAsnw_";
    psd.displayDaysInWeek.resize(0);
    psd.displayMonthsInYear.resize(0);
    psd.loginActionIndicator=0;
    psd.loginStatusIndicator=0;
    psd.loginFailedIndicator=0;
    psd.inFormSearch="";
    psd.inFormReplace="";
    psd.messEditCode="";
    psd.idOfMessageWhoseEditWasSubmitted="";
    psd.newTextOfMessage="";
    psd.passwordChangeRequested="no";
    psd.passwordChangeStatus="";
    psd.allowRespRecDisplayToOthers="no";
    psd.pageRequested="";
    psd.comfUserEdit="n";
    psd.startOfList="";
    psd.addModFileReq="nothing";
    psd.addModFileCodeReq="";
    psd.respRecRequested="";
    psd.respSubmitted="no";
    psd.sysDataRequested="no17";
    psd.debuggingEnvVarRequest="no";
    psd.debuggingModeRequest="no";
    psd.addModFileModifyInfo="no";
}
void fillOneVariable(std::string& toFill,
                     const std::map<std::string,std::string>& mMap,
                     const std::string& key){
    std::map<std::string,std::string>::const_iterator it=mMap.find(key);
    if(it!=mMap.end()){
        toFill=it->second;
    }
}
PSDI::SessionData getURLParameters(const cgicc::Cgicc & ch){
    PSDI::SessionData psd;
    initializeAttributes(psd);
    cgicc::const_form_iterator it=ch.getElements().begin();
    while(it!=ch.getElements().end()){
        (psd.respMap)[it->getName()]=it->getValue();
        ++it;
    }
    std::map<std::string,std::string>::const_iterator itM;
    fillOneVariable(psd.pageRequested,psd.respMap,MWII::GL_WI.get_e_parPage());
    fillOneVariable(psd.respRecRequested,psd.respMap,MWII::GL_WI.get_e_respRecReqRT());
    itM=itM=psd.respMap.find(MWII::GL_WI.get_e_respSubmitReqRT());
    if(itM!=psd.respMap.end()){
        if(itM->second==MWII::GL_WI.get_e_respSubmitSuccRT()){
            psd.respSubmitted="yes";
        }
    }
    itM=psd.respMap.find(LI::GL_LN.e_searchFor);
    if(itM!=psd.respMap.end()){
        psd.inFormSearch=SACF::sanitizeInFormSearchFor(itM->second);
    }
    itM=psd.respMap.find(LI::GL_LN.e_replaceWith);
    if(itM!=psd.respMap.end()){
        DISPPF::RequestsForSanitizer reqS;
        psd.inFormReplace=DISPPF::sanitizeForDisplay(itM->second,reqS);
    }
    fillOneVariable(psd.messEditCode,psd.respMap,LI::GL_LN.e_messToEdit);
    fillOneVariable(psd.idOfMessageWhoseEditWasSubmitted,psd.respMap,LI::GL_LN.e_messEditSubmission);
    fillOneVariable(psd.newTextOfMessage,psd.respMap,LI::GL_LN.e_newTextWithMessage);
    fillOneVariable(psd.respRecMode,psd.respMap,LI::GL_LN.e_responseReceiverDisplayRequest);
    fillOneVariable(psd.versionToLatex,psd.respMap,LI::GL_LN.e_verReq);
    fillOneVariable(psd.startOfList,psd.respMap,LI::GL_LN.e_startOfList);
    fillOneVariable(psd.sortCriterion,psd.respMap,LI::GL_LN.e_sortCriterion);
    fillOneVariable(psd.addModFileReq,psd.respMap,LI::GL_LN.e_addModFileReq);
    fillOneVariable(psd.addModFileCodeReq,psd.respMap,LI::GL_LN.e_addModFileCodeReq);
    fillOneVariable(psd.addModFileModifyInfo,psd.respMap,LI::GL_LN.e_addModFileModifyInfo);
    fillOneVariable(psd.sysDataRequested,psd.respMap,LI::GL_LN.e_systemDataDisplayRequest);
    fillOneVariable(psd.debuggingEnvVarRequest,psd.respMap,LI::GL_LN.e_debEnvVarRequest);
    fillOneVariable(psd.debuggingModeRequest,psd.respMap,LI::GL_LN.e_debModeRequest);
    itM=psd.respMap.find(LI::GL_LN.e_redirectOverwrite);
    if(itM!=psd.respMap.end()){
        MWII::GL_WI.setRedirectOverwrite(itM->second);
    }
    itM=psd.respMap.find(LI::GL_LN.e_redirectToForward);
    if(itM!=psd.respMap.end()){
        MWII::GL_WI.setRedirectForward(itM->second);
    }
    itM=psd.respMap.find(LI::GL_LN.e_editReq);
    if(itM!=psd.respMap.end()){
        if( (itM->second==LI::GL_LN.m_editReqY)
            ||
            (itM->second==LI::GL_LN.m_editReqR)
            ||
            (itM->second==LI::GL_LN.m_editReqW) ){
            psd.pEditReq=itM->second;
        }
    }
    itM=psd.respMap.find(LI::GL_LN.e_editMessReq);
    if(itM!=psd.respMap.end()){
        if( (itM->second==LI::GL_LN.m_editReqY)
            ||
            (itM->second==LI::GL_LN.m_editReqR) ){
            psd.messEditReq=itM->second;
        }
    }
    fillOneVariable(psd.couasEditReq,psd.respMap,LI::GL_LN.e_editCouasReq);
    fillOneVariable(psd.comfUserEdit,psd.respMap,LI::GL_LN.e_confirmationOfComfortableUserEdit);
    fillOneVariable(psd.passwordChangeRequested,psd.respMap,LI::GL_LN.e_passwCh);
    fillOneVariable(psd.rrgrader,psd.respMap,LI::GL_LN.e_respRecThatWillGradeCouas);
    itM=psd.respMap.find(LI::GL_LN.e_userLogin);
    if(itM!=psd.respMap.end()){
        if(itM->second==LI::GL_LN.m_logIn){
            psd.loginActionIndicator=1;
        }
        else{
            psd.loginActionIndicator=-1;
        }
    }
    return psd;
}
}
#endif
