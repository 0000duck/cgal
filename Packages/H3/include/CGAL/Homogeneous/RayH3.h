// ======================================================================
//
// Copyright (c) 1999 The CGAL Consortium
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
// file          : include/CGAL/Homogeneous/RayH3.h
// package       : H3
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Stefan Schirra
//
// coordinator   : MPI, Saarbruecken
// ======================================================================
 

#ifndef CGAL_RAYH3_H
#define CGAL_RAYH3_H

CGAL_BEGIN_NAMESPACE

template < class R_ > class RayH3;

template < class R >
class Ray_repH3
{
 public:
  typedef typename R::Kernel_base::Point_3             Point_3;
  typedef typename R::Kernel_base::Direction_3         Direction_3;

  Ray_repH3() {}
  Ray_repH3(const Point_3& p, const Direction_3& d)
   : startpoint(p), direction(d) {}

 friend class RayH3<R>;

 private:
  Point_3      startpoint;
  Direction_3  direction;
};


template < class R_ >
class RayH3
  : public R_::Ray_handle_3
{
   typedef typename R_::RT    RT;
   typedef typename R_::FT    FT;
   typedef typename R_::Kernel_base::Point_3              Point_3;
   typedef typename R_::Kernel_base::Line_3               Line_3;
   typedef typename R_::Kernel_base::Direction_3          Direction_3;
   typedef typename R_::Kernel_base::Aff_transformation_3 Aff_transformation_3;

   typedef typename R_::Ray_handle_3              Ray_handle_3_;
   typedef typename Ray_handle_3_::element_type   Ray_ref_3;

  public:
   typedef R_                R;

    RayH3()
      : Ray_handle_3_(Ray_ref_3()) {}

    RayH3( const Point_3& sp, const Point_3& secondp)
      : Ray_handle_3_(Ray_ref_3(sp, (secondp-sp).direction())) {}

    RayH3( const Point_3& sp, const Direction_3& d)
      : Ray_handle_3_(Ray_ref_3(sp, d)) {}

    const Point_3 & start() const;
    const Point_3 & source() const;
    Point_3 second_point() const;
    Point_3 point(int i) const;
    const Direction_3 & direction() const;
    Line_3  supporting_line() const;
    RayH3<R>   opposite() const;
    RayH3<R>   transform( const Aff_transformation_3 & t) const;
    bool           has_on(const Point_3& p) const;
    bool           collinear_has_on(const Point_3 &p) const;
    bool           is_degenerate() const;

    bool           operator==(const RayH3<R>& r) const;
    bool           operator!=(const RayH3<R>& r) const;
};

template < class R >
inline
const typename RayH3<R>::Point_3 &
RayH3<R>::source() const
{ return Ptr()->startpoint; }

template < class R >
inline
const typename RayH3<R>::Point_3 &
RayH3<R>::start() const
{ return Ptr()->startpoint; }

template < class R >
inline
const typename RayH3<R>::Direction_3 &
RayH3<R>::direction() const
{
  CGAL_kernel_precondition( !is_degenerate() );
  return Ptr()->direction;
}

template < class R >
CGAL_KERNEL_INLINE
typename RayH3<R>::Point_3
RayH3<R>::second_point() const
{ return start() + direction().to_vector(); }

template < class R >
CGAL_KERNEL_INLINE
typename RayH3<R>::Point_3
RayH3<R>::point(int i) const
{
  CGAL_kernel_precondition( i >= 0 );
  return start() + RT(i)*(direction().to_vector() ) ;
}

template < class R >
CGAL_KERNEL_INLINE
typename RayH3<R>::Line_3
RayH3<R>::supporting_line() const
{
  CGAL_kernel_precondition( !is_degenerate() );
  return Line_3(start(), second_point() );
}

template < class R >
CGAL_KERNEL_INLINE
RayH3<R>
RayH3<R>::opposite() const
{ return RayH3<R>( start(), - direction() ); }

template < class R >
CGAL_KERNEL_INLINE
RayH3<R>
RayH3<R>::transform( const Aff_transformation_3 & t) const
{ return RayH3<R>(t.transform(start()), t.transform(direction()) ); }


#ifndef CGAL_NO_OSTREAM_INSERT_RAYH3
template < class R >
std::ostream &operator<<(std::ostream &os, const RayH3<R> &r)
{
  switch(os.iword(IO::mode))
  {
      case IO::ASCII :
          return os << r.start() << ' ' << r.direction();
      case IO::BINARY :
          return os<< r.start() << r.direction();
      default:
          return os << "RayH3(" << r.start() <<  ", " << r.direction() << ")";
  }
}
#endif // CGAL_NO_OSTREAM_INSERT_RAYH3

#ifndef CGAL_NO_ISTREAM_EXTRACT_RAYH3
template < class R  >
std::istream &operator>>(std::istream &is, RayH3<R> &r)
{
  typename RayH3<R>::Point_3 p;
  typename RayH3<R>::Direction_3 d;
  is >> p >> d;
  r = RayH3<R>(p, d);
  return is;
}
#endif // CGAL_NO_ISTREAM_EXTRACT_RAYH3

template < class R >
CGAL_KERNEL_INLINE
bool
RayH3<R>::has_on(const typename RayH3<R>::Point_3 &p) const
{
  return ( (  p == start() )
         ||(  Direction_3(p - start()) == direction() ) );
}

template < class R >
inline                                      /* XXX */
bool
RayH3<R>::collinear_has_on(const typename RayH3<R>::Point_3 &p) const
{ return has_on(p); }

template < class R >
inline
bool
RayH3<R>::is_degenerate() const
{ return (Ptr()->direction).is_degenerate() ; }

template < class R >
CGAL_KERNEL_INLINE
bool
RayH3<R>::operator==(const RayH3<R>& r) const
{ return ( (start() == r.start() )&&( direction() == r.direction() ) ); }

template < class R >
CGAL_KERNEL_INLINE
bool
RayH3<R>::operator!=( const RayH3<R>& r) const
{ return !operator==(r); }

CGAL_END_NAMESPACE

#endif // CGAL_RAYH3_H
