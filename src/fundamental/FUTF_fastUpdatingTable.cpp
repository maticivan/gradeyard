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

#ifndef _INCL_FASTUPDATINGTABLE_CPP
#define _INCL_FASTUPDATINGTABLE_CPP



// Table (atomic table) may have multiple keys but the only search allowed
// is when the entire key is submitted. This structure cannot do any searches with partial keys


namespace FUTF{
  long timeToWaitBeforeDecidingThatPreviousProcessIsStuckAndNeedsToBeKilled=100;
  std::string xTableName="xTable";
  std::string sigmaTableName="sigmaTable";

  std::string futableSt1B="_";
  std::string futableSt1E="_/";
  std::string futableSt2="fut*";
  std::string futableSt3="!*_";

  std::string stTNB="_tName*_";
  std::string stTNE="_/tName*_";
  std::string stStFNB="_stFN*_";
  std::string stStFNE="_/stFN*_";

  std::string flagFileName="writingFlag.txt";

  std::string stfn;

  long timeToSleepBeforeAttemptingToClearTheQueueAgain=50000000;

  std::string tInitFName="tableInit.txt";
  std::string tCKIFName="tCKIInit.txt";

  std::string folderInstQueue="instQueue";
  std::string extForCommandsInQueue="cmq";
  std::string extForFlag="flg";

   class FastUpdatingTable{
   private:
       std::string stFN;
       std::string tableName;
       std::string tableFolder;
       std::string tFolderX;
       std::string tFolderSigma;
       ATF::Table x_table;
       ATF::Table sigma_table;
       std::vector<std::string> keyNames;

       int insertUnsafe(const std::string &, const long &);
       int repairUnsafe(const std::string &,const std::string &);
       int incrementUnsafe(const std::string &);
       int deleteUnsafe(const std::string &);

       long setFlag();
       long clearFlag();
       long checkForFlag() const;

   public:
       FastUpdatingTable(const std::string & ="notSet!***!", const std::string & ="defaultTable");
       std::string getTableName() const;
       int setFolderName(const std::string &, const std::string & = "no");
       // returns 1 if the table is created for the first time
       // If the second argument is "yes" then the function will trust that all folders are created
       //    and will save time by not performing the verifications
       int setTableName(const std::string &, const std::string & = "no");
       // returns 1 if the table is created for the first time
       // If the second argument is "yes" then the function will trust that all folders are created
       //    and will save time by not performing the verifications
       std::string updateDBInitStr(const std::string &) const;
       int initTableFromStr(const std::string &);



       std::pair<FUDSF::Sigma,long> searchX(const std::string &) const;
       std::pair<FUDSF::X,long> searchSigma(const std::string &) const;
       std::pair<FUDSF::X,long> searchXByIndex(const long & ) const;
       std::pair<FUDSF::Sigma,long> searchSigmaByIndex(const long &) const;

       long lowerBoundX(const std::string & ) const;

       int insertSafe(const std::string &, const long &);
       int incrementSafe(const std::string &);
       int deleteSafe(const std::string &);
       long size() const;
       std::string statusReportForDebugging() const;
    };

