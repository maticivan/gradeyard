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


#ifndef _INCL_HASHINGENC_CPP
#define _INCL_HASHINGENC_CPP

#include"HENCIF_hashingEncInc.cpp"


namespace HENCF{


  std::string oneWayEncHENCF(const std::string & x,
                        const std::string & salt ="*noSalt*",
                        const long & _saltLen=20){
  // If no salt is provided, it will be generated randomly
  // The function generates the encrypted text that is its own salt (the salt is in the beginning).
  // A ussage of this function is:
  // When you want to encrypt a text "MySecretPassword"
  // you call the function as
  // std::string enc=oneWayEnc("MySecretPassword");
  // Once you want to check the password
  // Then oneWayEnc("MySecretPassword",enc) will be equal to enc
  // However, onewWayEnc("WrongPassword",enc) will not be equal to enc

  // **** TEST
  //int main(){
  //  std::string enc=oneWayEnc("MySecretPassword");
  //  std::cout<<(oneWayEnc("MySecretPassword",enc)==enc)<<"\n";//prints 1
  //  std::cout<<(oneWayEnc("WrongPassword",enc)==enc)<<"\n";//prints 0 with probability 99.9999999999999%
  //  return 0;
  //}


      long saltLen=20;
      long correctSalt=0;
      long sSize=salt.size();
      std::string mySalt="";
      if(sSize== 64+ _saltLen){
          correctSalt=1;
          saltLen=_saltLen;

          for(long i=0;i<saltLen;++i){
              mySalt+=salt[i];
          }
      }

      if(correctSalt==0){
          long numNeededCharacters=6;
          long numNeededDigits=10;
          long uniformLimit=numNeededDigits+numNeededCharacters;
          std::vector<std::string> saltSeq;
          saltSeq.resize(uniformLimit);
          char tempCh='a';
          long i,j;
          for(i=0;i<numNeededCharacters;++i){

              saltSeq[i]=tempCh;
              tempCh += 1;
          }
          tempCh='0';
          j=i;
          for(long i=0;i<numNeededDigits;++i){
              saltSeq[j+i]=tempCh;
              tempCh += 1;
          }


          std::uniform_int_distribution<long> uInt(0,uniformLimit-1);
          long seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
          std::mt19937 mt_randIF(seed);
          long d0;
          for(long i=0;i<saltLen;++i){
              d0=uInt(mt_randIF);

              mySalt+=saltSeq[d0];
          }

      }


      std::string fR;
      std::string rI=x;


      rI=mySalt+ HENCIF::sha256(mySalt+x);

      return rI;
  }
}
#endif
