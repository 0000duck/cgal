// ======================================================================
//
// Copyright (c) 2000 The CGAL Consortium
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
// file          : include/CGAL/Cartesian/Segment_2.h
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Andreas Fabri, Herve Bronnimann
// coordinator   : INRIA Sophia-Antipolis (Mariette.Yvinec@sophia.inria.fr)
//
// ======================================================================

#ifndef CGAL_CARTESIAN_SEGMENT_2_H
#define CGAL_CARTESIAN_SEGMENT_2_H

#include <CGAL/Cartesian/predicates_on_points_2.h>

CGAL_BEGIN_NAMESPACE

template < class R_ >
class SegmentC2 CGAL_ADVANCED_KERNEL_PARTIAL_SPEC
  : public R_::Segment_handle_2
{
  typedef typename R_::FT                        FT;

  typedef typename R_::Segment_handle_2          base;
  typedef typename base::element_type            rep;

  typedef typename R_::Kernel_base::Point_2              Point_2;
  typedef typename R_::Kernel_base::Vector_2             Vector_2;
  typedef typename R_::Kernel_base::Direction_2          Direction_2;
  typedef typename R_::Kernel_base::Line_2               Line_2;
  typedef typename R_::Kernel_base::Aff_transformation_2 Aff_transformation_2;

public:
  typedef R_                                     R;

  SegmentC2()
    : base(rep()) {}

  SegmentC2(const Point_2 &sp, const Point_2 &ep)
    : base(rep(sp, ep)) {}

  bool        is_horizontal() const;
  bool        is_vertical() const;
  bool        has_on(const Point_2 &p) const;
  bool        collinear_has_on(const Point_2 &p) const;

  bool        operator==(const SegmentC2 &s) const;
  bool        operator!=(const SegmentC2 &s) const;

  const Point_2 &   source() const
  {
      return Ptr()->e0;
  }
  const Point_2 &   target() const
  {
      return Ptr()->e1;
  }
  
  const Point_2 &    start() const;
  const Point_2 &    end() const;

  const Point_2 &   min() const;
  const Point_2 &   max() const;
  const Point_2 &   vertex(int i) const;
  const Point_2 &   point(int i) const;
  const Point_2 &   operator[](int i) const;

  FT          squared_length() const;

  Direction_2 direction() const;
  Line_2      supporting_line() const;
  SegmentC2        opposite() const;
  SegmentC2        transform(const Aff_transformation_2 &t) const
  {
    return SegmentC2(t.transform(source()), t.transform(target()));
  }

  bool        is_degenerate() const;
  Bbox_2      bbox() const;
};

#ifdef CGAL_CFG_TYPENAME_BUG
#define typename
#endif

template < class R >
inline
bool
SegmentC2<R CGAL_CTAG>::operator==(const SegmentC2<R CGAL_CTAG> &s) const
{
  if (identical(s))
      return true;
  return source() == s.source() && target() == s.target();
}

template < class R >
inline
bool
SegmentC2<R CGAL_CTAG>::operator!=(const SegmentC2<R CGAL_CTAG> &s) const
{
  return !(*this == s);
}

template < class R >
inline
const typename SegmentC2<R CGAL_CTAG>::Point_2 &
SegmentC2<R CGAL_CTAG>::start() const
{
  return source();
}

template < class R >
inline
const typename SegmentC2<R CGAL_CTAG>::Point_2 &
SegmentC2<R CGAL_CTAG>::end() const
{
  return target();
}

template < class R >
CGAL_KERNEL_INLINE
const typename SegmentC2<R CGAL_CTAG>::Point_2 &
SegmentC2<R CGAL_CTAG>::min() const
{
  return lexicographically_xy_smaller(source(),target()) ? source() : target();
}

template < class R >
CGAL_KERNEL_INLINE
const typename SegmentC2<R CGAL_CTAG>::Point_2 &
SegmentC2<R CGAL_CTAG>::max() const
{
  return lexicographically_xy_smaller(source(),target()) ? target() : source();
}

