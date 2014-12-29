#ifndef TWIST_ITERATOR_TRAVERSAL_HPP
#define TWIST_ITERATOR_TRAVERSAL_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_concepts.hpp>
#include <boost/utility/enable_if.hpp>

#include <twist/type_traits/is_iterator.hpp>

namespace twist {
  namespace iterator {
    template< typename Iterator, typename Enable = void >
    struct is_convertible_to_incrementable_traversal;
    template< typename Iterator >
    struct is_convertible_to_incrementable_traversal<
      Iterator,
      typename boost::disable_if< type_traits::is_iterator< Iterator > >::type
    > :  public boost::mpl::bool_< false > {};
    template< typename Iterator >
    struct is_convertible_to_incrementable_traversal<
      Iterator,
      typename boost::enable_if< type_traits::is_iterator< Iterator > >::type
    > : 
      public boost::is_convertible<
        typename boost::iterator_traversal< Iterator >::type,
        boost::incrementable_traversal_tag
      > {};

    template< typename Iterator, typename Enable = void >
    struct is_convertible_to_single_pass_traversal;
    template< typename Iterator >
    struct is_convertible_to_single_pass_traversal<
      Iterator,
      typename boost::disable_if< type_traits::is_iterator< Iterator > >::type
    > : public boost::mpl::bool_< false > {};
    template< typename Iterator >
    struct is_convertible_to_single_pass_traversal<
      Iterator,
      typename boost::enable_if< type_traits::is_iterator< Iterator > >::type
    > :
      public boost::is_convertible<
        typename boost::iterator_traversal< Iterator >::type,
        boost::single_pass_traversal_tag
      > {};

    template< typename Iterator, typename Enable = void >
    struct is_convertible_to_forward_traversal;
    template< typename Iterator >
    struct is_convertible_to_forward_traversal<
      Iterator,
      typename boost::disable_if< type_traits::is_iterator< Iterator > >::type
    > : public boost::mpl::bool_< false > {}; 
    template< typename Iterator >
    struct is_convertible_to_forward_traversal<
      Iterator,
      typename boost::enable_if< type_traits::is_iterator< Iterator > >::type
    > : 
      public boost::is_convertible<
        typename boost::iterator_traversal< Iterator >::type,
        boost::forward_traversal_tag
      > {};

    template< typename Iterator, typename Enable = void >
    struct is_convertible_to_bidirectional_traversal;
    template< typename Iterator >
    struct is_convertible_to_bidirectional_traversal<
      Iterator,
      typename boost::disable_if< type_traits::is_iterator< Iterator > >::type
    > : public boost::mpl::bool_< false > {};
    template< typename Iterator >
    struct is_convertible_to_bidirectional_traversal<
      Iterator,
      typename boost::enable_if< type_traits::is_iterator< Iterator > >::type
    > :
      public boost::is_convertible<
        typename boost::iterator_traversal< Iterator >::type,
        boost::bidirectional_traversal_tag
      > {};

    template< typename Iterator, typename Enable = void >
    struct is_convertible_to_random_access_traversal;
    template< typename Iterator >
    struct is_convertible_to_random_access_traversal<
      Iterator,
      typename boost::disable_if< type_traits::is_iterator< Iterator > >::type
    > : public boost::mpl::bool_< false >  {};
    template< typename Iterator >
    struct is_convertible_to_random_access_traversal<
      Iterator,
      typename boost::enable_if< type_traits::is_iterator< Iterator > >::type
    > : 
      public boost::is_convertible<
        typename boost::iterator_traversal< Iterator >::type,
        boost::random_access_traversal_tag
      > {};

    template< typename Tag >
    struct is_convertible_to_incrementable_traversal_tag : 
      public boost::is_convertible<
        Tag,
        boost::incrementable_traversal_tag
      > {};
    template< typename Tag >
    struct is_convertible_to_single_pass_traversal_tag : 
      public boost::is_convertible<
        Tag,
        boost::single_pass_traversal_tag
      > {};
    template< typename Tag >
    struct is_convertible_to_forward_traversal_tag : 
      public boost::is_convertible<
        Tag,
        boost::forward_traversal_tag
      > {};
    template< typename Tag >
    struct is_convertible_to_bidirectional_traversal_tag : 
      public boost::is_convertible<
        Tag,
        boost::bidirectional_traversal_tag
      > {};
    template< typename Tag >
    struct is_convertible_to_random_access_traversal_tag : 
      public boost::is_convertible<
        Tag,
        boost::random_access_traversal_tag
      > {};

