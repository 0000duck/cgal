// ======================================================================
//
// Copyright (c) 1997 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       : 
// release_date  : 
//
// file          : Triangulation/include/CGAL/Triangulation_ds_face_2.h
// source        : $RCSfile$
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Mariette Yvinec
//
// coordinator   : Mariette Yvinec  <Mariette Yvinec@sophia.inria.fr>
//
// ======================================================================

#ifndef CGAL_TRIANGULATION_DS_FACE_2_H
#define CGAL_TRIANGULATION_DS_FACE_2_H

#include <CGAL/basic.h>
#include <CGAL/Triangulation_short_names_2.h>
#include <CGAL/Triangulation_utils_2.h>

CGAL_BEGIN_NAMESPACE 

template < class Tds >
class  Triangulation_ds_face_2
  : public Tds::Face_base,    
    public Triangulation_cw_ccw_2
{
public:
  typedef typename Tds::Vertex_base        Vb;
  typedef typename Tds::Face_base          Fb;
  typedef typename Tds::Vertex             Vertex;
  typedef typename Tds::Face               Face;
  typedef typename Tds::Vertex_handle      Vertex_handle;
  typedef typename Tds::Face_handle        Face_handle;

private:
  Face_handle _previous;
  Face_handle _next;

public :
  // creators
  Triangulation_ds_face_2()
    : Fb(), _previous(this), _next(this)
  {}
    
  Triangulation_ds_face_2(Vertex_handle v0, Vertex_handle v1, Vertex_handle v2)
    :  Fb(&*v0,&*v1,&*v2)
  {}
    
  Triangulation_ds_face_2(Vertex_handle v0, Vertex_handle v1, Vertex_handle v2,
			  Face_handle n0, Face_handle n1, Face_handle n2)
    :  Fb(&*v0,&*v1,&*v2,&*n0,&*n1,&*n2)
  {}

  Triangulation_ds_face_2( const Face& f)
    : Fb(f)
    {}

  //setting
  void set_vertex(int i, Vertex_handle v) { Fb::set_vertex(i, &*v);}
  void set_neighbor(int i, Face_handle n) { Fb::set_neighbor(i, &*n);}
  void set_vertices() { Fb::set_vertices();}
  void set_neighbors() { Fb::set_neighbors();}
  void set_vertices(Vertex_handle v0, Vertex_handle v1, Vertex_handle v2);
  void set_neighbors(Face_handle n0, Face_handle n1, Face_handle n2);
  //void reorient();  inherited from Fb
 
  //Vertex Access Member Functions
  Vertex_handle vertex(int i) const;
  Vertex_handle mirror_vertex(int i) const;
  bool has_vertex(Vertex_handle v) const;
  bool has_vertex(Vertex_handle v, int& i) const;
  int index(Vertex_handle v) const;

  // Neighbors Access Functions
  Face_handle neighbor(int i) const;
  bool has_neighbor(Face_handle n) const;
  bool has_neighbor(Face_handle n, int& i) const;
  int index(Face_handle n) const;
  int mirror_index(int i) const;

  //Miscelleanous
  Face_handle handle() const {return const_cast<Face*>(this);}
  Face_handle handle() {return const_cast<Face*>(this);}
  bool is_valid(bool verbose = false, int level = 0) const;

  //Handling the list of faces
  Face_handle previous() const {return _previous;}
  Face_handle next() const {return _next;}
  void  set_previous(Face_handle f) {_previous = f;};
  void  set_next(Face_handle f) {_next = f;};
};



template < class Tds >
inline void 
Triangulation_ds_face_2<Tds>::
set_vertices(Vertex_handle v0, Vertex_handle v1, Vertex_handle v2)
{
  Fb::set_vertices(&*v0,&*v1,&*v2);
}

template < class Tds >
inline void 
Triangulation_ds_face_2<Tds>::
set_neighbors(Face_handle n0, Face_handle n1, Face_handle n2)
{
  Fb::set_neighbors(&*n0,&*n1,&*n2);
}

template < class Tds >
inline
typename Triangulation_ds_face_2<Tds>::Vertex_handle
Triangulation_ds_face_2<Tds>::
vertex(int i) const
{
  return(Vertex_handle(static_cast<Vertex*>(Fb::vertex(i))));
} 

template < class Tds >
inline
typename Triangulation_ds_face_2<Tds>::Vertex_handle
Triangulation_ds_face_2<Tds>::
mirror_vertex(int i) const
{
  CGAL_triangulation_precondition ( &*neighbor(i) != NULL);
  //return neighbor(i)->vertex(neighbor(i)->index(this->handle()));
  return neighbor(i)->vertex(mirror_index(i));
}

template < class Tds >
inline int
Triangulation_ds_face_2<Tds>::
mirror_index(int i) const
{
  CGAL_triangulation_precondition (neighbor(i) != NULL);
  //return neighbor(i)->index(this->handle());
  return ccw( neighbor(i)->index(vertex(ccw(i))));
}

template < class Tds >
inline  bool 
Triangulation_ds_face_2<Tds>::
has_vertex(Vertex_handle v) const
{
  return (Fb::has_vertex(&*v));
}
    
template < class Tds >
inline  bool 
Triangulation_ds_face_2<Tds>::    
has_vertex(Vertex_handle v, int& i) const
{
  return (Fb::has_vertex(&*v,i));
}
    
template < class Tds >
inline  int 
Triangulation_ds_face_2<Tds>::  
index(Vertex_handle v) const
{
  return(Fb::vertex_index(&*v));
}

// Neighbors Access Functions
template < class Tds >
inline   
typename Triangulation_ds_face_2<Tds>::Face_handle
Triangulation_ds_face_2<Tds>::  
neighbor(int i) const
{
  return (static_cast<Face*>(Fb::neighbor(i)));
}
    
template < class Tds >
inline  bool 
Triangulation_ds_face_2<Tds>::  
has_neighbor(Face_handle n) const
{
  return (Fb::has_neighbor(&*n));
}
    
template < class Tds >
inline  bool 
Triangulation_ds_face_2<Tds>::      
has_neighbor(Face_handle n, int& i) const
{
  return (Fb::has_neighbor(&*n,i));
}
    
template < class Tds >
inline  int 
Triangulation_ds_face_2<Tds>::    
index(Face_handle n) const
{
  return(Fb::face_index(&*n));
}
    
//Miscelleanous
template < class Tds >
bool
Triangulation_ds_face_2<Tds>::  
is_valid(bool verbose, int level) const
{
  bool result = Fb::is_valid(verbose, level);
  for(int i = 0; i <= dimension(); i++) {
    Face_handle n = neighbor(i);
    int in = n->index(this->handle());
    result = result && ( this->handle() == n->neighbor(in) );
    switch(dimension()) {
    case 0 : 
      break;
    case 1 :
      result = result &&  in == 1-i;
      result = result && ( this->vertex(1-i) == n->vertex(1-in));
      break;
    case 2 :
      result = result && ( this->vertex(cw(i)) == n->vertex(ccw(in)))
	              && ( this->vertex(ccw(i)) == n->vertex(cw(in)));
      break;
    }
  }
  return result;
}


CGAL_END_NAMESPACE

#endif //CGAL_TRIANGULATION_DS_FACE_2_H
