// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Andreas.Fabri@sophia.inria.fr

#ifndef CGAL_CARTESIAN_CARTESIAN_SOLVE_D_H
#define CGAL_CARTESIAN_CARTESIAN_SOLVE_D_H

#ifndef CGAL_CARTESIAN_REDEFINE_NAMES_D_H
#include <CGAL/Cartesian/redefine_names_d.h>
#endif

#include <CGAL/solve.h>
#include <CGAL/Cartesian/Vector_d.h>

CGAL_BEGIN_NAMESPACE

template <class R>
void solve (const VectorCd<R CGAL_CTAG> &v0,
            const VectorCd<R CGAL_CTAG> &v1,
            const VectorCd<R CGAL_CTAG> &v2,
            const VectorCd<R CGAL_CTAG> &d,
            typename R::FT &alpha, typename R::FT &beta, typename R::FT &gamma)
{
  solve(v0.cartesian(0), v0.cartesian(1), v0.cartesian(2),
        v1.cartesian(0), v1.cartesian(1), v1.cartesian(2),
        v2.cartesian(0), v2.cartesian(1), v2.cartesian(2),
        d.cartesian(0),  d.cartesian(1),  d.cartesian(2),
        alpha, beta, gamma);
}

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_CARTESIAN_SOLVE_3_H
