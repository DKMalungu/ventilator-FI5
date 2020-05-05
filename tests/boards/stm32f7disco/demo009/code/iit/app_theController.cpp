
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/


// --------------------------------------------------------------------------------------------------------------------
// - public interface
// --------------------------------------------------------------------------------------------------------------------

#include "app_theController.h"



// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

    #if 0
    inputs.S1_Pi = 0.F;         // S1 Pressure [code] (float)
    inputs.S2_Qi = 0.F;         // S2 Flow Rate [code] (float)
    inputs.S3_i[0] = 0.F;       // S3 Pressure [code] (float)
    inputs.S3_i[1] = 0.F;       // S3 Temperature [code] (float)
    inputs.S4_Pi = 0.F;         // S4 Pressure [code] (float)
    inputs.S5_Qi = 0.F;         // S5 Flow Rate [code] (float)
    inputs.S7_Pi = 0.F;         // S7 Pressure [code] (float)
        // the others are FS
    inputs.params[0] = 50.F;    // Zephyr FS [SLPM] (float)
    inputs.params[1] = 160.F;   // HSC LP FS [mbar] (float)
    inputs.params[2] = 10000.F; // HSC HP FS [mbar] (float)       
    #endif
    
    bool app::fillCTRLinput(const board_sensor_data_t &sd, app::Input &in)
    {
        in[0] = sd.pressure2_raw;       // S1 Pressure [code] (float)
        in[1] = sd.flow1_raw;           // S2 Flow Rate [code] (float)
        in[2] = sd.pressure4_raw;       // S3 Pressure [code] (float)
        in[3] = sd.temperature4_raw;    // S3 Temperature [code] (float)
        in[4] = sd.pressure3_raw;       // S4 Pressure [code] (float)
        in[5] = sd.flow2_raw;           // S5 Flow Rate [code] (float)
        in[6] = sd.pressure1_raw;       // S7 Pressure [code] (float)
        
        return true;
    }

// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------

    
// - begin of placeholder 
//   for putting the interface towards the matlab generated code.
//   we need some .h files and a static object. maybe some macros to load one controller or another at compiletime

#include "controller.h"                     
#include "rtwtypes.h"
#include "zero_crossing_types.h"

static controllerModelClass controller_Obj; 



static void matlab_init()
{
    controller_Obj.initialize();
}

static void matlab_mode(int m)
{
    const controllerModelClass::P_controller_T& parameters = controller_Obj.getBlockParameters();
    controllerModelClass::P_controller_T p = parameters;
    //p.Controller_control_mode = 1; // set IDLE
    p.Controller_control_mode = m; // set IDLE
    controller_Obj.setBlockParameters(&p); 
}

//static void matlab_feed(const app::Input &inp)
//{
////    controllerModelClass::ExtU_controller_T inputs;
////    inputs.enable = true;
////    inputs.IV_Vdot_fbk = 0.F;
////    inputs.IV_P_fbk = (float)inp[0]; // assign the IV_P_fbk as input of the controller 
////    inputs.params[0] = 0.F;
////    inputs.params[1] = 160.F;
////    controller_Obj.setExternalInputs(&inputs);   

//    controllerModelClass::ExtU_controller_T inputs;
//    inputs.enable = true;
//    inputs.S1_Pi = inp[0];         // S1 Pressure [code] (float)
//    inputs.S2_Qi = inp[1];         // S2 Flow Rate [code] (float)
//    inputs.S3_i[0] = inp[2];       // S3 Pressure [code] (float)
//    inputs.S3_i[1] = inp[3];       // S3 Temperature [code] (float)
//    inputs.S4_Pi = inp[4];         // S4 Pressure [code] (float)
//    inputs.S5_Qi = inp[5];         // S5 Flow Rate [code] (float)
//    inputs.S7_Pi = inp[6];         // S7 Pressure [code] (float)
//    inputs.params[0] = 50.F;    // Zephyr FS [SLPM] (float)
//    inputs.params[1] = 160.F;   // HSC LP FS [mbar] (float)
//    inputs.params[2] = 10000.F; // HSC HP FS [mbar] (float)
//    controller_Obj.setExternalInputs(&inputs);   
//}


static void matlab_feed(const app::theController::Inp &inp)
{
//    controllerModelClass::ExtU_controller_T inputs;
//    inputs.enable = true;
//    inputs.IV_Vdot_fbk = 0.F;
//    inputs.IV_P_fbk = (float)inp[0]; // assign the IV_P_fbk as input of the controller 
//    inputs.params[0] = 0.F;
//    inputs.params[1] = 160.F;
//    controller_Obj.setExternalInputs(&inputs);   

    controllerModelClass::ExtU_controller_T inputs;
    inputs.enable = true;
    inputs.S1_Pi = inp.S1pressure;         // S1 Pressure [code] (float)
    inputs.S2_Qi = inp.S2flowrate;         // S2 Flow Rate [code] (float)
    inputs.S3_i[0] = inp.S3pressure;       // S3 Pressure [code] (float)
    inputs.S3_i[1] = inp.S3temperature;       // S3 Temperature [code] (float)
    inputs.S4_Pi = inp.S4pressure;         // S4 Pressure [code] (float)
    inputs.S5_Qi = inp.S5flowrate;         // S5 Flow Rate [code] (float)
    inputs.S7_Pi = inp.S7pressure;         // S7 Pressure [code] (float)
    inputs.params[0] = 50.F;    // Zephyr FS [SLPM] (float)
    inputs.params[1] = 160.F;   // HSC LP FS [mbar] (float)
    inputs.params[2] = 10000.F; // HSC HP FS [mbar] (float)
    controller_Obj.setExternalInputs(&inputs);   
}

static void matlab_tick()
{
    controller_Obj.step();
}

