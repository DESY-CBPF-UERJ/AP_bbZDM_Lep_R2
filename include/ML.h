#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <stdlib.h>
#include <TStyle.h>
#include <TH2D.h>
#include <TLegend.h>
#include <TColor.h>
#include <math.h>
#include <THnSparse.h>
#include <map>
#include <string>
#include <vector>
#include <random>
#include "THnSparse.h"
#include "TF1.h"
#include "TSystem.h"
#include "TLorentzVector.h"
#include "TGraphAsymmErrors.h"
#include <iomanip>
#include <sys/stat.h>
#include <time.h>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidcsv.h"
#include <fdeep/fdeep.hpp>
#include <fdeep/model.hpp>
#include <torch/torch.h>
#include <torch/script.h>
#include "onnxruntime_cxx_api.h"

using namespace std;


//-------------------------------------------------------------------------
// NN Model (Keras)
//-------------------------------------------------------------------------
class NN_Keras {
    private:
        fdeep::model NN_model = fdeep::load_model("AP_bbZDM_Lep_R2/include/keras_initial_model.json"); // It will return error
        rapidjson::Document preprocessing;
    public:
        void readFiles( string model_file, string preprocessing_file ){
            NN_model = fdeep::load_model(model_file.c_str());
            FILE *fp_proc = fopen(preprocessing_file.c_str(), "r"); 
            char buf_proc[0XFFFF];
            rapidjson::FileReadStream stat_values(fp_proc, buf_proc, sizeof(buf_proc));
            preprocessing.ParseStream(stat_values);
        }
        
        float predict( vector<float> mlp_input, bool binary=true ){
            //vector<float> mlp_input{LeadingLep_pt, LepLep_pt, LepLep_deltaR, LepLep_deltaM, MET_pt, MET_LepLep_Mt, MET_LepLep_deltaPhi};
            rapidjson::Value& pp_mean = preprocessing["mean"];
            rapidjson::Value& pp_std = preprocessing["std"];
            for (int ivar = 0; ivar < mlp_input.size(); ivar++) {
                float mean_i = pp_mean[ivar].GetFloat();
                float std_i = pp_std[ivar].GetFloat();
                mlp_input[ivar] = (mlp_input[ivar] - mean_i)/std_i;
            }
            vector<fdeep::internal::tensor, allocator<fdeep::internal::tensor> > result = NN_model.predict({fdeep::tensor(fdeep::tensor_shape(static_cast<size_t>(mlp_input.size())), mlp_input)});
            const fdeep::float_vec& prediction = *result[0].as_vector();
            
            if( binary ){ 
                return 1-prediction[0];
            }else{
                return prediction[0];
            }
        }
};


//-------------------------------------------------------------------------
// NN Model (Torch)
//-------------------------------------------------------------------------
class NN_Torch {
    private:
        torch::jit::script::Module NN_model;
    public:
        void readFile( string model_file ){
            NN_model = torch::jit::load(model_file.c_str());
            torch::NoGradGuard no_grad; // ensures that autograd is off
            NN_model.eval();
        }
        
        float predict( vector<float> mlp_input, bool binary=true  ){
            std::vector<torch::jit::IValue> inputs;
            inputs.push_back(torch::tensor(mlp_input));
            at::Tensor result = NN_model.forward(inputs).toTensor();
            float prediction = result[0].item<float>();
            
            if( binary ){ 
                return 1-prediction;
            }else{
                return prediction;
            }
        }
};


//-------------------------------------------------------------------------
// ONNX Model
//-------------------------------------------------------------------------
class ONNX_MODEL {
    private:
        Ort::Session session{nullptr};
        Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu( OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);
        size_t numInputNodes;
        size_t numOutputNodes;

    public:
        void readFile( string model_file ){
            Ort::SessionOptions sessionOptions;
            sessionOptions.SetIntraOpNumThreads(1);
            sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
            Ort::Env env(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING, "onnx_model");
            Ort::Session session_tmp(env, model_file.c_str(), sessionOptions);
            session = std::move(session_tmp);
            numInputNodes = session.GetInputCount();
            numOutputNodes = session.GetOutputCount();
        }

        vector<float> predict( const char* const* inputNames, vector<vector<float>> inputTensorValues, vector<vector<int64_t>> inputTensorDims, const char* const* outputNames, vector<vector<float>> outputTensorValues, vector<vector<int64_t>> outputTensorDims ){

            vector<Ort::Value> inputTensors;
            for (size_t i = 0; i < numInputNodes; i++){
                inputTensors.push_back(Ort::Value::CreateTensor<float>( memoryInfo, inputTensorValues[i].data(), inputTensorValues[i].size(), inputTensorDims[i].data(), inputTensorDims[i].size()));
            }

            vector<Ort::Value> outputTensors;
            for (size_t i = 0; i < numOutputNodes; i++){
                outputTensors.push_back(Ort::Value::CreateTensor<float>( memoryInfo, outputTensorValues[i].data(), outputTensorValues[i].size(), outputTensorDims[i].data(), outputTensorDims[i].size()));
            }

            session.Run(Ort::RunOptions{nullptr}, inputNames, inputTensors.data(), numInputNodes,
                        outputNames, outputTensors.data(), numOutputNodes);

            vector<float> prediction;
            if( outputTensorValues.at(0).size() == 1 ){
                prediction.push_back(1-outputTensorValues.at(0).at(0));
            }else{
                for (size_t i = 0; i < numOutputNodes; i++){
                    prediction.push_back(outputTensorValues.at(0).at(i));
                }
            }

            return prediction;

        }
};
