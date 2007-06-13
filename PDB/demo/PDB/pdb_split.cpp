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
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cctype>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <sstream>

/*!
  \example pdb_split.cpp

  This example shows how to split a pdb file various ways. A pdb can
  be split into different models, chains or event cut a particular
  chain.
*/

std::string make_fname(std::string format, int model, char chain, std::string name) {
  std::ostringstream oss;
  if (model ==-1) {
    oss << boost::format(format.c_str()) % '-' % chain %name;
  } else {
    oss << boost::format(format.c_str()) % model % chain %name;
  }
  std::cout << "Returning " << oss.str() << " for " << model << " " << chain << " " << name << std::endl;
  return oss.str();
}

int main(int argc, char *argv[]){
  bool split_chains=false;
  bool split_models=false;
  std::string input_file, output_template;
  bool print_help=false;
  bool verbose=false;

  boost::program_options::options_description o("Allowed options"), po, ao;
  o.add_options()
    ("help", boost::program_options::bool_switch(&print_help),
     "produce help message")
    ("verbose,v", boost::program_options::bool_switch(&verbose),
     "print out verbose messages about reading and writing pdb files")
    ("split-chains,c", boost::program_options::bool_switch(&split_chains),
     "Split all chains into separate files.")
    ("split-models,m", boost::program_options::bool_switch(&split_models),
     "Split all models into separate files.");
  po.add_options()
    ("input-pdb", boost::program_options::value< std::string>(&input_file),
     "input file")
    ("output-pdb-template", boost::program_options::value< std::string>(&output_template),
     "A boost::format style string that will be used to generate the names for the output files. %1% is the model, %2% is the chain, %3% is the chain name (if any).");

  ao.add(o).add(po);

  boost::program_options::positional_options_description p;
  p.add("input-pdb", 1);
  p.add("output-pdb-template", 2);

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::command_line_parser(argc, argv).
				options(ao).positional(p).run(), vm);
  boost::program_options::notify(vm);



  //boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  //boost::program_options::notify(vm);

  if (input_file.empty()
      || output_template.empty()
        || print_help) {
    std::cout << "This program splits a pdb file with multiple models or chains into separate files "
	      << " [-c -m] input-pdb template%1%%2%%3%.pdb\n" << std::endl;
    std::cout << "The second argument is a boost::format style string that will be used to generate the names for the output files.\n\n";
    std::cout << o << "\n";
    return EXIT_SUCCESS;
  }

  std::ifstream in(input_file.c_str());
  if (!in) {
    std::cerr << "Error opening input file " << input_file << std::endl;
    return EXIT_FAILURE;
  }
  using namespace CGAL_PDB_NS;

  //= new char[strlen(argv[2]+1000)];
  PDB pdb(in, verbose);

  std::map<std::string, PDB > outputs;

  if (split_chains || split_models) {
    for (PDB::Model_const_iterator it= pdb.models_begin(); it != pdb.models_end(); ++it) {
      if (split_models && !split_chains){
	PDB out;
	out.insert(PDB::Model_key(0), it->model());
	outputs[make_fname(output_template, it->key().index(),
			   '-', std::string())]= out;
      } else {
	for (Model::Chain_const_iterator cit= it->model().chains_begin();
	     cit != it->model().chains_end(); ++cit) {
	  std::string name;
	  if (split_models) {
	    name= make_fname(output_template, it->key().index(), cit->key().index(),
			     cit->chain().name());
	  } else {
	    name= make_fname(output_template, -1, cit->key().index(),
			     cit->chain().name());
	  }
	  Model model;
	  model.insert(cit->key(), cit->chain());
	  outputs[name].insert(it->key(), model);
	}
      }
    }
  } else {
    outputs[make_fname(output_template, -1, '-', std::string())]
					 =pdb;
  }

  for (std::map<std::string, PDB>::iterator it= outputs.begin(); 
       it != outputs.end(); ++it){
    std::ofstream out(it->first.c_str());
    if (!out) {
      std::cerr << "Error opening file " << it->first << std::endl;
      continue;
    }
    it->second.set_header(pdb.headers_begin(), pdb.headers_end());
    it->second.write(out);
  }

  //delete[] buf;
  return EXIT_SUCCESS;
}
