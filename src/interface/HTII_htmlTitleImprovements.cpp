//    GradeYard learning management system
//
//    Copyright (C) 2023 Ivan Matic, https://gradeyard.com
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

#ifndef _INCL_HTII_htmlTitleImprovements_CPP
#define _INCL_HTII_htmlTitleImprovements_CPP
namespace HTII{
  struct PageElements{
  public:
    std::vector<std::vector<std::string> > hTags;
    std::vector<std::string> pFormulations;
    std::vector<std::string> theorems;
    std::vector<std::string> definitions;
  };
  struct TitleSuggestions{
  public:
    std::string tSuggestion;
    std::string dSuggestion;
    std::string codewordThatTitleGenerationIsNeeded;
    std::string codewordThatDescGenerationIsNeeded;
    long nTGenerations;
    long nDGenerations;
    long maxTitleLength;
    long maxDescLength;
    PageElements pels;
  } GL_title;
  std::string truncateLength(const std::string& in, const long &mLen){
    if(in.length()>mLen){
      return in.substr(0,mLen);
    }
    return in;
  }
  std::string noHyperLinks(const std::string& x){
    std::map<std::string,std::string> replMap;
    replMap["<"]="";
    replMap[">"]="";
    replMap["\""]="";
    replMap["\'"]="";
    replMap["\\"]="";
    return MFRF::findAndReplace(x,replMap);
  }
  std::string extractTextForTD(const std::string& in,const long & mLen){
    std::string out=MFRF::findAndReplace(in,DISPPF::GL_HTML_Tags.tagRemovalMap);
    out=SF::findAndReplace(out,"\\big","");
    out=truncateLength(out,mLen);
    out=noHyperLinks(out);
    return out;
  }
  std::string nextVectorRow(const std::vector<std::vector<std::string> > & v, long & i){
    std::string out;
    long vsz=v.size();
    long sz=0;
    while((i<vsz)&&(sz==0)){
      sz=v[i].size();
      ++i;
    }
    if(sz>0){
      --i;
      long printed=0;
      for(long j=0;j<sz;++j){
        if(printed){
          out+=". ";
        }
        printed=1;
        out+=v[i][j];
      }
      ++i;
    }
    return out;
  }
  std::string generateTitle(const PageElements& pels){
    std::string out;
    long i=0;
    out=nextVectorRow(pels.hTags,i);
    if(out.length()>GL_title.maxTitleLength){
      return extractTextForTD(out,GL_title.maxTitleLength);
    }
    return extractTextForTD(out,GL_title.maxTitleLength);
  }
  std::string generateDescription(const PageElements& pels){
    std::string out;
    long i=0;
    while(i<pels.hTags.size()){
      out+=nextVectorRow(pels.hTags,i);
      out+=". ";
    }
    out=extractTextForTD(out,GL_title.maxDescLength);
    return out;
  }
  PageElements generatePageElements(const std::string& rawText){
    PageElements res;
    std::string etS;
    long numHs=6;
    res.hTags.resize(numHs);
    for(long i=0;i<numHs;++i){
      etS="h"+std::to_string(i+1)+">";
      res.hTags[i]=SF::stringToVector(rawText,"<"+etS,"</"+etS);
    }
    res.pFormulations=SF::stringToVector(rawText,"\\begin{problem}","\\end{problem}");
    res.theorems=SF::stringToVector(rawText,"\\begin{theorem}","\\end{theorem}");
    res.definitions=SF::stringToVector(rawText,"\\begin{definition}","\\end{definition}");
    return res;
  }
}
#endif
