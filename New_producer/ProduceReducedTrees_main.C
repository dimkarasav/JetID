#include <vector>
#ifdef __MAKECINT__
#pragma link C++ class std::vector<std::vector<float> >+;
#pragma link C++ class std::vector<std::vector<int> >+;
#endif

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TFrame.h"
#include "TH1F.h"
#include "TBenchmark.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TF1.h"
#include "TROOT.h"
#include "TGraphAsymmErrors.h"

#include <iostream>
#include<fstream>
#include <unistd.h>  
#include <getopt.h>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atoi */

//#define eta_bins 4

using namespace std;

bool getJetID(int era, bool isUL, bool isCHS, bool LepVeto, double eta, double cm, double nm, double nemf, double cemf, double nhf, double chf, double muf );

void ProduceReducedTrees(int era, bool isUL, bool isCHS, char name[150], char dir[500], char listPath[1000], int listIndex );

//int main(int era, bool isUL, bool isCHS, char name[150], char dir[500], char listPath[500], int listIndex)
int main(int argc, char *argv[])
{


	int opt;
	char era_c[20], listIndex_c[20];
	char isUL_c[20], isCHS_c[20];
	char name[500], dir[500], listPath[1000];



    while((opt = getopt(argc, argv, "y:t:u:n:d:l:i:")) != -1)  
        switch(opt)
        {
            case 'd': sprintf(dir,"%s", optarg); cout << "output directory = "<< optarg <<endl ; break;
            case 'y': sprintf(era_c,"%s",optarg); cout <<" era : "<< optarg <<endl; break;
            case 't': sprintf(isCHS_c,"%s", optarg); cout <<" isCHS:  "<< optarg <<endl; break;
			case 'u': sprintf(isUL_c,"%s", optarg); cout <<" isUL:  "<< optarg <<endl; break;
            case 'n': sprintf(name,"%s", optarg); cout <<" output name = "<< optarg <<endl; break;
            case 'l': sprintf(listPath,"%s", optarg); cout << "list used = "<< optarg <<endl ; break;
            case 'i': sprintf(listIndex_c,"%s", optarg); cout << "list index = "<< optarg <<endl ; break;
            case '?': fprintf(stderr, "usuage is \n -y 2017 -t true -u true -n test -d deleteme/ -l lists/UL_2017_MinBias/MinimumBias__Run2017D-09Aug2019 -i 1 ");
            default: cout<<endl; abort();
        }


	int era, listIndex; 
	bool isUL, isCHS;

	//era = era_c;
	era = atoi (era_c);

//	listIndex = listIndex_c;
	listIndex = atoi (listIndex_c);

//	cout << isCHS_c <<"   " << isUL_c << endl;



	if(strcmp(isCHS_c,"true")==0 ) isCHS = true;
	else isCHS = false;

	if(strcmp(isUL_c,"true")==0 ) isUL = true;
	else isUL = false;


	ProduceReducedTrees(era, isUL, isCHS, name, dir, listPath, listIndex );

	return 0;
}


