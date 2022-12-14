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



#ifndef _INCL_DATABASES_CPP
#define _INCL_DATABASES_CPP

#include "DSD_dbSetup.cpp"
namespace DD{
  DSD::Setup GL_DBS;
  const long GL_IND_ALLOW_WEBSITE_CLONES=0;
}
#include "MWID_mainWebsiteInit.cpp"

namespace DD{
  MWID::MainDB GL_MAIN_DB;
}

#include "CD_counters.cpp"
#include "WUD_users.cpp"
#include "PBD_permission_basics.cpp"
#include "AMD_abstractTableManagement.cpp"
#include "FMD_fileManagement.cpp"
#include "MD_messages.cpp"
#include "CAD_couas.cpp"
#include "ATMD_abstractTextManagement.cpp"
#include "TMD_textManagement.cpp"
#include "RMD_responseManagement.cpp"
#include "SD_stat.cpp"
#include "AICD_abstractInsertCenter.cpp"
#include "BMD_backupManager.cpp"
#endif