//static void matlab_get(app::Output &out)
//{
////    const controllerModelClass::ExtY_controller_T& outputs = controller_Obj.getExternalOutputs();
////    out[0] = outputs.IV_dc;
////    out[1] = outputs.IV_P_ref;
////    out[2] = outputs.signals[1];
////    out[3] = outputs.signals[2]; // P_hsc_filt(i)       

//    const controllerModelClass::ExtY_controller_T& outputs = controller_Obj.getExternalOutputs();
//    out[0] = outputs.IV_dc;              // CP valve DC command [%] (float)
//    out[1] = outputs.OV_cmd;             // CFB valve ON/OFF command [-] (boolean)
//    out[2] = outputs.Q_ref;              // Target Flow Rate [L/min] (float)
//    out[3] = outputs.P_ref;              // Target Pressure [cmH2O] (float)
//    out[4] = outputs.signals[0];         // S1 Filtered Pressure [cmH2O] (float)
//    out[5] = outputs.signals[1];         // S2 Flow Rate [L/min] (float)
//    out[6] = outputs.signals[2];         // S3 Filtered Pressure [cmH2O] (float)
//    out[7] = outputs.signals[3];         // S3 Temperature [�C] (float)
//    out[8] = outputs.signals[4];         // S4 Filtered Pressure [cmH2O] (float)
//    out[9] = outputs.signals[5];         // S5 Flow Rate [L/min] (float)
//    out[10] = outputs.signals[6];         // S7 Filtered Pressure [cmH2O] (float)
//    out[11] = outputs.signals[7];         // Tidal Volume estimates [L] (float)
//    out[12] = outputs.signals[8];         // Assisted Ventilation Trigger [-] (float)   
//}


static void matlab_get(app::theController::Out &out)
{
//    const controllerModelClass::ExtY_controller_T& outputs = controller_Obj.getExternalOutputs();
//    out[0] = outputs.IV_dc;
//    out[1] = outputs.IV_P_ref;
//    out[2] = outputs.signals[1];
//    out[3] = outputs.signals[2]; // P_hsc_filt(i)       

    const controllerModelClass::ExtY_controller_T& outputs = controller_Obj.getExternalOutputs();
    out.CPvalvePerc = outputs.IV_dc;              // CP valve DC command [%] (float)
    out.CFBvalveON = outputs.OV_cmd;             // CFB valve ON/OFF command [-] (boolean)
    out.targetFlowRate = outputs.Q_ref;              // Target Flow Rate [L/min] (float)
    out.targetPressure = outputs.P_ref;              // Target Pressure [cmH2O] (float)
    out.filtS1pressure = outputs.signals[0];         // S1 Filtered Pressure [cmH2O] (float)
    out.S2flowrate = outputs.signals[1];         // S2 Flow Rate [L/min] (float)
    out.filtS3pressure = outputs.signals[2];         // S3 Filtered Pressure [cmH2O] (float)
    out.S3temperature = outputs.signals[3];         // S3 Temperature [�C] (float)
    out.filtS4pressure = outputs.signals[4];         // S4 Filtered Pressure [cmH2O] (float)
    out.S5flowrate = outputs.signals[5];         // S5 Flow Rate [L/min] (float)
    out.filtS7pressure = outputs.signals[6];         // S7 Filtered Pressure [cmH2O] (float)
    out.estTidalVolume = outputs.signals[7];         // Tidal Volume estimates [L] (float)
    out.assistedVentilTrigger = outputs.signals[8];         // Assisted Ventilation Trigger [-] (float)   
}

// - end of placeholder
    
struct app::theController::Impl
{      
    size_t _counter {0};

    Config _config {};
//    Input _input {0};
//    Output _output {0};
    
    theController::Out _out {};
    theController::Inp _inp {};  
        
        
    Impl()
    { 
         
    }
    
    bool init(const Config &cfg)
    {
        _config = cfg;
             
        matlab_init();
        
        set(Mode::IDLE);
        
        return true;       
    }

//    bool set(const Input &inp)
//    {
//        _input = inp;
//        matlab_feed(_input);
//        
//        return true;
//    }
    
    bool set(const board_sensor_data_t &sd)
    {
       _inp.load(sd);
       matlab_feed(_inp);
       return true; 
    }
    
    bool set(Mode mode)
    {
        matlab_mode(vnt::core::tointegral(mode));
        return true;        
    }
    
    bool tick()
    {
        matlab_tick();
        matlab_get(_out);
                
        _counter++;
        return true;
    }

//    const Output& get() const
//    {
//       return _output; 
//    }    

    const Out& getout() const
    {
       return _out; 
    }    

    const Inp& getinp() const
    {
       return _inp; 
    }       
};



// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------


namespace app {

    theController& theController::getInstance()
    {
        static theController* p = new theController();
        return *p;
    }
    
    theController::theController()
    {
        pImpl = std::make_unique<Impl>();
    }  
    
    
    theController::~theController() { }
   

    bool theController::init(const Config &cfg)
    {
        return pImpl->init(cfg);
    } 
    
//    bool theController::set(const Input &inp)
//    {
//        return pImpl->set(inp);
//    }
    
    bool theController::set(const board_sensor_data_t &sd)
    {
        return pImpl->set(sd);
    }

    
    bool theController::set(Mode mode)
    {
        return pImpl->set(mode);
    }
    
    bool theController::tick()
    {
        return pImpl->tick();
    }

//    const Output& theController::get() const
//    {
//        return pImpl->get();
//    }
    
    const theController::Out& theController::getOut() const
    {
        return pImpl->getout();
    }

    const theController::Inp& theController::getInp() const
    {
        return pImpl->getinp();
    }    

    

} // namespace app {

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

