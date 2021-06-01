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

void Make_JetHT_btb_Histos()
{


	char rep_path[200] = {"/afs/cern.ch/user/d/dkarasav/public/testing_JetID/"}; 

//	char input_directory[200] = { "/eos/cms/store/user/dkarasav/MinBias_UL17_RunD_Puppi_trees/"};
	char input_directory[200] = { "/eos/cms/store/user/dkarasav/JetHT_UL17_RunD_Puppi_trees/"};

	char output_directory[200] = {"/afs/cern.ch/user/d/dkarasav/public/testing_JetID/UL2017/JetHT/RunD/"};
	char out_name[200] = {"Run2017D-09Aug2019_UL2017-v1__MINIAOD_HF_PlusMinusEta_samepTcuts"};

	bool useAbsoluteEta = false;


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



//	double yBnd[] = {0.0, 0.5, 1.0, 1.5, 2.0, 2.6, 2.7, 3.0, 5.0};    

	double yBnd[] = {-5.0, -3.0, -2.7, 0.0, 2.7, 3.0, 5.0};    

	double deltaPhiCut = 2.7;
	double pTcut_leading = 60;
	double pTcut_subleading = 30;
	double pTmin = 15;
	double pTmax = 50000;
	float pt_histo_max = 500;
	int pT_threshold_1D_histos = 160;
	int N_histo_bins = 25;


//	TTree *tree;
//	tree = (TTree*)f->Get("tree"); // get the tree from the file


	int nJets, lumi, event, run, nVtx, era; 
	bool isCHS, isUL;
	float met, metSig;
	bool passHLT3; 

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

	tree->SetBranchAddress("passHLT3",&passHLT3); 
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

	TH1D *h_pT[2][eta_bins], *h_CHF[2][eta_bins], *h_NHF[2][eta_bins], *h_Elf[2][eta_bins], *h_Phf[2][eta_bins], *h_MUF[2][eta_bins], *h_CEMF[2][eta_bins], *h_NEMF[2][eta_bins], *h_ChM[2][eta_bins], *h_NeutM[2][eta_bins], *h_PhM[2][eta_bins], *h_nVtx[2][eta_bins], *h_MetSig[2][eta_bins], *h_GoodVtx[2][eta_bins], *h_ETA[2], *h_PHI[2][eta_bins], *h_nJets[2][eta_bins];

	char name[150];
	for (int jetID=0;  jetID<2; jetID++)
	{
		
		if (jetID==0 ) sprintf(name,"h_ETA");
		else           sprintf(name,"h_ETA_with");
		h_ETA[jetID] = new TH1D(name,"", 100, -5.0, 5.0);
		for(Int_t h=0; h<eta_bins;h++)
		{    
			if (jetID==0 ) sprintf(name,"h_pT_%i",h);
			else sprintf(name,"h_pT_with_%i",h);
			h_pT[jetID][h] = new TH1D(name, "", 120, 0, 12000);

			if (jetID==0 ) sprintf(name,"h_CHF_%i",h);
			else sprintf(name,"h_CHF_with_%i",h);
			h_CHF[jetID][h] = new TH1D(name, "", 60, 0, 1.2);

			if (jetID==0 ) sprintf(name,"h_NHF_%i",h);
			else sprintf(name,"h_NHF_with_%i",h);
			h_NHF[jetID][h] = new TH1D(name, "", 60, 0, 1.2);

			if (jetID==0 ) sprintf(name,"h_Elf_%i",h);
			else sprintf(name,"h_Elf_with_%i",h);
			h_Elf[jetID][h] = new TH1D(name, "", 60, 0, 1.2);

			if (jetID==0 ) sprintf(name,"h_Phf_%i",h);
			else sprintf(name,"h_Phf_with_%i",h);
			h_Phf[jetID][h] = new TH1D(name, "", 60, 0, 1.2);

			if (jetID==0 ) sprintf(name,"h_CEMF_%i",h);
			else sprintf(name,"h_CEMF_with_%i",h);
			h_CEMF[jetID][h] = new TH1D(name, "", 60, 0, 1.2);

			if (jetID==0 ) sprintf(name,"h_NEMF_%i",h);
			else sprintf(name,"h_NEMF_with_%i",h);
			h_NEMF[jetID][h] = new TH1D(name, "", 60, 0, 1.2);

			if (jetID==0 ) sprintf(name,"h_MUF_%i",h);
			else sprintf(name,"h_MUF_with_%i",h);
			h_MUF[jetID][h] = new TH1D(name, "", 60, 0, 1.2);

			if (jetID==0 ) sprintf(name,"h_ChM_%i",h);
			else sprintf(name,"h_ChM_with_%i",h);
			h_ChM[jetID][h] = new TH1D(name, "", 100, 0, 100);

			if (jetID==0 ) sprintf(name,"h_NeutM_%i",h);
			else sprintf(name,"h_NeutM_with_%i",h);
			h_NeutM[jetID][h] = new TH1D(name, "", 100, 0, 100);

			if (jetID==0 ) sprintf(name,"h_PhM_%i",h);
			else sprintf(name,"h_PhM_with_%i",h);
			h_PhM[jetID][h] = new TH1D(name, "", 100, 0, 100);

			if (jetID==0 ) sprintf(name,"h_nVtx_%i",h);
			else sprintf(name,"h_nVtx_with_%i",h);
			h_nVtx[jetID][h] = new TH1D(name,"", 50, 0, 5.0);

			if (jetID==0 ) sprintf(name,"h_MetSig_%i",h);
			else sprintf(name,"h_MetSig_with_%i",h);
			h_MetSig[jetID][h] = new TH1D(name,"", 50, 0, 1.0);

			if (jetID==0 ) sprintf(name,"h_PHI_%i",h);
			else sprintf(name,"h_PHI_with_%i",h);
			h_PHI[jetID][h] = new TH1D(name,"",70, -3.5, 3.5);

			if (jetID==0 ) sprintf(name,"h_nJets_%i",h);
			else sprintf(name,"h_nJets_with_%i",h);
			h_nJets[jetID][h] = new TH1D(name,"",70, 1, 71);
		}
	}




	int nentries = tree->GetEntries();
	cout << "Number of total entries = " << nentries << endl;   
	
	//bool event_JetID = false;
	for (int i=0; i<nentries; i++) //event loop
//	for (int i=0; i<1000; i++) //event loop
	{
		if(i < 10 || i%1000000 == 0) cout << " jentry = " << i << endl; 
//		if(i < 10 || i%2 == 0) cout << " jentry = " << i << endl;     
		tree->GetEntry(i);
		if(!passHLT3) continue; //use trigger
		int no_jets = jpt->size();
		if (no_jets < 2) continue; // skip event if less than 2 jets.

		if ( !(  (jpt->at(0) > pTmin && jpt->at(0)<pTmax ) && (jpt->at(1) > pTmin && jpt->at(1)<pTmax)  ) ) continue;   //total pT cuts
		if ( !( jpt->at(0) > pTcut_leading && jpt->at(1) > pTcut_subleading) ) continue; // skip event if either j1 or j2 dont pass ptcuts.


		if (  !useAbsoluteEta && !( eta->at(0) > yBnd[0] && eta->at(0)<yBnd[eta_bins])) continue;     // skip events with leading eta out of wanted boundaries
		else if ( useAbsoluteEta && !(  fabs(eta->at(0)) > yBnd[0] && fabs(eta->at(0))<yBnd[eta_bins]   ) ) continue; // skip events with |eta| of wanted boundaries

		double deltaPhi = fabs( phi->at(0) - phi->at(1) ); // delta phi between two leading jets.
		if (deltaPhi < deltaPhiCut) continue;              // consider only events that the two leading are back-to-back.

		int ybin;

		if (useAbsoluteEta) ybin = getBin( fabs(eta->at(0)) ,yBnd, eta_bins); // get ybin of leading jet 
		else ybin = getBin( eta->at(0) ,yBnd, eta_bins);


		bool jetID_both;
		if ( JetIDlepVeto->at(0) && JetIDlepVeto->at(1) )  jetID_both = true ;
		else jetID_both = false;
		int jetIDint = BoolToInt(jetID_both);

/*
		h_nVtx[jetIDint][ybin]->Fill(nVtx);               
		h_MetSig[jetIDint][ybin]->Fill(metSig);
		h_ETA[jetIDint]->Fill(eta->at(0));
		h_PHI[jetIDint][ybin]->Fill(phi->at(0));
		h_pT[jetIDint][ybin]->Fill(jpt->at(0));
		h_CHF[jetIDint][ybin]->Fill(chf->at(0));
		h_NHF[jetIDint][ybin]->Fill(nhf->at(0));
		h_Elf[jetIDint][ybin]->Fill(elf->at(0));
		h_Phf[jetIDint][ybin]->Fill(phf->at(0));
		h_MUF[jetIDint][ybin]->Fill(muf->at(0));
		h_CEMF[jetIDint][ybin]->Fill(cemf->at(0));
		h_NEMF[jetIDint][ybin]->Fill(nemf->at(0));
		h_ChM[jetIDint][ybin]->Fill(chMult->at(0));
		if(isCHS) h_NeutM[jetIDint][ybin]->Fill(neMult->at(0));
		else h_NeutM[jetIDint][ybin]->Fill(neMultPuppi->at(0));
		if(isCHS)  h_PhM[jetIDint][ybin]->Fill(phoMult->at(0));
		else       h_PhM[jetIDint][ybin]->Fill(phoMultPuppi->at(0));
*/

		h_nVtx[0][ybin]->Fill(nVtx);               
		h_MetSig[0][ybin]->Fill(metSig);
		h_ETA[0]->Fill(eta->at(0));
		h_PHI[0][ybin]->Fill(phi->at(0));
		h_pT[0][ybin]->Fill(jpt->at(0));
		h_CHF[0][ybin]->Fill(chf->at(0));
		h_NHF[0][ybin]->Fill(nhf->at(0));
		h_Elf[0][ybin]->Fill(elf->at(0));
		h_Phf[0][ybin]->Fill(phf->at(0));
		h_MUF[0][ybin]->Fill(muf->at(0));
		h_CEMF[0][ybin]->Fill(cemf->at(0));
		h_NEMF[0][ybin]->Fill(nemf->at(0));
		h_ChM[0][ybin]->Fill(chMult->at(0));
		if(isCHS) h_NeutM[0][ybin]->Fill(neMult->at(0));
		else h_NeutM[0][ybin]->Fill(neMultPuppi->at(0));
		if(isCHS)  h_PhM[0][ybin]->Fill(phoMult->at(0));
		else       h_PhM[0][ybin]->Fill(phoMultPuppi->at(0));


		if(jetID_both)
		{
			h_nVtx[1][ybin]->Fill(nVtx);               
			h_MetSig[1][ybin]->Fill(metSig);
			h_ETA[1]->Fill(eta->at(0));
			h_PHI[1][ybin]->Fill(phi->at(0));
			h_pT[1][ybin]->Fill(jpt->at(0));
			h_CHF[1][ybin]->Fill(chf->at(0));
			h_NHF[1][ybin]->Fill(nhf->at(0));
			h_Elf[1][ybin]->Fill(elf->at(0));
			h_Phf[1][ybin]->Fill(phf->at(0));
			h_MUF[1][ybin]->Fill(muf->at(0));
			h_CEMF[1][ybin]->Fill(cemf->at(0));
			h_NEMF[1][ybin]->Fill(nemf->at(0));
			h_ChM[1][ybin]->Fill(chMult->at(0));
			if(isCHS) h_NeutM[1][ybin]->Fill(neMult->at(0));
			else h_NeutM[1][ybin]->Fill(neMultPuppi->at(0));
			if(isCHS)  h_PhM[1][ybin]->Fill(phoMult->at(0));
			else       h_PhM[1][ybin]->Fill(phoMultPuppi->at(0));
		}

	//	cout << no_jets << endl;
		int nJets_JetID[eta_bins] = {}; // initialize with zeros
		int nJets[eta_bins] = {};// initialize with zeros
	//	if(i < 1000 ) cout << " jentry = " << i << endl; 
		for(int j=0; j<no_jets; ++j)
		{
//			cout <<  " jet " << j << "jet pT " << jpt->at(j) << endl;
			if ( !(jpt->at(j) > pTmin  && jpt->at(j) < pTmax) ) continue;  
//			cout <<  " here  " << endl;

			if (  !useAbsoluteEta && !( eta->at(j) > yBnd[0] && eta->at(j)<yBnd[eta_bins])) continue;     // skip jets with eta out of wanted boundaries
			else if ( useAbsoluteEta && !(  fabs(eta->at(j)) > yBnd[0] && fabs(eta->at(j))<yBnd[eta_bins] ) ) continue; // skip jets |eta| of wanted boundaries


			if (useAbsoluteEta) ybin = getBin( fabs(eta->at(j)) ,yBnd, eta_bins); // get ybin of jet 
			else ybin = getBin( eta->at(j) ,yBnd, eta_bins);

			if (JetIDlepVeto->at(0)) nJets_JetID[ybin] = nJets_JetID[ybin]+1; // count number of jets in each eta bin in events that the two leading passed JetID
			nJets[ybin] = nJets[ybin] +1;                				     // count number of jets in each eta bin

		} // end of jet loop

		for (int h =0; h<eta_bins; h++)
		{

			h_nJets[0][h]->Fill(nJets[h]);
			h_nJets[1][h]->Fill(nJets_JetID[h]);
		}
	} // end of event loop

	char filename[200];
	sprintf(filename,"%s/%s.root",output_directory,out_name);

	TFile *fout = new TFile(filename,"recreate");
	fout->cd();
	for (int jetID=0;  jetID<2; jetID++)
	{
		h_ETA[jetID]->Write();
		for(Int_t ybin=0; ybin<eta_bins;ybin++)
		{

			h_pT[jetID][ybin]->Write();
			h_CHF[jetID][ybin]->Write();
			h_NHF[jetID][ybin]->Write();
			h_Elf[jetID][ybin]->Write();
			h_Phf[jetID][ybin]->Write();
			h_MUF[jetID][ybin]->Write();
			h_CEMF[jetID][ybin]->Write();
			h_NEMF[jetID][ybin]->Write();
			h_ChM[jetID][ybin]->Write();
			h_NeutM[jetID][ybin]->Write();
			h_PhM[jetID][ybin]->Write();
			h_nVtx[jetID][ybin]->Write();
			h_PHI[jetID][ybin]->Write();
			h_nJets[jetID][ybin]->Write();
			h_MetSig[jetID][ybin]->Write();

		}

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