    template< typename Iterator >
    struct is_incrementable_traversal : 
      public boost::mpl::and_<
        is_convertible_to_incrementable_traversal< Iterator >,
        boost::mpl::not_<
          is_convertible_to_single_pass_traversal< Iterator >
        >
      > {};
    template< typename Iterator >
    struct is_single_pass_traversal : 
      public boost::mpl::and_<
        is_convertible_to_single_pass_traversal< Iterator >,
        boost::mpl::not_<
          is_convertible_to_forward_traversal< Iterator >
        >
      > {};
    template< typename Iterator >
    struct is_forward_traversal : 
      public boost::mpl::and_<
        is_convertible_to_forward_traversal< Iterator >,
        boost::mpl::not_<
          is_convertible_to_bidirectional_traversal< Iterator >
        >
      > {};
    template< typename Iterator >
    struct is_bidirectional_traversal : 
      public boost::mpl::and_<
        is_convertible_to_bidirectional_traversal< Iterator >,
        boost::mpl::not_<
          is_convertible_to_random_access_traversal< Iterator >
        >
      > {};
    template< typename Iterator >
    struct is_random_access_traversal : 
      public is_convertible_to_random_access_traversal< Iterator > {};

    template< typename Tag >
    struct is_incrementable_traversal_tag : 
      public boost::mpl::and_<
        is_convertible_to_incrementable_traversal_tag< Tag >,
        boost::mpl::not_<
          is_convertible_to_single_pass_traversal_tag< Tag >
        >
      > {};
    template< typename Tag >
    struct is_single_pass_traversal_tag : 
      public boost::mpl::and_<
        is_convertible_to_single_pass_traversal_tag< Tag >,
        boost::mpl::not_<
          is_convertible_to_forward_traversal_tag< Tag >
        >
      > {};
    template< typename Tag >
    struct is_forward_traversal_tag : 
      public boost::mpl::and_<
        is_convertible_to_forward_traversal_tag< Tag >,
        boost::mpl::not_<
          is_convertible_to_bidirectional_traversal_tag< Tag >
        >
      > {};
    template< typename Tag >
    struct is_bidirectional_traversal_tag : 
      public boost::mpl::and_<
        is_convertible_to_bidirectional_traversal_tag< Tag >,
        boost::mpl::not_<
          is_convertible_to_random_access_traversal_tag< Tag >
        >
      > {};
    template< typename Tag >
    struct is_random_access_traversal_tag : 
      public is_convertible_to_random_access_traversal_tag< Tag > {};
    
    
    template< typename Traversal, typename Iterator >
    struct is_traversal_of {};
    template< typename Iterator >
    struct is_traversal_of< boost::incrementable_traversal_tag, Iterator > :
      public is_incrementable_traversal< Iterator > {};
    template< typename Iterator >
    struct is_traversal_of< boost::single_pass_traversal_tag, Iterator > :
      public is_single_pass_traversal< Iterator > {};
    template< typename Iterator >
    struct is_traversal_of< boost::forward_traversal_tag, Iterator > :
      public is_forward_traversal< Iterator > {};
    template< typename Iterator >
    struct is_traversal_of< boost::bidirectional_traversal_tag, Iterator > :
      public is_bidirectional_traversal< Iterator > {};
    template< typename Iterator >
    struct is_traversal_of< boost::random_access_traversal_tag, Iterator > :
      public is_random_access_traversal< Iterator > {};

    template< typename Tag, typename Enable = void >
    struct get_pure_traversal {};
    template< typename Tag >
    struct get_pure_traversal<
      Tag,
      typename boost::enable_if< is_incrementable_traversal_tag< Tag > >::type
    > {
      typedef boost::incrementable_traversal_tag type;
    };
    template< typename Tag >
    struct get_pure_traversal<
      Tag,
      typename boost::enable_if< is_single_pass_traversal_tag< Tag > >::type
    > {
      typedef boost::single_pass_traversal_tag type;
    };
    template< typename Tag >
    struct get_pure_traversal<
      Tag,
      typename boost::enable_if< is_forward_traversal_tag< Tag > >::type
    > {
      typedef boost::forward_traversal_tag type;
    };
    template< typename Tag >
    struct get_pure_traversal<
      Tag,
      typename boost::enable_if< is_bidirectional_traversal_tag< Tag > >::type
    > {
      typedef boost::bidirectional_traversal_tag type;
    };
    template< typename Tag >
    struct get_pure_traversal<
      Tag,
      typename boost::enable_if< is_random_access_traversal_tag< Tag > >::type
    > {
      typedef boost::random_access_traversal_tag type;
    };
  }
  
}

#endif

