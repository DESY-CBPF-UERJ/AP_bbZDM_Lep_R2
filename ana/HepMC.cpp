#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace HepMC{

    float part_pt; 
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupHepMC() {

    //======SETUP CUTFLOW==========================================================================
    //_cutFlow.insert(pair<string,double>("CutName", 0) );   [example]

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Define regions, same as in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    _outputTree->Branch("part_pt", &HepMC::part_pt ); 

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::HepMCRegion() {

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
void HEPHero::HepMCSelection() {


    class IsEventGood {
    public:
        /// check this event for goodness
        bool operator()( const HepMC::GenEvent* evt ) {
            std::cout << "Event " << evt->event_number() << std::endl;
            for ( HepMC::GenEvent::particle_const_iterator p = evt->particles_begin(); p != evt->particles_end(); ++p ){
            //if ( (*p)->pdg_id() == 22 && (*p)->momentum().perp() > 25. ) {
                if ( (*p)->momentum().perp() > 25. ) {   
                    std::cout << "Event " << evt->event_number() << " is a good event." << std::endl;
                    (*p)->print();
                    return 1;
                }
            }
        return 0;
        }
    };

    
    HepMC::IO_GenEvent ascii_in("ana/testHepMC.input",std::ios::in);
	// declare another IO_GenEvent for writing out the good events
	
    HepMC::IO_GenEvent ascii_out("ana/testHepMC.output",std::ios::out);
	// declare an instance of the event selection predicate
	IsEventGood is_good_event;
	//........................................EVENT LOOP
	int icount=0;
	int num_good_events=0;
	HepMC::GenEvent* evt = ascii_in.read_next_event();
	while ( evt ) {
        
        for ( HepMC::GenEvent::particle_const_iterator p = evt->particles_begin(); p != evt->particles_end(); ++p ){
            HepMC::part_pt = (*p)->momentum().perp();
            _outputTree->Fill();
        }
        
	    icount++;
	    //if ( icount%50==1 ) std::cout << "Processing Event Number " << icount << " its # " << evt->event_number() << std::endl;
	    if ( is_good_event(evt) ) {
            ascii_out << evt;
            ++num_good_events;
	    }
	    delete evt;
	    ascii_in >> evt;
	}
	//........................................PRINT RESULT
	std::cout << num_good_events << " out of " << icount  << " processed events passed the cuts. Finished." << std::endl;
    










    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //HepMC::variable1Name = 100;      [Example]

    //======FILL THE HISTOGRAMS====================================================================
    //_histograms1D.at("histogram1DName").Fill( var, evtWeight );               [Example]
    //_histograms2D.at("histogram2DName").Fill( var1, var2, evtWeight );        [Example]

    //======FILL THE OUTPUT TREE===================================================================
    //_outputTree->Fill();

    return;
}


//-------------------------------------------------------------------------------------------------
// Produce systematic histograms
//-------------------------------------------------------------------------------------------------
void HEPHero::HepMCSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishHepMC() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
