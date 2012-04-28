SpecSwap-RMC version 1.0a0
Copyright (c) 2008 - 2012   Mikael Leetmaa   <leetmaa@fysik.su.se>


-------------------------------------------------------------------------------
*Release Note*

This is the first alpha release of the SpecSwap-RMC program. Please
report any bugs or issues to Mikael Leetmaa   <leetmaa@fysik.su.se>

Thank you!


-------------------------------------------------------------------------------
*License*

See the LICENSE.txt file for copying conditions.


-------------------------------------------------------------------------------
*Description of the Method / Citing the Code*

This program implements the SpecSwap-RMC method as described in:

[1]  "SpecSwap-RMC: a novel reverse Monte Carlo approach using a discrete
      set of local configurations and pre-computed properties"
      Mikael Leetmaa, Kjartan Thor Wikfeldt and Lars G. M. Pettersson
      J. Phys.: Cond. Matter, 22, 135001, (2010)

Before we have a publication describing the code itself, please cite this
paper when using this program.


Further examples of the method in use can be found in:

[2]  "Oxygen-oxygen correlations in liquid water: Addressing the discrepancy
      between diffraction and extended x-ray absorption fine-structure using
      a novel multiple-data set fitting technique"
      Kjartan Thor Wikfeldt, Mikael Leetmaa, Amber Mace, Anders Nilsson and
      Lars G. M. Pettersson
      J. Chem.Phys., 132, 104513, (2010)

Additional explanations and summaries of the above papers can be found in these
doctoral thesis:
      < http://su.diva-portal.org/smash/record.jsf?pid=diva2:277117 >
      < http://su.diva-portal.org/smash/record.jsf?searchId=2&pid=diva2:412365 >


-------------------------------------------------------------------------------
*Installation*

* Modify the Makefile to suit your needs.

* Compile the code.


-------------------------------------------------------------------------------
*Tests and Usage Examples*

* When the code is compiled you should run the functionality tests
  to ensure that everything is setup and works as expected before
  performing any other work with the program.

* See the README files in each test directory for instructions on how
  to run the tests.

* Instructions on how to setup and run the unit testing framework are
  found in the README file in the "unittest" directory.

* For input examples see the input files in the test directories.

* The computed data in the libraries provided with the examples should
  be regarded as dummy data. For questions concerning real sets of
  data please contact the corresponding authors of the above cited
  papers [1] and [2].

* For any further questions or comments regarding the code, please
  contact Mikael Leetmaa   <leetmaa@fysik.su.se>


-------------------------------------------------------------------------------
*Code Documentation*

There are doxygen comments throughout the code and a "Doxygen" file in
the "./doc" folder to be used with Doxygen <http://www.doxygen.org/>
to generate comprehensive code documentation.

The file "doc/refman_v1.0a0.pdf" is the doxygen generated pdf
reference manual for this version.

