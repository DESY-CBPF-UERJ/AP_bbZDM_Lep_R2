#include "HEPHero.h"


//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace TorchTest{

    float one;
    float zero;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupTorchTest() {

    //======SETUP CUTFLOW==========================================================================
    //_cutFlow.insert(pair<string,double>("CutName", 0) );   [example]

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    _outputTree->Branch("MLP_score_keras", &MLP_score_keras );
    _outputTree->Branch("MLP_score_torch", &MLP_score_torch );
    
    HDF_insert("MLP_score_keras", &MLP_score_keras );
    HDF_insert("MLP_score_torch", &MLP_score_torch );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::TorchTestRegion() {

    //-------------------------------------------------------------------------
    // Cut description
    //-------------------------------------------------------------------------
    //if( !(CutCondition) ) return false;           [Example]
    //_cutFlow.at("CutName") += evtWeight;          [Example]

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::TorchTestSelection() {
    
    Signal_discriminators();

    /*
    torch::Tensor tensor = torch::eye(3);
        
    TorchTest::one = tensor[0][0].item<float>();
    TorchTest::zero = tensor[0][1].item<double>();
    
    torch::jit::script::Module module;
    
    module = torch::jit::load("Metadata/ML/model_scripted.pt");
    torch::NoGradGuard no_grad; // ensures that autograd is off
    module.eval();
    
    std::vector<torch::jit::IValue> inputs;
    //inputs.push_back(torch::tensor({100., 100., 2., 10., 100., 120., 1.1}));
    inputs.push_back(torch::tensor({50., 50., 2., 2., 50., 100., 1.1}));

    at::Tensor output = module.forward(inputs).toTensor();

    float result0 = output[0].item<float>();
    //float result1 = output[1].item<float>();
    
    cout << "result = " << result0 << endl;
    //cout << "result = " << result1 << endl;
    */
    
    
    /*
    struct Net : torch::nn::Module {
  	Net(int64_t N, int64_t M) {
        W = register_parameter("W", torch::randn({N, M}));
        b = register_parameter("b", torch::randn(M));
  	}
  	torch::Tensor forward(torch::Tensor input) {
        return torch::addmm(b, input, W);
  	}
  	torch::Tensor W, b;
    };

    Net net(4, 5);
    
    for (const auto& p : net.parameters()) {
        std::cout << p << std::endl;
    }
    
    for (const auto& pair : net.named_parameters()) {
        std::cout << pair.key() << ": " << pair.value() << std::endl;
    }
    */






    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //TorchTest::variable1Name = 100;      [Example]

    //======FILL THE HISTOGRAMS====================================================================
    //_histograms1D.at("histogram1DName").Fill( var, evtWeight );               [Example]
    //_histograms2D.at("histogram2DName").Fill( var1, var2, evtWeight );        [Example]

    //======FILL THE OUTPUT TREE===================================================================
    _outputTree->Fill();
    HDF_fill();

    return;
}


//-------------------------------------------------------------------------------------------------
// Produce systematic histograms
//-------------------------------------------------------------------------------------------------
void HEPHero::TorchTestSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishTorchTest() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
