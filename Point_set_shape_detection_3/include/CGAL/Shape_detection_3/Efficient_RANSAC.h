// Copyright (c) 2015 INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
//
// Author(s)     : Sven Oesau, Yannick Verdie, Clément Jamin, Pierre Alliez
//

#ifndef CGAL_SHAPE_DETECTION_3_EFFICIENT_RANSAC_H
#define CGAL_SHAPE_DETECTION_3_EFFICIENT_RANSAC_H

#include <CGAL/Shape_detection_3/Octree.h>
#include <CGAL/Shape_detection_3/Shape_base.h>

//for octree ------------------------------
#include <boost/iterator/filter_iterator.hpp>
#include <CGAL/bounding_box.h>
#include <CGAL/Iterator_range.h>
//----------

#include <vector>
#include <random>
#include <cmath>

#include <fstream>
#include <sstream>

//boost --------------
#include <boost/iterator/counting_iterator.hpp>
#include <boost/shared_ptr.hpp>
//---------------------


/*! 
  \file Efficient_RANSAC.h
*/

namespace CGAL {
  namespace Shape_detection_3 {

/*!
\ingroup PkgPointSetShapeDetection3
\brief A shape detection algorithm using a RANSAC method.

Given a point set in 3D space with unoriented normals, sampled on surfaces,
this class enables to detect subsets of connected points lying on the surface of primitive shapes.
Each input point is assigned to either none or at most one detected primitive
shape. The implementation follows \cgalCite{schnabel2007efficient}.

\tparam Traits a model of `EfficientRANSACTraits`

*/
  template <class Traits>
  class Efficient_RANSAC {
  public:

    /// \cond SKIP_IN_MANUAL
    struct Filter_unassigned_points {
      Filter_unassigned_points() : m_shape_index() {}
      Filter_unassigned_points(const std::vector<int> &shapeIndex)
        : m_shape_index(shapeIndex) {}

      bool operator()(std::size_t x) {
        if (x < m_shape_index.size())
          return m_shape_index[x] == -1;
        else return true; // to prevent infinite incrementing
      }
      const std::vector<int>&  m_shape_index;
    };

    typedef boost::filter_iterator<Filter_unassigned_points,
      boost::counting_iterator<std::size_t> > Point_index_iterator;
    ///< iterator for indices of points.
    /// \endcond

    /// \name Types 
    /// @{
    /// \cond SKIP_IN_MANUAL
    typedef typename Traits::Input_range::iterator Input_iterator;
    typedef typename Traits::FT FT; ///< number type.
    typedef typename Traits::Point_3 Point; ///< point type.
    typedef typename Traits::Vector_3 Vector; ///< vector type.
    /// \endcond

    typedef typename Traits::Input_range Input_range;
    ///< Model of the concept `Range` with random access iterators, providing input points and normals
    /// through the following two property maps.

    typedef typename Traits::Point_map Point_map;
    ///< property map to access the location of an input point.
    typedef typename Traits::Normal_map Normal_map;
    ///< property map to access the unoriented normal of an input point
    typedef Shape_base<Traits> Shape; ///< shape type.

#ifdef DOXYGEN_RUNNING
    typedef unspecified_type Shape_range;
#else
    typedef
    Iterator_range<typename std::vector<boost::shared_ptr<Shape> >::const_iterator>  Shape_range;
#endif
    ///< An `Iterator_range` with a bidirectional constant iterator type with 
    ///  value type `boost::shared_ptr<Shape>`. 


#ifdef DOXYGEN_RUNNING
    typedef unspecified_type Point_index_range;
    ///< An `Iterator_range` with a bidirectional iterator with value type `std::size_t`
    ///  as indices into the input data that has not been assigned to a shape.
    ///  As this range class has no `size()` method, the method 
    ///  `Efficient_RANSAC::number_of_unassigned_points()` is provided.
#else 
    typedef typename Iterator_range<Point_index_iterator>
      Point_index_range;
#endif     

    /// @}
    
    /// \name Parameters 
    /// @{
      /*!
       %Parameters for the shape detection algorithm. They are explained in detail 
       in Section \ref Point_set_shape_detection_3Parameters  of the User Manual.
       */
    struct Parameters {
      Parameters()
        : probability(0.01)
        , min_points(SIZE_MAX)
        , epsilon(-1)
        , normal_threshold(0.9)
        , cluster_epsilon(-1)
      {}

