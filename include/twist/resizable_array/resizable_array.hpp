#ifndef TWIST_FUSION_RESIZABLE_ARRAY_HPP
#define TWIST_FUSION_RESIZABLE_ARRAY_HPP

#include <array>
#include <initializer_list>
#include <stdexcept>
#include <iterator>
#include <boost/range/iterator_range.hpp>
#include <boost/utility/enable_if.hpp>

#include <twist/iterator/traversal.hpp>

namespace twist {
  template< typename T, size_t max_size_ >
  class resizable_array {
  public:
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator< iterator > reverse_iterator;
    typedef std::reverse_iterator< const_iterator > const_reverse_iterator;

    resizable_array() : size_( 0u ) {}
    resizable_array( const std::initializer_list<T> &init ) : size_( 0u ) {
      const size_t incoming_size = size_t( std::distance( init.begin(), init.end() ) );
      if( incoming_size > max_size_ )
        throw std::out_of_range( "resizable_array( init ) : The initializer_list is larger than max size of resizable_array." );
      for( const auto &elem : init ) {
        new ( end() ) T( elem );
        ++size_;
      }
    }
    template< typename Iterator >
    resizable_array(
      Iterator begin, Iterator end,
      typename boost::enable_if< twist::iterator::is_convertible_to_single_pass_traversal< Iterator > >::type* = 0
    ) : size_( 0u ) {
      size_t incoming_size = size_t( std::distance( begin, end ) );
      if( incoming_size > max_size_ )
        throw std::out_of_range( "resizable_array( begin, end ) : The iterator range is larger than max size of resizable_array." );
      for( const auto &elem : boost::make_iterator_range( begin, end ) ) {
        new ( end() ) T( elem );
        ++size_;
      }
    }
    resizable_array( const resizable_array &src ) : size_( 0u ) {
      for( const auto &elem : src ) {
        new ( end() ) T( elem );
        ++size_;
      }
    }
    resizable_array( resizable_array &&src ) : size_( 0u ) {
      for( const auto &elem : src ) {
        new ( end() ) T( std::move( elem ) );
        ++size_;
      }
    }
    resizable_array &operator=( const resizable_array &src ) {
      size_ = 0u;
      for( const auto &elem : src ) {
        new ( end() ) T( elem );
        ++size_;
      }
      return *this;
    }
    resizable_array &operator=( resizable_array &&src ) {
      size_ = 0u;
      for( const auto &elem : src ) {
        new ( end() ) T( std::move( elem ) );
        ++size_;
      }
      return *this;
    }
    reference at( size_type pos ) {
      if( pos >= size() )
        throw std::out_of_range( "at(pos) : pos must be smaller than size()." );
      return *std::next( begin(), pos );
    }
    const_reference at( size_type pos ) const {
      if( pos >= size() )
        throw std::out_of_range( "at(pos) : pos must be smaller than size()." );
      return *std::next( begin(), pos );
    }
    reference operator[]( size_type pos ) {
      return *std::next( begin(), pos );
    }
    const_reference operator[]( size_type pos ) const {
      return *std::next( begin(), pos );
    }
    reference front() {
      return *begin();
    }
    const_reference front() const {
      return *begin();
    }
    reference back() {
      return *std::next( begin(), size() - 1 );
    }
    const_reference back() const {
      return *std::next( begin(), size() - 1 );
    }
    pointer data() {
      return reinterpret_cast< pointer >( base );
    }
    const_pointer data() const {
      return reinterpret_cast< const_pointer >( base );
    }
    iterator begin() {
      return reinterpret_cast< pointer >( base );
    }
    const_iterator begin() const {
      return reinterpret_cast< const_pointer >( base );
    }
    const_iterator cbegin() const {
      return reinterpret_cast< const_pointer >( base );
    }
    iterator end() { return std::next( begin(), size_ ); }
    const_iterator end() const { return std::next( begin(), size_ ); }
    const_iterator cend() const { return std::next( cbegin(), size_ ); }
    
