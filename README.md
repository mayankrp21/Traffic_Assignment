# Static-Traffic-Assignment
Traffic-Assignment is a C++ repository designed to solve the static traffic assignment problem using user equilibrium (UE) for a city network. The program employs the Method of Successive Averages (MSA) algorithm to achieve the solution.
# Dependencies
Standard Template Library (STL) for data structures and algorithms.
# How to Run the Static Traffic Assignment Program
1. #### Clone the Repository:

Clone the repository containing the C++ code to your local machine.

2. #### Prepare the Data:

Ensure that you have the necessary data files ("network.dat" and "demand.dat") formatted correctly. These files should be placed in the same directory as the C++ code.

network.dat: This file should contain the network data with columns: startNode, endNode, travelTime, capacity. 
<br/>
demand.dat: This file should contain the demand data with columns: origin, destination, demand.

3. #### Compile the Code:

Navigate to the directory containing the C++ code and compile it using a C++ compiler such as g++.
<br/>
`g++ -o traffic_assignment traffic_assignment.cpp
`

4. #### Run the Program:

Execute the compiled program and provide the number of iterations for the Method of Successive Averages (MSA) traffic assignment.
<br/>
``./traffic_assignment
``
<br/>

5. #### Output:

The program will parse the input files, perform the MSA traffic assignment, and output the final flow and travel time for each link.