    FastUpdatingTable::FastUpdatingTable(const std::string & _sfn,const std::string &_tn){
        keyNames.resize(1);
        keyNames[0]="dfKey";
        if(_sfn!="notSet!***!"){
            setFolderName(_sfn);
            if(tableName!="defaultTable"){
                setTableName(_tn);
            }
        }
    }
    std::string FastUpdatingTable::getTableName() const{return tableName;}
    int FastUpdatingTable::setFolderName(const std::string &f, const std::string & trust){
        stFN=f;
        if(trust=="yes"){
          return 0;
        }
        return IOF::sys_createFolderIfDoesNotExist(stFN,"readme.txt","Do not edit this folder.");
    }
    void initFileInFolder(const std::string &folderName, const std::string & fileName, const std::string & st){
      std::string t2Init=folderName;
      t2Init+="/"+fileName;
      std::ifstream f2Test(t2Init);
      if(!f2Test.good()){
        IOF::toFile(t2Init,st);
      }
    }
    int FastUpdatingTable::setTableName(const std::string & _s,const std::string & _trust){
        tableName=_s;
        tableFolder=stFN+"/"+tableName;
        tFolderX=tableFolder+"/"+xTableName;
        tFolderSigma=tableFolder+"/"+sigmaTableName;
        x_table.initialize(tFolderX,xTableName);
        x_table.setKeyNames(keyNames);
        sigma_table.initialize(tFolderSigma,sigmaTableName);
        sigma_table.setKeyNames(keyNames);
        if((_trust=="yes")||(IOF::sys_createFolderIfDoesNotExist(tableFolder,tInitFName,"Do not edit this folder.")==0)){
          // table existed from before
          // or we just created one


          std::string initData=IOF::fileToString(tableFolder+"/"+tCKIFName);
          int succ_x=x_table.initTableFromStr(initData);
          if(succ_x==0){
            x_table.initialize(tFolderX,xTableName);
            x_table.setKeyNames(keyNames);
          }
          int succ_s=sigma_table.initTableFromStr(initData);
          if(succ_s==0){
            sigma_table.initialize(tFolderSigma,sigmaTableName);
            sigma_table.setKeyNames(keyNames);
          }
          if(succ_x==0){
            initData=x_table.updateDBInitStr(initData);
          }
          if(succ_s==0){
            initData=sigma_table.updateDBInitStr(initData);
          }
          if(succ_x*succ_s==0){
            IOF::toFile(tableFolder+"/"+tCKIFName,initData);

          }
        }

        if((_trust=="yes")||(IOF::sys_createFolderIfDoesNotExist(tFolderX,tInitFName,"Do not edit this folder.")==0)){
          // table existed from before
          // The call to the function inside if statement accomplished this:
          //  std::string initData=IOF::fileToString(tFolderX+"/"+tInitFName);
          //  x_table.initTableFromStr(initData);
        }

        if((_trust=="yes")||(IOF::sys_createFolderIfDoesNotExist(tFolderSigma,tInitFName,"Do not edit this folder.")==0)){
          // table existed from before
          // The call to the function inside if statement accomplished this:
          //std::string initData=IOF::fileToString(tFolderSigma+"/"+tInitFName);
          //sigma_table.initTableFromStr(initData);
        }



      if(_trust!="yes"){
        initFileInFolder(tableFolder,tCKIFName," ");
        initFileInFolder(tFolderX,tCKIFName," ");
        initFileInFolder(tFolderSigma,tCKIFName," ");
      }

      return 0;
    }


    std::string FastUpdatingTable::updateDBInitStr(const std::string & tableDBInitStr) const{
        std::string st1=x_table.updateDBInitStr(tableDBInitStr);
        st1=sigma_table.updateDBInitStr(st1);

        std::pair<std::string,int> allData;
        long pos;
        std::string stB=futableSt1B+futableSt2+tableName+futableSt3;
        std::string stE=futableSt1E+futableSt2+tableName+futableSt3;
        pos=0;
        std::string newTableDBInitStr=SF::eraseStuffBetween(st1,stB,stE,pos).first;

        return stB+stStFNB+stFN+stStFNE+stE+newTableDBInitStr;

    }

