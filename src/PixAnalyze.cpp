#define PixAnalyze_cxx
#include "../include/PixAnalyze.hpp"
#include <TH2.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TBox.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TPaveText.h>
#include <iostream>

void PixAnalyze::Loop()
{
//   In a ROOT session, you can do:
//      root> .L PixAnalyze.C
//      root> PixAnalyze t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

    timeStamp_beginValue      = new long long[nentries];
    timeStamp_endValue        = new long long[nentries];
    nClusterValue             = new double[nentries];

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      timeStamp_beginValue[jentry] = timeStamp_begin;
      timeStamp_endValue[jentry] = timeStamp_end;
      nClusterValue[jentry] = nCluster;
   }
}

TH1F* PixAnalyze::nClusterAverageTot(TString s, TString axeX, TString axeY, int n, TString log, long long min1, long long max1, long long min2, long long max2, double& mean, double& error){
    TCanvas* c = new TCanvas(s,"", 200,10,800,600);
    TH1F* h = new TH1F("","", 100, 0, 5000);
    for(int i=0; i<n; i++){
        if((timeStamp_beginValue[i] > min1 && timeStamp_beginValue[i] < max1) || (timeStamp_beginValue[i] > min2 && timeStamp_beginValue[i] < max2))
            h->Fill(nClusterValue[i]);
    }

    h->GetXaxis()->SetTitle(axeX);
    h->GetYaxis()->SetTitle(axeY);

    h->Draw();
    h->SetLineWidth(2);        h->SetLineColor(kBlue);

    if(log=="log")
        c->SetLogy();

    mean = h->GetMean(1);
    error = h->GetMeanError(1);

    c->SaveAs("results/"+s+".png");
    return h;
}

void PixAnalyze::nClusterComp(TString s, TH1F* h1, TH1F* h2){
    TCanvas* c = new TCanvas(s,"", 200,10,800,600);
    h1->SetStats(0);
    h1->Draw();
    h2->SetLineColor(kRed);
    h2->Draw("SAME");

    TLegend* legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->AddEntry(h1,"FPix","l"); // option "l" is for line (form of legend)
    legend->AddEntry(h2,"BPix","l");
    legend->Draw();

    c->SetLogy();
    c->SaveAs("results/"+s+".png");
}

TH1F* PixAnalyze::nClusterZoom(TString s, TString axeX, TString axeY, int n, int nbin, TString log, long long min1, long long max1, long long min2, long long max2, double& mean, double& error){
    TCanvas* c = new TCanvas(s,"", 200,10,800,600);
    TH1F* h = new TH1F("","", nbin, 0, nbin);
    for(int i=0; i<n; i++){
        if((timeStamp_beginValue[i] > min1 && timeStamp_beginValue[i] < max1) || (timeStamp_beginValue[i] > min2 && timeStamp_beginValue[i] < max2))
            h->Fill(nClusterValue[i]);
    }

    h->GetXaxis()->SetTitle(axeX);
    h->GetYaxis()->SetTitle(axeY);
    h->SetStats(0);

    h->Draw();
    h->SetLineWidth(2);        h->SetLineColor(kBlue);

    if(log=="log")
        c->SetLogy();

    mean = h->GetMean(1);
    error = h->GetMeanError(1);

    c->SaveAs("results/"+s+".png");
    return h;
}


TH1F* PixAnalyze::timeHisto(TString s, TString axeX, TString axeY, int div, int n, long long minRes, long long maxRes, double& mean, double& error, bool isSystematics, double syst){
    TH1F* foo;
    TString minResString = Form("%d", minRes);
    TString maxResString = Form("%d", maxRes);
    int step = (maxRes-minRes)/((double)div);
    long double moyClust[div]{0};
    double stdClust[div]{0};
    int nb[div] = {0};
    double x[div];

    for(int i=0; i<div; i++){
        x[i] = i;
        for(int j=0; j<n; j++){
            if((minRes+i*step) < timeStamp_beginValue[j] && timeStamp_beginValue[j] < (minRes+(i+1)*step)){
                moyClust[i] += nClusterValue[j];
                nb[i]++;
            }
        }
        if(nb[i]==0)
            moyClust[i]=0;
        else
            moyClust[i] /= (double)nb[i];
        for(int j=0; j<n; j++){
            if((minRes+i*step) < timeStamp_beginValue[j] && timeStamp_beginValue[j] < (minRes+(i+1)*step))
                stdClust[i] += nClusterValue[j]*nClusterValue[j] - 2*nClusterValue[j]*moyClust[i] + moyClust[i]*moyClust[i];
        }
        if(nb[i]==0)
            stdClust[i]=0;
        else
        stdClust[i] /= (double)nb[i];
        stdClust[i] = sqrt(stdClust[i]);
    }
    TCanvas* c = new TCanvas(s+"("+minRes+"<timeStamp<"+maxRes+")","", 200,10,800,600);
    TH1F* h = new TH1F("","", div, minRes, maxRes);
    for(int i=0; i<div; i++){
        h->SetBinContent(i+1, moyClust[i]);
        h->SetBinError(i+1, (stdClust[i])/(sqrt(nb[i])));
    }

    h->SetMaximum(0.3);
    h->SetMinimum(0);

    h->GetXaxis()->SetTitle(axeX);
    h->GetYaxis()->SetTitle(axeY);

    h->SetStats(0);

    h->Draw();
    h->SetLineWidth(2);        h->SetLineColor(kBlue);

    double ntot=0;
    for(int i=0; i<div; i++){
        ntot += nb[i];
    }

    double moy = 0;
    double err = 0;
    for(int i=0; i<div; i++){
        moy += (nb[i]/ntot)*moyClust[i];
        err += stdClust[i]*stdClust[i]/((double)nb[i]);
    }
    err = sqrt(err)/10.;

    if(isSystematics){
        TBox* b = new TBox(minRes, moy+err+syst, maxRes, moy-err-syst);
        b->SetFillColor(6);
        b->SetFillStyle(3001);
        b->Draw("SAME");
        c->SaveAs("results/"+s+"("+minRes+"<timeStamp<"+maxRes+")"+"Sys.png");
    }
    else if(!isSystematics)
        c->SaveAs("results/"+s+"("+minRes+"<timeStamp<"+maxRes+")"+".png");

    mean = moy;
    error = err;


    foo=h;
    return foo;
}

