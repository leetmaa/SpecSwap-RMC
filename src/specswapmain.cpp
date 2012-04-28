/*
    Part of the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    This program is distributed under the terms of the GNU General Public
    License version 3 as published by the Free Software Foundation,
    with the Additional Terms as stated in the SpecSwap-RMC LICENSE file.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the SpecSwap-RMC LICENSE file for full details.

    Author: Mikael Leetmaa    <leetmaa@fysik.su.se>
*/


/*! \file  specswapmain.cpp
 *  \brief File for the SpecSwap program's main routine.
 */

#include <stdio.h>

#include "mlrmc.h"
#include "ioutils.h"
#include "ioexception.h"

/*! \brief The main routine of the SpecSwap-RMC program. */
int main(int argc, char *argv[])
{
    // Screen IO.
    print_startup();

    try
    {
        // Check the number of input arguments.
        if (argc != 2)
        {
            error("Wrong number of arguments.\n", "Reading command line arguments.");
        }

        // Send in the input file-name to the constructor.
        Mlrmc rmc(argv[argc-1]);
        // Launch the rmc simulation.
        rmc.run_rmc();
        // Do the post processing.
        rmc.post_process();
        // Print the exit message.
        print_success();


    }
    // Catch all input and file IO errors.
    catch (IOException & e)
    {
        e.print();
        error_exit();
    }

    // DONE
    return 0;
}


