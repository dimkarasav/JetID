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

double error1(double n1,double n2);
double error2(double n1,double n2);
bool getJetID(int era, bool isUL, bool isCHS, bool LepVeto, double eta, double cm, double nm, double nemf, double cemf, double nhf, double chf, double muf );
bool FindProbeJetID (int irand, bool JetID_j1, bool JetID_j2 );
bool FindTagJetID (int irand, bool JetID_j1, bool JetID_j2 );
double FindProbeVariable(int irand, double var_j1,  double var_j2);
double FindTagVariable(int irand, double var_j1,  double var_j2);
int getBin(double x, double boundaries[], int bins) ;
TGraphAsymmErrors* GetEfficiencyGraph(TH1D* numerator, TH1D* denominator );


void Make_Tag_and_Probe_eff_graphs()
{


//	char rep_path[200] = {"/afs/cern.ch/user/d/dkarasav/public/testing_JetID/"}; 

//	char input_directory[200] = { "/eos/cms/store/user/dkarasav/MinBias_UL17_RunD_Puppi_trees/"};
	char input_directory[200] = { "/eos/cms/store/user/dkarasav/JetHT_UL17_RunD_Puppi_trees/"};

	char output_directory[200] = {"/afs/cern.ch/user/d/dkarasav/public/testing_JetID/UL2017/JetHT/RunD/"};
	char image_name[500] = {"btb_vs_noise_allRuns"}; 

	bool useAbsoluteEta = true;
	bool recalcJetID = true;


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



	double yBnd[] = {0.0, 0.5, 1.0, 1.5, 2.0, 2.6, 2.7, 3.0, 5.0};    

//	double yBnd[] = {-5.0, -3.0, -2.7, 0.0, 2.7, 3.0, 5.0};    

	double deltaPhiCut = 2.7;
	double pTcut_leading = 60;
	double pTcut_subleading = 30;
	double pTmin = 15;
	double pTmax = 50000;
	int N_histo_bins = 300;


//	TTree *tree;
//	tree = (TTree*)f->Get("tree"); // get the tree from the file


	int nJets, lumi, event, run, nVtx, era; 
	bool isCHS, isUL;
	float met, metSig;
	bool passHLT3,passHLT2; 

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

	tree->SetBranchAddress("passHLT2",&passHLT2); 
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
  
	TH1D *h_pT_probe_all[eta_bins], *h_pT_probe_JetID[eta_bins];  
	TH1D *h_pT_probe_all_HLT_PFJet400[eta_bins], *h_pT_probe_JetID_HLT_PFJet400[eta_bins]; 

	char name[150];
	for(Int_t h=0; h<eta_bins;h++)
	{
		sprintf(name,"h_pT_probe_all%i",h);
		h_pT_probe_all[h] = new TH1D(name, "", N_histo_bins,0,9000);
		sprintf(name,"h_pT_probe_JetID%i",h);
		h_pT_probe_JetID[h] = new TH1D(name, "", N_histo_bins,0,9000); 
		sprintf(name,"h_pT_probe_all_HLT_PFJet400%i",h);
		h_pT_probe_all_HLT_PFJet400[h] = new TH1D(name, "", N_histo_bins,0,9000);
		sprintf(name,"h_pT_probe_JetID_HLT_PFJet400%i",h);
		h_pT_probe_JetID_HLT_PFJet400[h] = new TH1D(name, "", N_histo_bins,0,9000);      
	}



//	TChain *tree = new TChain("tree");


//==========================================================UL17 ak4CHS================================================

//	tree->Add("/eos/cms/store/group/phys_jetmet/dimitris/UL2017_btb_CHS_JetID_reduced/RunB/JetHT__Run2017B-09Aug2019_UL2017-v1_reduced_skim.root");
//	tree->Add("/eos/cms/store/group/phys_jetmet/dimitris/UL2017_btb_CHS_JetID_reduced/RunC/JetHT__Run2017C-09Aug2019_UL2017-v1_reduced_skim.root");
//	tree->Add("/eos/cms/store/group/phys_jetmet/dimitris/UL2017_btb_CHS_JetID_reduced/RunD/JetHT__Run2017D-09Aug2019_UL2017-v1_reduced_skim.root");
//	tree->Add("/eos/cms/store/group/phys_jetmet/dimitris/UL2017_btb_CHS_JetID_reduced/RunE/JetHT__Run2017E-09Aug2019_UL2017-v1_reduced_skim.root");
//	tree->Add("/eos/cms/store/group/phys_jetmet/dimitris/UL2017_btb_CHS_JetID_reduced/RunF/JetHT__Run2017F-09Aug2019_UL2017-v1_reduced_skim.root");


//==========================================================UL17 ak4PUPPI================================================
/*
	tree->Add("/eos/cms/store/group/phys_jetmet/dimitris/UL2017_btb_PUPPI_JetID_reduced/RunB/JetHT__Run2017B-09Aug2019_UL2017-v1_reduced_skim.root");
	tree->Add("/eos/cms/store/group/phys_jetmet/dimitris/UL2017_btb_PUPPI_JetID_reduced/RunC/JetHT__Run2017C-09Aug2019_UL2017-v1_reduced_skim.root");
	tree->Add("/eos/cms/store/group/phys_jetmet/dimitris/UL2017_btb_PUPPI_JetID_reduced/RunD/JetHT__Run2017D-09Aug2019_UL2017-v1_reduced_skim.root");
	tree->Add("/eos/cms/store/group/phys_jetmet/dimitris/UL2017_btb_PUPPI_JetID_reduced/RunE/JetHT__Run2017E-09Aug2019_UL2017-v1_reduced_skim.root");
	tree->Add("/eos/cms/store/group/phys_jetmet/dimitris/UL2017_btb_PUPPI_JetID_reduced/RunF/JetHT__Run2017F-09Aug2019_UL2017-v1_reduced_skim.root");
*/

/*
	tree->Add("/eos/cms/store/group/phys_exotica/dijet/Dijet13TeV/dimitris/Puppi_ak8_reduced_trees/RunA/small_tree_RunA.root");
	tree->Add("/eos/cms/store/group/phys_exotica/dijet/Dijet13TeV/dimitris/Puppi_ak8_reduced_trees/RunB/small_tree_RunB.root");
	tree->Add("/eos/cms/store/group/phys_exotica/dijet/Dijet13TeV/dimitris/Puppi_ak8_reduced_trees/RunC/small_tree_RunC.root");
	tree->Add("/eos/cms/store/group/phys_exotica/dijet/Dijet13TeV/dimitris/Puppi_ak8_reduced_trees/RunD/small_tree_RunD.root");
*/


	Int_t nentries=(Int_t)tree->GetEntries(); 
	cout<<"Number of entries =  "<<nentries<<endl;
   	int irand;
	cout.precision(13);
	bool JetID_j1, JetID_j2, JetID_tag, JetID_probe;	
	double pT_tag, pT_probe, eta_probe, eta_tag, muf_tag, muf_probe, cemf_tag, cemf_probe;
	for (int i=0; i<nentries; i++) //event loop
//	for (int i=0; i<30000; i++) //event loop
	{    
	//	if(i>0 && i%1000==0) cout<<i<<endl;
		if(i>0 && i%500000==0) cout<<i<<endl;
		tree->GetEntry(i);

		tree->GetEntry(i);
		if(!(passHLT3 || passHLT2 )) continue; //use trigger
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

//		gRandom->SetSeed(i);

		irand = (gRandom->Uniform()>0.5) ? 0 : 1;  

		if (recalcJetID)
		{
			double nm_j1, nm_j2;
			if (isCHS)  {   nm_j1 = neMult->at(0);	    nm_j2 = neMult->at(1); }
			else 		{	nm_j1 = neMultPuppi->at(0);	nm_j2 = neMultPuppi->at(1);	} 
			JetID_j1 = getJetID(era, isUL, isCHS, true, eta->at(0), chMult->at(0), nm_j1, nemf->at(0), cemf->at(0), nhf->at(0), chf->at(0), muf->at(0) );
			JetID_j2 = getJetID(era, isUL, isCHS, true, eta->at(1), chMult->at(1), nm_j2, nemf->at(1), cemf->at(1), nhf->at(1), chf->at(1), muf->at(1) );
		}
		else
		{
			JetID_j1 = JetIDlepVeto->at(0);
			JetID_j2 = JetIDlepVeto->at(1);
		}


		pT_tag   = FindTagVariable(irand, jpt->at(0) , jpt->at(1)  );
		eta_tag  = FindTagVariable(irand, eta->at(0) , eta->at(1)  );
		cemf_tag = FindTagVariable(irand, cemf->at(0), cemf->at(1) );
		muf_tag  = FindTagVariable(irand, muf->at(0) , muf->at(1)  );
		JetID_tag = FindTagJetID(irand, JetID_j1, JetID_j2 );

		pT_probe   = FindProbeVariable(irand, jpt->at(0) , jpt->at(1)  );
		eta_probe  = FindProbeVariable(irand, eta->at(0) , eta->at(1)  );
		cemf_probe = FindProbeVariable(irand, cemf->at(0), cemf->at(1) );
		muf_probe  = FindProbeVariable(irand, muf->at(0) , muf->at(1)  );
		JetID_probe = FindProbeJetID(irand, JetID_j1, JetID_j2 );

		bool lepton_cut =  fabs(eta_probe)>2.7 || ( muf_probe<0.8 && cemf_probe < 0.8 && fabs(eta_probe)<2.7 );


		if(passHLT2==1 )
		{
			for (int eta_bin_counter=0; eta_bin_counter<eta_bins; eta_bin_counter++ )
			{
				//cout << " irand = " << irand << endl;
				if(fabs(eta_probe)>=yBnd[eta_bin_counter] && fabs(eta_probe)<yBnd[eta_bin_counter+1])
				{
					if(JetID_tag && lepton_cut) h_pT_probe_all[eta_bin_counter]->Fill(pT_probe);        //denominator for efficiency
					if(JetID_tag && JetID_probe && lepton_cut) h_pT_probe_JetID[eta_bin_counter]->Fill(pT_probe);  //numerator for efficiency
				}
			}
		}

		if(passHLT3==1 )
		{
			for (int eta_bin_counter=0; eta_bin_counter<eta_bins; eta_bin_counter++ )
			{
				if(fabs(eta_probe)>=yBnd[eta_bin_counter] && fabs(eta_probe)<yBnd[eta_bin_counter+1])
				{
					if(JetID_tag && lepton_cut) h_pT_probe_all_HLT_PFJet400[eta_bin_counter]->Fill(pT_probe);        //denominator for efficiency
					if(JetID_tag && JetID_probe && lepton_cut) h_pT_probe_JetID_HLT_PFJet400[eta_bin_counter]->Fill(pT_probe);  //numerator for efficiency
				}
			}
		}

	}//end of event loop

     
	TGraphAsymmErrors *Eff_passHLT2[eta_bins], *Eff_passHLT3[eta_bins]; 

	TCanvas *pad1 = new TCanvas("pad1", "",1);
	pad1->Divide(3,3);

	double high_trigger_numerator =0 , high_trigger_denominator =0 , low_trigger_numerator =0 , low_trigger_denominator=0 ;

	for(int eta_bin_counter=0; eta_bin_counter<eta_bins; eta_bin_counter++)
	{  
		cout << "Eta bin: " << "" << yBnd[eta_bin_counter] << "-"<< yBnd[eta_bin_counter+1] << endl;

		cout << "PFHT 1050 Eff= " << h_pT_probe_JetID[eta_bin_counter]->Integral() << "/" << h_pT_probe_all[eta_bin_counter]->Integral() << "= "<<h_pT_probe_JetID[eta_bin_counter]->Integral()/h_pT_probe_all[eta_bin_counter]->Integral() << endl;

		cout << "HLT_PFjet200 Eff= " << h_pT_probe_JetID_HLT_PFJet400[eta_bin_counter]->Integral() << "/" << h_pT_probe_all_HLT_PFJet400[eta_bin_counter]->Integral() << "= "<<h_pT_probe_JetID_HLT_PFJet400[eta_bin_counter]->Integral()/h_pT_probe_all_HLT_PFJet400[eta_bin_counter]->Integral() << endl;

		high_trigger_numerator = high_trigger_numerator + h_pT_probe_JetID[eta_bin_counter]->Integral();
		high_trigger_denominator = high_trigger_denominator + h_pT_probe_all[eta_bin_counter]->Integral();
		
		low_trigger_numerator = low_trigger_numerator + h_pT_probe_JetID_HLT_PFJet400[eta_bin_counter]->Integral();
		low_trigger_denominator = low_trigger_denominator + h_pT_probe_all_HLT_PFJet400[eta_bin_counter]->Integral();

		Eff_passHLT2[eta_bin_counter] = GetEfficiencyGraph(h_pT_probe_JetID[eta_bin_counter], h_pT_probe_all[eta_bin_counter] );
		Eff_passHLT3[eta_bin_counter] = GetEfficiencyGraph(h_pT_probe_JetID_HLT_PFJet400[eta_bin_counter], h_pT_probe_all_HLT_PFJet400[eta_bin_counter] );

	} //end of ybin

	cout << "PFHT1050 tot eff = " << high_trigger_numerator << "/" <<high_trigger_denominator <<" = "    << high_trigger_numerator / high_trigger_denominator << endl;
	cout << "HLT_ PFJet200 tot eff = " << low_trigger_numerator << "/" <<low_trigger_denominator <<" = " <<  low_trigger_numerator /  low_trigger_denominator << endl;
	cout <<"\n" <<endl;
	cout <<"\n" <<endl;


	char filename[256]; 
	sprintf(filename,"%s/effLepton_btb_2triggers_allEtaBins_%s.root", output_directory, image_name);
	TFile *fout = new TFile(filename,"recreate");
	fout->cd();
	pad1->Write();
	for(int iy=0;iy<eta_bins;iy++)
	{
		h_pT_probe_JetID[iy]->Write();
		h_pT_probe_all[iy]->Write();
		h_pT_probe_all_HLT_PFJet400[iy]->Write();
		h_pT_probe_JetID_HLT_PFJet400[iy]->Write();
		sprintf(name,"Eff_passHLT2_%i",iy);
		Eff_passHLT2[iy]->SetName(name);
		Eff_passHLT2[iy]->Write();
		sprintf(name,"Eff_passHLT3_%i",iy);
		Eff_passHLT3[iy]->SetName(name);
		Eff_passHLT3[iy]->Write();
	}
	
	fout->Close();

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

TGraphAsymmErrors* GetEfficiencyGraph(TH1D* numerator, TH1D* denominator )
{
	const int nbins = denominator->GetNbinsX(); 
	double eff_x[nbins], eff_y[nbins], eff_eyu[nbins], eff_eyl[nbins], eff_ex[nbins];

	for (int i=0; i<nbins; i++)
	{
		eff_x[i] = 0.0; eff_y[i] = 0.0; eff_eyu[i] = 0.0; eff_eyl[i] = 0.0; eff_ex[i] = 0.0;

		if( denominator->GetBinContent(i)>0 )
		{
			eff_x[i]   = denominator->GetBinCenter(i);
			eff_y[i]   = numerator->GetBinContent(i) / denominator->GetBinContent(i);
			eff_eyu[i] = error1(denominator->GetBinContent(i), numerator->GetBinContent(i) );
			eff_eyl[i] = error2(denominator->GetBinContent(i), numerator->GetBinContent(i) );
			eff_ex[i]  = denominator->GetBinWidth(i)/2;
		}
	}
	TGraphAsymmErrors *Eff = new TGraphAsymmErrors( nbins, eff_x, eff_y,eff_ex, eff_ex, eff_eyl, eff_eyu );	

	return Eff;
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

				tight = ( fabs(eta)<=2.6 && cm>0 && nemf<0.9 && chf>0 && nhf < 0.9 && ( cm + nm)>1   );
				extracut_26_27 = ( fabs(eta)> 2.6 && fabs(eta)<=2.7 && nemf<0.99 && nhf<0.9 );
				extracut_27_3 = ( fabs(eta)> 2.7 && fabs(eta)<=3.0 && nhf<1.0 );  
				extracut_3_5 = ( fabs(eta)> 3.0 && fabs(eta)<=5.0 && nemf< 0.9 && nhf>0.02 && nm>2 );  //for UL 17
				lepton_cut =  fabs(eta)>2.7 || ( muf<0.8 && cemf<0.8 && fabs(eta)<=2.7 );

				if (LepVeto) JetID = ( tight || extracut_26_27 || extracut_27_3 || extracut_3_5) && lepton_cut ;
				else JetID =  tight || extracut_26_27 || extracut_27_3 || extracut_3_5 ;
			}
			else 
			{
				if (era != 2018) cout << "\nWARNING! era "<< era <<" is not valid. Will use era 2018. Valid era's are the integers: 2016, 2017, 2018. " << endl;

				tight = ( fabs(eta)<=2.6 && cm>0 && nemf<0.9 && chf>0 && nhf < 0.9 && ( cm + nm)>1   );
				extracut_26_27 = ( fabs(eta)> 2.6 && fabs(eta)<=2.7 && nemf<0.99 && nhf<0.9 );
				extracut_27_3 = ( fabs(eta)> 2.7 && fabs(eta)<=3.0 && nhf<1.0 );  
				extracut_3_5 = ( fabs(eta)> 3.0 && fabs(eta)<=5.0 && nemf< 0.9 && nhf>0.02 && nm>2 );  
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



double FindTagVariable(int irand, double var_j1,  double var_j2)
{
	double var_tag;
	if ( irand < 0.5 ) var_tag = var_j1;
	else var_tag = var_j2;

	return var_tag;
}

double FindProbeVariable(int irand, double var_j1,  double var_j2)
{
	double var_probe;
	if ( irand < 0.5 ) var_probe = var_j2;
	else var_probe = var_j1;

	return var_probe;
}


bool FindTagJetID (int irand, bool JetID_j1, bool JetID_j2 )
{
	bool JetID_tag;
	if ( irand < 0.5 ) JetID_tag = JetID_j1;
	else JetID_tag = JetID_j2;

	return JetID_tag;
}


bool FindProbeJetID (int irand, bool JetID_j1, bool JetID_j2 )
{
	bool JetID_probe;
	if ( irand < 0.5 ) JetID_probe = JetID_j2;
	else JetID_probe = JetID_j1;

	return JetID_probe;
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