      FT probability;         ///< Probability to control search endurance. %Default value: 0.05.
      std::size_t min_points; ///< Minimum number of points of a shape. %Default value: 1% of total number of input points.
      FT epsilon;             ///< Maximum tolerance Euclidian distance from a point and a shape. %Default value: 1% of bounding box diagonal.
      FT normal_threshold;	  ///< Maximum tolerance normal deviation from a point's normal to the normal on shape at projected point. %Default value: 0.9 (around 25 degrees).
      FT cluster_epsilon;	    ///< Maximum distance between points to be considered connected. %Default value: 1% of bounding box diagonal.
    };
    /// @}

  private:
    typedef internal::Octree<internal::DirectPointAccessor<Traits> >
      Direct_octree;
    typedef internal::Octree<internal::IndexedPointAccessor<Traits> >
      Indexed_octree;
    //--------------------------------------------typedef

    // Creates a function pointer for instancing shape instances.
    template <class ShapeT>
    static Shape *factory() {
      return new ShapeT;
    }

  public:

  /// \name Initialization
  /// @{

    /*! 
      Constructs an empty shape detection engine.
    */ 
    Efficient_RANSAC()
      : m_rng(std::random_device()())
      , m_direct_octrees(NULL)
      , m_global_octree(NULL)
      , m_num_subsets(0)
      , m_num_available_points(0)
      , m_valid_iterators(false)
    {}

    /*! 
      Releases all memory allocated by this instances including shapes.
    */ 
    ~Efficient_RANSAC() {
      clear();
    }

    /*!
      Sets the input data. The range must stay valid
      until the detection has been performed and the access to the
      results is no longer required. The data in the input is reordered by the methods
      `detect()` and `preprocess()`. This function first calls `clear()`.
    */
    void set_input(
      Input_range& input_range,
      ///< Range of input data.
      Point_map point_map = Point_map(),
      ///< property map to access the position of an input point.
      Normal_map normal_map = Normal_map()
      ///< property map to access the normal of an input point.
      ) {
        clear();

        m_extracted_shapes.clear();

        m_point_pmap = point_map;
        m_normal_pmap = normal_map;

        m_inputIterator_first = input_range.begin();
        m_inputIterator_beyond = input_range.end();

        m_num_available_points = m_inputIterator_beyond - m_inputIterator_first;

        m_valid_iterators = true;
    }
    /*!
      Registers in the detection engine the shape type `ShapeType` that must inherit from `Shape_base`.
    */ 
    template <class ShapeType>
    void add_shape_factory() {
      m_shape_factories.push_back(factory<ShapeType>);
    }

    /*!
      Constructs internal data structures required for the shape detection.
      These structures only depend on the input data, i.e. the points and
      normal vectors. This method is called by `detect()`, if it was not called
      before by the user.
    */ 
    bool preprocess() {
      if (m_num_available_points == 0)
        return false;

      // Generation of subsets
      m_num_subsets = (std::max<std::size_t>)((std::size_t)
        std::floor(std::log(double(m_num_available_points))/std::log(2.))-9, 2);

      // SUBSET GENERATION ->
      // approach with increasing subset sizes -> replace with octree later on
      Input_iterator last = m_inputIterator_beyond - 1;
      std::size_t remainingPoints = m_num_available_points;

      m_available_octree_sizes.resize(m_num_subsets);
      m_direct_octrees = new Direct_octree *[m_num_subsets];
      for (int s = m_num_subsets - 1;s >= 0;--s) {
        std::size_t subsetSize = remainingPoints;
        std::vector<std::size_t> indices(subsetSize);
        if (s) {
          subsetSize >>= 1;
          for (std::size_t i = 0;i<subsetSize;i++) {
            std::size_t index = m_rng() % 2;
            index = index + (i<<1);
            index = (index >= remainingPoints) ? remainingPoints - 1 : index;
            indices[i] = index;
          }

          // move points to the end of the point vector
          for (int i = subsetSize - 1;i >= 0;i--) {
            typename std::iterator_traits<Input_iterator>::value_type
              tmp = (*last);
            *last = m_inputIterator_first[indices[std::size_t(i)]];
            m_inputIterator_first[indices[std::size_t(i)]] = tmp;
            last--;
          }
          m_direct_octrees[s] = new Direct_octree(last + 1,
            last + subsetSize + 1,
            remainingPoints - subsetSize);
        }
        else
          m_direct_octrees[0] = new Direct_octree(m_inputIterator_first,
          m_inputIterator_first + (subsetSize), 
          0);

        m_available_octree_sizes[s] = subsetSize;
        m_direct_octrees[s]->createTree();

        remainingPoints -= subsetSize;
      }

      m_global_octree = new Indexed_octree(m_inputIterator_first, m_inputIterator_beyond);
      m_global_octree->createTree();

      return true;
    }

