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


#ifndef _INCL_AEF_ANSWEREXTRACTION_CPP
#define _INCL_AEF_ANSWEREXTRACTION_CPP


namespace AEF{
  std::string allLatexTagsIntoDollars(const std::string& input){
    std::string result=input;
    result=SF::findAndReplace(result,"\\$","");
    result=SF::findAndReplace(result,"$$"," $ ");
    result=SF::findAndReplace(result,"\\begin{eqnarray*}"," $");
    result=SF::findAndReplace(result,"\\begin{eqnarray}"," $");
    result=SF::findAndReplace(result,"\\end{eqnarray*}","$ ");
    result=SF::findAndReplace(result,"\\end{eqnarray}","$ ");
    result=SF::findAndReplace(result,"\\begin{equation*}"," $");
    result=SF::findAndReplace(result,"\\begin{equation}"," $");
    result=SF::findAndReplace(result,"\\end{equation*}","$ ");
    result=SF::findAndReplace(result,"\\end{equation}","$ ");
    result=SF::findAndReplace(result,"\\("," $");
    result=SF::findAndReplace(result,"\\["," $");
    result=SF::findAndReplace(result,"\\)","$ ");
    result=SF::findAndReplace(result,"\\]","$ ");
    return result;
  }
  std::string reverse(const std::string& input){
    std::string result;
    long i=input.length();
    while(i>0){
      --i;
      result+=input[i];
    }
    return result;
  }
  std::pair<std::string,int> extractLastFormula(const std::string& input){
     std::string result=allLatexTagsIntoDollars(input);
     result=reverse(result);
     std::pair<std::string,int> allD;
     long pos;
     pos=0; allD=SF::extract(result,pos,"$","$");
     if(allD.second==0){
       return allD;
     }
     allD.first=reverse(allD.first);
     return allD;
   }
   int isDigit(const char & c){
     if((c>='0')&&(c<='9')){
       return 1;
     }
     return 0;
   }
   long getPositionOfLastDigit(const std::string& input, const long& maxLengthOfSearch=-1){
     long i=maxLengthOfSearch;
     long iL=input.length();
     if((i<0)||(i>iL)){
       i=iL;
     }
     --i;
     while((i>-1)&&(!isDigit(input[i]))){
       --i;
     }
     return i;
   }
   struct LastIntegerData{
   public:
     int integerExists;
     long indexOfFirstCharacterOfLastInteger;
     long value;
     int spaceCharactersDetectedBeforeInteger;
     int decimalSeparatorDectectedBeforeInteger;
     long leadingZeroCount;
     LastIntegerData();
     std::string debuggingString() const;
   };
   LastIntegerData::LastIntegerData(){
     integerExists=0; value=0; indexOfFirstCharacterOfLastInteger=-1;
     spaceCharactersDetectedBeforeInteger=0;decimalSeparatorDectectedBeforeInteger=0;
     leadingZeroCount=0;
   }
   std::string LastIntegerData::debuggingString() const{
     std::string res;
     res+="Integer exists: "+std::to_string(integerExists);
     res+="\nValue: "+std::to_string(value);
     res+="\nIndex of first character of last integer: "+std::to_string(indexOfFirstCharacterOfLastInteger);
     res+="\nSpace detected before: "+std::to_string(spaceCharactersDetectedBeforeInteger);
     res+="\nDecimal separator detected before: "+std::to_string(decimalSeparatorDectectedBeforeInteger);
     res+="\nNumber of leading zeros: "+std::to_string(leadingZeroCount);
     return res;
   }
   LastIntegerData getLastIntegerData(const std::string& input, const long& maxLengthOfSearchString=-1){
     long i=getPositionOfLastDigit(input,maxLengthOfSearchString);
     LastIntegerData result;
     if(i<0){
       return result;
     }
     long multiplier=1;
     result.integerExists=1;
     while((i>-1)&&(isDigit(input[i]))){
       result.value += multiplier * static_cast<long>(input[i]-'0');
       multiplier*=10;
       if(input[i]=='0'){
         ++result.leadingZeroCount;
       }
       else{
         result.leadingZeroCount=0;
       }
       --i;
     }
     result.indexOfFirstCharacterOfLastInteger=i+1;
     if((i>-1)&&(input[i]==BF::CONST_DECIMAL_SEPARATOR)){
       result.decimalSeparatorDectectedBeforeInteger=1;
       return result;
     }
     while((i>-1)&&((input[i]==' ')||(input[i]=='\t')||(input[i]=='\n'))){
       --i;result.spaceCharactersDetectedBeforeInteger=1;
     }
     if((i>-1)&&(input[i]=='-')){
       result.value *=-1;
       result.indexOfFirstCharacterOfLastInteger=i;
     }
     return result;
   }
   double fractionalPart(const long& afterDecimalPoint,const long &leadingZeroCount){
     double multiplier=1.0;
     for(long i=0;i<leadingZeroCount;++i){
       multiplier*=0.1;
     }
     long digitKeeper=afterDecimalPoint;
     while(digitKeeper>0){
       multiplier*=0.1; digitKeeper/=10;
     }
     return multiplier*static_cast<double>(afterDecimalPoint);
   }
   double createRealNumber(const long& beforeDecimalPoint, const long& afterDecimalPoint, const long & leadingZeroCount){
     double fPart=fractionalPart(afterDecimalPoint,leadingZeroCount);
     long absValBefore=beforeDecimalPoint;
     double sign=1.0;
     if(absValBefore<0){
       absValBefore *=-1;
       sign=-1.0;
     }
     return sign*(static_cast<double>(absValBefore)+fPart);
   }

   double getLastNumericalValue(const std::string& input){
     LastIntegerData lid=getLastIntegerData(input);
     if(lid.integerExists==0){
       return -171717.17171777;
     }
     if(lid.decimalSeparatorDectectedBeforeInteger==0){
       return static_cast<double>(lid.value);
     }
     if( (lid.indexOfFirstCharacterOfLastInteger<2) || (!isDigit(input[lid.indexOfFirstCharacterOfLastInteger-2])) ){
       return static_cast<double>(lid.value);
     }
     LastIntegerData lidBeforeSeparator=getLastIntegerData(input,lid.indexOfFirstCharacterOfLastInteger);
     if(lidBeforeSeparator.integerExists==0){
       return static_cast<double>(lid.value);
     }
     return createRealNumber(lidBeforeSeparator.value,lid.value,lid.leadingZeroCount);
   }
}
#endif
