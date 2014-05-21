#ifndef CGAL_INTERNAL_TRIANGULATION_2_IMSERT_CONSTRAINTS_H
#define CGAL_INTERNAL_TRIANGULATION_2_IMSERT_CONSTRAINTS_H

namespace CGAL {
  namespace internal {



    template <class T, class IndicesIterator>
    std::size_t insert_constraints( T& t,
                                    const std::vector<typename T::Point>& points,
                                    IndicesIterator indices_first,
                                    IndicesIterator indices_beyond )
  {
    typedef typename T::Vertex_handle Vertex_handle;
    typedef typename T::Face_handle Face_handle;
    typedef typename T::Geom_traits Geom_traits;
    typedef std::vector<std::ptrdiff_t> Vertex_indices;
    typedef std::vector<Vertex_handle> Vertices;
    
    Vertex_indices vertex_indices;
    vertex_indices.resize(points.size());

    std::copy(boost::counting_iterator<std::ptrdiff_t>(0),
              boost::counting_iterator<std::ptrdiff_t>(points.size()),
              std::back_inserter(vertex_indices));

    T::size_type n = t.number_of_vertices();
    Spatial_sort_traits_adapter_2<Geom_traits, const Point*> sort_traits(&(points[0]));

    spatial_sort(vertex_indices.begin(), vertex_indices.end(), sort_traits);

    Vertices vertices;
    vertices.resize(points.size());

    Face_handle hint;
    for(typename Vertex_indices::const_iterator
          it_pti = vertex_indices.begin(), end = vertex_indices.end();
          it_pti != end; ++it_pti)
    {
      vertices[*it_pti] = t.insert(points[*it_pti], hint);
      hint=vertices[*it_pti]->face();
    }

    for(IndicesIterator it_cst=indices_first, end=indices_beyond;
        it_cst!=end; ++it_cst)
    {
      Vertex_handle v1 = vertices[it_cst->first];
      Vertex_handle v2 = vertices[it_cst->second];
      if(v1 != v2) t.insert_constraint(v1, v2);
    }

    return t.number_of_vertices() - n;
  }



    template <class T,class ConstraintIterator>
    std::size_t insert_constraints(T& t,
                                   ConstraintIterator first,
                                   ConstraintIterator beyond)
  {
    typedef typename T::Point Point;
    typedef typename T::Point Point;
    std::vector<Point> points;
    for (ConstraintIterator s_it=first; s_it!=beyond; ++s_it)
    {
      points.push_back( T::get_source(*s_it) );
      points.push_back( T::get_target(*s_it) );
    }

    std::vector< std::pair<std::size_t, std::size_t> > segment_indices;
    std::size_t nb_segments = points.size() / 2;
    segment_indices.reserve( nb_segments );
    for (std::size_t k=0; k < nb_segments; ++k)
      segment_indices.push_back( std::make_pair(2*k,2*k+1) );

    return insert_constraints( t,
                               points,
                               segment_indices.begin(),
                               segment_indices.end() );
  }


  }
}

#endif // CGAL_INTERNAL_TRIANGULATION_2_IMSERT_CONSTRAINTS_H
