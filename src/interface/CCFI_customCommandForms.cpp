//    GradeYard learning management system
//
//    Copyright (C) 2022 Ivan Matic, https://gradeyard.com
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


#ifndef _INCL_WI_CUSTOMCOMMANDFORMS_CPP
#define _INCL_WI_CUSTOMCOMMANDFORMS_CPP

namespace CCFI{
  class ArgsPowerModifyForm{
  public:
    std::string textToModify;
    std::string linkForm;
    std::string modifyMe;
    std::string dbName;
    std::string textDoNotEdit="DO NOT EDIT THIS LINE";
    std::string textStartEdit="START EDITING AFTER THIS LINE";
    std::string editMessage=MWII::GL_WI.getDefaultWebText("Edit text by editing the command below");
    std::string editMessageW=" ";
    std::string textNameLabel="Text name:";
  };
  std::string createSimpleFormForTextEdit(const ArgsPowerModifyForm & apmf){
    std::string fR="_insert_\n";
    fR+="_n*_formInitialization_/n*_\n";
    fR+="_n*_reqCommand_/n*_\n";
    fR+="_n*_";
    fR+=apmf.linkForm+"&ccu=y";
    fR+="_/n*_\n";
    fR+="_/insert__insert__n*_textAreaField_/n*__n*_reqCommand_/n*__n*_commands_/n*_\n";
    fR+="_n*_"+apmf.editMessage+"_/n*_\n";
    fR+="_n*_\n";
    fR+=apmf.textToModify;
    fR+="_/n*__/insert_";
    fR+="_insert_";
    fR+="_n*_formPlacement_/n*_";
    fR+="_n*_reqCommand_/n*_";
    fR+="_/insert_ \n";
    fR=SF::findAndReplace(fR,"!verbSave!","*fjkl3"+GL_MAIN_SETUP_FILE_NAME+"2!3211");
    return fR;
  }
  std::string createCustomCommandForm(const ArgsPowerModifyForm & apmf){
    std::string fR="_insert_\n";
    fR+="_n*_formInitialization_/n*_\n";
    fR+="_n*_reqCommand_/n*_\n";
    fR+="_n*_";
    fR+=apmf.linkForm;
    fR+="_/n*_\n";
    fR+="_/insert__insert__n*_textAreaField_/n*__n*_reqCommand_/n*__n*_commands_/n*_\n";
    fR+="_n*_"+apmf.editMessage+"_/n*_\n";
    fR+="_n*_\n";
    fR+=apmf.textDoNotEdit;
    fR+="_command_\n"+apmf.textDoNotEdit+"_nc***_";
    fR+=apmf.modifyMe;
    fR+="_/nc***_\n";
    fR+=apmf.textDoNotEdit+" "+apmf.textNameLabel;
    fR+="_nc***_"+apmf.dbName+"_/nc***_\n";
    fR+=apmf.textStartEdit;
    fR+="_nc***_";
    fR+=apmf.textToModify;
    fR+="_/nc***_\n";
    fR+=apmf.textDoNotEdit;
    fR+="_/command_";
    fR+="_/n*__/insert_";
    fR+="_insert_";
    fR+="_n*_formPlacement_/n*_";
    fR+="_n*_reqCommand_/n*_";
    fR+="_/insert_ \n";
    fR=SF::findAndReplace(fR,"!verbSave!","*fjkl3"+GL_MAIN_SETUP_FILE_NAME+"2!3211");
    return fR;
  }
  std::string enhanceTheCommandDueToComfUserEdit(const PSDI::SessionData  & _psd, const std::string & c){
    std::string enh="_command_\n_nc***_modifyText_/nc***_\n_nc***_"+_psd.pageRequested+"_/nc***_\n";
    enh+="_nc***_"+c+"_/nc***_\n_/command_";
    return enh;
  }
  std::string createRRBackupActivationText(const std::string & name, const std::string & action, const std::string & text){
    std::string res="DO NOT EDIT THIS LINE_command_\n";
    res+="DO NOT EDIT THIS LINE_nc***_"+action+"_/nc***_\n";
    res+="DO NOT EDIT THIS LINE Text name:_nc***_"+name+"_/nc***_";
    res+="START EDITING AFTER THIS LINE_nc***_\n";
    res+=text;
    res+="_/nc***_\n";
    res+="DO NOT EDIT THIS LINE_/command_\n";
    return res;
  }
}

#endif