    /// @}

    /// \name Memory Management
    /// @{
    /*!
      Removes all shape types registered for detection.
     */ 
    void clear_shape_factories() {
      m_shape_factories.clear();
    }

    /*!
      Frees memory allocated for the internal search structures but keeps the detected shapes.
      It invalidates the range retrieved using `unassigned_points()`.
     */ 
    void clear_octrees() {
      // If there is no data yet, there are no data structures.
      if (!m_valid_iterators)
        return;

      if (m_global_octree) {
        delete m_global_octree;
        m_global_octree = NULL;
      }

      if (m_direct_octrees) {
        for (std::size_t i = 0;i<m_num_subsets;i++) 
          delete m_direct_octrees[i];
        delete [] m_direct_octrees;

        m_direct_octrees = NULL;
      }

      m_num_subsets = 0;
    }

    /*!
      Calls `clear_shape_factories()`, `clear_octrees()` and removes all detected shapes.
      All internal structures are cleaned, including formerly detected shapes.
      Thus iterators and ranges retrieved through `shapes()` and `indices_of_unassigned_points()` 
      are invalidated.
    */ 
    void clear() {
      clear_shape_factories();

      // If there is no data yet, there are no data structures.
      if (!m_valid_iterators)
        return;

      std::vector<int>().swap(m_shape_index);

      m_extracted_shapes.clear();

      m_num_available_points = m_inputIterator_beyond - m_inputIterator_first;

      clear_octrees();
    }
    /// @}

