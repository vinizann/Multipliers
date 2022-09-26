This repository holds implementation of multiplier circuits. The main goal is to design energy-efficient multipliers by using near-threshold operation and approximate computing techniques.

The multipliers are described at the electric level using Spice language and simulated with NGSPICE circuit simulator (https://ngspice.sourceforge.io/index.html). The circuits are designed using the 16 nm high-performance and the 16 nm low-power technology nodes provided by Predictive Technology Model (PTM), which is based on Bulk CMOS. The models can be found in this link: https://ptm.asu.edu/

In order to run the experiments, you will need to copy the model (16nm_LP or 16nm_HP), the "circuit.txt", the "library.cir", and the respective "script.cpp" inside the "bin" folder of Ngspice. For example, to simulate the Array multiplier with the low-power model at the nominal supply voltage and adopting 30% of the Approximate Copy Adder, the following files are required: 
- low_power/circuits/
  - -> "16nm_LP.pm"
  - copy_adder/copy_30/array
    - -> "array.txt"
    - -> "library.cir"
- low_power/scripts/nominal
  - -> "delay_unsigned.cpp"
  - -> "operation_unsigned.cpp"

Then, to measure the delay of the circuits, run the "delay_unsigned.cpp" file. To measure the energy and the result of multiplication, run the "operation_unsigned.cpp".

Contact:
Vinicius Zanandrea - viniciuszanandrea@gmail.com
