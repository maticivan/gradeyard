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


#ifndef _INCL_WI_FORMHANDLING_H
#define _INCL_WI_FORMHANDLING_H
namespace FHI{

  class FormField{
  private:
    std::string type;
    std::string availableDrawings;
    long codeLength;
    std::string nameField;
    std::string preFilledValue;
    std::string textBefore;
    std::string textAfter;

    std::string answerKeep;
    std::string challengeCode;

    std::string fileAnswerKeep;
    std::string fileQuestionImg;

    std::vector<std::string> allChoicesValues;
    std::vector<std::string> allChoicesDescriptions;
    std::string preSelection;


    long numR;
    long numC;
    std::string displayTextAreaField() const;
    std::string displayTextInputField() const;
    std::string displayRadioButtonsField() const;
    std::string displayFileInputField() const;
  public:
    FormField();
    void convertToTextArea(const std::string & = "noNameOrDescription", const std::string & = "noTextBefore", const std::string & = "", const long & = 20, const long & =90);
    void convertToRadioButtons(const std::string & ="noNameOrDescription",const std::string & = "noTextBefore", const std::string & = "noTextAfter", const std::string & = "", const std::string & = "");
    void convertToTextInput(const std::string & = "noNameOrDescription", const std::string & = "noTextBefore", const std::string & = "", const long & = 30);
    void convertToFileInput(const std::string & = "noNameOrDescription", const std::string & = "noTextBefore");
    void convertToAntiSpamChallenge(const std::string & = "noNameOrDescription", const std::string & = "noTextBefore", const long & = 10, const long & = 8);
    void setDimensions(const long & = 45, const long & =90);
    void setTextBefore(const std::string & );
    void setTextAfter(const std::string & );
    std::string getAnswerLocation() const;
    std::string displayField(const std::string &) const;
    std::string getType() const;
  };


  class InputForm{
  private:
    std::string enc_mfd="multipart/form-data";
    std::string enc_www="x-www-form-urlencoded";
    std::map<long, FormField> fields;
    std::string actionLink;
    std::string actionNameH,actionNameL;
    std::string buttonLabel;
    std::string antiSpamAnswerKeep;
    std::string redirectInfo;
    long nextFieldNum;
  public:
    InputForm(const std::string & ="index.cgi", const std::string & ="runH",const std::string & ="runL", const std::string & = MWII::GL_WI.getDefaultWebText("Submit"));
    void setMainVariables(const std::string & ="index.cgi", const std::string & ="runH",const std::string & ="runL", const std::string & = MWII::GL_WI.getDefaultWebText("Submit"));
    void addChangeField(const FormField & , const long & =-1);
    void setRedirectInfo(const std::string &);

    void clearForm();
    std::string displaySubmitButton(const std::string &, const std:: string &) const;
    std::string displayForm(const std::string & = "0", const std::string & = "notFound",const std::string & = "default") const;
    std::string displayAllFields(const std::string &) const;
  };

}

#endif
