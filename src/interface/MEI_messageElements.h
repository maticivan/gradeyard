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


#ifndef _INCL_WI_MElement_H
#define _INCL_WI_MElement_H

namespace MEI{

  std::set<std::string> GL_MESSAGES_ALLOWED_TO_READ;
  std::set<std::string> GL_MESSAGES_NOT_ALLOWED_TO_READ;
  std::set<std::string> GL_MESSAGES_ALLOWED_TO_WRITE;
  std::set<std::string> GL_MESSAGES_NOT_ALLOWED_TO_WRITE;

  class MElement{
  private:
    std::string st_sepUNameB="_user_";
    std::string st_sepUNameE="_/user_";
    std::string st_sepFNameB="_firstName_";
    std::string st_sepFNameE="_/firstName_";
    std::string st_sepLNameB="_lastName_";
    std::string st_sepLNameE="_/lastName_";
    std::string st_sepNavB="_navigation_";
    std::string st_sepNavE="_/navigation_";

    std::string st_sepCreatedB="_created_";
    std::string st_sepCreatedE="_/created_";
    std::string st_sepMTextB="_messText!*_";
    std::string st_sepMTextE="_/messText!*_";
    std::string st_sepTitleB="_title!*_";
    std::string st_sepTitleE="_/title!*_";
    std::string st_sepMessTypeB="_mType!*_";
    std::string st_sepMessTypeE="_/mType!*_";
    std::string st_sepAddMessB="_createMessagePermits!*_";
    std::string st_sepAddMessE="_/createMessagePermits!*_";
    std::string st_sepElementsB="_els!*_";
    std::string st_sepElementsE="_/els!*_";

    std::string s_permissionStringB="_permissionString!!_";
    std::string s_permissionStringE="_/permissionString!!_";

    std::string s_individualPermissionB="_permission_";
    std::string s_individualPermissionE="_/permission_";

    std::set<std::string> permitRead;
    std::set<std::string> permitWrite;
    std::set<std::string> permitAdd;
    std::string mUserN;
    std::string mFirstN;
    std::string mLastN;
    std::string navigationSt;
    std::string mCreated;
    std::string mText;
    std::string mType;
    std::string mElements;
    std::string mtTitle;
    std::string myCode;
    std::string rawText;
    std::vector<std::string> mElCodes;
    std::vector<std::string> mNavEls;
    long nmCodes;
    long nmNavEls;
    std::string createLinkToTopic(const std::string & , const std::string &, const std::string &) const;
    std::string prepareEditBox(const std::string & ,const std::string &, const std::string & , const std::string &) const;
    std::string prepareMT(const PSDI::SessionData &, const std::string &, const std::string &, const std::string & , const std::string &, const int &,const std::string &) const;

  public:
    MElement(const std::string & = "", const std::string & = "",const std::string & = "", const std::string & = "");
    int initialize(const std::string & , const std::string & , const std::string &,const std::string & );
    std::string createMTEditLink(const std::string & , const std::string & , const std::string & , const std::string & , const std::string &) const;
    std::string prepareIndividualTopic(const PSDI::SessionData &, const std::string &, const std::string & , const std::string &, const int &, const std::string &) const;
    std::string prepareIndividualMessage(const PSDI::SessionData &, const std::string &,const std::string & , const std::string &, const int &, const std::string & ) const;
    std::string newMessageBoxOrLink(const std::string &,const long &,const std::string &) const;
    std::string navString() const;
    std::string getRawText() const;
    long numCodes() const;
    long numNavEls() const;
    std::string code(const long & ) const;
    std::string navEl(const long &) const;
    std::string type() const;
    std::string title() const;
    std::string text() const;
    std::string navigationBar(const std::string &) const;
    int allowedToRead(const std::string & ) const;
    int allowedToWrite(const std::string & ) const;
    int allowedToCreateMessage(const std::string & ) const;
    int allowedToModifyMessage(const std::string & ) const;
  };
}


#endif