    /// \name Detection 
    /// @{
    /*! 
      Performs the shape detection. Shape types considered during the detection
      are those registered using `add_shape_factory()`.

      \return `true` if shape types have been registered and
              input data has been set. Otherwise, `false` is returned.
    */ 
    bool detect(
      const Parameters &options = Parameters()
      ///< %Parameters for shape detection.
                ) {
      // no shape types for detection or no points provided, exit
      if (m_shape_factories.size() == 0 ||
          (m_inputIterator_beyond - m_inputIterator_first) == 0)
        return false;

      if (m_num_subsets == 0 || m_global_octree == 0) {
        if (!preprocess())
          return false;
      }

      // Reset data structures possibly used by former search
      m_extracted_shapes.clear();
      m_num_available_points = m_inputIterator_beyond - m_inputIterator_first;

      for (std::size_t i = 0;i<m_num_subsets;i++) {
        m_available_octree_sizes[i] = m_direct_octrees[i]->size();
      }

      // use bounding box diagonal as reference for default values
      Bbox_3 bbox = m_global_octree->boundingBox();
      FT bboxDiagonal = sqrt((bbox.xmax() - bbox.xmin()) * (bbox.xmax() - bbox.xmin()) + (bbox.ymax() - bbox.ymin()) * (bbox.ymax() - bbox.ymin()) + (bbox.zmax() - bbox.zmin()) * (bbox.zmax() - bbox.zmin()));

      m_options = options;

      // epsilon or cluster_epsilon have been set by the user? if not, derive from bounding box diagonal
      m_options.epsilon = (m_options.epsilon < 0) ? bboxDiagonal * 0.01 : m_options.epsilon;
      m_options.cluster_epsilon = (m_options.cluster_epsilon < 0) ? bboxDiagonal * 0.01 : m_options.cluster_epsilon;

      // minimum number of points has been set?
      m_options.min_points = (m_options.min_points >= m_num_available_points) ? (std::size_t)((FT)0.001 * m_num_available_points) : m_options.min_points;
      
      // initializing the shape index
      m_shape_index.assign(m_num_available_points, -1);

      // list of all randomly drawn candidates
      // with the minimum number of points
      std::vector<Shape *> candidates;

      // Identifying minimum number of samples
      std::size_t requiredSamples = 0;
      for (std::size_t i = 0;i<m_shape_factories.size();i++) {
        Shape *tmp = (Shape *) m_shape_factories[i]();
        requiredSamples = (std::max<std::size_t>)(requiredSamples, tmp->minimum_sample_size());
        delete tmp;
      }

      std::size_t firstSample; // first sample for RANSAC

      FT bestExp = 0;

      // number of points that have been assigned to a shape
      std::size_t numInvalid = 0;

      std::size_t nbNewCandidates = 0;
      std::size_t nbFailedCandidates = 0;
      bool forceExit = false;

      do { // main loop
        bestExp = 0;

        do {  //generate candidate
          //1. pick a point p1 randomly among available points
          std::set<std::size_t> indices;
          bool done = false;
          do {
            do 
              firstSample = m_rng() % m_num_available_points;
              while (m_shape_index[firstSample] != -1);
              
            done = m_global_octree->drawSamplesFromCellContainingPoint(
              get(m_point_pmap, 
                  *(m_inputIterator_first + firstSample)),
              select_random_octree_level(),
              indices,
              m_shape_index,
              requiredSamples);

          } while (m_shape_index[firstSample] != -1 || !done);

          nbNewCandidates++;

          //add candidate for each type of primitives
          for(typename std::vector<Shape *(*)()>::iterator it =
            m_shape_factories.begin(); it != m_shape_factories.end(); it++)	{
            Shape *p = (Shape *) (*it)();
            //compute the primitive and says if the candidate is valid
            p->compute(indices,
                       m_inputIterator_first,
                       m_point_pmap,
                       m_normal_pmap,
                       m_options.epsilon, 
                       m_options.normal_threshold);

            if (p->is_valid()) {
              improve_bound(p, m_num_available_points - numInvalid, 1, 500);

              //evaluate the candidate
              if(p->max_bound() >= m_options.min_points) {
                if (bestExp < p->expected_value())
                  bestExp = p->expected_value();

                candidates.push_back(p);
              }
              else {
                nbFailedCandidates++;
                delete p;
              }        
            }
            else {
              nbFailedCandidates++;
              delete p;
            }
          }
          
          if (nbFailedCandidates >= 10000)
            forceExit = true;

        } while( !forceExit
          && stop_probability(bestExp,
                             m_num_available_points - numInvalid, 
                             nbNewCandidates,
                             m_global_octree->maxLevel()) 
                > m_options.probability
          && stop_probability(m_options.min_points,
                             m_num_available_points - numInvalid, 
                             nbNewCandidates,
                             m_global_octree->maxLevel())
                > m_options.probability);
        // end of generate candidate

        if (forceExit) {
          break;
        }

        if (candidates.empty())
          continue;

        // Now get the best candidate in the current set of all candidates
        // Note that the function sorts the candidates:
        //  the best candidate is always the last element of the vector

        Shape *best_Candidate = 
          get_best_candidate(candidates, m_num_available_points - numInvalid);

        if (!best_Candidate)
          continue;

        best_Candidate->m_indices.clear();

        best_Candidate->m_score = 
          m_global_octree->score(best_Candidate,
                                 m_shape_index,
                                 3 * m_options.epsilon,
                                 m_options.normal_threshold);

        best_Candidate->connected_component(best_Candidate->m_indices, m_options.cluster_epsilon);

        if (stop_probability(best_Candidate->expected_value(),
                            (m_num_available_points - numInvalid),
                            nbNewCandidates,
                            m_global_octree->maxLevel())
                <= m_options.probability) {
          //we keep it
          if (best_Candidate->indices_of_assigned_points().size() >=
                       m_options.min_points) {
            candidates.back() = NULL;

            //1. add best candidate to final result.
            m_extracted_shapes.push_back(boost::shared_ptr<Shape>(best_Candidate));

            //2. remove the points
            //2.1 update boolean
            const std::vector<std::size_t> &indices_points_best_candidate =
              best_Candidate->indices_of_assigned_points();

            for (std::size_t i = 0;i<indices_points_best_candidate.size();i++) {
              m_shape_index[indices_points_best_candidate.at(i)] =
                m_extracted_shapes.size() - 1;

              numInvalid++;
              
              for (std::size_t j = 0;j<m_num_subsets;j++) {
                if (m_direct_octrees[j] && m_direct_octrees[j]->m_root) {
                  std::size_t offset = m_direct_octrees[j]->offset();

                  if (offset <= indices_points_best_candidate.at(i) &&
                      (indices_points_best_candidate.at(i) - offset) 
                      < m_direct_octrees[j]->size()) {
                    m_available_octree_sizes[j]--;
                  }
                }
              }
            }

            //2.3 block also the points for the subtrees        

            nbNewCandidates--;
            nbFailedCandidates = 0;
            bestExp = 0;
          }

          std::vector<std::size_t> subsetSizes(m_num_subsets);
          subsetSizes[0] = m_available_octree_sizes[0];
          for (std::size_t i = 1;i<m_num_subsets;i++) {
            subsetSizes[i] = subsetSizes[i-1] + m_available_octree_sizes[i];
          }


          //3. Remove points from candidates common with extracted primitive
          //#pragma omp parallel for
          bestExp = 0;
          for (std::size_t i=0;i< candidates.size()-1;i++) {
            if (candidates[i]) {
              candidates[i]->update_points(m_shape_index);

              if (candidates[i]->max_bound() < m_options.min_points) {
                delete candidates[i];
                candidates[i] = NULL;
              }
              else {
                candidates[i]->compute_bound(
                   subsetSizes[candidates[i]->m_nb_subset_used - 1],
                   m_num_available_points - numInvalid);
                bestExp = (candidates[i]->expected_value() > bestExp) ?
                  candidates[i]->expected_value() : bestExp;
              }
            }
          }

          std::size_t start = 0, end = candidates.size() - 1;
          while (start < end) {
            while (candidates[start] && start < end) start++;
            while (!candidates[end] && start < end) end--;
            if (!candidates[start] && candidates[end] && start < end) {
              candidates[start] = candidates[end];
              candidates[end] = NULL;
              start++;
              end--;
            }
          }

          candidates.resize(end);
        }

      }
      while((stop_probability(m_options.min_points,
                            m_num_available_points - numInvalid,
                            nbNewCandidates, 
                            m_global_octree->maxLevel())
               > m_options.probability
        && FT(m_num_available_points - numInvalid) >= m_options.min_points)
        || bestExp >= m_options.min_points);

      m_num_available_points -= numInvalid;

      return true;
    }

