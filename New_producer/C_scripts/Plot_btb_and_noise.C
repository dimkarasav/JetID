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


	



//	TFile *f = TFile::Open("root://eoscms//eos/cms/store/user/dkarasav/ULJetID_JMAR_followUps/MinBiasPt0_100/MinimumBias__Run2017D-09Aug2019_reduced_skim.root","READ");
	TFile *fnoise = TFile::Open("UL2017/MinBias/RunD/MinimumBias__Run2017D-09Aug2019_reduced_tree_histos_HF_PlusMinusEta.root","READ");		

	TFile *fbtb = TFile::Open("UL2017/JetHT/RunD/Run2017D-09Aug2019_UL2017-v1__MINIAOD_HF_PlusMinusEta_samepTcuts.root","READ");		


	char output_directory[500] = {"UL2017/MinBias/RunD/"};     
	char image_name[500] = {"btb_vs_noise_allRuns"}; 


	double yBnd[] = {-5.0, -3.0, -2.7, 0.0, 2.7, 3.0, 5.0};    
	const int eta_bins = sizeof(yBnd)/sizeof(yBnd[0])-1;
	bool isCHS = false;
	bool isUL = true;
	bool Save_Plots = true;
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
	char filename[256]; 
	int era = 2018; 




	TH1D *h_MetSig[2][eta_bins], *h_CHFJet1[2][eta_bins], *h_NHFJet1[2][eta_bins], *h_CEMFJet1[2][eta_bins], *h_NEMFJet1[2][eta_bins], *h_MUFJet1[2][eta_bins], *h_TOTCONSTJet1[2][eta_bins], *h_CMJet1[2][eta_bins], *h_NMJet1[2][eta_bins], *h_ptJet1[2][eta_bins],*h_PMJet1[2][eta_bins],*h_ETAJet1[2], *h_PHIJet1[2][eta_bins], *h_nJets[2][eta_bins];


	TH1D *h_MetSig_noise[2][eta_bins], *h_CHF_noise[2][eta_bins], *h_NHF_noise[2][eta_bins], *h_CEMF_noise[2][eta_bins], *h_NEMF_noise[2][eta_bins], *h_MUF_noise[2][eta_bins], *h_ChM_noise[2][eta_bins], *h_NeutM_noise[2][eta_bins], *h_pt_noise[2][eta_bins],*h_PhM_noise[2][eta_bins], *h_ETA_noise[2], *h_PHI_noise[2][eta_bins], *h_nJets_noise[2][eta_bins];



	char with_c[2][40]={ "", "_with" };
	

	for (int jetID=0; jetID<2; jetID++)
	{
	
		sprintf(name,"h_ETA%s",with_c[jetID]);
		h_ETAJet1[jetID] = (TH1D*)fbtb->Get(name); 

		sprintf(name,"h_ETA%s",with_c[jetID]);
		h_ETA_noise[jetID] = (TH1D*)fnoise->Get(name); 

		for(Int_t iy=0; iy<eta_bins;iy++)
		{ 

			if(jetID==0) sprintf(name,"h_pT_%i" ,iy);
			if(jetID==1) sprintf(name,"h_pT%s_%i",with_c[jetID] ,iy);
			cout << name<< endl;
			h_ptJet1[jetID][iy] = (TH1D*)fbtb->Get(name); 
//			cout << " jetID = " << jetID << "  integral =  " << h_ptJet1[jetID][iy]->Integral() << endl;;

			sprintf(name,"h_nJets%s_%i",with_c[jetID] ,iy);
			h_nJets[jetID][iy] = (TH1D*)fbtb->Get(name); 

			cout << "\n jetID = " << jetID << "  integral =  " << h_nJets[jetID][iy]->Integral() << endl;;
			 
			sprintf(name,"h_MetSig%s_%i",with_c[jetID] ,iy);
			h_MetSig[jetID][iy] = (TH1D*)fbtb->Get(name); 
			 
			sprintf(name,"h_CHF%s_%i",with_c[jetID] ,iy);
			h_CHFJet1[jetID][iy] = (TH1D*)fbtb->Get(name); 
		 
			sprintf(name,"h_NHF%s_%i",with_c[jetID] ,iy);
			h_NHFJet1[jetID][iy] = (TH1D*)fbtb->Get(name); 
				 
			sprintf(name,"h_CEMF%s_%i",with_c[jetID] ,iy);
			h_CEMFJet1[jetID][iy] = (TH1D*)fbtb->Get(name); 
			 
			sprintf(name,"h_NEMF%s_%i",with_c[jetID] ,iy);
			h_NEMFJet1[jetID][iy] = (TH1D*)fbtb->Get(name); 

			sprintf(name,"h_MUF%s_%i",with_c[jetID] ,iy);
			h_MUFJet1[jetID][iy] = (TH1D*)fbtb->Get(name); 

			sprintf(name,"h_ChM%s_%i",with_c[jetID] ,iy);
			h_CMJet1[jetID][iy] = (TH1D*)fbtb->Get(name); 

			sprintf(name,"h_NeutM%s_%i",with_c[jetID] ,iy);
			h_NMJet1[jetID][iy] = (TH1D*)fbtb->Get(name); 

			sprintf(name,"h_PhM%s_%i",with_c[jetID] ,iy);
			h_PMJet1[jetID][iy] = (TH1D*)fbtb->Get(name); 

			sprintf(name,"h_PHI%s_%i",with_c[jetID] ,iy);
			h_PHIJet1[jetID][iy] = (TH1D*)fbtb->Get(name); 
			


		//========== noise ===========
			sprintf(name,"h_pT%s_%i",with_c[jetID] ,iy);
			h_pt_noise[jetID][iy] = (TH1D*)fnoise->Get(name); 

			sprintf(name,"h_nJets%s_%i",with_c[jetID] ,iy);
			h_nJets_noise[jetID][iy] = (TH1D*)fnoise->Get(name); 
			cout << "\n jetID = " << jetID << "noise   integral =  " << h_nJets_noise[jetID][iy]->Integral() << endl;;

			 
			sprintf(name,"h_MetSig%s_%i",with_c[jetID] ,iy);
			h_MetSig_noise[jetID][iy] = (TH1D*)fnoise->Get(name); 
			 
			sprintf(name,"h_CHF%s_%i",with_c[jetID] ,iy);
			h_CHF_noise[jetID][iy] = (TH1D*)fnoise->Get(name); 
		 
			sprintf(name,"h_NHF%s_%i",with_c[jetID] ,iy);
			h_NHF_noise[jetID][iy] = (TH1D*)fnoise->Get(name); 
				 
			sprintf(name,"h_CEMF%s_%i",with_c[jetID] ,iy);
			h_CEMF_noise[jetID][iy] = (TH1D*)fnoise->Get(name); 
			 
			sprintf(name,"h_NEMF%s_%i",with_c[jetID] ,iy);
			h_NEMF_noise[jetID][iy] = (TH1D*)fnoise->Get(name); 

			sprintf(name,"h_MUF%s_%i",with_c[jetID] ,iy);
			h_MUF_noise[jetID][iy] = (TH1D*)fnoise->Get(name); 

			sprintf(name,"h_ChM%s_%i",with_c[jetID] ,iy);
			h_ChM_noise[jetID][iy] = (TH1D*)fnoise->Get(name); 

			sprintf(name,"h_NeutM%s_%i",with_c[jetID] ,iy);
			h_NeutM_noise[jetID][iy] = (TH1D*)fnoise->Get(name); 

			sprintf(name,"h_PhM%s_%i",with_c[jetID] ,iy);
			h_PhM_noise[jetID][iy] = (TH1D*)fnoise->Get(name); 


			sprintf(name,"h_PHI%s_%i",with_c[jetID] ,iy);
			h_PHI_noise[jetID][iy] = (TH1D*)fnoise->Get(name); 

		}
	}
 


	TCanvas *pad1 = new TCanvas("pad1", "CHF",1);
	pad1->Divide(3,3);
	TCanvas *pad2 = new TCanvas("pad2", "NHF",1);
	pad2->Divide(3,3);
	TCanvas *pad3 = new TCanvas("pad3", "CEMF",1);
	pad3->Divide(3,3);
	TCanvas *pad4 = new TCanvas("pad4", "NEMF",1);
	pad4->Divide(3,3);
	TCanvas *pad5 = new TCanvas("pad5", "MUF",1);
	pad5->Divide(3,3);
	TCanvas *pad9 = new TCanvas("pad9", "pT",1);
	pad9->Divide(3,3);
	TCanvas *pad10= new TCanvas("pad10", "MetovSumEt",1);
	pad10->Divide(3,3);
	TCanvas *pad11= new TCanvas("pad11", "NM",1);
	pad11 ->Divide(3,3);
	TCanvas *pad12= new TCanvas("pad12", "CM",1);
	pad12 ->Divide(3,3);
	TCanvas *pad13= new TCanvas("pad13", "PM",1);
	pad13 ->Divide(3,3);

	TCanvas *pad14= new TCanvas("pad14", "Eta",1);

	TCanvas *pad15= new TCanvas("pad15", "nJet",1);
	pad15 ->Divide(3,3);
	
	TCanvas *pad16= new TCanvas("pad16", "phi",1);
	pad16 ->Divide(3,3);
	//TCanvas *pad17= new TCanvas("pad17", "",1);
	//pad17 ->Divide(3,3); 
	

	TLegend *leg1 =new TLegend(.2, .6, .9, .9);//7899//4899
	leg1->SetTextSize(0.06);
	leg1->SetFillColor(0); 
	leg1->SetBorderSize(0);
	leg1->SetHeader(jet_type); 
	leg1->AddEntry(h_CHFJet1[0][0], "Back-to-back", "L");
	leg1->AddEntry(h_CHFJet1[1][0], "Back-to-back JetID", "L");
	leg1->AddEntry(h_CHF_noise[0][0], "MinBias", "L");
	leg1->AddEntry(h_CHF_noise[1][0], "MinBias JetID", "L");

	for(int iy=0; iy<eta_bins; iy++)
	{
 
		etamin = yBnd[iy];
		etamax = yBnd[iy+1];
		char seta[100];
		sprintf(seta, "%3.1f #leq |y| < %3.1f",yBnd[iy],yBnd[iy+1]);
	//	const char *seta = (etamin==0 ? Form("%1.2g #leq |y| < %1.2g",etamax) :
	//	Form("%1.2g #leq |y| < %1.2g",etamin,etamax));
		TLatex *teta = new TLatex(0.68,0.96,seta); cout<<seta<<endl;
		teta->SetNDC();
		teta->SetTextSize(0.05);

		pad1->cd(iy+1);
		pad1->cd(iy+1)->SetLogy(1);
		h_CHFJet1[0][iy]->GetXaxis()->SetTitle("Charged Hadron Fraction");
		h_CHFJet1[0][iy]->GetYaxis()->SetTitle("Entries");
		h_CHFJet1[0][iy]->GetYaxis()->SetTitleOffset(1.3);
		h_CHFJet1[1][iy]->SetLineColor(4);
		h_CHFJet1[0][iy]->SetLineWidth(2);
		h_CHFJet1[1][iy]->SetLineWidth(2);
		h_CHF_noise[0][iy]->SetLineColor(2);
		h_CHF_noise[1][iy]->SetLineColor(6);
		h_CHF_noise[0][iy]->SetLineWidth(2);
		h_CHF_noise[1][iy]->SetLineWidth(2);
		h_CHFJet1[0][iy]->SetLineStyle(1);
		h_CHF_noise[0][iy]->SetLineStyle(1);
		h_CHFJet1[0][iy]->SetMinimum(0.1);
		h_CHFJet1[0][iy]->SetMaximum(1000000000);
		h_CHFJet1[0][iy]->Draw("");

		//h_CHF_noise[0][iy]->Scale( h_CHFJet1[0][iy]->Integral() /h_CHF_noise[0][iy]->Integral() );
		//h_CHF_noise[1][iy]->Scale( h_CHFJet1[0][iy]->Integral() /h_CHF_noise[1][iy]->Integral() );
		h_CHFJet1[1][iy]->Draw("same hist");
		h_CHF_noise[0][iy]->Draw("same hist");
		h_CHF_noise[1][iy]->Draw("same hist");

		paveCMS ->Draw("same");
		teta->Draw();

		//leg1->Draw();

		pad2->cd(iy+1);
		pad2->cd(iy+1)->SetLogy(1);
		h_NHFJet1[0][iy]->GetXaxis()->SetTitle("Neutral Hadron Fraction");
		h_NHFJet1[0][iy]->GetYaxis()->SetTitle("Entries");
		h_NHFJet1[0][iy]->GetYaxis()->SetTitleOffset(1.3);
		h_NHFJet1[0][iy]->SetLineColor(1);
		h_NHFJet1[1][iy]->SetLineColor(4);
		h_NHFJet1[0][iy]->SetLineWidth(2);
		h_NHFJet1[1][iy]->SetLineWidth(2);
		// h_NHF_noise[0][iy]->Scale( h_NHFJet1[0][iy]->Integral() /h_NHF_noise[0][iy]->Integral() );
		//h_NHF_noise[1][iy]->Scale( h_NHFJet1[0][iy]->Integral() /h_NHF_noise[1][iy]->Integral() );
		h_NHF_noise[0][iy]->SetLineColor(2);
		h_NHF_noise[1][iy]->SetLineColor(6);
		h_NHF_noise[0][iy]->SetLineWidth(2);
		h_NHF_noise[1][iy]->SetLineWidth(2);
		h_NHFJet1[0][iy]->SetLineStyle(1);
		h_NHF_noise[0][iy]->SetLineStyle(1);
		if(iy>4)h_NHFJet1[0][iy]->SetMinimum(0.1);
		h_NHFJet1[0][iy]->SetMaximum(1000000000);
		h_NHFJet1[0][iy]->Draw();
		h_NHFJet1[1][iy]->Draw("same hist");
		h_NHF_noise[0][iy]->Draw("same hist");
		h_NHF_noise[1][iy]->Draw("same hist");
		paveCMS ->Draw("same");
		teta->Draw();
		 //leg1->Draw(); 

		pad3->cd(iy+1);
		pad3->cd(iy+1)->SetLogy(1);
		h_CEMFJet1[0][iy]->GetXaxis()->SetTitle("Charged Electromagnetic Fraction");
		h_CEMFJet1[0][iy]->GetYaxis()->SetTitle("Entries");
		h_CEMFJet1[0][iy]->GetYaxis()->SetTitleOffset(1.3);
		h_CEMFJet1[0][iy]->SetLineColor(1);
		h_CEMFJet1[1][iy]->SetLineColor(4);
		h_CEMFJet1[0][iy]->SetLineWidth(2);
		h_CEMFJet1[1][iy]->SetLineWidth(2);
		// h_CEMF_noise[0][iy]->Scale( h_CEMFJet1[0][iy]->Integral() /h_CEMF_noise[0][iy]->Integral() );
		// h_CEMF_noise[1][iy]->Scale( h_CEMFJet1[0][iy]->Integral() /h_CEMF_noise[1][iy]->Integral() );
		h_CEMF_noise[0][iy]->SetLineColor(2);
		h_CEMF_noise[1][iy]->SetLineColor(6);
		h_CEMF_noise[0][iy]->SetLineWidth(2);
		h_CEMF_noise[1][iy]->SetLineWidth(2);
		h_CEMFJet1[0][iy]->SetLineStyle(1);
		h_CEMF_noise[0][iy]->SetLineStyle(1);
		h_CEMFJet1[0][iy]->SetMinimum(0.1);
		h_CEMFJet1[0][iy]->SetMaximum(100000000);
		h_CEMFJet1[0][iy]->Draw();
		h_CEMFJet1[1][iy]->Draw("same hist");
		h_CEMF_noise[0][iy]->Draw("same hist");
		h_CEMF_noise[1][iy]->Draw("same hist");
		paveCMS ->Draw("same");
		teta->Draw();
		//leg1->Draw();

		pad4->cd(iy+1);
		pad4->cd(iy+1)->SetLogy(1);
		h_NEMFJet1[0][iy]->GetXaxis()->SetTitle("Neutral Electromagnetic Fraction");
		h_NEMFJet1[0][iy]->GetYaxis()->SetTitle("Entries");
		h_NEMFJet1[0][iy]->GetYaxis()->SetTitleOffset(1.3);
		h_NEMFJet1[0][iy]->SetLineColor(1);
		h_NEMFJet1[1][iy]->SetLineColor(4);
		h_NEMFJet1[0][iy]->SetLineWidth(2);
		h_NEMFJet1[1][iy]->SetLineWidth(2);
		// h_NEMF_noise[0][iy]->Scale( h_NEMFJet1[0][iy]->Integral() /h_NEMF_noise[0][iy]->Integral() );
		// h_NEMF_noise[1][iy]->Scale( h_NEMFJet1[0][iy]->Integral() /h_NEMF_noise[1][iy]->Integral() );
		h_NEMF_noise[0][iy]->SetLineColor(2);
		h_NEMF_noise[1][iy]->SetLineColor(6);
		h_NEMF_noise[0][iy]->SetLineWidth(2);
		h_NEMF_noise[1][iy]->SetLineWidth(2);
		h_NEMFJet1[0][iy]->SetLineStyle(1);
		h_NEMF_noise[0][iy]->SetLineStyle(1);
		h_NEMFJet1[0][iy]->SetMinimum(0.1);
		h_NEMFJet1[0][iy]->SetMaximum(1000000000.);
		h_NEMFJet1[0][iy]->Draw();
		h_NEMFJet1[1][iy]->Draw("same hist");
		h_NEMF_noise[0][iy]->Draw("same hist");
		h_NEMF_noise[1][iy]->Draw("same hist");
		paveCMS ->Draw("same");
		teta->Draw();
		//leg1->Draw();
		pad5->cd(iy+1);
		pad5->cd(iy+1)->SetLogy(1);
		h_MUFJet1[0][iy]->GetXaxis()->SetTitle("Muon Fraction");
		h_MUFJet1[0][iy]->GetYaxis()->SetTitle("Entries");
		h_MUFJet1[0][iy]->GetYaxis()->SetTitleOffset(1.3);
		h_MUFJet1[0][iy]->SetLineColor(1);
		h_MUFJet1[1][iy]->SetLineColor(4);
		h_MUFJet1[0][iy]->SetLineWidth(2);
		h_MUFJet1[1][iy]->SetLineWidth(2);
		// h_MUF_noise[0][iy]->Scale( h_MUFJet1[0][iy]->Integral() /h_MUF_noise[0][iy]->Integral() );
		// h_MUF_noise[1][iy]->Scale( h_MUFJet1[0][iy]->Integral() /h_MUF_noise[1][iy]->Integral() );
		h_MUF_noise[0][iy]->SetLineColor(2);
		h_MUF_noise[1][iy]->SetLineColor(6);
		h_MUF_noise[0][iy]->SetLineWidth(2);
		h_MUF_noise[1][iy]->SetLineWidth(2); 
		h_MUFJet1[0][iy]->SetLineStyle(1);
		h_MUF_noise[0][iy]->SetLineStyle(1);
		h_MUFJet1[0][iy]->SetMaximum(100000000);
		if(iy>4)h_MUFJet1[0][iy]->SetMinimum(10);
		h_MUFJet1[0][iy]->Draw();
		h_MUFJet1[1][iy]->Draw("same hist");
		h_MUF_noise[0][iy]->Draw("same hist");
		h_MUF_noise[1][iy]->Draw("same hist");
		paveCMS ->Draw("same");
		teta->Draw();
		TLegend *leg1a =new TLegend(.4, .6, .9, .9);//7899//4899
		leg1a->SetTextSize(0.06);
		leg1a->SetFillColor(0); 
		leg1a->SetBorderSize(0);
		//leg1a->SetHeader("ReReco");
		leg1a->AddEntry(h_CHFJet1[0][0], "Back-to-back", "L");
		//leg1a->AddEntry(h_CHFJet1[1][0], "Back-to-back JetID", "L");
		leg1a->AddEntry(h_CHF_noise[0][0], "MinBias", "L");
		//leg1a->AddEntry(h_CHF_noise[1][0], "MinBias JetID", "L");
		//leg1a->Draw();

		pad10->cd(iy+1);
		pad10->cd(iy+1)->SetLogy(1);
		h_MetSig[0][iy]->GetXaxis()->SetTitle("MET/SumET") ;
		h_MetSig[0][iy]->GetYaxis()->SetTitle("Entries");
		h_MetSig[0][iy]->GetYaxis()->SetTitleOffset(1.3);
		h_MetSig[0][iy]->SetLineColor(1);
		h_MetSig[1][iy]->SetLineColor(4);
		h_MetSig[0][iy]->SetLineWidth(2);
		h_MetSig[1][iy]->SetLineWidth(2);
		//h_MetSig_noise[0][iy]->Scale( h_MetSig[iy]->Integral() /h_MetSig_noise[0][iy]->Integral() );
		// h_MetSig_noise[1][iy]->Scale( h_MetSig[iy]->Integral() /h_MetSig_noise[1][iy]->Integral() );
		h_MetSig_noise[0][iy]->SetLineColor(2);
		h_MetSig_noise[1][iy]->SetLineColor(6);
		h_MetSig_noise[0][iy]->SetLineWidth(2);
		h_MetSig_noise[1][iy]->SetLineWidth(2); 
		h_MetSig[0][iy]->SetLineStyle(1);
		h_MetSig_noise[0][iy]->SetLineStyle(1);
		h_MetSig[0][iy]->SetMaximum(100000000);
		h_MetSig[0][iy]->SetMinimum(0.1);
		h_MetSig[0][iy]->Draw();
		h_MetSig[1][iy]->Draw("same hist");
		h_MetSig_noise[0][iy]->Draw("same hist");
		h_MetSig_noise[1][iy]->Draw("same hist");
		paveCMS ->Draw("same");
		teta->Draw();
		//leg1->Draw();

		pad11->cd(iy+1);
		pad11->cd(iy+1)->SetLogy(1);
		h_NMJet1[0][iy]->GetXaxis()->SetTitle("Neutral Multiplicity") ;
		h_NMJet1[0][iy]->GetYaxis()->SetTitle("Entries");
		h_NMJet1[0][iy]->GetYaxis()->SetTitleOffset(1.3);
		h_NMJet1[0][iy]->SetLineColor(1);
		h_NMJet1[1][iy]->SetLineColor(4);

		//h_NeutM_noise[0][iy]->Scale( h_NMJet1[0][iy]->Integral() /h_NeutM_noise[0][iy]->Integral() );
		//h_NeutM_noise[1][iy]->Scale( h_NMJet1[0][iy]->Integral() /h_NeutM_noise[1][iy]->Integral() );
		h_NeutM_noise[0][iy]->SetLineColor(2);
		h_NeutM_noise[1][iy]->SetLineColor(6);
		h_NeutM_noise[0][iy]->SetLineWidth(2);
		h_NeutM_noise[1][iy]->SetLineWidth(2);
		h_NMJet1[0][iy]->SetLineStyle(1);
		h_NeutM_noise[0][iy]->SetLineStyle(1); 
		h_NMJet1[0][iy]->SetMinimum(0.1);
		h_NMJet1[0][iy]->SetMaximum(100000000.);
		h_NMJet1[0][iy]->Draw();
		h_NMJet1[1][iy]->Draw("same hist");
		h_NeutM_noise[0][iy]->Draw("same hist");
		h_NeutM_noise[1][iy]->Draw("same hist");
		paveCMS ->Draw("same");
		teta->Draw();
		//leg1a->Draw();

		pad12->cd(iy+1);
		pad12->cd(iy+1)->SetLogy(1);
		h_CMJet1[0][iy]->GetXaxis()->SetTitle("Charged Multiplicity") ;
		h_CMJet1[0][iy]->GetYaxis()->SetTitle("Entries");
		h_CMJet1[0][iy]->GetYaxis()->SetTitleOffset(1.3);
		h_CMJet1[0][iy]->SetLineColor(1);
		h_CMJet1[1][iy]->SetLineColor(4);
		h_CMJet1[0][iy]->SetLineWidth(2);
		h_CMJet1[1][iy]->SetLineWidth(2);
		//h_ChM_noise[0][iy]->Scale( h_CMJet1[0][iy]->Integral() /h_ChM_noise[0][iy]->Integral() );
		//h_ChM_noise[1][iy]->Scale( h_CMJet1[0][iy]->Integral() /h_ChM_noise[1][iy]->Integral() );
		h_ChM_noise[0][iy]->SetLineColor(2);
		h_ChM_noise[1][iy]->SetLineColor(6);
		h_ChM_noise[0][iy]->SetLineWidth(2);
		h_ChM_noise[1][iy]->SetLineWidth(2);
		h_CMJet1[0][iy]->SetLineStyle(1);
		h_ChM_noise[0][iy]->SetLineStyle(1); 
		h_CMJet1[0][iy]->SetMinimum(0.1);
		if (iy<5) h_CMJet1[0][iy]->SetMaximum(100000.);
		if (iy>4) h_CMJet1[0][iy]->SetMaximum(100000000.);
		h_CMJet1[0][iy]->Draw();
		h_CMJet1[1][iy]->Draw("same hist");
		h_ChM_noise[0][iy]->Draw("same hist");
		h_ChM_noise[1][iy]->Draw("same hist");
		paveCMS ->Draw("same");
		teta->Draw();
		//leg1a->Draw();

		pad13->cd(iy+1);
		pad13->cd(iy+1)->SetLogy(1);
		h_PMJet1[0][iy]->GetXaxis()->SetTitle("Photon Multiplicity") ;
		h_PMJet1[0][iy]->GetYaxis()->SetTitle("Entries");
		h_PMJet1[0][iy]->GetYaxis()->SetTitleOffset(1.3);
		h_PMJet1[0][iy]->SetLineColor(1);
		h_PMJet1[1][iy]->SetLineColor(4);
		h_PMJet1[0][iy]->SetLineWidth(2);
		h_PMJet1[1][iy]->SetLineWidth(2);
		//h_PhM_noise[0][iy]->Scale( h_PMJet1[0][iy]->Integral() /h_PhM_noise[0][iy]->Integral() );
		//h_PhM_noise[1][iy]->Scale( h_PMJet1[0][iy]->Integral() /h_PhM_noise[1][iy]->Integral() );
		h_PhM_noise[0][iy]->SetLineColor(2);
		h_PhM_noise[1][iy]->SetLineColor(6);
		h_PhM_noise[0][iy]->SetLineWidth(2);
		h_PhM_noise[1][iy]->SetLineWidth(2);
		h_PMJet1[0][iy]->SetLineStyle(1);
		h_PhM_noise[0][iy]->SetLineStyle(1); 
		h_PMJet1[0][iy]->SetMinimum(0.1);
		h_PMJet1[0][iy]->SetMaximum(100000000.);
		h_PMJet1[0][iy]->Draw();
		h_PMJet1[1][iy]->Draw("same hist");
		h_PhM_noise[0][iy]->Draw("same hist");
		h_PhM_noise[1][iy]->Draw("same hist");
		paveCMS ->Draw("same");
		teta->Draw();
		//leg1a->Draw();

		pad15->cd(iy+1);
		pad15->cd(iy+1)->SetLogy(1);
		h_nJets[0][iy]->GetXaxis()->SetTitle("nJets per Event") ;
		h_nJets[0][iy]->GetYaxis()->SetTitle("Entries");
		h_nJets[0][iy]->GetYaxis()->SetTitleOffset(1.3);
		h_nJets[0][iy]->GetXaxis()->SetRangeUser(0.5,100);
		h_nJets[0][iy]->SetLineColor(1);
		h_nJets[1][iy]->SetLineColor(4);
		h_nJets[0][iy]->SetLineWidth(2);
		h_nJets[1][iy]->SetLineWidth(2);
		h_nJets_noise[0][iy]->SetLineColor(2);
		h_nJets_noise[1][iy]->SetLineColor(6);
		h_nJets_noise[0][iy]->SetLineWidth(2);
		h_nJets_noise[1][iy]->SetLineWidth(2);
		h_nJets[0][iy]->SetLineStyle(1);
		h_nJets_noise[0][iy]->SetLineStyle(1); 
		h_nJets[0][iy]->SetMinimum(0.5); 
		h_nJets[0][iy]->SetMaximum(10000000.);
//		h_nJets_noise[0][iy]->Draw();
//		h_nJets[0][iy]->Draw("same hist");
		h_nJets[0][iy]->Draw();
		h_nJets[1][iy]->Draw("same hist"); //edw
		h_nJets_noise[0][iy]->Draw("same hist");
		h_nJets_noise[1][iy]->Draw("same hist");
		paveCMS ->Draw("same");
		teta->Draw();


		pad16->cd(iy+1);
		pad16->cd(iy+1)->SetLogy(1);
		h_PHIJet1[0][iy]->GetXaxis()->SetTitle("#phi") ;
		h_PHIJet1[0][iy]->GetYaxis()->SetTitle("Entries");
		h_PHIJet1[0][iy]->GetYaxis()->SetTitleOffset(1.3);
		h_PHIJet1[0][iy]->SetLineColor(1);
		h_PHIJet1[1][iy]->SetLineColor(4);
		h_PHIJet1[0][iy]->SetLineWidth(2);
		h_PHIJet1[1][iy]->SetLineWidth(2);
		h_PHI_noise[0][iy]->SetLineColor(2);
		h_PHI_noise[1][iy]->SetLineColor(6);
		h_PHI_noise[0][iy]->SetLineWidth(2);
		h_PHI_noise[1][iy]->SetLineWidth(2);
		h_PHIJet1[0][iy]->SetLineStyle(1);
		h_PHI_noise[0][iy]->SetLineStyle(1); 
		h_PHIJet1[0][iy]->SetMinimum(0.5); 
		h_PHIJet1[0][iy]->SetMaximum(1000000.);
		h_PHIJet1[0][iy]->Draw();
		h_PHIJet1[1][iy]->Draw("same hist");
		h_PHI_noise[0][iy]->Draw("same hist");
		h_PHI_noise[1][iy]->Draw("same hist");
		paveCMS ->Draw("same");
		teta->Draw();
		//leg1->Draw();
/*
		pad17->cd(iy+1);
		pad17->cd(iy+1)->SetLogy(1);
		h_ETAJet1[0][iy]->GetXaxis()->SetTitle("#eta") ;
		h_ETAJet1[0][iy]->GetYaxis()->SetTitle("Entries");
		h_ETAJet1[0][iy]->GetYaxis()->SetTitleOffset(1.3);
		h_ETAJet1[0][iy]->SetLineColor(1);
		h_ETAJet1[1][iy]->SetLineColor(4);
		h_ETA_noise[0][iy]->SetLineColor(2);
		h_ETA_noise[1][iy]->SetLineColor(6);
		h_ETA_noise[0][iy]->SetLineWidth(2);
		h_ETA_noise[1][iy]->SetLineWidth(2);
		h_ETAJet1[0][iy]->SetLineStyle(1);
		h_ETA_noise[0][iy]->SetLineStyle(1); 
		h_ETAJet1[0][iy]->SetMinimum(0.5); 
		h_ETAJet1[0][iy]->SetMaximum(100000000.);
		h_ETAJet1[0][iy]->Draw();
		h_ETAJet1[1][iy]->Draw("same hist");
		h_ETA_noise[0][iy]->Draw("same hist");
		h_ETA_noise[1][iy]->Draw("same hist");
		paveCMS ->Draw("same");
		teta->Draw();
		//leg1->Draw();
		*/
		pad9->cd(iy+1);
		pad9->cd(iy+1)->SetLogy(1);
		h_ptJet1[0][iy]->GetXaxis()->SetTitle("pT of Jet1 (GeV)");
		h_ptJet1[0][iy]->GetYaxis()->SetTitle("Entries");
		h_ptJet1[0][iy]->GetYaxis()->SetTitleOffset(1.3);
		h_ptJet1[0][iy]->SetLineColor(1);
		h_ptJet1[1][iy]->SetLineColor(4);
		h_ptJet1[0][iy]->SetLineWidth(2);
		h_ptJet1[1][iy]->SetLineWidth(2);
		//h_pt_noise[0][iy]->Scale( h_ptJet1[0][iy]->Integral() /h_pt_noise[0][iy]->Integral() );
		//h_pt_noise[1][iy]->Scale( h_ptJet1[0][iy]->Integral() /h_pt_noise[1][iy]->Integral() );
		h_pt_noise[0][iy]->SetLineColor(2);
		h_pt_noise[1][iy]->SetLineColor(6);
		h_pt_noise[0][iy]->SetLineWidth(2);
		h_pt_noise[1][iy]->SetLineWidth(2);
		h_ptJet1[0][iy]->SetLineStyle(1);
		h_pt_noise[0][iy]->SetLineStyle(1); 
		h_ptJet1[0][iy]->SetMinimum(0.1); 
		h_ptJet1[0][iy]->SetMaximum(50000000);
		h_ptJet1[0][iy]->Draw();
		h_ptJet1[1][iy]->Draw("same hist");
		h_pt_noise[0][iy]->Draw("same hist");
		h_pt_noise[1][iy]->Draw("same hist");
		paveCMS ->Draw("same");
		teta->Draw();
		//leg1->Draw();
	} //end of eta regions

	pad1->cd(eta_bins+1);
	leg1->Draw();
	pad2->cd(eta_bins+1);
	leg1->Draw();
	pad3->cd(eta_bins+1);
	leg1->Draw();
	pad4->cd(eta_bins+1);
	leg1->Draw();
	pad5->cd(eta_bins+1);
	leg1->Draw();
	pad9->cd(eta_bins+1);
	leg1->Draw();
	pad10->cd(eta_bins+1);
	leg1->Draw();
	pad11->cd(eta_bins+1);
	leg1->Draw();
	pad12->cd(eta_bins+1);
	leg1->Draw();
	pad13->cd(eta_bins+1);
	leg1->Draw();
	pad16->cd(eta_bins+1);
	leg1->Draw();
	pad15->cd(eta_bins+1);
	leg1->Draw();