void PixAnalyze::systematic(TString s, int div, int n, long long min1, long long max1, long long min2, long long max2, double& mean, double& error){


//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//

  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

  // For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

  // For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

  //tdrStyle->SetEndErrorSize(2);
  //tdrStyle->SetErrorMarker(20);
  //tdrStyle->SetErrorX(0.);

  tdrStyle->SetMarkerStyle(20);

  //For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

  //For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

  // For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

  // Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.13);
  tdrStyle->SetPadRightMargin(0.05);

  // For the Global title:

  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.1);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

  // For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.04, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(1);
  tdrStyle->SetTitleYOffset(1.5);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.03, "XYZ");

  // For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

  // Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  // Postscript options:
  // tdrStyle->SetPaperSize(15.,15.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();

//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//



    long long minRes = 0;
    long long maxRes = 0;
    long long min[2];
    long long max[2];
    min[0] = min1; min[1] = min2;
    max[0] = max1; max[1] = max2;
    double moy = 0;
    double err = 0;
    int step;
    long double moyClust[div]{0};
    double stdClust[div]{0};
    int nb[div] = {0};
    double x[div];
    TCanvas* c = new TCanvas(s,"", 600,600,600,600);
    TH1F* h = new TH1F("","", 2*div, min1, max2);

    for(int k=0; k<2; k++){
        minRes = min[k];
        maxRes = max[k];
        step = (maxRes-minRes)/((double)div);
        for(int i= 0 ; i<div; i++){
            moyClust[i]=0;
            stdClust[i]=0;
            nb[i]=0;
            x[i]=0;
        }

        for(int i=0; i<div; i++){
            x[i] = i;
            for(int j=0; j<n; j++){
                if((minRes+i*step) < timeStamp_beginValue[j] && timeStamp_beginValue[j] < (minRes+(i+1)*step)){
                    moyClust[i] += nClusterValue[j];
                    nb[i]++;
                }
            }
            if(nb[i]==0)
                moyClust[i]=0;
            else
                moyClust[i] /= (double)nb[i];
            for(int j=0; j<n; j++){
                if((minRes+i*step) < timeStamp_beginValue[j] && timeStamp_beginValue[j] < (minRes+(i+1)*step))
                    stdClust[i] += nClusterValue[j]*nClusterValue[j] - 2*nClusterValue[j]*moyClust[i] + moyClust[i]*moyClust[i];
            }
            if(nb[i]==0)
                stdClust[i]=0;
            else
            stdClust[i] /= (double)nb[i];
            stdClust[i] = sqrt(stdClust[i]);
        }

        h->SetMaximum(0.3);
        h->SetMinimum(0);

        h->GetXaxis()->SetTitle("time values");
        h->GetYaxis()->SetTitle("<nCluster>");

        h->SetStats(0);

        h->Draw("");
        h->SetLineWidth(2);        h->SetLineColor(kBlue);

        for(int i=0; i<div; i++){
            if(k==0){
                h->SetBinContent(i+1, moyClust[i]);
                h->SetBinError(i+1, (stdClust[i])/(sqrt(nb[i])));
                moy += moyClust[i];
                err += stdClust[i]*stdClust[i]/nb[i];
            }
            if(k==1){
                h->SetBinContent(i+div+1, moyClust[i]);
                h->SetBinError(i+div+1, (stdClust[i])/(sqrt(nb[i])));
                moy += moyClust[i];
                err += stdClust[i]*stdClust[i]/nb[i];
            }
        }
    }

    mean = moy/(2*div);
    err = sqrt(err);
    error = err/(2*div);

    TBox* b = new TBox(min2+200, 0.169+0.01, max1-200, 0.169-0.01);
    b->SetFillColor(6);
    b->SetFillStyle(3001);
    b->Draw("SAME");

    TLine *line = new TLine(1530427080,0,1530427080,0.3);
    line->SetLineColor(kRed);
    line->Draw();

    TPaveText *pt1 = new TPaveText(1530412500,.1,1530413500,.2,"br");
    pt1->AddText("long separation scan 1");
    pt1->Draw("SAME");
    TPaveText *pt2 = new TPaveText(1530412500,.2,1530413500,.3,"br");
    pt2->AddText("long separation scan 2");
    pt2->Draw("SAME");

    c->Update();
    c->SaveAs("results/"+s+".png");

}
