//#include "CMS_lumi.C"
#include <TFile.h>
#include <TDirectoryFile.h>
#include "TH1D.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "setTDRStyle_teliko.C"
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

double error1(double n1,double n2);
double error2(double n1,double n2);
void Calculate_bkg_rejection()
{

	const int eta_bins = 8;

//	gROOT->LoadMacro("setTDRStyle_teliko.C");
	setTDRStyle_teliko();

	TH1D *h_pt_noise[eta_bins];    
	TH1D *h_pt_noise_with[eta_bins];

	//=======Minimum Bias Sample =====

//	TFile *f = TFile::Open("root://eoscms//eos/cms/store/group/phys_jetmet/dimitris/UL2017_MinBias/Puppi_RunD/MinimumBias__Run2017D-09Aug2019_reduced_skim.root","READ");
	TFile *f = TFile::Open("UL2018/MinBias_ak4Puppi/RunC/UL18_RunC_ULJetID.root","READ");
//	TFile *f = TFile::Open("UL2016/PUPPI/nonAPV/testing_35/RunGH_nemf0p6.root","READ");

	assert(f && !f->IsZombie());
	 
	char pt_histo_name[50];
	char eta_histo_name[50];
	
	h_pt_noise[0] = (TH1D*)f->Get("h_pT_0"); 
	h_pt_noise[1] = (TH1D*)f->Get("h_pT_1"); 
	h_pt_noise[2] = (TH1D*)f->Get("h_pT_2"); 
	h_pt_noise[3] = (TH1D*)f->Get("h_pT_3"); 
	h_pt_noise[4] = (TH1D*)f->Get("h_pT_4"); 
	h_pt_noise[5] = (TH1D*)f->Get("h_pT_5"); 
	h_pt_noise[6] = (TH1D*)f->Get("h_pT_6"); 
	h_pt_noise[7] = (TH1D*)f->Get("h_pT_7"); 

	h_pt_noise_with[0] = (TH1D*)f->Get("h_pT_with_0"); 
	h_pt_noise_with[1] = (TH1D*)f->Get("h_pT_with_1"); 
	h_pt_noise_with[2] = (TH1D*)f->Get("h_pT_with_2"); 
	h_pt_noise_with[3] = (TH1D*)f->Get("h_pT_with_3"); 
	h_pt_noise_with[4] = (TH1D*)f->Get("h_pT_with_4"); 
	h_pt_noise_with[5] = (TH1D*)f->Get("h_pT_with_5"); 
	h_pt_noise_with[6] = (TH1D*)f->Get("h_pT_with_6"); 
	h_pt_noise_with[7] = (TH1D*)f->Get("h_pT_with_7"); 


	 
//	char eta_bins_legend[eta_bins][25] = { "|#eta|<0.5", "0.5 #leq |#eta| < 1.0", "1.0 #leq |#eta| < 1.5", "1.5 #leq |#eta| < 2.0", "2.0 #leq |#eta| < 2.6","2.6 #leq |#eta| < 2.7", "2.7 #leq |#eta| < 3.0" , "3.0 #leq |#eta| < 5.0" };

	float yBnd[eta_bins+1]={0.0, 0.5, 1.0, 1.5, 2.0, 2.4, 2.7, 3.0, 5.0};

	for ( int j=0; j<eta_bins; j++) 
	{		
		TH1D *h_pt_noise_all = (TH1D*) h_pt_noise[j]->Clone();
		TH1D *h_pt_noise_with_all = (TH1D*) h_pt_noise_with[j]->Clone();
		cout << "\n\n For " <<  yBnd[j]  <<" <= |#eta| < " << yBnd[j+1] << ": "<< 100*( 1 - h_pt_noise_with_all->Integral()/h_pt_noise_all->Integral() )<<" % "  << endl;
	}


}

	//=====================================================================
	//The error bars are determined from Wilson interval of binomial errors. 
	//define upper error
	double error1 (double n1, double n2)
	{
		if (n1 == 0)
		{
			return (0);
		}
		else
		{
			return ( (n2/n1 + 0.5/n1 + sqrt(n2/pow(n1,2)*(1-n2/n1) + 0.25/pow(n1,2)))/(1+1.0/n1) - n2/n1 );
		}
	}
	//define lower error
	double error2 (double n1, double n2)
	{
		if (n1 == 0)
		{
			return (0);
		}
		else
		{
			return ( n2/n1 - (n2/n1 + 0.5/n1 - sqrt(n2/pow(n1,2)*(1-n2/n1) + 0.25/pow(n1,2)))/(1+1.0/n1) );
		}
	}


