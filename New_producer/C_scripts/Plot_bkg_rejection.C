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
void Plot_bkg_rejection()
{

	char *analyzer_path = "/afs/cern.ch/user/d/dkarasav/public/testing_JetID/"; 
	char *output_directory = "UL2016/PUPPI/nonAPV/testing_35/";     
	char *image_name = "UL16_RunGH_puppi_0p6"; 
	char *Legend_header = "AK4 PUPPI"; 

	const int eta_bins = 8;

//	gROOT->LoadMacro("setTDRStyle_teliko.C");
	setTDRStyle_teliko();

	TH1D *h_pt_noise[eta_bins], *h_eta_noise[eta_bins];    
	TH1D *h_pt_noise_with[eta_bins],*h_eta_noise_with[eta_bins];

	//=======Minimum Bias Sample =====

//	TFile *f = TFile::Open("root://eoscms//eos/cms/store/group/phys_jetmet/dimitris/UL2017_MinBias/Puppi_RunD/MinimumBias__Run2017D-09Aug2019_reduced_skim.root","READ");
//	TFile *f = TFile::Open("UL2016/PUPPI/nonAPV/RunGH_PuppiAPVJetIDused.root","READ");
	TFile *f = TFile::Open("UL2016/PUPPI/nonAPV/testing_35/RunGH_nemf0p6.root","READ");

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




	TPaveText *paveCMS = new TPaveText(0.16,0.95,0.88,0.99,"NDC");
	paveCMS->AddText("CMS Preliminary    #sqrt{s} = 13 TeV");
	paveCMS->SetFillColor(0);
	paveCMS->SetBorderSize(0);
	paveCMS->SetTextSize(0.04);
	 
//	char eta_bins_legend[eta_bins][25] = { "|#eta|<0.5", "0.5 #leq |#eta| < 1.0", "1.0 #leq |#eta| < 1.5", "1.5 #leq |#eta| < 2.0", "2.0 #leq |#eta| < 2.6","2.6 #leq |#eta| < 2.7", "2.7 #leq |#eta| < 3.0" , "3.0 #leq |#eta| < 5.0" };

	char eta_bins_legend[eta_bins][25] = { "|#eta|<0.5", "0.5 #leq |#eta| < 1.0", "1.0 #leq |#eta| < 1.5", "1.5 #leq |#eta| < 2.0", "2.0 #leq |#eta| < 2.4","2.4 #leq |#eta| < 2.7", "2.7 #leq |#eta| < 3.0" , "3.0 #leq |#eta| < 5.0" }; 

	TCanvas *pad1 = new TCanvas("pad1", "",1); 

	pad1->Divide(3,3);

	for ( int j=0; j<eta_bins; j++) 
	{



		//int j=3; //0-6

		
		TH1D *h_pt_noise_all = (TH1D*) h_pt_noise[j]->Clone();
		TH1D *h_pt_noise_with_all = (TH1D*) h_pt_noise_with[j]->Clone();

		
		/*
		//if you want pt only for the central region don't run through the loop
		for(int iy=1; iy<7; iy++){
		h_pt_noise_all->Add(h_pt_noise[iy]);
		h_pt_noise_with_all->Add(h_pt_noise_with[iy]);
		h_eta_noise_all->Add(h_eta_noise[iy]);
		h_eta_noise_with_all->Add(h_eta_noise_with[iy]);
		}
		*/

		//if you want to rebin the hist's, otherwise command it out
//		h_pt_noise_all->Rebin();
//		h_eta_noise_all->Rebin(5);
//		h_pt_noise_with_all->Rebin();
//		h_eta_noise_with_all->Rebin(5);

		//change the array size, depending on the number of bins you have after rebining
		//double y[61], eyu[61], eyl[61], x[61], ex[61];
		double y[120], eyu[120], eyl[120], x[120], ex[120];

		int nbins = h_pt_noise_all->GetNbinsX(); 

		double eta_bin_efficiency_sum = 0;
		double non_zero_efficiency_bins_sum = 0;
		double No_of_noise_jets = 0;
		double Sum_No_of_noise_jets = 0;

		for(int ibin=1; ibin<nbins+1; ibin++)
		{ 
			x[ibin]=0.0;
			y[ibin]=0.0;
			eyu[ibin]=0.0;
			eyl[ibin]=0.0;
			ex[ibin]=0.0;

			if(h_pt_noise_all->GetBinContent(ibin)>0)
			{
				x[ibin] = h_pt_noise_with_all->GetBinCenter(ibin);
				y[ibin] = 1-h_pt_noise_with_all->GetBinContent(ibin)/h_pt_noise_all->GetBinContent(ibin);
				//y[ibin] = h_pt_noise_with_all->GetBinContent(ibin)/h_pt_noise_all->GetBinContent(ibin);//I changed eyl to eyu and vice-verse
				// if(h_pt_noise_with_all->GetBinContent(ibin)>0){ //use this if-else statement, if you want to set errors to 0 with bin content=0, in order not to have the large errors in high pt values, so you can zoom the y-axis
				eyu[ibin] = error1(h_pt_noise_all->GetBinContent(ibin),h_pt_noise_with_all->GetBinContent(ibin));
				eyl[ibin] = error2(h_pt_noise_all->GetBinContent(ibin),h_pt_noise_with_all->GetBinContent(ibin));
	
				No_of_noise_jets = h_pt_noise_all->GetBinContent(ibin);
				Sum_No_of_noise_jets = Sum_No_of_noise_jets + No_of_noise_jets;	
				// }else{
				//	eyu[ibin]=0.0; eyl[ibin]=0.0;
				//}
				ex[ibin] = h_pt_noise_with_all->GetBinWidth(ibin)/2;
				eta_bin_efficiency_sum = eta_bin_efficiency_sum + y[ibin]*No_of_noise_jets;
				if ( y[ibin]>0 ) non_zero_efficiency_bins_sum = non_zero_efficiency_bins_sum + 1;
				//cout<<ibin<<"	"<<h_pt_noise_with_all->GetBinContent(ibin)<<" "<< h_pt_noise_all->GetBinContent(ibin) <<" "<<x[ibin]<<"  "<<y[ibin]<<"  "<<eyl[ibin]<<"	"<<eyu[ibin] <<endl;
			 }
			
		}//end of ibins

		cout << " eta bin: " << eta_bins_legend[j] << "  , non-zero pT bins = " <<non_zero_efficiency_bins_sum   << ",  pT bin eff sum = " << eta_bin_efficiency_sum <<",  total efficiency = " << eta_bin_efficiency_sum/Sum_No_of_noise_jets  << endl; 
		//double y_eta[20], eyu_eta[20], eyl_eta[20], x_eta[20], ex_eta[20];



		pad1->cd(j+1);
		// draw a frame to define the range, in order to plot many graphs in the same pad
		TH1F *hr ;
/*
		if (j < 3) hr = pad1->cd(j+1)->DrawFrame(0, 0.9, 3000, 1.05); //(0, -0.05, 3000, 0.55); //(0,-0.001,3000,0.006); //(0, 0.995, 3000, 1.002);//0.4-1.2 //0.75-1.05
		else if (j == 3) hr = pad1->cd(j+1)->DrawFrame(0, 0.5, 1600, 1.2);
		else if (j == 4) hr = pad1->cd(j+1)->DrawFrame(0, 0.2, 1600, 1.2);
		else if (j == 5) hr = pad1->cd(j+1)->DrawFrame(0, 0.2, 1500, 1.2);
		else hr = pad1->cd(j+1)->DrawFrame(0, -0.05, 2500, 1.2);
*/
		if (j < 4) hr = pad1->cd(j+1)->DrawFrame(0, 0.8, 3000, 1.2); //(0, -0.05, 3000, 0.55); //(0,-0.001,3000,0.006); //(0, 0.995, 3000, 1.002);//0.4-1.2 //0.75-1.05
		else if (j == 4) hr = pad1->cd(j+1)->DrawFrame(0, 0.8, 1600, 1.2);
		else if (j == 5) hr = pad1->cd(j+1)->DrawFrame(0, 0.2, 800, 1.2);
//		else hr = pad1->cd(j+1)->DrawFrame(0, -0.05, 1000, 1.2);
		else hr = pad1->cd(j+1)->DrawFrame(0, -0.002, 1000, 0.01);
		//if (j != 6) TH1F *hr = pad1->cd()->DrawFrame(0, 0.95, 3000, 1.05); //(0, -0.05, 3000, 0.55); //(0,-0.001,3000,0.006); //(0, 0.995, 3000, 1.002);//0.4-1.2 //0.75-1.05
		//else TH1F *hr = pad1->cd()->DrawFrame(0, 0.6, 3000, 1.4);
		hr->SetXTitle("jet pT (GeV)");
		hr->SetYTitle("PFJetID Background Rejection");
	//	hr->SetYTitle("HitFilter Background Rejection");
		hr->GetYaxis()->SetTitleOffset(1.5); //2.1

		TGraphAsymmErrors *gr = new TGraphAsymmErrors(nbins, x, y,ex, ex,eyu,eyl);
		gr->SetMarkerStyle(24);
		gr->SetMarkerColor(1);
		gr->SetMarkerSize(0.3);
		gr->SetLineColor(1);
		gr->Draw("p"); 
		//CMS_lumi( pad1, 0,10 );







		TLegend *leg2 =new TLegend(.55, .9, .9, .85);//4899
		leg2->SetTextSize(0.06);
		leg2->SetFillColor(0); 
		leg2->SetBorderSize(0);
		leg2->SetHeader(eta_bins_legend[j]); 
		leg2->Draw();
		paveCMS->Draw("same");
	
		

	/*	
		TCanvas *pad2 = new TCanvas("pad2", "",1); 
		pad2->cd();
		// draw a frame to define the range, in order to plot many graphs in the same pad
		TH1F *hr2 = pad2->cd()->DrawFrame(-5,0.6,5,1.15); //0.75-1.05
		hr2->SetXTitle("#eta");
		hr2->SetYTitle("PFJetID Background Rejection");
		//hr2->GetYaxis()->SetTitleOffset(1.3);

		TGraphAsymmErrors *gr2 = new TGraphAsymmErrors(nbins, x_eta, y_eta,ex_eta, ex_eta,eyu_eta,eyl_eta);
		gr2->SetMarkerStyle(24);
		gr2->SetMarkerColor(1);
		gr2->SetMarkerSize(0.3);
		gr2->SetLineColor(1);
		gr2->Draw("p"); 

		//CMS_lumi( pad2, 0,10 );
		TLegend *leg1 =new TLegend(.6, .9, .9, .85);//4899
		leg1->SetTextSize(0.04);
		leg1->SetFillColor(0); 
		leg1->SetBorderSize(0);
		leg1->SetHeader(Legend_header); 
		leg1->Draw();
	

		sprintf(filename,"%s/%s/bg_rej_vsPt_%s.png",analyzer_path,output_directory,image_name);
		pad2->SaveAs(filename);
		sprintf(filename,"%s/%s/bg_rej_vsPt_%s.pdf",analyzer_path,output_directory,image_name);
		pad2->SaveAs(filename);
*/	
	}

		char filename[256]; 
		sprintf(filename,"%s/%s/bg_rej_vsPt_%s.png",analyzer_path,output_directory,image_name);
		pad1->SaveAs(filename);
		sprintf(filename,"%s/%s/bg_rej_vsPt_%s.pdf",analyzer_path,output_directory,image_name);
		//pad1->SaveAs(filename);

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


