// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Herve.Bronnimann@sophia.inria.fr

#ifndef CGAL_CARTESIAN_PLANE_D_H
#define CGAL_CARTESIAN_PLANE_D_H

#include <CGAL/Cartesian/redefine_names_d.h>
#include <CGAL/Cartesian/d_utils.h>
#include <CGAL/d_tuple.h>
#include <algorithm>
#include <functional>


CGAL_BEGIN_NAMESPACE

template <class _R>
class PlaneCd
#ifndef CGAL_CFG_NO_ADVANCED_KERNEL
// This is a partial specialization
<_R,Cartesian_tag>
#endif
  : public Handle
{
public:
  typedef _R                                    R;
  typedef typename R::FT                        FT;
  typedef typename R::RT                        RT;
  typedef const RT*                             const_iterator ;
  typedef RT*                                   iterator ;
#ifndef CGAL_CFG_NO_ADVANCED_KERNEL
  typedef PlaneCd<R,Cartesian_tag>              Self;
  typedef typename R::Point_d                   Point_d;
  typedef typename R::Vector_d                  Vector_d;
  typedef typename R::Direction_d               Direction_d;
  // typedef typename R::Line_d                    Line_d;
  // typedef typename R::Ray_d                     Ray_d;
  // typedef typename R::Segment_d                 Segment_d;
  // typedef typename R::Aff_transformation_d      Aff_transformation_d;
#else
  typedef PlaneCd<R>                            Self;
  typedef typename R::Point_d_base              Point_d;
  typedef typename R::Vector_d_base             Vector_d;
  typedef typename R::Direction_d_base          Direction_d;
  // typedef typename R::Line_d_base               Line_d;
  // typedef typename R::Ray_d_base                Ray_d;
  // typedef typename R::Segment_d_base            Segment_d;
  // typedef typename R::Aff_transformation_d_base Aff_transformation_d;
#endif

  PlaneCd(int d = 0);
  PlaneCd(const Self &p);
  PlaneCd(const Point_d &p, const Direction_d &d);
  PlaneCd(const Point_d &p, const Vector_d &d);
  template < class InputIterator >
  PlaneCd(const int d,
          const InputIterator &first, const InputIterator &last)
    {
      CGAL_kernel_assertion( last-first == d+1);
      new_rep(first,last);
    }
  template < class InputIterator >
  PlaneCd(const int d,
          const InputIterator &first, const InputIterator &last, const RT &w)
    {
      CGAL_kernel_assertion( last-first == d);
      new_rep(first,last,w);
    }
  template < class InputIterator >
  PlaneCd(const InputIterator &first, const InputIterator &last)
    { 
      Self h = plane_from_points(first->dimension(),first,last,R());
      new_rep(h.begin(),h.end());
    }
  template < class InputIterator >
  PlaneCd(const InputIterator &begin, const InputIterator &end,
          const Self &o, Oriented_side side = POSITIVE)
    {
      Self h = plane_from_points(first->dimension(),first,last,o,side);
      new_rep(h.begin(),h.end());
    }
  ~PlaneCd();

  Self           &operator=(const Self &p);

  bool           operator==(const Self &p) const;
  bool           operator!=(const Self &p) const;
  long           id() const;

  RT             operator[](int i) const;
  Point_d        point() const;
  Point_d        projection(const Point_d &p) const;
  Vector_d       orthogonal_vector() const;
  Direction_d    orthogonal_direction() const;
  Vector_d       base(const int i) const;
  // Line_d         perpendicular_line(const Point_d &p) const;
  Self           opposite() const;

  Point_d        to_plane_basis(const Point_d &p) const;

  // Self           transform(const Aff_transformation_d &t) const;

  Oriented_side  oriented_side(const Point_d &p) const;
  bool           has_on_boundary(const Point_d &p) const;
  bool           has_on_positive_side(const Point_d &l) const;
  bool           has_on_negative_side(const Point_d &l) const;
  bool           has_on(const Point_d &p) const;

  bool           is_degenerate() const;

  int            dimension() const { return ptr()->d; }
  const_iterator begin()     const { return ptr()->e; }
  const_iterator end()       const { return ptr()->e+dimension(); }

protected:
  iterator       begin()           { return ptr()->e; }
  iterator       end()             { return ptr()->e+dimension(); }

private:
  const _d_tuple<RT>* ptr()  const { return (const _d_tuple<RT>*)PTR; }
  _d_tuple<RT>*       ptr()        { return (_d_tuple<RT>*)PTR; }
  void new_rep(int d);
  void new_rep(const_iterator hb, const_iterator he);
  void new_rep(const_iterator hb, const_iterator he, const RT &w);
};

CGAL_END_NAMESPACE

#ifndef CGAL_CARTESIAN_CLASS_DEFINED
#include <CGAL/Cartesian/Plane_d.C>
#endif 

#endif  // CGAL_CARTESIAN_PLANE_D_H
