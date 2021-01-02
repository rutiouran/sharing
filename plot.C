{
    const int nIncidents = 5000000;
    const float thickness = 0.1; //cm
    const float Nv = 4.6e22; // 1/cm3

    vector<float> inEnergyVec;
    vector<float> xsVec;

    for(int iFile = 10; iFile <= 100; iFile += 10)
    {
    	string fileName("run_example");
    	fileName = fileName + to_string(iFile) + ".root";
    	TFile* f = new TFile(fileName.c_str());
    
    	TTree* ntuple = (TTree*)f->Get("output");

	double edep;

	ntuple->SetBranchAddress("Ehwt", &edep);

	TH1F* histogram = new TH1F("h", "h", 100, 0, 1000);

	for(int entry = 0; entry < ntuple->GetEntries(); ++entry)
	{
		ntuple->GetEntry( entry );
		histogram->Fill( edep );
	}

	std::cout << "mean: " <<  histogram->GetMean() << std::endl;

    	//inEnergyVec.push_back( en );
    	//xsVec.push_back( xs );
    
    }
    
#if 0
    // vector2array
    float* inEnergyArray = &inEnergyVec[0];
    float* xsArray = &xsVec[0];
    
    TGraph *gr = new TGraph(counter, inEnergyArray, xsArray);

    gr->SetLineColor(2);
    gr->SetLineWidth(4);
    gr->SetMarkerColor(4);
    gr->SetMarkerStyle(26);
    gr->SetTitle("D(d, n)^{3}He Yield");
    gr->GetXaxis()->SetTitle("deuteron energy (MeV)");
    gr->GetYaxis()->SetTitle("Yield)");
    gr->Draw("ACP");
#endif
}
