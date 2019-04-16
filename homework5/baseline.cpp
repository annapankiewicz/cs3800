#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cerrno>
#include <vector>
#include <unistd.h>
#include "mpi.h"
#include "phil.h"
#include "pomerize.h"

//run compiled code (for 5 philosophers) with mpirun -n 5 program

using namespace std;

//this is how many poems you want each Phil to construct & save
const int MAXMESSAGES = 10;

const int REQUEST = 0;
const int AVAILABLE = 1;
const int QUIT = 2;

//if you change this base, update the Makefile "clean" accordingly
const string fileBase = "outFile"; 

int main ( int argc, char *argv[] ) 
{
    int id; //my MPI ID
    int p;  //total MPI processes
    MPI::Status status;
    int tag = 1;
    int neighbor;

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

    int numWritten = 0;     // number of poems written
    int used = true;        // each file starts out as "used"

    //setup message storage locations
    int msgIn;
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
    if(id == p-1) {
        // highest numbered philosopher gets both
        philosophers[id].setRightAvailable(true);
        philosophers[id].setLeftAvailable(true);
    }
    else if (id == 0) {
        // Phil 0 gets neither
        philosophers[0].setRightAvailable(false);
        philosophers[0].setLeftAvailable(false);
    }
    else {
        // make the rest of the philosophers have their left file
        philosophers[id].setRightAvailable(false);
        philosophers[id].setLeftAvailable(true);
    }

    while (numWritten < MAXMESSAGES) {
        // current philosopher tied to process is philosophers[id]

        // check if the philosopher has access to its files
        if(!philosophers[id].isLeftAvailable() || !philosophers[id].isRightAvailable() || !used)
        {
            if(!philosophers[id].isLeftAvailable()) {
                // ask for the left file
                MPI::COMM_WORLD.Send(&REQUEST, 1, MPI::INT, leftNeighbor, tag);
                // receive response
                MPI::COMM_WORLD.Recv(&msgIn, 1, MPI::INT, leftNeighbor, tag, status);

                if(msgIn == AVAILABLE) {
                    // we can have the file!
                    philosophers[id].setLeftAvailable(true);
                }
                else if(msgIn == QUIT) {
                    // remove "access", we're done writing messages
                    philosophers[id].setLeftAvailable(false);
                }
            }

            if(!philosophers[id].isRightAvailable()) {
                // ask for the right file
                MPI::COMM_WORLD.Send(&REQUEST, 1, MPI_INT, rightNeighbor, tag);
                // receive response
                MPI::COMM_WORLD.Recv(&msgIn, 1, MPI::INT, rightNeighbor, tag, status);

                if(msgIn == AVAILABLE) {
                    // we can have the file!
                    philosophers[id].setRightAvailable(true);
                }
                else if(msgIn == QUIT) {
                    // remove "access", we're done writing messages
                    philosophers[id].setRightAvailable(false);
                }
            }
        }

        used = false; // haven't used the files we have yet

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

        used = true; // the files have been used

        while(philosophers[id].isLeftAvailable()
            || philosophers[id].isRightAvailable()) {

            // handle neighbors asking for files
            MPI::COMM_WORLD.Recv(&msgIn, 1, MPI::INT, MPI::ANY_SOURCE, tag, status);
            neighbor = status.Get_source();
            if(neighbor == leftNeighbor) {
                // remove own access to file
                philosophers[id].setLeftAvailable(false);
                // tell neighbor they can have it, or stop if all messages written
                if(numWritten == MAXMESSAGES) {
                    MPI::COMM_WORLD.Send(&QUIT, 1, MPI::INT, leftNeighbor, tag);
                }
                else {
                    MPI::COMM_WORLD.Send(&AVAILABLE, 1, MPI::INT, leftNeighbor, tag);
                }
            }
            else if(neighbor == rightNeighbor) {
                // remove own access to file
                philosophers[id].setRightAvailable(false);
                // tell neighbor they can have it, or stop if all messages written
                if(numWritten == MAXMESSAGES) {
                    MPI::COMM_WORLD.Send(&QUIT, 1, MPI::INT, rightNeighbor, tag);
                }
                else {
                    MPI::COMM_WORLD.Send(&AVAILABLE, 1, MPI::INT, rightNeighbor, tag);
                }
            }
        }
    }

  foutLeft.close();
  foutRight.close();
  
  //  Terminate MPI.
  MPI::Finalize ( );
  return 0;
}