    reverse_iterator rbegin() {
      return reverse_iterator( std::next( begin(), size() - 1 ) );
    }
    const_reverse_iterator rbegin() const {
      return const_reverse_iterator( std::next( begin(), size() - 1 ) );
    }
    const_reverse_iterator crbegin() const {
      return const_reverse_iterator( std::next( begin(), size() - 1 ) );
    }

    reverse_iterator rend() {
      return reverse_iterator( std::prev( begin(), 1 ) );
    }
    const_reverse_iterator rend() const {
      return const_reverse_iterator( std::prev( begin(), 1 ) );
    }
    const_reverse_iterator crend() const {
      return const_reverse_iterator( std::prev( cbegin(), 1 ) );
    }
    bool empty() const { return size_ == 0u; }
    size_type size() const { return size_; }
    constexpr size_type max_size() const { return max_size_; }
    void fill( const_reference value ) {
      std::fill( begin(), end(), value );
      base.fill( value );
    }
    template< typename OtherType, size_t other_size >
    bool operator==( const resizable_array< OtherType, other_size > &other ) const {
      if( size_ != other.size() )
        return false;
      else 
        return std::equal( begin(), end(), other.begin() );
    }
    template< typename OtherType, size_t other_size >
    bool operator!=( const resizable_array< OtherType, other_size > &other ) const {
      if( size_ != other.size() )
        return true;
      else 
        return std::mismatch( begin(), end(), other.begin() ) != std::make_pair( end(), other.end() );
    }
    template< typename OtherType, size_t other_size >
    bool operator<( const resizable_array< OtherType, other_size > &other ) const {
      return std::lexicographical_compare( begin(), end(), other.begin(), other.end());
    }
    template< typename OtherType, size_t other_size >
    bool operator>( const resizable_array< OtherType, other_size > &other ) const {
      return std::lexicographical_compare( other.begin(), other.end(), begin(), end());
    }
    template< typename OtherType, size_t other_size >
    bool operator<=( const resizable_array< OtherType, other_size > &other ) const {
      return !std::lexicographical_compare( other.begin(), other.end(), begin(), end());
    }
    template< typename OtherType, size_t other_size >
    bool operator>=( const resizable_array< OtherType, other_size > &other ) const {
      return !std::lexicographical_compare( begin(), end(), other.begin(), other.end());
    }

