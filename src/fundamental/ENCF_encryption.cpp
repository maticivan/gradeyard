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

// https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption

#ifndef _INCL_ENCF_ENCRYPTION_CPP
#define _INCL_ENCF_ENCRYPTION_CPP


namespace ENCF{
  static const unsigned int KEY_SIZE = 32;
  static const unsigned int BLOCK_SIZE = 16;

  std::string sep_encB="_*@senc**!!!_";
  std::string sep_encE="_/*@senc**!!!_";
  std::string sep_invB="_*@inv**!!!_";
  std::string sep_invE="_/*@inv**!!!_";
  std::string sep_codeB="_*@scode**!!!_";
  std::string sep_codeE="_/*@scode**!!!_";

  typedef unsigned char byte;
  using EVP_CIPHER_CTX_free_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;

  struct SecretCodeLocations{
  public:
    std::string folder;
  } GL_SECRET_LOCATION;

  int aes_encrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE], const std::string& ptext, std::string& ctext){
      EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
      int rc = EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, key, iv);
      if (rc != 1){
        ctext="encryption initialization failed";
        return 2;
      }


      ctext.resize(ptext.size()+BLOCK_SIZE);
      int out_len1 = (int)ctext.size();

      rc = EVP_EncryptUpdate(ctx.get(), (byte*)&ctext[0], &out_len1, (const byte*)&ptext[0], (int)ptext.size());
      if (rc != 1){
        ctext="encryption update failed";
        return 3;
      }

      int out_len2 = (int)ctext.size() - out_len1;
      rc = EVP_EncryptFinal_ex(ctx.get(), (byte*)&ctext[0]+out_len1, &out_len2);
      if (rc != 1){
        ctext="encryption final failed";
        return 4;
      }


      ctext.resize(out_len1 + out_len2);
      return 1;
  }

  int aes_decrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE], const std::string& ctext, std::string& rtext){
      EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
      int rc = EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, key, iv);
      if (rc != 1){
        rtext="decryption initialization failed";
        return 2;
      }

      rtext.resize(ctext.size());
      int out_len1 = (int)rtext.size();

      rc = EVP_DecryptUpdate(ctx.get(), (byte*)&rtext[0], &out_len1, (const byte*)&ctext[0], (int)ctext.size());
      if (rc != 1){
        rtext="decrytpion update failed";
        return 3;
      }

      int out_len2 = (int)rtext.size() - out_len1;
      rc = EVP_DecryptFinal_ex(ctx.get(), (byte*)&rtext[0]+out_len1, &out_len2);
      if (rc != 1){
        rtext ="decryption failed";
        return 4;
      }

      rtext.resize(out_len1 + out_len2);
      return 1;
  }


  byte extend(const byte key[], const long & n){
    return static_cast<byte>(BF::GLOBAL_PRIME_SEQUENCES.primes[n % BF::GLOBAL_NUM_PRIME_SEQUENCES]%200);

  }
  void generateKeyFromStr(byte key[],const long &sz, const std::string &_kStr){

    long lim=_kStr.length();
    if(lim>sz){
      lim=sz;
    }
    long i=0;
    while(i<lim){
      key[i]=static_cast<byte>(_kStr[i]);
      ++i;
    }
    while(i<sz){
      key[i]=extend(key,i);
      ++i;
    }

  }
  std::string bToStr(const byte key[], const long &sz){
    std::string fR;
    fR+="_n*_";
    for(long i=0;i<sz;++i){
      fR+=std::to_string(key[i]);
      fR+="_/n*_\n_n*_";
    }
    fR+="-11_/n*_";
    return fR;
  }
  void strToByte(const std::string & s,byte key[], const long &sz){
    std::vector<std::string> vals=SF::stringToVector(s,"_n*_","_/n*_");
    if(vals.size()==sz+1){
      for(long i=0;i<sz;++i){
        key[i]=static_cast<byte>( BF::stringToInteger(vals[i]) );
      }
    }
    else{
       for(long i=0;i<sz;++i){
         key[i]=0;
       }
    }

  }

  std::string symmetricEncrypt(const std::string & src, const std::string &sKey){
    byte ky[KEY_SIZE],iv[BLOCK_SIZE];
    generateKeyFromStr(ky,KEY_SIZE, sKey);
    RAND_bytes(iv,BLOCK_SIZE);
    std::string result;
    aes_encrypt(ky, iv, src, result);
    std::string encryption= sep_encB+"\n"+sep_invB+"\n"+bToStr(iv,BLOCK_SIZE);
    encryption+="\n"+sep_invE+"\n";
    encryption+=sep_codeB;
    encryption+=result;
    encryption+=sep_codeE+"\n";
    encryption+=sep_encE;
    OPENSSL_cleanse(ky, KEY_SIZE);
    OPENSSL_cleanse(iv, BLOCK_SIZE);
    return encryption;
  }
  std::string symmetricDecrypt(const std::string & src, const std::string & sKey){
    std::pair<std::string,int> allD;
    long pos;
    pos=0;allD=SF::extract(src,pos,sep_encB,sep_encE);
    if(allD.second==0){
      return "";
    }
    std::string srcIn=allD.first;

    pos=0;allD=SF::extract(srcIn,pos,sep_invB,sep_invE);
    if(allD.second==0){
      return "";
    }

    byte ky[KEY_SIZE],iv[BLOCK_SIZE];

    strToByte(allD.first,iv,BLOCK_SIZE);

    pos=0;allD=SF::extract(srcIn,pos,sep_codeB,sep_codeE);
    if(allD.second==0){
      return "";
    }

    generateKeyFromStr(ky,KEY_SIZE,sKey);


    std::string ctext=allD.first;
    std::string rtext;



    aes_decrypt(ky, iv, ctext, rtext);


    OPENSSL_cleanse(ky, KEY_SIZE);
    OPENSSL_cleanse(iv, BLOCK_SIZE);
    return rtext;
  }
  struct KeyPair{
  public:
    std::string privateKey;
    std::string publicKey;
  };

  std::string prepareWorkingFolder(const std::string & baseName){
    std::string subfolderName=baseName;
    if(subfolderName==""){
      subfolderName=RNDF::genRandCode(15);
    }
    std::string fName=GL_SECRET_LOCATION.folder+"/"+"tk"+subfolderName;
    IOF::sys_createFolderIfDoesNotExist(fName,"readme.txt","Do not edit this folder");
    return fName;
  }

  KeyPair createKeyPair(const std::string & _subfolderName = ""){

    std::string folderToKeepTmpKeys= prepareWorkingFolder(_subfolderName);

    std::string privateKeyFile=folderToKeepTmpKeys+"/privateKey.pem";
    std::string publicKeyFile=folderToKeepTmpKeys+"/publicKey.pem";
    std::string comPrivate="openssl genrsa -out "+privateKeyFile+" 2048";
    std::string comPublic="openssl rsa -in "+privateKeyFile+" -outform PEM -pubout -out "+publicKeyFile;
    IOF::sys_executeCommandThatDoesNotPrint(comPrivate);
    IOF::sys_executeCommandThatDoesNotPrint(comPublic);

    KeyPair twoKeys;

    twoKeys.privateKey=IOF::fileToString(privateKeyFile);
    twoKeys.publicKey=IOF::fileToString(publicKeyFile);
    IOF::toFile(privateKeyFile,RNDF::genRandCode(100));
    IOF::sys_deleteFolderAndSubfolders(folderToKeepTmpKeys);

    return twoKeys;
  }

  std::string encryptWithPublicKey(const std::string & plain, const std::string &publicKey, const std::string & _subfolderName=""){

    std::string folderToKeepTmpKeys= prepareWorkingFolder(_subfolderName);
    std::string publicKeyFile=folderToKeepTmpKeys+"/publicKey.pem";
    std::string messageFile=folderToKeepTmpKeys+"/fileMessage.txt";
    std::string encTextFile=folderToKeepTmpKeys+"/encTxtFile.txt";
    IOF::toFile(publicKeyFile,publicKey);
    IOF::toFile(messageFile,plain);
    std::string comEnc="openssl rsautl -encrypt -inkey "+publicKeyFile+" -pubin -in "+messageFile+" -out "+encTextFile;
    IOF::sys_executeCommandThatDoesNotPrint(comEnc);
    std::string enc=IOF::fileToString(encTextFile);
    IOF::toFile(messageFile,RNDF::genRandCode(100));
    IOF::sys_deleteFolderAndSubfolders(folderToKeepTmpKeys);
    return enc;
  }

  std::string decryptWithPrivateKey(const std::string & enc, const std::string &privateKey, const std::string & _subfolderName=""){
    std::string folderToKeepTmpKeys= prepareWorkingFolder(_subfolderName);
    std::string privateKeyFile=folderToKeepTmpKeys+"/publicKey.pem";
    std::string encTextFile=folderToKeepTmpKeys+"/encTxtFile.txt";
    std::string plainTextFile=folderToKeepTmpKeys+"/plainTxtFile.txt";
    IOF::toFile(privateKeyFile,privateKey);
    IOF::toFile(encTextFile,enc);

    std::string comDec="openssl rsautl -decrypt -inkey "+privateKeyFile+" -in "+encTextFile+" -out "+plainTextFile;
    IOF::sys_executeCommandThatDoesNotPrint(comDec);
    std::string plain=IOF::fileToString(plainTextFile);
    IOF::toFile(plainTextFile,RNDF::genRandCode(100));
    IOF::sys_deleteFolderAndSubfolders(folderToKeepTmpKeys);
    return plain;
  }

  std::string mainEncTests(){

    std::string tstKey=IOF::fileToString("entestKey.txt");
    std::string tstSource=IOF::fileToString("entestSrc.txt");
    std::string tstEnc1=IOF::fileToString("entestEnc.txt");

    std::string encryptionOfSource=symmetricEncrypt(tstSource,tstKey);
    std::string decryptionOfEnc1=symmetricDecrypt(tstEnc1,tstKey);
    IOF::toFile("entestEnc.txt",encryptionOfSource);
    IOF::toFile("entestDec.txt",decryptionOfEnc1);

    return "done";

  }
}

#endif
