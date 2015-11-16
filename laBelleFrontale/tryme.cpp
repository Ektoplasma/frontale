#include <message.h>
#include <requete.h>
#include "engine_event.h"
#include "network_interface.h"
#include <cstdlib>
#include <iostream>
#include <unistd.h>


int main(int argc, char *argv[])
{

    network_interface net;

    string message = "ENSIMAG";
    string * showRep = new string[2];

    showRep = net.send_look(message);

    cout << "NOM :" << showRep[0] << endl;
    cout << "Hash du statut correspondant :" << showRep[1] << endl << endl;
}