void ProduceReducedTrees(int era, bool isUL, bool isCHS, char name[150], char dir[500], char listPath[1000], int listIndex )
{

//	cout << "\n\n Entered main script" << endl;

	cout << " era = " << era << endl;
	cout << " isUL = " << isUL << endl;
	cout << " isCHS = " << isCHS << endl;
	cout << " name = " << name << endl;
	cout << " dir = " << dir << endl;
	cout << " listPath = " << listPath << endl;
	cout << " listIndex = " << listIndex << endl;




	TChain * fChain = new TChain("dijets/events");

	char FullListPath[1000];
//	sprintf(FullListPath,"%s_%d.txt",listPath,listIndex);
	sprintf(FullListPath,"%s",listPath);

	cout << "\n Reading from list : " << FullListPath << endl;

	cout << "1 " << endl;
	ifstream myReadFile;
	cout << "2 " << endl;
	myReadFile.open(FullListPath);
	cout << "3 " << endl;
	char FilesFromList[1000];
	if (myReadFile.is_open())
	{
	cout << "4 " << endl;
		while (!myReadFile.eof())
		{

			myReadFile >> FilesFromList;
			fChain->Add(FilesFromList);
			cout<<"\nAdding file "<< FilesFromList << endl;

		}
	}
	cout << "5 " << endl;
	myReadFile.close();




/*
	TChain * fChain = new TChain("dijets/events");
	fChain->Add("/eos/cms/store/group/phys_jetmet/magda/MinBiasFlat_UL2017/MinimumBias/crab_MinimumBias__Run2017D-09Aug2019_UL2017-v1__MINIAOD/191202_114544//0000/MinimumBias__Run2017D-09Aug2019_UL2017-v1__MINIAOD_27.root");
*/

	// Declaration of leaf types

	double pTCut = 15;

	int  runNo;
	int  evtNo;
	int  lumi;
	int  nvtx;
	float rho;
	float met;
	float metSig;
	int  nJets;
	bool passFilterHBHE;
	bool passFilterHBHEIso;
	bool passFilterglobalSuperTightHalo2016;
	bool passFilterECALDeadCell;
	bool passFilterGoodVtx;
	bool passFilterEEBadSc;
	bool passFilterBadChargedCandidate;
	bool passFilterBadPFMuon;


	std::vector<bool> *triggerResult = 0;

	

	std::vector<float>  *jetPt = 0;
	std::vector<float>  *jetEta = 0;
	std::vector<float>  *jetPhi = 0;
	std::vector<float>  *jetChf = 0;
	std::vector<float>  *jetNhf = 0;
	std::vector<float>  *jetPhf = 0;
	std::vector<float>  *jetMuf = 0;
	std::vector<float>  *jetElf = 0;
	std::vector<float>  *jetNemf = 0;
	std::vector<float>  *jetCemf = 0;
	std::vector<int>  *chMult = 0;
	std::vector<int>  *neMult = 0;
	std::vector<int>  *phoMult = 0;
	std::vector<float>  *neMultPuppi = 0;
	std::vector<float>  *phoMultPuppi = 0;


	fChain->SetBranchAddress("runNo", &runNo);
	fChain->SetBranchAddress("evtNo", &evtNo);
	fChain->SetBranchAddress("lumi", &lumi);
	fChain->SetBranchAddress("nvtx", &nvtx);
	fChain->SetBranchAddress("rho", &rho);
	fChain->SetBranchAddress("met", &met);
	fChain->SetBranchAddress("metSig", &metSig);
	fChain->SetBranchAddress("triggerResult", &triggerResult);
	fChain->SetBranchAddress("passFilterHBHE", &passFilterHBHE);
	fChain->SetBranchAddress("passFilterHBHEIso", &passFilterHBHEIso);
	fChain->SetBranchAddress("passFilterglobalSuperTightHalo2016", &passFilterglobalSuperTightHalo2016);
	fChain->SetBranchAddress("passFilterECALDeadCell", &passFilterECALDeadCell);
	fChain->SetBranchAddress("passFilterGoodVtx", &passFilterGoodVtx);
	fChain->SetBranchAddress("passFilterEEBadSc", &passFilterEEBadSc);
	fChain->SetBranchAddress("passFilterBadChargedCandidate", &passFilterBadChargedCandidate);
	fChain->SetBranchAddress("passFilterBadPFMuon", &passFilterBadPFMuon);

	if (isCHS)
	{
		fChain->SetBranchAddress("nJetsAK4", &nJets);
		fChain->SetBranchAddress("jetPtAK4", &jetPt);
		fChain->SetBranchAddress("jetEtaAK4", &jetEta);
		fChain->SetBranchAddress("jetPhiAK4", &jetPhi);
		fChain->SetBranchAddress("jetChfAK4", &jetChf);
		fChain->SetBranchAddress("jetNhfAK4", &jetNhf);
		fChain->SetBranchAddress("jetPhfAK4", &jetPhf);
		fChain->SetBranchAddress("jetMufAK4", &jetMuf);
		fChain->SetBranchAddress("jetElfAK4", &jetElf);
		fChain->SetBranchAddress("jetNemfAK4", &jetNemf);
		fChain->SetBranchAddress("jetCemfAK4", &jetCemf);
		fChain->SetBranchAddress("chMultAK4", &chMult);
		fChain->SetBranchAddress("neMultAK4", &neMult);
		fChain->SetBranchAddress("phoMultAK4", &phoMult);
	}
	else 
	{
		fChain->SetBranchAddress("nJetsAK8", &nJets);
		fChain->SetBranchAddress("jetPtAK8", &jetPt);
		fChain->SetBranchAddress("jetEtaAK8", &jetEta);
		fChain->SetBranchAddress("jetPhiAK8", &jetPhi);
		fChain->SetBranchAddress("jetChfAK8", &jetChf);
		fChain->SetBranchAddress("jetNhfAK8", &jetNhf);
		fChain->SetBranchAddress("jetPhfAK8", &jetPhf);
		fChain->SetBranchAddress("jetMufAK8", &jetMuf);
		fChain->SetBranchAddress("jetElfAK8", &jetElf);
		fChain->SetBranchAddress("jetNemfAK8", &jetNemf);
		fChain->SetBranchAddress("jetCemfAK8", &jetCemf);
		fChain->SetBranchAddress("chMultAK8", &chMult);
		fChain->SetBranchAddress("neMultAK8", &neMultPuppi);
		fChain->SetBranchAddress("phoMultAK8", &phoMultPuppi);
	}
	

	std::vector<bool> *JetID = 0;
	std::vector<bool> *JetIDlepVeto = 0;


	std::vector<float>  *Stored_jetPt = 0;
	std::vector<float>  *Stored_jetEta = 0;
	std::vector<float>  *Stored_jetPhi = 0;
	std::vector<float>  *Stored_jetChf = 0;
	std::vector<float>  *Stored_jetNhf = 0;
	std::vector<float>  *Stored_jetPhf = 0;
	std::vector<float>  *Stored_jetMuf = 0;
	std::vector<float>  *Stored_jetElf = 0;
	std::vector<float>  *Stored_jetNemf = 0;
	std::vector<float>  *Stored_jetCemf = 0;
	std::vector<int>    *Stored_chMult = 0;
	std::vector<int>    *Stored_neMult = 0;
	std::vector<int>    *Stored_phoMult = 0;
	std::vector<float>  *Stored_neMultPuppi = 0;
	std::vector<float>  *Stored_phoMultPuppi = 0;


	bool passHLT, passHLT1, passHLT2, passHLT3, passHLT4;

	TTree *tree = new TTree("tree","");


    tree->Branch("nJets",&nJets,"nJets/I");
    tree->Branch("run",&runNo,"run/I");
    tree->Branch("event",&evtNo,"event/I");
    tree->Branch("lumi",&lumi,"lumi/I");
    tree->Branch("nVtx",&nvtx,"nVtx/I");
    tree->Branch("met",&met,"met/F");
    tree->Branch("metSig",&metSig,"metSig/F");
    tree->Branch("isCHS",&isCHS,"isCHS/O");
    tree->Branch("isUL",&isUL,"isUL/O");
    tree->Branch("era",&era,"era/I");
    tree->Branch("passHLT",&passHLT,"passHLT/O");
    tree->Branch("passHLT1",&passHLT1,"passHLT1/O");
    tree->Branch("passHLT2",&passHLT2,"passHLT2/O");
    tree->Branch("passHLT3",&passHLT3,"passHLT3/O");
    tree->Branch("passHLT4",&passHLT4,"passHLT4/O");
	tree->Branch("passFilterHBHE", &passFilterHBHE,"passFilterHBHE/O");
	tree->Branch("passFilterHBHEIso", &passFilterHBHEIso,"passFilterHBHEIso/O");
	tree->Branch("passFilterglobalSuperTightHalo2016", &passFilterglobalSuperTightHalo2016,"passFilterglobalSuperTightHalo2016/O");
	tree->Branch("passFilterECALDeadCell", &passFilterECALDeadCell,"passFilterECALDeadCell/O");
	tree->Branch("passFilterGoodVtx", &passFilterGoodVtx,"passFilterGoodVtx/O");
	tree->Branch("passFilterEEBadSc", &passFilterEEBadSc,"passFilterEEBadSc/O");
	tree->Branch("passFilterBadChargedCandidate", &passFilterBadChargedCandidate,"passFilterBadChargedCandidate/O");
	tree->Branch("passFilterBadPFMuon", &passFilterBadPFMuon,"passFilterBadPFMuon/O");

	tree->Branch("jetPt"  , "std::vector<float>", &Stored_jetPt  );		
	tree->Branch("jetEta" , "std::vector<float>", &Stored_jetEta );		
	tree->Branch("jetPhi" , "std::vector<float>", &Stored_jetPhi );		
	tree->Branch("jetChf" , "std::vector<float>", &Stored_jetChf );		
	tree->Branch("jetNhf" , "std::vector<float>", &Stored_jetNhf );	
	tree->Branch("jetPhf" , "std::vector<float>", &Stored_jetPhf );
	tree->Branch("jetMuf" , "std::vector<float>", &Stored_jetMuf );
	tree->Branch("jetElf" , "std::vector<float>", &Stored_jetElf );												
	tree->Branch("jetCemf", "std::vector<float>", &Stored_jetCemf );	
	tree->Branch("jetNemf", "std::vector<float>", &Stored_jetNemf );			
	tree->Branch("chMult" , "std::vector<int>"  , &Stored_chMult );	


	if (isCHS) tree->Branch("neMult" , "std::vector<int>", &Stored_neMult );	
	else tree->Branch("neMult" , "std::vector<float>", &Stored_neMultPuppi );	
	if (isCHS)	tree->Branch("phoMult", "std::vector<int>", &Stored_phoMult );	
	else tree->Branch("phoMult", "std::vector<float>", &Stored_phoMultPuppi );	

	tree->Branch("JetID"  , "std::vector<bool>" , &JetID);		
	tree->Branch("JetIDlepVeto","std::vector<bool>",&JetIDlepVeto);										


	bool JetID_, JetIDlepVeto_; 

	int nentries = fChain->GetEntries(); 
	cout << " Total number of entries for the chain : " << nentries << endl;
	for (int jentry=0; jentry<nentries; jentry++)
//	for (int jentry=0; jentry<100000; jentry++)
	{

		fChain->GetEntry(jentry);
		if(jentry < 10 || jentry%10000 == 0) std::cout << "Event entry = " << jentry << std::endl;   
		
	//	if(jentry < 100 ) cout << met << "  <-met  metSig-> "<< metSig<<  endl;   
		

		int no_jets = jetPt->size();
		

//		if (no_jets < 1 || passFilterGoodVtx == true) continue; // if no jets in event skip the event
		if (no_jets < 1) continue; // if no jets in event skip the event

		passHLT  = triggerResult->at(0);
		passHLT1 = triggerResult->at(1);
		passHLT2 = triggerResult->at(2);
		passHLT3 = triggerResult->at(3);
		passHLT4 = triggerResult->at(4);

		for (int ijet=0; ijet<no_jets; ijet++)
		{

			if ( jetPt->at(ijet) < pTCut) continue;  // if jet pT < pTcut , skip the jet 

			Stored_jetPt->push_back(  jetPt->at(ijet) );
			Stored_jetEta->push_back( jetEta->at(ijet) );
			Stored_jetPhi->push_back( jetPhi->at(ijet) );
			Stored_jetChf->push_back( jetChf->at(ijet) );
			Stored_jetNhf->push_back( jetNhf->at(ijet) );
			Stored_jetPhf->push_back( jetPhf->at(ijet) );
			Stored_jetMuf->push_back( jetMuf->at(ijet) );
			Stored_jetElf->push_back( jetElf->at(ijet) );
			Stored_jetCemf->push_back( jetCemf->at(ijet) );
			Stored_jetNemf->push_back( jetNemf->at(ijet) );
			Stored_chMult->push_back( chMult->at(ijet) );

			if(isCHS )
			{
				Stored_neMult->push_back( neMult->at(ijet) );
				Stored_phoMult->push_back( phoMult->at(ijet) );
			}
			else
			{
				Stored_neMultPuppi->push_back( neMultPuppi->at(ijet) );
				Stored_phoMultPuppi->push_back( phoMultPuppi->at(ijet) );
			}


			double NM;
			
			if (isCHS) NM = neMult->at(ijet);
			else NM = neMultPuppi->at(ijet);  
			 
		
			JetID_ = getJetID( era, isUL, isCHS, false, jetEta->at(ijet), chMult->at(ijet), NM, jetNemf->at(ijet), jetCemf->at(ijet), jetNhf->at(ijet), jetChf->at(ijet), jetMuf->at(ijet));
			JetIDlepVeto_ = getJetID( era, isUL, isCHS, false, jetEta->at(ijet), chMult->at(ijet), NM , jetNemf->at(ijet), jetCemf->at(ijet), jetNhf->at(ijet), jetChf->at(ijet), jetMuf->at(ijet));

			JetID->push_back(JetID_);
			JetIDlepVeto->push_back(JetIDlepVeto_);
		} //end of jet loop

		
		if( Stored_jetPt->size() > 0 )  tree->Fill();  // fill the output tree only if at least one jet passed the pT cut per event


		JetID->clear();
		JetIDlepVeto->clear();
		Stored_jetPt->clear();
		Stored_jetEta->clear();
		Stored_jetPhi->clear();
		Stored_jetChf->clear();
		Stored_jetNhf->clear();
		Stored_jetPhf->clear();
		Stored_jetMuf->clear();
		Stored_jetElf->clear();
		Stored_jetCemf->clear();
		Stored_jetNemf->clear();
		Stored_chMult->clear();
		if(isCHS)Stored_neMult->clear();
		if(isCHS)Stored_phoMult->clear();
		if(!isCHS)Stored_neMultPuppi->clear();
		if(!isCHS)Stored_phoMultPuppi->clear();


	} // end of event loop

	char outputPath[2000];

	sprintf(outputPath, "%s/%s_reduced_tree_%d.root",dir,name,listIndex );




	TFile *fout = new TFile (outputPath,"recreate");
	fout->cd();
	tree->Write();
//	fout->Close();
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
				extracut_27_3 = ( fabs(eta)> 2.7 && fabs(eta)<=3.0 && nemf>0.01 && nemf < 0.99 && nm> 2 );  
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