template < class R >
CGAL_KERNEL_INLINE
const typename SegmentC2<R CGAL_CTAG>::Point_2 &
SegmentC2<R CGAL_CTAG>::vertex(int i) const
{
  return (i%2 == 0) ? source() : target();
}

template < class R >
CGAL_KERNEL_INLINE
const typename SegmentC2<R CGAL_CTAG>::Point_2 &
SegmentC2<R CGAL_CTAG>::point(int i) const
{
  return (i%2 == 0) ? source() : target();
}

template < class R >
inline
const typename SegmentC2<R CGAL_CTAG>::Point_2 &
SegmentC2<R CGAL_CTAG>::operator[](int i) const
{
  return vertex(i);
}

template < class R >
CGAL_KERNEL_INLINE
typename SegmentC2<R CGAL_CTAG>::FT
SegmentC2<R CGAL_CTAG>::squared_length() const
{
  return squared_distance(source(), target());
}

template < class R >
CGAL_KERNEL_INLINE
typename SegmentC2<R CGAL_CTAG>::Direction_2
SegmentC2<R CGAL_CTAG>::direction() const
{
  return Direction_2( target() - source() );
}

template < class R >
inline
typename SegmentC2<R CGAL_CTAG>::Line_2
SegmentC2<R CGAL_CTAG>::supporting_line() const
{
  return Line_2(*this);
}

template < class R >
inline
SegmentC2<R CGAL_CTAG>
SegmentC2<R CGAL_CTAG>::opposite() const
{
  return SegmentC2<R CGAL_CTAG>(target(), source());
}

template < class R >
CGAL_KERNEL_INLINE
Bbox_2
SegmentC2<R CGAL_CTAG>::bbox() const
{
  return source().bbox() + target().bbox();
}

template < class R >
inline
bool
SegmentC2<R CGAL_CTAG>::is_degenerate() const
{
  return source() == target();
}

template < class R >
CGAL_KERNEL_INLINE
bool
SegmentC2<R CGAL_CTAG>::is_horizontal() const
{
  return y_equal(source(), target());
}

template < class R >
CGAL_KERNEL_INLINE
bool
SegmentC2<R CGAL_CTAG>::is_vertical() const
{
  return x_equal(source(), target());
}

template < class R >
CGAL_KERNEL_INLINE
bool
SegmentC2<R CGAL_CTAG>::
has_on(const typename SegmentC2<R CGAL_CTAG>::Point_2 &p) const
{
  return are_ordered_along_line(source(), p, target());
}

template < class R >
CGAL_KERNEL_MEDIUM_INLINE
bool
SegmentC2<R CGAL_CTAG>::
collinear_has_on(const typename SegmentC2<R CGAL_CTAG>::Point_2 &p) const
{
    return collinear_are_ordered_along_line(source(), p, target());
}

#ifndef CGAL_NO_OSTREAM_INSERT_SEGMENTC2
template < class R >
std::ostream &
operator<<(std::ostream &os, const SegmentC2<R CGAL_CTAG> &s)
{
    switch(os.iword(IO::mode)) {
    case IO::ASCII :
        return os << s.source() << ' ' << s.target();
    case IO::BINARY :
        return os << s.source() << s.target();
    default:
        return os << "SegmentC2(" << s.source() <<  ", " << s.target() << ")";
    }
}
#endif // CGAL_NO_OSTREAM_INSERT_SEGMENTC2

#ifndef CGAL_NO_ISTREAM_EXTRACT_SEGMENTC2
template < class R >
std::istream &
operator>>(std::istream &is, SegmentC2<R CGAL_CTAG> &s)
{
    typename SegmentC2<R CGAL_CTAG>::Point_2 p, q;

    is >> p >> q;

    if (is)
	s = SegmentC2<R CGAL_CTAG>(p, q);
    return is;
}
#endif // CGAL_NO_ISTREAM_EXTRACT_SEGMENTC2

#ifdef CGAL_CFG_TYPENAME_BUG
#undef typename
#endif

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_SEGMENT_2_H
