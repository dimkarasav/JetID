#include "TRandom.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TTree.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TStyle.h"
#include "TChain.h"
#include <iostream>
#include <fstream>
#include "TSystem.h"
#include "TROOT.h"
#include "TMath.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "setTDRStyle_teliko.C"
#include "TSystemFile.h" 
#include "TSystemDirectory.h"
#include "TLatex.h"
#include "TSpline.h"
#include <algorithm>
#include <string>
#include <stdarg.h>
#define N_histo_bins 300
//#define eta_bins 8







void Plot_btb_and_noise()
{


	
	TFile *fnoise = TFile::Open("UL2017/MinBias/RunD/MinBias_nJet_asymmetry_n1remove.root","READ");		
//	TFile *fbtb = TFile::Open("UL2017/JetHT/RunD/JetHT_nJet_asymmetry.root","READ");	
	TFile *fbtb = TFile::Open("UL2017/JetHT/RunD/JetHT_nJet_asymmetry_n1remove.root","READ");			


	char output_directory[500] = {"UL2017/nJet_asymmetry_comparisons/"};     
	char image_name[500] = {"btb_vs_noise_nJet_asymmetry_pTgr100"}; 


	double yBnd[] = {-5.0, -3.0, -2.7, 0.0, 2.7, 3.0, 5.0};    
	double yBndFabs[] = {0.0, 2.7, 3.0, 5.0};



	const int eta_bins = sizeof(yBnd)/sizeof(yBnd[0])-1;
	const int eta_binsFabs = sizeof(yBndFabs)/sizeof(yBndFabs[0])-1;


	bool isCHS = false;
	bool isUL = true;
	bool Save_Plots = false;
	bool useAbsoluteEta = false;
	double pTmaxCut = 50000;
	double pTminCut = 100;
	char *legend_1 = "Back-to-Back";
	char *legend_2 = "Minimum Bias";





	char *jet_type;
	if (isCHS) jet_type = "CHS";
    else jet_type = "PUPPI";

	gROOT->LoadMacro("setTDRStyle_teliko.C");
	setTDRStyle_teliko();
	TPaveText *paveCMS = new TPaveText(0.15,0.95,0.5,1.0,"NDC");
	// paveCMS->AddText("CMS Preliminary L=9.2 fb^{-1} #sqrt{s} = 13 TeV");
	paveCMS->AddText("CMS Preliminary #sqrt{s} = 13 TeV");
	paveCMS->SetFillColor(0);
	paveCMS->SetBorderSize(0);
	paveCMS->SetTextSize(0.03);

	//float lumi = 2500.0;
	char name[100]; 
	float etamin=0, etamax=0;
	char filename[512]; 
	int era = 2018; 



	TH1D  *h_nJets[eta_bins], *h_nJets_asymmetry[eta_binsFabs];
	TH2D *h_NM_nJets[eta_bins];

	TH1D  *h_nJets_noise[eta_bins], *h_nJets_asymmetry_noise[eta_binsFabs];
	TH2D *h_NM_nJets_noise[eta_bins];



	

	for(Int_t iy=0; iy<eta_bins;iy++)
	{ 

		//========== JetHT ===========

		sprintf(name,"h_nJets_%i", iy);
		h_nJets[iy] = (TH1D*)fbtb->Get(name); 			

		sprintf(name,"h_NM_nJets_%i", iy);
		h_NM_nJets[iy] = (TH2D*)fbtb->Get(name); 	

		if( iy < eta_binsFabs )
		{
			sprintf(name,"h_nJets_asymmetry_%i",iy);
			h_nJets_asymmetry[iy] = (TH1D*)fbtb->Get(name); 		
		}
		//========== noise ===========

		sprintf(name,"h_nJets_%i" ,iy);
		h_nJets_noise[iy] = (TH1D*)fnoise->Get(name); 		

		sprintf(name,"h_NM_nJets_%i", iy);
		h_NM_nJets_noise[iy] = (TH2D*)fnoise->Get(name); 	

		if( iy < eta_binsFabs )
		{
			sprintf(name,"h_nJets_asymmetry_%i",iy);
			h_nJets_asymmetry_noise[iy] = (TH1D*)fnoise->Get(name); 		

		}
	}


	TCanvas *pad_nJets[eta_bins], *pad_NM_nJets[eta_bins], *pad_NM_nJets_noise[eta_bins], *pad_nJets_asym[eta_binsFabs];

	for(Int_t iy=0; iy<eta_bins;iy++)
	{
		sprintf(name,"pad_nJets_%i" ,iy);
		pad_nJets[iy] = new TCanvas(name, name,1);
		sprintf(name,"pad_NM_nJets_%i" ,iy);
		pad_NM_nJets[iy] = new TCanvas(name, name,1);

		sprintf(name,"pad_NM_nJets_noise_%i" ,iy);
		pad_NM_nJets_noise[iy] = new TCanvas(name, name,1);

		if( iy < eta_binsFabs ) sprintf(name,"pad_nJets_asym_%i" ,iy);
		if( iy < eta_binsFabs ) pad_nJets_asym[iy] = new TCanvas(name, name,1);


	}

	

	TLegend *leg1 =new TLegend(.7, .7, .9, .9);//7899//4899
	leg1->SetTextSize(0.035);
	leg1->SetFillColor(0); 
	leg1->SetBorderSize(0);
	leg1->SetHeader(jet_type); 
	leg1->AddEntry(h_nJets[0], "Back-to-back", "L");
	leg1->AddEntry(h_nJets_noise[0], "MinBias", "L");

	TLatex *xLab = new TLatex();
	xLab->SetTextAlign(2);
	xLab->SetTextFont(42);
	xLab->SetTextSize(0.05);
	xLab->SetNDC();


	for(int iy=0; iy<eta_bins; iy++)
	{
 
		etamin = yBnd[iy];
		etamax = yBnd[iy+1];
		char seta[100];
		sprintf(seta, "%3.1f #leq y < %3.1f",yBnd[iy],yBnd[iy+1]);
	//	const char *seta = (etamin==0 ? Form("%1.2g #leq |y| < %1.2g",etamax) :
	//	Form("%1.2g #leq |y| < %1.2g",etamin,etamax));
		TLatex *teta = new TLatex(0.68,0.96,seta); cout<<seta<<endl;
		teta->SetNDC();
		teta->SetTextSize(0.05);


		pad_nJets[iy]->cd();
		pad_nJets[iy]->cd()->SetLogy(1);
		h_nJets[iy]->GetXaxis()->SetTitle("nJets per Event") ;
		h_nJets[iy]->GetYaxis()->SetTitle("Entries");
		h_nJets[iy]->GetYaxis()->SetTitleOffset(1.3);
		h_nJets[iy]->GetXaxis()->SetRangeUser(0.5,100);
		h_nJets[iy]->SetLineColor(1);
		h_nJets[iy]->SetLineWidth(2);
		h_nJets_noise[iy]->SetLineColor(2);
		h_nJets_noise[iy]->SetLineWidth(2);
		h_nJets[iy]->SetLineStyle(1);
		h_nJets_noise[iy]->SetLineStyle(1); 
		h_nJets[iy]->SetMinimum(0.5); 
		h_nJets[iy]->SetMaximum(10000000.);
		h_nJets[iy]->Draw();
		h_nJets_noise[iy]->Draw("same hist");
		paveCMS ->Draw("same");
		leg1->Draw("same");
		teta->Draw();


		pad_NM_nJets[iy]->cd();
		pad_NM_nJets[iy]->cd()->SetLogz(1);

		h_NM_nJets[iy]->SetStats(true);
    	pad_NM_nJets[iy]->cd()->SetRightMargin(0.18);
   		pad_NM_nJets[iy]->cd()->SetTopMargin(0.07);
		h_NM_nJets[iy]->SetTitle("JetHT: NM-nJets");
		h_NM_nJets[iy]->GetYaxis()->SetTitleOffset(1.3);
		h_NM_nJets[iy]->GetXaxis()->SetLabelSize(0.04);
		h_NM_nJets[iy]->GetXaxis()->SetTitle("#bar{Neutral Multiplicity}");
		h_NM_nJets[iy]->GetYaxis()->SetTitle("nJets per Event");
		h_NM_nJets[iy]->GetYaxis()->SetRangeUser(0., 50.);
		h_NM_nJets[iy]->GetXaxis()->SetRangeUser(0., 30.);
		h_NM_nJets[iy]->SetMinimum(0.9);
		h_NM_nJets[iy]->SetMaximum();
		h_NM_nJets[iy]->Draw("colz");
		xLab->DrawLatex(0.15, 0.96, "JetHT: back-to-back");
		teta->Draw();


		pad_NM_nJets_noise[iy]->cd();
		pad_NM_nJets_noise[iy]->cd()->SetLogz(1);

		h_NM_nJets_noise[iy]->SetStats(true);
    	pad_NM_nJets_noise[iy]->cd()->SetRightMargin(0.18);
   		pad_NM_nJets_noise[iy]->cd()->SetTopMargin(0.07);
		h_NM_nJets_noise[iy]->GetYaxis()->SetTitleOffset(1.3);
		h_NM_nJets_noise[iy]->SetTitle("JetHT: NM-nJets");
		h_NM_nJets_noise[iy]->GetXaxis()->SetLabelSize(0.04);
		h_NM_nJets_noise[iy]->GetXaxis()->SetTitle("#bar{Neutral Multiplicity}");
		h_NM_nJets_noise[iy]->GetYaxis()->SetTitle("nJets per Event");
		h_NM_nJets_noise[iy]->GetYaxis()->SetRangeUser(0., 70.);
		h_NM_nJets_noise[iy]->GetXaxis()->SetRangeUser(0., 30.);
		h_NM_nJets_noise[iy]->SetMinimum(0.9);
		h_NM_nJets_noise[iy]->SetMaximum();
		h_NM_nJets_noise[iy]->Draw("colz");
		teta->Draw();
		xLab->DrawLatex(0.15, 0.96, "MinBias: noise events");





		if( iy< eta_binsFabs )
		{

			char setaFabs[100];
			sprintf(setaFabs, "%3.1f #leq |y| < %3.1f",yBndFabs[iy],yBndFabs[iy+1]);
		//	const char *seta = (etamin==0 ? Form("%1.2g #leq |y| < %1.2g",etamax) :
		//	Form("%1.2g #leq |y| < %1.2g",etamin,etamax));
			TLatex *tetaFabs = new TLatex(0.68,0.96,setaFabs); cout<<setaFabs<<endl;
			tetaFabs->SetNDC();
			tetaFabs->SetTextSize(0.05);

	
			pad_nJets_asym[iy]->cd();
			pad_nJets_asym[iy]->cd()->SetLogy(1);
			h_nJets_asymmetry[iy]->GetXaxis()->SetTitle("nJets asymmetry") ;
			h_nJets_asymmetry[iy]->GetYaxis()->SetTitle("Entries");
			h_nJets_asymmetry[iy]->GetYaxis()->SetTitleOffset(1.3);
			h_nJets_asymmetry[iy]->GetXaxis()->SetRangeUser(-1.2,1.2);
			h_nJets_asymmetry[iy]->SetLineColor(1);
			h_nJets_asymmetry[iy]->SetLineWidth(2);
			h_nJets_asymmetry_noise[iy]->SetLineColor(2);
			h_nJets_asymmetry_noise[iy]->SetLineWidth(2);
			h_nJets_asymmetry[iy]->SetLineStyle(1);
			h_nJets_asymmetry_noise[iy]->SetLineStyle(1); 
			h_nJets_asymmetry[iy]->SetMinimum(0.5); 
			h_nJets_asymmetry[iy]->SetMaximum(100000000.);
			h_nJets_asymmetry[iy]->Draw();
			h_nJets_asymmetry_noise[iy]->Draw("same hist");
			paveCMS ->Draw("same");
			leg1->Draw("same");
			tetaFabs->Draw();
		}

	} //end of eta regions




	if(Save_Plots)
	{
		for(int iy=0; iy<eta_bins; iy++)
		{

			sprintf(filename,"%s/%s_h_nJets_%i.png",output_directory,image_name,iy);
			pad_nJets[iy]->SaveAs(filename);

			sprintf(filename,"%s/%s_h_NM_nJets_%i.png",output_directory,image_name,iy);
			pad_NM_nJets[iy]->SaveAs(filename);

			sprintf(filename,"%s/%s_h_NM_nJets_noise_%i.png",output_directory,image_name,iy);
			pad_NM_nJets_noise[iy]->SaveAs(filename);
	
			if( iy< eta_binsFabs )	sprintf(filename,"%s/%s_h_nJets_asym_%i.png",output_directory,image_name,iy);
			if( iy< eta_binsFabs )	pad_nJets_asym[iy]->SaveAs(filename);

		}
		

	}
	

}