    int FastUpdatingTable::initTableFromStr(const std::string &stToInitFrom){
      int fRX= x_table.initTableFromStr(stToInitFrom);
      int fRSigma = sigma_table.initTableFromStr(stToInitFrom);

      std::pair<std::string,int> allData;
      long pos;
      std::string stB=futableSt1B+futableSt2+tableName+futableSt3;
      std::string stE=futableSt1E+futableSt2+tableName+futableSt3;
      pos=0;allData=SF::extract(stToInitFrom,pos,stB,stE);
      if(allData.second==1){
        std::string nms=allData.first;
        pos=0;allData=SF::extract(nms,pos,stStFNB,stStFNE);
        if(allData.second==1){
          stFN=allData.first;

          tableFolder=stFN+"/"+tableName;
          tFolderX=tableFolder+"/"+xTableName;
          tFolderSigma=tableFolder+"/"+sigmaTableName;


        }
        x_table.initTableFromStr(stToInitFrom);
        sigma_table.initTableFromStr(stToInitFrom);
        return 1;
      }
      return 0;

    }
    int FastUpdatingTable::repairUnsafe(const std::string& _x, const std::string & xRes){


      std::vector<std::string> w;
      w.resize(1);w[0]=xRes;
      std::string sigmaRes=sigma_table.select(w);
      if(sigmaRes!="notFound"){
        return 1;
      }
      FUDSF::Sigma sigmaT;
      sigmaT.loadSigmaTFromString(xRes);
      FUDSF::Sigma sigmaPlInfinity;
      sigmaPlInfinity.sigma=sigmaT.sigma;
      sigmaPlInfinity.infinityIndicator=1;

      HDDBRF::Record rKeyTemp;
      std::vector<std::string> keysTemp;
      keysTemp.resize(1);
      keysTemp[0]=sigmaPlInfinity.putSigmaTToString();
      rKeyTemp.setKeys(keysTemp);
      rKeyTemp.setMainDataRTD(_x);
      IRFNF::IndRecFName<HDDBRF::Record> irfn=sigma_table.lowerBoundIndexRecordFileName(rKeyTemp,1);
      long up,down;
      if(irfn.index>-1){
        up=irfn.index;
        down=irfn.index;
      }
      std::pair<FUDSF::Sigma,long> psSearch;
      long sz=x_table.size();
      long next=1;
      long index;
      long found=0;
      while( ((up<sz) || (down>-1)) &&(found==0) ) {

        if(next>0){
          index=up;
          ++up;
        }
        else{
          index=down;
          --down;
        }
        psSearch=FastUpdatingTable::searchSigmaByIndex(index);

        if(psSearch.second==1){
          if(psSearch.first.x==_x){
            std::string ststring=psSearch.first.putSigmaTToString();
            std::vector<std::string> keysTemp;
            keysTemp.resize(1);
            keysTemp[0]=_x;
            int insReport=x_table.insertOrUpdateAndGiveReport(keysTemp,ststring);
            found=1;
          }
        }
        next*=-1;
      }

      return found;
    }
    int FastUpdatingTable::insertUnsafe(const std::string & _x, const long & _sigma){
      if(searchX(_x).second==1){
        return 0;
      }
      HDDBRF::Record rKeyTemp;
      std::vector<std::string> keysTemp;
      FUDSF::Sigma stemp;
      stemp.x=_x;
      stemp.sigma=_sigma;
      stemp.infinityIndicator=1;// stemp=(sigma,+infinity)
      keysTemp.resize(1);
      keysTemp[0]=stemp.putSigmaTToString();
      rKeyTemp.setKeys(keysTemp);
      rKeyTemp.setMainDataRTD(_x);
      IRFNF::IndRecFName<HDDBRF::Record> irfn=sigma_table.lowerBoundIndexRecordFileName(rKeyTemp,1);
      FUDSF::Sigma sResult;
      stemp.t=0;
      if(irfn.index>-1){
        sResult.loadSigmaTFromString(irfn.record.getKeys()[0]);
        if(sResult.sigma==_sigma){
          stemp.t=sResult.t+1;
        }
      }
      stemp.infinityIndicator=0;
      keysTemp[0]=_x;
      std::string ststring=stemp.putSigmaTToString();
      int insReport=x_table.insertOrUpdateAndGiveReport(keysTemp,ststring);
      if(insReport==1){

        // Update table initialization - the root may have changed
        std::string t2Init=tableFolder;
        t2Init+="/"+tCKIFName;
        std::string inSt=IOF::fileToString(t2Init);
        inSt=x_table.updateDBInitStr(inSt);
        keysTemp[0]=ststring;
        insReport = sigma_table.insertOrUpdateAndGiveReport(keysTemp,_x);
        if(insReport==1){
          inSt=sigma_table.updateDBInitStr(inSt);
        }
        IOF::toFile(t2Init,inSt);
      }
      return insReport;
    }
    int FastUpdatingTable::insertSafe(const std::string & _x, const long & _sigma){
      if(setFlag()==0){
        return 0;
      }
      int insReport=insertUnsafe(_x,_sigma);
      clearFlag();
      return insReport;
    }