/*
	pad14->cd()->SetLogy(1);
	h_ETAJet1[0]->GetXaxis()->SetTitle("#eta") ;
	h_ETAJet1[0]->GetYaxis()->SetTitle("Entries");
	h_ETAJet1[0]->GetYaxis()->SetRangeUser(0.5,1000000);
	h_ETAJet1[0]->GetYaxis()->SetTitleOffset(1.3);
	h_ETAJet1[0]->SetLineWidth(2);
	h_ETAJet1[0]->SetLineColor(1);
	h_ETAJet1[0]->SetMarkerSize(0.04);
	h_ETAJet1[1]->SetLineWidth(2);
	h_ETAJet1[1]->SetLineColor(4);
	h_ETAJet1[1]->SetMarkerSize(0.04);
//	h_ETAJet1->Scale(h_ETAJet1[1]->Integral()/h_ETAJet1->Integral());
	h_ETAJet1[0]->GetYaxis()->SetRangeUser(0.5,1000000);
	h_ETA_noise[0]->SetLineColor(2);
	h_ETA_noise[1]->SetLineColor(6);
	h_ETA_noise[0]->SetLineWidth(2);
	h_ETA_noise[1]->SetLineWidth(2);
	h_ETA_noise[0]->SetLineStyle(1); 
	h_ETAJet1[0]->Draw("hist");
	h_ETAJet1[1]->Draw("same");
	h_ETA_noise[0]->Draw("same");
	h_ETA_noise[1]->Draw("same");
	paveCMS ->Draw("same");

	TLegend *leg2 =new TLegend(.2, .75, .4, .9);//7899//4899
	leg2->SetTextSize(0.025);
	leg2->SetFillColor(0); 
	leg2->SetBorderSize(0);
	leg2->SetHeader("PUPPI"); 
	leg2->AddEntry(h_ETAJet1[0], "Back-to-back", "L");
	leg2->AddEntry(h_ETAJet1[1], "Back-to-back JetID", "L");
	leg2->AddEntry(h_ETA_noise[0], "MinBias", "L");
	leg2->AddEntry(h_ETA_noise[1], "MinBias JetID", "L");
	leg2->Draw("same");
*/



	if(Save_Plots)
	{
		sprintf(filename,"%s/%s_chf_jet1.png",output_directory,image_name);
		pad1->SaveAs(filename);
		sprintf(filename,"%s/%s_nhf_jet1.png",output_directory,image_name);
		pad2->SaveAs(filename);
		sprintf(filename,"%s/%s_cemf_jet1.png",output_directory,image_name);
		pad3->SaveAs(filename);
		sprintf(filename,"%s/%s_nemf_jet1.png",output_directory,image_name);
		pad4->SaveAs(filename);
		sprintf(filename,"%s/%s_muf_jet1.png",output_directory,image_name);
		pad5->SaveAs(filename);
		sprintf(filename,"%s/%s_pt_jet1.png",output_directory,image_name);
		pad9->SaveAs(filename);
		sprintf(filename,"%s/%s_MetSig.png",output_directory,image_name);
		pad10->SaveAs(filename);
		sprintf(filename,"%s/%s_NM.png",output_directory,image_name);
		pad11->SaveAs(filename);
		sprintf(filename,"%s/%s_CM.png",output_directory,image_name);
		pad12->SaveAs(filename);
		sprintf(filename,"%s/%s_PM.png",output_directory,image_name);
		pad13->SaveAs(filename);
		sprintf(filename,"%s/%s_eta.png",output_directory,image_name);
		pad14->SaveAs(filename);
		sprintf(filename,"%s/%s_nJetsPerEvent.png",output_directory,image_name);
		pad15->SaveAs(filename);
		sprintf(filename,"%s/%s_phi.png",output_directory,image_name);
		pad16->SaveAs(filename);
		
		//sprintf(filename,"%s/%s/%s_%s_eta.png",analyzer_path,output_directory,image_name,c);
		//pad17->SaveAs(filename);
		//sprintf(filename,"%s/%s/%s_%s_%s_eta_phi.png",analyzer_path,output_directory,image_name,legend_1,c);
		//eta_phi_canvas->SaveAs(filename);
		//sprintf(filename,"%s/%s/%s_%s_%s_eta_phi.png",analyzer_path,output_directory,image_name,legend_2,c);
		//eta_phi_canvas_with->SaveAs(filename);
	}
	
