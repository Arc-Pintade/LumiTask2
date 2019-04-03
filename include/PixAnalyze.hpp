//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Dec 18 11:04:18 2018 by ROOT version 6.14/04
// from TTree pccminitree/pcc vdm scan data
// found on file: data/mergedFPIX.root
//////////////////////////////////////////////////////////

#ifndef PixAnalyze_hpp
#define PixAnalyze_hpp

#include <TString.h>
#include <TROOT.h>
#include <TH1F.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class PixAnalyze {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           run;
   Int_t           LS;
   Int_t           LN;
   UInt_t          event;
   UInt_t          timeStamp_begin;
   UInt_t          timeStamp_end;
   Int_t           bunchCrossing;
   Double_t        nCluster;
   Double_t        nClusterPerLayer[5];
   Double_t        nVtx;
   Int_t           vtx_isGood;
   Int_t           vtx_isFake;
   Int_t           vtx_nTrk;
   Int_t           vtx_isValid;
   Double_t        vtx_x;
   Double_t        vtx_y;
   Double_t        vtx_z;
   Double_t        vtx_xError;
   Double_t        vtx_yError;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_LS;   //!
   TBranch        *b_LN;   //!
   TBranch        *b_event;   //!
   TBranch        *b_timeStamp_begin;   //!
   TBranch        *b_timeStamp_end;   //!
   TBranch        *b_bunchCrossing;   //!
   TBranch        *b_nCluster;   //!
   TBranch        *b_nClusterPerLayer;   //!
   TBranch        *b_nVtx;   //!
   TBranch        *b_vtx_isGood;   //!
   TBranch        *b_vtx_isFake;   //!
   TBranch        *b_vtx_nTrk;   //!
   TBranch        *b_vtx_isValid;   //!
   TBranch        *b_vtx_x;   //!
   TBranch        *b_vtx_y;   //!
   TBranch        *b_vtx_z;   //!
   TBranch        *b_vtx_xError;   //!
   TBranch        *b_vtx_yError;   //!

    long long* timeStamp_beginValue;
    long long* timeStamp_endValue;
    double* nClusterValue;

   PixAnalyze(TString rootFile, TTree *tree=0);
   virtual ~PixAnalyze();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

    TH1F* timeHisto(TString s, TString axeX, TString axeY, int div, int n, long long minRes, long long maxRes, double& mean, double& error, bool isSystematics, double syst);
    TH1F* nClusterAverageTot(TString s, TString axeX, TString axeY, int n, TString log, long long min1, long long max1, long long min2, long long max2, double& mean, double& error);
    TH1F* nClusterZoom(TString s, TString axeX, TString axeY, int n, int nbin, TString log, long long min1, long long max1, long long min2, long long max2, double& mean, double& error);
    static void nClusterComp(TString s, TH1F* h1, TH1F* h2);

    void systematic(TString s, int div, int n, long long min1, long long max1, long long min2, long long max2, double& mean, double& error);

};

#endif

#ifdef PixAnalyze_cxx
PixAnalyze::PixAnalyze(TString rootFile, TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("data/"+rootFile);
      if (!f || !f->IsOpen()) {
         f = new TFile("data/"+rootFile);
      }
      f->GetObject("pccminitree",tree);

   }
   Init(tree);
}

PixAnalyze::~PixAnalyze()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t PixAnalyze::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t PixAnalyze::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void PixAnalyze::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("LS", &LS, &b_LS);
   fChain->SetBranchAddress("LN", &LN, &b_LN);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("timeStamp_begin", &timeStamp_begin, &b_timeStamp_begin);
   fChain->SetBranchAddress("timeStamp_end", &timeStamp_end, &b_timeStamp_end);
   fChain->SetBranchAddress("bunchCrossing", &bunchCrossing, &b_bunchCrossing);
   fChain->SetBranchAddress("nCluster", &nCluster, &b_nCluster);
   fChain->SetBranchAddress("nClusterPerLayer", nClusterPerLayer, &b_nClusterPerLayer);
   fChain->SetBranchAddress("nVtx", &nVtx, &b_nVtx);
   fChain->SetBranchAddress("vtx_isGood", &vtx_isGood, &b_vtx_isGood);
   fChain->SetBranchAddress("vtx_isFake", &vtx_isFake, &b_vtx_isFake);
   fChain->SetBranchAddress("vtx_nTrk", &vtx_nTrk, &b_vtx_nTrk);
   fChain->SetBranchAddress("vtx_isValid", &vtx_isValid, &b_vtx_isValid);
   fChain->SetBranchAddress("vtx_x", &vtx_x, &b_vtx_x);
   fChain->SetBranchAddress("vtx_y", &vtx_y, &b_vtx_y);
   fChain->SetBranchAddress("vtx_z", &vtx_z, &b_vtx_z);
   fChain->SetBranchAddress("vtx_xError", &vtx_xError, &b_vtx_xError);
   fChain->SetBranchAddress("vtx_yError", &vtx_yError, &b_vtx_yError);
   Notify();
}

Bool_t PixAnalyze::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void PixAnalyze::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t PixAnalyze::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef PixAnalyze_cxx
