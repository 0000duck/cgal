/* Copyright 2004
Stanford University

This file is part of the DSR PDB Library.

The DSR PDB Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The DSR PDB Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the DSR PDB Library; see the file LICENSE.LGPL.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */
#include <CGAL/PDB/PDB.h>
#include <fstream>
#include <CGAL/Testsuite/assert.h>

#include "include/check_equal.h"


int main(int , char *[]){
  //dsr::Residue res= dsr::Residue(dsr::Residue::VAL);
  //res.write(std::cout);
  //CGAL_test_assert(argc==3);
  CGAL_SET_LOG_LEVEL(CGAL::Log::LOTS);
  std::ifstream in("data/check_atom_hetatom.pdb");
  CGAL_PDB_NS::PDB p(in);
  //p.write(std::cout);
  std::ostringstream of;
  
  std::cout << "There are " << p.number_of_models() << " models." << std::endl;
  
  for (CGAL_PDB_NS::PDB::Model_iterator it= p.models_begin();
       it != p.models_end(); ++it){
    const CGAL_PDB_NS::Model &m= it->model();
    std::cout << "Model " << it->key() << " has " << m.number_of_chains() 
	      << " chains" << " and " << m.number_of_heterogens() 
	      << " heterogens" << std::endl;
    for (CGAL_PDB_NS::Model::Chain_const_iterator it = m.chains_begin();
	 it != m.chains_end(); ++it) {
      std::cout << "Chain " << it->key() << " has " 
		<< it->chain().number_of_monomers() << " residues" << std::endl;
    }
  }
  p.write(of);

  return EXIT_SUCCESS;
}
