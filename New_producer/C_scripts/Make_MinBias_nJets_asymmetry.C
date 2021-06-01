#include <vector>
#ifdef __MAKECINT__
#pragma link C++ class vector<vector<float> >+;
#pragma link C++ class vector<vector<int> >+;
#endif
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TBenchmark.h"
#include "TRandom.h"
#include "TChain.h"
#include "TSystem.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TF1.h"
#include "TROOT.h"
#include "TGraphAsymmErrors.h"
#include "TSystemDirectory.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;




int getBin(double x, double boundaries[], int bins) ;
int BoolToInt(bool boolean);

void Make_MinBias_nJets_asymmetry()
{


	char rep_path[200] = {"/afs/cern.ch/user/d/dkarasav/public/testing_JetID/"}; 

//	TFile *f = new TFile("/eos/cms/store/user/dkarasav/Test/MinimumBias__Run2017D-09Aug2019_reduced_tree_1.root","READ");

	char input_directory[200] = { "/eos/cms/store/user/dkarasav/MinBias_UL17_RunD_Puppi_trees/"};
	char output_directory[200] = {"UL2017/MinBias/RunD/"};
//	char out_name[200] = {"MinimumBias_RunD_HF_NegativeEta"};
	char out_name[200] = {"MinBias_nJet_asymmetry_n1remove"};

	bool useAbsoluteEta = false;
	bool Make_nJetsAssymetry = true;


	double yBndFabs[] = {0.0, 2.7, 3.0, 5.0};
	double yBnd[] = {-5.0, -3.0, -2.7, 0.0, 2.7, 3.0, 5.0};    


	double pTmin = 15;
	double pTmax = 50000;
	float pt_histo_max = 500;
	int pT_threshold_1D_histos = 160;
	int N_histo_bins = 25;
	bool passFilterGoodVtx;




	const char *ext=".root";

	TChain *tree = new TChain("tree");

	Char_t filechain[1024];
	TSystemDirectory dir(input_directory, input_directory); 
	TList *files = dir.GetListOfFiles(); 

	if (files)
	{ 
		TSystemFile *file; 
		TString fname;
		TIter next(files); 
		while ((file=(TSystemFile*)next()))
		{
			fname	 = file->GetName(); 
			if (!file->IsDirectory() && fname.EndsWith(ext))
			{ 
				cout << fname.Data() << endl; 
				sprintf(filechain,"%s/%s",input_directory,fname.Data());
				tree->Add(filechain);
			} 
		}
	} 





//	TTree *tree;
//	tree = (TTree*)f->Get("tree"); // get the tree from the file


	int nJets, lumi, event, run, nVtx, era; 
	bool isCHS, isUL;
	float met, metSig;
	

	std::vector<float> *jpt = 0;
	std::vector<float> *eta = 0;
	std::vector<float> *phi = 0;
	std::vector<float> *nhf = 0;
	std::vector<float> *chf = 0;
	std::vector<float> *phf = 0;
	std::vector<float> *cemf = 0;
	std::vector<float> *nemf = 0;
	std::vector<float> *muf = 0;
	std::vector<float> *elf = 0;
	std::vector<int> *chMult = 0;
	std::vector<int> *neMult = 0;
	std::vector<int> *phoMult = 0;
	std::vector<float> *neMultPuppi = 0;
	std::vector<float> *phoMultPuppi = 0;
	std::vector<bool> *JetID = 0;
	std::vector<bool> *JetIDlepVeto = 0;


	tree->SetBranchAddress("era",&era);
	tree->SetBranchAddress("isCHS",&isCHS);
	tree->SetBranchAddress("isUL",&isUL);

	tree->GetEntry(0);  // just to read the boolean isCHS 

	tree->SetBranchAddress("met",&met);
	tree->SetBranchAddress("metSig",&metSig);
	tree->SetBranchAddress("run",&run);
	tree->SetBranchAddress("nJets",&nJets);
	tree->SetBranchAddress("lumi",&lumi);
	tree->SetBranchAddress("event",&event);
	tree->SetBranchAddress("nVtx",&nVtx);
	tree->SetBranchAddress("passFilterGoodVtx",&passFilterGoodVtx); 

	tree->SetBranchAddress("JetID",&JetID); 
	tree->SetBranchAddress("JetIDlepVeto",&JetIDlepVeto); 
	tree->SetBranchAddress("jetPt",&jpt);
	tree->SetBranchAddress("jetEta",&eta);
	tree->SetBranchAddress("jetPhi",&phi);
	tree->SetBranchAddress("jetNhf",&nhf);
	tree->SetBranchAddress("jetChf",&chf);
	tree->SetBranchAddress("jetPhf",&phf);
	tree->SetBranchAddress("jetMuf",&muf);
	tree->SetBranchAddress("jetElf",&elf);
	tree->SetBranchAddress("jetCemf",&cemf);
	tree->SetBranchAddress("jetNemf",&nemf);
	tree->SetBranchAddress("chMult",&chMult); 
	if (isCHS) 	tree->SetBranchAddress("neMult",&neMult); 
	else tree->SetBranchAddress("neMult",&neMultPuppi); 
	if (isCHS) 	tree->SetBranchAddress("phoMult",&phoMult); 
	else tree->SetBranchAddress("phoMult",&phoMultPuppi); 



	const int eta_bins = sizeof(yBnd)/sizeof(yBnd[0])-1;
	const int eta_binsFabs = sizeof(yBndFabs)/sizeof(yBndFabs[0])-1;

	TH1D  *h_nJets[eta_bins], *h_nJets_asymmetry[eta_binsFabs];
	TH2D *h_NM_nJets[eta_bins];

	char name[150];

	for(Int_t h=0; h<eta_bins;h++)
	{    
		sprintf(name,"h_nJets_%i",h);
		h_nJets[h] = new TH1D(name,"",70, 0, 70);

		sprintf(name,"h_NM_nJets_%i",h);
		h_NM_nJets[h] = new TH2D(name,"", 70, 0, 70, 70, 0, 70);
	}
	for(Int_t h=0; h<eta_binsFabs;h++)
	{    
		sprintf(name,"h_nJets_asymmetry_%i",h);
		h_nJets_asymmetry[h] = new TH1D(name,"",31, -1.2, 1.2);
	}



	int nentries = tree->GetEntries();
	cout << "Number of total entries = " << nentries << endl;   
	
	//bool event_JetID = false;
	for (int i=0; i<nentries; i++) //event loop
//	for (int i=0; i<5000000; i++) //event loop
//	for (int i=2629248; i<2629250; i++) //event loop
	{
		if(i < 10 || i%1000000 == 0) cout << " jentry = " << i << endl; 
//		if(i < 10 || i%2 == 0) cout << " jentry = " << i << endl;     
		tree->GetEntry(i);
		//bool event_JetID = false;
		if( passFilterGoodVtx ) continue; // for noise jets we want only events with passFilterGoodVtx==0.
		int no_jets = jpt->size();
	//	cout << no_jets << endl;

		double nJets_noJetID[eta_bins] = {};// initialize with zeros
		double nJetsAssym_noJetID[eta_binsFabs] = {};// initialize with zeros
		double NM_average[eta_bins] = {};// initialize with zeros


		for(int j=0; j<no_jets; ++j)
		{


			int ybin;

			if ( !(jpt->at(j) > pTmin && jpt->at(j)<pTmax) ) continue;   //pT cuts

			if (  !useAbsoluteEta && !( eta->at(j) > yBnd[0] && eta->at(j)<yBnd[eta_bins])) continue;     // skip jets with eta out of wanted boundaries
			else if ( useAbsoluteEta && !(  fabs(eta->at(j)) > yBnd[0] && fabs(eta->at(j))<yBnd[eta_bins]   ) ) continue; // skip jets with |eta| of wanted boundaries

			if (useAbsoluteEta) ybin = getBin( fabs(eta->at(j)) ,yBnd, eta_bins);
			else ybin = getBin( eta->at(j) ,yBnd, eta_bins);
	
			nJets_noJetID[ybin] = nJets_noJetID[ybin] +1;                     // count number of jets in each eta bin

			if(isCHS )	NM_average[ybin] = NM_average[ybin] + neMult->at(j);
			else NM_average[ybin] = NM_average[ybin] + neMultPuppi->at(j);

		} // end of jet loop

	//	cout << "  1 "  << endl;

		for (int h =0; h<eta_binsFabs; h++)
		{
			if ( ( nJets_noJetID[h] + nJets_noJetID[eta_bins-1-h]) == 1  ) continue;
			if (( nJets_noJetID[h] + nJets_noJetID[eta_bins-1-h]) == 0  ) continue;
			nJetsAssym_noJetID[h] = ( nJets_noJetID[h] - nJets_noJetID[eta_bins-1-h] ) / ( nJets_noJetID[h] + nJets_noJetID[eta_bins-1-h] ) ;
			h_nJets_asymmetry[h]->Fill(nJetsAssym_noJetID[h]);
		}

		//cout << "  2 "  << endl;

		for (int h =0; h<eta_bins; h++)
		{
			NM_average[h] = NM_average[h] / nJets_noJetID[h];
			h_nJets[h]->Fill(nJets_noJetID[h]);
			h_NM_nJets[h]->Fill(NM_average[h], nJets_noJetID[h] );
			
		}
	} // end of event loop

	char filename[200];
	sprintf(filename,"%s/%s.root",output_directory,out_name);

	TFile *fout = new TFile(filename,"recreate");
	fout->cd();

	for(Int_t ybin=0; ybin<eta_bins;ybin++)
	{
		h_nJets[ybin]->Write();
		h_NM_nJets[ybin]->Write();
	}

	for(Int_t ybin=0; ybin<eta_binsFabs; ybin++)
	{
		h_nJets_asymmetry[ybin]->Write();
	}
	fout->Close();
}

int BoolToInt(bool boolean)
{
	int i;
	if (boolean) i = 1;
	else i=0;

	return i;
}


int getBin(double x, double boundaries[], int bins) 
{
	int i;
	int n = bins; //sizeof(boundaries)/sizeof(boundaries[0])-1;
//	int n = sizeof(boundaries)/sizeof(boundaries[0])-1;
//	cout << n << endl;
	if (x<boundaries[0] || x>=boundaries[n]) return -1;
	for(i=0;i<n;i++)
	{
		if (x>=boundaries[i] && x<boundaries[i+1]) return i;
	}
	return 0;
}