    int FastUpdatingTable::incrementUnsafe(const std::string & _x){
      std::vector<std::string> v;
      v.resize(1);v[0]=_x;
      std::string xRes=x_table.select(v);
      if(xRes=="notFound"){
        return 0;
      }
      repairUnsafe(_x,xRes);
      FUDSF::Sigma sigmaT;
      sigmaT.loadSigmaTFromString(xRes);

      FUDSF::Sigma sigmaPlInfinity;
      sigmaPlInfinity.sigma=sigmaT.sigma;
      sigmaPlInfinity.infinityIndicator=1;

      HDDBRF::Record rKeyTemp;
      std::vector<std::string> keysTemp;
      keysTemp.resize(1);
      keysTemp[0]=sigmaPlInfinity.putSigmaTToString();
      rKeyTemp.setKeys(keysTemp);
      rKeyTemp.setMainDataRTD(_x);
      IRFNF::IndRecFName<HDDBRF::Record> irfn=sigma_table.lowerBoundIndexRecordFileName(rKeyTemp,1);
      FUDSF::Sigma sigmaMaxS;
      if(irfn.index>-1){
        long maximalS=sigmaT.t;
        sigmaMaxS.loadSigmaTFromString(irfn.record.getKeys()[0]);
        if(sigmaMaxS.sigma==sigmaT.sigma){

          if(sigmaMaxS.t>sigmaT.t){
            maximalS=sigmaMaxS.t;
            std::string xPrime;
            std::pair<FUDSF::X,long> xPFinder=searchSigma(sigmaMaxS.putSigmaTToString());
            if(xPFinder.second==0){
              return 0;
            }
            xPrime=xPFinder.first.x;
            std::vector<std::string> keys1,keys2;
            keys1.resize(1);keys2.resize(1);
            keys1[0]=xPrime;
            keys2[0]=_x;



            x_table.insertOrUpdateAndGiveReport(keys1,sigmaT.putSigmaTToString());

            x_table.insertOrUpdateAndGiveReport(keys2,sigmaMaxS.putSigmaTToString());

            keys1[0]=sigmaMaxS.putSigmaTToString();
            keys2[0]=sigmaT.putSigmaTToString();

            sigma_table.insertOrUpdateAndGiveReport(keys2,xPrime);
            sigma_table.insertOrUpdateAndGiveReport(keys1,_x);


          }

          FUDSF::Sigma nextSigmaMinInfinity;
          nextSigmaMinInfinity.sigma=sigmaT.sigma+1;
          nextSigmaMinInfinity.infinityIndicator=-1;
          keysTemp[0]=nextSigmaMinInfinity.putSigmaTToString();
          rKeyTemp.setKeys(keysTemp);
          irfn=sigma_table.lowerBoundIndexRecordFileName(rKeyTemp,1);
          long indicatorNextSigmaExistsInTable=0;

          FUDSF::Sigma smallestWithSigmaPl1;

          std::pair<FUDSF::Sigma,long> tempSearchForNextSigma;
          if(irfn.index+1 < sigma_table.size()){
            tempSearchForNextSigma = searchSigmaByIndex(irfn.index+1);
            if(tempSearchForNextSigma.second==1){
              if(tempSearchForNextSigma.first.sigma==sigmaT.sigma+1){
                indicatorNextSigmaExistsInTable=1;
              }
            }
          }

          std::vector<std::string> keysX,keysSigma,keysSigmaNew;
          keysX.resize(1);keysSigma.resize(1);keysSigmaNew.resize(1);
          keysX[0]=_x;
          FUDSF::Sigma sigmaImproved;
          sigmaImproved.sigma=sigmaT.sigma+1;

          FUDSF::Sigma sigmaForKeys;
          sigmaForKeys.sigma=sigmaT.sigma;
          sigmaForKeys.t=maximalS;
          keysSigma[0]=sigmaForKeys.putSigmaTToString();

          if(indicatorNextSigmaExistsInTable==1){
            sigmaImproved.t=tempSearchForNextSigma.first.t-1;
          }
          else{
            sigmaImproved.t=0;
          }
          keysSigmaNew[0]=sigmaImproved.putSigmaTToString();
          x_table.insertOrUpdateAndGiveReport(keysX,sigmaImproved.putSigmaTToString());
          sigma_table.inPlaceKeyDataModification(keysSigma,keysSigmaNew,_x,0,0);
          return 1;
        }

        return 0;

      }
      return 0;
    }
    int FastUpdatingTable::incrementSafe(const std::string & _x){
      if(setFlag()==0){
        return 0;
      }
      int incRes=incrementUnsafe(_x);
      clearFlag();
      return incRes;
    }
    int FastUpdatingTable::deleteUnsafe(const std::string & _x){
      std::vector<std::string> v;
      v.resize(1);v[0]=_x;
      std::string xRes=x_table.select(v);
      if(xRes=="notFound"){
        return 0;
      }
      std::vector<std::string> w;
      w.resize(1);w[0]=xRes;
      std::string sigmaRes=sigma_table.select(w);
      if(sigmaRes=="notFound"){
        return 0;
      }

      if(x_table.delRow(v)==1){
        std::string t2Init=tableFolder;
        t2Init+="/"+tCKIFName;
        std::string inSt=IOF::fileToString(t2Init);
        inSt=x_table.updateDBInitStr(inSt);

        if(sigma_table.delRow(w)==1){
          inSt=sigma_table.updateDBInitStr(inSt);
        }
        IOF::toFile(t2Init,inSt);
        return 1;
      }

      return 0;
    }
    int FastUpdatingTable::deleteSafe(const std::string & _x){
      if(setFlag()==0){
        return 0;
      }
      int delRes=deleteUnsafe(_x);
      clearFlag();
      return delRes;
    }
    std::pair<FUDSF::Sigma,long> FastUpdatingTable::searchX(const std::string & s) const{
      std::pair<FUDSF::Sigma,long > res;
      res.second=0;
      std::vector<std::string> v;
      v.resize(1);v[0]=s;
      std::string sRes=x_table.select(v);
      if(sRes=="notFound"){
        return res;
      }
      res.second=1;
      res.first.loadSigmaTFromString(sRes);
      res.first.x=s;
      return res;
    }
    std::pair<FUDSF::X,long> FastUpdatingTable::searchSigma(const std::string & s) const{
      std::pair<FUDSF::X,long > res;
      res.second=0;
      std::vector<std::string> v;
      v.resize(1);v[0]=s;
      std::string sRes=sigma_table.select(v);

      if(sRes=="notFound"){
        return res;
      }
      res.second=1;
      res.first.loadSigmaTFromString(s);
      res.first.x=sRes;
      return res;
    }
    std::pair<FUDSF::X,long> FastUpdatingTable::searchXByIndex(const long & i) const{
      std::pair<FUDSF::X,long > res;
      std::pair<HDDBRF::Record,std::string> rs=x_table[i];
      res.second=0;
      if(rs.second=="notFound") return res;
      res.second=1;
      res.first.x=rs.first.getKeys()[0];
      res.first.loadSigmaTFromString(rs.first.getMainDataRTD());
      return res;
    }
    std::pair<FUDSF::Sigma,long> FastUpdatingTable::searchSigmaByIndex(const long &i) const{
      std::pair<FUDSF::Sigma,long > res;
      std::pair<HDDBRF::Record,std::string> rs=sigma_table[i];
      res.second=0;
      if(rs.second=="notFound") return res;
      res.second=1;
      res.first.loadSigmaTFromString(rs.first.getKeys()[0]);
      std::pair<FUDSF::X,long> xRecoveryHelp= searchSigma(rs.first.getKeys()[0]);
      if(xRecoveryHelp.second==1){
        res.first.x=xRecoveryHelp.first.x;
      }

      return res;
    }