    void resize ( size_type n ) {
      if( n > max_size() )
        throw std::out_of_range( "The required size is larger than max size of resizable_array." );
      if( n > size_ )
        for( iterator iter = std::next( begin(), size_ ); iter != std::next( begin(), n ); ++iter )
          new ( iter ) value_type();
      size_ = n;
    }
    void resize ( size_type n, const value_type& val ) {
      if( n > max_size() )
        throw std::out_of_range( "The required size is larger than max size of resizable_array." );
      if( n > size_ )
        for( iterator iter = std::next( begin(), size_ ); iter != std::next( begin(), n ); ++iter )
          new ( iter ) value_type( val );
      size_ = n;
    }
    constexpr size_type capacity() const {
      return max_size_;
    }
    void push_back( const value_type& val ) {
      if( size_ == max_size() )
        throw std::out_of_range( "No space left to push_back on the resizable_array." );
      new ( end() ) value_type( val );
      ++size_;
    }
    void push_back( value_type&& val ) {
      if( size_ == max_size() )
        throw std::out_of_range( "No space left to push_back on the resizable_array." );
      new ( end() ) value_type( std::move( val ) );
      ++size_;
    }
    void pop_back() {
      if( size_ == 0u )
        throw std::out_of_range( "Empty resizable_array cannot pop_back." );
      --size_;
      end()->~value_type();
    }
    void assign( std::initializer_list< value_type > init ) {
      clear();
      insert( begin(), init.begin(), init.end() );
    }
    void assign( size_type n, const value_type &val ) {
      clear();
      insert( begin(), n, val );
    }
    template< typename Iterator >
    void assign(
      Iterator first, Iterator last,
      typename boost::enable_if< twist::iterator::is_convertible_to_single_pass_traversal< Iterator > >::type* = 0
    ) {
      clear();
      insert( begin(), first, last );
    }
    void insert( const_iterator position_, const value_type& val ) {
      if( size_ == max_size() )
        throw std::out_of_range( "No space left to insert on the resizable_array." );
      if( position_ == end() ) {
        push_back( val );
      }
      else {
        iterator position = std::next( begin(), std::distance( begin(), position_ ) );
        create_space_to_insert( position, 1 );
        *position = val;
        ++size_;
        return position;
      }
    }
    iterator insert ( const_iterator position_, size_type n, const value_type& val ) {
      if( size_ + n > max_size() )
        throw std::out_of_range( "No space left to insert on the resizable_array." );
      if( position_ == end() ) {
        auto oiter = end();
        for( size_t count = 0u; count != n; ++count, ++oiter )
          new ( oiter ) value_type( val );
        iterator position = end();
        size_ += n;
        return position;
      }
      else {
        iterator position = std::next( begin(), std::distance( cbegin(), position_ ) );
        create_space_to_insert( position, n );
        const auto output_begin = position;
        const auto output_end = std::next( position, n );
        const auto move_output_begin = std::min( output_begin, end() );
        const auto move_output_end = std::min( output_end, end() );
        const auto create_output_begin = std::max( output_begin, end() );
        const auto create_output_end = std::max( output_end, end() );
        if( create_output_begin < create_output_end ) {
          auto oiter = create_output_begin;
          const auto oend = create_output_end;
          for( ; oiter != oend; ++oiter )
            new ( oiter ) value_type( val );

        }
        if( move_output_begin < move_output_end ) {
          auto oiter = move_output_begin;
          const auto oend = move_output_end;
          for( ; oiter != oend; ++oiter )
            new ( oiter ) value_type( val );
        }
        size_ += n;
        return position;
      }
    }
    template < typename Iterator >
    iterator insert(
      const_iterator position_, Iterator first, Iterator last,
      typename boost::enable_if< twist::iterator::is_convertible_to_single_pass_traversal< Iterator > >::type* = 0
    ) {
      size_t n = std::distance( first, last );
      if( size_ + n > max_size() )
        throw std::out_of_range( "No space left to insert on the resizable_array." );
      if( position_ == end() ) {
        auto iiter = first;
        const auto iend = last;
        auto oiter = end();
        for( ; iiter != iend; ++iiter, ++oiter )
          new ( oiter ) value_type( *iiter );
        iterator position = end();
        size_ += n;
        return position;
      }
      else {
        iterator position = std::next( begin(), std::distance( cbegin(), position_ ) );
        create_space_to_insert( position, n );
        const auto output_begin = position;
        const auto output_end = std::next( position, n );
        const auto move_output_begin = std::min( output_begin, end() );
        const auto move_output_end = std::min( output_end, end() );
        const auto create_output_begin = std::max( output_begin, end() );
        const auto create_output_end = std::max( output_end, end() );
        auto iiter = first;
        if( create_output_begin < create_output_end ) {
          auto oiter = create_output_begin;
          const auto oend = create_output_end;
          for( ; oiter != oend; ++iiter, ++oiter )
            new ( oiter ) value_type( *iiter );

        }
        if( move_output_begin < move_output_end ) {
          auto oiter = move_output_begin;
          const auto oend = move_output_end;
          for( ; oiter != oend; ++iiter, ++oiter )
            new ( oiter ) value_type( *iiter );
        }
        size_ += n;
        return position;
      }
    }
    iterator insert ( const_iterator position_, value_type&& val ) {
      if( size_ == max_size() )
        throw std::out_of_range( "No space left to insert on the resizable_array." );
      iterator position = std::next( begin(), std::distance( cbegin(), position_ ) );
      if( position == end() ) {
        push_back( val );
        return position;
      }
      else {
        create_space_to_insert( position, 1 );
        *position = std::move( val );
        ++size_;
        return position;
      }
    }
    iterator insert ( const_iterator position, std::initializer_list<value_type> il ) {
      return insert( position, il.begin(), il.end() );
    }
    iterator erase ( const_iterator position_ ) {
      iterator position = std::next( begin(), std::distance( cbegin(), position_ ) );
      std::move(
        std::next( position, 1 ),
        end(),
        position
      );
      std::next( begin(), size_ - 1 )->~value_type();
      --size_;
      return position;
    }
    iterator erase ( const_iterator first_, const_iterator last_ ) {
      const iterator first = std::next( begin(), std::distance( cbegin(), first_ ) );
      const iterator last = std::next( begin(), std::distance( cbegin(), last_ ) );
      const size_t n = std::distance( first, last );
      std::move(
        last,
        end(),
        first
      );
      for( auto iter = last; iter != end(); ++iter )
        iter->~value_type();
      size_ -= n;
      return first;
    }
    void clear() {
      for( auto iter = begin(); iter != end(); ++iter )
        iter->~value_type();
      size_ = 0;
    }
    template < typename ... Args >
    iterator emplace ( const_iterator position_, Args && ... args ) {
      if( size_ == max_size() )
        throw std::out_of_range( "No space left to insert on the resizable_array." );
      iterator position = std::next( begin(), std::distance( cbegin(), position_ ) );
      if( position_ == end() ) {
        emplace_back( args... );
        return position;
      }
      else {
        create_space_to_insert( position, 1 );
        *position = value_type( args... );
        ++size_;
        return position;
      }
    }
    template < typename ... Args >
    void emplace_back ( Args && ... args ) {
      if( size_ == max_size() )
        throw std::out_of_range( "No space left to emplace on the resizable_array." );
      new ( end() ) value_type( args... );
      ++size_;
    }
    void swap( resizable_array< T, max_size_ > &other ) { return swap_( other ); }
  private:
    void create_space_to_insert( iterator position, size_type n ) {
      const auto input_begin = position;
      const auto input_end = end();
      const auto output_begin = std::next( input_begin, n );
      const auto output_end = std::next( input_end, n );
      const auto move_output_begin = std::min( output_begin, end() );
      const auto move_output_end = std::min( output_end, end() );
      const auto create_output_begin = std::max( output_begin, end() );
      const auto create_output_end = std::max( output_end, end() );
      const auto move_input_begin = std::prev( move_output_begin, n );
      const auto move_input_end = std::prev( move_output_end, n );
      const auto create_input_begin = std::prev( create_output_begin, n );
      const auto create_input_end = std::prev( create_output_end, n );
      if( create_output_begin < create_output_end ) {
        auto iiter = create_input_begin;
        const auto iend = create_input_end;
        auto oiter = create_output_begin;
        for( ; iiter != iend; ++oiter, ++iiter )
          new ( oiter ) value_type( std::move( *iiter ) );
      }
      if( move_input_begin < move_input_end ) {
        
        std::move_backward(
          move_input_begin,
          move_input_end,
          move_output_end
        );
      }
    }
    template< typename value_type_ = value_type >
    void swap_(
      resizable_array< T, max_size_ > &other,
      typename boost::enable_if< boost::is_pod< value_type_ > >::type* = 0
    ) {
      size_t index = 0u;
      for( ; index != std::min( size_, other.size_ ); ++index )
        std::swap( (*this)[ index ], other[ index ] );
      if( size_ > other.size_ )
        std::move( std::next( begin(), index ), end(), other.end() );
      else if( size_ < other.size_ )
        std::move( std::next( other.begin(), index ), other.end(), end() );
      std::swap( size_, other.size_ );
    }
    template< typename value_type_ = value_type >
    void swap_(
      resizable_array< T, max_size_ > &other,
      typename boost::disable_if< boost::is_pod< value_type_ > >::type* = 0
    ) {
      size_t index = 0u;
      for( ; index != std::min( size_, other.size_ ); ++index )
        std::swap( (*this)[ index ], other[ index ] );
      if( size_ > other.size_ ) {
        for( ; index != size_; ++index ) {
          new ( &other[ index ] ) value_type( std::move( (*this)[ index ] ) );
          (*this)[ index ].~value_type();
        }
      }
      else if( size_ < other.size_ ) {
        for( ; index != other.size_; ++index ) {
          new ( &(*this)[ index ] ) value_type( std::move( other[ index ] ) );
          other[ index ].~value_type();
        }
      }
      std::swap( size_, other.size_ );
    }
    char base[ sizeof( T ) * max_size_ ];
    size_type size_;
  };
}

#endif