    /// @}

    /// \name Access
    /// @{            
    /*!
      Returns an `Iterator_range` with a bidirectional iterator with value type
      `boost::shared_ptr<Shape>` over the detected shapes in the order of detection.
      Depending on the chosen probability
      for the detection, the shapes are ordered with decreasing size.
    */
    Shape_range shapes() const {
      return make_range(m_extracted_shapes.begin(),
                        m_extracted_shapes.end());
    }
      
    /*! 
      Number of points not assigned to a shape.
    */ 
    std::size_t number_of_unassigned_points() {
      return m_num_available_points;
    }
    
    /*! 
      Returns an `Iterator_range` with a bidirectional iterator with value type `std::size_t`
      as indices into the input data that has not been assigned to a shape. 
    */ 
    Point_index_range indices_of_unassigned_points() {
      Filter_unassigned_points fup(m_shape_index);

      Point_index_iterator p1 =
        boost::make_filter_iterator<Filter_unassigned_points>(
        fup,
        boost::counting_iterator<std::size_t>(0),
        boost::counting_iterator<std::size_t>(m_shape_index.size()));

      return make_range(p1, Point_index_iterator(p1.end()));
    }
    /// @}

  private:
    int select_random_octree_level() {
      return m_rng() % (m_global_octree->maxLevel() + 1);
    }

