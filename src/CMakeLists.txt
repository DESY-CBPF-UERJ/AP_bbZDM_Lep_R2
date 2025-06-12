set(ANALYSIS "AP_bbZDM_Lep_R2")

target_sources(RunAnalysis PRIVATE ${ANALYSIS}/src/HEPHeroB.cpp ${ANALYSIS}/src/HEPRoutines.cpp ${ANALYSIS}/src/HEPWeights.cpp ${ANALYSIS}/src/HEPVerticalSystematics.cpp ${ANALYSIS}/src/EXPTools.cpp ${ANALYSIS}/src/ANATools.cpp ${ANALYSIS}/src/correction.cc ${ANALYSIS}/src/formula_ast.cc ${ANALYSIS}/src/RoccoR.cc ${ANALYSIS}/src/RecoilCorrector.cc)
