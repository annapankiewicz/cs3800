#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cerrno>
#include <vector>
#include <unistd.h>
#include "mpi.h"
#include "phil.h"
#include "pomerize.h"

#define USED 0
#define UNUSED 1

//run compiled code (for 5 philosophers) with mpirun -n 5 program

using namespace std;

//this is how many poems you want each Phil to construct & save
const int MAXMESSAGES = 10; 

//if you change this base, update the Makefile "clean" accordingly
const string fileBase = "outFile"; 

int main ( int argc, char *argv[] ) 
{
    int id; //my MPI ID
    int p;  //total MPI processes
    MPI::Status status;
    int tag = 1;

    //  Initialize MPI.
    MPI::Init ( argc, argv );

    //  Get the number of processes.
    p = MPI::COMM_WORLD.Get_size ( );

    //  Determine the rank of this process.
    id = MPI::COMM_WORLD.Get_rank ( );
  
    //Safety check - need at least 2 philosophers to make sense
    if (p < 2) {
        MPI::Finalize ( );
        std::cerr << "Need at least 2 philosophers! Try again" << std::endl;
        return 1; //non-normal exit
    }

    srand(id + time(NULL)); //ensure different seeds...

    int numWritten = 0;

    //setup message storage locations
    int msgInLeft, msgInRight, msgOut;
    int leftNeighbor = (id + p - 1) % p;
    int rightNeighbor = (id + 1) % p;

    pomerize P;

    string lFile = fileBase + to_string(id);
    string rFile = fileBase + to_string(rightNeighbor);
    ofstream foutLeft(lFile.c_str(), ios::out | ios::app );
    ofstream foutRight(rFile.c_str(), ios::out | ios::app );

    vector<Phil> philosophers;

    // make the philosophers exist
    for(int i = 0; i < p; i++) {
        philosophers.push_back((Phil(i, true, true)));
    }

    // set appropriate access to files for philosophers
    philosophers[0].setRightAvailable(false);
    philosophers[p-2].setLeftAvailable(false);
    for(int i = p-1; i >= 0; i--) {
        if(philosophers[i].isLeftAvailable() && philosophers[i].isRightAvailable()) {
            if(i == 0) {
                philosophers[i+1].setRightAvailable(false);
                philosophers[p-1].setLeftAvailable(false);
            }
            else {
                philosophers[i-1].setLeftAvailable(false);
                philosophers[i+1].setRightAvailable(false);
            }
        }
    }

    while (numWritten < MAXMESSAGES) {
        // current philosopher tied to process is philosophers[id]

        // check that the philosopher has access to its files
        if(philosophers[id].isLeftAvailable() && philosophers[id].isRightAvailable())
        {
            // remove access from other philosophers
            philosophers[id].setLeftAvailable(false);
            philosophers[id].setRightAvailable(false);

            // we have both files, okay to write our poems
            // construct poem & output stanzas into the files 'simultaneously'
            foutLeft << id << "'s poem:" << endl;
            foutRight << id << "'s poem:" << endl;

            string stanza1, stanza2, stanza3;
            stanza1 = P.getLine();
            foutLeft << stanza1 << endl;
            foutRight << stanza1 << endl;

            stanza2 = P.getLine();
            foutLeft << stanza2 << endl;
            foutRight << stanza2 << endl;

            stanza3 = P.getLine();
            foutLeft << stanza3 << endl << endl;
            foutRight << stanza3 << endl << endl;

            numWritten++;

            // send the message we're done writing to the files and they're available
            msgOut = UNUSED;
            MPI::COMM_WORLD.Send ( &msgOut, 1, MPI::INT, leftNeighbor, tag);
            MPI::COMM_WORLD.Send ( &msgOut, 1, MPI::INT, rightNeighbor, tag);
        }
        else {
            // files needed are unavailable, wait for them
            do {
                // wait for file on the left
                MPI::COMM_WORLD.Recv(&msgInLeft, 1, MPI::INT, leftNeighbor, tag, status);
                // wait for file on the right
                MPI::COMM_WORLD.Recv(&msgInRight, 1, MPI::INT, rightNeighbor, tag, status);
            }
            while (!msgInLeft && !msgInRight);
            // just to double check we're good
            if(msgInLeft && msgInRight) {
                // allow this philosopher to write
                philosophers[id].setLeftAvailable(true);
                philosophers[id].setRightAvailable(true);
            }
        }
    }

  foutLeft.close();
  foutRight.close();
  
  //  Terminate MPI.
  MPI::Finalize ( );
  return 0;
}