/*
	sprintf(filename,"/afs/cern.ch/work/e/etzia/private/JetID_13TeV_2017/2017/rereco_plots/puppi/btb_vs_noise/btbdijets_and_noise_no_vs_withJetID_Jet400_%s_chf.pdf", c);
	pad1->SaveAs(filename);
	sprintf(filename,"/afs/cern.ch/work/e/etzia/private/JetID_13TeV_2017/2017/rereco_plots/puppi/btb_vs_noise/btbdijets_and_noise_no_vs_withJetID_Jet400_%s_nhf.pdf", c);
	pad2->SaveAs(filename);
	sprintf(filename,"/afs/cern.ch/work/e/etzia/private/JetID_13TeV_2017/2017/rereco_plots/puppi/btb_vs_noise/btbdijets_and_noise_no_vs_withJetID_Jet400_%s_cemf.pdf", c);
	pad3->SaveAs(filename);
	sprintf(filename,"/afs/cern.ch/work/e/etzia/private/JetID_13TeV_2017/2017/rereco_plots/puppi/btb_vs_noise/btbdijets_and_noise_no_vs_withJetID_Jet400_%s_nemf.pdf", c);
	pad4->SaveAs(filename);
	sprintf(filename,"/afs/cern.ch/work/e/etzia/private/JetID_13TeV_2017/2017/rereco_plots/puppi/btb_vs_noise/btbdijets_and_noise_no_vs_withJetID_Jet400_%s_muf.pdf", c);
	pad5->SaveAs(filename);
	sprintf(filename,"/afs/cern.ch/work/e/etzia/private/JetID_13TeV_2017/2017/rereco_plots/puppi/btb_vs_noise/btbdijets_and_noise_no_vs_withJetID_Jet400_%s_pt.pdf", c);
	pad9->SaveAs(filename);
	sprintf(filename,"/afs/cern.ch/work/e/etzia/private/JetID_13TeV_2017/2017/rereco_plots/puppi/btb_vs_noise/btbdijets_and_noise_no_vs_withJetID_Jet400_%s_MetSig.pdf", c);
	pad10->SaveAs(filename);
	sprintf(filename,"/afs/cern.ch/work/e/etzia/private/JetID_13TeV_2017/2017/rereco_plots/puppi/btb_vs_noise/btbdijets_and_noise_no_vs_withJetID_Jet400_%s_NM.pdf", c);
	pad11->SaveAs(filename);
	sprintf(filename,"/afs/cern.ch/work/e/etzia/private/JetID_13TeV_2017/2017/rereco_plots/puppi/btb_vs_noise/btbdijets_and_noise_no_vs_withJetID_Jet400_%s_CM.pdf", c);
	pad12->SaveAs(filename);
	sprintf(filename,"/afs/cern.ch/work/e/etzia/private/JetID_13TeV_2017/2017/rereco_plots/puppi/btb_vs_noise/btbdijets_and_noise_no_vs_withJetID_Jet400_%s_PM.pdf", c);
	pad13->SaveAs(filename);
*/
}

