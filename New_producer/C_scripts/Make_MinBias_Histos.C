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
bool getJetID(int era, bool isUL, bool isCHS, bool LepVeto, double eta, double cm, double nm, double nemf, double cemf, double nhf, double chf, double muf );
void Make_MinBias_Histos()
{


	char rep_path[200] = {"/afs/cern.ch/user/d/dkarasav/public/testing_JetID/"}; 

//	TFile *f = new TFile("/eos/cms/store/user/dkarasav/Test/MinimumBias__Run2017D-09Aug2019_reduced_tree_1.root","READ");

	char input_directory[200] = { "/eos/cms/store/group/phys_jetmet/dimitris/UL2016_MinBias_nonAPV_reduced_PUPPI/"};
//	char input_directory[200] = { "/eos/cms/store/user/dkarasav/MinBias_UL17_RunD_Puppi_trees/"};

//	char output_directory[200] = {"UL2018/MinBias/RunC/"};
	char output_directory[200] = {"UL2016/PUPPI/nonAPV/testing_35/"};

	char out_name[200] = {"RunGH_nemf0p6"};


	bool useAbsoluteEta = true;
	bool recalcJetID = true;

	int era = 2016;

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
	double yBnd[] = {0.0, 0.5, 1.0, 1.5, 2.0, 2.4, 2.7, 3.0, 5.0};    

//	double yBnd[] = {-5.0, -3.0, -2.7, 0.0, 2.7, 3.0, 5.0};    
//	double yBnd[] = {-5.0, -3.0, -2.7, 0.0, 2.7, 3.0, 5.0};    



	double pTmin = 15;
	double pTmax = 50000;
	int N_histo_bins = 25;
	bool passFilterGoodVtx;


//	TTree *tree;
//	tree = (TTree*)f->Get("tree"); // get the tree from the file


	int nJets, lumi, event, run, nVtx; 
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


//	tree->SetBranchAddress("era",&era);
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

	if(!recalcJetID)	tree->SetBranchAddress("JetID",&JetID); 
	if(!recalcJetID)	tree->SetBranchAddress("JetIDlepVeto",&JetIDlepVeto); 
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

	TH1D *h_pT[2][eta_bins], *h_CHF[2][eta_bins], *h_NHF[2][eta_bins], *h_Elf[2][eta_bins], *h_Phf[2][eta_bins], *h_MUF[2][eta_bins], *h_CEMF[2][eta_bins], *h_NEMF[2][eta_bins], *h_ChM[2][eta_bins], *h_NeutM[2][eta_bins], *h_PhM[2][eta_bins], *h_nVtx[2][eta_bins], *h_MetSig[2][eta_bins], *h_GoodVtx[2][eta_bins], *h_ETA[2], *h_PHI[2][eta_bins], *h_nJets[2][eta_bins], *h_nJets_asymmetry[2][eta_bins];

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
			h_nJets[jetID][h] = new TH1D(name,"",70, 0, 70);
		}
	}




	int nentries = tree->GetEntries();
	cout << "Number of total entries = " << nentries << endl;   
	
	//bool event_JetID = false;
	for (int i=0; i<nentries; i++) //event loop
