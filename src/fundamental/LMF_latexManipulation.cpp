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


#ifndef _INCL_LATEXMANIPULATION_CPP
#define _INCL_LATEXMANIPULATION_CPP
namespace LMF{
  struct LPrintingInstructions{
    long numChoicesInOneLine;
  };
  struct LatexDataSingleQuestion{
    std::string formulation;
    std::string questionType;
    std::string multipleChoicesSt;
    std::string pointsSt;
    long questionNumber;
    std::string solution;
    std::string answer;
    LPrintingInstructions lPInst;
  };
  struct LatexDataExamPaper{
    std::string studentName;
    std::string examCode;
    std::string studentCode;
    std::string courseName;
    std::string courseCode;
    std::string storageLocation;
    std::string examPreambleLatexTemplate;
    std::string answerBoxLatexTemplate;
    std::string prefix;
    std::string addNameIndicator;
    std::string pdfTopOfThePage;
    std::set<std::string> pdfNewPageSeparators;
    std::stack<LatexDataSingleQuestion> questionsStack;
  };
  LPrintingInstructions getPrintingInstructions(const std::string & rawText){
    LPrintingInstructions fR;
    fR.numChoicesInOneLine=-1;
    std::pair<std::string,int> allD; long pos;
    pos=0; allD=SF::extract(rawText,pos,"_choicesInLine*_","_/choicesInLine*_");
    if(allD.second==1){
      fR.numChoicesInOneLine=BF::stringToInteger(allD.first);
    }
    return fR;
  }
  long calculateWidth(const std::string & _input){
    std::string input=_input;
    std::string input2=input;
    input=SF::findAndReplace(input,"eqnarray","a");
    input=SF::findAndReplace(input,"$$","a");
    if(input.length()!=input2.length()){
      return 500;
    }
    input=SF::findAndReplace(input,"\\frac","");
    input=SF::findAndReplace(input,"\\hfill","");
    input=SF::findAndReplace(input,"\\mathbb","");
    input=SF::findAndReplace(input,"\\mathcal","");
    input=SF::findAndReplace(input,"\\noindent","");
    input=SF::findAndReplace(input,"\\alpha","a");
    input=SF::findAndReplace(input,"\\beta","a");
    input=SF::findAndReplace(input,"\\gamma","a");
    input=SF::findAndReplace(input,"\\delta","a");
    input=SF::findAndReplace(input,"\\varphi","a");
    input=SF::findAndReplace(input,"\\phi","a");
    input=SF::findAndReplace(input,"\\Phi","a");
    input=SF::findAndReplace(input,"\\Gamma","a");
    input=SF::findAndReplace(input,"\\nabla","a");
    input=SF::findAndReplace(input,"\\circ","a");
    input=SF::findAndReplace(input,"\\mapsto","a");
    input=SF::findAndReplace(input,"\\to","a");
    input=SF::findAndReplace(input,"\\left","a");
    input=SF::findAndReplace(input,"\\right","a");
    input=SF::findAndReplace(input,"\\equiv","a");
    input=SF::findAndReplace(input,"\\cong","a");
    input=SF::findAndReplace(input,"\\triangle","a");
    input=SF::findAndReplace(input,"\\star","a");
    input=SF::findAndReplace(input,"\\ast","a");
    return input.length();
  }
  long calculateProblemHeight(const std::string & _input){
    std::string input=_input;
    input=SF::findAndReplace(input,"\n","_nL*_s_/nL*_");
    input=SF::findAndReplace(input,"\\item","_nL*_s_/nL*_");
    input=SF::findAndReplace(input,"\\\\","_nL*_s_/nL*_");
    input=SF::findAndReplace(input,"\\begin{enumerate}","_nL*_s_/nL*_");
    input=SF::findAndReplace(input,"\\begin{itemize}","_nL*_s_/nL*_");
    long countNLines=SF::countInString(input,"_nL*_","_/nL*_");
    return countNLines + (input.length() / 150);
  }
  std::string prepareMultipleChoiceText(const std::string & mcst){
    long numCharactersToSwitchToLines=120;
    std::string fRSpaces="", fRNLines="";
    std::string fRSpacesStart="\n\n \\vspace{0.3cm}\n \\noindent ";
    std::string fRNLinesStart="\n\n \\vspace{0.3cm}\n \\noindent ";
    std::vector<std::string> mcVect=SF::stringToVector(mcst,"_rb*_","_/rb*_");
    long vSize=mcVect.size();
    std::vector<std::string> vPair;
    long doneFirst=0;
    for(long i=0;i<vSize;++i){
      vPair=SF::stringToVector(mcVect[i],"_vl*_","_/vl*_");
      if(vPair[0]!="N"){
        if(doneFirst==1){
          fRSpaces+="\\hfill ";
          fRNLines+="\n\n \\noindent ";
        }
        if(vPair.size()>1){
          fRSpaces+=vPair[1]+" ";
          fRNLines+=vPair[1]+" ";
        }
      }
      doneFirst=1;
    }
    long wdth=calculateWidth(fRSpaces);
    if(wdth>numCharactersToSwitchToLines){
      return fRNLinesStart+ fRNLines;
    }
    return fRSpacesStart+ fRSpaces;
  }
  std::string prepareMultipleChoiceText(const std::string & mcst,const long& chInOneLine){
    std::string currentLine="", entireText="";
    std::string startLine="\n\n \\vspace{0.3cm}\n \\noindent ";
    std::vector<std::string> mcVect=SF::stringToVector(mcst,"_rb*_","_/rb*_");
    long vSize=mcVect.size();
    std::vector<std::string> vPair;
    long flushed=1;
    for(long i=0;i<vSize;++i){
      vPair=SF::stringToVector(mcVect[i],"_vl*_","_/vl*_");
      if(vPair[0]!="N"){
        if(flushed==0){
          currentLine+="\\hfill ";
        }
        else{
          currentLine+="\n\n \\noindent ";
        }
        if(vPair.size()>1){
          currentLine+=vPair[1]+" ";
        }
        flushed=0;
      }
      if(i%chInOneLine == chInOneLine-1){
        entireText+=currentLine;
        currentLine="";
        flushed=1;
      }
    }
    if(flushed==0){
      entireText+=currentLine;
    }
    return entireText;
  }
  std::string htmlToLatexPictures(const std::string & _input){
    std::string input=_input;
    input=SF::findAndReplace(input,"\"/>","\">");
    long pos;
    std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(input,pos,"<img ","\">");
    std::string fName,oldText;
    while(allD.second==1){
      fName=allD.first;
      oldText="<img "+fName+"\">";
      fName+="_/n*_ end";
      pos=0;allD=SF::extract(fName,pos,"src=\"","_/n*_");
      if(allD.second==1){
        fName=allD.first;
        fName=SF::findAndReplace(fName,".gif",".png");
        input=SF::findAndReplace(input,oldText,"\\includegraphics[scale=0.3]{"+fName+"}");
      }
      pos=0;allD=SF::extract(input,pos,"<img ","\">");
    }
    return input;
  }
  std::string htmlToLatexFormatting(const std::string &_input){
    std::string input=_input;
    input=SF::findAndReplace(input,"<b>","{\\bf ");
    input=SF::findAndReplace(input,"</b>","}");
    input=SF::findAndReplace(input,"<i>","{\\em ");
    input=SF::findAndReplace(input,"</i>","}");
    input=SF::findAndReplace(input,"_cde_","\\verb@");
    input=SF::findAndReplace(input,"_/cde_","@");
    input=SF::findAndReplace(input,"_code_","\\begin{verbatim}");
    input=SF::findAndReplace(input,"_/code_","\\end{verbatim}");
    input=SF::findAndReplace(input,"<code>","\\verb@");
    input=SF::findAndReplace(input,"</code>","@");
    input=SF::findAndReplace(input,"<pre>","\\begin{verbatim}");
    input=SF::findAndReplace(input,"</pre>","\\end{verbatim}\n");
    input=SF::findAndReplace(input,"\\newline","\\\\");
    input=SF::findAndReplace(input,"<ul>","\\begin{itemize}");
    input=SF::findAndReplace(input,"</ul>","\\end{itemize}");
    input=SF::findAndReplace(input,"<center>","\\begin{center}");
    input=SF::findAndReplace(input,"</center>","\\end{center}");
    input=SF::findAndReplace(input,"<li>","\\item ");
    input=SF::findAndReplace(input,"</li>","");
    input=SF::findAndReplace(input,"<br>","\n\n");
    input=SF::findAndReplace(input,"<p>","\n\n\\vspace{0.1cm}\n\\noindent ");
    input=SF::findAndReplace(input,"<div>","\n\n");
    input=SF::findAndReplace(input,"</p>","");
    input=SF::findAndReplace(input,"</div>","");
    input=SF::findAndReplace(input,"</br>","");
    input=htmlToLatexPictures(input);
    return input;
  }
  std::string prepareSingleProblemForPrinting(const LatexDataSingleQuestion & sq,const LatexDataExamPaper & exam){
    std::string fR="";
    fR+="\\item["+std::to_string(sq.questionNumber +1)+"] ";
    fR+="{\\bf ("+sq.pointsSt+" points)}";
    fR+=sq.formulation;
    if(sq.questionType=="radioButtonsField"){
      if(sq.lPInst.numChoicesInOneLine>1){
        fR+=prepareMultipleChoiceText(sq.multipleChoicesSt,sq.lPInst.numChoicesInOneLine);
      }
      else{
        fR+=prepareMultipleChoiceText(sq.multipleChoicesSt);
      }
    }
    if(sq.questionType=="textInputField"){
      fR+=exam.answerBoxLatexTemplate;
    }
    fR=htmlToLatexFormatting(fR);
    return fR;
  }
  std::vector<std::string> uniqueRandomCodes(const long& numCodes,
                                             const long& noiseSize,
                                             const long& numVersions){
    std::vector<std::string> res;
    res.resize(numCodes);
    long padNum=1;
    while(padNum<numCodes){
      padNum*=10;
    }
    padNum/=10;

    std::vector<long> randP=RNDF::genRandPermutation(numCodes);
    for(long i=0;i<numCodes;++i){
      for(long j=0;j<noiseSize;++j){
        res[i]+=std::to_string(RNDF::randNum(10));
      }
      res[i]+=BF::padded(randP[i],padNum,"0");
    }
    if(numCodes>0){
      long csz=res[0].size();
      if(csz>0){
        std::vector<long> digitPermutation=RNDF::genRandPermutation(csz);
        for(long i=0;i<numCodes;++i){
          res[i]=BF::permuteString(res[i],digitPermutation);
        }
      }
    }
    return res;
  }
  std::vector<std::string> genCodes(const long& numCodes,
                                    const long& noiseSize,
                                    const long& numVersions,
                                    const long& versionHidingShift=0,
                                    const long& versionHidingIndex=0){
    std::vector<std::string> res;
    if(numCodes<1){return res;}
    if(noiseSize<1){return res;}
    if(numVersions<1){return res;}
    if(versionHidingShift<0){return res;}
    if(versionHidingIndex<0){return res;}
    if(versionHidingIndex>noiseSize){return res;}
    res=uniqueRandomCodes(numCodes,noiseSize,numVersions);
    long padNum=1;
    while(padNum<numVersions){
      padNum*=10;
    }
    padNum/=10;
    long sz=res.size();
    long currentVersion=versionHidingShift;
    std::string versionAddition;
    char tmp;
    for(long i=0;i<sz;++i){
      currentVersion=currentVersion%numVersions;
      versionAddition=BF::padded(currentVersion,padNum,"0");
      if(versionAddition.size()>0){
        tmp=versionAddition[0];
        versionAddition[0]=res[i][versionHidingIndex];
        res[i][versionHidingIndex]=tmp;
      }
      res[i]+=versionAddition;
      ++currentVersion;
    }
    return res;
  }
  std::string addCodeToSource(const std::string & source,  const std::string & code, const std::string & codePlaceHolder="_|*StudentCode*|_"){
    std::string replacementText="";
    long sz=code.length();
    if(sz>15){sz=15;}
    long i=sz;
    std::string toAdd;
    while(i>0){
      --i;
      toAdd="&";
      toAdd+=code[i];
      replacementText=toAdd+replacementText;
    }
    std::string startingSpace="";
    for(long i=0;i<15-sz;++i){
      startingSpace+="&{ }^{ }";
    }
    replacementText=startingSpace+replacementText;
    std::string startingCode,endingCode;
    startingCode="\n \\noindent $\\;$ \\hfill $\\begin{array}{cc|ccccccccccccccc|}\\cline{3-17} ";
    startingCode+="&&{ }^{ }&{ }^{ }&{ }^{ }&{ }^{ }&{ }^{ }&{ }^{ }&{ }^{ }&{ }^{ }&{ }^{ }&{ }^{ }&{ }^{ }&{ }^{ }&{ }^{ }&{ }^{ }&{ }^{ } \\\\ \n";
    startingCode+="\\mbox{Code:} & ";
    endingCode="\\\\ \n";
    endingCode+="\\cline{3-17}\\end{array}$ \n\n";
    endingCode+="$$\\quad$$\n\n";
    replacementText=startingCode+replacementText+endingCode;
    return SF::findAndReplace(source,codePlaceHolder,replacementText);
  }
  std::string prepareLatexSource(const LatexDataExamPaper & exam, const long &indInClass, const std::string &addOnTop="", const std::string &addToEnd=""){
    std::string fR="";
    long maxHeightForPage=25;
    std::string problemBreakString="\n\n\\vfill\n\n";
    std::string pageBreakString=problemBreakString+ "\\clearpage\n";
    std::string codeBoxTop="";
    if(indInClass==1){
      maxHeightForPage=75;
      codeBoxTop="_|*StudentCode*|_";
      pageBreakString+=codeBoxTop;
    }
    std::stack problems=exam.questionsStack;
    SF::flipTheStack(problems);
    std::string singleProblem;long pHeight;
    long currentHeight=0;
    long problemCounter=0;
    while(!problems.empty()){
      singleProblem=prepareSingleProblemForPrinting(problems.top(),exam);
      if(exam.pdfNewPageSeparators.size()<1){
        pHeight=calculateProblemHeight(singleProblem);
        if(currentHeight>0){
          if(currentHeight+pHeight>maxHeightForPage){
            fR+=pageBreakString;
            currentHeight=pHeight;
          }
          else{
            currentHeight+=pHeight;
          }
        }
        else{
          currentHeight=pHeight;
        }
      }
      else{
        if((exam.pdfNewPageSeparators).find(std::to_string(problemCounter))!=(exam.pdfNewPageSeparators).end()){
          fR+=pageBreakString;
        }
        ++problemCounter;
      }
      fR+=singleProblem;
      fR+=problemBreakString;
      problems.pop();
    }
    fR= "\n\\begin{enumerate}\n"+ fR+"\\end{enumerate}\n%%%EXAM VERSION END%%%";
    if(indInClass==0){
      std::string nmToAdd="\\begin{center} {\\bf "+exam.studentName+"} \\end{center}\n\n";
      if(exam.addNameIndicator=="no"){
        nmToAdd="";
      }
      fR=exam.pdfTopOfThePage+ nmToAdd+fR;
      fR="\n\n \\begin{document}"+fR+"\n\n\\end{document} \n";
      fR=exam.examPreambleLatexTemplate+fR;
    }
    else{
      fR="%%%EXAM VERSION BEGIN%%%\n\n"+codeBoxTop+fR+ "\n\n \\cleardoublepage\n";
    }
    if(addOnTop!=""){
      fR=SF::findAndReplace(addOnTop,"_*codeBoxTop*_",codeBoxTop)+fR;
    }
    if(addToEnd!=""){
      fR+=SF::findAndReplace(addToEnd,"_*codeBoxTop*_",codeBoxTop);
    }
    return fR;
  }
  int createPdfFromTex(const std::string &lSource, const std::string &lFName, const std::string &storageLocation){
    IOF::toFile(lFName+".tex",lSource);
    std::string latexCommand="pdflatex -interaction=batchmode -output-directory="+storageLocation;
    latexCommand+=" "+lFName+".tex";
    IOF::sys_runLatexCommand(latexCommand);
    IOF::sys_deleteFile(lFName+".out");
    IOF::sys_deleteFile(lFName+".log");
    IOF::sys_deleteFile(lFName+".aux");
    IOF::sys_deleteFile(lFName+".dvi");
    IOF::sys_deleteFile(lFName+".tex");
    return 1;
  }
  std::string prepareExamForPrinting(const LatexDataExamPaper & exam, const long & indInClass, const std::string &beforeProblems, const std::string &afterProblems){
    std::string lSource=prepareLatexSource(exam,indInClass,beforeProblems,afterProblems);
    if(indInClass==1){
      return lSource;
    }
    std::string lFName=exam.storageLocation+"/"+exam.prefix+exam.examCode;
    if(exam.addNameIndicator=="yes"){
      lFName+="_"+BF::cleanAllSpaces(exam.studentName);
    }
    createPdfFromTex(lSource,lFName,exam.storageLocation);
    return lFName+".pdf";
  }
  std::string finalizeLatexForInClassExam(const std::string & rawLatex,const std::string & latexTop,const std::string & fName, const std::string & storageLocation){
    std::string finalSource=latexTop;
    finalSource+="\n\n \\begin{document}\n";
    finalSource+=rawLatex+"\n\n\\end{document} \n";
    createPdfFromTex(finalSource,fName,storageLocation);
    return finalSource;
  }
  std::string correctSSWithoutBraces(const std::string &s, const char & needle){
    long pos=0;
    std::pair<std::string,int> allD;
    std::string res="";
    long sz=s.length();
    long i=0;
    while(i<sz){
      if(s[i]==needle){
        if(i<sz-1){
          if(s[i+1]=='{'){
            res+=needle;
          }
          else{
            res+=needle;
            res+="{";
            res+=s[i+1];
            res+="}";
            ++i;
          }
        }
        else{
          res+=s[i];
        }
      }
      else{
        res+=s[i];
      }
      ++i;
    }
    return res;
  }
  std::string correctSubscriptsAndSuperscriptsWithoutBraces(const std::string &s){
    return correctSSWithoutBraces(correctSSWithoutBraces(s,'_'),'^');
  }
}
#endif
