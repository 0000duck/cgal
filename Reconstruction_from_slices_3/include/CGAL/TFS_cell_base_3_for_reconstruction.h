// Copyright (c) 1998, 2001, 2003  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
//
// Author(s)     : Raphaelle Chaine (Raphaelle.Chaine@sophia.inria.fr, raphaelle.chaine@liris.cnrs.fr)
//                 Bastien Manuel
//                 Laurent Rincon
//                 Jerome Piovano
//                 

#ifndef CGAL_TFS_CELL_BASE_3_FOR_RECONSTRUCTION
#define CGAL_TFS_CELL_BASE_3_FOR_RECONSTRUCTION

#include <CGAL/basic.h>
#include <CGAL/Triangulation_short_names_3.h>

CGAL_BEGIN_NAMESPACE

#define MASK_INTERNAL         1
#define MASK_EXTERNAL         2
#define MASK_INTERN_D         4
#define MASK_INTERN_U         8

#define MASK_NON_SOLID        1
#define MASK_SOLID_TEST_D     2
#define MASK_SOLID_TEST_U     4
#define MASK_H_SOLID_TEST_D   8
#define MASK_H_SOLID_TEST_U   16

#define MASK_BIFURCATION    128

template < class Cb >
class TFS_cell_base_3_for_reconstruction
  : public Cb
{
 public :
  typedef Cb                                           Base;
  typedef typename Base::Triangulation_data_structure   Tds;

  typedef typename Tds::Vertex_handle                   Vertex_handle;
  typedef typename Tds::Cell_handle                     Cell_handle;
  typedef typename Base::Point                          Point;

  template < typename TDS2 >
  struct Rebind_TDS {
    typedef typename Cb::template Rebind_TDS<TDS2>::Other      Cb2;
    typedef TFS_cell_base_3_for_reconstruction<Cb2>          Other;
  };

 private :

  /*==================================*/
  /*       Private Member Datas       */
  /*==================================*/
  
  unsigned char mask_in_out;
  unsigned char mask_solidity;

 public :
  TFS_cell_base_3_for_reconstruction()
    : Base(), mask_in_out(0), mask_solidity(0) {}

  TFS_cell_base_3_for_reconstruction(const Vertex_handle& v0, const Vertex_handle& v1,
				       const Vertex_handle& v2, const Vertex_handle& v3)
    : Base(v0, v1, v2, v3), mask_in_out(0), mask_solidity(0) {}

  TFS_cell_base_3_for_reconstruction(const Vertex_handle& v0, const Vertex_handle& v1,
				       const Vertex_handle& v2, const Vertex_handle& v3,
				       const Cell_handle&   n0, const Cell_handle&   n1,
				       const Cell_handle&   n2, const Cell_handle&   n3)
    : Base(v0, v1, v2, v3, n0, n1, n2, n3), mask_in_out(0), mask_solidity(0) {}
  
  /*==================================*/
  /*     Public Members Functions     */
  /*==================================*/
  
  //CHECKING 
    bool is_Internal()           {return((((mask_in_out & MASK_INTERNAL)!=0)
					  ||(((mask_in_out & MASK_INTERN_D)!=0)&&((mask_in_out & MASK_INTERN_U)!=0)))
					 &&((mask_solidity & MASK_NON_SOLID)==0));} // TO CHANGE FOR SPEED
    bool is_External()  {return !is_Internal();}
    bool is_T2_2_down_internal() {return((mask_in_out & MASK_INTERN_D)!=0);}
    bool is_T2_2_up_internal()   {return((mask_in_out & MASK_INTERN_U)!=0);}
    
    bool is_explicit_in_out_tagged() {return(((mask_in_out & MASK_INTERNAL) 
					      | (mask_in_out & MASK_EXTERNAL) )!= 0);} 
                                      //| (mask_in_out & MASK_INTERN_D) | (mask_in_out & MASK_INTERN_U)

    bool is_non_solid()          {return((mask_solidity & MASK_NON_SOLID)!=0);}

    bool is_solid_tested_down()  {return((mask_solidity & MASK_SOLID_TEST_D)!=0);}
    bool is_solid_tested_up()    {return((mask_solidity & MASK_SOLID_TEST_U)!=0);}
    bool is_heavy_solid_tested_down()  {return((mask_solidity & MASK_H_SOLID_TEST_D)!=0);}
    bool is_heavy_solid_tested_up()    {return((mask_solidity & MASK_H_SOLID_TEST_U)!=0);}

    bool is_bifurcation_tested() {return((mask_in_out & MASK_BIFURCATION)!= 0) ;}
  
    //SETTING
    void set_explicit_Internal()   {mask_in_out|= MASK_INTERNAL;}
    void set_explicit_External()   {mask_in_out &= (~MASK_INTERNAL);
                                    mask_in_out |= MASK_EXTERNAL;}
    void set_T2_2_down_internal()  {mask_in_out |= MASK_INTERN_D;} 
    void set_T2_2_up_internal()    {mask_in_out |= MASK_INTERN_U;}

    void set_non_solid()           {mask_solidity |= MASK_NON_SOLID;}

    void set_solid_tested_down()   {mask_solidity |= MASK_SOLID_TEST_D;}
    void set_solid_tested_up()     {mask_solidity |= MASK_SOLID_TEST_U;}

    void set_heavy_solid_tested_down()   {mask_solidity |= MASK_H_SOLID_TEST_D;}
    void set_heavy_solid_tested_up()     {mask_solidity |= MASK_H_SOLID_TEST_U;}

    void unset_heavy_solid_tested_down()   {mask_solidity &=(~MASK_H_SOLID_TEST_D);}
    void unset_heavy_solid_tested_up()     {mask_solidity &=(~MASK_H_SOLID_TEST_U);}

    void set_bifurcation_tested()  {mask_in_out |= MASK_BIFURCATION;}
};

CGAL_END_NAMESPACE

#endif // CGAL_TFS_CELL_BASE_3_FOR_RECONSTRUCTION
