// ======================================================================
//
// Copyright (c) 2002 INRIA
//
// ----------------------------------------------------------------------
//
// file          : include/NUAGE/Local_selection_cell_base_3.h
// package       : Local_selections_3(1.0)
// revision_date : $Date$
// author(s)     : Tran Kai Frank DA <Frank.Da@sophia.inria.fr>
//
// ======================================================================

#ifndef LOCAL_SELECTION_CELL_BASE_3_H
#define LOCAL_SELECTION_CELL_BASE_3_H


template < class CellBase >
class Local_selection_cell_base_3 : public CellBase
{

public:
  template < typename TDS2 >
  struct Rebind_TDS {
    typedef typename CellBase::template Rebind_TDS<TDS2>::Other  Cb2;
    typedef Local_selection_cell_base_3<Cb2>                    Other;
  };

  typedef typename CellBase::Vertex_handle Vertex_handle;
  typedef typename CellBase::Cell_handle Cell_handle;

private:
     
#ifdef FACET_NUMBER
  int _facet_number[4];
#endif
  typedef double coord_type;        
  typedef typename CGAL::Simple_cartesian<coord_type>::Point_3 D_Point;

  //-------------------- DATA MEMBERS ---------------------------------

  coord_type* _smallest_radius_facet_tab;
  unsigned char selected_facet;
#ifndef NOLAZY
  D_Point* _circumcenter;
  coord_type* _squared_radius;
#endif //NOLAZY 

  //-------------------- CONSTRUCTORS ----------------------------------

public:
  
  Local_selection_cell_base_3() 
    : CellBase(),
      _smallest_radius_facet_tab(NULL), selected_facet(0)
#ifndef NOLAZY
      , _circumcenter(NULL), _squared_radius(NULL)
#endif //NOLAZY
    {
#ifdef FACET_NUMBER
      for(int i = 0; i < 4; i++){
	_facet_number[i] = -1;
      }
#endif
    }
  
  Local_selection_cell_base_3(Vertex_handle v0, Vertex_handle v1, Vertex_handle v2, Vertex_handle v3)
    : CellBase( v0, v1, v2, v3),
      _smallest_radius_facet_tab(NULL), selected_facet(0)
#ifndef NOLAZY
      , _circumcenter(NULL), _squared_radius(NULL)
#endif //NOLAZY
    {
#ifdef FACET_NUMBER
      for(int i = 0; i < 4; i++){
	_facet_number[i] = -1;
      }
#endif
    }
  
  Local_selection_cell_base_3(Vertex_handle v0, Vertex_handle v1, Vertex_handle v2, Vertex_handle v3,
			      Cell_handle n0, Cell_handle n1, Cell_handle n2, Cell_handle n3)
    : CellBase(v0,  v1,  v2, v3,
	       n0,  n1,  n2, n3),
      _smallest_radius_facet_tab(NULL), selected_facet(0)
#ifndef NOLAZY
      , _circumcenter(NULL), _squared_radius(NULL)
#endif //NOLAZY
    {
#ifdef FACET_NUMBER
      for(int i = 0; i < 4; i++){
	_facet_number[i] = -1;
      }
#endif
    }

  //-------------------- DESTRUCTOR -----------------------------------

  inline ~Local_selection_cell_base_3()
    {
      if (_smallest_radius_facet_tab != NULL)
	 delete[] _smallest_radius_facet_tab;
#ifndef NOLAZY
      if (_circumcenter != NULL)
	delete _circumcenter;
      if (_squared_radius != NULL)
	delete _squared_radius;
#endif //NOLAZY
    }

  //--------------------  MEMBER FUNCTIONS ----------------------------

public:
  
  inline void clear()
    {
      if (_smallest_radius_facet_tab != NULL)
	delete[] _smallest_radius_facet_tab;
      _smallest_radius_facet_tab = NULL;
      selected_facet = 0;
#ifndef NOLAZY
      if (_circumcenter != NULL)
	delete _circumcenter;
      _circumcenter = NULL;
      if (_squared_radius != NULL)
	delete _squared_radius;
      _squared_radius = NULL;
#endif //NOLAZY
    }

  //-------------------------------------------------------------------

  inline coord_type get_smallest_radius(const int& i)
    { 
      if (_smallest_radius_facet_tab == NULL)
	return -1;
      return _smallest_radius_facet_tab[i];
    }
  
  inline void set_smallest_radius(const int& i, const coord_type& c)
    {
      if (_smallest_radius_facet_tab == NULL)
	{
	  _smallest_radius_facet_tab = new coord_type[4];
	  for(int i = 0; i < 4; i++)
	    _smallest_radius_facet_tab[i] = -1;
	}
      _smallest_radius_facet_tab[i] = c;
    }

  // pour un controle de l'allocation memoire... utile???
  inline bool alloc_smallest_radius_tab(coord_type* ptr)
    {
      if (_smallest_radius_facet_tab==NULL)
	{
	  _smallest_radius_facet_tab = ptr;
	  return true;
	}
      return false;
    }

  
  //-------------------------------------------------------------------
void
set_facet_number(int i, int n)
{
#ifdef FACET_NUMBER
  _facet_number[i] = n;
#endif
}
int
facet_number(int i)
{
#ifdef FACET_NUMBER
  return _facet_number[i];
#endif
  return 0;
}
  //-------------------------------------------------------------------
  
  inline void select_facet(const int& i)
    {
      selected_facet |= (1 << i);
    }
  
  inline void unselect_facet(const int& i)
    { 
      selected_facet &= (15 - (1 << i));
    }

  inline bool is_selected_facet(const int& i)
    {
      return selected_facet & (1 << i);
    }
 
#ifndef NOLAZY
 
  //-------------------------------------------------------------------

  inline D_Point* get_lazy_circumcenter()
    {
      return _circumcenter;
    }

  inline void set_lazy_circumcenter(const D_Point& p)
    {
      _circumcenter = new D_Point(p);
    }
  
  //-------------------------------------------------------------------

  inline coord_type* get_lazy_squared_radius()
    { 
      return _squared_radius;
    }

  inline void set_lazy_squared_radius(const coord_type& sr)
    { 
      _squared_radius = new coord_type(sr);
    }

#endif //NOLAZY

};

#endif // LOCAL_SELECTION_CELL_BASE_3_H
