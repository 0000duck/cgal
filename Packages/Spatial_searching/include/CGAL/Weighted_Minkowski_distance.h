// ======================================================================
//
// Copyright (c) 2002 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       : $CGAL_Revision: CGAL-2.5-I-99 $
// release_date  : $CGAL_Date: 2003/05/23 $
//
// file          : include/CGAL/Weighted_Minkowski_distance.h
// package       : ASPAS (3.12)
// maintainer    : Hans Tangelder <hanst@cs.uu.nl>
// revision      : 2.4 
// revision_date : 2002/16/08 
// authors       : Hans Tangelder (<hanst@cs.uu.nl>)
// coordinator   : Utrecht University
//
// ======================================================================

// Note: this implementation is based on a weight vector,
// another traits class for weighted Minkowski distances based on a 
// weight matrix will be added to ASPAS in the future

// Note: Use p=0 to denote the weighted Linf-distance 
// For 0<p<1 Lp is not a metric

#ifndef CGAL_WEIGHTED_MINKOWSKI_DISTANCE_H
#define CGAL_WEIGHTED_MINKOWSKI_DISTANCE_H
#include <math.h>
#include <CGAL/Kd_tree_rectangle.h>

namespace CGAL {

  template <class Item>
  class Weighted_Minkowski_distance {

    public:

    typedef typename Item::R::FT NT;
    typedef std::vector<NT> Weight_vector;

    private:

    NT p; // p denotes the power

    unsigned int The_dimension;

    Weight_vector *The_weights;

    public:

    // default constructor
    // default case dim=2, L2.
    
    /*
    Weighted_Minkowski_distance() : p(NT(2)), The_dimension(2) 
    { 
         The_weights = new Weight_vector(2);
         (*The_weights)[0]=NT(1);
         (*The_weights)[1]=NT(1);
    }*/

    Weighted_Minkowski_distance() 
    { 
       Item P;
       The_dimension=P.dimension();  The_weights = new Weight_vector(The_dimension);
	 for (unsigned int i = 0; i < The_dimension; ++i) (*The_weights)[i]=NT(1);
    }

	Weighted_Minkowski_distance (NT power, int dim,
		Weight_vector Weights) : p(power), The_dimension(dim)
	{
		assert(p >= NT(0));
		assert(The_dimension==Weights.size());
		for (unsigned int i = 0; i < The_dimension; ++i)
                assert(Weights[i]>=NT(0));
		The_weights = new Weight_vector(The_dimension);
		for (unsigned int i = 0; i < The_dimension; ++i) 
		(*The_weights)[i]=Weights[i];
	}

	~Weighted_Minkowski_distance() {
		delete The_weights;
	};

	inline NT distance(const Item& p1, const Item& p2) {
	        NT distance = NT(0);
		if (p == NT(0)) {
			for (unsigned int i = 0; i < The_dimension; ++i)
			if ((*The_weights)[i] * fabs(p1[i] - p2[i]) > distance)
			distance = (*The_weights)[i] * fabs(p1[i]-p2[i]);
		}
		else
			for (unsigned int i = 0; i < The_dimension; ++i)
				distance += 
				(*The_weights)[i] * pow(fabs(p1[i]-p2[i]),p);
        return distance;
	}


	inline NT min_distance_to_queryitem(const Item& Point,
					    const Kd_tree_rectangle<NT>& r) const {
		NT distance = NT(0);
		if (p == NT(0))
		{
		    for (unsigned int i = 0; i < The_dimension; ++i) {
			if ((*The_weights)[i]*(r.min_coord(i) - 
				Point[i]) > distance)
				distance = (*The_weights)[i] * (r.min_coord(i)-
				Point[i]);
			if ((*The_weights)[i] * (Point[i] - r.max_coord(i)) > 
				distance)
				distance = (*The_weights)[i] * 
				(Point[i]-r.max_coord(i));
			}
		}
		else
		{
			for (unsigned int i = 0; i < The_dimension; ++i) {
				if (Point[i] < r.min_coord(i))
					distance += (*The_weights)[i] * 
					pow(r.min_coord(i)-Point[i],p);
				if (Point[i] > r.max_coord(i))
					distance += (*The_weights)[i] * 
					pow(Point[i]-r.max_coord(i),p);
			}
		};
		return distance;
	}

	inline NT max_distance_to_queryitem(const Item& Point,
					      const Kd_tree_rectangle<NT>& r) const {
		NT distance=NT(0);
		if (p == NT(0))
		{
			for (unsigned int i = 0; i < The_dimension; ++i) {
				if (Point[i] >= (r.min_coord(i) + 
						r.max_coord(i))/NT(2.0))
				if ((*The_weights)[i] * (Point[i] - 
					r.min_coord(i)) > distance)
					distance = (*The_weights)[i] * 
					(Point[i]-r.min_coord(i));
				else
					if ((*The_weights)[i] * 
					(r.max_coord(i) - Point[i]) > distance)
					distance = (*The_weights)[i] * 
					( r.max_coord(i)-Point[i]);
			}
		}
		else
		{
			for (unsigned int i = 0; i < The_dimension; ++i) {
				if (Point[i] <= (r.min_coord(i)+r.max_coord(i))/NT(2.0))
					distance += (*The_weights)[i] * pow(r.max_coord(i)-Point[i],p);
				else
					distance += (*The_weights)[i] * pow(Point[i]-r.min_coord(i),p);
			}
		};
		return distance;
	}

	inline NT new_distance(NT dist, NT old_off, NT new_off,
			int cutting_dimension)  const {
		NT new_dist;
		if (p == NT(0))
		{
			if ((*The_weights)[cutting_dimension]*fabs(new_off) 
				> dist) 
			new_dist= 
			(*The_weights)[cutting_dimension]*fabs(new_off);
			else new_dist=dist;
		}
		else
		{
			new_dist = dist + (*The_weights)[cutting_dimension] * 
				(pow(fabs(new_off),p)-pow(fabs(old_off),p));
		}
                return new_dist;
	}

  inline NT transformed_distance(NT d) const {

		if (p <= NT(0)) return d;
		else return pow(d,p);

	}

  inline NT inverse_of_transformed_distance(NT d) const {

		if (p <= NT(0)) return d;
		else return pow(d,1/p);

	}

  }; // class Weighted_Minkowski_distance

} // namespace CGAL
#endif // WEIGHTED_MINKOWSKI_DISTANCE_H
