import torch

model = torch.jit.load('/home/gilson/Dropbox/HEP/HEPHero/AP_bbZDM_Lep_R2/Metadata/ML/Torch/DeepCSV/2017/model_scripted.pt')

input_example = torch.rand(1, 9)
output_example = model(input_example)
print(output_example)


torch.onnx.export(
    model,
    input_example,
    "model_torch_2017.onnx",
    input_names=["features"],
    output_names=["output"],
    dynamic_axes={
        "features": {0: 'N'},
        "output": {0: 'N'},
    },
)
