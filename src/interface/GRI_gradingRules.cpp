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


#ifndef _INCL_GRI_GradingRules_CPP
#define _INCL_GRI_GradingRules_CPP

namespace GRI{
  struct GradeUpdateFromRules{
  public:
    std::string rawScore;
    std::string scoreForDisplay;
    double scoreForCalculations;
    std::string rawComment;
    std::string grCommentDisplay;
  };
  GradeUpdateFromRules getUpdateFromRules(const std::map<std::string,PSDI::GradingRule>& mRules,
                                          const std::string& gComment,
                                          const std::string& maxPoints){
    GradeUpdateFromRules res;
    long pos;std::pair<std::string,int>allD;
    res.rawScore="";
    double calculatedScore=0.0; std::string displScoreToOverwrite="";
    pos=0;allD=SF::extract(gComment,pos,"_score_","_/score_");
    if(allD.second==1){
      res.rawScore=allD.first;
      res.scoreForCalculations=BF::stringToDouble(res.rawScore);
    }
    DISPPF::RequestsForSanitizer reqS;
    res.scoreForDisplay=DISPPF::sanitizeForDisplay(res.rawScore,reqS);
    res.rawComment="";
    pos=0;allD=SF::extract(gComment,pos,"_comment_","_/comment_");
    if(allD.second==1){
      res.rawComment=allD.first;
      res.grCommentDisplay=res.rawComment;
      std::vector<std::string> rlsUsed=SF::stringToVector(res.grCommentDisplay,MWII::GL_WI.getDefaultWebText("[rule]"),MWII::GL_WI.getDefaultWebText("[/rule]"));
      long sz=rlsUsed.size();
      std::map<std::string,PSDI::GradingRule>::const_iterator it,itE=mRules.end();
      for(long i=0;i<sz;++i){
        it=mRules.find(rlsUsed[i]);
        if(it!=itE){
          res.grCommentDisplay=SF::findAndReplace(res.grCommentDisplay,MWII::GL_WI.getDefaultWebText("[rule]")+it->first+MWII::GL_WI.getDefaultWebText("[/rule]"),(it->second).display);
          displScoreToOverwrite="y";
          calculatedScore+=(it->second).points;
        }
      }
      if(displScoreToOverwrite!=""){
        if(calculatedScore<0.0){
          calculatedScore+=BF::stringToDouble(maxPoints);
        }
        res.scoreForCalculations=calculatedScore;
        res.scoreForDisplay=BF::doubleToString(calculatedScore);
      }
      res.grCommentDisplay=DISPPF::prepareForHTMLDisplay(res.grCommentDisplay);
    }
    return res;
  }
  void updateScoresIfRulesWereUsed(std::string& scoreHolder,
                                   double& score,
                                   const std::map<std::string,std::map<std::string,PSDI::GradingRule> > & gRulesAll,
                                   const std::map<std::string,std::string>& maxPoints,
                                   const std::string& qLabel,
                                   const std::string& gComments){
    GradeUpdateFromRules gur;
    std::map<std::string,std::string>::const_iterator itSM,itSME;
    itSME=maxPoints.end();
    itSM=maxPoints.find(qLabel);
    std::string mPoints="10";
    if(itSM!=itSME){
      mPoints=itSM->second;
    }
    std::map<std::string,std::map<std::string,PSDI::GradingRule> >::const_iterator it,itE;
    itE=gRulesAll.end();
    it=gRulesAll.find(qLabel);
    if(it==itE){
      std::map<std::string,PSDI::GradingRule> empty;
      gur=getUpdateFromRules(empty,gComments,mPoints);
    }
    else{
      gur=getUpdateFromRules(it->second,gComments,mPoints);
    }
    scoreHolder=gur.scoreForDisplay;
    score+=gur.scoreForCalculations;
  }
  void updateCommentAndScoreWithGradingRules(RTI::SingleQuestionInfo& sqi){
    if(sqi.displType=="certificate"){return;}
    GradeUpdateFromRules gur=getUpdateFromRules(sqi.gRulesMap,sqi.graderComment,sqi.maxPoints);
    sqi.rawScore=gur.rawScore;
    sqi.scoreForDisplay=gur.scoreForDisplay;
    sqi.scoreForCalculations=gur.scoreForCalculations;
    sqi.rawComment=gur.rawComment;
    sqi.grCommentDisplay=gur.grCommentDisplay;
    return;
  }
  std::string rulesToString(const std::map<std::string,PSDI::GradingRule>& m){
    std::string out;
    std::map<std::string,PSDI::GradingRule>::const_iterator it,itE;
    it=m.begin();itE=m.end();
    while(it!=itE){
      out+="_rl*_\n";
      out+="_rule_"+it->first+"_/rule_\n";
      out+="_points_"+std::to_string((it->second).points)+"_/points_\n";
      if((it->second).display!="notFound"){
        out+="_display_"+(it->second).display+"_/display_\n";
      }
      out+="_/rl*_";
      ++it;
    }
    return out;
  }
  std::map<std::string,PSDI::GradingRule> gradingRulesMapFromRaw(const std::string& rawSt){
    std::map<std::string,PSDI::GradingRule> res;
    std::vector<std::string> rVector=SF::stringToVector(rawSt,"_rl*_","_/rl*_");
    long sz=rVector.size();
    std::pair<std::string,int> allD; long pos;
    for(long i=0;i<sz;++i){
      PSDI::GradingRule nextRule;std::string nextName;
      pos=0; allD=SF::extract(rVector[i],pos,"_rule_","_/rule_");
      if(allD.second==1){
        nextName=allD.first;
        allD=SF::extract(rVector[i],pos,"_points_","_/points_");
        if(allD.second==1){
          nextRule.points=BF::stringToDouble(allD.first);
          pos=0;allD=SF::extract(rVector[i],pos,"_display_","_/display_");
          nextRule.display=nextName;
          if(allD.second==1){
            nextRule.display=allD.first;
          }
          res[nextName]=nextRule;
        }
      }
    }
    return res;
  }
  std::string updateSingleProblem(const std::string& _in, const std::map<std::string,std::map<std::string,PSDI::GradingRule> >& mRules){
    long pos; std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(_in,pos,"_lb*|_","_/lb*|_");
    if(allD.second==0){
      return _in;
    }
    std::map<std::string,std::map<std::string,PSDI::GradingRule> >::const_iterator it_b,it_bE=mRules.end();
    it_b=mRules.find(allD.first);
    if(it_b==it_bE){
      return _in;
    }
    std::map<std::string,PSDI::GradingRule> allRules;
    std::string in=_in;
    pos=0;allD=SF::extract(in,pos,"_gru*_","_/gru*_");
    if(allD.second==0){
      in=SF::findAndReplace(in,"_lb*|_","_gru*__/gru*__lb*|_");
    }
    else{
      allRules=gradingRulesMapFromRaw(allD.first);
    }
    std::map<std::string,PSDI::GradingRule>::const_iterator it,itE=(it_b->second).end();
    it=(it_b->second).begin();
    while(it!=itE){
      if((it->second).display=="*del*"){
        allRules.erase(it->first);
      }
      else{
        allRules[it->first]=it->second;
      }
      ++it;
    }
    pos=0;allD=SF::extractAndReplace(in,pos,"_gru*_","_/gru*_",0,"_gru*_\n"+rulesToString(allRules)+"_/gru*_");
    if(allD.second==0){
      return _in;
    }
    return allD.first;
  }
  std::string updateGradingRules(const std::string & in, const std::map<std::string,std::map<std::string,PSDI::GradingRule> >& mRules){
    std::string out;
    long oldPosSave;
    long pos=0; std::pair<std::string,int> allD;
    long len=in.length();
    long openTagLen=6;
    std::string nonQuestionTextToCopy;
    while(pos<len){
      oldPosSave=pos;
      allD=SF::getEverythingBefore(in,pos,"_in*|_");
      if(allD.second==0){
        pos=SF::copyTheRestOfTheStringAndGiveUp(in,oldPosSave,out,1);
      }
      else{
        nonQuestionTextToCopy=allD.first;
        pos-=openTagLen;
        allD=SF::extract(in,pos,"_in*|_","_/in*|_");
        if(allD.second==0){
          return in;
        }
        else{
          out+=nonQuestionTextToCopy+"_in*|_"+updateSingleProblem(allD.first,mRules)+"_/in*|_";
        }
      }
    }
    return out;
  }
}

#endif