    Shape* get_best_candidate(std::vector<Shape* >& candidates,
                            const int _SizeP) {
      if (candidates.size() == 1)
        return candidates.back();

      int index_worse_candidate = 0;
      bool improved = true;

      while (index_worse_candidate < (int)candidates.size() - 1 && improved) {
        improved = false;

        typename Shape::Compare_by_max_bound comp;

        std::sort(candidates.begin() + index_worse_candidate,
                  candidates.end(),
                  comp);

        //refine the best one 
        improve_bound(candidates.back(),
                     _SizeP, m_num_subsets,
                     m_options.min_points);

        int position_stop;

        //Take all those intersecting the best one, check for equal ones
        for (position_stop = candidates.size() - 1;
             position_stop > index_worse_candidate;
             position_stop--) {
          if (candidates.back()->min_bound() >
            candidates.at(position_stop)->max_bound())
            break;//the intervals do not overlaps anymore

          if (candidates.at(position_stop)->max_bound()
              <= m_options.min_points)
            break;  //the following candidate doesnt have enough points!

          //if we reach this point, there is an overlap
          //  between best one and position_stop
          //so request refining bound on position_stop
          improved |= improve_bound(candidates.at(position_stop),
                                   _SizeP,
                                   m_num_subsets,
                                   m_options.min_points);

          //test again after refined
          if (candidates.back()->min_bound() >
            candidates.at(position_stop)->max_bound()) 
            break;//the intervals do not overlaps anymore
        }

        index_worse_candidate = position_stop;
      }

      return candidates.back();
    }

    bool improve_bound(Shape *candidate,
                      const int _SizeP,
                      std::size_t max_subset,
                      std::size_t min_points) {
      if (candidate->m_nb_subset_used >= max_subset)
        return false;

      if (candidate->m_nb_subset_used >= m_num_subsets)
        return false;

      candidate->m_nb_subset_used =
        (candidate->m_nb_subset_used >= m_num_subsets) ? 
        m_num_subsets - 1 : candidate->m_nb_subset_used;

      //what it does is add another subset and recompute lower and upper bound
      //the next subset to include is provided by m_nb_subset_used

      std::size_t numPointsEvaluated = 0;
      for (std::size_t i=0;i<candidate->m_nb_subset_used;i++)
        numPointsEvaluated += m_available_octree_sizes[i];

      // need score of new subset as well as sum of
      // the score of the previous considered subset
      std::size_t newScore = 0;
      std::size_t newSampledPoints = 0;

      do {
        newScore = m_direct_octrees[candidate->m_nb_subset_used]->score(
          candidate, 
          m_shape_index, 
          m_options.epsilon,
          m_options.normal_threshold);

        candidate->m_score += newScore;
        
        numPointsEvaluated += 
          m_available_octree_sizes[candidate->m_nb_subset_used];

        newSampledPoints +=
          m_available_octree_sizes[candidate->m_nb_subset_used];

        candidate->m_nb_subset_used++;
      } while (newSampledPoints < min_points &&
        candidate->m_nb_subset_used < m_num_subsets);

      candidate->m_score = candidate->m_indices.size();

      candidate->compute_bound(numPointsEvaluated, _SizeP);

      return true;
    }
    
    inline FT stop_probability(FT _sizeC, FT _np, FT _dC, FT _l) const {
      return (std::min<FT>)(std::pow(1.f - _sizeC / (_np * _l * 3), _dC), 1.);
    }
    
  private:
    Parameters m_options;

    std::mt19937 m_rng;

    Direct_octree **m_direct_octrees;
    Indexed_octree *m_global_octree;
    std::vector<int> m_available_octree_sizes;
    std::size_t m_num_subsets;

    // maps index into points to assigned extracted primitive
    std::vector<int> m_shape_index;
    std::size_t m_num_available_points; 

    //give the index of the subset of point i
    std::vector<int> m_index_subsets;

    std::vector<boost::shared_ptr<Shape> > m_extracted_shapes;

    std::vector<Shape *(*)()> m_shape_factories;

    // iterators of input data
    bool m_valid_iterators;
    Input_iterator m_inputIterator_first, m_inputIterator_beyond; 
    Point_map m_point_pmap;
    Normal_map m_normal_pmap;

    std::vector<FT> m_level_weighting;  // sum must be 1
  };
}
}

#endif // CGAL_SHAPE_DETECTION_3_EFFICIENT_RANSAC_H