    long FastUpdatingTable::lowerBoundX(const std::string & s) const{
      std::vector<std::string> v;
      v.resize(1);v[0]=s;
      return x_table.findLowerBound(v);
    }

    long FastUpdatingTable::size() const{
      return x_table.size();
    }


    std::string FastUpdatingTable::statusReportForDebugging() const{
      std::string fR;
      fR="starting folder name: "+  stFN;

      fR+="\nTable name: "+tableName;
      fR+="<BR>\nSTATUS REPORT FOR X_TABLE BEGIN<BR>\n";
      fR+=x_table.statusReportForDebugging();
      fR+="<BR>\nSTATUS REPORT FOR X_TABLE END<BR>\n";
      fR+="<BR>\nSTATUS REPORT FOR SIGMA_TABLE BEGIN<BR>\n";
      fR+=sigma_table.statusReportForDebugging();
      fR+="<BR>\nSTATUS REPORT FOR SIGMA_TABLE END<BR>\n";

      return fR;
    }

    long FastUpdatingTable::checkForFlag() const{
      long res=0;

      std::string fullFlagFName=tableFolder+"/"+flagFileName;
      std::string fRes=IOF::fileToString(fullFlagFName);
      if(fRes!="fileNotFound"){
        res= 1;
      }
      return res;
    }
    long FastUpdatingTable::setFlag(){
      long res=1;
      std::string fullFlagFName=tableFolder+"/"+flagFileName;
      if(checkForFlag()==1){
        TMF::Timer tm;
        long tN=tm.timeNow(1000000);
        long tCr=1000*static_cast<long>(IOF::timeOfCreation(fullFlagFName));
        if(tN-tCr<timeToWaitBeforeDecidingThatPreviousProcessIsStuckAndNeedsToBeKilled){
          res=0;
        }
        else{
          IOF::sys_deleteFile(fullFlagFName);
        }
      }
      if(res==1){
        IOF::toFile(fullFlagFName,"set");
      }
      return res;
    }
    long FastUpdatingTable::clearFlag(){
      long res=0;
      std::string fullFlagFName=tableFolder+"/"+flagFileName;
      if(checkForFlag()==1){
        res=1;
        IOF::sys_deleteFile(fullFlagFName);
      }
      return res;
    }
}
#endif