//	for (int i=2629248; i<2629250; i++) //event loop
	{
		if(i < 10 || i%1000000 == 0) cout << " jentry = " << i << endl; 
//		if(i < 10 || i%2 == 0) cout << " jentry = " << i << endl;     
		tree->GetEntry(i);
		//bool event_JetID = false;
		if( passFilterGoodVtx ) continue; // for noise jets we want only events with passFilterGoodVtx==0.
		int no_jets = jpt->size();
	//	cout << no_jets << endl;
		int nJets_JetID[eta_bins] = {}; // initialize with zeros
		int nJets_noJetID[eta_bins] = {};// initialize with zeros
		for(int j=0; j<no_jets; ++j)
		{

			bool JetID_new = false;
			int ybin;
			if ( !(jpt->at(j) > pTmin && jpt->at(j)<pTmax) ) continue;   //pT cuts

			if (  !useAbsoluteEta && !( eta->at(j) > yBnd[0] && eta->at(j)<yBnd[eta_bins])) continue;     // skip jets with eta out of wanted boundaries
			else if ( useAbsoluteEta && !(  fabs(eta->at(j)) > yBnd[0] && fabs(eta->at(j))<yBnd[eta_bins]   ) ) continue; // skip jets with |eta| of wanted boundaries

			if (useAbsoluteEta) ybin = getBin( fabs(eta->at(j)) ,yBnd, eta_bins);
			else ybin = getBin( eta->at(j) ,yBnd, eta_bins);
		//	int jetIDint = BoolToInt(JetIDlepVeto->at(j));
		//	cout << jetIDint << endl;
			
			if (recalcJetID && isCHS ) JetID_new = getJetID(era, isUL, isCHS, true, eta->at(j) , chMult->at(j), neMult->at(j), nemf->at(j), cemf->at(j), nhf->at(j), chf->at(j), muf->at(j) );
			else if (recalcJetID && !isCHS ) JetID_new = getJetID(era, isUL, isCHS, true, eta->at(j) , chMult->at(j), neMultPuppi->at(j), nemf->at(j), cemf->at(j), nhf->at(j), chf->at(j), muf->at(j) );
			
			if ( (!recalcJetID && JetIDlepVeto->at(j) ) || ( recalcJetID && JetID_new ) ) nJets_JetID[ybin] = nJets_JetID[ybin]+1; // count number of jets in each eta bin passing JetID
			nJets_noJetID[ybin] = nJets_noJetID[ybin] +1;                     // count number of jets in each eta bin


			h_ETA[0]->Fill(eta->at(j));
			h_PHI[0][ybin]->Fill(phi->at(j));
			h_pT[0][ybin]->Fill(jpt->at(j));
			h_CHF[0][ybin]->Fill(chf->at(j));
			h_NHF[0][ybin]->Fill(nhf->at(j));
			h_Elf[0][ybin]->Fill(elf->at(j));
			h_Phf[0][ybin]->Fill(phf->at(j));
			h_MUF[0][ybin]->Fill(muf->at(j));
			h_CEMF[0][ybin]->Fill(cemf->at(j));
			h_NEMF[0][ybin]->Fill(nemf->at(j));
			h_ChM[0][ybin]->Fill(chMult->at(j));
			if(isCHS) h_NeutM[0][ybin]->Fill(neMult->at(j));
			else h_NeutM[0][ybin]->Fill(neMultPuppi->at(j));
			if(isCHS)  h_PhM[0][ybin]->Fill(phoMult->at(j));
			else       h_PhM[0][ybin]->Fill(phoMultPuppi->at(j));

			if(j==0) h_nVtx[0][ybin]->Fill(nVtx);               
			if(j==0) h_MetSig[0][ybin]->Fill(metSig);



			if( ( !recalcJetID && JetIDlepVeto->at(j) ) || (recalcJetID && JetID_new) )
			{
				h_ETA[1]->Fill(eta->at(j));
				h_PHI[1][ybin]->Fill(phi->at(j));
				h_pT[1][ybin]->Fill(jpt->at(j));
				h_CHF[1][ybin]->Fill(chf->at(j));
				h_NHF[1][ybin]->Fill(nhf->at(j));
				h_Elf[1][ybin]->Fill(elf->at(j));
				h_Phf[1][ybin]->Fill(phf->at(j));
				h_MUF[1][ybin]->Fill(muf->at(j));
				h_CEMF[1][ybin]->Fill(cemf->at(j));
				h_NEMF[1][ybin]->Fill(nemf->at(j));
				h_ChM[1][ybin]->Fill(chMult->at(j));
				if(isCHS) h_NeutM[1][ybin]->Fill(neMult->at(j));
				else h_NeutM[1][ybin]->Fill(neMultPuppi->at(j));
				if(isCHS)  h_PhM[1][ybin]->Fill(phoMult->at(j));
				else       h_PhM[1][ybin]->Fill(phoMultPuppi->at(j));
				if(j==0) h_nVtx[1][ybin]->Fill(nVtx);               
				if(j==0) h_MetSig[1][ybin]->Fill(metSig);
			}

			



		} // end of jet loop

		for (int h =0; h<eta_bins; h++)
		{
		//	cout << " hi " << endl;
		//	cout << "\n\n nJets with = " << nJets_JetID[h] << endl;
		//	cout << " nJets      = " << nJets_noJetID[h] << endl;
			h_nJets[0][h]->Fill(nJets_noJetID[h]);
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

bool getJetID(int era, bool isUL, bool isCHS, bool LepVeto, double eta, double cm, double nm, double nemf, double cemf, double nhf, double chf, double muf )
{
	bool tight, extracut_26_27, extracut_27_3, extracut_3_5, lepton_cut, JetID;

	if (isUL)
	{
		if ( isCHS )
		{
			if ( era == 2016 ) 
			{
				tight = ( fabs(eta)<=2.4 && cm>0 && nemf<0.98 && chf>0 && nhf < 0.9 && ( cm + nm)>1   );
				extracut_26_27 = ( fabs(eta)> 2.4 && fabs(eta)<=2.7 && nemf<0.99 && nhf<0.9 );
				extracut_27_3 = ( fabs(eta)> 2.7 && fabs(eta)<=3.0 && nemf>0.0 && nemf < 0.99 && nm> 1 );  
				extracut_3_5 = ( fabs(eta)> 3.0 && fabs(eta)<=5.0 && nemf< 0.9 && nhf>0.2 && nm>10 ); 
				lepton_cut =  fabs(eta)>2.4 || ( muf<0.8 && cemf<0.8 && fabs(eta)<=2.4 );

				if (LepVeto) JetID = ( tight || extracut_26_27 || extracut_27_3 || extracut_3_5) && lepton_cut;
				else JetID =  tight || extracut_26_27 || extracut_27_3 || extracut_3_5 ;
			}
			else  
			{
				//if (era != 2018) cout << "\nWARNING! era "<< era <<" is not valid. Will use era 2018. Valid era's are the integers: 2016, 2017, 2018. " << endl;

				tight = ( fabs(eta)<=2.6 && cm>0 && nemf<0.9 && chf>0 && nhf < 0.9 && ( cm + nm)>1   );
				extracut_26_27 = ( fabs(eta)> 2.6 && fabs(eta)<=2.7 && cm>0 && nemf<0.99 && nhf<0.9 );
				extracut_27_3 = ( fabs(eta)> 2.7 && fabs(eta)<=3.0 && nemf>0.01 && nemf < 0.99 && nm> 1 );  
				extracut_3_5 = ( fabs(eta)> 3.0 && fabs(eta)<=5.0 && nemf< 0.9 && nhf>0.2 && nm>10 ); 
				lepton_cut =  fabs(eta)>2.7 || ( muf<0.8 && cemf<0.8 && fabs(eta)<=2.7 );

				if (LepVeto) JetID = ( tight || extracut_26_27 || extracut_27_3 || extracut_3_5) && lepton_cut;
				else JetID =  tight || extracut_26_27 || extracut_27_3 || extracut_3_5 ;
			}
		}//end of CHS jets if

		if ( isCHS == false)
		{
			if ( era == 2016) 
			{
				tight = ( fabs(eta)<=2.4 && cm>0 && nemf<0.9 && chf>0 && nhf < 0.9 && ( cm + nm)>1   );
				//tight = ( fabs(eta)<=2.4 && nemf<0.9 && nhf < 0.9 && ( cm + nm)>1   );
				extracut_26_27 = ( fabs(eta)> 2.4 && fabs(eta)<=2.7 && nemf<0.99 && nhf<0.98 );
				extracut_27_3 = ( fabs(eta)> 2.7 && fabs(eta)<=3.0 && nm>=1. );  
//				extracut_27_3 = ( fabs(eta)> 2.7 && fabs(eta)<=3.0 && nhf<0.99999 );  
				extracut_3_5 = ( fabs(eta)> 3.0 && fabs(eta)<=5.0 && nemf< 0.6 && nm>1 ); 
				lepton_cut =  fabs(eta)>2.4 || ( muf<0.8 && cemf<0.8 && fabs(eta)<=2.4 );

				if (LepVeto) JetID = ( tight || extracut_26_27 || extracut_27_3 || extracut_3_5) && lepton_cut;
				else JetID =  tight || extracut_26_27 || extracut_27_3 || extracut_3_5 ;
			}
			else 
			{
				if (era != 2018 && era!= 2017 ) cout << "\nWARNING! era "<< era <<" is not valid. Will use era 2018. Valid era's are the integers: 2016, 2017, 2018. " << endl;

				tight = ( fabs(eta)<=2.6 && cm>0 && nemf<0.9 && chf>0 && nhf < 0.9 && ( cm + nm)>1   );
				extracut_26_27 = ( fabs(eta)> 2.6 && fabs(eta)<=2.7 && nemf<0.99 && nhf<0.9 );
				extracut_27_3 = ( fabs(eta)> 2.7 && fabs(eta)<=3.0 && nhf<0.9999 );  
				extracut_3_5 = ( fabs(eta)> 3.0 && fabs(eta)<=5.0 && nemf< 0.9 && nm>2 );  

				lepton_cut =  fabs(eta)>2.7 || ( muf<0.8 && cemf<0.8 && fabs(eta)<=2.7 );

				if (LepVeto) JetID = ( tight || extracut_26_27 || extracut_27_3 || extracut_3_5) && lepton_cut ;
				else JetID =  tight || extracut_26_27 || extracut_27_3 || extracut_3_5 ;
			} 
		} //end of Puppi jets if
	} //end of UL if

	else // for non UL, rereco datasets.
	{
		if ( isCHS )
		{
			if ( era == 2016 ) 
			{
				bool tight_24_27, lepton_cut_24_27, lepton_tot;

				tight = ( fabs(eta)<=2.4 && cm>0 && nemf<0.9 && chf>0 && nhf < 0.9 && (cm + nm)>1 && cemf<0.99 );
				tight_24_27 = ( fabs(eta)>2.4 && fabs(eta)<=2.7 && nemf<0.9 && nhf < 0.9 && ( cm + nm )>1 ) ;
				extracut_27_3 = ( fabs(eta)> 2.7 && fabs(eta)<=3.0 && nhf <0.98 && nemf>0.01 && nm>2 );  
				extracut_3_5 =  ( fabs(eta)> 3.0 && fabs(eta)<=5.0 && nemf< 0.9 && nm > 10 ); 
				lepton_cut =  fabs(eta)>2.7  || ( muf<0.8 && cemf < 0.9 && fabs(eta)<=2.4);
				lepton_cut_24_27 =  fabs(eta)>2.7 || (muf<0.8 && fabs(eta)>2.4 && fabs(eta )<=2.7) ;
				lepton_tot = lepton_cut || lepton_cut_24_27;

				if (LepVeto) JetID = ( tight || tight_24_27 || extracut_27_3 || extracut_3_5 ) && lepton_tot ;
				else JetID = tight || tight_24_27 || extracut_27_3 || extracut_3_5;
			}
			else if( era == 2017 )
			{

				tight = ( fabs(eta)<=2.7 && cm>0 && nemf<0.9 && chf>0 && nhf < 0.9 &&   (cm + nm)>1   );
				extracut_27_3 = ( fabs(eta)> 2.7 && fabs(eta)<=3.0 && nemf>0.02 && nemf< 0.99 && nm>2	);  
				extracut_3_5 = ( fabs(eta)> 3.0 && fabs(eta)<=5.0 && nemf< 0.9 && nhf>0.02 && nm>10 ); 
				lepton_cut =  fabs(eta)>2.7 || (muf<0.8 && cemf<0.8 && fabs(eta)<2.7);

				if (LepVeto) JetID = ( tight ||  extracut_27_3 || extracut_3_5) && lepton_cut ;
				else  JetID =  tight ||  extracut_27_3 || extracut_3_5;
			}
			else  
			{
				if (era != 2018) cout << "\nWARNING! era "<< era <<" is not valid. Will use era 2018. Valid era's are the integers: 2016, 2017, 2018. " << endl;

				tight = ( fabs(eta)<=2.6 && cm>0 && nemf<0.9 && chf>0 && nhf < 0.9 && ( cm + nm)>1   );
				extracut_26_27 = ( fabs(eta)> 2.6 && fabs(eta)<=2.7 && cm>0 && nemf<0.99 && nhf<0.9 );
				extracut_27_3 = ( fabs(eta)> 2.7 && fabs(eta)<=3.0 && nemf>0.02 && nemf < 0.99 && nm> 2 );  
				extracut_3_5 = ( fabs(eta)> 3.0 && fabs(eta)<=5.0 && nemf< 0.9 && nhf>0.2 && nm>10 ); 
				lepton_cut =  fabs(eta)>2.7 || ( muf<0.8 && cemf<0.8 && fabs(eta)<=2.7 );

				if (LepVeto) JetID = ( tight || extracut_26_27 || extracut_27_3 || extracut_3_5) && lepton_cut;
				else JetID =  tight || extracut_26_27 || extracut_27_3 || extracut_3_5 ;
			}
		}//end of CHS jets if

		if ( isCHS == false)
		{
			if ( era == 2016) 
			{
				bool tight_24_27, lepton_cut_24_27, lepton_tot;

				tight = ( fabs(eta)<=2.4 && cm>0 && nemf<0.9 && chf>0 && nhf < 0.9 && (cm + nm)>1 && cemf<0.99 );
				tight_24_27 = ( fabs(eta)>2.4 && fabs(eta)<=2.7 && nemf<0.9 && nhf < 0.9 && ( cm + nm )>1 ) ;
				extracut_27_3 = ( fabs(eta)> 2.7 && fabs(eta)<=3.0 && nhf <0.98 && nemf>0.01 && nm>2 );  
				extracut_3_5 =  ( fabs(eta)> 3.0 && fabs(eta)<=5.0 && nemf< 0.9 && nm > 10 ); 
				lepton_cut =  fabs(eta)>2.7  || ( muf<0.8 && cemf < 0.9 && fabs(eta)<=2.4);
				lepton_cut_24_27 =  fabs(eta)>2.7 || (muf<0.8 && fabs(eta)>2.4 && fabs(eta )<=2.7) ;
				lepton_tot = lepton_cut || lepton_cut_24_27;

				if (LepVeto) JetID = ( tight || tight_24_27 || extracut_27_3 || extracut_3_5 ) && lepton_tot ;
				else JetID = tight || tight_24_27 || extracut_27_3 || extracut_3_5;
			}
			else if( era == 2017 )
			{

				tight = (fabs(eta)<=2.7 && cm>0 && nemf<0.9 && chf>0 && nhf < 0.9 &&   (cm + nm)>1   );
				extracut_27_3 = (fabs(eta)> 2.7 && fabs(eta)<=3.0 && nhf<0.99);  
				extracut_3_5 = (fabs(eta)> 3.0 && fabs(eta)<=5.0 && nemf< 0.9 && nhf>0.02 && nm>2 && nm<15); 
				lepton_cut =  fabs(eta)>2.7 || (muf<0.8 && cemf<0.8 && fabs(eta)<2.7);

				if (LepVeto) JetID = ( tight ||  extracut_27_3 || extracut_3_5) && lepton_cut ;
				else  JetID =  tight ||  extracut_27_3 || extracut_3_5;
			}
			else 
			{
				if (era != 2018) cout << "\nWARNING! era "<< era <<" is not valid. Will use era 2018. Valid era's are the integers: 2016, 2017, 2018. " << endl;

				tight = ( fabs(eta)<=2.6 && cm>0 && nemf<0.9 && chf>0 && nhf < 0.9 && ( cm + nm)>1   );
				extracut_26_27 = ( fabs(eta)> 2.6 && fabs(eta)<=2.7 && nemf<0.99 && nhf<0.9 );
				extracut_27_3 = ( fabs(eta)> 2.7 && fabs(eta)<=3.0 && nhf< 0.99 );  
				extracut_3_5 = ( fabs(eta)> 3.0 && fabs(eta)<=5.0 && nemf< 0.9 && nhf>0.02 && nm>2 && nm<15);  
				lepton_cut =  fabs(eta)>2.7 || ( muf<0.8 && cemf<0.8 && fabs(eta)<=2.7 );

				if (LepVeto) JetID = ( tight || extracut_26_27 || extracut_27_3 || extracut_3_5) && lepton_cut ;
				else JetID =  tight || extracut_26_27 || extracut_27_3 || extracut_3_5 ;
			} 
		} //end of Puppi jets if


	}
return JetID;
}


