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


#ifndef _INCL_WI_MAINWI_CPP
#define _INCL_WI_MAINWI_CPP

#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include "WSI_wiSetup.cpp"
namespace MWII{
  WSI::Setup GL_WI;
  std::string FILE_PREFIX="f";
  long GL_MAX_FILE_RESP_REC=3500000;
}
#include "PSDI_portableSessionData.h"
#include "BI_basicFunctions.cpp"
#include "LI_labelNames.h"
#include "TDI_textDataForTable.cpp"
#include "DCEI_dockerContainerExecution.cpp"
#include "MCEWCPI_modifyCertWithoutCheckingPermissions.cpp"
#include "MCWCPI_modifyCouasWithoutCheckingPermissions.cpp"
#include "FHI_formHandling.h"
#include "CCFI_customCommandForms.cpp"
#include "MEI_messageElements.h"
#include "GEI_gradeEvaluators.h"
#include "CEI_couasElements.h"
#include "HTII_htmlTitleImprovements.cpp"
#include "APTI_abstractPageText.h"
#include "MPTI_mainPageText.h"
#include "RTI_responseText.h"
#include "SII_sessionInformation.h"
#include "URLPI_getParameters.cpp"
#include "CCI_commandCenter.h"
#include "FFI_fileFormCenter.h"
#include "FFI_fileFormCenter.cpp"
#include "QSCI_quickSpamCheck.cpp"
#include "GCSI_guestCloneServices.cpp"
#include "FHI_formHandling.cpp"
#include "MEI_messageElements.cpp"
#include "CCLI_couasConvenienceLib.cpp"
#include "CERTCLI_certConvenienceLib.cpp"
#include "ICEI_initCouasElements.cpp"
#include "GRI_gradingRules.cpp"
#include "GEI_gradeEvaluators.cpp"
#include "CEI_couasElements.cpp"
#include "CAGI_codeAutoGrader.h"
#include "EMAI_errorMessageAnalysis.cpp"
#include "AGRDI_autoGraderResultDisplay.cpp"
#include "PISI_pythonIndentationSupport.cpp"
#include "CAGI_codeAutoGrader.cpp"
#include "CMTI_couasMigrationTools.h"
#include "CMTI_couasMigrationTools.cpp"
#include "WSCI_websiteCommunications.cpp"
#include "UMCI_userMapCenter.cpp"
#include "APTI_abstractPageText.cpp"
#include "MPTI_mainPageText.cpp"
#include "TFTI_timerForTests.cpp"
#include "RTI_responseText.cpp"
#include "STI_statText.cpp"
#include "CEVI_customizeExamVersions.cpp"
#include "INCII_inCommandInsert.cpp"
//#include "WSCI_websiteCommunications.cpp"
#include "HAI_helpWithADA.cpp"
#include "SII_sessionInformation.cpp"
#include "CCI_commandCenter.cpp"
#endif
