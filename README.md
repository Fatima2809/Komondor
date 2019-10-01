<p align="center"> 
<img src="https://github.com/wn-upf/Komondor/blob/master/Documentation/Other/Images and resources/komondor_logo.png">
</p>

# Komondor: An IEEE 802.11ax Simulator

## Table of Contents
- [Authors](#authors)
- [Introduction](#introduction)
- [Overview](#overview)
- [Usage](#usage)
- [Validation](#validation)
- [Contribute](#contribute)
- [Acknowledgements](#acknowledgements)

## Authors
* [Sergio Barrachina-Muñoz](https://github.com/sergiobarra)
* [Francesc Wilhelmi](https://github.com/fwilhelmi)

Both authors have the same contribution in this project.

## Introduction

Komondor is a wireless network simulator that includes novel mechanisms for next-generation WLANs, such as dynamic channel bonding or enhanced spatial reuse. One of the main purposes of Komondor is to simulate the behavior of IEEE 802.11ax-2019 networks, an amendment designed to boost spectral efficiency in dense deployments.
		
Komondor has been conceived as an open source tool that contributes to the ongoing research in wireless networks, especially regarding the implementation of novel functionalities that are not available in other well-known wireless simulators. In addition, it has been prepared for a simple integration with Machine Learning (ML) modules.

The project is structured as follows:
* ```Apps```: contains auxiliary applications that support the Komondor's core operation. For example, in Apps we find the "Input Generation" project, which contains a Java application that generates inputs for Komondor in an easy and flexible way.
* ```Code```: contains the core files to compile and run Komondor, including input and output folders. 
* ```Documentation```: contains Documentation related to Komondor, such as a Manual, a user's guide, presentations, etc.

Doxygen documentation available [here](http://htmlpreview.github.io/?https://github.com/wn-upf/Komondor/blob/master/Documentation/doxy/html/index.html).

## Overview

The code to run simulations is organized as follows:

* ```COST```: constitute the Komondor's primitive operation. Here we find the CompC++ library that allows generating discrete event simulations. For further information about COST, please refer to its main [website](http://www.ita.cs.rpi.edu/cost.html). 
* ```main```: contains the core files (komondor.cc, node.h, traffic_generator.h, agent.h and central_controller.h) that are in charge of orchestrating all the simulation. "komondor.cc" is the main component, which initializes all the other components of "Type II". All these modules are aware of the existence of the simulation time. In addition to the core components, here we find "build_local", a bash script that compiles the libraries for executing the code. Note that file "compcxx_komondor_main.h" is also required to carry out such a compilation.
* ```methods```: by following clean architecture guidelines, independent methods used by core files are contained in the methods folder. Several libraries are provided according to the nature of their functions. For instance, "backoff_methods.h" contains methods to handle the backoff operation in the Distributed Coordination Function (DCF).
* ```structures```: the Komondor simulator considers several header files to carry out its operation. Among them, we find "wlan.h", which defines the main characteristics of a WLAN (WLAN id, list of associated STAs, etc.). In addition, the "notification.h" structure allows to define the information to be exchanged between devices. 
* ```learning_modules```: here we find the implementation of ML methods that receive feedback about the networks performance in simulation time. 
* ```list_of_macros.h```: all the static parameters (e.g., constants) are contained in this file.
* ```input```: contains the input files that allow building the simulation environment.
* ```output```: contains the data generated by Komondor as a result of a given simulation.	
* ```scripts_multiple_executions```: contains bash scripts to perform multiple simulations.

An overview of the current modules available in Komondor is next shown:
<p align="center"> 
<img src="https://github.com/wn-upf/Komondor/blob/master/Documentation/Other/Images and resources/modules_overview.png">
</p>

## Usage

Detailed installation and execution instructions can be found in the [Komondor User's Guide](https://github.com/wn-upf/Komondor/blob/master/Documentation/User%20guide/LaTeX%20files/komondor_user_guide.pdf).

In short, to run Komondor, just build the project by using the "build_local" script and then execute it by following the next steps:

STEP 0: Set permissions to the folder

```
$ chmod -R 777 <dirname>
```

STEP 1: Build the project

```
$ ./build_local
```

STEP 2: Run Komondor simulator for the given input information (basic simulation)

```
$ ./komondor_main INPUT_FILE_SYSTEM_CONFIGURATION INPUT_FILE_NODES OUTPUT_FILE_LOGS FLAG_SAVE_SYSTEM_LOGS FLAG_SAVE_NODE_LOGS FLAG_PRINT_SYSTEM_LOGS FLAG_PRINT_NODE_LOGS SIM_TIME SEED
```

The inputs are further described next:
* ```INPUT_FILE_SYSTEM_CONFIGURATION```: file containing system information (e.g., number of channels available, traffic model, etc.). The file must be a .csv with semicolons as separators.
* ```INPUT_FILE_NODES```: file containing nodes information (e.g., position, channels allowed, etc.).The file must be a .csv with semicolons as separators.
* ```OUTPUT_FILE_LOGS```: path to the output file to which write results at the end of the execution (if the file does not exist, the system will create it).
* ```FLAG_SAVE_SYSTEM_LOGS```: flag to indicate whether to save the system logs into a file (1) or not (0).
* ```FLAG_SAVE_NODE_LOGS```: flag to indicate whether to save the nodes logs into separate files (1) or not (0). If this flag is activated, one file per node will be created.
* ```FLAG_PRINT_SYSTEM_LOGS```: flag to indicate whether to print the system logs (1) or not (0).
* ```FLAG_PRINT_NODE_LOGS```: flag to indicate whether to print the nodes logs (1) or not (0). 
* ```SIM_TIME```: simulation time
* ```SEED```: random seed the user wishes to use

IMPORTANT NOTE (!): Setting ```FLAG_SAVE_SYSTEM_LOGS``` and ```FLAG_SAVE_NODE_LOGS``` to TRUE (1) entails a larger execution time. 

STEP 2-1: Run Komondor simulator with intelligent agents

Alternatively, and in order to indicate the usage of agents, the console input must add the following extra information:

```
$ ./komondor_main INPUT_FILE_SYSTEM_CONFIGURATION INPUT_FILE_NODES INPUT_FILE_AGENTS OUTPUT_FILE_LOGS FLAG_SAVE_SYSTEM_LOGS FLAG_SAVE_NODE_LOGS FLAG_SAVE_AGENT_LOGS FLAG_PRINT_SYSTEM_LOGS FLAG_PRINT_NODE_LOGS FLAG_PRINT_AGENT_LOGS SIM_TIME SEED
```

The new inputs are described next:
* ```INPUT_FILE_AGENTS```: file containing agents information (e.g., wlan code, allowed actions, etc.).The file must be a .csv with semicolons as separators.
* ```FLAG_SAVE_AGENT_LOGS``` :flag to indicate whether to save the agent logs into separate files (1) or not (0). If this flag is activated, one file per agent will be created.
* ```FLAG_PRINT_AGENT_LOGS```: flag to indicate whether to print the agent logs (1) or not (0). 

### Input files

There are two types of input files that are required for basic Komondor's execution. These files are located at the "input" folder, and which allow to configure system and nodes parameters, respectively:
* ```input_system_conf.csv```: define parameters such as the number of total available channels, the CW...
* ```input_nodes_conf.csv```: define parameters such as the node id, the node location, etc.

Additionally, the agents operation is ruled by the agents input file. The most important inputs refer to:
1) WLAN code: a code that must match with WLAN codes provided in node input files
2) Time between requests: an agent is supposed to request data to the AP every inputted period
3) Allowed actions as lists: for each type of modifiable parameter, the user must introduce a list of possible values (e.g. CCA = {-70, -75, -80, -82})

Regarding the output ("output" folder), some logs and statistics are created at the end of the execution.

### Other installations

Debugging: 

```
$ apt-get install gdb valgrind 
```

## Validation
Komondor v.2.0 has been validated by means of ns-3 and SF-CTMN and Bianchi analytical models. The presentation of the validation can be found at "S. Barrachina-Muñoz, F. Wilhelmi, I. Selinis & B. Bellalta. Komondor: a Wireless Network Simulator for Next-Generation High-Density WLANs. 2018". Additional resources are available in this repository, in folder /Documentation/Validation. Files used for each simulation tool can be found:
1) ns-3: execution script and instructions, together with the simulation environment used for validation.
2) Komondor: input "nodes" and "system" files. Release pointing to v.2.0 must be used.
3) SF-CTMN: simulation environment, to be executed through the SF-CTMN framework, available at https://github.com/sergiobarra/SFCTMN
4) Bianchi: Matlab files emulating the Bianchi model ("Bianchi, G., Fratta, L., & Oliveri, M. (1996, October). Performance evaluation and enhancement of the CSMA/CA MAC protocol for 802.11 wireless LANs. In Personal, Indoor and Mobile Radio Communications, 1996. PIMRC'96., Seventh IEEE International Symposium on (Vol. 2, pp. 392-396). IEEE.") can be found, which simulate the throughput achieved by each WLAN in each of the proposed scenarios.

### Regression test
An automated regression test is available to ensure that the development of new features does not affect to the previous implementation, which was validated in Barrachina-Muñoz, S., Wilhelmi, F., Selinis, I., & Bellalta, B. (2019, April). Komondor: a Wireless Network Simulator for Next-Generation High-Density WLANs. In 2019 Wireless Days (WD) (pp. 1-8). IEEE.

In order to execute the regression test, go to ./Komondor/Code/input and run the script named "script_regression_validation_scenarios.sh". This script will take the inputs from the "validation" folder, execute the corresponding simulations, and compare the output with the expected results (i.e., the results obtained for the aforementioned paper).

The output of the regression test will be displayed by console. In case of success, the following output should be observed:
<p align="center"> 
<img src="https://github.com/wn-upf/Komondor/blob/master/Documentation/Other/Images and resources/example_execution_regression_test.png">
</p>

Before executing the regression test, it is important to ensure that "simulation_index" in komondor_main.cc is set to 10.


## Contribute

If you want to contribute, please contact to sergio.barrachina@upf.edu and/or francisco.wilhelmi@upf.edu

More details in [CONTRIBUTING.md](https://github.com/wn-upf/Komondor/blob/master/CONTRIBUTING.md)

## Acknowledgements

This work has been partially supported by a Gift from the Cisco University Research Program (CG\#890107, Towards Deterministic Channel Access in High-Density WLANs) Fund, a corporate advised fund of Silicon Valley Community Foundation.
