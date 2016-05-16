# av-turing-machines
experiments with audiovisual Turing machines in openFrameworks

each TM derives its rules of operation from the other's tape.

3 * 2^16 cells map (current state, scanned symbol) -> (new state, written symbol, jump distance). 8-bit alphabet of 2^8 symbols, also the number of states and possible jump distances.

Triples of cells (each representing the behavior of the other TM in a particular state) are drawn as RGB pixels

TMs run at 48kHz and symbol values are mapped from [0,127] to [-.5, .5] and sent straight to the sound card


# build
tested with oF 0.9.3, Win 7, msys2

# run

set the audio device in bin/data/settings.xml

press 'i' to randomize just current instruction (most interesting)

press 'r' to randomize entire tapes

press 'p' to randomize head positions

press 's' to randomize TM states

press 'z' to zero TM tapes (doesn't work quite right due to concurrency)