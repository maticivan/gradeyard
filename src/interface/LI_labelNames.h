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


#ifndef _INCL_WI_LabelNames_H
#define _INCL_WI_LabelNames_H

namespace LI{
  struct Labels{
  public:
    std::string s_sysDataB="_systemData!!_";
    std::string s_sysDataE="_/systemData!!_";
    std::string s_tDataB="_textData!!_";
    std::string s_tDataE="_/textData!!_";
    std::string s_createdB="_created!!_";
    std::string s_createdE="_/created!!_";
    std::string s_modifiedB="_modified!!_";
    std::string s_modifiedE="_/modified!!_";
    std::string s_createdByB="_createdBy!!_";
    std::string s_createdByE="_/createdBy!!_";
    std::string s_modifiedByB="_modifiedBy!!_";
    std::string s_modifiedByE="_/modifiedBy!!_";

    std::string s_permissionStringB="_permissionString!!_";
    std::string s_permissionStringE="_/permissionString!!_";
    std::string s_docTypeStringB="_documentType!!_";
    std::string s_docTypeStringE="_/documentType!!_";


    std::string st_clea_sepUNameB="_u!_";
    std::string st_clea_sepUNameE="_/u!_";
    std::string st_clea_sepFNameB="_f!_";
    std::string st_clea_sepFNameE="_/f!_";
    std::string st_clea_sepLNameB="_l!_";
    std::string st_clea_sepLNameE="_/l!_";

    std::string st_clea_sepCodeB="_c!_";
    std::string st_clea_sepCodeE="_/c!_";

    std::string st_clea_sepGradeB="_g!_";
    std::string st_clea_sepGradeE="_/g!_";


    std::string st_sepUNameB="_user_";
    std::string st_sepUNameE="_/user_";
    std::string st_sepFNameB="_firstName_";
    std::string st_sepFNameE="_/firstName_";
    std::string st_sepLNameB="_lastName_";
    std::string st_sepLNameE="_/lastName_";

    std::string st_sepCreatedB="_created_";
    std::string st_sepCreatedE="_/created_";
    std::string st_sepCTextB="_couasText!*_";
    std::string st_sepCTextE="_/couasText!*_";
    std::string st_sepTitleB="_title!*_";
    std::string st_sepTitleE="_/title!*_";
    std::string st_sepMessTypeB="_eType!*_";
    std::string st_sepMessTypeE="_/eType!*_";
    std::string st_sepElementsB="_els!*_";
    std::string st_sepElementsE="_/els!*_";
    std::string st_sepElementsEncB="_elsEnc!*_";
    std::string st_sepElementsEncE="_/elsEnc!*_";
    std::string st_sepBossCB="_bossC!*_";
    std::string st_sepBossCE="_/bossC!*_";
    std::string st_sepBossNB="_bossN!*_";
    std::string st_sepBossNE="_/bossN!*_";
    std::string st_sepIndElB="_n!_";
    std::string st_sepIndElE="_/n!_";
    std::string st_sepGradeFormulaB="_grFormulas!*_";
    std::string st_sepGradeFormulaE="_/grFormulas!*_";

    std::string st_sepGradeFSB="_fScope!*_";
    std::string st_sepGradeFSE="_/fScope!*_";

    std::string st_sepGradeFMB="_fmath!*_";
    std::string st_sepGradeFME="_/fmath!*_";

    std::string st_sepGetFromB="_getFrom_";
    std::string st_sepGetFromE="_/getFrom_";

    std::string st_sepGrDisplB="_grDisplay!*_";
    std::string st_sepGrDisplE="_/grDisplay!*_";
    std::string st_sepLabelB="_label!_";
    std::string st_sepLabelE="_/label!_";
    std::string st_sepValueB="_value!_";
    std::string st_sepValueE="_/value!_";

    std::string st_sepAbbrevDispB="_disp_";
    std::string st_sepAbbrevDispE="_/disp_";

    std::string st_sepCNmTextB="_couasName!*_";
    std::string st_sepCNmTextE="_/couasName!*_";
    std::string defaultGradeName="{result}";

    std::string st_createMPermitsB="_createMessagePermits!*_";
    std::string st_createMPermitsE="_/createMessagePermits!*_";
    std::string st_subForumB="_subForum!*_";
    std::string st_subForumE="_/subForum!*_";


    std::string st_sep_vVPairB="_vVPair_";
    std::string st_sep_vVPairE="_/vVPair_";
    std::string st_sep_vrB="_vr_";
    std::string st_sep_vrE="_/vr_";
    std::string st_sep_vlB="_vl_";
    std::string st_sep_vlE="_/vl_";

    std::string st_sep_tnB="_templateName_";
    std::string st_sep_tnE="_/templateName_";
    std::string st_sep_tcB="_templateContent_";
    std::string st_sep_tcE="_/templateContent_";

    std::string st_sep_kB="_key_";
    std::string st_sep_kE="_/key_";
    std::string st_sep_vB="_val_";
    std::string st_sep_vE="_/val_";

    std::string st_sep_keB="_examExt_";
    std::string st_sep_keE="_/examExt_";
    std::string st_sep_veB="_genCode_";
    std::string st_sep_veE="_/genCode_";

    std::string st_envVName_Variable="ev";
    std::string st_envVName_Value="el";

    std::string st_envVName_ForwardVariable="fv";
    std::string st_envVName_ForwardValue="fl";
  } GL_LN;

}

#endif
