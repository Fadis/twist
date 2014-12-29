#ifndef TWIST_ITERATOR_LOWEST_TRAVERSAL_HPP
#define TWIST_ITERATOR_LOWEST_TRAVERSAL_HPP

#include <boost/mpl/find_if.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/mpl/vector.hpp>
#include <twist/iterator/traversal.hpp>

namespace twist {
  namespace iterator {
    template< typename Vector, typename Enable = void >
    struct lowest_traversal {};
    
    template< typename Vector, typename Traversal >
    struct all_iterators_are_lower_than :
      public boost::mpl::not_<
        boost::is_same<
          typename boost::mpl::find_if<
            Vector,
            twist::iterator::is_traversal_of< Traversal, boost::mpl::_1 >
          >::type,
          typename boost::mpl::end<
            Vector
          >::type
        >
      > {};

    template< typename Vector >
    struct lowest_traversal<
      Vector,
      typename boost::enable_if<
        all_iterators_are_lower_than< Vector, boost::incrementable_traversal_tag >
      >::type
    > {
      typedef boost::incrementable_traversal_tag type;
    };

    template< typename Vector >
    struct lowest_traversal<
      Vector,
      typename boost::enable_if<
        boost::mpl::and_<
          boost::mpl::not_< all_iterators_are_lower_than< Vector, boost::incrementable_traversal_tag > >,
          all_iterators_are_lower_than< Vector, boost::single_pass_traversal_tag >
        >
      >::type
    > {
      typedef boost::single_pass_traversal_tag type;
    };
    
    template< typename Vector >
    struct lowest_traversal<
      Vector,
      typename boost::enable_if<
        boost::mpl::and_<
          boost::mpl::not_< all_iterators_are_lower_than< Vector, boost::incrementable_traversal_tag > >,
          boost::mpl::not_< all_iterators_are_lower_than< Vector, boost::single_pass_traversal_tag > >,
          all_iterators_are_lower_than< Vector, boost::forward_traversal_tag >
        >
      >::type
    > {
      typedef boost::forward_traversal_tag type;
    };

    template< typename Vector >
    struct lowest_traversal<
      Vector,
      typename boost::enable_if<
        boost::mpl::and_<
          boost::mpl::not_< all_iterators_are_lower_than< Vector, boost::incrementable_traversal_tag > >,
          boost::mpl::not_< all_iterators_are_lower_than< Vector, boost::single_pass_traversal_tag > >,
          boost::mpl::not_< all_iterators_are_lower_than< Vector, boost::forward_traversal_tag > >,
          all_iterators_are_lower_than< Vector, boost::bidirectional_traversal_tag >
        >
      >::type
    > {
      typedef boost::bidirectional_traversal_tag type;
    };

    template< typename Vector >
    struct lowest_traversal<
      Vector,
      typename boost::enable_if<
        boost::mpl::and_<
          boost::mpl::not_< all_iterators_are_lower_than< Vector, boost::incrementable_traversal_tag > >,
          boost::mpl::not_< all_iterators_are_lower_than< Vector, boost::single_pass_traversal_tag > >,
          boost::mpl::not_< all_iterators_are_lower_than< Vector, boost::forward_traversal_tag > >,
          boost::mpl::not_< all_iterators_are_lower_than< Vector, boost::bidirectional_traversal_tag > >,
          all_iterators_are_lower_than< Vector, boost::random_access_traversal_tag >
        >
      >::type
    > {
      typedef boost::random_access_traversal_tag type;
    };

    template< typename Vector, typename Enable = void >
    struct lowest_traversal_tag {};

    template< typename Vector, typename Traversal >
    struct all_tags_are_lower_than :
      public boost::mpl::not_<
        boost::is_same<
          typename boost::mpl::find_if<
            Vector,
            boost::is_convertible< Traversal, boost::mpl::_1 >
          >::type,
          typename boost::mpl::end<
            Vector
          >::type
        >
      > {};

    template< typename Vector >
    struct lowest_traversal_tag<
      Vector,
      typename boost::enable_if<
        all_tags_are_lower_than< Vector, boost::incrementable_traversal_tag >
      >::type
    > {
      typedef boost::incrementable_traversal_tag type;
    };

    template< typename Vector >
    struct lowest_traversal_tag<
      Vector,
      typename boost::enable_if<
        boost::mpl::and_<
          boost::mpl::not_< all_tags_are_lower_than< Vector, boost::incrementable_traversal_tag > >,
          all_tags_are_lower_than< Vector, boost::single_pass_traversal_tag >
        >
      >::type
    > {
      typedef boost::single_pass_traversal_tag type;
    };
    
    template< typename Vector >
    struct lowest_traversal_tag<
      Vector,
      typename boost::enable_if<
        boost::mpl::and_<
          boost::mpl::not_< all_tags_are_lower_than< Vector, boost::incrementable_traversal_tag > >,
          boost::mpl::not_< all_tags_are_lower_than< Vector, boost::single_pass_traversal_tag > >,
          all_tags_are_lower_than< Vector, boost::forward_traversal_tag >
        >
      >::type
    > {
      typedef boost::forward_traversal_tag type;
    };

    template< typename Vector >
    struct lowest_traversal_tag<
      Vector,
      typename boost::enable_if<
        boost::mpl::and_<
          boost::mpl::not_< all_tags_are_lower_than< Vector, boost::incrementable_traversal_tag > >,
          boost::mpl::not_< all_tags_are_lower_than< Vector, boost::single_pass_traversal_tag > >,
          boost::mpl::not_< all_tags_are_lower_than< Vector, boost::forward_traversal_tag > >,
          all_tags_are_lower_than< Vector, boost::bidirectional_traversal_tag >
        >
      >::type
    > {
      typedef boost::bidirectional_traversal_tag type;
    };

    template< typename Vector >
    struct lowest_traversal_tag<
      Vector,
      typename boost::enable_if<
        boost::mpl::and_<
          boost::mpl::not_< all_tags_are_lower_than< Vector, boost::incrementable_traversal_tag > >,
          boost::mpl::not_< all_tags_are_lower_than< Vector, boost::single_pass_traversal_tag > >,
          boost::mpl::not_< all_tags_are_lower_than< Vector, boost::forward_traversal_tag > >,
          boost::mpl::not_< all_tags_are_lower_than< Vector, boost::bidirectional_traversal_tag > >,
          all_tags_are_lower_than< Vector, boost::random_access_traversal_tag >
        >
      >::type
    > {
      typedef boost::random_access_traversal_tag type;
    };
  }
}

#endif

