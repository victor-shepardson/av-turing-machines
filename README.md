# av-turing-machines
experiments with audiovisual Turing machines in openFrameworks

each TM derives its rules of operation from the other's tape.

3 * 2^16 cells map (current state, scanned symbol) -> (new state, written symbol, jump distance). 8-bit alphabet of 2^8 symbols, also the number of states and possible jump distances.

Triples of cells (each representing the behavior of the other TM in a particular state) are drawn as RGB pixels

TMs run at 48kHz and symbol values are mapped from [0,127] to [-.5, .5] and sent straight to the sound card


# build
oF 0.8.4, Win 7, Code::Blocks, C++11

port to newer oF, different OS and IDE should be straightforward

core logic is C++11 stdlib, port to different A/V framework should be simple

you may need to alter the line 'ss.setDeviceID(...)' to grab the right audio device on your system


# run
press 'i' to randomize the current instruction

press 'r' to randomize entire tapes

press 'p' to randomize head positions