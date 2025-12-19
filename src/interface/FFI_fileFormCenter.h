//    GradeYard learning management system
//
//    Copyright (C) 2025 Ivan Matic, https://gradeyard.com
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

#ifndef _INCL_WI_FileFormCenter_H
#define _INCL_WI_FileFormCenter_H

namespace FFI{
  struct FormParameters{
    std::string openingTagForm1;
    std::string openingTagForm2;
    std::string openingTextInput1;
    std::string openingTextInput2;
    std::string closingText1;
    std::string closingText2;
    std::string openingAreaInput1;
    std::string openingAreaInput2;
    std::string closingArea;
    std::string closingTagForm1;
    std::string closingTagForm2;
    std::string saveButtonTitle;
    std::string calculateButtonTitle;
    std::string saveAsMessage;
    std::string editButtonTitle;
    std::string tableBegin;
    std::string tableEnd;
    std::string tdDarkCell;
    FormParameters();
  } GL_fParameters;
  FormParameters::FormParameters(){
    openingTagForm1="<FORM ACTION=\"";
    openingTagForm2="\" ENCTYPE=\"multipart/form-data\" METHOD=\"POST\">";
    closingTagForm1="<button type=\"submit\" name=\"runL\" ";
    closingTagForm1+="class=\"btn btn-primary mb-2\" VALUE=\"Submit\">";
    saveButtonTitle="Save";
    calculateButtonTitle="Calculate";
    editButtonTitle="Edit";
    saveAsMessage="Save as: ";
    closingTagForm2="</button>";
    closingTagForm2+="\n</FORM>\n";
    openingTextInput1="<INPUT NAME=\"";
    openingTextInput2="\" VALUE=\"";
    closingText1="\" TYPE=\"text\" SIZE=\"";
    closingText2="\">\n";
    openingAreaInput1="<textarea class=\"form-control\" name=\"";
    openingAreaInput2="\" rows=\"10\">";
    closingArea="</textarea>";
    tableBegin="<div class=\"table-responsive\"><table class=\"table table-bordered\" aria-label=\"main content\"><tbody>";
    tdDarkCell="<td class=\"text-white bg-dark\" align=\"center\">";
    tableEnd="</tbody></table></div>\n";
  }
  std::string GL_openingSeparatorInFileName="o1@2";
  std::string GL_closingSeparatorInFileName="5@6c";
  std::string GL_filePrefixRegularFile="rf";
  std::string GL_filePrefixUsersList="ul";
  std::string GL_respVariableName="fFSbm";
  std::string GL_respVariableConvForm="f17S";
  std::string GL_foSectionName="fillOut";
  std::string GL_calcSectionName="calcResult";
  std::string GL_vType="variableType";
  std::string GL_qLink="quickLink";
  std::string GL_numDecimals="numberOfDecimals";
  std::string GL_vValue="value";
  std::string GL_matrixName="matrixName";
  std::string GL_rowPosition="rowPosition";
  std::string GL_colPosition="colPosition";
  std::string GL_displayWidth="displayWidth";
  std::string GL_matrixTotalNumRows="totalNumRows";
  std::string GL_matrixTotalNumCols="totalNumCols";
  std::string GL_descriptionLabel="description";
  std::string GL_matrixTemplateLabel="matrixTemplate";
  std::string GL_codeInsertionLabel="codeInsertion";
  std::string GL_openTagForVNames="_o_";
  std::string GL_closeTagForVNames="_c_";
  std::string GL_urlForPDFPlacer="_|*urlForPDF*|_";
  std::string GL_svBTitle="saveButtonTitle";
  std::string GL_calcBTitle="calculateButtonTitle";
  std::string GL_editBTitle="editButtonTitle";
  std::string GL_mustBeLoggedIn="\\begin{box}You must be logged in to see this page.\\end{box}";
  std::string GL_displMatrixB="[displMatrix]";
  std::string GL_displMatrixE="[/displMatrix]";
  long GL_maxMatrixCoordinate=55;
  long GL_defaultDisplayWidth=10;
  long GL_defaultDisplayWidthForMatrix=10;
  std::string GL_nameForSpecialRequest="a1r8";
  std::string GL_deleteReq="d79";
  std::string GL_displayReq="d01";
  std::string GL_progSectionName="program";
  long GL_maxOSizeDocker=1000000;
  long GL_pad=100000;
  long GL_shift=2;// Must be >=2
  std::string GL_endStructureStrName="Str"+std::to_string(RNDF::randNum(10000));
  std::string GL_globalPrefix="GLOBAL_";
  struct PDFFormOptions{
  public:
    long maxTimeToKeepPDF=3600;
    std::string pdfsAllowed="no";
  } GL_PDFFormOptions;
  struct PairRC{
  public:
    long row;
    long col;
    int operator<(const PairRC& )const;
  };
  struct MatrixInfo{
  public:
    std::string matrixName;
    std::string varName;
    std::string quickJumpLink;
    PairRC coordinates;
    long nRows;
    long nCols;
    long displayWidth;
    int matrixCellIndicator;
    int matrixTemplateIndicator;
    long nDecimalsIfDouble;
    std::string type;
    MatrixInfo();
  };
  struct MatrixForDisplay{
  public:
    std::string name;
    std::string quickJumpLink;
    long numRows;
    long numCols;
    int matrixFromTemplateIndicator;
    std::map<PairRC,std::string> cells;
    MatrixForDisplay();
  };
  struct VariableInForm{
  public:
    std::string varName;
    std::string description;
    std::string type;
    std::string strValue;
    std::string quickJumpLink;
    std::string codeInsertionKey;
    double dblValue;
    long lValue;
    long nDecimals;
    MatrixInfo matrixData;
    long displayWidth;
    VariableInForm();
  };
  struct CodeExecutionParameters{
  public:
    std::vector<std::string> sources,languages,cFlags;
    std::vector<std::vector<std::string> > includes, dbIncludes,inputTestCases;
    std::string rawSource;
    std::string rawExecutionResult;
    CodeExecutionParameters();
  };
  class FormComponents{
  private:
    std::string templateName;
    std::string userName;
    std::string fileName;
    std::string regFilePath;
    std::string userFolderPath;
    std::string submissionFolderPath;
    void update();
  public:
    std::string mainPageName;
    std::string get_templateName() const;
    std::string get_userName() const;
    std::string get_fileName() const;
    std::string get_regFilePath() const;
    std::string get_userFolderPath() const;
    std::string get_submissionFolderPath() const;
    void set_templateName(const std::string& );
    void set_userName(const std::string& );
    void set_fileName(const std::string& );
    void set_parameters(const std::string&, const std::string&, const std::string& );
    std::string specialRequest;
    long maxNumberOfFiles;
    std::map<std::string,VariableInForm> fillOutVariables;
    std::map<std::string,VariableInForm> calculatedVariables;
    std::vector<std::string> fillOutOrdering;
    CodeExecutionParameters cep;
    std::string htmlSrcDoc, htmlSrcCont, latexSrcDoc, latexSrcCont;
    std::map<std::string,MatrixForDisplay> matrices;
    std::set<std::string> mTemplates;
    std::map<std::string,std::string> mapCodeInsertions;
    std::string title;
    FormComponents();
  };
  struct QuestionForForm{
  public:
    std::string questionDisplay;
    std::string questionDisplayForMatrix;
    std::string preFilledText;
    std::string assignedLink;
    std::string quickJumpLink;
    std::string parentMatrix;
    std::string textAreaIndicator;
    long textWidth;
  };
  struct QuestionCollector{
  public:
    std::vector<QuestionForForm> v;
    std::map<std::string,long> assignedLinkTranslator;
  };
}
#endif
