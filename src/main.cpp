#include "../include/PixAnalyze.hpp"

#include <TCanvas.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TApplication.h>
#include <TString.h>
#include <iostream>
#include <fstream>

using namespace std;

# ifndef __CINT__


int main(){

    TApplication app("app", nullptr, nullptr);
    long long nMinScan[2];
    long long nMaxScan[2];
    TH1F** histFPix = new TH1F*[2];
    TH1F** histBPix = new TH1F*[2];
    TH1F** histFB = new TH1F*[2];
    nMinScan[0]= 1530412500;            nMinScan[1] = 1530427080;
    nMaxScan[0] = 1530412800;           nMaxScan[1] = 1530427440;
    double avClusterF, avClusterB, avClusterFB;
    double errClusterF, errClusterB, errClusterFB;
    double avClusterFZoom, errClusterFZoom;
    double avClusterBZoom, errClusterBZoom;
    double timeAvClusterF[2], timeAvClusterB[2], timeAvClusterFB[2];
    double timeErrClusterF[2], timeErrClusterB[2], timeErrClusterFB[2];
    double avSystematicF, avSystematicB;
    double errSystematicF, errSystematicB;
    double avSystematicFB, errSystematicFB;
    bool isSystematics[2];

    PixAnalyze FB("mergedFB.root",0);
    FB.Loop();
/*    PixAnalyze F("mergedFPix.root",0);
    F.Loop();
    PixAnalyze B("mergedBPix.root",0);
    B.Loop();
*/
    int nbin = 10;
//    int nValueF = F.fChain->GetEntriesFast();
//    int nValueB = B.fChain->GetEntriesFast();
    int nValueFB = FB.fChain->GetEntriesFast();
/*
    TH1F* hF = F.nClusterAverageTot("nClusterFPix", "<nCluster>", "number of events", nValueF, "log", nMinScan[0], nMaxScan[0], nMinScan[1], nMaxScan[1], avClusterF, errClusterF);
    TH1F* hB = B.nClusterAverageTot("nClusterBPix", "<nCluster>", "number of events", nValueB, "log", nMinScan[0], nMaxScan[0], nMinScan[1], nMaxScan[1], avClusterB, errClusterB);
    TH1F* hFB = FB.nClusterAverageTot("nClusterFB", "<nCluster>", "number of events", nValueFB, "log", nMinScan[0], nMaxScan[0], nMinScan[1], nMaxScan[1], avClusterFB, errClusterFB);

    TH1F* hFZ = F.nClusterZoom("nClusterFPixZoom", "<nCluster>", "number of events", nValueF, 40, "log", nMinScan[0], nMaxScan[0], nMinScan[1], nMaxScan[1], avClusterFZoom, errClusterFZoom);
    TH1F* hBZ = B.nClusterZoom("nClusterBPixZoom", "<nCluster>", "number of events", nValueB, 40, "log", nMinScan[0], nMaxScan[0], nMinScan[1], nMaxScan[1], avClusterBZoom, errClusterBZoom);
//    TH1F* hFBZ = FB.nClusterZoom("nClusterFBZoom", "nCluster FB", "number of events", nValueFB, 40, "log", nMinScan[0], nMaxScan[0], nMinScan[1], nMaxScan[1]);

    PixAnalyze::nClusterComp("FPix/BPix",hF,hB);
    PixAnalyze::nClusterComp("FPix/BPixZoom",hFZ,hBZ);

    for(int i=0; i<2; i++){
        histFPix[i] = F.timeHisto("F", "time", "<nCluster>", 10, nValueF, nMinScan[i], nMaxScan[i], timeAvClusterF[i], timeErrClusterF[i], true, 0.004);
        histFPix[i] = F.timeHisto("F", "time", "<nCluster>", 10, nValueF, nMinScan[i], nMaxScan[i], timeAvClusterF[i], timeErrClusterF[i], false, 0.004);
        histBPix[i] = B.timeHisto("B", "time", "<nCluster>", 10, nValueB, nMinScan[i], nMaxScan[i], timeAvClusterB[i], timeErrClusterB[i], true, 0.008);
        histBPix[i] = B.timeHisto("B", "time", "<nCluster>", 10, nValueB, nMinScan[i], nMaxScan[i], timeAvClusterB[i], timeErrClusterB[i], false, 0.008);
        histFB[i] = FB.timeHisto("FB", "time", "<nCluster>", 10, nValueFB, nMinScan[i], nMaxScan[i], timeAvClusterFB[i], timeErrClusterFB[i], true, 0.01);
        histFB[i]->Draw();
    }

    F.systematic("systematicF", 10, nValueF, nMinScan[0], nMaxScan[0], nMinScan[1], nMaxScan[1], avSystematicF, errSystematicF);
    B.systematic("systematicB", 10, nValueB, nMinScan[0], nMaxScan[0], nMinScan[1], nMaxScan[1], avSystematicB, errSystematicB);

        cout<<"Systematics"<<endl;
        cout<<"F : "<<avSystematicF<<" +- "<<errSystematicF<<endl;
        cout<<"B : "<<avSystematicB<<" +- "<<errSystematicB<<endl;

        cout<<"Numbers"<<endl;
        cout<<"For F : "<<nValueF<<endl;
        cout<<"     nCluster F : "<<avClusterF<<" +- "<<errClusterF<<endl;
        cout<<"For B : "<<nValueB<<endl;
        cout<<"     nCluster B : "<<avClusterB<<" +- "<<errClusterB<<endl;
        cout<<"For FB : "<<nValueFB<<endl;
        cout<<"     nCluster FB : "<<avClusterFB<<" +- "<<errClusterFB<<endl;

        cout<<"Numbers for Zoom"<<endl;
        cout<<"For F : "<<nValueF<<endl;
        cout<<"     nCluster F Zoom: "<<avClusterFZoom<<" +- "<<errClusterFZoom<<endl;
        cout<<"For B : "<<nValueB<<endl;
        cout<<"     nCluster B Zoom: "<<avClusterBZoom<<" +- "<<errClusterBZoom<<endl;

        cout<<endl<<"for time"<<endl;
        cout<<"For F : "<<nValueF<<endl;
        cout<<"     time 1 : "<<timeAvClusterF[0]<<" +- "<<timeErrClusterF[0]<<endl;
        cout<<"     time 2 : "<<timeAvClusterF[1]<<" +- "<<timeErrClusterF[1]<<endl;
        cout<<"For B : "<<nValueB<<endl;
        cout<<"     time 1 : "<<timeAvClusterB[0]<<" +- "<<timeErrClusterB[0]<<endl;
        cout<<"     time 2 : "<<timeAvClusterB[1]<<" +- "<<timeErrClusterB[1]<<endl;
*/

    FB.systematic("systematicFB", 10, nValueFB, nMinScan[0], nMaxScan[0], nMinScan[1], nMaxScan[1], avSystematicFB, errSystematicFB);
        cout<<"F : "<<avSystematicFB<<" +- "<<errSystematicFB<<endl;


    cout<<endl<<"finish"<<endl;

    app.Run();
    return 0;
}

#endif
