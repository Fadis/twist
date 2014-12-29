#ifndef TWIST_ITERATOR_ADAPTOR_SHADOWED_HPP
#define TWIST_ITERATOR_ADAPTOR_SHADOWED_HPP

#include <memory>
#include <iterator>
#include <boost/optional.hpp>
#include <boost/array.hpp>
#include <boost/fusion/adapted/boost_array.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <twist/iterator/traversal.hpp>

namespace twist {
  namespace iterator {
    namespace adaptor {
      template<
        typename Traversal,
        typename ReferenceType,
        typename Enable = void
      > class shadowed {};

      template< typename BaseIterator >
      struct get_shadowed_iterator {
        typedef shadowed<
          typename twist::iterator::get_pure_traversal<
            typename boost::iterator_traversal< BaseIterator >::type
          >::type,
          typename boost::iterator_reference< BaseIterator >::type
        > type;
      };
      template< typename Traversal, typename ReferenceType >
      struct shadowed_value_type {
        typedef typename boost::remove_reference< ReferenceType >::type type;
      };
      
      template< typename Traversal, typename ReferenceType >
      struct shadowed_base_type {
        typedef boost::iterator_facade<
          shadowed< Traversal, ReferenceType >,
          typename shadowed_value_type< Traversal, ReferenceType >::type,
          Traversal,
          ReferenceType
        > type;
      };
     
      template< typename Traversal, typename ReferenceType >
      class shadowed_internal_base {
      public:
        typedef shadowed_internal_base< Traversal, ReferenceType > base_type;
        typedef typename shadowed_value_type< Traversal, ReferenceType >::type value_type;
        shadowed_internal_base() {}
        virtual ReferenceType dereference() const = 0;
        virtual void increment() = 0;
        virtual void decrement() = 0;
        virtual bool equal( const base_type &y ) const = 0;
        virtual void advance( int n ) = 0;
        virtual ptrdiff_t distance_to( const base_type &z ) const = 0;
        virtual std::shared_ptr< base_type > copy() const = 0;
      };
      template< typename BaseIterator >
      class shadowed_internal : public shadowed_internal_base<
        typename twist::iterator::get_pure_traversal<
          typename boost::iterator_traversal< BaseIterator >::type
        >::type,
        typename boost::iterator_reference< BaseIterator >::type
      > {
      public:
        typedef shadowed_internal< BaseIterator > self_type;
        typedef shadowed_internal_base<
          typename twist::iterator::get_pure_traversal<
            typename boost::iterator_traversal< BaseIterator >::type
          >::type,
          typename boost::iterator_reference< BaseIterator >::type
        > base_type;
        typedef typename shadowed_value_type<
          typename twist::iterator::get_pure_traversal<
            typename boost::iterator_traversal< BaseIterator >::type
          >::type,
          typename boost::iterator_reference< BaseIterator >::type
        >::type value_type;
        shadowed_internal() {}
        shadowed_internal( const BaseIterator &iter_ ) : iter( iter_ ) {}
        virtual typename boost::iterator_reference< BaseIterator >::type dereference() const {
          return *iter;
        }
        virtual void increment() {
          ++iter;
        }
        virtual void decrement() {
          --iter;
        }
        virtual bool equal( const base_type &y ) const {
          try {
            const self_type &casted = dynamic_cast< const self_type& >( y );
            return iter == casted.iter;
          }
          catch( const std::bad_cast& ) {
            return false;
          }
        }
        virtual void advance( int n ) {
          std::advance( iter, n );
        }
        virtual ptrdiff_t distance_to( const base_type &z ) const {
          const self_type &casted = dynamic_cast< const self_type& >( z );
          return std::distance( iter, casted.iter );
        }
        virtual std::shared_ptr< base_type > copy() const {
          return std::shared_ptr< base_type >( new self_type( iter ) );
        }
      private:
        BaseIterator iter;
      };
      template< typename Traversal, typename ReferenceType >
      class shadowed<
        Traversal, ReferenceType,
        typename boost::enable_if<
          twist::iterator::is_convertible_to_forward_traversal_tag< Traversal >
        >::type
      > :
        public shadowed_base_type< Traversal, ReferenceType >::type {
      public:
        typedef shadowed<
          Traversal, ReferenceType,
          typename boost::enable_if<
            twist::iterator::is_convertible_to_forward_traversal_tag< Traversal >
          >::type
        > self_type;
        typedef shadowed_internal_base< Traversal, ReferenceType > internal_type;
        typedef std::shared_ptr< internal_type > internal_pointer;
        shadowed() {}
        template< typename BaseIterator >
        shadowed( const BaseIterator &iter ) : internal( new shadowed_internal< BaseIterator >( iter ) ) {}
        shadowed( const self_type &src ) : internal( src.internal ? src.internal->copy() : nullptr ) {}
        shadowed( self_type && ) = default;
        template< typename BaseIterator >
        shadowed &operator=( const BaseIterator &iter ) {
          internal.reset( new shadowed_internal< BaseIterator >( iter ) );
          return *this;
        }
        shadowed &operator=( const self_type &src ) {
          internal = src.internal.copy();
          return *this;
        }
        shadowed &operator=( self_type &&src ) = default;
        ReferenceType dereference() const {
          return internal->dereference();
        }
        void increment() {
          internal->increment();
        }
        void decrement() {
          internal->decrement();
        }
        bool equal( const self_type &y ) const {
          return internal->equal( *y.internal );
        }
        template< typename T = void >
        void advance( int n ) {
          internal->advance( n );
        }
        ptrdiff_t
        distance_to( const self_type &z ) const {
          return internal->distance_to( *z.internal );
        }
      private:
        internal_pointer internal;
      };
      template< typename BaseIterator >
      typename get_shadowed_iterator< BaseIterator >::type
      shadow( const BaseIterator &base ) {
        return typename get_shadowed_iterator< BaseIterator >::type( base );
      }
    }
  }
}

#endif

