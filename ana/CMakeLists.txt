set(ANALYSIS "AP_bbZDM_Lep_R2")

target_sources(RunAnalysis PRIVATE ${ANALYSIS}/ana/ORDER.cpp ${ANALYSIS}/ana/ONNX.cpp ${ANALYSIS}/ana/Test.cpp)
